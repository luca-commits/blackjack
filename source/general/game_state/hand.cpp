#include "hand.hpp"

#include "../exceptions/BlackjackException.hpp"
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

int hand::get_points(std::string &err) {
    int point_sum = 0;
    int ace_counter = 0;

    for (auto card : _cards) {
        int value = card->get_value();
        if (value == 1) {
            ++ace_counter;
        } else if (2 <= value && value <= 13) {
            point_sum += std::min(value, 10);
        } else {
            // throws error
            err = "Invalid card with value " + std::to_string(value) + " encountered.";
            return 0;
        }
    }

    int result = 0;
    //This assumes either all aces count 11 or all aces count 1
    if (point_sum + 11*ace_counter <= 21){
      result = point_sum + 11*ace_counter;
    }
    else{
      result = point_sum + ace_counter;
    }

    return result;
}


bool hand::is_over_21(std::string &err) {
    int points = this->get_points(err);
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
        throw BlackjackException("Could not parse hand from json. 'cards' were missing.");
    }
}
