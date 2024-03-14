#!/bin/bash
for i in 4;do d=$(echo "10.229.${i}.0"); eval './nconf ${d} "
set routing-instances pfr_4_38060_condadv description ri_for_coditional_advertisiment
set routing-instances pfr_4_38060_condadv instance-type vrf
set routing-instances pfr_4_38060_condadv route-distinguisher 10.229.4.0:38060
set routing-instances pfr_4_38060_condadv vrf-target target:3333:38060
set routing-instances pfr_4_38060_condadv vrf-table-label
set routing-instances pfr_4_38060_condadv routing-options auto-export
set routing-instances pfr_4_38060_condadv vrf-import PL-BGP-FROM-UPSTREAM-06-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-06-CONDADV term t0 from community CM-UPSTREAM-06-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-06-CONDADV term t0 from route-filter 3.3.3.3/32 exact
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-06-CONDADV term t0 then accept
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-06-CONDADV term t1000 then reject
set policy-options community CM-UPSTREAM-06-CONDADV members [ target:3333:1 3333:10106 ]
set policy-options community CM-PFR-06-CONDADV members [ 3333:38060 ]
set policy-options condition PL-BGP-FROM-UPSTREAM-06-CONDADV if-route-exists 3.3.3.3/32 table pfr_4_38060_condadv.inet.0
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 06-CONDADV from condition PL-BGP-FROM-UPSTREAM-06-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 06-CONDADV from community CM-PFR-06-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 06-CONDADV then accept
set routing-instances pfr_1_38020_condadv description ri_for_coditional_advertisiment
set routing-instances pfr_1_38020_condadv instance-type vrf
set routing-instances pfr_1_38020_condadv route-distinguisher 10.229.4.0:38020
set routing-instances pfr_1_38020_condadv vrf-target target:3333:38020
set routing-instances pfr_1_38020_condadv vrf-table-label
set routing-instances pfr_1_38020_condadv routing-options auto-export
set routing-instances pfr_1_38020_condadv vrf-import PL-BGP-FROM-UPSTREAM-02-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-02-CONDADV term t0 from community CM-UPSTREAM-02-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-02-CONDADV term t0 from route-filter 3.3.3.3/32 exact
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-02-CONDADV term t0 then accept
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-02-CONDADV term t1000 then reject
set policy-options community CM-UPSTREAM-02-CONDADV members [ target:3333:1 3333:10102 ]
set policy-options community CM-PFR-02-CONDADV members [ 3333:38020 ]
set policy-options condition PL-BGP-FROM-UPSTREAM-02-CONDADV if-route-exists 3.3.3.3/32 table pfr_1_38020_condadv.inet.0
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 02-CONDADV from condition PL-BGP-FROM-UPSTREAM-02-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 02-CONDADV from community CM-PFR-02-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 02-CONDADV then accept
set routing-instances pfr_3_38050_condadv description ri_for_coditional_advertisiment
set routing-instances pfr_3_38050_condadv instance-type vrf
set routing-instances pfr_3_38050_condadv route-distinguisher 10.229.4.0:38050
set routing-instances pfr_3_38050_condadv vrf-target target:3333:38050
set routing-instances pfr_3_38050_condadv vrf-table-label
set routing-instances pfr_3_38050_condadv routing-options auto-export
set routing-instances pfr_3_38050_condadv vrf-import PL-BGP-FROM-UPSTREAM-05-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-05-CONDADV term t0 from community CM-UPSTREAM-05-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-05-CONDADV term t0 from route-filter 3.3.3.3/32 exact
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-05-CONDADV term t0 then accept
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-05-CONDADV term t1000 then reject
set policy-options community CM-UPSTREAM-05-CONDADV members [ target:3333:1 3333:10105 ]
set policy-options community CM-PFR-05-CONDADV members [ 3333:38050 ]
set policy-options condition PL-BGP-FROM-UPSTREAM-05-CONDADV if-route-exists 3.3.3.3/32 table pfr_3_38050_condadv.inet.0
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 05-CONDADV from condition PL-BGP-FROM-UPSTREAM-05-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 05-CONDADV from community CM-PFR-05-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 05-CONDADV then accept
set routing-instances pfr_6_38080_condadv description ri_for_coditional_advertisiment
set routing-instances pfr_6_38080_condadv instance-type vrf
set routing-instances pfr_6_38080_condadv route-distinguisher 10.229.132.0:38080
set routing-instances pfr_6_38080_condadv vrf-target target:3333:38080
set routing-instances pfr_6_38080_condadv vrf-table-label
set routing-instances pfr_6_38080_condadv routing-options auto-export
set routing-instances pfr_6_38080_condadv vrf-import PL-BGP-FROM-UPSTREAM-08-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-08-CONDADV term t0 from community CM-UPSTREAM-08-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-08-CONDADV term t0 from route-filter 3.3.3.3/32 exact
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-08-CONDADV term t0 then accept
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-08-CONDADV term t1000 then reject
set policy-options community CM-UPSTREAM-08-CONDADV members [ target:3333:1 3333:10108 ]
set policy-options community CM-PFR-08-CONDADV members [ 3333:38080 ]
set policy-options condition PL-BGP-FROM-UPSTREAM-08-CONDADV if-route-exists 3.3.3.3/32 table pfr_6_38080_condadv.inet.0
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 08-CONDADV from condition PL-BGP-FROM-UPSTREAM-08-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 08-CONDADV from community CM-PFR-08-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 08-CONDADV then accept
set routing-instances pfr_2_38040_condadv description ri_for_coditional_advertisiment
set routing-instances pfr_2_38040_condadv instance-type vrf
set routing-instances pfr_2_38040_condadv route-distinguisher 10.229.132.0:38040
set routing-instances pfr_2_38040_condadv vrf-target target:3333:38040
set routing-instances pfr_2_38040_condadv vrf-table-label
set routing-instances pfr_2_38040_condadv routing-options auto-export
set routing-instances pfr_2_38040_condadv vrf-import PL-BGP-FROM-UPSTREAM-04-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-04-CONDADV term t0 from community CM-UPSTREAM-04-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-04-CONDADV term t0 from route-filter 3.3.3.3/32 exact
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-04-CONDADV term t0 then accept
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-04-CONDADV term t1000 then reject
set policy-options community CM-UPSTREAM-04-CONDADV members [ target:3333:1 3333:10104 ]
set policy-options community CM-PFR-04-CONDADV members [ 3333:38040 ]
set policy-options condition PL-BGP-FROM-UPSTREAM-04-CONDADV if-route-exists 3.3.3.3/32 table pfr_2_38040_condadv.inet.0
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 04-CONDADV from condition PL-BGP-FROM-UPSTREAM-04-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 04-CONDADV from community CM-PFR-04-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 04-CONDADV then accept
set routing-instances pfr_5_38070_condadv description ri_for_coditional_advertisiment
set routing-instances pfr_5_38070_condadv instance-type vrf
set routing-instances pfr_5_38070_condadv route-distinguisher 10.229.132.0:38070
set routing-instances pfr_5_38070_condadv vrf-target target:3333:38070
set routing-instances pfr_5_38070_condadv vrf-table-label
set routing-instances pfr_5_38070_condadv routing-options auto-export
set routing-instances pfr_5_38070_condadv vrf-import PL-BGP-FROM-UPSTREAM-07-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-07-CONDADV term t0 from community CM-UPSTREAM-07-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-07-CONDADV term t0 from route-filter 3.3.3.3/32 exact
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-07-CONDADV term t0 then accept
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-07-CONDADV term t1000 then reject
set policy-options community CM-UPSTREAM-07-CONDADV members [ target:3333:1 3333:10107 ]
set policy-options community CM-PFR-07-CONDADV members [ 3333:38070 ]
set policy-options condition PL-BGP-FROM-UPSTREAM-07-CONDADV if-route-exists 3.3.3.3/32 table pfr_5_38070_condadv.inet.0
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 07-CONDADV from condition PL-BGP-FROM-UPSTREAM-07-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 07-CONDADV from community CM-PFR-07-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 07-CONDADV then accept
set routing-instances pfr_7_38090_condadv description ri_for_coditional_advertisiment
set routing-instances pfr_7_38090_condadv instance-type vrf
set routing-instances pfr_7_38090_condadv route-distinguisher 127.0.0.1:38090
set routing-instances pfr_7_38090_condadv vrf-target target:3333:38090
set routing-instances pfr_7_38090_condadv vrf-table-label
set routing-instances pfr_7_38090_condadv routing-options auto-export
set routing-instances pfr_7_38090_condadv vrf-import PL-BGP-FROM-UPSTREAM-09-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-09-CONDADV term t0 from community CM-UPSTREAM-09-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-09-CONDADV term t0 from route-filter 3.3.3.3/32 exact
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-09-CONDADV term t0 then accept
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-09-CONDADV term t1000 then reject
set policy-options community CM-UPSTREAM-09-CONDADV members [ target:3333:1 3333:10109 ]
set policy-options community CM-PFR-09-CONDADV members [ 3333:38090 ]
set policy-options condition PL-BGP-FROM-UPSTREAM-09-CONDADV if-route-exists 3.3.3.3/32 table pfr_7_38090_condadv.inet.0
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 09-CONDADV from condition PL-BGP-FROM-UPSTREAM-09-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 09-CONDADV from community CM-PFR-09-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 09-CONDADV then accept
set routing-instances pfr_8_38100_condadv description ri_for_coditional_advertisiment
set routing-instances pfr_8_38100_condadv instance-type vrf
set routing-instances pfr_8_38100_condadv route-distinguisher 192.168.122.19:38100
set routing-instances pfr_8_38100_condadv vrf-target target:3333:38100
set routing-instances pfr_8_38100_condadv vrf-table-label
set routing-instances pfr_8_38100_condadv routing-options auto-export
set routing-instances pfr_8_38100_condadv vrf-import PL-BGP-FROM-UPSTREAM-10-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-10-CONDADV term t0 from community CM-UPSTREAM-10-CONDADV
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-10-CONDADV term t0 from route-filter 3.3.3.3/32 exact
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-10-CONDADV term t0 then accept
set policy-options policy-statement PL-BGP-FROM-UPSTREAM-10-CONDADV term t1000 then reject
set policy-options community CM-UPSTREAM-10-CONDADV members [ target:3333:1 3333:10110 ]
set policy-options community CM-PFR-10-CONDADV members [ 3333:38100 ]
set policy-options condition PL-BGP-FROM-UPSTREAM-10-CONDADV if-route-exists 3.3.3.3/32 table pfr_8_38100_condadv.inet.0
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 10-CONDADV from condition PL-BGP-FROM-UPSTREAM-10-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 10-CONDADV from community CM-PFR-10-CONDADV
set policy-options policy-statement PL-BGP-FROM-PFR-00 term 10-CONDADV then accept
set policy-options policy-statement PL-BGP-FROM-PFR-00 term deny then reject"';done
