fetch-dependencies:
    mkdir -p ./external_deps
    vcs import --recursive ./external_deps < ./dependencies.yaml

devel-create:
    #!/bin/bash
    docker build ./ -t autoware-jazzy -f ./Dockerfile
    xhost + >/dev/null
    set -x
    docker create -it \
    --init \
    --privileged \
    --net=host \
    --name=autoware-jazzy \
    -e LOCAL_UID=$(id -u) -e LOCAL_GID=$(id -g) -e LOCAL_USER=$(id -un) -e LOCAL_GROUP=$(id -gn) \
    -e DISPLAY=${DISPLAY} -v /tmp/.X11-unix:/tmp/.X11-unix \
    -e XAUTHORITY=${XAUTHORITY} \
    -e XDG_RUNTIME_DIR=${XDG_RUNTIME_DIR} \
    -e ROS_DOMAIN_ID=${ROS_DOMAIN_ID} \
    -v ./autoware_map:/map \
    -v "$(pwd)":/autoware \
    -v /etc/localtime:/etc/localtime:ro \
    autoware-jazzy

devel-start:
    #!/bin/bash
    docker start autoware-jazzy 

devel-stop:
    #!/bin/bash
    docker stop -t 10 autoware-jazzy

devel-new-terminal:
    #!/bin/bash
    CONTAINER_ID="$(docker ps -aqf "name=^autoware-jazzy$")"
    docker exec -it --user $(id -un) $CONTAINER_ID bash