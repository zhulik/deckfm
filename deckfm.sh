#!/usr/bin/bash

set -eu

LD_LIBRARY_PATH=$(pwd) gamescope -f ./build/deckfm
