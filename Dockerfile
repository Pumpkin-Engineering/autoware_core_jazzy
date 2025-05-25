# syntax=docker/dockerfile:1.7-labs@sha256:2807c8826dfcb4a4254c8c98e4fa723ee3f572e78a64ecd16d0de3d32e2fe2f9
ARG ROS_DISTRO=jazzy

FROM ros:${ROS_DISTRO} AS core-jazzy
SHELL ["/bin/bash", "-c"]
ARG DEBIAN_FRONTEND=noninteractive
ARG UID=1000
ARG GID=1000
ARG ROS_DOMAIN_ID=${ROS_DOMAIN_ID}
ENV ROS_DISTRO=${ROS_DISTRO}
ENV ROS_DOMAIN_ID=${ROS_DOMAIN_ID}
RUN rm -f /etc/apt/apt.conf.d/docker-clean # needed in ubuntu to persist apt download cache
RUN --mount=type=cache,sharing=locked,mode=0755,target=/var/cache/apt \
     apt-get update && rosdep update && \
     apt install --yes --no-install-recommends auto-apt-proxy

WORKDIR /autoware