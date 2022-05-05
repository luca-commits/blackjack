#include "server_response.hpp"
#include "answer_rqst_response.hpp"
#include "change_gamestate_msg.hpp"

#include "../../exceptions/BlackjackException.hpp"

// for deserialization
const std::unordered_map<std::string, ResponseType> server_response::_string_to_response_type = {
        {"req_response", ResponseType::req_response},
        {"state_diff_msg", ResponseType::state_diff_msg}, //is this even needed ? not in SDS !
        {"change_gamestate", ResponseType::change_gamestate}
};
// for serialization
const std::unordered_map<ResponseType, std::string> server_response::_response_type_to_string = {
        { ResponseType::req_response, "req_response"},
        { ResponseType::state_diff_msg, "state_diff_msg"},
        { ResponseType::change_gamestate, "change_gamestate"}
};

server_response::server_response(server_response::base_class_properties params):
        _type(params.type),
        _game_id(params.game_id)
{ }

ResponseType server_response::get_type() const {
    return this->_type;
}

std::string server_response::get_game_id() const {
    return this->_game_id;
}


server_response::base_class_properties
server_response::create_base_class_properties(ResponseType type, const std::string &game_id) {
    server_response::base_class_properties params;
    params.type = type;
    params.game_id = game_id;
    return params;
}

server_response::base_class_properties server_response::extract_base_class_properties(const rapidjson::Value& json) {
    if (json.HasMember("type") && json.HasMember("game_id")) {
        std::string game_id = json["game_id"].GetString();
        return create_base_class_properties(
                server_response::_string_to_response_type.at(json["type"].GetString()),
                game_id
        );
    }
    else
    {
        throw BlackjackException("Server Response did not contain game_id");
    }
}


server_response *server_response::from_json(const rapidjson::Value& json) {

    if (json.HasMember("type") && json["type"].IsString()) {
        std::string type = json["type"].GetString();
        ResponseType response_type = server_response::_string_to_response_type.at(type);

        if (response_type == ResponseType::req_response) {
            return answer_rqst_response::from_json(json);                   //not sure if correct
        }
        else if (response_type == ResponseType::change_gamestate) {
            return change_gamestate_msg::from_json(json);                   //not sure if correct
        } else {
            throw BlackjackException("Encountered unknown ServerResponse type " + response_type);   //should response_type not be string?
        }
    }
    throw BlackjackException("Could not determine type of ClientRequest");
}

void server_response::write_into_json(rapidjson::Value &json,
                                      rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    rapidjson::Value type_val(_response_type_to_string.at(this->_type).c_str(), allocator);
    json.AddMember("type", type_val, allocator);

    rapidjson::Value game_id_val(_game_id.c_str(), allocator);
    json.AddMember("game_id", game_id_val, allocator);
}