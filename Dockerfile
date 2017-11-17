FROM ubuntu:16.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update -qq && apt-get install -yq --no-install-recommends  \
    	apt-utils bzip2 ca-certificates curl locales unzip \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN apt-get update -qq && apt-get install -yq --no-install-recommends \
      build-essential \
      cmake \
      git \
      less \
      libblitz0-dev \
      libboost-all-dev \
      libthrust-dev \
      python \
      python-dev \
      python-numpy \
      python-scipy \
      python-pip \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* \
    && pip install -U --no-cache-dir pip setuptools \
    && pip install --no-cache-dir matplotlib pytest

WORKDIR /usr/local/src/libcloudphxx
COPY . .
RUN mkdir build \
    && cd build \
    && cmake .. \
    && make \
    && cd tests \
    && make test \
    && cd .. \
    && make install \
    && cd .. \
    && rm -rf build

RUN useradd --no-user-group --create-home --shell /bin/bash clouds
USER clouds
WORKDIR /home/clouds
