#ifndef BLACKJACK_HAND_HPP
#define BLACKJACK_HAND_HPP

#include <vector>
#include "../../../rapidjson/include/rapidjson/document.h"
#include "card.hpp"

class hand : public unique_serializable {

private:
    std::vector<card*> _cards;

    hand(std::string id);
    hand(std::string id, std::vector<card*> cards);

public:
    hand();
    ~hand();


// serializable interface
    static hand* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

// accessors
    int get_nof_cards() const;
    const std::vector<card*> get_cards() const;

#ifdef BLACKJACK_SERVER
// state update functions
    void setup_round(std::string& err);
    bool add_card(card* card, std::string& err);
#endif

    int get_points(std::string &err);
    bool is_over_21(std::string &err);
    std::vector<card*>::iterator get_card_iterator();
};


#endif //BLACKJACK_HAND_HPP
