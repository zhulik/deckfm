#!/usr/bin/bash

set -eu

export QT_SCALE_FACTOR=3

export LD_LIBRARY_PATH=$(pwd)
./build/deckfm
