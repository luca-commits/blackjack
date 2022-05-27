#ifndef ANSWER_RQST_RESPONSE_HPP
#define ANSWER_RQST_RESPONSE_HPP

#include <string>
#include "server_response.hpp"


class answer_rqst_response : public server_response {
private:
    bool _success;
    std::string _err;
    std::string _req_id;                        
    rapidjson::Value* _state_json = nullptr;

    answer_rqst_response(base_class_properties props, std::string req_id, bool success, rapidjson::Value* state_json, std::string& err);

public:

    answer_rqst_response(std::string game_id, std::string req_id, bool success, rapidjson::Value* state_json, std::string err);
    ~answer_rqst_response();

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static answer_rqst_response* from_json(const rapidjson::Value& json);

#ifdef BLACKJACK_CLIENT
    virtual void Process() const override;
#endif
};

#endif
