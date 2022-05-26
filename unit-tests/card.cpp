//
// Created by Flavia Taras on 24.05.22.
//

#include "gtest/gtest.h"
#include "../source/general/exceptions/BlackjackException.hpp"
#include "../source/general/game_state/card.hpp"
#include "../source/general/serialization/json_utils.h"

// Serialization and subsequent deserialization must yield the same object
TEST(CardTest, SerializationEquality) {
    card card_send(2, 3);
    rapidjson::Document* json_send = card_send.to_json();
    std::string message = json_utils::to_string(json_send);
    delete json_send;

    rapidjson::Document json_received = rapidjson::Document(rapidjson::kObjectType);
    json_received.Parse(message.c_str());
    card* card_received = card::from_json(json_received);
    EXPECT_EQ(card_send.get_id(), card_received->get_id());
    EXPECT_EQ(card_send.get_value(), card_received->get_value());
    EXPECT_EQ(card_send.get_suit(), card_received->get_suit());
    delete card_received;
}

// Deserializing an invalid string must throw a BlackjackException
TEST(CardTest, SerializationException) {
    rapidjson::Document json = rapidjson::Document(rapidjson::kObjectType);
    json.Parse("not json");
    EXPECT_THROW(card::from_json(json), BlackjackException);
}
