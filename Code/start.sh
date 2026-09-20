#!/usr/bin/env bash
set -e

cmake --build build-windows
./build-windows/BinomialModel.exe