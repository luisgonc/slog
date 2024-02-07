# ubunutu is the base image

FROM ubuntu:20.04

# this is for timezone config
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Lisbon
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update
#-y is for accepting yes when the system asked us for installing the package
RUN apt-get install -y build-essential cmake git openssh-server gdb pkg-config valgrind systemd-coredump

# 1) google test
RUN echo "************************ google test ************************"
RUN git clone https://github.com/google/googletest.git -b v1.14.0
RUN mkdir -p  googletest/build && cd googletest/build
WORKDIR "googletest/build"
#RUN cmake -DCMAKE_CXX_FLAGS=-std=c++1z   -DGOOGLETEST_PATH=../../googletest   -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON ../ && cmake --build .  --parallel && cmake --install .
RUN cmake .. && cmake --build .  --parallel && cmake --install .
WORKDIR "/"
RUN rm -rf googletest

