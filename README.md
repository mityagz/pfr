PFR is performance routing. This type of routing is based on round trip time 
and packet loss instead of routing protocols metrics.
This code sends icmp probes to list of destinations through several isp at the same time
and measure rtt and packet loss over these isp for each destination of the list.
Then the code injecs specific(/32||/128) bgp routes with appropriate next-hop 
into own as (don"t advertise any external as), it directs traffic throught 
external peer with min rtt&&packet loss.

This code isn't production ready!