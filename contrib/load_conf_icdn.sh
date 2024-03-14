#!/bin/bash

fl=""
exct=0

while getopts c:x:e flag
do
    case "${flag}" in
        c) vrfs_name=${OPTARG} fl=c;;
        x) vrfs_name=${OPTARG} fl=x;;
        e) exct=1;;
    esac
done


#echo ${exct} ${fl} ${vrfs_name}


#list name of vrf
#vrfs_name="icdn2 icdn3"

declare -a rd
declare -a rt
#get conf from db
cidx=0
for v in ${vrfs_name};do
 conf0=$(echo "select v0.id, v0.name, v0.rd, v0.rt_import, v0.rt_export, v0.ltint, v0.net, v0.vlan_id, v00.name, v00.rd, v00.rt_import, v00.rt_export, v000.name 
              from vrf v0 join vrf_vrf v1 on v0.id = v1.vrf_id0 
              join vrf v00 on v00.id  = v1.vrf_id1 
              join vrf v000 on v000.id = v1.vrf_id2 
              where v0.name='${v}';" |psql -t  -Uvc -h localhost vc)

 vrf_id=$(echo ${conf0} | awk -F'|' '{print $1}' |  tr -d ' ')
 rd[${cidx}]=$(echo ${conf0} | awk -F'|' '{print $3}' |  tr -d ' ')
 rt[${cidx}]=$(echo ${conf0} | awk -F'|' '{print $4}' |  tr -d ' ')
 vplsrd[${cidx}]=$(echo ${conf0} | awk -F'|' '{print $10}' |  tr -d ' ')
 vplsrt[${cidx}]=$(echo ${conf0} | awk -F'|' '{print $11}' |  tr -d ' ')
 intf[${cidx}]=$(echo ${conf0} | awk -F'|' '{print $6}' |  tr -d ' ')
 vlanid[${cidx}]=$(echo ${conf0} | awk -F'|' '{print $8}' |  tr -d ' ')
 nets[${cidx}]=$(echo ${conf0} | awk -F'|' '{print $7}' |  tr -d ' ')
 ((cidx++))
done

#route distingusher and route target arrays
declare -A vplsip

idx1=0
sifs=${IFS}
IFS=$'\n'
conf1=$(echo "select v.name, n.ip, n.is_asbr, n.pfr_vplsip 
              from vrf v join vrf_nodes vn on v.id = vn.vrf_id 
              join node n on n.id  = vn.node_id
              where v.id=${vrf_id};" |psql -t  -Uvc -h localhost vc)
for cc in ${conf1};do
 ipnode=$(echo ${cc} | awk -F'|' '{print $2}' |  tr -d ' ')
 ifasbr=$(echo ${cc} | awk -F'|' '{print $3}' |  tr -d ' ')
 ipvpls=$(echo ${cc} | awk -F'|' '{print $4}' |  tr -d ' ')
 pe[${idx1}]=${ipnode}
 vplsip[${ipnode}]=${ipvpls}
 if [ x${ifasbr} == x1 ];then
    asbr[${idx1}]=${ipnode}
 fi
 ((idx1++))
done
IFS=${sifs}

declare -A checkasbr
for node in ${asbr[*]};do
 checkasbr[${node}]=1
done



delete_vrfs () {
for node in ${pe[*]};do
if [[ ${checkasbr[${node}]} == 1 ]]; then
ifcfg=""
idxp=0
for((idx=0; idx < ${#intf[@]}; idx++)){
    intf0=${intf[${idx}]%.*}
    intf1=${intf[${idx}]#*.}
    intf11=$((intf1--))
    intf2=$((intf1-1))
    intf3=$((intf1-2))
    net=${nets[${idx}]%.*}
    ifcfg="${ifcfg}
    delete interfaces $intf0 unit $intf1
    delete interfaces $intf0 unit $intf11

    delete interfaces $intf0 unit $intf2 
    delete interfaces $intf0 unit $intf3
    "
}
if [ ${exct} == 1 ];then
 echo "Send config to ${node} ..."
 eval './nconf ${node} "
 ${ifcfg}
 "'
else
 eval 'echo ${node} "
 ${ifcfg}
 "'
fi

 ((idxp++))
fi
done

idx=0

for vrf_name in ${vrfs_name};do
idxp=0
for node in ${pe[*]};do
 asbrt=""
 if [[ ${checkasbr[${node}]} == 1 ]]; then
 asbrt="delete routing-instances pfr00_${vrf_name}_bgp
 delete routing-instances pfr01_${vrf_name}_bgp"
 ((idxp++))
 fi
if [ ${exct} == 1 ];then
 echo "Send config to ${node} ..."
 eval './nconf ${node} "
 delete routing-instances ${vrf_name}
 ${asbrt}"'
else
 eval 'echo ${node} "
 delete routing-instances ${vrf_name}
 ${asbrt}"'
fi
 done
 ((idx++))
done
}



create_vrfs () {
idx=0
idxp=0

#: '
for node in ${pe[*]};do
if [[ ${checkasbr[${node}]} == 1 ]]; then
ifcfg=""
for((idx=0; idx < ${#intf[@]}; idx++)){
    intf0=${intf[${idx}]%.*}
    intf1=${intf[${idx}]#*.}
    intf11=$((intf1--))
    intf2=$((intf1-1))
    intf3=$((intf1-2))
    net=${nets[${idx}]%.*}
    ifcfg="${ifcfg}
    set interfaces $intf0 unit $intf1 encapsulation vlan
    set interfaces $intf0 unit $intf1 vlan-id ${vlanid[$idx]}
    set interfaces $intf0 unit $intf1 peer-unit $intf11
    set interfaces $intf0 unit $intf1 family inet address ${net}.${vplsip[${node}]}/25 arp ${net}.100 mac 52:54:00:03:20:bd
    set interfaces $intf0 unit $intf11 encapsulation vlan-vpls
    set interfaces $intf0 unit $intf11 vlan-id ${vlanid[$idx]}
    set interfaces $intf0 unit $intf11 peer-unit $intf1

    set interfaces $intf0 unit $intf2 encapsulation vlan
    set interfaces $intf0 unit $intf2 vlan-id ${vlanid[$idx]}
    set interfaces $intf0 unit $intf2 peer-unit $intf3
    set interfaces $intf0 unit $intf2 family inet address ${net}.$(($idxp*2+129))/31
    set interfaces $intf0 unit $intf3 encapsulation vlan
    set interfaces $intf0 unit $intf3 vlan-id ${vlanid[$idx]}
    set interfaces $intf0 unit $intf3 peer-unit $intf2
    set interfaces $intf0 unit $intf3 family inet address ${net}.$(($idxp*2+128))/31
    "
}
if [ ${exct} == 1 ];then
 echo "Send config to ${node} ..."
 eval './nconf ${node} "
 ${ifcfg}
 "'
else
 eval 'echo ${node} "
 ${ifcfg}
 "'
fi
 ((idxp++))
fi
done
#'

idx=0

for vrf_name in ${vrfs_name};do
    intf0=${intf[${idx}]%.*}
    intf1=${intf[${idx}]#*.}
    ((intf1--))
    intf2=$((intf1-1))
    intf3=$((intf1-2))
    net=${nets[${idx}]%.*}

idxp=0
for node in ${pe[*]};do
 vrfint="set routing-instances ${vrf_name} instance-type vrf"
 asbrt=""
 if [[ ${checkasbr[${node}]} == 1 ]]; then
    vrfint="set routing-instances ${vrf_name} interface ${intf0}.${intf3}
 set routing-instances ${vrf_name} protocols bgp group PFR-BGP-PEER_pfr00_i local-address ${net}.$(($idxp*2+128))
 set routing-instances ${vrf_name} protocols bgp group PFR-BGP-PEER_pfr00_i export PL-PFR-EXPORT0
 set routing-instances ${vrf_name} protocols bgp group PFR-BGP-PEER_pfr00_i peer-as 3333
 set routing-instances ${vrf_name} protocols bgp group PFR-BGP-PEER_pfr00_i neighbor ${net}.$(($idxp*2+129)) family inet unicast"

 asbrt="set routing-instances pfr00_${vrf_name}_bgp instance-type vpls
 set routing-instances pfr00_${vrf_name}_bgp interface ${intf[${idx}]}
 set routing-instances pfr00_${vrf_name}_bgp route-distinguisher ${node}:${vplsrd[${idx}]}
 set routing-instances pfr00_${vrf_name}_bgp vrf-target target:3333:${vplsrt[${idx}]}
 set routing-instances pfr00_${vrf_name}_bgp protocols vpls site-range 256
 set routing-instances pfr00_${vrf_name}_bgp protocols vpls no-tunnel-services
 set routing-instances pfr00_${vrf_name}_bgp protocols vpls site s0 site-identifier ${vplsip[${node}]}
 set routing-instances pfr00_${vrf_name}_bgp protocols vpls site s0 interface ${intf[${idx}]}

 set routing-instances pfr01_${vrf_name}_bgp instance-type virtual-router
 set routing-instances pfr01_${vrf_name}_bgp interface ${intf0}.${intf1}
 set routing-instances pfr01_${vrf_name}_bgp interface ${intf0}.${intf2}
 set routing-instances pfr01_${vrf_name}_bgp protocols bgp group PFR-BGP-PEER local-address ${net}.${vplsip[${node}]}
 set routing-instances pfr01_${vrf_name}_bgp protocols bgp group PFR-BGP-PEER export PL-PFR-EXPORT0
 set routing-instances pfr01_${vrf_name}_bgp protocols bgp group PFR-BGP-PEER peer-as 3333
 set routing-instances pfr01_${vrf_name}_bgp protocols bgp group PFR-BGP-PEER neighbor ${net}.100 family inet unicast
 set routing-instances pfr01_${vrf_name}_bgp protocols bgp group PFR-BGP-PEER_i local-address ${net}.$(($idxp*2+129))
 set routing-instances pfr01_${vrf_name}_bgp protocols bgp group PFR-BGP-PEER_i export PL-BGP-FROM-PFR-00
 set routing-instances pfr01_${vrf_name}_bgp protocols bgp group PFR-BGP-PEER_i peer-as 3333
 set routing-instances pfr01_${vrf_name}_bgp protocols bgp group PFR-BGP-PEER_i neighbor ${net}.$(($idxp*2+128)) family inet unicast
 set routing-instances pfr01_${vrf_name}_bgp protocols bgp group PFR-BGP-PEER_i neighbor ${net}.$(($idxp*2+128)) cluster ${net}.$(($idxp*2+128))"
 ((idxp++))
 fi
if [ ${exct} == 1 ];then
 echo "Send config to ${node} ..."
 eval './nconf ${node} "
 set routing-instances ${vrf_name} description \"vrf ${vrf_name} for direct outgoing traffic to apropriate peer and fall back to i.inet.0\"
 set routing-instances ${vrf_name} instance-type vrf
 ${vrfint}
 set routing-instances ${vrf_name} route-distinguisher ${node}:${rd[${idx}]}
 set routing-instances ${vrf_name} vrf-target import target:3333:${rt[${idx}]}
 set routing-instances ${vrf_name} vrf-target export target:3333:${rt[${idx}]}
 set routing-instances ${vrf_name} vrf-table-label
 set routing-instances ${vrf_name} routing-options static route 0.0.0.0/0 next-table i.inet.0
 set routing-instances ${vrf_name} routing-options static route 0.0.0.0/0 no-readvertise
 set routing-instances ${vrf_name} routing-options auto-export

 ${asbrt}"'
 else
 eval 'echo ${node} "
 set routing-instances ${vrf_name} description \"vrf ${vrf_name} for direct outgoing traffic to apropriate peer and fall back to i.inet.0\"
 set routing-instances ${vrf_name} instance-type vrf
 ${vrfint}
 set routing-instances ${vrf_name} route-distinguisher ${node}:${rd[${idx}]}
 set routing-instances ${vrf_name} vrf-target import target:3333:${rt[${idx}]}
 set routing-instances ${vrf_name} vrf-target export target:3333:${rt[${idx}]}
 set routing-instances ${vrf_name} vrf-table-label
 set routing-instances ${vrf_name} routing-options static route 0.0.0.0/0 next-table i.inet.0
 set routing-instances ${vrf_name} routing-options static route 0.0.0.0/0 no-readvertise
 set routing-instances ${vrf_name} routing-options auto-export

 ${asbrt}"'
 fi
 done
 ((idx++))
done
}

if [ ${fl} == "x" ];then
 delete_vrfs
fi

if [ ${fl} == "c" ];then
 create_vrfs
fi
