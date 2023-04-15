#include <sstream>
#include "pfr_sql_log.h"




extern std::string pghost;
extern std::string pgport;
extern std::string db_name;
extern std::string login;
extern std::string pwd;

extern std::shared_ptr<spdlog::logger> syslog_logger;

pfr_sql_log::pfr_sql_log(bool allow_sql_log) {
  if (allow_sql_log) {
    this->allow_sql_log = allow_sql_log;
    conn = PQsetdbLogin(pghost.c_str(), pgport.c_str(), pgoptions, pgtty, db_name.c_str(), login.c_str(), pwd.c_str());
    if(PQstatus(conn) == CONNECTION_BAD) {
      fprintf(stderr, "Connection to database '%s' failed.\n", db_name.c_str());
      fprintf(stderr, "%s", PQerrorMessage(conn));
      exit(1);
    } else {
#ifdef DEBUG
     fprintf(stderr, "Connection to database '%s' Ok.\n", db_name.c_str());
#endif
    } 
  }
}

pfr_sql_log::~pfr_sql_log() {
    PQfinish(conn);
}

int pfr_sql_log::insert(int ccase, std::string dst_ip, int probe_id, int peer_id, double min_rtt, double avg_rtt, int lost, int ts) {
    if (!this->allow_sql_log)
        return 0;
    syslog_logger->debug("pfr_sql_log::insert() ccase: {}, dst_ip: {}, probe_id: {}, peer_id: {}, min_rtt: {}, avg_rtt: {}, lost: {}, ts: {}", ccase, dst_ip, probe_id, peer_id, min_rtt, avg_rtt, lost, ts);

    PGresult *res;
    std::string ins0 = "insert into pfr_log (datetime, ccase_id, ipv4_dest_address, probe_id, peer_id, pkts, rtt_min, rtt_avg, pkt_loss, ts) ";
    std::stringstream ins1;
    ins1 << "values (now(), " << ccase << ", '" << dst_ip << "', " << probe_id << ", " << peer_id << ", 5, " << min_rtt << ", " << avg_rtt << ", " << lost << ", " << ts << ");";

    ins0 += ins1.str();


    syslog_logger->debug("pfr_sql_log::insert() ins0: {}", ins0);

    res = PQexec(conn, ins0.c_str());
              
    if(PQresultStatus(res) != PGRES_COMMAND_OK) {
     syslog_logger->debug("pfr_sql_log::insert() INSERT command failed");
     PQclear(res);
    }
    if(res!=NULL)
     PQclear(res);
    /*
    */
    return 0;

}
