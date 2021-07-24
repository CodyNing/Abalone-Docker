# Adapted from https://github.com/jasujm/apparatus-examples/tree/master/docker-cpp/Dockerfile-v3

# The new base image to contain runtime dependencies

FROM debian:buster AS base

RUN set -ex;         \
    apt-get update;  \
    apt-get install -y libzmq5

# The first stage will install build dependencies on top of the
# runtime dependencies, and then compile

FROM base AS builder

RUN set -ex;                                                                      \
    apt-get install -y g++ curl cmake libzmq3-dev;                                \
    mkdir -p /usr/src;                                                            \
    cd /usr/src;                                                                  \
    curl -L https://github.com/zeromq/cppzmq/archive/v4.6.0.tar.gz | tar -zxf -;  \
    cd /usr/src/cppzmq-4.6.0;                                                     \
    cmake -D CPPZMQ_BUILD_TESTS:BOOL=OFF .; make; make install

COPY . /usr/src/GameAgent

RUN set -ex;              \
    cd /usr/src/GameAgent;  \
    cmake .; make; make install

# The second stage will already contain all dependencies, just copy
# the compiled executables

FROM base AS runtime

COPY --from=builder /usr/local/bin /usr/local/bin

ENTRYPOINT ["GameAgent"]
EXPOSE 5555
