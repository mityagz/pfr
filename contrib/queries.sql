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

-- 8. This query shows num prefixes per peer w/stats/metric  and round which pfrd send to gobgpd
select n.hostname, l.peer_id, count(l.ipv4_dest_address) as num_prefixes_per_peer,
       round(cast(sum(rtt_min) / count(rtt_min) as numeric), 2) as avg_rtt_min,
       sum(pkt_loss) / count(pkt_loss) as avg_pkt_loss,
       round(cast(sum(rtt_min) / count(rtt_min) as numeric), 2),
       round(cast(sum(max_rtt) / count(max_rtt) as numeric), 2) as avg_max_rtt,
       round(cast(sum(stddev_rtt) / count(stddev_rtt) as numeric), 2) as avg_stddev_rtt,
       round(cast(sum(mediana_rtt) / count(mediana_rtt) as numeric), 2) as avg_mediana_rtt,
       (sum(rtt_min + rtt_min * pkt_loss) / count(rtt_min)) as avg_cmpx_metric
from pfr_log l
join pfr_peers p on l.peer_id = p.id
join node n on p.node_id = n.id 
where probe_id in (select max(probe_id) from pfr_log l) group by n.hostname, l.peer_id;

/*
hostname | peer_id | num_prefixes_per_peer | avg_rtt_min | avg_pkt_loss | round | avg_max_rtt | avg_stddev_rtt | avg_mediana_rtt | avg_cmpx_metric  
----------+---------+-----------------------+-------------+--------------+-------+-------------+----------------+-----------------+------------------
 IPFIX1   |      10 |                     6 |       50.52 |            1 | 50.52 |       53.65 |           0.37 |           51.84 | 113.608498086532
 IPFIX0   |       9 |                     2 |       77.32 |            1 | 77.32 |       79.78 |           0.24 |           79.12 | 154.641998291016
 PE0      |       6 |                     3 |       74.98 |            2 | 74.98 |       76.07 |           0.16 |           76.07 | 224.944999694824
 PE1      |       4 |                     6 |       65.03 |            2 | 65.03 |       67.48 |           0.35 |           66.41 | 170.369164943695
 PE1      |       8 |                     4 |       57.87 |            1 | 57.87 |       59.20 |           0.13 |           58.81 | 116.617000699043
 PE0      |       2 |                     4 |       75.96 |            1 | 75.96 |       80.38 |           0.47 |           78.88 | 209.330244064331
 PE1      |       7 |                     2 |       40.38 |            2 | 40.38 |       40.84 |           0.09 |           40.57 | 158.062999486923
(7 rows)
*/

-- 9. This query shows num prefixes per peer w/stats/metric  and round which pfrd send to gobgpd
select n.hostname, l.peer_id, count(l.ipv4_dest_address) as num_prefixes_per_peer,
       round(cast(sum(rtt_min) / count(rtt_min) as numeric), 2) as avg_rtt_min,
       sum(pkt_loss) / count(pkt_loss) as avg_pkt_loss,
       round(cast(sum(rtt_min) / count(rtt_min) as numeric), 2),
       round(cast(sum(max_rtt) / count(max_rtt) as numeric), 2) as avg_max_rtt,
       round(cast(sum(stddev_rtt) / count(stddev_rtt) as numeric), 2) as avg_stddev_rtt,
       round(cast(sum(mediana_rtt) / count(mediana_rtt) as numeric), 2) as avg_mediana_rtt,
       round(cast((sum(rtt_min + rtt_min * pkt_loss) / count(rtt_min)) as numeric), 2) as avg_cmpx_metric
from pfr_log l
join pfr_peers p on l.peer_id = p.id
join node n on p.node_id = n.id
where probe_id in (select max(probe_id) from pfr_log l) group by n.hostname, l.peer_id;

/*
 hostname | peer_id | num_prefixes_per_peer | avg_rtt_min | avg_pkt_loss | round | avg_max_rtt | avg_stddev_rtt | avg_mediana_rtt | avg_cmpx_metric 
----------+---------+-----------------------+-------------+--------------+-------+-------------+----------------+-----------------+-----------------
 IPFIX1   |      10 |                     6 |       76.10 |            1 | 76.10 |       78.70 |           0.37 |           77.85 |          151.02
 IPFIX0   |       9 |                     4 |       75.69 |            1 | 75.69 |       78.65 |           0.32 |           77.66 |          172.24
 PE0      |       6 |                     4 |       58.99 |            1 | 58.99 |       61.72 |           0.25 |           60.27 |          138.14
 PE1      |       4 |                     3 |       77.10 |            1 | 77.10 |       79.40 |           0.30 |           78.88 |          157.53
 PE0      |       2 |                     3 |       55.01 |            0 | 55.01 |       57.68 |           0.24 |           57.25 |           82.73
 PE0      |       5 |                     2 |       75.20 |            1 | 75.20 |       79.40 |           0.38 |           78.17 |          190.72
 PE1      |       7 |                     5 |       30.87 |            1 | 30.87 |       33.08 |           0.24 |           31.84 |           48.16
(7 rows)
*/
