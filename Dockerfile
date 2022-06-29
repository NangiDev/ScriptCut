FROM ubuntu:20.04
# Needed to be set for the cmake package to be installed properly without interactive input
# Set gcc/g++ so they default to the installed versions
# zip / pkg-config were required for vcpkg to build some dependencies
ENV DEBIAN_FRONTEND=noninteractive

RUN apt update -y
RUN apt install -y cmake make git curl zip gcc g++ mesa-common-dev xorg-dev

ENV DISPLAY=host.docker.internal:0.0

WORKDIR /app
RUN mkdir vcpkg

COPY .git .git
COPY .gitmodules .gitmodules
COPY src src
COPY CMakeLists.txt CMakeLists.txt

WORKDIR /app/build
RUN cmake .. && make

WORKDIR /app/runtime
CMD ./ScriptCut