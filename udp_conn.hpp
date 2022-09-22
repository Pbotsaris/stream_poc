#ifndef UDP_CONN_H
#define UDP_CONN_H

#include <SDL2/SDL_types.h>

#include "connection.hpp"
#include <iostream>

class UDPConn : public Connection {

public:
  UDPConn(int port, std::string &&address) : Connection(port, SOCK_DGRAM) {
     bind_socket(address);

  }

  bool bind_socket(std::string &address) {

    bool valid = setup(address);

    if (!is_valid(valid, "Could not setup UDP connection.")) {
      return valid;
    }

    auto addr = get_address();

    valid = bind(get_socket(), reinterpret_cast<struct sockaddr *>(&addr),
                 sizeof(address));

    return is_valid(valid, "Could not bind socket in UDP connection.");
  }

  bool send_out(const std::string &&address, const std::string &&port,
                Uint8 *buffer, int len) {



    sockaddr_in addr_in = Connection::to_sockaddr_in(port, address);

    int res =
        sendto(get_socket(), buffer, len, 0,
               reinterpret_cast<struct sockaddr *>(&addr_in), sizeof(addr_in));

  std::cout << "send -> " << res << std::endl;

    return is_valid(res, "UDPTextIO could not send.");
  }

private:
  Connection m_conn;
};

#endif
