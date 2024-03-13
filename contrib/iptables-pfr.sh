#!/bin/bash

iptables -t mangle -I OUTPUT 1 -s 1.0.5.230 -p icmp --icmp-type 8 -m u32 --u32 "44&0xFFFFFFFF=0x1:0x1" -j MARK --set-mark 1
iptables -t mangle -I OUTPUT 1 -s 1.0.5.230 -p icmp --icmp-type 8 -m u32 --u32 "44&0xFFFFFFFF=0x2:0x2" -j MARK --set-mark 2
iptables -t mangle -I OUTPUT 1 -s 1.0.5.230 -p icmp --icmp-type 8 -m u32 --u32 "44&0xFFFFFFFF=0x3:0x3" -j MARK --set-mark 3
iptables -t mangle -I OUTPUT 1 -s 1.0.5.230 -p icmp --icmp-type 8 -m u32 --u32 "44&0xFFFFFFFF=0x4:0x4" -j MARK --set-mark 4
iptables -t mangle -I OUTPUT 1 -s 1.0.5.230 -p icmp --icmp-type 8 -m u32 --u32 "44&0xFFFFFFFF=0x5:0x5" -j MARK --set-mark 5
iptables -t mangle -I OUTPUT 1 -s 1.0.5.230 -p icmp --icmp-type 8 -m u32 --u32 "44&0xFFFFFFFF=0x6:0x6" -j MARK --set-mark 6
iptables -t mangle -I OUTPUT 1 -s 1.0.5.230 -p icmp --icmp-type 8 -m u32 --u32 "44&0xFFFFFFFF=0x7:0x7" -j MARK --set-mark 7
iptables -t mangle -I OUTPUT 1 -s 1.0.5.230 -p icmp --icmp-type 8 -m u32 --u32 "44&0xFFFFFFFF=0x8:0x8" -j MARK --set-mark 8
iptables -t mangle -I OUTPUT 1 -s 1.0.5.230 -p icmp --icmp-type 8 -m u32 --u32 "44&0xFFFFFFFF=0x9:0x9" -j MARK --set-mark 9

#echo "100   pfr_peer0" >> /etc/iproute2/rt_tables

ip rule add fwmark 0x1 table 100
ip rule add fwmark 0x2 table 100
ip rule add fwmark 0x3 table 100
ip rule add fwmark 0x4 table 100
ip rule add fwmark 0x5 table 100
ip rule add fwmark 0x6 table 100
ip rule add fwmark 0x7 table 100
ip rule add fwmark 0x8 table 100
ip rule add fwmark 0x9 table 100

ip route add table 100 default via 192.168.122.1
ip route add table 100 10.60.0.0/25 via 127.0.0.1
ip route add table 100 10.60.0.128/25 via 127.0.0.1
ip route add table 100 10.100.0.0/24 via 127.0.0.1
ip route add table 100 10.229.0.0/16 via 127.0.0.1
ip route add table 100 10.230.0.0/16 via 127.0.0.1
ip route add table 100 10.240.0.0/12 via 127.0.0.1
ip route add table 100 60.128.128.0/17 via 127.0.0.1
ip route add table 100 61.128.0.0/15 via 127.0.0.1
ip route add table 100 200.200.200.0/24 via 127.0.0.1
