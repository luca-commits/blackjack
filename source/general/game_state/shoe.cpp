#include "shoe.hpp"

#include "../serialization/vector_utils.h"
#include "../exceptions/BlackjackException.hpp"

// deserialization constructor
shoe::shoe(std::string id, std::vector<card*> &cards)
        : unique_serializable(id),
          _cards(cards)
{ }

// from_diff constructor
shoe::shoe(std::string id) : unique_serializable(id) { }


shoe::shoe(std::vector<card*> &cards)
        : unique_serializable(), _cards(cards)
{ }


shoe::shoe() : unique_serializable() { }

shoe::~shoe() {
    for (card* & _card : _cards) {
        delete _card;
    }
    _cards.clear();
}

// shuffle the shoe
void shoe::shuffle() {
    std::shuffle(_cards.begin(), _cards.end(), std::mt19937(std::random_device()()));
}

void shoe::setup_round(std::string &err) {
    // remove all cards
    for (int i = 0; i < _cards.size(); i++) {
        delete _cards[i];
    }
    _cards.clear();

    // add a fresh set of cards (4 decks)
    for(int deck = 0; deck < 4; ++deck) {
        for(int suit = 0; suit < 4; ++suit) {
            for(int value = 1; value <= 13; ++value) {
                _cards.push_back(new card(value, suit));
            }
        }
    }

    // shuffle them
    this->shuffle();
}

// draw a card from shoe to given hand
card* shoe::draw_card(hand* h, std::string& err)  {
    if (!_cards.empty()) {
        card* drawn_card = _cards.back();
        bool check = h->add_card(drawn_card, err);
        _cards.pop_back();
        return drawn_card;
    } else {
        err = "Could not draw card because draw pile is empty.";
        return nullptr;
    }
}


void shoe::write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    unique_serializable::write_into_json(json, allocator);
    json.AddMember("cards", vector_utils::serialize_vector(_cards, allocator), allocator);
}


shoe *shoe::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id") && json.HasMember("cards")) {
        std::vector<card*> deserialized_cards = std::vector<card*>();
        for (auto &serialized_card : json["cards"].GetArray()) {
            deserialized_cards.push_back(card::from_json(serialized_card.GetObject()));
        }
        return new shoe(json["id"].GetString(), deserialized_cards);
    } else {
        throw BlackjackException("Could not parse shoe from json. 'id' or 'cards' were missing.");
    }
}
