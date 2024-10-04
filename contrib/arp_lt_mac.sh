#!/bin/bash


fl=""

while getopts cx flag
do
    case "${flag}" in
        c) fl=c;;
        x) fl=x;;
    esac
done

if [[ $fl == "" ]];then
    exit
fi

sifs=${IFS}
IFS=$'\n'

vrfs=$(echo "select v.name  from vrf v where v.name LIKE 'icdn%';" |psql -t  -Uvc -h localhost vc)
get_asbr=$(echo "select n.ip
                 from node n join vrf_nodes vn on n.id = vn.node_id join vrf v on v.id = vn.vrf_id 
                 where n.is_asbr = 1;" |psql -t  -Uvc -h localhost vc)

declare -A asbr_lt
declare -A asbr_lt_mac
declare -A asbr_lt_ip
cidx=0
for asbr in ${get_asbr};do
    asbr=$(echo ${asbr} | awk -F'|' '{print $1}' |  tr -d ' ')
    lt=$(echo "select v.ltint 
               from node n join vrf_nodes vn on n.id = vn.node_id join vrf v on v.id = vn.vrf_id 
               where n.is_asbr = 1 and n.ip = '${asbr}' limit 1;" |psql -t  -Uvc -h localhost vc)
    lt=$(echo ${lt} | awk -F'|' '{print $1}' |  tr -d ' ')
    lt=${lt%.*}
    asbr_lt[${asbr}]=${lt}
 ((cidx++))
done

IFS=${sifs}


sifs=${IFS}
IFS=$'\n'
cidx=0
 for asbr in ${get_asbr};do
 for vrf in ${vrfs};do
    asbr=$(echo ${asbr} | awk -F'|' '{print $1}' |  tr -d ' ')
    vrf=$(echo ${vrf} | awk -F'|' '{print $1}' |  tr -d ' ')
    net=$(echo "select n.ip, n.pfr_vplsip, v.net 
                from vrf v join vrf_nodes vn on v.id = vn.vrf_id join node n on n.id  = vn.node_id
                where n.is_asbr = 1 and v.name LIKE '${vrf}' and n.ip = '${asbr}';" |psql -t  -Uvc -h localhost vc)
    asbr_ip=$(echo ${net} | awk -F'|' '{print $1}' |  tr -d ' ')
    four_oct_ip=$(echo ${net} | awk -F'|' '{print $2}' |  tr -d ' ')
    lt_net=$(echo ${net} | awk -F'|' '{print $3}' |  tr -d ' ')
    lt_ip=${lt_net%.*}.${four_oct_ip}
    if [[ ${asbr_lt_ip[${asbr_ip}]} == "" ]];then
     asbr_lt_ip[${asbr_ip}]="${lt_ip}"
    elif [[ "${asbr_lt_ip[${asbr_ip}]}" != *"${lt_ip}"* ]];then
     lt_ip_t=${asbr_lt_ip[${asbr_ip}]}
     asbr_lt_ip[${asbr_ip}]="${lt_ip_t} ${lt_ip}"
    fi
 ((cidx++))
 done
 done
IFS=${sifs}

if [[ $fl == "x" ]];then
 for node in ${!asbr_lt_ip[@]};do
  for host0 in ${asbr_lt_ip[${node}]};do
     arp -d $host0
  done
 done
 exit
fi

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
if [[ $fl == "c" ]];then
for node in ${!asbr_lt[@]};do
    mac_lt=$(./netconf5 ${node} -g "<rpc> \
                                <get-interface-information> \
                                <statistics/> \
                                <detail/> \
                                <interface-name>${asbr_lt[${node}]}</interface-name> \
                                </get-interface-information> \
                                </rpc>"|grep -i mac|head -n1|awk '{print $3}'|tr -d '">')
    echo "${node}:${asbr_lt[${node}]}:${mac_lt}"
    asbr_lt_mac[${node}]=${mac_lt}
done

for node in ${!asbr_lt_ip[@]};do
  echo "${node}:${asbr_lt_ip[${node}]}:${asbr_lt_mac[${node}]}"
  for host0 in ${asbr_lt_ip[${node}]};do
     arp -s $host0 ${asbr_lt_mac[${node}]}
  done
done
fi
