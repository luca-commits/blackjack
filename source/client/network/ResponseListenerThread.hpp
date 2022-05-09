#ifndef LAMA_CLIENT_NETWORK_THREAD_HPP
#define LAMA_CLIENT_NETWORK_THREAD_HPP

#include <functional>
#include <wx/wx.h>
#include <sockpp/tcp_socket.h>
#include <sockpp/tcp_connector.h>


class ResponseListenerThread : public wxThread {

public:
    ResponseListenerThread(sockpp::tcp_connector* connection);
    ~ResponseListenerThread();

protected:
    virtual ExitCode Entry();

private:
    void outputError(std::string title, std::string message);


    sockpp::tcp_connector* _connection;

};

#endif
