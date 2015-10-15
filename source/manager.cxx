/**
 * @file manager.cxx
 * @brief
 * @author Miroslav Cibulka - xcibul10
 * @details
 */

#include <iostream>
#include <fcntl.h>
#include <thread>
#include "../include/manager.hxx"
#include "../include/log.hxx"


static ::std::string *s_obj_id;

namespace __crt_bnd_lst
  {
      using namespace ::Packet::PDU::Bindings;

      BindingList *helper(
          BindingList *ret)
        { return ret; }

      template<
          typename BindingT,
          typename ...BindingsT>
        BindingList *helper(
            BindingList *ret,
            BindingT &&bind,
            BindingsT &&...binds)
          {
            ret->push_back(bind);
            return helper(ret, binds...);
          }
  }

::Packet::PDU::Bindings::Object *create_bind(
    ::std::string obj_id,
    BinaryVectorT obj_value,
    ::DataTypesE obj_type)
  {
    using namespace ::Packet::PDU::Bindings;
    return new Object(obj_id, obj_value, obj_type);
  }

template<
    typename ...BindingsT>
  ::Packet::PDU::Bindings::BindingList *create_binding_list(
      BindingsT &&...binds)
    {
      using namespace ::Packet::PDU::Bindings;
      return ::__crt_bnd_lst::helper(new BindingList(), binds...);
    }

::Packet::PDU::SNMPv2 *create_pdu(
    ::Packet::PDU::Type::TypeE pdu_t,
    ::uint32_t req_id,
    ::Packet::PDU::Error::ErrorsE non_rep,
    ::uint32_t max_rep,
    ::Packet::PDU::Bindings::BindingList *_bind)
  {
    using namespace ::Packet::PDU;
    SNMPv2 *_pdu = new SNMPv2(new Type(pdu_t));
    _pdu->push_back(new RequestID(req_id));
    _pdu->push_back(new Error(non_rep));
    _pdu->push_back(new ErrorIndex(max_rep));
    _pdu->push_back(_bind);
    return _pdu;
  }

::Packet::SNMPv2 *create_snmp(
    ::std::string ver,
    ::std::string cmn)
  {
    using namespace ::Packet;
    SNMPv2 *_snmp = new SNMPv2();
    _snmp->push_back(new Version(ver));
    _snmp->push_back(new CommunityString(cmn));
    return _snmp;
  }

::Packet::SNMPv2 *create_pck(
    ::Packet::SNMPv2 *_snmp,
    ::Packet::PDU::SNMPv2 *_pdu)
  {
    _snmp->push_back(_pdu);
    return _snmp;
  }

