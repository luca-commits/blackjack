#include "card.hpp"

#include <utility>

#include "../exceptions/BlackjackException.hpp"

card::card(std::string id) : unique_serializable(std::move(id)) { }

card::card(std::string id, serializable_value<int> *val, serializable_value<int>* suit)
        : unique_serializable(std::move(id)), _value(val), _suit(suit)
{ }

card::card(int val, int suit) :
        unique_serializable(),
        _value(new serializable_value<int>(val)),
        _suit(new serializable_value<int>(suit))
{ }

card::~card() { }


int card::get_value() const noexcept {
    return _value->get_value();
}

int card::get_suit() const noexcept {
    return _suit->get_value();
}

card *card::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id") && json.HasMember("value") && json.HasMember("suit")) {
        return new card(json["id"].GetString(), serializable_value<int>::from_json(json["value"].GetObject()), \
        serializable_value<int>::from_json(json["suit"].GetObject()));
    } else {
        throw BlackjackException("Could not parse json of card. Was missing 'id', 'value' or 'suit'.");
    }
}


void card::write_into_json(rapidjson::Value &json, rapidjson::Document::AllocatorType& allocator) const {
    unique_serializable::write_into_json(json, allocator);

    rapidjson::Value value_val(rapidjson::kObjectType);
    _value->write_into_json(value_val, allocator);
    json.AddMember("value", value_val, allocator);

    rapidjson::Value suit_val(rapidjson::kObjectType);
    _suit->write_into_json(suit_val, allocator);
    json.AddMember("suit", suit_val, allocator);
}

