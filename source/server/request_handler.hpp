#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "../general/network/responses/server_response.hpp"
#include "../general/network/requests/client_request.hpp"
#include "../general/network/responses/answer_rqst_response.hpp"

class request_handler {
public:
    static request_response* handle_request(const client_request* const req);
};

#endif
