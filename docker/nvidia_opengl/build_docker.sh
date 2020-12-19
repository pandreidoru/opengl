#!/bin/bash
set -e

function usage() {
    echo "usage: $0 [-i <IMAGE_NAME> [-t <TAG>]] | [-h]"
    echo "-i | --image  Docker image name"
    echo "-t | --tag    Docker tag (default $TAG)"
    echo "-h | --help   This message"
}

IMAGE_NAME="nvidia_opengl"
IMAGE_TAG="1.2-glvnd-devel-ubuntu20.04"

# Iterate through command line inputs.
while [ "$1" != "" ]; do
    case $1 in
    -i | --image)
        shift
        IMAGE_NAME=$1
        ;;
    -t | --tag)
        shift
        IMAGE_TAG=$1
        ;;
    -h | --help)
        usage
        exit
        ;;
    *)
        usage
        exit 1
        ;;
    esac
    shift
done

# Use new features.
export DOCKER_BUILDKIT=1

docker build --progress=plain -t pandreidoru/"${IMAGE_NAME}":"${IMAGE_TAG}" .