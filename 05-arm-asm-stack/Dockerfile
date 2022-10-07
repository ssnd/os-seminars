FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive

WORKDIR /home

# install GDB + GCC for ARM
RUN apt-get update && apt-get install -y libc6-dev-arm64-cross make gdb-multiarch gcc-aarch64-linux-gnu qemu-user

# don't stop
ENTRYPOINT ["tail", "-f", "/dev/null"] 