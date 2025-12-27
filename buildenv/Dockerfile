FROM randomdude/gcc-cross-x86_64-elf

RUN apt-get update 
RUN apt-get upgrade -y
RUN apt-get install -y nasm
RUN apt-get install -y xorriso
RUN apt-get install -y grub-pc-bin
RUN apt-get install -y grub-common

VOLUME /root/env
WORKDIR /root/env
# build the container:
# bash: docker build -t mykernel buildenv
# build the kernel inside the container:
# bash: docker run --rm -it -v ${PWD}:/root/env mykernel
# docker: (~env/#)  make build-x86_64
# finally run the iso in qemu:
# bash: qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso