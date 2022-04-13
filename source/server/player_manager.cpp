#include "player_manager.hpp"

// Initialize static map
std::unordered_map<std::string, player*> player_manager::_players_lut = {};

bool player_manager::get_player(const std::string& player_id, player *&player_ptr) {
    player_ptr = nullptr;
    _rw_lock.lock_shared();
    auto it = player_manager::_players_lut.find(player_id);
    if (it != _players_lut.end()) {
        player_ptr = it->second;
    }
    _rw_lock.unlock_shared();
    return player_ptr;
}

bool player_manager::add_player(std::string name, const std::string& player_id, player *&player_ptr) {
    if (get_player(player_id, player_ptr)) {
        return true;
    }
    player_ptr = new player(player_id, name);
    _rw_lock.lock();
    player_manager::_players_lut.insert({player_id, player_ptr});
    _rw_lock.unlock();
    return true;
}

bool player_manager::remove_player(const std::string& player_id, player *&player) {
    if (get_player(player_id, player)) {
        _rw_lock.lock();
        int nof_removals = player_manager::_players_lut.erase(player_id);
        _rw_lock.unlock();
        return true;
    }
    return false;
}
