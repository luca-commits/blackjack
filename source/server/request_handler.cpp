#ifndef REQUEST_HANDLER_CPP
#define REQUEST_HANDLER_CPP

#include "request_handler.hpp"

#include "player_manager.hpp"
#include "game_instance.hpp"

//include the correct requests !!!!
#include "../common/network/requests/join_game_request.h"
#include "../common/network/requests/draw_card_request.h"
#include "../common/network/requests/play_card_request.h"


request_response* request_handler::handle_request(const client_request* const req) {

    // Prepare variables that are used by every request type
    player* player;
    std::string err;
    game_instance* game_instance_ptr = nullptr;

    // Get common properties of requests
    RequestType type = req->get_type();
    std::string req_id = req->get_req_id();
    std::string game_id = req->get_game_id(); //not needed?
    std::string player_id = req->get_player_id();


    // Switch behavior according to request type
    switch(type) {

        // ##################### JOIN GAME #####################  //
        case RequestType::join_game: {
            
        }


        // ##################### START GAME ##################### //
        case RequestType::start_game: {
            
        }


        // ##################### MAKE BET ##################### //
        case RequestType::make_bet: {
            
        }


        // ##################### ACTION ##################### //
        case RequestType::action: {
            
        }


        // ##################### UNKNOWN REQUEST ##################### //
        default:
            return new request_response("", req_id, false, nullptr, "Unknown RequestType " + type);
    }
}

#endif