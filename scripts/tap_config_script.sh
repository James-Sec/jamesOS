#!/bin/sh
#modprobe tun tap                  # unnecessary if tun/tap is built-in
tunctl -t tap0
#ip tuntap add dev tap0 mode tap
ip link set tap0 up
