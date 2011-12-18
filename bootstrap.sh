#!/bin/bash

set -e -x

  DEV_DIR=$HOME/raspberry-pi
  export PATH=$DEV_DIR/bin:$DEV_DIR/arm-2011.03/bin:$PATH

  sudo apt-get install -y git-core zlib1g-dev libsdl1.2-dev

  mkdir -p $DEV_DIR
  cd $DEV_DIR
  mkdir -p $DEV_DIR/modules
  # wget 'https://sourcery.mentor.com/sgpp/lite/arm/portal/package8734/public/arm-none-eabi/arm-2011.03-42-arm-none-eabi-i686-pc-linux-gnu.tar.bz2'
  # tar -xvf arm-*-arm-none-eabi-i686-pc-linux-gnu.tar.bz2

  export PATH=$DEV_DIR/bin:$DEV_DIR/arm-2011.03/bin:$PATH
  cd $DEV_DIR
  git clone https://github.com/qemu/QEMU.git qemu
  cd $DEV_DIR/qemu
  ./configure --enable-debug --enable-debug-tcg --target-list="arm-softmmu" --prefix=$DEV_DIR --enable-sdl --disable-pie
  make -j 4
  fakeroot make install

  export 