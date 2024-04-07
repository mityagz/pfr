# **PFR**  

PFR is performance routing. This type of routing is based on round trip time 
and packet loss instead of routing protocols metrics.
This code sends icmp probes to list of destinations through several SPs at the same time
and measure rtt and packet loss over these SPs for each destination of the list.
Then the code injects specific(/32) bgp routes with appropriate next-hop 
into own AS (don't advertise any external AS), it directs traffic throught 
external peer with min rtt && packet loss. Gobgp(exec gobgp or grpc are used) is used for routes injection.  

This code gets list of destination prefixes(top n) from [ipfixcol2pfr](https://github.com/mityagz/ipfixcol2pfr) to use shm ipc.  
We should use some external mechanism to direct probes toward appropriate SP.
SP id is contained into icmp payload, we can match the id(see [ping_data.h](https://github.com/mityagz/pfr/tree/master/src/ping_data.h)) to use packet filter and send to corresponding vrf/vlan and further to appropriate SP.

The code isn't production ready yet!

TODO:  

* Handle IPv6 routes.
