#!/usr/bin/make

KCFLAGS = -Wall -nostdinc -I/usr/include -I/usr/src/linux-headers-3.2.0-3-rpi/include -I/usr/src/linux-headers-3.2.0-3-common/include/

all: build/memtest build/simple_module.ko

build/memtest: src/memtest.c
	mkdir -p $(shell dirname $@)
	gcc -O -o $@ $<

build/simple_module.ko: src/simple_module.c
	mkdir -p $(shell dirname $@)
	gcc -c $(KCFLAGS) -o $@ $<


