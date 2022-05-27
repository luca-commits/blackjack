#include "player.hpp"

#include "../exceptions/BlackjackException.hpp"

player::player(std::string name) : unique_serializable() {
    this->_player_name = new serializable_value<std::string>(name);
    this->_bet_size = new serializable_value<int>(0);
    this->_money = new serializable_value<int>(100);                    // they get $100 at the beginning of the game!
    this->_finished_turn = new serializable_value<bool>(false);
    this->_player_hand = new hand();
}

player::player(std::string id, serializable_value<std::string>* name, serializable_value<int>* bet_size, \
            serializable_value<int>* money, serializable_value<bool>* finished_turn, hand* player_hand) :
        unique_serializable(id),
        _player_name(name),
        _bet_size(bet_size),
        _money(money),
        _finished_turn(finished_turn),
        _player_hand(player_hand)
{ }

player::~player() {
    if (_player_name != nullptr) {
        delete _player_hand;
        delete _bet_size;
        delete _money;
        delete _finished_turn;
        delete _player_name;

        _player_hand = nullptr;
        _bet_size = nullptr;
        _money = nullptr;
        _finished_turn = nullptr;
        _player_name = nullptr;
    }
}

#ifdef BLACKJACK_SERVER
player::player(std::string id, std::string name) :
        unique_serializable(id)
{
    this->_player_name = new serializable_value<std::string>(name);
    this->_bet_size = new serializable_value<int>(0);
    this->_money = new serializable_value<int>(100);                    // they get $100 at the beginning of the game!
    this->_finished_turn = new serializable_value<bool>(false);
    this->_player_hand = new hand();
}

std::string player::get_game_id() {
    return _game_id;
}

void player::set_game_id(std::string game_id) {
    _game_id = game_id;
}
#endif


int player::get_bet_size() const noexcept {
    return this->_bet_size->get_value();
}

int player::get_money() const noexcept {
    return this->_money->get_value();
}

bool player::has_finished_turn() const noexcept {
    return this->_finished_turn->get_value();
}

hand* player::get_hand() const noexcept {
    return this->_player_hand;
}

std::string player::get_player_name() const noexcept {
    return this->_player_name->get_value();
}

void player::set_finished_turn() {
    this->_finished_turn->set_value(true);
}

bool player::is_broke() {
    return _money->get_value() + _bet_size->get_value() <= 0;
}

#ifdef BLACKJACK_SERVER

void player::setup_round(std::string& err) {
    _bet_size->set_value(0);
    _finished_turn->set_value(false);
    _player_hand->setup_round(err);
}

// Based on how many points the dealer has, chooses to call won_round or draw_round.
// We do not need lose_round since the bet will just not be returned to the player
// and it is set to 0 in setup_round().
void player::wrap_up_round(int dealer_points, std::string& err) {
    int player_points = _player_hand->get_points(err);
    if(player_points > 21 || (player_points < dealer_points && dealer_points <= 21)){
        return;
    }
    if(player_points > dealer_points || dealer_points > 21) {
        this->won_round();
        return;
    }
    if(player_points == dealer_points) {
        this->draw_round();
        return;
    } else {
        err = "Something went wrong during wrapping round for player " + this->_player_name->get_value();
    }
}


bool player::make_bet(int bet_size, std::string &err) {
    if(bet_size > this->get_money()) {
        err = "bet_size is bigger than amount of money the player " + this->_player_name->get_value() + " has.";
        return false;
    } else if (bet_size <= 0) {
        err = "Player " + this->_player_name->get_value() + " tried to place a negative (or no) bet.";
        return false;
    } else if (this->has_finished_turn()) {
        err = "Player " + this->_player_name->get_value() + " has already finished his turn and cannot make bets.";
        return false;
    }

    _bet_size->set_value(bet_size);
    _money->set_value(this->get_money() - bet_size);
    return true;
}

void player::won_round() {
    int winnings = this->get_bet_size();
    int holdings = this->get_money();
    _money->set_value(holdings + 2 * winnings);
}

void player::draw_round() {
    int winnings = this->get_bet_size();
    int holdings = this->get_money();
    _money->set_value(holdings + winnings);
}

#endif

void player::write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const {
    unique_serializable::write_into_json(json, allocator);

    rapidjson::Value id_val(_id.c_str(), allocator);
    json.AddMember("id", id_val, allocator);

    rapidjson::Value name_val(rapidjson::kObjectType);
    _player_name->write_into_json(name_val, allocator);
    json.AddMember("player_name", name_val, allocator);

    rapidjson::Value bet_size_val(rapidjson::kObjectType);
    _bet_size->write_into_json(bet_size_val, allocator);
    json.AddMember("bet_size", bet_size_val, allocator);

    rapidjson::Value money_val(rapidjson::kObjectType);
    _money->write_into_json(money_val, allocator);
    json.AddMember("money", money_val, allocator);

    rapidjson::Value finished_val(rapidjson::kObjectType);
    _finished_turn->write_into_json(finished_val, allocator);
    json.AddMember("finished_turn", finished_val, allocator);

    rapidjson::Value hand_val(rapidjson::kObjectType);
    _player_hand->write_into_json(hand_val, allocator);
    json.AddMember("player_hand", hand_val, allocator);
}

player *player::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id")
        && json.HasMember("player_name")
        && json.HasMember("bet_size")
        && json.HasMember("money")
        && json.HasMember("finished_turn")
        && json.HasMember("player_hand"))
    {
        return new player(
                json["id"].GetString(),
                serializable_value<std::string>::from_json(json["player_name"].GetObject()),
                serializable_value<int>::from_json(json["bet_size"].GetObject()),
                serializable_value<int>::from_json(json["money"].GetObject()),
                serializable_value<bool>::from_json(json["finished_turn"].GetObject()),
                hand::from_json(json["player_hand"].GetObject()));
    } else {
        throw BlackjackException("Failed to deserialize player from json. Required json entries were missing.");
    }
}
