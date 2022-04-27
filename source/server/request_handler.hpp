#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "../common/network/responses/server_response.hpp"
#include "../common/network/requests/client_request.hpp"
#include "../common/network/responses/answer_rqt_response.hpp"

class request_handler {
public:
    static request_response* handle_request(const client_request* const req);
};

#endif
