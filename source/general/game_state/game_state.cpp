//
// BLACKJACK
//

#include "game_state.hpp"

#include "../exceptions/BlackjackException.hpp"
#include "../serialization/vector_utils.h"
#include <iostream>


game_state::game_state() : unique_serializable() {
    this->_players = std::vector<player*>();
    this->_shoe = new shoe();
    this->_dealers_hand = new hand();
    this->_is_started = new serializable_value<bool>(false);
    this->_is_finished = new serializable_value<bool>(false);
    this->_round_number = new serializable_value<int>(0);
    this->_current_player_idx = new serializable_value<int>(0);
    this->_starting_player_idx = new serializable_value<int>(0);
}

game_state::game_state(std::string id) : unique_serializable(id) {
    this->_players = std::vector<player*>();
    this->_shoe = new shoe();
    this->_dealers_hand = new hand();
    this->_is_started = new serializable_value<bool>(false);
    this->_is_finished = new serializable_value<bool>(false);
    this->_round_number = new serializable_value<int>(0);
    this->_current_player_idx = new serializable_value<int>(0);
    this->_starting_player_idx = new serializable_value<int>(0);
}

game_state::game_state(std::string id, std::vector<player*>& players, shoe* shoe,
                       hand* dealers_hand, serializable_value<bool>* is_started,
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
    if (_is_started != nullptr) {
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
    return std::find(_players.begin(), _players.end(), player) < _players.end();
}

bool game_state::is_allowed_to_play_now(player* player) const {
    return player == get_current_player() && !player->has_finished_turn();
}

std::vector<player*>& game_state::get_players() {
    return _players;
}

int game_state::get_round_number() const {
    return _round_number->get_value();
}

int game_state::get_player_index(player* player) const {
    auto it = std::find(_players.begin(), _players.end(), player);
    if (it == _players.end()) {
        return -1;
    } else {
        return it - _players.begin();
    }
}

shoe* game_state::get_shoe() const {
    return _shoe;
}

player* game_state::get_current_player() const {
    if(_current_player_idx == nullptr || _players.size() == 0) {
        return nullptr;
    }
    return _players[_current_player_idx->get_value()];
}

hand* game_state::get_dealers_hand() const {
    return _dealers_hand;
}

bool game_state::everyone_finished() const {
    unsigned int s = _players.size();
    for(unsigned int i = 0; i < s; ++i) {
        if(!(_players[i]->has_finished_turn()))
            return false;
    }
    return true;
}

bool game_state::round_begin() const {
    unsigned int s = _players.size();
    for(unsigned int i = 0; i < s; ++i) {
        if(!(_players[i]->get_bet_size() == 0))
            return false;
    }
    return true;
}

#ifdef BLACKJACK_SERVER

// server-side state update functions (same as in LAMA)
void game_state::setup_round(std::string& err) {  // server side initialization (start_round in our SDS)

    _shoe->setup_round(err);

    //update round number and set current player to starting player
    _round_number->set_value(_round_number->get_value() + 1);
    _current_player_idx->set_value(_starting_player_idx->get_value());

    //setup players
    for (auto player : _players) {
        player->setup_round(err);
        //TODO: how do we get the instream for the amount of the bet
        // I think we do not need to do anything because make_bet request will call make_bet for each player!
        _shoe->draw_card(player->get_hand(), err);
        _shoe->draw_card(player->get_hand(), err);
    }

    //initialize the dealer
    _dealers_hand->setup_round(err);
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

    if(player->get_hand()->get_points(err) < 21) {
        _shoe->draw_card(player->get_hand(), err);
        if(player->get_hand()->get_points(err) >= 21) {
            player->set_finished_turn();
            update_current_player(err);
        }
        return true;
    } else {
        err = "Could not hit since the player already has 21 points or more.";
        return false;
    }
}

bool game_state::stand(player* player, std::string& err) {
    if (!is_player_in_game(player)) {
        err = "Server refused to perform stand. Player is not part of the game.";
        return false;
    }
    else if (!is_allowed_to_play_now(player)) {
        err = "It's not this player's turn yet.";
        return false;
    }
    else if (_is_finished->get_value()) {
        err = "Could not stand, because the requested game is already finished.";
        return false;
    }
    else if (player->has_finished_turn()) {
        err = "Player " + player->get_player_name() + " has already finished their turn.";
        return false;
    }
    else if(player->get_hand()->get_points(err) <= 21) {
        player->set_finished_turn();
        std::cout << std::endl << "idx before update: " << _current_player_idx->get_value() << std::endl;
        update_current_player(err);
        std::cout << std::endl << "idx after update: " << _current_player_idx->get_value() << std::endl;
        return true;
    }
    else{
      err = "Could not stand, unknown error.";
      return false;
    }
}

//TODO: makes bets for everyone before the people actually start the round
//TODO: other cases to take into account?
bool game_state::make_bet(player* player, int bet_size, std::string& err) {
    if(!player->is_broke()) {
        player->make_bet(bet_size, err);
        return true;
    } else {
        err = "Player cannot make a bet because they are broke.";
        return false;
    }
}

// functions from our SDS
void game_state::update_current_player(std::string& err) {
    int current_player_idx = _current_player_idx->get_value();
    if(current_player_idx + 1 >= _players.size()) {
        wrap_up_round(err);
    } else {
        ++current_player_idx;
        _current_player_idx->set_value(current_player_idx);
    }
}

// end of round functions
void game_state::wrap_up_round(std::string& err) {
    //hardcoded dealer action
    while(_dealers_hand->get_points(err) <= 16) {
        _shoe->draw_card(_dealers_hand, err);
    }

    int dealer_points = _dealers_hand->get_points(err);

    for(auto player : _players) {
        player->wrap_up_round(dealer_points, err);
    }

    if (_round_number->get_value() >= _max_nof_rounds) {
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
game_state* game_state::from_json(const rapidjson::Value& json) {
    if (json.HasMember("id")
        && json.HasMember("players")
        && json.HasMember("shoe")
        && json.HasMember("dealers_hand")
        && json.HasMember("is_started")
        && json.HasMember("is_finished")
        && json.HasMember("round_number")
        && json.HasMember("current_player_idx")
        && json.HasMember("starting_player_idx"))
    {
        std::vector<player*> deserialized_players;
        for (auto &serialized_player : json["players"].GetArray()) {
            deserialized_players.push_back(player::from_json(serialized_player.GetObject()));
        }

        return new game_state(json["id"].GetString(),
                              deserialized_players,
                              shoe::from_json(json["shoe"].GetObject()),
                              hand::from_json(json["dealers_hand"].GetObject()),
                              serializable_value<bool>::from_json(json["is_started"].GetObject()),
                              serializable_value<bool>::from_json(json["is_finished"].GetObject()),
                              serializable_value<int>::from_json(json["round_number"].GetObject()),
                              serializable_value<int>::from_json(json["current_player_idx"].GetObject()),
                              serializable_value<int>::from_json(json["starting_player_idx"].GetObject()));


    } else {
        throw BlackjackException("Failed to deserialize game_state. Required entries were missing.");
    }
}
void game_state::write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const {
    unique_serializable::write_into_json(json, allocator);

    rapidjson::Value is_started_val(rapidjson::kObjectType);
    _is_started->write_into_json(is_started_val, allocator);
    json.AddMember("is_started", is_started_val, allocator);

    rapidjson::Value is_finished_val(rapidjson::kObjectType);
    _is_finished->write_into_json(is_finished_val, allocator);
    json.AddMember("is_finished", is_finished_val, allocator);

    rapidjson::Value round_number_val(rapidjson::kObjectType);
    _round_number->write_into_json(round_number_val, allocator);
    json.AddMember("round_number", round_number_val, allocator);

    std::cout << std::endl << "idx: " << _current_player_idx->get_value() << std::endl;
    rapidjson::Value current_player_idx_val(rapidjson::kObjectType);
    _current_player_idx->write_into_json(current_player_idx_val, allocator);
    json.AddMember("current_player_idx", current_player_idx_val, allocator);

    rapidjson::Value starting_player_idx_val(rapidjson::kObjectType);
    _starting_player_idx->write_into_json(starting_player_idx_val, allocator);
    json.AddMember("starting_player_idx", starting_player_idx_val, allocator);

    rapidjson::Value shoe_val(rapidjson::kObjectType);
    _shoe->write_into_json(shoe_val, allocator);
    json.AddMember("shoe", shoe_val, allocator);

    rapidjson::Value dealers_hand_val(rapidjson::kObjectType);
    _dealers_hand->write_into_json(dealers_hand_val, allocator);
    json.AddMember("dealers_hand", dealers_hand_val, allocator);

    json.AddMember("players", vector_utils::serialize_vector(_players, allocator), allocator);
}
