#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <libpq-fe.h>
#include <semaphore.h>
#include <iomanip>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "pfr_dst_list.h"
#include "pfr_dst.h"
#include "pfr_sql.h"
#include "pfr_sql_list.h"

extern std::shared_ptr<spdlog::logger> syslog_logger;

pfr_dst_list::pfr_dst_list() {}

pfr_dst_list::pfr_dst_list(int nlist) {
        //this->nlist = nlist;
        pfr_sql_list psl = pfr_dst_list::get_pfr_dst_sql();
        std::vector<pfr_sql> pslq = psl.get();

        for(int i = 0; i < pslq.size(); i++) {
            pfrDstList.push_back(pfr_dst(pslq[i].pfr_sql_get_id(), pslq[i].pfr_sql_get_ipv4(), pslq[i].pfr_sql_get_ipv6(), pslq[i].pfr_sql_get_descr()));
        }
}

pfr_dst_list::pfr_dst_list(int nlist, int nnlist, pfr_dst_list &prevdstList) {
        const char *key_sem0 = "/key_sem0";
        sem_t *sem0;

        //std::cout << "pfr_dst_list(int, int, prevdstList)" << std::endl;
        syslog_logger->debug("pfr_dst_list(int, int, prevdstList)");

        const char *shm_key_p0 = "/tmp/key0_shm0";
        key_t shm_key0;
        int shm_id = 0;
        void *shm_addr;
        char *ret_shm_addr;
        const char* delimiters = ":";

        shm_key0 = ftok(shm_key_p0, 1);
        if(shm_key0 == -1) {
            perror("ftok error");
            exit(1);
        }

        sem0 = sem_open(key_sem0, 0, 0);

        if(sem_wait(sem0) == 0) {
            //std::cout << "SEM0: Locked! overload" << std::endl;
            syslog_logger->debug("SEM0: Locked! overload");
        }

        shm_id = shmget(shm_key0, 0, 0);
        if(shm_id == -1) {
           perror("Shared memory 0");
           exit(1);
        }

        shm_addr = shmat(shm_id, NULL, 0);
        if(shm_addr == (void *) -1) {
           perror("Shared memory attach");
           exit(1);
        }
                                 
        ret_shm_addr = (char *)shm_addr;

        int dst_id = 0;
        char *token = std::strtok(ret_shm_addr, delimiters);
        while (token) {
            //std::cout << dst_id << ":" << token << std::endl;
            syslog_logger->debug("{}:{}", dst_id, token);
            token = std::strtok(nullptr, delimiters);
            if(token != NULL) {
             pfrDstList.push_back(pfr_dst(dst_id , std::string(token), "" ,""));
             dst_id++;
            }
        }

        if(pfrDstList.size() < 3) {
            pfrDstList.clear();
            pfr_dst_list::iterator it = prevdstList.begin();
            pfr_dst_list::iterator ite = prevdstList.end();
            while(it != ite) {
             int hid = (*it).get_id();
             std::string hip = (*it).get_ipv4();
             pfrDstList.push_back((*it));
             it++;
            }
        }

        shmdt(shm_addr);
        if(sem_post(sem0) == 0) {
           //std::cout << "SEM0: UnLocked! overload" << std::endl;
           syslog_logger->debug("SEM0: UnLocked! overload");
        }

}

pfr_dst_list::pfr_dst_list(int nlist, int nnlist) {
        const char *key_sem0 = "/key_sem0";
        sem_t *sem0;
        //std::cout << "pfr_dst_list(int, int)" << std::endl;
        syslog_logger->debug("pfr_dst_list(int, int)");

        const char *shm_key_p0 = "/tmp/key0_shm0";
        key_t shm_key0;
        int shm_id = 0;
        void *shm_addr;
        char *ret_shm_addr;
        const char* delimiters = ":";

        shm_key0 = ftok(shm_key_p0, 1);
        if(shm_key0 == -1) {
            perror("ftok error");
            exit(1);
        }

        sem0 = sem_open(key_sem0, 0, 0);

        if(sem_wait(sem0) == 0) {
            //std::cout << "SEM0: Locked!" << std::endl;
        }

        shm_id = shmget(shm_key0, 0, 0);
        if(shm_id == -1) {
           perror("Shared memory 0");
           exit(1);
        }

        shm_addr = shmat(shm_id, NULL, 0);
        if(shm_addr == (void *) -1) {
           perror("Shared memory attach");
           exit(1);
        }
                                 
        /*
        std::cout << "shm_addr: " << shm_addr << std::endl;
        ret_shm_addr = (char *)shm_addr;
        std::cout << "ret_shm_addr: " << ret_shm_addr << std::endl;
        std::cout << "-----------------------------------------------: " << std::endl;
        */
        ret_shm_addr = (char *)shm_addr;

        int dst_id = 0;
        char *token = std::strtok(ret_shm_addr, delimiters);
        while (token) {
            //std::cout << dst_id << ":" << token << std::endl;
            syslog_logger->debug("{}:{}", dst_id, token);
            token = std::strtok(nullptr, delimiters);
            if(token != NULL) {
             pfrDstList.push_back(pfr_dst(dst_id , std::string(token), "" ,""));
             dst_id++;
             //if(dst_id > 1000)
               //  break;
            }
        }

        shmdt(shm_addr);
        if(sem_post(sem0) == 0) {
           //std::cout << "SEM0: UnLocked!" << std::endl;
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

int pfr_dst_list::size() {
    return pfrDstList.size();
}
