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
  do { \
    if (cond) \
      throw ::std::runtime_error( \
          ::std::string("Assertion failed: '") + #cond + "'!\n" + msg + "\n"); \
  } while (0)

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
        ::socketT s,
        ::std::string *obj_id);

    ::Packet::SNMPv2 *__create_pck(
        ::std::string ver,
        ::std::string cmn,
        ::Packet::PDU::Type::TypeE pdu_t,
        ::uint32_t req_id,
        ::Packet::PDU::Error::ErrorsE err,
        ::uint32_t err_idx,
        ::std::string obj_id,
        BinaryVectorT obj_value,
        ::DataTypesE obj_type);

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
        ::Packet::PDU::Error::ErrorsE Error,
        ::uint32_t ErrorIndex,
        ::std::string *ObjectId,
        BinaryVectorT ObjectValue,
        ::DataTypesE ObjectType)
      {
        _obj_id = ObjectId;
        ::__impl_manager::__send(
            ::__impl_manager::__create_pck(
                "1", Community, PDUType, RequestID, Error,
                ErrorIndex, *ObjectId, ObjectValue, ObjectType),
            _sin, _socket);
        return *this;
      }

    /** Returns object filled with SNMPv2 response data */
    ::std::unique_ptr<::Packet::SNMPv2> retrieve()
      {
        return ::std::unique_ptr<::Packet::SNMPv2>(
            ::__impl_manager::__retrieve(_sin, _socket, _obj_id));
      }

    virtual ~Manager()
      {  }

private:
    ::std::string *_obj_id;
    struct ::sockaddr_in _sin;
    struct ::hostent *_hostent;
    socketT _socket;
  };
