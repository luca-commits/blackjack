#ifndef GAME_INSTANCE_HPP
#define GAME_INSTANCE_HPP

#include <vector>
#include <string>
#include <mutex>

#include "../general/game_state/player/player.hpp"
#include "../general/game_state/game_state.hpp"

class game_instance {
private:
    game_state* _game_state;
    bool is_player_allowed_to_play(player* player);
    inline static std::mutex modification_lock;
public:
    game_instance();
    ~game_instance() {
        if (_game_state != nullptr) {
            delete _game_state;
        }
        _game_state = nullptr;
    }
    std::string get_id();

    game_state* get_game_state();

    bool is_full();
    bool is_started();
    bool is_finished();

    // game update functions
    bool start_game(player* player, std::string& err);
    bool try_add_player(player* new_player, std::string& err);
    bool try_remove_player(player* player, std::string& err);
    bool hit(player* player, std::string& err);
    bool stand(player* player, std::string& err);
    bool make_bet(player* player, const int& bet_size, std::string& err);
};

#endif