ARG ROS_DISTRO=noetic

FROM osrf/ros:${ROS_DISTRO}-desktop-full

ARG DEBIAN_FRONTEND=noninteractive
ARG UID=1000
ARG DOCKERUSER=
ARG DOCKERUSERCOMMENT=
ARG WEBOTS_VERSION=2023b

RUN id -u ubuntu &>/dev/null && userdel -r ubuntu || echo "User ubuntu does not exist" && \
    useradd -d /${DOCKERUSER} -m \
            -u ${UID} -U \
            -s /usr/bin/bash \
            -c "${DOCKERUSERCOMMENT}" ${DOCKERUSER} && \
    echo "${DOCKERUSER} ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers && \
    usermod -a -G video ${DOCKERUSER} && \
    usermod -a -G dialout ${DOCKERUSER}

RUN apt update && apt install -y wget && \
    wget https://github.com/cyberbotics/webots/releases/download/R${WEBOTS_VERSION}/webots_${WEBOTS_VERSION}_amd64.deb -O /tmp/webots.deb && \
    apt install -y /tmp/webots.deb && \
    rm /tmp/webots.deb

# Add other ROS dependencies here
RUN apt-get update && apt-get install -y \
    ros-${ROS_DISTRO}-moveit \
    ros-${ROS_DISTRO}-gmapping \
    ros-${ROS_DISTRO}-teleop-twist-keyboard

COPY ./docker/bashrc /tmp/bashrc
COPY ./docker/webots.conf /${DOCKERUSER}/.config/Cyberbotics/Webots-R${WEBOTS_VERSION}.conf
RUN cat /tmp/bashrc >> /${DOCKERUSER}/.bashrc

ENV USERNAME=default
USER ${DOCKERUSER}
RUN mkdir -p /${DOCKERUSER}/ros_ws/src
WORKDIR /${DOCKERUSER}/ros_ws
