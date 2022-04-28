#include "server_network_manager.hpp"
#include "request_handler.hpp"

#include "../general/network/default.conf"
#include "../general/network/responses/request_response.hpp"

server_network_manager::server_network_manager(){
  //If there's no server_network_manager yet, initialize
  if(_instance == nullptr){
    _instance = *this;
  }
  sockpp::socket_initializer socket_initializer;
  this->connect(default_server_host, default_port)
}

server_network_manager::~server_network_manager() = default;
