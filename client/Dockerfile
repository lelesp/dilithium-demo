FROM archlinux:latest

ENV DEBIAN_FRONTEND=noninteractive
RUN pacman -Sy --noconfirm base-devel cmake ninja qt6-base qt6-network-auth \
    python python-pip git

WORKDIR /build

RUN git clone --depth 1 https://github.com/open-quantum-safe/liboqs.git && \
    cd liboqs && \
    mkdir build && cd build && \
    cmake -GNinja -DCMAKE_INSTALL_PREFIX=/usr .. && \
    ninja && ninja install && \
    cd /build && rm -rf liboqs

COPY . /build/DilithiumClient
WORKDIR /build/DilithiumClient

RUN mkdir build && cd build && \
    cmake -GNinja .. && \
    ninja

RUN cd /build/DilithiumClient/build && ctest

ENTRYPOINT ["/build/DilithiumClient/build/DilithiumClient"]
