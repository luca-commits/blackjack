#ifndef PLAYER_MANAGER_HPP
#define PLAYER_MANAGER_HPP

#include <string>
#include <shared_mutex>
#include <unordered_map>

#include "../general/game_state/player.hpp" //adapt this line to where player.hpp will be

class player_manager {

private:

    inline static std::shared_mutex _rw_lock;
    static std::unordered_map<std::string, player*> _players_lut;

public:
    static bool does_name_conflict(const std::string name, const std::string& player_id);
    static bool try_get_player(const std::string& player_id, player*& player_ptr);
    static bool add_or_get_player(std::string name, const std::string& player_id, player*& player_ptr);
    static bool remove_player(const std::string& player_id, player*& player);
};


#endif
