#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

//include the correct responses!!!!!
#include "../common/network/responses/server_response.h"
#include "../common/network/requests/client_request.h"
#include "../common/network/responses/request_response.h"

class request_handler {
public:
    static request_response* handle_request(const client_request* const req);
};

#endif
