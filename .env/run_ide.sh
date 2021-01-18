#! /bin/bash

DIR=$(dirname "$0")
APP=$1

source $DIR/dockerhub.env

$DIR/run.sh $TAG_IDE $CONTAINER_IDE $APP

