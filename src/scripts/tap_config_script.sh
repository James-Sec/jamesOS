#!/bin/sh
#modprobe tun tap                  # unnecessary if tun/tap is built-in
#tunctl -t tap0
ip tuntap add dev tap0 mode tap
ip link set tap0 up
ip link set dev tap0 address 46:d7:93:5c:13:e8
ip address add dev tap0 30.30.30.30
ip route add 1.2.3.4 dev tap0
