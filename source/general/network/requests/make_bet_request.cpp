#include "make_bet_request.hpp"

#ifdef BLACKJACK_SERVER
#include "../../../server/game_instance_manager.hpp" // not planned for now ?
#include "../../../server/game_instance.hpp"
#endif

// Public constructor
make_bet_request::make_bet_request(std::string game_id, std::string player_id, int bet)
        : client_request( client_request::create_base_class_properties(RequestType::make_bet, uuid_generator::generate_uuid_v4(), player_id, game_id) )
{
    _bet = bet;
}

// private constructor for deserialization
make_bet_request::make_bet_request(client_request::base_class_properties props, int bet) :
        client_request(props),
        _bet(bet)
{ }

make_bet_request* make_bet_request::from_json(const rapidjson::Value &json) {
    base_class_properties props = client_request::extract_base_class_properties(json);
    if (json.HasMember("bet") ) {
        return new make_bet_request(props, json["bet"].GetInt());
    } else {
        throw LamaException("Could not find 'bet' in make_bet_request");
    }
}

void make_bet_request::write_into_json(rapidjson::Value &json,
                                        rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
    json.AddMember("bet", rapidjson::Value(this->_bet),allocator);
}
