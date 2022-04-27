#include "player.hpp"

#include "../exceptions/BlackjackException.hpp"

player::player(std::string name) : unique_serializable() {
    this->_player_name = new serializable_value<std::string>(name);
    this->_bet_size = new serializable_value<int>(0);
    this->_money = new serializable_value<int>(100);                    // they get $100 at the beginning of the game!
    this->_has_insurance = new serializable_value<bool>(false);
    this->_has_doubled_down = new serializable_value<bool>(false);
    this->_hand = new std::vector<card*>();
}

player::player(std::string id, serializable_value<std::string>* name, serializable_value<int>* bet_size, \
            serializable_value<int>* money, serializable_value<bool>* has_insurance, \
            serializable_value<bool>* has_doubled_down, std::vector<card*> hand) :
        unique_serializable(id),
        _player_name(name),
        _bet_size(bet_size),
        _money(money),
        _has_insurance(has_insurance),
        _has_doubled_down(has_doubled_down),
        _hand(hand);
{ }

player::~player() {
    if (_player_name != nullptr) {
        delete _hand;
        delete _has_doubled_down;
        delete _has_insurance;
        delete _money;
        delete _bet_size;
        delete _player_name;

        _hand = nullptr;
        _has_doubled_down = nullptr;
        _has_insurance = nullptr;
        _money = nullptr;
        _bet_size = nullptr;
        _player_name = nullptr;
    }
}

#ifdef LAMA_SERVER
player::player(std::string id, std::string name) :
        unique_serializable(id)
{
    this->_player_name = new serializable_value<std::string>(name);
    this->_bet_size = new serializable_value<int>(0);
    this->_money = new serializable_value<int>(100);                    // they get $100 at the beginning of the game!
    this->_has_insurance = new serializable_value<bool>(false);
    this->_has_doubled_down = new serializable_value<bool>(false);
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

bool player::is_insured() const noexcept {
    return this->_has_insurance->get_value();
}

bool player::is_doubled_down() const noexcept {
    return this->_has_doubled_down->get_value();
}

std::vector<card*> player::get_hand() const noexcept {
    return this->_hand;
}

std::string get_player_name() const noexcept {
    return this->_player_name->get_value();
}