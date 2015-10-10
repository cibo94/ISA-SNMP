/**
 * @file packet.hxx
 * @brief
 * @author Miroslav Cibulka - xcibul10
 * @details
 */

#pragma once

#include "packet.hxx"
#include "pdu.hxx"
#include "pdu_bindings.hxx"


extern "C" {
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
typedef int socketT;
}

#define MAN_ASSERT(cond, msg) \
  { \
    if (cond) \
      throw ::std::runtime_error( \
          ::std::string("Assertion failed: '") + #cond + "'!\n" + msg + "\n"); \
  }

class __impl_manager
  {
protected:
    __impl_manager(
        ::std::string addr,
        uint16_t port,
        socketT &_socket,
        struct ::sockaddr_in &_sin,
        struct ::hostent *&_hostent);

    __impl_manager &__send(
        ::Packet::SNMPv2 *pck,
        ::sockaddr_in sockaddr,
        ::socketT s);

    ::Packet::SNMPv2 *__retrieve(
        ::sockaddr_in sockaddr,
        ::socketT s);

    ::Packet::SNMPv2 *__create_pck(
        ::std::string ver,
        ::std::string cmn,
        ::Packet::PDU::Type::TypeE pdu_t,
        ::uint32_t req_id,
        ::uint32_t non_rep,
        ::uint32_t max_rep);

    virtual ~__impl_manager();
  };

/**
 * @brief Container
 */
class Manager :
    private __impl_manager
  {
public:

    Manager(
        ::std::string addr,
        uint16_t port) :
        __impl_manager(addr, port, _socket, _sin, _hostent)
      { }

    Manager &send(
        ::std::string Community,
        ::Packet::PDU::Type::TypeE PDUType,
        ::uint32_t RequestID,
        ::uint32_t NonRepeaters,
        ::uint32_t MaxRepeaters)
      {
        ::__impl_manager::__send(
            ::__impl_manager::__create_pck(
                "2", Community, PDUType, RequestID, NonRepeaters, MaxRepeaters),
            _sin, _socket);
        return *this;
      }

    ::std::unique_ptr<::Packet::SNMPv2> retrieve()
      {
        return ::std::unique_ptr<::Packet::SNMPv2>(
            ::__impl_manager::__retrieve(_sin, _socket));
      }

    virtual ~Manager()
      { MAN_ASSERT(close(_socket) < 0, "Failed to close socket"); }

private:

    struct ::sockaddr_in _sin;
    struct ::hostent *_hostent;
    socketT _socket;
  };
