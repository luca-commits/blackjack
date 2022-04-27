#ifndef CHANGE_GAMESTATE_MSG_HPP
#define CHANGE_GAMESTATE_MSG_HPP

#include "server_response.hpp"
#include "../../game_state/game_state.hpp" //adapt this potentially ?

class change_gamestate_msg : public server_response {
private:
    rapidjson::Value* _state_json;  //game_state in SDS, not changing

    /*
     * Private constructor for deserialization
     */
    change_gamestate_msg(base_class_properties props, rapidjson::Value* state_json);

public:

    change_gamestate_msg(std::string game_id, const game_state& state);
    ~change_gamestate_msg();

    rapidjson::Value* get_state_json() const;

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static change_gamestate_msg* from_json(const rapidjson::Value& json);

#ifdef BLACKJACK_CLIENT
    virtual void Process() const override;
#endif
};

#endif
