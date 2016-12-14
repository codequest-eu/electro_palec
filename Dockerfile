FROM node:latest

RUN mkdir /glove
WORKDIR /glove
ADD glove /glove
RUN npm install
