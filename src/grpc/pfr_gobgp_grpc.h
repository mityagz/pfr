#ifndef PFR_GOBGP_GRPC_H
#define PFR_GOBGP_GRPC_H
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
#include "gobgp.grpc.pb.h"
#include "attribute.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using apipb::GobgpApi;

class gobgp_grpc {
  private:
	std::unique_ptr<GobgpApi::Stub> stub_;
  public:
	gobgp_grpc();
	gobgp_grpc(std::shared_ptr<Channel> channel);
    bool add_route4(std::string dst_ip, std::string nh, std::string comm);
    bool del_route4(std::string dst_ip, std::string nh, std::string comm);
    bool del_route4(std::string dst_ip);
    bool add_route6(std::string dst_ip, std::string nh, std::string comm);
    bool del_route6(std::string dst_ip);
};
#endif // PFR_GOBGP_GRPC_H
