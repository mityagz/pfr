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
