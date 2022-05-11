#include "player.hpp"

#include "../exceptions/BlackjackException.hpp"

player::player(std::string name) : unique_serializable() {
    this->_player_name = new serializable_value<std::string>(name);
    this->_bet_size = new serializable_value<int>(0);
    this->_money = new serializable_value<int>(100);                    // they get $100 at the beginning of the game!
    this->_finished_turn = new serializable_value<bool>(false);
    this->_hand = new std::vector<card*>();
}

player::player(std::string id, serializable_value<std::string>* name, serializable_value<int>* bet_size, \
            serializable_value<int>* money, serializable_value<bool>* finished_turn, std::vector<card*> hand) :
        unique_serializable(id),
        _player_name(name),
        _bet_size(bet_size),
        _money(money),
        _finished_turn(finished_turn),
        _hand(hand)
{ }

player::~player() {
    if (_player_name != nullptr) {
        delete _hand;
        delete _bet_size;
        delete _money;
        delete _finished_turn;
        delete _player_name;

        _hand = nullptr;
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
    this->_hand = new std::vector<card*>();
}

std::string player::get_game_id() {
    return _game_id;
}

void player::set_game_id(std::string game_id) {
    _game_id = game_id;
}
#endif


int player::get_nof_cards() const noexcept {
    return _hand.size();
}

int player::get_bet_size() const noexcept {
    return this->_bet_size->get_value();
}

int player::get_money() const noexcept {
    return this->_money->get_value();
}

bool player::has_finished_turn() const noexcept {
    return this->_finished_turn->get_value();
}

std::vector<card*> player::get_hand() const noexcept {
    return this->_hand;
}

std::string player::get_player_name() const noexcept {
    return this->_player_name->get_value();
}

#ifdef BLACKJACK_SERVER

void player::add_card(card* card) {
    _hand.push_back(card);
}

// should make_bet be already called here ?
void player::setup_round(std::string& err) {
    _bet_size->set_value(0);
    _finished_turn->set_value(false);
    for (int i = 0; i < _hand.size(); i++) {
        delete _hand[i];
    }
    _hand.clear();
}

// based on how many points the dealer has, chooses to call won_round, lost_round or draw round
void player::wrap_up_round(int dealer_points, std::string& err) {
    int player_points = this->get_points(err);
    if(player_points > dealer_points) {
        this->won_round();
    } else if(player_points = dealer_points) {
        this->draw_round();
    } else if(player_points < dealer_points) {
        this->lost_round();
    } else {
        err = "Something went wrong during wrapping round for player " + this->_player_name->get_value();
    }
}

/*
// possibly not needed?
bool player::hit(card *card, std::string &err) {
    if (this->has_finished_turn()) {
        err = "Player " + this->_player_name->get_value() + " has already finished his turn and cannot hit.";
        return false;
    } 

    this->add_card(card);
    return true;
}

// possibly not needed?
bool player::stand(std::string &err) {
    if (this->has_finished_turn()) {
        err = "Player " + this->_player_name->get_value() + " has already finished his turn and cannot stand.";
        return false
    } 

    _finished_turn->set_value(true);
    return true;
}

// possibly not needed?
bool player::make_bet(int bet_size, std::string &err) {
    // add check if player already has a bet to throw error?
    if(bet_size > this->get_money()) {
        err = "bet_size is bigger than amount of money the player " + this->player_name + " has.";
        return false;
    } else if (bet_size <= 0) {
        err = "Player " + this->_player_name->get_value() + " tried to place a negative (or no) bet.";
        return false;
    } else if (this->has_finished_turn()) {
        err = "Player " + this->_player_name->get_value() + " has already finished his turn and cannot make bets.";
        return false
    }
        
    int holdings = this->get_money();
    _bet_size->set_value(bet_size);
    _money->set_value(holdings - bet_size);
    return true;
}
*/

//alternative: keep track in a member variable while drawing
int player::get_points(std::string &err) {
    int point_sum = 0;
    int ace_counter = 0;

    for(auto card : _hand) {
        int value = card->get_value();
        switch(value):
            //ace
            case 1:
                ace_counter += 1;
                break;
            //numeric cards
            case 2:
                point_sum += 2;
                break;
            case 3:
                point_sum += 3;
                break;
            case 4:
                point_sum += 4;
                break;
            case 5:
                point_sum += 5;
                break;
            case 6:
                point_sum += 6;
                break;
            case 7:
                point_sum += 7;
                break;
            case 8:
                point_sum += 8;
                break;
            case 9:
                point_sum += 8;
                break;
            case 10:
                point_sum += 10;
                break;
            //face cards
            case 11:
                point_sum += 10;
                break;
            case 12:
                point_sum += 10;
                break;
            case 13:
                point_sum += 10;
                break;
            default:
                // throws error
                std::string errstr = "Invalid card with value " + std::to_string(value) + " encountered.";
                err = errstr;
                break;
    }

    if(ace_counter == 1) {
        if(point_sum + 11 <= 21) {
            point_sum += 11;
        } else {
            point_sum += 1;
        }
    } else {
        if(point_sum + 11 <= 21 - (ace_counter - 1)) {
            point_sum = point_sum + 11 + (ace_counter - 1);
        } else {
            point_sum += ace_counter;
        }
    }

    return point_sum;
}

bool player::is_broke() {
    if(_money->get_value() <= 0)
        return true;
    else
        return false;
}

//this does not seem to be used at all
bool player::is_over_21() {
    int points = this->get_points();
    if(points > 21)
        return true;
    else
        return false;
}

void player::won_round() {
    int winnings = this->get_bet_size();
    int holdings = this->get_money();
    _money->set_value(holdings + 2 * winnings);
    _bet_size->set_value(0);
}

void player::lost_round() {
    _bet_size->set_value(0);
}

void player::draw_round() {
    int winnings = this->get_bet_size();
    int holdings = this->get_money();
    _money->set_value(holdings + winnings);
    _bet_size->set_value(0);
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

    // no idea if this will work
    rapidjson::Value hand_val(rapidjson::kObjectType);
    _hand->write_into_json(hand_val, allocator);
    json.AddMember("hand", vector_utils::serialize_vector(_hand, allocator), allocator);
}

player *player::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id")
        && json.HasMember("player_name")
        && json.HasMember("bet_size")
        && json.HasMember("money")
        && json.HasMember("finished_turn")
        && json.HasMember("hand"))
    {
        std::vector<card*> deserialized_hand = std::vector<card*>();
        for (auto &serialized_card : json["hand"].GetArray()) {
            deserialized_hand.push_back(card::from_json(serialized_card.GetObject()));
        }
        return new player(
                json["id"].GetString(),
                serializable_value<std::string>::from_json(json["player_name"].GetObject()),
                serializable_value<int>::from_json(json["bet_size"].GetObject()),
                serializable_value<int>::from_json(json["money"].GetObject()),
                serializable_value<bool>::from_json(json["finished_turn"].GetObject()),
                deserialized_hand); // will this work ? I have no idea
    } else {
        throw BlackjackException("Failed to deserialize player from json. Required json entries were missing.");
    }
}
