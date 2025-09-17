-- 1.
-- num of uniq addresses
select count(distinct ipv4_dest_address)
from pfr_log;

-- 2.
-- list of uniq addresses
select ipv4_dest_address 
from pfr_log 
group by ipv4_dest_address 
order by ipv4_dest_address;

-- 3.
-- select all
select * 
from pfr_log l, pfr_peers p 
where l.peer_id = p.id
order by ipv4_dest_address;
