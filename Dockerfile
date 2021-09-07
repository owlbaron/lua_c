FROM gcc:latest

RUN apt-get update && apt-get install -y lua5.4 lua5.4-dev

COPY . /app
WORKDIR /app