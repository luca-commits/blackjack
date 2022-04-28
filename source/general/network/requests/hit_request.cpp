#include "hit_request.hpp"

#ifdef BLACKJACK_SERVER
#include "../../../server/game_instance_manager.hpp" //not planned for now ?
#include "../../../server/game_instance.hpp"
#endif

// Public constructor
hit_request::hit_request(std::string game_id, std::string player_id)
        : client_request( client_request::create_base_class_properties(RequestType::hit, uuid_generator::generate_uuid_v4(), player_id, game_id) )
{ }

// private constructor for deserialization
hit_request::hit_request(client_request::base_class_properties props) :
        client_request(props)
{ }

hit_request* hit_request::from_json(const rapidjson::Value& json) {
    return new hit_request(client_request::extract_base_class_properties(json));
}

void hit_request::write_into_json(rapidjson::Value &json,
                                         rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
}
