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
