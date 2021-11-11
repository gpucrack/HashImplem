FROM alpine:latest
RUN apk --no-cache add make gcc g++ musl-dev binutils autoconf automake libtool pkgconfig check-dev file patch openssl openssl-libs-static openssl-dev

RUN mkdir /app
WORKDIR /app
