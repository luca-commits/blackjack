#include "game_instance.hpp"

#include "server_network_manager.hpp"
#include "../general/network/responses/change_gamestate_msg.hpp"


game_instance::game_instance() {
    _game_state = new game_state();
}

game_state *game_instance::get_game_state() {
    return _game_state;
}

std::string game_instance::get_id() {
    return _game_state->get_id();
}

bool game_instance::is_player_allowed_to_play(player *player) {
    return _game_state->is_allowed_to_play_now(player);
}

bool game_instance::is_full() {
    return _game_state->is_full();
}

bool game_instance::is_started() {
    return _game_state->is_started();
}

bool game_instance::is_finished() {
    return _game_state->is_finished();
}

bool game_instance::start_game(player* player, std::string &err) {
    modification_lock.lock();
    if (_game_state->start_game(err)) { // ADAPT THIS LINE!
        // send state update to all other players
        change_gamestate_msg state_update_msg = change_gamestate_msg(this->get_id(), *_game_state);
        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}

bool game_instance::try_add_player(player *new_player, std::string &err) {
    modification_lock.lock();
    if (_game_state->add_player(new_player, err)) { // ADAPT THIS LINE!
        new_player->set_game_id(get_id());
        // send state update to all other players
        change_gamestate_msg state_update_msg = change_gamestate_msg(this->get_id(), *_game_state);
        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), new_player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}

bool game_instance::try_remove_player(player *player, std::string &err) {
    modification_lock.lock();
    if (_game_state->remove_player(player, err)) { // ADAPT THIS LINE!
        player->set_game_id("");
        // send state update to all other players
        change_gamestate_msg state_update_msg = change_gamestate_msg(this->get_id(), *_game_state);
        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}

bool game_instance::hit(player* player, std::string& err) {
    modification_lock.lock();
    if (_game_state->hit(player, err)) { // ADAPT THIS LINE!
        // send state update to all other players
        change_gamestate_msg state_update_msg = change_gamestate_msg(this->get_id(), *_game_state);
        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}

bool game_instance::stand(player* player, std::string& err) {
    modification_lock.lock();
    if (_game_state->stand(player, err)) { // ADAPT THIS LINE!
        // send state update to all other players
        change_gamestate_msg state_update_msg = change_gamestate_msg(this->get_id(), *_game_state);
        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}

bool game_instance::make_bet(player *player, const int& bet_size, std::string& err) {
    modification_lock.lock();
    if (_game_state->make_bet(player, bet_size, err)) { // ADAPT THIS LINE!
        change_gamestate_msg state_update_msg = change_gamestate_msg(this->get_id(), *_game_state);
        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}

bool game_instance::perform_setup_if_needed(std::string& err){
    if(_game_state->needs_setup){
        modification_lock.lock();
        _game_state->setup_round(err);
        change_gamestate_msg state_update_msg = change_gamestate_msg(this->get_id(), *_game_state);
        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), nullptr);
        modification_lock.unlock();
        return true;
    }
    else{
        return false;
    }
}
