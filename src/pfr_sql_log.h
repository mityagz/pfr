#ifndef PFR_SQL_LOG_H
#define PFR_SQL_LOG_H
#include <libpq-fe.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <string>
#include <iostream>
#include <sstream>

class pfr_sql_log {
 
 const char *pgoptions=NULL,
                *pgtty=NULL;
    PGconn *conn;
    PGresult *res;
    bool allow_sql_log;
    std::string ins_head;
    //std::stringstream ins_multi;
    std::string ins_multi;

 public:
    pfr_sql_log(bool allow_sql_log);
    ~pfr_sql_log();
    //int insert(int ccase, std::string dst_ip, int probe_id, int peer_id, double min_rtt, double avg_rtt, int lost, int ts);
    int insert(int ccase, std::string dst_ip, int probe_id, int peer_id, double min_rtt, double avg_rtt,double max_rtt, double stddev_rtt, double mediana_rtt, int lost, int ts);
    int insert1(int ccase, std::string dst_ip, int probe_id, int peer_id, double min_rtt, double avg_rtt, int lost, int ts);
    int commit();
};

typedef struct {
    int probe_id;
    pfr_sql_log *log;
} thlog;

#endif //PFR_SQL_LOG
