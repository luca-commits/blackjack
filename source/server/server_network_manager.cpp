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

/*The first argument seems to be unused*/
void server_network_manager::connect(const std::string &url, const uint16_t port){
  //Construct connection acceptor
  this->_acc = sockpp::tcp_acceptor(port);

  //error check
  if(!_acc){
    std::cerr << "Error creating the acceptor: " << _acc.last_error_str() << std::endl;
    return;
  }

  //start listening for client connections
  std::cout << "Awaiting connections on port " << port << "..." << std::endl;
  listener_loop();
}

void server_network_manager::listener_loop(){
  //This is UB
  while(true){
    sockpp::inet_address peer;

    //listen for new client connections
    //this blocks until a new connection comes in
    sockpp::tcp_socket sock = _acc.accept(&peer);
    std::cout << "Received a connection request from " << peer << std::endl;

    //check for invalid socket
    if(!sock){
      std::cerr << "Error accepting incoming connection: "
                << _acc.last_error_str() << std::endl;
    }
    //spawn handler for this connection
    else{
      _rw_lock.lock(); //want to write
      _address_to_socket.emplace(sock.peer_address().to_string(), std::move(sock.clone()));
      _rw_lock.unlock(); //done writing
      //spawn a new thread that listens for and handles messages on this new connection
      //thread executes read_message(std::move(sock), handle_incoming_message)
      std::thread listener(read_message, std::move(sock), handle_incoming_message);
      listener.detach();
    }
  }
}

void server_network_manager::read_message(sockpp::tcp_socket socket, const std::function<void(const std::string&, const sockpp::tcp_socket::addr_t&)> &message_handler){
  sockpp::socket_initializer sockInit; //This just calls socket::initialize()

  char buffer[512]; //message read goes in here
  /*I'm not sure ssize_t is portable*/
  ssize_t count = 0; //number of bytes read by socket
  ssize_t msg_bytes_read = 0; //length of the message within the received packet
  ssize_t msg_length = 0; //length of message according to packet header

  /*
  Note that socket.read calls ::recv (system call?), which blocks until a message arrives.
  Thus in the absence of errors, socket.read always returns a positive number.
  See https://man7.org/linux/man-pages/man2/recv.2.html
  */
  while((count = socket.read(buffer, sizeof(buffer))) > 0){
    try{
      //extract declared message length
      int i = 0;
      std::stringstream ss_msg_length;
      while(buffer[i] != ':' && i < count){
        ss_msg_length << buffer[i];
        i++;
      }
      msg_length = std::stoi(ss_msg_length.str());
      std::cout << "Expecting message of length " << msg_length << std::endl;

      //extract message
      std::stringstream ss_msg;
      ss_msg.write(&buffer[i+1], count - (i + 1)); //discard header plus :
      msg_bytes_read = count - (i + 1);

      //read the remaining packages (complete message may be larger than 512 bytes)
      while(msg_bytes_read < msg_length && count > 0){
        count = socket.read(buffer, sizeof(buffer));
        msg_bytes_read += count; //everything we read now is part of the message
        ss_msg.write(buffer, count);
      }

      if(msg_bytes_read == msg_length){ //have received complete message
        std::string msg = ss_msg.str();
        message_handler(msg, socket.peer_address()); //handle message
      }
      else{
        std::cerr << "Could not read entire message. TCP stream ended before. Received " << msg_bytes_read << " bytes, expected " << msg_length << "." << std::endl;
        std::cerr << "Received: " << ss_msg.str() << std::endl;
      }
    }
    catch (std::exception& e){
      std::cerr << "Error while reading message from " << socket.peer_address() << std::endl << e.what() << std::endl;
    }
  }

  if(count <= 0){ //nonpositive count values indicate errors
    std::cout << "Read error [" << socket.last_error() << "]: " << socket.last_error_str() << std::endl;
  }

  std::cout << "Closing connection to " << socket.peer_address() << std::endl;
  socket.shutdown();
}

void server_network_manager::handle_incoming_message(const std::string& msg, const sockpp::tcp_socket::addr_t& peer_address){
  try{
    //parse json from message
    rapidjson::Document req_json;
    req_json.Parse(msg.c_str());
    //reconstruct request from json (might throw)
    client_request* req = client_request::from_json(req_json);

    //Check if this is the first message from this address
    std::string player_id = req->get_player_id();
    _rw_lock.lock_shared(); //want to read
    if(_player_id_to_address.find(player_id) == _player_id_to_address.end()){ //player_id not found
      _rw_lock.unlock_shared(); //done reading
      std::cout << "New client with id " << player_id << std::endl;
      _rw_lock.lock(); //want to write (add new player)
      _player_id_to_address.emplace(player_id, peer_address.to_string());
      _rw_lock.unlock(); //done writing
    }
    else{ //player_id found
      _rw_lock.unlock_shared(); //done reading
    }
#ifdef PRINT_NETWORK_MESSAGES
    std::cout << "Received valid request : " << msg << std::endl;
#endif
    //handle request, get response
    server_response* res = request_handler::handle_request(req); //implicit conversion super->base
    delete req;

    //send response back to client
    rapidjson::Document* res_json = res->to_json();
    delete res;
    std::string res_msg = json_utils::to_string(res_json);
    delete res_json; //potential issue: placement of this

#ifdef PRINT_NETWORK_MESSAGES
    std::cout << "Sending response : " << res_msg << std::endl;
#endif

    send_message(res_msg, peer_address.to_string());
  }
  catch(const std::exception& e){
    std::cerr << "Failed to execute client request. Content was\n" << msg << std::endl;
    std::cerr << "Error was " << e.what() << std::endl;
  }
}

//This assumes that the player has been seen before
void server_network_manager::on_player_left(std::string player_id){
  _rw_lock.lock(); //want to write (delete player)
  std::string address = _player_id_to_address[player_id];
  _player_id_to_address.erase(player_id);
  _address_to_socket.erase(address);
  _rw_lock.unlock(); //done writing
}

//this assumes that a client with address exists
ssize_t server_network_manager::send_message(const std::string &msg, const std::string& address){
  std::stringstream ss_msg;
  //generate message in correct format, i.e. payload_size:payload
  ss_msg << std::to_string(msg.size()) << ':' << msg;
  return _address_to_socket.at(address).write(ss_msg.str());
}

void server_network_manager::broadcast_message(server_response &msg, const std::vector<player*> &players, const player* excluded_player){
  //craft message string from msg
  rapidjson::Document* msg_json = msg.to_json();
  std::string msg_string = json_utils::to_string(msg_json); //this returns a std::string
  delete msg_json; //potential issue: placement of this

#ifdef PRINT_NETWORK_MESSAGES
  std::cout << "Broadcasting message : " << msg_string << std::endl;
#endif

  //send message to all players except excluded_player
  _rw_lock.lock_shared();
  try{
    for(auto& player: players){
      if(player != excluded_player){
        int nof_bytes_written = send_message(msg_string, _player_id_to_address.at(player->get_id()));
      }
    }
  }
  catch(std::exception& e){
    std::cerr << "Encountered exception when broadcasting message: " << e.what() << std::endl;
  }
  _rw_lock.unlock_shared();
}
