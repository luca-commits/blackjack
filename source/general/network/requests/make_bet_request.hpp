#ifndef MAKE_BET_REQUEST_HPP
#define MAKE_BET_REQUEST_HPP

#include <string>
#include "client_request.hpp"
#include "../../../../rapidjson/include/rapidjson/document.h"

class make_bet_request : public client_request {

private:
    int _bet;

    /*
     * Private constructor for deserialization
     */
    make_bet_request(base_class_properties, int val);

public:

    [[nodiscard]] int get_bet() const { return this->_bet; }

    make_bet_request(std::string game_id, std::string player_id, int bet);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static make_bet_request* from_json(const rapidjson::Value& json);
};

#endif
