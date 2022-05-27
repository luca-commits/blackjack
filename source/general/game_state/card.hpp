#ifndef CARD_HPP
#define CARD_HPP

#include <string>
#include "../serialization/unique_serializable.h"
#include "../serialization/serializable_value.h"
#include "../../../rapidjson/include/rapidjson/document.h"

class card : public unique_serializable {
private:
    serializable_value<int>* _value;        // represents figure on card: 'A' - 1, '2' - 2, ... , '10' - 10, 'J' - 11, 'Q' - 12, 'K' - 13
    serializable_value<int>* _suit;         // '0' for spades, '1' for clubs, '2' for diamonds, '3' for hearts

    // from_diff constructor
    card(std::string id);                   
    // deserialization constructor
    card(std::string id, serializable_value<int>* val, serializable_value<int>* suit);
public:
    card(int val, int suit);
    ~card();

// accessors
    int get_value() const noexcept;
    int get_suit() const noexcept;

// serializable interface
    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static card* from_json(const rapidjson::Value& json);
};


#endif