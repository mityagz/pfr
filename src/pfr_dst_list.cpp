#include <vector>
#include <string>
#include <iostream>
#include <libpq-fe.h>
#include "pfr_dst_list.h"
#include "pfr_dst.h"
#include "pfr_sql.h"
#include "pfr_sql_list.h"

pfr_dst_list::pfr_dst_list(int nlist) {
        //this->nlist = nlist;
        pfr_sql_list psl = pfr_dst_list::get_pfr_dst_sql();
        std::vector<pfr_sql> pslq = psl.get();

        for(int i = 0; i < pslq.size(); i++) {
            pfrDstList.push_back(pfr_dst(pslq[i].pfr_sql_get_id(), pslq[i].pfr_sql_get_ipv4(), pslq[i].pfr_sql_get_ipv6(), pslq[i].pfr_sql_get_descr()));
        }
}

pfr_sql_list pfr_dst_list::get_pfr_dst_sql() {
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

    res = PQexec(conn, "select  id, ipv4_dst_address, ipv6_dst_address, description from pfr_dst");
    //res = PQexec(conn, "select  id, ipv4_dst_address, ipv6_dst_address, description from pfr_dst limit 500");
    int ncols = PQnfields(res);
    /*
    printf("There are %d columns:", ncols);
    for(int i = 0; i < ncols; i++) {
        char *name = PQfname(res, i);
        printf(" %s", name);
    }
    printf("\n");
    */

    pfr_sql_list psl;

    int nrows = PQntuples(res);
    this->nlist = nrows;
    for(int i = 0; i < nrows; i++) {
        int id = std::stoi(PQgetvalue(res, i, 0));
        std::string ipv4_dst_address = PQgetvalue(res, i, 1);
        std::string ipv6_dst_address = PQgetvalue(res, i, 2);
        std::string description = PQgetvalue(res, i, 3);
        psl.pfr_sql_list_add(pfr_sql(id, ipv4_dst_address, ipv6_dst_address, description));
        //std::cout << id << ':' << ipv4_dst_address << ':' << ipv6_dst_address << ':' <<  description << '\n';
    }
    PQclear(res);
    PQfinish(conn);
    return(psl);
}

void pfr_dst_list::pfr_dst_print() {
        for(int i = 0; i < nlist; i++) {
            pfrDstList[i].pfr_dst_print();
        }
    
}
