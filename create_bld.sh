#!/bin/sh
GENERATOR=Ninja
rm -r ./bld
mkdir -p bld/Debug && cd bld/Debug && cmake -G $GENERATOR -DCMAKE_BUILD_TYPE=Debug ../.. && \
    cd ../.. && \
    mkdir -p bld/Release && cd bld/Release && cmake -G $GENERATOR -DCMAKE_BUILD_TYPE=Release ../.. && \
    cd ../.. && \
    mkdir -p bld/Final && cd bld/Final && cmake -G $GENERATOR -DCMAKE_BUILD_TYPE=Final ../..
