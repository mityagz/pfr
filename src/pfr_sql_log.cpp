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

int pfr_sql_log::insert(std::string dst_ip, int probe_id, int peer_id, double min_rtt, double avg_rtt, int lost, int ts) {
    if (!this->allow_sql_log)
        return 0;
    syslog_logger->debug("pfr_sql_log::insert() dst_ip: {}, probe_id: {}, peer_id: {}, min_rtt: {}, avg_rtt: {}, lost: {}, ts: {}", dst_ip, probe_id, peer_id, min_rtt, avg_rtt, lost, ts);
    return 0;

}
