#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string.h>

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include "gobgp.grpc.pb.h"
#include "attribute.grpc.pb.h"
#include "pfr_gobgp_grpc.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using apipb::GobgpApi;

extern std::shared_ptr<spdlog::logger> syslog_logger;

gobgp_grpc::gobgp_grpc(std::shared_ptr<Channel> channel) : stub_(GobgpApi::NewStub(channel)) {}
gobgp_grpc::gobgp_grpc() {}
bool gobgp_grpc::add_route4(std::string dst_ip, std::string nh, std::string comm) {
        std::string prefix_route = dst_ip;
        int lcomm = std::stoi(comm.substr(comm.find(':') + 1));
        int hcomm = std::stoi(comm.substr(0, comm.find(':')));
        int prefix_len = 32;

        syslog_logger->debug("gobgp_grpc::add_route4: {}:{}:{}:{}:{}", prefix_route, prefix_len, nh, hcomm, lcomm);
		apipb::AddPathRequest request;
		ClientContext context;
		apipb::AddPathResponse response;

		apipb::Path *current_path = new apipb::Path;

		apipb::Family *current_family = new apipb::Family;
		current_family->set_afi(apipb::Family::AFI_IP);
		current_family->set_safi(apipb::Family::SAFI_UNICAST);
		current_path->set_allocated_family(current_family);

		google::protobuf::Any *current_nlri = new google::protobuf::Any;
		apipb::IPAddressPrefix current_ipaddrprefix;
		current_ipaddrprefix.set_prefix(prefix_route);
		current_ipaddrprefix.set_prefix_len(prefix_len);
		current_nlri->PackFrom(current_ipaddrprefix);
		current_path->set_allocated_nlri(current_nlri);

		google::protobuf::Any *current_origin = current_path->add_pattrs();
		apipb::OriginAttribute current_origin_t;
		current_origin_t.set_origin(0);
		current_origin->PackFrom(current_origin_t);

		google::protobuf::Any *current_next_hop = current_path->add_pattrs();
		apipb::NextHopAttribute current_next_hop_t;
		current_next_hop_t.set_next_hop(nh);
		current_next_hop->PackFrom(current_next_hop_t);

		google::protobuf::Any *current_communities = current_path->add_pattrs();
		apipb::CommunitiesAttribute current_communities_t;
        int cm = (hcomm * 65536) + lcomm;
		current_communities_t.add_communities(cm);
		current_communities->PackFrom(current_communities_t);

		request.set_table_type(apipb::TableType::GLOBAL);
		request.set_allocated_path(current_path);

		Status status = stub_->AddPath(&context, request, &response);
		if (status.ok()) {
		} else {
			std::cout << status.error_code() << ": " << status.error_message() << std::endl;
		}

       return true;
	}

bool gobgp_grpc::del_route4(std::string dst_ip, std::string nh, std::string comm) {
        std::string prefix_route = dst_ip;
        int lcomm = std::stoi(comm.substr(comm.find(':') + 1));
        int hcomm = std::stoi(comm.substr(0, comm.find(':')));
        int prefix_len = 32;

        syslog_logger->debug("gobgp_grpc::del_route4: {}:{}:{}:{}:{}", prefix_route, prefix_len, nh, hcomm, lcomm);
		apipb::DeletePathRequest request;
		ClientContext context;
        google::protobuf::Empty response;

		apipb::Path *current_path = new apipb::Path;

		apipb::Family *current_family = new apipb::Family;
		current_family->set_afi(apipb::Family::AFI_IP);
		current_family->set_safi(apipb::Family::SAFI_UNICAST);
		current_path->set_allocated_family(current_family);

		google::protobuf::Any *current_nlri = new google::protobuf::Any;
		apipb::IPAddressPrefix current_ipaddrprefix;
		current_ipaddrprefix.set_prefix(prefix_route);
		current_ipaddrprefix.set_prefix_len(prefix_len);
		current_nlri->PackFrom(current_ipaddrprefix);
		current_path->set_allocated_nlri(current_nlri);

		google::protobuf::Any *current_origin = current_path->add_pattrs();
		apipb::OriginAttribute current_origin_t;
		current_origin_t.set_origin(0);
		current_origin->PackFrom(current_origin_t);

		google::protobuf::Any *current_next_hop = current_path->add_pattrs();
		apipb::NextHopAttribute current_next_hop_t;
		current_next_hop_t.set_next_hop(nh);
		current_next_hop->PackFrom(current_next_hop_t);

		google::protobuf::Any *current_communities = current_path->add_pattrs();
		apipb::CommunitiesAttribute current_communities_t;
        int cm = (hcomm * 65536) + lcomm;
		current_communities_t.add_communities(cm);
		current_communities->PackFrom(current_communities_t);

		request.set_table_type(apipb::TableType::GLOBAL);
		request.set_allocated_path(current_path);

		Status status = stub_->DeletePath(&context, request, &response);
		if (status.ok()) {
		} else {
			std::cout << status.error_code() << ": " << status.error_message() << std::endl;
		}

       return true;
	}
bool gobgp_grpc::add_route6(std::string dst_ip, std::string nh, std::string comm) {}
bool gobgp_grpc::del_route6(std::string dst_ip) {}
