#include "hand.hpp"

#include "../exceptions/LamaException.h"
#include "../serialization/vector_utils.h"


hand::hand() : unique_serializable() { }

hand::hand(std::string id) : unique_serializable(id) { }

// deserialization constructor
hand::hand(std::string id, std::vector<card*> cards) : unique_serializable(id) {
    this->_cards = cards;
}

hand::~hand() {
    for (int i = 0; i < _cards.size(); i++) {
        delete _cards.at((i));
        _cards.at(i) = nullptr;
    }
    _cards.clear();
}

int hand::get_nof_cards() const {
    return _cards.size();
}

const std::vector<card*> hand::get_cards() const {
    return _cards;
}

std::vector<card*>::iterator hand::get_card_iterator() {
    return _cards.begin();
}


#ifdef BLACKJACK_SERVER
void hand::setup_round(std::string &err) {
    // remove all cards (if any) and clear it
    for (int i = 0; i < _cards.size(); i++) {
        delete _cards[i];
    }
    _cards.clear();
}

bool hand::add_card(card* new_card, std::string &err) {
    _cards.push_back(new_card);
    return true;
}
#endif

int hand::get_points(std::string &err) {
    int point_sum = 0;
    int ace_counter = 0;

    for(auto card : _cards) {
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
                return 0;
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


bool hand::is_over_21(std::string &err) {
    int points = this->get_points(std::string &err);
    if(points > 21)
        return true;
    else
        return false;
}

void hand::write_into_json(rapidjson::Value &json, rapidjson::Document::AllocatorType& allocator) const {
    unique_serializable::write_into_json(json, allocator);
    json.AddMember("cards", vector_utils::serialize_vector(_cards, allocator), allocator);
}

hand *hand::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id") && json.HasMember("cards")) {
        std::vector<card*> deserialized_cards = std::vector<card*>();
        for (auto &serialized_card : json["cards"].GetArray()) {
            deserialized_cards.push_back(card::from_json(serialized_card.GetObject()));
        }
        return new hand(json["id"].GetString(), deserialized_cards);
    } else {
        throw LamaException("Could not parse hand from json. 'cards' were missing.");
    }
}
