#ifndef GAME_INSTANCE_HPP
#define GAME_INSTANCE_HPP

#include <vector>
#include <string>
#include <mutex>

//adapt the includes!!!!
#include "../common/game_state/player/player.h"
#include "../common/game_state/game_state.h"

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
    //std::string get_id(); probably not needed in our case?

    game_state* get_game_state();

    bool is_full();
    bool is_started();
    bool is_finished();

    // game update functions
    bool start_game(player* player, std::string& err);
    bool add_player(player* new_player, std::string& err);
    bool try_remove_player(player* player, std::string& err);
    bool hit(player* player, std::string& err);
    bool stand(player* player, std::string& err);
    bool split(player* player, std::string& err);
    bool double_down(player* player, std::string& err);
    bool insure(player* player, std::string& err);
};

#endif