#ifndef ACTION_REQUEST_HPP
#define ACTION_REQUEST_HPP

#include "client_request.hpp"
#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"

class action_request : public client_request {

private:

    std::string _action_type;

    //Private constructor for deserialization
    action_request(base_class_properties, std::string action_type);

public:
    [[nodiscard]] std::string get_action_type() const { return this->_action_type; }

    action_request(std::string game_id, std::string player_id, std::string action_type);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static play_card_request* from_json(const rapidjson::Value& json);
};

#endif
