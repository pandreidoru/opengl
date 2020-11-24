# syntax=docker/dockerfile:experimental
FROM ubuntu:18.04

# Configure Timezone
ENV TZ=Europe
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Set args/environment variables.
ENV WORKSPACE=/workspace
WORKDIR $WORKSPACE

# Make sure that there is no interactive steps.
ARG DEBIAN_FRONTEND=noninteractive

# Dependencies for glvnd and X11.
RUN apt-get update \
  && apt-get install --assume-yes -qq --no-install-recommends \
    libglvnd0 \
    libgl1 \
    libglx0 \
    libegl1 \
    libxext6 \
    libx11-6 \
  && rm -rf /var/lib/apt/lists/*

# Env vars for the nvidia-container-runtime.
ENV NVIDIA_VISIBLE_DEVICES all
ENV NVIDIA_DRIVER_CAPABILITIES graphics,utility,compute