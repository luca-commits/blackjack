#ifndef SERVER_NETWORK_MANAGER_HPP
#define SERVER_NETWORK_MANAGER_HPP

#include <thread>
#include <functional>
#include <unordered_map>
#include <shared_mutex>

#include "sockpp/tcp_socket.h"
#include "sockpp/tcp_connector.h"
#include "sockpp/tcp_acceptor.h"

#include "../general/network/requests/client_request.hpp"
#include "../general/network/responses/server_response.hpp"
#include "../general/game_state/player.hpp"
#include "../general/game_state/game_state.hpp"

class server_network_manager{
private:

  inline static server_network_manager* _instance;
  /*
  Mutex logic:
  - shared lock can be acquired by multiple threads, only if no exclusive lock is acquired
  - exclusive lock can be acquired only by a single thread, only if no shared lock is acquired
  - only read with shared lock acquired, only write with exclusive lock acquired
  - lock_shared before reading, so no other thread can write while we read
  - lock before writing, so no other thread can read or write while we write
  */
  inline static std::shared_mutex _rw_lock;
  inline static sockpp::tcp_acceptor _acc;

  inline static std::unordered_map<std::string, std::string> _player_id_to_address;
  inline static std::unordered_map<std::string, sockpp::tcp_socket> _address_to_socket;

  void connect(const std::string& url, const uint16_t port);

  static void listener_loop();
  static void read_message(sockpp::tcp_socket socket, const std::function<void(const std::string&, const sockpp::tcp_socket::addr_t&)>& message_handler);
  static void handle_incoming_message(const std::string& msg, const sockpp::tcp_socket::addr_t& peer_address);
  static ssize_t send_message(const std::string& msg, const std::string& address);

public:
  server_network_manager();
  ~server_network_manager();

  static void broadcast_message(server_response& msg, const std::vector<player*>& players, const player* excluded_player);
  static void on_player_left(std::string player_id);

};

#endif
