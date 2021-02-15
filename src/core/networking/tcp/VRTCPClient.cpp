#include "VRTCPClient.h"

#include <boost/asio.hpp>

#include <cstdlib>
#include <iostream>
#include <thread>
#include <string>
#include <memory>
#include <list>

using namespace OSG;
using namespace boost::asio;
using ip::tcp;

class TCPClient {
    private:
        boost::asio::io_service io_service;
        boost::asio::io_service::work worker;
        tcp::socket socket;
        list<string> messages;
        thread service;

        vector<boost::asio::ip::tcp::endpoint> uriToEndpoints(const string& uri) {
            boost::asio::ip::tcp::resolver resolver(io_service);
            boost::asio::ip::tcp::resolver::query query(uri, "");
            vector<boost::asio::ip::tcp::endpoint> res;
            for(boost::asio::ip::tcp::resolver::iterator i = resolver.resolve(query); i != boost::asio::ip::tcp::resolver::iterator(); ++i) {
                res.push_back(*i);
            }
            return res;
        }

        void processQueue() {
            boost::asio::async_write(socket, boost::asio::buffer(messages.front().data(), messages.front().length()),
                                    [this](boost::system::error_code ec, size_t N) {
                    if (!ec) {
                        messages.pop_front();
                        if (!messages.empty()) processQueue();
                    } else {
                        cout << " tcp client write ERROR: " << ec << "  N: " << N << endl;
                        socket.close();
                    }
                });
        }

		void run() {
			io_service.run();
		}

        void acceptHolePunching() {
            cout << "VRTCPClient::acceptHolePunching" << endl;
            bool stop = false;
            unsigned short port = 11111;
            boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), port);
            boost::asio::io_service ios;
            boost::asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
            boost::system::error_code ec;
            acceptor.bind(ep, ec); //    s.bind(('', port))

            //Handling Errors
            if (ec != 0) {
                std::cout << "Failed to bind the acceptor socket." << "Error code = " << ec.value() << ". Message: " << ec.message() << endl;
            }
            boost::asio::ip::tcp::socket sock(ios);//    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            boost::asio::socket_base::reuse_address reuseAddress(true);
            acceptor.set_option(reuseAddress); //    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            boost::asio::detail::socket_option::boolean<SOL_SOCKET, SO_REUSEPORT> reusePort;
            acceptor.set_option(reusePort); //    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)

            acceptor.listen(1); //    s.listen(1)
        //    s.settimeout(5)
            while (!stop) {  //    while not STOP.is_set():
                bool exception_caught = true;
                try {  //        try:
                    acceptor.accept(sock); //            conn, addr = s.accept()
                }
                catch (boost::system::error_code e) { //        except socket.timeout:
                    cout << "Exception at VRSyncConnection::connect2. Exception Nr. " << e.message() << endl;
                    continue; //            continue
                }
                if (!exception_caught) { //        else:
                    stop = true; //            STOP.set()
                }
            }
        }

        void connectHolePunching(string localIP, string remoteIP) {
            cout << "VRTCPClient::connectHolePunching" << endl;
            bool stop = false;
            unsigned short port = 11111;
            auto localAddr = boost::asio::ip::address::from_string(localIP);
            boost::asio::ip::tcp::endpoint local_ep(localAddr, port);
            boost::asio::io_service ios;
            boost::asio::ip::tcp::acceptor acceptor(ios, local_ep.protocol());
            boost::system::error_code ec;

            acceptor.bind(local_ep, ec); //    s.bind(local_addr)

                        //Handling Errors
            if (ec != 0) {
                std::cout << "Failed to bind the acceptor socket." << "Error code = " << ec.value() << ". Message: " << ec.message() << endl;
            }
            boost::asio::ip::tcp::socket sock(ios);//    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            boost::asio::socket_base::reuse_address reuseAddress(true);
            acceptor.set_option(reuseAddress); //    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            boost::asio::detail::socket_option::boolean<SOL_SOCKET, SO_REUSEPORT> reusePort;
            acceptor.set_option(reusePort); //    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)

            auto remoteAddr = boost::asio::ip::address::from_string(remoteIP);
            boost::asio::ip::tcp::endpoint remote_ep(remoteAddr, port);

            while (!stop) { //while not STOP.is_set():
                bool exception_caught = true;
                try {//        try:
                    sock.connect(remote_ep);//            s.connect(addr)
                    exception_caught = false;
                }
                catch (boost::system::error_code e) {//        except socket.error:
                    cout << "Exception at VRSyncConnection::connect2. Exception Nr. " << e.message() << endl;
                    continue;//            continue
                }
                if (!exception_caught) {//        else:
                    stop = true;//            STOP.set()
                }
            }
        }

    public:
        TCPClient() : worker(io_service), socket(io_service) {
			service = thread([this]() { run(); });
        }

        ~TCPClient() { close(); }

        void close() {
            io_service.stop();
            socket.cancel();
            boost::system::error_code _error_code;
            socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, _error_code);
            if (service.joinable()) service.join();
        }

        void connect(string host, int port) {
            socket.connect( tcp::endpoint( boost::asio::ip::address::from_string(host), port ));
        }

        void tcpHolePunching(string localIP, string remoteIP) {
            //TODO: multi-threading
            acceptHolePunching();
            connectHolePunching(localIP, remoteIP);
        }

        void connect(string uri) {
            socket.connect( uriToEndpoints(uri)[0] );
        }

        void send(string msg) {
            msg += "TCPPVR\n";
            bool write_in_progress = !messages.empty();
            messages.push_back(msg);
            if (!write_in_progress) processQueue();
        }

        bool connected() {
            return socket.is_open();
        }
};


VRTCPClient::VRTCPClient() { client = new TCPClient(); }
VRTCPClient::~VRTCPClient() { delete client; }

VRTCPClientPtr VRTCPClient::create() { return VRTCPClientPtr(new VRTCPClient()); }

void VRTCPClient::connect(string host, int port) { client->connect(host, port); }
void VRTCPClient::connect(string host) { client->connect(host); }
void VRTCPClient::send(const string& message) { client->send(message); }
bool VRTCPClient::connected() { return client->connected(); }
void VRTCPClient::tcpHolePunching(string localIP, string remoteIP) { client->tcpHolePunching(localIP, remoteIP); }