::__impl_manager::__impl_manager(
    ::std::string addr,
    uint16_t port,
    socketT &_socket,
    struct ::sockaddr_in &_sin,
    struct ::hostent *&_hostent)
  {
    MAN_ASSERT(
        (_socket = ::socket(
            PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0,
               "Failed to create socket");
    ::memset((char *) &_sin, 0, sizeof(_sin));
    _sin.sin_family = AF_INET;
    _sin.sin_port = ::htons(static_cast<uint8_t>(port));
    MAN_ASSERT((_hostent = ::gethostbyname(addr.c_str())) == nullptr,
               "Host have not been recognized (or internal error)");
    MAN_ASSERT((::memcpy(
        static_cast<void *>(&_sin.sin_addr), _hostent->h_addr,
        static_cast<size_t>(_hostent->h_length))) == nullptr,
        "Memcopy failed");
  }

::__impl_manager &::__impl_manager::__send(
    ::Packet::SNMPv2 *pck,
    ::sockaddr_in sock_out,
    ::socketT s)
  {
    auto binary = pck->getBinary();
    auto str = ::BinToStr(binary);
    MAN_ASSERT((sendto(s, binary.data(), binary.size(), 0,
                       (::sockaddr *) &sock_out, sizeof(::sockaddr)) < 0),
               ::std::string("Failed to send message:\n") + str + "\n" +
               pck->getStrRepre());
    delete pck;
    return *this;
  }

namespace DECODE
  {
      namespace PDU
        {
            namespace BINDS
              {
                  using namespace ::Packet::PDU::Bindings;

                  BinaryVectorT obj_value(
                      BinaryVectorT &msg,
                      ::DataTypesE &type)
                    {
                      type = (::DataTypesE) msg[0];
                      size_t size = msg[1];
                      BinaryVectorT ret(
                          msg.begin() + 2, msg.begin() + size + 2);
                      msg.erase(msg.begin(), msg.begin() + size + 2);
                      return ret;
                    }

                  ::std::string obj_name(
                      BinaryVectorT &msg)
                    {
                      size_t size = msg[1];
                      BinaryVectorT temp(
                          msg.begin() + 2, msg.begin() + size + 2);
                      msg.erase(msg.begin(), msg.begin() + size + 2);
                      *s_obj_id = ::decodeObjectName(temp);
                      return *s_obj_id;
                    }

                  Object *bind(
                      BinaryVectorT &msg)
                    {
                      msg.erase(msg.begin(), msg.begin() + 2);
                      ::DataTypesE type;
                      return new Object(
                          obj_name(msg), obj_value(msg, type), type);
                    }

                  BindingList * decode(
                      BinaryVectorT &msg)
                    {
                      msg.erase(msg.begin(), msg.begin() + 2);
                      BindingList *bindings = new BindingList();
                      bindings->push_back(bind(msg));
                      return bindings;
                    }
              }

            using namespace ::Packet::PDU;

            ErrorIndex *err_idx(
                BinaryVectorT &msg)
              {
                size_t size = msg[1];
                ByteT indx = msg[size + 1];
                msg.erase(msg.begin(), msg.begin() + 2 + size);
                return new ErrorIndex(indx);
              }

            Error *error(
                BinaryVectorT &msg)
              {
                size_t size = msg[1];
                ByteT etype = msg[size + 1];
                msg.erase(msg.begin(), msg.begin() + 2 + size);
                return new Error((Error::ErrorsE) etype);
              }

            RequestID *request(
                BinaryVectorT &msg)
              {
                size_t size = msg[1];
                /// !!!!!!!!!!! TODO: rewrite this!
                BinaryVectorT req(
                    msg.begin() + 2, msg.begin() + size + 2);
                uint32_t ret = 0;
                for (auto d: req)
                  ret = (ret << 8) | d;
                msg.erase(msg.begin(), msg.begin() + 2 + size);
                return new RequestID(ret);
              }

            SNMPv2 *decode(
                BinaryVectorT &msg)
              {
                msg.erase(msg.begin(), msg.begin() + 2);
                SNMPv2 *pdu = new SNMPv2(new Type(Type::_byte_type[msg[2]]));
                pdu->push_back(request(msg));
                pdu->push_back(error(msg));
                pdu->push_back(err_idx(msg));
                pdu->push_back(BINDS::decode(msg));
                return pdu;
              }
        }
      using namespace ::Packet;

      CommunityString *community(
          BinaryVectorT &msg)
        {
          size_t size = msg[1];
          ::std::string ret(msg.begin() + 2, msg.begin() + 2 + size);
          msg.erase(msg.begin(), msg.begin() + size + 2);
          return new CommunityString(ret);
        }

      Version *version(
          BinaryVectorT &msg)
        {
          size_t size = msg[1];
          ::std::string ver = ::std::to_string(msg[size + 1]);
          msg.erase(msg.begin(), msg.begin() + size + 2);
          return new Version(ver);
        }

      /**
       * @brief Decode message - SNMPv2 object representation will be returned
       * @param msg raw data
       */
      SNMPv2 *decode(
          BinaryVectorT msg)
        {
          if (msg.empty()) return nullptr;
          SNMPv2 *snmp = new SNMPv2();
          msg.erase(msg.begin(), msg.begin() + 2);
          snmp->push_back(version(msg));
          snmp->push_back(community(msg));
          snmp->push_back(PDU::decode(msg));
          return snmp;
        }
  }

/**
 * @brief reads whole message
 * @param s socket
 * @return vector of read bytes
 */
BinaryVectorT readall(
    ::socketT s)
  {
    BinaryVectorT message;
    ByteT *buff = new ByteT[128];
    ssize_t read;
    auto flags = fcntl(s, F_GETFL, 0);
    fcntl(s, F_SETFL, flags | O_NONBLOCK);

    /*
     * Read from socket until there is nothing to read.
     */
    int counter = 500;
    while ((read = ::recv(s, (void *) buff, 128, 0)) >= 128 || read < 0)
      {
        if (read < 0)
          /* Non blocking solution requires to try again after some time */
          if (((errno == EAGAIN) || (errno == EWOULDBLOCK)) && counter)
            {
              ::std::this_thread::sleep_for(
                  ::std::chrono::milliseconds(5));
              --counter;
            }
          else
            {
              throw ::std::runtime_error(
                  strerror(errno));
            }
        else
          ::std::copy(buff, buff + read, ::std::back_inserter(message));
      }
    if (read > 0)
      ::std::copy(buff, buff + read, ::std::back_inserter(message));
    delete[](buff);
    return message;
  }

::Packet::SNMPv2 *::__impl_manager::__retrieve(
    ::sockaddr_in,
    ::socketT s,
    ::std::string *obj_id)
  {
    using namespace ::Packet::PDU::Bindings;
    using namespace ::Packet::PDU;
    s_obj_id = obj_id;
    return ::DECODE::decode(::readall(s));
  }

::Packet::SNMPv2 *::__impl_manager::__create_pck(
    ::std::string ver,
    ::std::string cmn,
    ::Packet::PDU::Type::TypeE pdu_t,
    ::uint32_t req_id,
    ::Packet::PDU::Error::ErrorsE err,
    ::uint32_t err_idx,
    ::std::string obj_id,
    BinaryVectorT obj_value,
    ::DataTypesE obj_type)
  {
    return ::create_pck(
        ::create_snmp(ver, cmn),
        ::create_pdu(
            pdu_t, req_id, err, err_idx,
            ::create_binding_list(
                ::create_bind(obj_id, obj_value, obj_type))));
  }

__impl_manager::~__impl_manager()
  { }