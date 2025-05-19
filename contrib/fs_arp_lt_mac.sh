#!/bin/bash


fl=""

while getopts cxv flag
do
    case "${flag}" in
        c) fl=c;;
        x) fl=x;;
        v) flv=v;;
    esac
done

if [[ $fl == "" ]];then
    exit
fi

sifs=${IFS}
IFS=$'\n'

declare -A node_lt
declare -A node_lt_mac
declare -A node_lt_ip

    node_ip_lt=$(echo "select n.ip, ip.address, i.name, si.unit from sub_interfaces si 
                       join interface i on i.id = si.parent_id 
                       join node n on n.id = i.node_id 
                       join ipam_addresses ip on ip.interface_id = si.id where si.role_id = 4;" |psql -t  -Uvc -h localhost vc)
    for e in ${node_ip_lt};do
        node_ip=$(echo ${e} | awk -F'|' '{print $1}' |  tr -d ' ')
        ip=$(echo ${e} | awk -F'|' '{print $2}' |  tr -d ' ')
        lt=$(echo ${e} | awk -F'|' '{print $3}' |  tr -d ' ')
        unit=$(echo ${e} | awk -F'|' '{print $4}' |  tr -d ' ')
        node_lt[${node_ip}]="${lt}"."${unit}"
        node_lt_ip[${node_ip}]="${ip%/*}"
    done

IFS=${sifs}


if [[ $fl == "x" ]];then
 for node in ${!node_lt[@]};do
    if [[ $flv == "v" ]];then
        echo "arp -d ${node_lt_ip[${node}]}"
    fi
    arp -d ${node_lt_ip[${node}]}
 done
 exit
fi

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
if [[ $fl == "c" ]];then
for node in ${!node_lt[@]};do
    mac_lt=$(./netconf5 ${node} -g "<rpc> \
                                <get-interface-information> \
                                <statistics/> \
                                <detail/> \
                                <interface-name>${node_lt[${node}]}</interface-name> \
                                </get-interface-information> \
                                </rpc>"|grep -i mac|head -n1|awk '{print $3}'|tr -d '">')
    node_lt_mac[${node}]=${mac_lt}
done

for node in ${!node_lt[@]};do
    if [[ $flv == "v" ]];then
        echo "arp -s ${node_lt_ip[${node}]} ${node_lt_mac[${node}]}"
    fi
     arp -s ${node_lt_ip[${node}]} ${node_lt_mac[${node}]}
: <<'END_COMMENT'
END_COMMENT
done
fi
