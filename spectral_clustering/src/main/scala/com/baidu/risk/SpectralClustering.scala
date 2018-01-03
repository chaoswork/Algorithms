package com.baidu.risk

import org.apache.spark.rdd._
import org.apache.spark.mllib.linalg.{SparseVector,Vector, Vectors, DenseVector,Matrix, DenseMatrix}
import org.apache.hadoop.io.Text
import org.apache.hadoop.io.LongWritable
import org.apache.spark._
import org.apache.spark.SparkContext._
import org.apache.spark.mllib.linalg.distributed.{RowMatrix,CoordinateMatrix, MatrixEntry}
import org.apache.spark.mllib.linalg.SingularValueDecomposition
import org.apache.spark.mllib.clustering.{KMeans,KMeansModel}
import scala.math.sqrt

// 1. given a matrix A.
// 2. D=degree of A
// 3. Lsym=I+D^(-1/2)AD^(-1/2)
// 4. svd on Lsym, find the eigenvector w
// 5. Lrw's eignvector u which santisfy w=D^(1/2)u
// 5. kmeans of topk largest eigenvector

object SpectralClustering {

    def main(args: Array[String]) {
        if (args.length < 3) {
            println("usage: spark-submit program <input> <output> k")
            System.exit(1)
        }
        val conf = new SparkConf()
        conf.setAppName("SpectralClustering-" + System.getenv("USER"))
        val sc = new SparkContext(conf)
        val input = args(0)

        val data1 = sc.textFile(input).repartition(100).cache()
        val data2 = data1.map(_.split("\t")).map(v=>"%s\t%s\t%s".format(v(1),v(0),v(2)))
        val node = data1.union(data2).distinct.map(
            line=>{
                val v = line.split("\t")
                (v(0), v(1), v(2).toDouble)
            }).cache()
      val uniq_id = node.flatMap(v=>List(v._1,v._2)).distinct.zipWithIndex.cache()// maybe if not cache, id changed?
      println("length=%d".format(uniq_id.collect.length))
        val data = node.map(v=>v._1->(v._2,v._3)) // (ID1,(ID2,w))
            .join(uniq_id) // (ID1, ((ID2,w), id1))
            .map(v=>v._2._1._1->(v._2._2,v._2._1._2)) // (ID2, (id1, w))
            .join(uniq_id) // (ID2, ((id1,w), id2))
            .map(v=>(v._2._1._1.toLong,v._2._2.toLong,v._2._1._2.toDouble))


      val D = data.map(v=>(v._1.toLong,v._3.toDouble)).reduceByKey(_+_)
      val I = D.map(v=>(v._1,v._1,1.0))
      val half_inv_D = D.map(v=>(v._1,1.0/sqrt(v._2)))

        // B=D*A*D ,if D is diag.then B[i,j]=A[i,j]*v[i]*v[j]
      val DAD = data.map(v=>(v._1.toLong,(v._2.toLong,v._3.toDouble))).join(half_inv_D).map{v=>(v._2._1._1,(v._1,v._2._1._2*v._2._2))}.join(half_inv_D).map{v=>(v._2._1._1,v._1,v._2._1._2*v._2._2)}

        // create RowMatrix https://gist.github.com/vrilleup/9e0613175fab101ac7cd
      val inputData = DAD.union(I).map(v=>(v._1.toLong,v._2.toLong,v._3.toDouble))
        // Number of columns
         val nCol = inputData.map(_._2).distinct().count().toInt

        // Construct rows of the RowMatrix
        val dataRows = inputData.groupBy(_._1).map[(Long, Vector)]{ row =>
          //  val (indices, values) = row._2.toList.sortBy(_._2).map(e => (e._2, e._3)).unzip
          val (indices, values) = row._2.toList.sortBy(_._2).map(e => (e._2.toInt, e._3)).unzip
          (row._1, new SparseVector(nCol, indices.toArray, values.toArray))
        }

        // Compute 20 largest singular values and corresponding singular vectors
        //val svd = new RowMatrix(dataRows.map(_._2).persist()).computeSVD(20, computeU = true)
        val Lsym = new RowMatrix(dataRows.map(_._2).persist())
      //val coo_matrix_matrixEntry = inputData.map(e => MatrixEntry(e._1, e._2, e._3))
      //println("coo_matrix_matrixEntry map done")
      //val coo_matrix = new CoordinateMatrix(coo_matrix_matrixEntry)
      //println("coo_matrix done")
      //val Lsym = coo_matrix.toRowMatrix
      println ("Lsym done")
      //Lsym.rows.collect.foreach(println)
      val k = args(2).toInt
      println("begin svd...")
      val svd: SingularValueDecomposition[RowMatrix, Matrix] = Lsym.computeSVD(k, computeU = false)
      println("svd done..")
         val U: RowMatrix = svd.U // The U factor is a RowMatrix.
        val s: Vector = svd.s // The singular values are stored in a local dense vector.
        val V: Matrix = svd.V // The V factor is a local dense matrix.
        // println(s.toString())
        println("V:")
	    //svd.V.toArray.grouped(svd.V.numRows).toList.transpose.foreach(println)
      println("s:")
      //      s.toArray.foreach(println)

         println("U:")
//         svd.U.rows.collect.foreach(println)
        // kmeans
        val half_inv_D_list = half_inv_D.collect.map(v=>v._2).toList
        val points = sc.parallelize(svd.V.toArray.grouped(svd.V.numRows).toList.transpose.map(v=>Vectors.dense(v.toArray)).zipWithIndex)

        val model = new KMeans() 
            .setK(k)
            .setSeed(0L)
            .setInitializationMode("random")
            .setMaxIterations(100)
            .setRuns(5)
            .run(points.map(v=>v._1))

        println("Cluster result:")
        //points.map(p => (p._2.toString()+"->",model.predict(p._1))).foreach(println)
        println("org->id")
        //uniq_id.map(v=> (v._1.toString()+"->",v._2.toString())).foreach(println)
        // points.map(p => ("%s\t%s".format(p._2.toString(),model.predict(p._1)))).saveAsTextFile(args(1))
        val res = points.map(p=> (p._2.toLong, model.predict(p._1))).join(uniq_id.map(v=>(v._2.toLong, v._1)))
            .map(v=>(v._2._2, v._2._1))
        res.map(a=>"%s\t%d".format(a._1, a._2)).saveAsTextFile(args(1))
        val wssse = model.computeCost(points.map(v=>v._1));
        println(s"Within Set Sum of Squared Errors = $wssse")

        // model.clusterCenters.foreach(println)



        System.exit(0)
    }
}
 
