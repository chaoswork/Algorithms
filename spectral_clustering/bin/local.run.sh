#!/bin/sh

SPARK_BIN="/home/work/local/spark-1.6/bin/spark-submit"

INPUT="/app/ecom/aries/fengkong/huangchao07/spark/demo/input" 
OUTPUT="/app/ecom/aries/fengkong/huangchao07/local/sc.test/" 
hadoop fs -rmr $OUTPUT
hadoop fs -rmr /app/ecom/aries/fengkong/huangchao07/spark-debug/
sh ${SPARK_BIN} \
    --master local[4] \
    --driver-memory 8g \
    --class com.baidu.risk.SpectralClustering \
    target/spark-spectralcluster-1.0-SNAPSHOT.jar \
    $INPUT \
    $OUTPUT \
    2
