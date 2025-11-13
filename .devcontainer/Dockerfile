FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
    build-essential \
    openmpi-bin \
    libopenmpi-dev \
    gdb \
    make \
    git \
    vim \
    && apt clean

# Criar usuário normal
RUN useradd -m devuser
USER devuser
WORKDIR /workspace
