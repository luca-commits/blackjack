#include "player_manager.hpp"

// Initialize static map
std::unordered_map<std::string, player*> player_manager::_players_lut = {};

bool player_manager::does_name_conflict(const std::string name, const std::string& player_id){
    //Check if the player_id already exists
    _rw_lock.lock_shared();
    auto it = player_manager::_players_lut.find(player_id);
    if(it != _players_lut.end()){
      _rw_lock.unlock_shared();
      return false; //Player is already in game, no special action required
    }
    _rw_lock.unlock_shared();
    //Check if name is already taken by existing player
    bool name_taken = false;
    _rw_lock.lock_shared();
    std::for_each(_players_lut.begin(), _players_lut.end(), [&name_taken, &name](auto p){name_taken |= (p.second->get_player_name() == name);});
    _rw_lock.unlock_shared();
    return name_taken;
}

bool player_manager::try_get_player(const std::string& player_id, player *&player_ptr) {
    player_ptr = nullptr;
    _rw_lock.lock_shared();
    auto it = player_manager::_players_lut.find(player_id);
    if (it != _players_lut.end()) {
        player_ptr = it->second;
    }
    _rw_lock.unlock_shared();
    return player_ptr;
}

bool player_manager::add_or_get_player(std::string name, const std::string& player_id, player *&player_ptr) {
    if (try_get_player(player_id, player_ptr)) {
        return true;
    }
    player_ptr = new player(player_id, name);
    _rw_lock.lock();    // exclusive
    player_manager::_players_lut.insert({player_id, player_ptr});
    _rw_lock.unlock();
    return true;
}

bool player_manager::remove_player(const std::string& player_id, player *&player) {
    if (try_get_player(player_id, player)) {
        _rw_lock.lock();    // exclusive
        int nof_removals = player_manager::_players_lut.erase(player_id);
        _rw_lock.unlock();
        return true;
    }
    return false;
}
