-- 1.  num of uniq addresses
select count(distinct ipv4_dest_address)
from pfr_log;

-- 2.  list of uniq addresses
select ipv4_dest_address 
from pfr_log 
group by ipv4_dest_address 
order by ipv4_dest_address;

-- 3. select all
select * 
from pfr_log l, pfr_peers p 
where l.peer_id = p.id
order by ipv4_dest_address;

-- 4. select all for last probe_id
select * 
from pfr_log l 
join pfr_peers p on l.peer_id = p.id  
where probe_id in (select max(probe_id) from pfr_log l) 
order by ipv4_dest_address, probe_id;

-- 5. This query shows num prefixes per peer which pfrd send to gobgpd
select n.hostname, l.peer_id, count(l.ipv4_dest_address) as num_prefixes_per_peer 
from pfr_log l
join pfr_peers p on l.peer_id = p.id
join node n on p.node_id = n.id
where probe_id in (select max(probe_id) from pfr_log l) group by n.hostname, l.peer_id;

/*
hostname | peer_id | num_prefixes_per_peer 
----------+---------+-----------------------
 IPFIX0   |       9 |                     6
 PE0      |       6 |                     7
 PE1      |       4 |                     2
 PE1      |       8 |                     1
 PE0      |       2 |                     1
 PE0      |       5 |                     1
 PE1      |       7 |                     4
 IPFIX1   |      10 |                     5
(8 rows)
*/

-- 6. This query shows num prefixes per peer w/stats which pfrd send to gobgpd
select n.hostname, l.peer_id, count(l.ipv4_dest_address) as num_prefixes_per_peer, sum(rtt_min) / count(rtt_min) as avg_rtt_min, 
       sum(pkt_loss) / count(pkt_loss) as avg_pkt_loss
from pfr_log l
join pfr_peers p on l.peer_id = p.id
join node n on p.node_id = n.id 
where probe_id in (select max(probe_id) from pfr_log l) group by n.hostname, l.peer_id;

/*
hostname | peer_id | num_prefixes_per_peer |   avg_rtt_min    | avg_pkt_loss 
----------+---------+-----------------------+------------------+--------------
 IPFIX0   |       9 |                     5 | 77.6192016601562 |            1
 PE0      |       6 |                     5 | 62.5815979003906 |            0
 PE1      |       4 |                     1 | 2.01200008392334 |            2
 PE0      |       2 |                     3 | 50.0956624348958 |            1
 PE1      |       8 |                     2 | 74.6735000610352 |            1
 PE0      |       5 |                     3 | 51.0523376464844 |            1
 PE1      |       7 |                     4 |  72.619499206543 |            2
 IPFIX1   |      10 |                     4 | 56.4552459716797 |            1
(8 rows)
*/

-- 7. This query shows num prefixes per peer w/stats and round which pfrd send to gobgpd
select n.hostname, l.peer_id, count(l.ipv4_dest_address) as num_prefixes_per_peer, round(cast(sum(rtt_min) / count(rtt_min) as numeric), 2) as avg_rtt_min,
       sum(pkt_loss) / count(pkt_loss) as avg_pkt_loss
from pfr_log l
join pfr_peers p on l.peer_id = p.id
join node n on p.node_id = n.id  
where probe_id in (select max(probe_id) from pfr_log l) group by n.hostname, l.peer_id;

/*
hostname | peer_id | num_prefixes_per_peer | avg_rtt_min | avg_pkt_loss 
----------+---------+-----------------------+-------------+--------------
 IPFIX0   |       9 |                     3 |       76.05 |            1
 PE0      |       6 |                     3 |        2.42 |            1
 PE1      |       4 |                     5 |       76.76 |            1
 PE0      |       2 |                     7 |       77.05 |            1
 PE1      |       8 |                     2 |       77.65 |            1
 PE0      |       5 |                     4 |       58.75 |            1
 PE1      |       7 |                     1 |       72.61 |            1
 IPFIX1   |      10 |                     2 |       39.93 |            2
(8 rows)
*/
