#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <vector>
#include <string>
#include "../../../rapidjson/include/rapidjson/document.h"
#include "player.hpp"
#include "shoe.hpp"
#include "../serialization/serializable.h"
#include "../serialization/serializable_value.h"
#include "../serialization/unique_serializable.h"

class game_state : public unique_serializable {
private:

    static const int _max_nof_rounds = 100;
    static const int _max_nof_players = 5;
    static const int _min_nof_players = 2;
    static const int _min_bet = 1;

    std::vector<player*> _players;
    shoe* _shoe;
    player* _dealers_hand;
    serializable_value<bool>* _is_started;
    serializable_value<bool>* _is_finished;
    serializable_value<int>* _round_number;
    serializable_value<int>* _current_player_idx;
    serializable_value<int>* _starting_player_idx;

    // from_diff constructor
    game_state(std::string id);

    // deserialization constructor
    game_state(
            std::string id,
            std::vector<player*>& players,
            shoe* _shoe,
            std::vector<card*>& dealers_hand,
            serializable_value<bool>* is_started,
            serializable_value<bool>* is_finished,
            serializable_value<int>* round_number,
            serializable_value<int>* current_player_idx,
            serializable_value<int>* starting_player_idx);

    // returns the index of 'player' in the '_players' vector
    int get_player_index(player* player) const;
public:
    game_state();
    ~game_state();

// accessors
    bool is_full() const;
    bool is_started() const;
    bool is_finished() const;
    bool is_player_in_game(player* player) const;
    bool is_allowed_to_play_now(player* player) const;
    std::vector<player*>& get_players();
    int get_round_number() const;

    shoe* get_shoe() const;
    player* get_current_player() const;

#ifdef BLACKJACK_SERVER
    // server-side state update functions (same as in LAMA)
    void setup_round(std::string& err);   // server side initialization (start_round in our SDS)
    bool remove_player(player* player, std::string& err);
    bool add_player(player* player, std::string& err);
    bool start_game(std::string& err);
    bool hit(player* player, std::string& err);
    bool stand(player* player, std::string& err);
    bool make_bet(player* player, int bet_size, std::string& err);

    // functions from our SDS
    int compute_dealers_hand(); // does hardcoded actions for dealer
    void check_winner(); // checks if player beat the dealer

    // end of round functions
    void update_current_player(std::string& err);
    void wrap_up_round(std::string& err);
#endif

// serializable interface
    static game_state* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

};

#endif