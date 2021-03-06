#include "VRTCPServer.h"

#include <boost/asio.hpp>

#include <cstdlib>
#include <iostream>
#include <thread>
#include <string>
#include <memory>

//#ifdef _WINDOWS // TODO
//#include <ws2tcpip.h>
//#endif

using namespace std;
using namespace OSG;
using namespace boost::asio;
using ip::tcp;

class TCPServer {
    private:
        boost::asio::io_service io_service;
        boost::asio::io_service::work worker;
        tcp::socket socket;
        unique_ptr<tcp::acceptor> acceptor;
        thread waiting;
        thread service;
        boost::asio::streambuf buffer;

        function<void (string)> onMessageCb;

        template <typename Itr, typename Out>
        void copy_n(Itr it, size_t count, Out out) {
            for (size_t i=0; i<count; i++) out = *it++;
        }

        void read_handler(const boost::system::error_code& ec, size_t N) {
            if (!ec && N > 7) {
                string data;
                std::istream is(&buffer);
                std::istreambuf_iterator<char> it(is);
                copy_n( it, N-7, std::back_inserter<std::string>(data) );
                for (int i=0; i<7; i++) it++;
                //data += "\n";
                if (onMessageCb) onMessageCb(data);
                serve();
            } else {}
        }

        void serve() {
            boost::asio::async_read_until( socket, buffer, "TCPPVR\n", bind(&TCPServer::read_handler, this, std::placeholders::_1, std::placeholders::_2) );
        }

        void waitFor() {
            acceptor->async_accept(socket, [this](boost::system::error_code ec) { if (!ec) serve(); /*waitFor();*/ });
        }

		void run() {
			io_service.run();
		}

    public:
        TCPServer() : worker(io_service), socket(io_service) {
            service = thread([this](){ run(); });
        }

        ~TCPServer() { close(); }

        void onMessage( function<void (string)> f ) { onMessageCb = f; }

        void listen(int port) {
            if (!acceptor) acceptor = unique_ptr<tcp::acceptor>( new tcp::acceptor(io_service, tcp::endpoint(tcp::v4(), port)) );
            waitFor();
        }

        void close() {
            io_service.stop();
            socket.cancel();
            boost::system::error_code _error_code;
            socket.shutdown(tcp::socket::shutdown_both, _error_code);
            if (service.joinable()) service.join();
        }
};

VRTCPServer::VRTCPServer() { server = new TCPServer(); }
VRTCPServer::~VRTCPServer() { delete server; }

VRTCPServerPtr VRTCPServer::create() { return VRTCPServerPtr(new VRTCPServer()); }

void VRTCPServer::onMessage( function<void (string)> f ) { server->onMessage(f); }
void VRTCPServer::listen(int port) { this->port = port; server->listen(port); }
void VRTCPServer::close() { server->close(); }
int VRTCPServer::getPort() { return port; }

string VRTCPServer::getPublicIP() {
#ifndef _WINDOWS // TODO
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    assert(sock != -1);

    const char* kGoogleDnsIp = "8.8.8.8";
    uint16_t kDnsPort = 53;
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
    serv.sin_port = htons(kDnsPort);

    connect(sock, (const sockaddr*) &serv, sizeof(serv));

    sockaddr_in name;
    socklen_t namelen = sizeof(name);
    getsockname(sock, (sockaddr*) &name, &namelen);

    char addressBuffer[INET_ADDRSTRLEN];

#ifdef _WINDOWS
	inet_ntop(AF_INET, &name.sin_addr, addressBuffer, INET_ADDRSTRLEN);
	closesocket(sock);
#else
	inet_ntop(AF_INET, &name.sin_addr, addressBuffer, INET_ADDRSTRLEN);
	::close(sock);
#endif

    return string(addressBuffer);
#else
	return "";
#endif
}

