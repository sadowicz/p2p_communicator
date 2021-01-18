#! /bin/bash

DIR=$(dirname "$0")

source $DIR/dockerhub.env

$DIR/run.sh $TAG_BASE $CONTAINER_BASE

