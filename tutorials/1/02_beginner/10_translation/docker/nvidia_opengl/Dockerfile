# syntax=docker/dockerfile:experimental
FROM nvidia/opengl:1.2-glvnd-devel-ubuntu20.04

# Configure Timezone
ENV TZ=Europe
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Set args/environment variables.
ENV WORKSPACE=/workspace
WORKDIR $WORKSPACE

# Make sure that there is no interactive steps.
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install --no-install-recommends --assume-yes \
        # C++
        apt-utils \
        build-essential \
        cmake \
        g++ \
        gcc \
        gdb \
        googletest \
        libboost-all-dev \
        libgoogle-glog-dev \
        libncurses5-dev \
        # GLEW
        libgl-dev \
        libgl1-mesa-dev \
        libgl1-mesa-glx \
        libgles2-mesa-dev \
        libglu1-mesa-dev \
        libosmesa-dev \
        libxi-dev \
        libxmu-dev \
        mesa-utils \
        # GLFW
        libglfw3 \
        libglfw3-dev \
        xorg-dev \
        # Extra
        eog \
        git \
        kdiff3 \
        python \
        python3-pip \
        ssh \
        sudo \
        tar \
        tree \
        unzip \
        vim \
        valgrind \
        wget \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && ldconfig

# GLEW
RUN wget https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.tgz/download \
    && tar xzvf download \
    && cd glew-2.1.0 \
    && make -j4 install \
    && mv /usr/lib64/libGLEW* /usr/lib/x86_64-linux-gnu/ \
    && ldconfig \
    && cd ${WORKSPACE} && rm -rf ${WORKSPACE}/*

# GLFW
RUN git clone https://github.com/glfw/glfw.git \
    && cd glfw \
    && mkdir build \
    && cd build \
    && cmake -G "Unix Makefiles" -DBUILD_SHARED_LIBS=ON .. \
    && make -j4 install \
    && ldconfig \
    && cd ${WORKSPACE} && rm -rf ${WORKSPACE}/*

# GLM
RUN wget https://github.com/g-truc/glm/archive/0.9.9.8.tar.gz \
    && tar xzvf 0.9.9.8.tar.gz \
    && cd glm-0.9.9.8 \
    && mv glm /usr/include \
    && cd ${WORKSPACE} && rm -rf ${WORKSPACE}/*
