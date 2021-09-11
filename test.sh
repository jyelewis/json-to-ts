#!/bin/sh

./build.sh
cd ./tests
yarn install
yarn test

