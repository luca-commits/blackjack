#ifndef REQUEST_HANDLER_CPP
#define REQUEST_HANDLER_CPP

#include "request_handler.hpp"

#include "player_manager.hpp"
#include "game_instance.hpp"
#include "game_instance_manager.hpp"

#include "../general/network/requests/start_game_request.hpp"
#include "../general/network/requests/join_game_request.hpp"
#include "../general/network/requests/stand_request.hpp"
#include "../general/network/requests/hit_request.hpp"
#include "../general/network/requests/make_bet_request.hpp"


answer_rqst_response* request_handler::handle_request(const client_request* const req) {

    // Prepare variables that are used by every request type
    player* player;
    std::string err;
    game_instance* game_instance_ptr = nullptr;

    // Get common properties of requests
    RequestType type = req->get_type();
    std::string req_id = req->get_req_id();
    std::string game_id = req->get_game_id();
    std::string player_id = req->get_player_id();


    // Switch behavior according to request type
    switch(type) {
        // ##################### JOIN GAME #####################  //
        case RequestType::join_game: {
            std::string player_name = ((join_game_request *) req)->get_player_name();

            //Check if name is already taken
            if(player_manager::does_name_conflict(player_name, player_id)){
              err = "Name " + player_name + " is already taken";
              return new answer_rqst_response("", req_id, false, nullptr, err);
            }
            // Create new player or get existing one with that id
            player_manager::add_or_get_player(player_name, player_id, player);

            if (game_id.empty()) {
                // join any game
                if (game_instance_manager::try_add_player_to_any_game(player, game_instance_ptr, err)) {
                    // game_instance_ptr got updated to the joined game

                    // return response with full game_state attached
                    return new answer_rqst_response(game_instance_ptr->get_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                } else {
                    // failed to find game to join
                    return new answer_rqst_response("", req_id, false, nullptr, err);
                }
            } else {
                // join a specific game denoted by req->get_game_id()
                if (game_instance_manager::try_get_game_instance(game_id, game_instance_ptr)) {
                    if (game_instance_manager::try_add_player(player, game_instance_ptr, err)) {
                        // return response with full game_state attached
                        return new answer_rqst_response(game_id, req_id, true,
                                                    game_instance_ptr->get_game_state()->to_json(), err);
                    } else {
                        // failed to join requested game
                        return new answer_rqst_response("", req_id, false, nullptr, err);
                    }
                } else {
                    // failed to find requested game
                    return new answer_rqst_response("", req_id, false, nullptr, "Requested game could not be found.");
                }
            }
        }


        // ##################### START GAME ##################### //
        case RequestType::start_game: {
            if (game_instance_manager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                if (game_instance_ptr->start_game(player, err)) {
                    // if successful, send all clients the new game state
                    return new answer_rqst_response(game_instance_ptr->get_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                }
            }
            // otherwise, send all clients an error message
            return new answer_rqst_response("", req_id, false, nullptr, err);
        }


        // ##################### MAKE BET ##################### //
        case RequestType::make_bet: {
            if (game_instance_manager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                int bet_size = ((make_bet_request *) req)->get_bet();
                if (game_instance_ptr->make_bet(player, bet_size, err)) {
                    // if successful, send all clients the new game state
                    return new answer_rqst_response(game_instance_ptr->get_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                }
            }
            // otherwise, send all clients an error message
            return new answer_rqst_response("", req_id, false, nullptr, err);
        }


        // ##################### HIT ##################### //
        case RequestType::hit: {
            if (game_instance_manager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                if (game_instance_ptr->hit(player, err)) {
                    // if successful, send all clients the new game state
                    return new answer_rqst_response(game_instance_ptr->get_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                }
            }
            // otherwise, send all clients an error message
            return new answer_rqst_response("", req_id, false, nullptr, err);
        }


        // ##################### STAND ##################### //
        case RequestType::stand: {
             if (game_instance_manager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                if (game_instance_ptr->stand(player, err)) {
                    // if successful, send all clients the new game state
                    return new answer_rqst_response(game_instance_ptr->get_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                }
            }
            // otherwise, send all clients an error message
            return new answer_rqst_response("", req_id, false, nullptr, err);
        }


        // ##################### UNKNOWN REQUEST ##################### //
        default:
            return new answer_rqst_response("", req_id, false, nullptr, "Unknown RequestType " + std::to_string(type)); //That's the best I can come up with
    }
}

bool request_handler::perform_setup_if_needed(const client_request* const req){
    //Could potentially do something meaningful with err before returning
    player* player;
    game_instance* game_instance_ptr = nullptr;
    std::string player_id = req->get_player_id();
    std::string err;

    game_instance_manager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err);

    return game_instance_ptr->perform_setup_if_needed(err);
}

#endif
