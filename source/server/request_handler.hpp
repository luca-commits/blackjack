#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "../general/network/responses/server_response.hpp"
#include "../general/network/requests/client_request.hpp"
#include "../general/network/responses/answer_rqst_response.hpp"

class request_handler {
public:
    static answer_rqst_response* handle_request(const client_request* const req);
    static bool perform_setup_if_needed(const client_request* const req);
};

#endif
