FROM ubuntu:latest

RUN apt update && apt install -y cmake git g++ autoconf

RUN git clone https://github.com/fastflow/fastflow.git && mv fastflow/ff /usr/local/include/

RUN git clone https://github.com/jemalloc/jemalloc.git && cd jemalloc && ./autogen.sh && make && make install

WORKDIR /app

COPY . .

RUN cmake . && make
