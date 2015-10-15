/**
 * @file packet.cxx
 * @brief
 * @author Miroslav Cibulka - xcibul10
 * @details
 */

#include <iostream>
#include "../include/packet.hxx"
#include "../include/pdu.hxx"
#include "../include/pdu_bindings.hxx"
#include "../include/log.hxx"


namespace Packet
  {
      /* Types of SNMPv2 packets */
      BinaryVectorT PDU::Type::_type_byte {0xA0, 0xA1, 0xA2, 0xA3};
      ::std::unordered_map<ByteT, PDU::Type::TypeE> PDU::Type::_byte_type {
          {0xA0, PDU::Type::GET_REQ},
          {0xA1, PDU::Type::GET_NEXT_REQ},
          {0xA2, PDU::Type::GET_RESP},
          {0xA3, PDU::Type::SET_REQ}
      };
      enum if_types :
          ByteT
        {
          ifIndex = 0,
          ifDescr,
          ifType,
          ifMtu,
          ifSpeed,
          ifPhysAddress,
          ifAdminStatus,
          ifOperStatus,
          ifLastChange,
          ifInOctets,
          ifInUcastPkts,
          ifInNUcastPkts,
          ifInDiscards,
          ifInErrors,
          ifInUnknownProtos,
          ifOutOctets,
          ifOutUcastPkts,
          ifOutNUcastPkts,
          ifOutDiscards,
          ifOutErrors,
          ifOutQLen,
          ifSpecific
        };
      ::std::unordered_map<
          ::std::string, ::std::string> PDU::Bindings::Object::_decoded = {
              {"1.3.6.1.2.1.2.2.1.1", "ifIndex"},
              {"1.3.6.1.2.1.2.2.1.2", "ifDescr"},
              {"1.3.6.1.2.1.2.2.1.3", "ifType"},
              {"1.3.6.1.2.1.2.2.1.4", "ifMtu"},
              {"1.3.6.1.2.1.2.2.1.5", "ifSpeed"},
              {"1.3.6.1.2.1.2.2.1.6", "ifPhysAddress"},
              {"1.3.6.1.2.1.2.2.1.7", "ifAdminStatus"},
              {"1.3.6.1.2.1.2.2.1.8", "ifOperStatus"},
              {"1.3.6.1.2.1.2.2.1.9", "ifLastChange"},
              {"1.3.6.1.2.1.2.2.1.10", "ifInOctets"},
              {"1.3.6.1.2.1.2.2.1.11", "ifInUcastPkts"},
              {"1.3.6.1.2.1.2.2.1.12", "ifInNUcastPkts"},
              {"1.3.6.1.2.1.2.2.1.13", "ifInDiscards"},
              {"1.3.6.1.2.1.2.2.1.14", "ifInErrors"},
              {"1.3.6.1.2.1.2.2.1.15", "ifInUnknownProtos"},
              {"1.3.6.1.2.1.2.2.1.16", "ifOutOctets"},
              {"1.3.6.1.2.1.2.2.1.17", "ifOutUcastPkts"},
              {"1.3.6.1.2.1.2.2.1.18", "ifOutNUcastPkts"},
              {"1.3.6.1.2.1.2.2.1.19", "ifOutDiscards"},
              {"1.3.6.1.2.1.2.2.1.20", "ifOutErrors"},
              {"1.3.6.1.2.1.2.2.1.21", "ifOutQLen"},
              {"1.3.6.1.2.1.2.2.1.22", "ifSpecific"},
      };
      ::std::vector<::std::string> ifType_decode = {
          "other",
          "regular1822",
          "hdh1822",
          "ddn-x25",
          "rfc877-x25",
          "ethernet-csmacd",
          "iso88023-csmacd",
          "iso88024-tokenBus",
          "iso88025-tokenRing",
          "iso88026-man",
          "starLan",
          "proteon-10Mbit",
          "proteon-80Mbit",
          "hyperchannel",
          "fddi",
          "lapb",
          "sdlc",
          "ds1",
          "e1",
          "basicISDN",
          "primaryISDN",
          "propPointToPointSerial",
          "ppp",
          "softwareLoopback",
          "eon",
          "ethernet-3Mbit",
          "nsip",
          "slip",
          "ultra",
          "ds3",
          "sip",
          "frame-relay"
      };

      if_types getEnumObject(::std::string str_T)
        {
          return(if_types)
              (::std::stoi(*(::SplitStr(str_T, '.').end() - 2)) - 1);
        }

      ::std::string SNMPv2::getStrRepre()
        {
          ::std::string ret;
          for (auto &dat: data)
            ret += dat->getStrRepre();
          return ret;
        }

      BinaryVectorT SNMPv2::getBinary()
        {
          /* Sequence, length */
          BinaryVectorT temp;
          for (auto &dat:  data)
            ::JoinVectors(temp, dat->getBinary());
          BinaryVectorT ret {::DataTypesE::SEQ, (ByteT) temp.size()};
          ::JoinVectors(ret, temp);
          return ret;
        }

      void SNMPv2::push_back(BitMap *dat)
        { data.emplace_back(dat); }

      SNMPv2::~SNMPv2()
        { }

      ::std::string Version::getStrRepre()
        { return ""; }

      BinaryVectorT Version::getBinary()
        { return {::DataTypesE::INT, 0x01, (ByteT) ::std::stoi(_version)}; }

      CommunityString::CommunityString(::std::string str) :
          _str(str)
        { }

      ::std::string CommunityString::getStrRepre()
        { return ""; }

      BinaryVectorT CommunityString::getBinary()
        {
          BinaryVectorT ret {::DataTypesE::OCTET_STR, (ByteT)_str.size()};
          return ::JoinVectors(ret, ::StrToBin(_str));
        }

      Version::Version(::std::string ver) :
          _version(ver)
        { }

      Version::~Version()
        { }

      CommunityString::~CommunityString()
        { }

      namespace PDU
        {
            SNMPv2::SNMPv2(BitMap *_t) :
                __type(_t)
              { }

            ::std::string SNMPv2::getStrRepre()
              {
                ::std::string ret;
                for (auto &dat: data)
                  ret += dat->getStrRepre();
                return ret;
              }

            BinaryVectorT SNMPv2::getBinary()
              {
                BinaryVectorT temp;
                for (auto &dat:  data)
                  ::JoinVectors(temp, dat->getBinary());
                BinaryVectorT ret {
                    __type->getBinary()[0], (ByteT) temp.size()
                };
                return ::JoinVectors(ret, temp);
              }

            void SNMPv2::push_back(BitMap *dat)
              { data.emplace_back(dat); }

            SNMPv2::~SNMPv2()
              { }

            Type::Type(TypeE type) :
                _type(type)
              { }

            ::std::string Type::getStrRepre()
              { return ""; }

            BinaryVectorT Type::getBinary()
              { return {_type_byte[_type]}; }

            RequestID::RequestID(uint32_t id) :
                _id(id)
              { }

            ::std::string RequestID::getStrRepre()
              { return ""; }

            BinaryVectorT RequestID::getBinary()
              {
                BinaryVectorT ret = {::DataTypesE::INT, 0x04};
                return ::JoinVectors(ret, ::NumToBin(_id));
              }

            Error::Error(ErrorsE rep) :
                _err_code(rep)
              { }

            ::std::string Error::getStrRepre()
              { return ""; }

            BinaryVectorT Error::getBinary()
              {
                BinaryVectorT ret = {::DataTypesE::INT, 0x01};
                return ::JoinVectors(ret, ::NumToBin((ByteT) _err_code));
              }

            ErrorIndex::ErrorIndex(uint32_t rep) :
                _index(rep)
              { }

            ::std::string ErrorIndex::getStrRepre()
              { return ""; }

            BinaryVectorT ErrorIndex::getBinary()
              {
                BinaryVectorT ret = {::DataTypesE::INT, 0x01};
                return ::JoinVectors(ret, ::NumToBin((ByteT) _index));
              }


            Type::~Type()
              { }

            RequestID::~RequestID()
              { }

            Error::~Error()
              { }

            ErrorIndex::~ErrorIndex()
              { }

            namespace Bindings
              {
                  ::std::string BindingList::getStrRepre()
                    {
                      ::std::string ret;
                      for (auto &obj: objs)
                        ret += obj->getStrRepre();
                      return ret;
                    }

                  BinaryVectorT BindingList::getBinary()
                    {
                      BinaryVectorT temp;
                      for (auto &obj: objs)
                        ::JoinVectors(temp, obj->getBinary());
                      BinaryVectorT ret {::DataTypesE::SEQ , (ByteT) temp.size()};
                      return ::JoinVectors(ret, temp);
                    }

                  void BindingList::push_back(BitMap *dat)
                    { objs.emplace_back(dat); }

                  BindingList::~BindingList()
                    { }

                  Object::Object(
                      ::std::string name,
                      BinaryVectorT value,
                      ::DataTypesE type) :
                      _name(name),
                      _value_b(value),
                      _type(type)
                    {
                      switch (getEnumObject(name))
                        {
                          case ifOutQLen:
                          case ifInUnknownProtos:
                          case ifInErrors:
                          case ifInDiscards:
                          case ifInNUcastPkts:
                          case ifInUcastPkts:
                          case ifInOctets:
                          case ifSpeed:
                          {
                              unsigned long ival = 0;
                              for (auto ite = value.begin();
                                   ite != value.end();
                                   ++ite)
                                ival = (ival << 8) | *ite;
                              _value_s = ::std::to_string(ival);
                              break;
                            }
                          case ifOutErrors:
                          case ifOutDiscards:
                          case ifOutNUcastPkts:
                          case ifOutUcastPkts:
                          case ifOutOctets:
                          case ifLastChange:
                          case ifOperStatus:
                          case ifAdminStatus:
                          case ifIndex:
                          case ifType:
                          case ifMtu:
                            {
                              long ival = 0;
                              for (auto ite = value.begin();
                                   ite != value.end();
                                   ++ite)
                                ival = (ival << 8) | *ite;
                              _value_s = ::std::to_string(ival);
                              break;
                            }
                          case ifPhysAddress:
                            {
                              _value_s = ::BinToStr(
                                  BinaryVectorT(value));
                              ::InsertChar(_value_s, ':');
                              break;
                            }
                          case ifSpecific:
                            {
                              if (value.size() > 0)
                                _value_s = ::decodeObjectName(value);
                              else
                                _value_s = "0.0";
                              break;
                            }
                          case ifDescr:
                          default:
                            _value_s = ::std::string(
                                value.begin(), value.end());
                          break;
                        }
                    }

                  ::std::string Object::getStrRepre()
                    {
                      return _value_s;
                    }

                  BinaryVectorT Object::getBinary()
                    {
                      ::std::vector<::std::string> attrs(::SplitStr(_name, '.'));
                      BinaryVectorT temp {
                          (ByteT)(::std::stoul(attrs[0]) * 40 + ::std::stoul(attrs[1]))
                      };
                      ::JoinVectors(temp, ::EncodeANS1(attrs));
                      BinaryVectorT ret {
                          ::DataTypesE::SEQ, (ByteT) (_value_b.size() + temp.size() + 4),
                          ::DataTypesE::OBJECT_IDENT, (ByteT) temp.size()
                      };
                      ::JoinVectors(ret, temp);
                      ret.push_back(_type);
                      ret.push_back((ByteT) _value_b.size());
                      return ::JoinVectors(ret, _value_b);
                    }

                  Object::~Object()
                    { }
              }
        }
  }
