#ifndef HIT_REQUEST_HPP
#define HIT_REQUEST_HPP

#include <string>
#include "client_request.hpp"
#include "../../../../rapidjson/include/rapidjson/document.h"

class hit_request : public client_request{

private:

    /*
     * Private constructor for deserialization
     */
    explicit hit_request(base_class_properties);

public:
    hit_request(std::string game_id, std::string player_id);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static hit_request* from_json(const rapidjson::Value& json);
};

#endif
