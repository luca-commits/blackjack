//
// BLACKJACK
//

#include "game_state.hpp"

#include "../exceptions/BlackjackException.hpp"
#include "../serialization/vector_utils.h"


game_state::game_state() : unique_serializable() {
    this->_players = std::vector<player*>();
    this->_shoe = new shoe();
    this->_dealers_hand = std::vector<card*>();
    this->_is_started = new serializable_value<bool>(false);
    this->_is_finished = new serializable_value<bool>(false);
    this->_round_number = new serializable_value<int>(0);
    this->_current_player_idx = new serializable_value<int>(0);
    this->_starting_player_idx = new serializable_value<int>(0);
}

game_state::game_state(std::string id) : unique_serializable(id) {
    this->_players = std::vector<player*>();
    this->_is_started = new serializable_value<bool>(false);
    this->_is_finished = new serializable_value<bool>(false);
    this->_round_number = new serializable_value<int>(0);
    this->_current_player_idx = new serializable_value<int>(0);
    this->_starting_player_idx = new serializable_value<int>(0);
}

//TODO: rearrange stuff so its the same order as the declaration of the member variables in both this and the hpp file
game_state::game_state(std::string id, std::vector<player*>& players, shoe* shoe,
                       std::vector<card*>& dealers_hand, serializable_value<bool>* is_started,
                       serializable_value<bool>* is_finished, serializable_value<int>* current_player_idx,
                       serializable_value<int>* round_number, serializable_value<int>* starting_player_idx))
        : unique_serializable(id),
          _players(players),
          _shoe(shoe),
          _dealers_hand(dealers_hand),
          _is_started(is_started),
          _is_finished(is_finished),
          _round_number(round_number),
          _current_player_idx(current_player_idx),
          _starting_player_idx(starting_player_idx)
{ }

//they also didn't delete the players vector... not sure why (also the only one that was not created with new in the default constructor)
game_state::~game_state() {
    if (_is_started != nullptr) {//de ce facem asta?
        delete _shoe;
        delete _dealers_hand;
        delete _is_started;
        delete _is_finished;
        delete _round_number;
        delete _current_player_idx;
        delete _starting_player_idx;

        _shoe = nullptr;
        _dealers_hand = nullptr;
        _is_started = nullptr;
        _is_finished = nullptr;
        _round_number = nullptr;
        _current_player_idx = nullptr;
        _starting_player_idx = nullptr;
    }
}

// accessors

bool game_state::is_full() const {
    return _players.size() == _max_nof_players;
}

bool game_state::is_started() const {
    return _is_started->get_value();
}

bool game_state::is_finished() const {
    return _is_finished->get_value();
}

bool game_state::is_player_in_game(player* player) const {
    if(std::find(_players.begin(), _players.end(), player) != _players.end())
        return true;
    else
        return false;
}

bool game_state::is_allowed_to_play_now(player* player) const {
    auto it = std::find(_players.begin(), _players.end(), player);
    return it == _players[_current_player_idx->get_value()];
}

int game_state::get_round_number() const {
    return _round_number;
}

std::vector<player*>& game_state::get_players() {
    return _players;
}

int game_state::get_round_number() const {
    return _round_number;
}

int game_state::get_player_index(player* player) const {
    return _current_player_idx;
}

shoe* game_state::get_shoe() const {
    return _shoe;
}

player* game_state::get_current_player() const {
    return _players[_current_player_idx];
}
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
    std::vector<card*> compute_dealers_hand(); // does hardcoded actions for dealer
    bool check_winner(player* player, std::string& err); // checks if player beat the dealer

    // end of round functions
    void update_current_player(std::string& err);
    void wrap_up_round(std::string& err);
#endif


// Serializable interface
static game_state* from_json(const rapidjson::Value& json);
virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
