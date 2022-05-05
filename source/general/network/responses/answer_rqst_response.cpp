#include "answer_rqst_response.hpp"
#include "../../serialization/json_utils.h"
#include "../../exceptions/BlackjackException.hpp"
#include "../../game_state/game_state.hpp"

#ifdef BLACKJACK_CLIENT
#include "../../../client/GameController.hpp"     //adapt this if needed!
#endif


answer_rqst_response::answer_rqst_response(server_response::base_class_properties props, std::string req_id, bool success, rapidjson::Value* state_json, std::string &err) :
    server_response(props),
    _req_id(req_id),
    _state_json(state_json),
    _success(success),
    _err(err)
{ }

answer_rqst_response::answer_rqst_response(std::string game_id, std::string req_id, bool success, rapidjson::Value* state_json, std::string err):
    server_response(server_response::create_base_class_properties(ResponseType::req_response, game_id)),
    _req_id(req_id),
    _state_json(state_json),
    _success(success),
    _err(err)
{ }


answer_rqst_response::~answer_rqst_response() {
    if (_state_json != nullptr) {
        delete _state_json;
        _state_json = nullptr;
    }
}

void answer_rqst_response::write_into_json(rapidjson::Value &json,
                                       rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    answer_rqst_response::write_into_json(json, allocator);

    rapidjson::Value err_val(_err.c_str(), allocator);
    json.AddMember("err", err_val, allocator);

    rapidjson::Value req_id_val(_req_id.c_str(), allocator);
    json.AddMember("req_id", req_id_val, allocator);

    json.AddMember("success", _success, allocator);

    if (_state_json != nullptr) {
        json.AddMember("state_json", *_state_json, allocator);
    }
}


answer_rqst_response *answer_rqst_response::from_json(const rapidjson::Value& json) {
    if (json.HasMember("err") && json.HasMember("success")) {
        std::string err = json["err"].GetString();

        rapidjson::Value* state_json = nullptr;
        if (json.HasMember("state_json")) {
            state_json = json_utils::clone_value(json["state_json"].GetObject());
        }
        return new answer_rqst_response(
                server_response::extract_base_class_properties(json),
                json["req_id"].GetString(),
                json["success"].GetBool(),
                state_json,
                err);
    } else {
        throw BlackjackException("Could not parse answer_rqst_response from json. err or success is missing.");
    }
}

#ifdef BLACKJACK_CLIENT

void answer_rqst_response::Process() const {
    if (_success) {
        if (this->_state_json != nullptr) {
            game_state* state = game_state::from_json(*_state_json);
            GameController::updateGameState(state);

        } else {
            GameController::showError("Network error", "Expected a state as JSON inside the answer_rqst_response. But there was none.");
        }
    } else {
        GameController::showError("Not possible", _err);
    }
}

#endif