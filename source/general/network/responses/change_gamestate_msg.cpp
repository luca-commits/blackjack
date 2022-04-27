#include "change_gamestate_msg.hpp"

#include "../../exceptions/BlackjackException.hpp"
#include "../../serialization/json_utils.h"

#ifdef LAMA_CLIENT
#include "../../../client/GameController.hpp"       //adapt if necessary
#endif

change_gamestate_msg::change_gamestate_msg(server_response::base_class_properties props, rapidjson::Value* state_json) :
        server_response(props),
        _state_json(state_json)
{ }

change_gamestate_msg::change_gamestate_msg(std::string game_id, const game_state& state) :
        server_response(server_response::create_base_class_properties(ResponseType::change_gamestate, game_id))
{
    this->_state_json = state.to_json();
}


void change_gamestate_msg::write_into_json(rapidjson::Value &json,
                                       rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    server_response::write_into_json(json, allocator);
    json.AddMember("state_json", *_state_json, allocator);
}

change_gamestate_msg *change_gamestate_msg::from_json(const rapidjson::Value& json) {
    if (json.HasMember("state_json")) {
        return new change_gamestate_msg(server_response::extract_base_class_properties(json),
                                       json_utils::clone_value(json["state_json"].GetObject()));
    } else {
        throw BlackjackException("Could not parse change_gamestate_msg from json. state is missing.");
    }
}

change_gamestate_msg::~change_gamestate_msg() {
    if (_state_json != nullptr) {
        delete _state_json;
        _state_json = nullptr;
    }
}

rapidjson::Value* change_gamestate_msg::get_state_json() const {
    return _state_json;
}

#ifdef LAMA_CLIENT

void change_gamestate_msg::Process() const {
    try {
        game_state* state = game_state::from_json(*_state_json);
        GameController::updateGameState(state);                     //do we have this function?

    } catch(std::exception& e) {
        std::cerr << "Failed to extract game_state from change_gamestate_msg" << std::endl
                  << e.what() << std::endl;
    }
}

#endif
