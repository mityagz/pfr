#ifndef PFR_PEERS_H
#define PFR_PEERS_H
#include <string>
#include <map>

/*
vc=> select p.id, p.peer_group_id, n.ip as ip, n.hostname as pe, p.interface_id, a.address, p.ipv4_peer_address, p.ipv6_peer_address, p.type, p.name, p.pfr_dst_id  from pfr_peers p join node n on n.id = p.node_id join ipam_addresses a on a.id = p.address_id;
id | peer_group_id |      ip      | pe  | interface_id |    address     | ipv4_peer_address | ipv6_peer_address |  type  | name  | pfr_dst_id 
----+---------------+--------------+-----+--------------+----------------+-------------------+-------------------+--------+-------+------------
4 |             1 | 10.229.132.0 | PE1 |            2 | 198.51.0.2/31  | 198.51.0.3        |                   | uplink | ISP1  |          0
6 |             0 | 10.229.4.0   | PE0 |            4 | 198.51.0.26/31 | 198.51.0.27       |                   | uplink | ISP02 |          0
5 |             0 | 10.229.4.0   | PE0 |            3 | 198.51.0.24/31 | 198.51.0.25       |                   | uplink | ISP01 |          0
7 |             1 | 10.229.132.0 | PE1 |            5 | 198.51.0.28/31 | 198.51.0.29       |                   | uplink | ISP12 |          0
8 |             1 | 10.229.132.0 | PE1 |            6 | 198.51.0.30/31 | 198.51.0.31       |                   | uplink | ISP13 |          0
2 |             0 | 10.229.4.0   | PE0 |            1 | 198.51.0.0/31  | 198.51.0.1        |                   | uplink | ISP0  |          0
*/

class pfr_peer {
    int id; 
    int peer_group_id;
    std::string pe_ip;
    std::string pe;
    int interface_id;
    std::string address;
    std::string laddress;
    std::string ipv4_peer_address;
    std::string lipv4_peer_address;
    std::string ipv6_peer_address;
    std::string lipv6_peer_address;
    std::string type;
    std::string sp_name;
    int pfr_dst_id;
   public:
    pfr_peer(int id, int peer_group_id, std::string pe_ip, std::string pe, int interface_id, std::string address, \
             std::string ipv4_peer_address, std::string ipv6_peer_address, \
             std::string sp_name, int pfr_dst_id);
    //pfr_peer(int id, int peer_group_id, std::string pe_ip, std::string pe, int interface_id, std::string address, \
    //         std::string laddress, std::string ipv4_peer_address, std::string lipv4_peer_address, std::string ipv6_peer_address, \
    //         std::string lipv6_peer_address, std::string type, std::string sp_name, int pfr_dst_id);
    int pfr_peer_get_id();
    std::string pfr_peer_get_pe_ip();
};

class pfr_peers {
    std::map<int, pfr_peer> pfr_peers_v;
   public:
    pfr_peers();
    pfr_peers(std::map<int, pfr_peer> &m);
    void pfr_peer_add(int id, pfr_peer p);
    void pfr_dst_print();
    std::map<int, pfr_peer> get_pfr_peers();
    int size();
};


#endif // PFR_PEERS_H
