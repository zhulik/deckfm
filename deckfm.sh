#!/usr/bin/bash

set -eu

QSG_INFO=1 QSG_RENDER_LOOP=basic QT_QPA_PLATFORM=wayland LD_LIBRARY_PATH=$(pwd) ./build/deckfm
