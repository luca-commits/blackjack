#include "action_request.hpp"

// Public constructor
action_request::action_request(std::string game_id, std::string player_id, std::string action_type)
        : client_request(client_request::create_base_class_properties(RequestType::action, uuid_generator::generate_uuid_v4(), player_id, game_id) ),
        _action_type(action_type)
{ }

// private constructor for deserialization
action_request::action_request(client_request::base_class_properties props, std::string action_type) :
        client_request(props),
        _action_type(action_type)
{ }

action_request* action_request::from_json(const rapidjson::Value& json) {
    base_class_properties props = client_request::extract_base_class_properties(json);
    if (json.HasMember("action_type")) {
        return new action_request(props, json["action_type"].GetString());
    } else {
        throw BlackjackException("Could not find 'action_type' or 'value' in action_request");
    }
}

void action_request::write_into_json(rapidjson::Value &json,
                                        rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
    rapidjson::Value action_type_val(_action_type.c_str(), allocator);
    json.AddMember("action_type", action_type_val, allocator);
}
