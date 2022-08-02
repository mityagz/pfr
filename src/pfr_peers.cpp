#include <iostream>
#include <string>
#include <map>
#include <libpq-fe.h>
#include "pfr_peers.h"

using namespace std;

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

/*
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
   */
pfr_peer::pfr_peer() {}
//pfr_peer::pfr_peer(const pfr_peer &) {}
pfr_peer::pfr_peer(int id, int peer_group_id, std::string pe_ip, std::string pe, int interface_id, std::string address, \
             std::string ipv4_peer_address, std::string ipv6_peer_address, \
             std::string sp_name, int pfr_dst_id) : id(id), pe_ip(pe_ip) {
//pfr_peer::pfr_peer(int id, int peer_group_id, std::string pe_ip, std::string pe, int interface_id, std::string address, \
//             std::string laddress, std::string ipv4_peer_address, std::string lipv4_peer_address, std::string ipv6_peer_address, \
//             std::string lipv6_peer_address, std::string type, std::string sp_name, int pfr_dst_id) {
}

int pfr_peer::pfr_peer_get_id() { return id; }
int pfr_peer::peer_group_get_id() { return peer_group_id; }
std::string pfr_peer::pfr_peer_get_pe_ip() { return pe_ip; }
std::string pfr_peer::get_pe_name() { return pe; }
int pfr_peer::get_interface_id() { return interface_id; }
std::string pfr_peer::get_address() { return address; }
std::string pfr_peer::get_laddress() { return laddress; }
std::string pfr_peer::get_ipv4_peer_address() { return ipv4_peer_address; }
std::string pfr_peer::get_lipv4_peer_address() { return lipv4_peer_address; }
std::string pfr_peer::get_ipv6_peer_address() { return ipv6_peer_address; }
std::string pfr_peer::get_lipv6_peer_address() { return lipv6_peer_address; }
std::string pfr_peer::get_type() { return type; }
std::string pfr_peer::get_sp_name() { return sp_name; }
int         pfr_peer::get_pfr_dst_id() { return pfr_dst_id; }

//class pfr_peers {
//    std::map<int, pfr_peer> pfr_peers_v;
//   public:

pfr_peers::pfr_peers() {}
pfr_peers::pfr_peers(std::map<int, pfr_peer> &m) {
    const char  *pghost="127.0.0.1",
    //const char  *pghost="192.168.122.19",
    *pgport="5432",
    *pgoptions=NULL,
    *pgtty=NULL,
    *dbName="vc",
    *login="vc",
    *pwd="vc";

    PGconn *conn;
    PGresult *res;


    conn = PQsetdbLogin(pghost, pgport, pgoptions, pgtty, dbName, login, pwd);
        if(PQstatus(conn) == CONNECTION_BAD) {
            fprintf(stderr, "Connection to database '%s' failed.\n", dbName);
            fprintf(stderr, "%s", PQerrorMessage(conn));
        } else {
#ifdef DEBUG
            fprintf(stderr, "Connection to database '%s' Ok.\n", dbName);
#endif
        } 

        res = PQexec(conn, "select p.id, p.peer_group_id, n.ip as ip, n.hostname as pe, p.interface_id, a.address, p.ipv4_peer_address, \
                p.ipv6_peer_address, p.name, p.pfr_dst_id  from pfr_peers p join node n on n.id = p.node_id \
                join ipam_addresses a on a.id = p.address_id");

                //p.ipv6_peer_address, p.type, p.name, p.pfr_dst_id  from pfr_peers p join node n on n.id = p.node_id \

        int ncols = PQnfields(res);
        /*
        printf("There are %d columns:", ncols);
        for(int i = 0; i < ncols; i++) {
            char *name = PQfname(res, i);
            printf(" %s", name);
        }
        printf("\n");
        */

        int nrows = PQntuples(res);
        for(int i = 0; i < nrows; i++) {
            int id = std::stoi(PQgetvalue(res, i, 0));
            int peer_group_id = std::stoi(PQgetvalue(res, i, 1));
            std::string pe_ip = PQgetvalue(res, i, 2);
            std::string pe = PQgetvalue(res, i, 3);
            int interface_id = std::stoi(PQgetvalue(res, i, 4));
            std::string address = PQgetvalue(res, i, 5);
            //std::string laddress = PQgetvalue(res, i, 6);
            std::string ipv4_peer_address = PQgetvalue(res, i, 6);
            //std::string lipv4_peer_address = PQgetvalue(res, i, 8);
            std::string ipv6_peer_address = PQgetvalue(res, i, 7);
            //std::string lipv6_peer_address = PQgetvalue(res, i, 10);
            std::string sp_name = PQgetvalue(res, i, 8);
            int pfr_dst_id = std::stoi(PQgetvalue(res, i, 9));
            //pfr_peers::pfr_peers_v[id] = pfr_peer(id, peer_group_id, pe_ip, pe, interface_id, address, \
            //                              ipv4_peer_address, ipv6_peer_address, \
            //                              sp_name, pfr_dst_id);
            pfr_peers::pfr_peers_v.insert(std::make_pair(id, pfr_peer(id, peer_group_id, pe_ip, pe, interface_id, address, \
                                          ipv4_peer_address, ipv6_peer_address, \
                                          sp_name, pfr_dst_id)));
            //pfr_peers::pfr_peers_v.insert(std::make_pair(id, pfr_peer(id, peer_group_id, pe_ip, pe, interface_id, address, \
            //                              laddress, ipv4_peer_address, lipv4_peer_address, ipv6_peer_address, lipv6_peer_address, \
            //                              type, sp_name, pfr_dst_id)));
        }
        PQclear(res);
        PQfinish(conn);

        for(std::map<int, pfr_peer>::iterator it = pfr_peers::pfr_peers_v.begin(); it != pfr_peers::pfr_peers_v.end(); ++it) {
              m.insert(std::make_pair(it->first, it->second));
        }

}

    int pfr_peers::size() {
        return pfr_peers_v.size();
    }
    //void pfr_peer_add(int id, pfr_peer p);
    //void pfr_dst_print();
    //std::map<int, pfr_peer> get_pfr_peers() { return pfr_peers::pfr_peers_v; }
//}
