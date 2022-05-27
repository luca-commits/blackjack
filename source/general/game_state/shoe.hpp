#ifndef SHOE_HPP
#define SHOE_HPP

#include "card.hpp"
#include "player.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include "../serialization/serializable.h"
#include "../serialization/unique_serializable.h"
#include "../serialization/serializable_value.h"
#include "../../../rapidjson/include/rapidjson/document.h"


class shoe : public unique_serializable {
private:
    std::vector<card*> _cards;

    /*
     * Deserialization constructor
     */
    shoe(std::string id, std::vector<card*>& cards);

    // from_diff constructor
    shoe(std::string id);

public:
    // constructors
    shoe();
    shoe(std::vector<card*>& cards);
    ~shoe();

    // pile functions
    void shuffle();
    bool is_empty() const noexcept;
    void setup_round(std::string &err);
    card* draw_card(hand* h, std::string& err);

    // serialization
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static shoe* from_json(const rapidjson::Value& json);
};


#endif
