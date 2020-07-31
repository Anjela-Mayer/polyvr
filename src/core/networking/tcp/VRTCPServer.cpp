#include "VRTCPServer.h"

#include <boost/asio.hpp>

#include <cstdlib>
#include <iostream>
#include <thread>
#include <string>
#include <memory>

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

    public:
        TCPServer() : worker(io_service), socket(io_service) {
            service = thread([this](){ io_service.run(); });
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
            socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, _error_code);
            if (service.joinable()) service.join();
        }
};

VRTCPServer::VRTCPServer() { server = new TCPServer(); }
VRTCPServer::~VRTCPServer() { delete server; }

VRTCPServerPtr VRTCPServer::create() { return VRTCPServerPtr(new VRTCPServer()); }

void VRTCPServer::onMessage( function<void (string)> f ) { server->onMessage(f); }
void VRTCPServer::listen(int port) { server->listen(port); }
void VRTCPServer::close() { server->close(); }
