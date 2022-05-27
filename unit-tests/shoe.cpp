//
// Created by Flavia Taras on 24.05.22.
//

#include "gtest/gtest.h"
#include "../source/general/game_state/shoe.hpp"
#include "../source/general/exceptions/BlackjackException.hpp"
#include "../source/general/serialization/json_utils.h"

std::vector<card*> get_cards(const shoe& shoe) {
    return shoe._cards;
}

class ShoeTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        cards.resize(224);
        for (int i = 0; i < 4; ++i) { //number of decks in a shoe
            for (int j = 1; j < 14; ++j) { //card values
                for (int k = 0; k < 4; ++k) { //card suits
                    cards.push_back(new card(j, k));
                }
            }
        }

    }

    /* Any object and subroutine declared here can be accessed in the tests */
    shoe shoe_;
    std::vector<card*> cards;
    std::string err;
};


// Serialization and subsequent deserialization must yield the same object
TEST_F(ShoeTest, SerializationEquality) {
    rapidjson::Document* json_send = shoe_.to_json();
    std::string message = json_utils::to_string(json_send);
    delete json_send;

    rapidjson::Document json_received = rapidjson::Document(rapidjson::kObjectType);
    json_received.Parse(message.c_str());
    shoe* shoe_received = shoe::from_json(json_received);
    EXPECT_EQ(get_cards(shoe_), get_cards(*shoe_received));
    delete shoe_received;
}

// Deserializing an invalid string must throw a BlackjackException
TEST_F(ShoeTest, SerializationException) {
    rapidjson::Document json = rapidjson::Document(rapidjson::kObjectType);
    json.Parse("not json");
    EXPECT_THROW(shoe::from_json(json), BlackjackException);
}