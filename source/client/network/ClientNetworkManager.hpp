#ifndef LAMA_CLIENTNETWORKMANAGER_HPP
#define LAMA_CLIENTNETWORKMANAGER_HPP


#include <string>
#include "ResponseListenerThread.hpp"
#include "../../general/network/requests/client_request.hpp"


class ClientNetworkManager {

public:
    static void init(const std::string& host, const uint16_t port);

    static void sendRequest(const client_request& request);

    static void parseResponse(const std::string& message);

private:
    static bool connect(const std::string& host, const uint16_t port);


    static sockpp::tcp_connector* _connection;

    static bool _connectionSuccess;
    static bool _failedToConnect;

};


#endif
