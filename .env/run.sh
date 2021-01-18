#! /bin/bash

GIT_EMAIL=$(git config --global user.email)
GIT_USER=$(git config --global user.name)

if [ -z "$GIT_EMAIL" ] || [ -z "$GIT_USER" ]; then
    echo "Configure GIT before running containers:"
    echo ""
    echo "  git config --global user.email \"you@example.com\""
    echo "  git config --global user.name \"Your Name\""
    echo ""
    exit 1;
fi

TAG=$1
CONTAINER=$2
APP=$3

DIR=$(dirname "$0")

docker run --rm \
    --cap-add SYS_ADMIN \
    --privileged \
    -h ${TAG} \
    -e DISPLAY \
    -e CONTAINER=${CONTAINER} \
    -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
    -v $DIR/../:$DIR/../ \
    -v /var/run/docker.sock:/var/run/docker.sock:rw \
    -v $HOME/.ssh:$HOME/.ssh \
    -v $HOME/.gitconfig:$HOME/.gitconfig \
    -v $DIR/home/config/JetBrains:/home/$(whoami)/.config/JetBrains \
    -v $DIR/home/config/QtProject:/home/$(whoami)/.config/QtProject \
    -v $DIR/home/local/share/JetBrains:/home/$(whoami)/.local/share/JetBrains \
    -v $DIR/home/java:/home/$(whoami)/.java \
    -v $DIR/home/cache/JetBrains:/home/$(whoami)/.cache/JetBrains \
    -v $DIR/home/cache/mozilla:/home/$(whoami)/.cache/mozilla \
    -v $DIR/home/mozilla:/home/$(whoami)/.mozilla \
    --net=host \
    -w $DIR/../ \
    -it ${CONTAINER} ${APP}

