//
// BLACKJACK
//

#include "game_state.hpp"

#include "../exceptions/BlackjackException.hpp"
#include "../serialization/vector_utils.h"


game_state::game_state() : unique_serializable() {
    this->_players = std::vector<player*>();
    this->_shoe = new shoe();
    this->_dealers_hand = new player("Dealer");
    this->_is_started = new serializable_value<bool>(false);
    this->_is_finished = new serializable_value<bool>(false);
    this->_round_number = new serializable_value<int>(0);
    this->_current_player_idx = new serializable_value<int>(0);
    this->_starting_player_idx = new serializable_value<int>(0);
}

game_state::game_state(std::string id) : unique_serializable(id) {
    this->_players = std::vector<player*>();
    this->_shoe = new shoe();
    this->_dealers_hand = std::vector<card*>();
    this->_is_started = new serializable_value<bool>(false);
    this->_is_finished = new serializable_value<bool>(false);
    this->_round_number = new serializable_value<int>(0);
    this->_current_player_idx = new serializable_value<int>(0);
    this->_starting_player_idx = new serializable_value<int>(0);
}

game_state::game_state(std::string id, std::vector<player*>& players, shoe* shoe,
                       std::vector<card*>& dealers_hand, serializable_value<bool>* is_started,
                       serializable_value<bool>* is_finished, serializable_value<int>* round_number,
                       serializable_value<int>* current_player_idx, serializable_value<int>* starting_player_idx)
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
    return *it == _players[_current_player_idx->get_value()];
}

std::vector<player*>& game_state::get_players() {
    return _players;
}

int game_state::get_round_number() const {
    return _round_number->get_value();
}

// emmm pretty sure this is not correct (look at LAMA)
int game_state::get_player_index(player* player) const {
    return _current_player_idx->get_value();
}

shoe* game_state::get_shoe() const {
    return _shoe;
}

player* game_state::get_current_player() const {
    return _players[_current_player_idx->get_value()];
}

#ifdef BLACKJACK_SERVER

// server-side state update functions (same as in LAMA)
void game_state::setup_round(std::string& err) {  // server side initialization (start_round in our SDS)

    // TODO: implement some sort of shoe setup (done)
    _shoe->setup_round(err);

    //update round number
    _round_number->set_value(_round_number->get_value() + 1);

    //setup players
    for (int i = 0; i < _players.size(); i++) {
        _players[i]->setup_round(err);
        card* drawn_card = nullptr;
        _shoe->draw_card(_players[i], err);
        _shoe->draw_card(_players[i], err);
    }

    //initialize the dealer
    _shoe->draw_card(_dealers_hand, err);
    _shoe->draw_card(_dealers_hand, err);

}

//would be better with a linked list for _players
bool game_state::remove_player(player* player, std::string& err) {
    auto idx_it = std::find(_players.begin(), _players.end(), player);
    int idx = idx_it - _players.begin();
    if (idx_it != _players.end()) {
        if (idx < _current_player_idx->get_value()) {
            // reduce current_player_idx if the player who left had a lower index
            _current_player_idx->set_value(_current_player_idx->get_value() - 1);
        }
        _players.erase(idx_it);
        return true;
    } else {
        err = "Could not leave game, as the requested player was not found in that game.";
        return false;
    }
}

bool game_state::add_player(player* player, std::string& err) {
    if (_is_started->get_value()) {
        err = "Could not join game, because the requested game is already started.";
        return false;
    }
    if (_is_finished->get_value()) {
        err = "Could not join game, because the requested game is already finished.";
        return false;
    }
    if (_players.size() >= _max_nof_players) {
        err = "Could not join game, because the max number of players is already reached.";
        return false;
    }
    if (std::find(_players.begin(), _players.end(), player) != _players.end()) {
        err = "Could not join game, because this player is already subscribed to this game.";
        return false;
    }

    _players.push_back(player);
    return true;
}

bool game_state::start_game(std::string& err) {
    if (_players.size() < _min_nof_players) {
        err = "You need at least " + std::to_string(_min_nof_players) + " players to start the game.";
        return false;
    }

    if (!_is_started->get_value()) {
        this->setup_round(err);
        this->_is_started->set_value(true);
        return true;
    } else {
        err = "Could not start game, as the game was already started";
        return false;
    }
}

//TODO: check if legal turn (player asking for hit without being allowed to)
//TODO: if after hit you are over 21, automatically end turn (set _finished_turn to true) and call update_current_player!
bool game_state::hit(player* player, std::string& err) {
    if (!is_player_in_game(player)) {
        err = "Server refused to perform hit. Player is not part of the game.";
        return false;
    }
    if (!is_allowed_to_play_now(player)) {
        err = "It's not this players turn yet.";
        return false;
    }
    if (_is_finished->get_value()) {
        err = "Could not hit, because the requested game is already finished.";
        return false;
    }

    if(player->get_points() < 21) {
        player->hit(_shoe->draw_card(player, err)); //this is wrong for sure (look at what draw_card does)
        return true;
    } else {
        err = "Could not hit since the player already has 21 points or more.";
        return false;
    }
}

//TODO: this should check and set the _finished_turn flag of player and call update_current_player!
bool game_state::stand(player* player, std::string& err) {
    if(player->get_points() < 21) {
        player->stand(err);
        return true;
    }
    else {
        err = "Could not stand since the player already finished his turn.";
        return false;
    }
}

//TODO: other cases to take into account?
bool game_state::make_bet(player* player, int bet_size, std::string& err) {
    if(!player->is_broke()) {
        player->make_bet(bet_size, err);
    } else {
        err = "Player cannot make a bet because they are broke.";
        return false;
    }
}

// functions from our SDS
//TODO: check if any other actions
//TODO: maybe if dealer is player than after we are done hitting for him set _finished_turn to true?
int game_state::compute_dealers_hand(std::string& err) { // does hardcoded actions for dealer
    while(_dealers_hand->get_points(err) <= 16) {
        _dealers_hand->hit(_shoe->draw_card(_dealers_hand, err), err);
    }
    return _dealers_hand->get_points(err);
}
// wrap_up_round of player does the same thing,
// maybe just get rid of this function and in wrap_up_round of game_state call wrap_up_round for each player?
void game_state::check_winner(std::string& err) { // checks if player beat the dealer
    int dealer = compute_dealers_hand(err);
    for(auto player : _players) {
        int pts = player->get_points(err);
        if (pts > 21 || pts < dealer) {
            player->lost_round();
        } else if (pts == dealer) {
            player->draw_round();
        }  else {
            player->won_round();
        }
    }
}

// end of round functions
void game_state::update_current_player(std::string& err) {
    if(_current_player_idx->get_value() + 1 >= _players.size()) {
        wrap_up_round(err);
    } else {
        ++_current_player_idx;
    }
}

//look at where Auswertung of the winners of the round and stuff
void game_state::wrap_up_round(std::string& err) {
    bool is_game_over = false;
    int dealer_points = _dealers_hand->get_points(err);
    for(int i = 0; i < _players.size(); i++) {
        _players[i]->wrap_up_round(dealer_points, err);
    }

    if (_round_number->get_value() >= _max_nof_rounds) {
        // The game ends when the first player reaches 40 points
        is_game_over = true;
    }

    if (is_game_over) {
        this->_is_finished->set_value(true);
    } else {
        // decide which player starts in the next round
        _starting_player_idx->set_value(0);
        // start next round
        setup_round(err);
    }
}
#endif


// Serializable interface
game_state* game_state::from_json(const rapidjson::Value& json);
void game_state::write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
