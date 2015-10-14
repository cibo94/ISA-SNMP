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


namespace Packet
  {
      ::std::vector<BYTE> PDU::Type::_type_byte {0xA0, 0xA1, 0xA2, 0xA3};
      ::std::unordered_map<BYTE, PDU::Type::TypeE> PDU::Type::_byte_type {
          {0xA0, PDU::Type::GET_REQ},
          {0xA1, PDU::Type::GET_NEXT_REQ},
          {0xA2, PDU::Type::GET_RESP},
          {0xA3, PDU::Type::SET_REQ}
      };
      ::std::vector<::std::string> PDU::Type::_type_dec = {
          "getRequest", "GetNextRequest", "GetResponse",
          "SetResponse", "obsolete", "GetBulkRequest",
          "InformRequest", "SNMPv2-TRAP"
      };
      ::std::unordered_map<
          ::std::string, ::std::string> PDU::Bindings::ObjectName::_decoded = {
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
              {"1.3.6.1.2.1.2.2.1.22", "ifSpecific"}
      };


      ::std::string SNMPv2::getStrRepre()
        {
          ::std::string ret;
          for (auto &dat: data)
            ret += dat->getStrRepre();
          return ret;
        }

      ::std::vector<BYTE> SNMPv2::getBinary()
        {
          /* Sequence, length */
          ::std::vector<BYTE> temp;
          for (auto &dat:  data)
            ::JoinVectors(temp, dat->getBinary());
          ::std::vector<BYTE> ret {::DataTypesE::SEQ, (BYTE) temp.size()};
          ::JoinVectors(ret, temp);
          return ret;
        }

      void SNMPv2::push_back(BitMap *dat)
        { data.emplace_back(dat); }

      SNMPv2::~SNMPv2()
        { }

      ::std::string Version::getStrRepre()
        { return ""; }

      ::std::vector<BYTE> Version::getBinary()
        { return {::DataTypesE::INT, 0x01, (BYTE) ::std::stoi(_version)}; }

      CommunityString::CommunityString(::std::string str) :
          _str(str)
        { }

      ::std::string CommunityString::getStrRepre()
        { return ""; }

      ::std::vector<BYTE> CommunityString::getBinary()
        {
          ::std::vector<BYTE> ret {::DataTypesE::OCTET_STR, (BYTE)_str.size()};
          return ::JoinVectors(ret, ::StrToBin(_str));
        }

      Version::Version(::std::string ver) :
          _version(ver)
        { }

      namespace PDU
        {
            SNMPv2::SNMPv2(Type *_t) :
                __type(_t)
              { }

            ::std::string SNMPv2::getStrRepre()
              {
                ::std::string ret;
                for (auto &dat: data)
                  ret += dat->getStrRepre();
                return ret;
              }

            ::std::vector<BYTE> SNMPv2::getBinary()
              {
                ::std::vector<BYTE> temp;
                for (auto &dat:  data)
                  ::JoinVectors(temp, dat->getBinary());
                ::std::vector<BYTE> ret {
                    __type->getBinary()[0], (BYTE) temp.size()
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

            ::std::vector<BYTE> Type::getBinary()
              { return {_type_byte[_type]}; }

            RequestID::RequestID(uint32_t id) :
                _id(id)
              { }

            ::std::string RequestID::getStrRepre()
              { return ""; }

            ::std::vector<BYTE> RequestID::getBinary()
              {
                ::std::vector<BYTE> ret = {::DataTypesE::INT, 0x04};
                return ::JoinVectors(ret, ::NumToBin(_id));
              }

            Error::Error(ErrorsE rep) :
                _err_code(rep)
              { }

            ::std::string Error::getStrRepre()
              { return ""; }

            ::std::vector<BYTE> Error::getBinary()
              {
                ::std::vector<BYTE> ret = {::DataTypesE::INT, 0x01};
                return ::JoinVectors(ret, ::NumToBin((BYTE) _err_code));
              }

            ErrorIndex::ErrorIndex(uint32_t rep) :
                _index(rep)
              { }

            ::std::string ErrorIndex::getStrRepre()
              { return ""; }

            ::std::vector<BYTE> ErrorIndex::getBinary()
              {
                ::std::vector<BYTE> ret = {::DataTypesE::INT, 0x01};
                return ::JoinVectors(ret, ::NumToBin((BYTE) _index));
              }

            namespace Bindings
              {
                  ::std::string BindingList::getStrRepre()
                    {
                      ::std::string ret;
                      for (auto &obj: objs)
                        ret += obj->getStrRepre();
                      return ret;
                    }

                  ::std::vector<BYTE> BindingList::getBinary()
                    {
                      ::std::vector<BYTE> temp;
                      for (auto &obj: objs)
                        ::JoinVectors(temp, obj->getBinary());
                      ::std::vector<BYTE> ret {::DataTypesE::SEQ , (BYTE) temp.size()};
                      return ::JoinVectors(ret, temp);
                    }

                  void BindingList::push_back(BitMap *dat)
                    { objs.emplace_back(dat); }

                  BindingList::~BindingList()
                    { }

                  Bind::Bind(
                      BitMap *obj_name,
                      BitMap *obj_value) :
                      _obj_name(obj_name),
                      _obj_value(obj_value)
                    { }

                  ::std::string Bind::getStrRepre()
                    {
                      return _obj_value != nullptr ?
                             _obj_value->getStrRepre() : "";
                    }

                  ::std::vector<BYTE> Bind::getBinary()
                    {
                      ::std::vector<BYTE> temp;
                      ::JoinVectors(temp, _obj_name->getBinary());
                      ::JoinVectors(temp, _obj_value->getBinary());
                      ::std::vector<BYTE> ret {::DataTypesE::SEQ, (BYTE) temp.size()};
                      return ::JoinVectors(ret, temp);
                    }

                  Bind::~Bind()
                    { }

                  ObjectName::ObjectName(::std::string name) :
                      _name(name)
                    { }

                  ::std::string ObjectName::getStrRepre()
                    {
                      auto split = StrSplit(_name, '.');
                      ::std::string name = StrJoin(
                          split.begin(), split.end() - 1, '.');
                      ::std::string num = split.back();
                      return _decoded[name] + "." + num;
                    }

                  ::std::vector<BYTE> ObjectName::getBinary()
                    {
                      ::std::vector<::std::string> attrs(::SplitStr(_name, '.'));
                      ::std::vector<BYTE> temp {
                          (BYTE)(::std::stoul(attrs[0]) * 40 + ::std::stoul(attrs[1]))
                      };
                      ::JoinVectors(temp, ::EncodeANS1(attrs));
                      ::std::vector<BYTE> ret {
                          ::DataTypesE::OBJECT_IDENT, (BYTE) temp.size()
                      };
                      return ::JoinVectors(ret, temp);
                    }

                  ObjectValue::ObjectValue(::std::string value)
                    {
                      _value_s = value;
                      _value_b = {::DataTypesE::OCTET_STR, (BYTE) value.size()};
                      ::JoinVectors(_value_b, _value_s);
                      type = ::DataTypesE::OCTET_STR;
                    }

                  ObjectValue::ObjectValue(int value)
                    {
                      _value_i = value;
                      _value_b = {::DataTypesE::INT, sizeof(_value_i)};
                      ::JoinVectors(_value_b, ::NumToBin(_value_i));
                      type = ::DataTypesE::INT;
                    }

                  ObjectValue::ObjectValue()
                    {
                      type = ::DataTypesE::NULL_T;
                      _value_b = {::DataTypesE::NULL_T, 0};
                    }

                  ::std::string ObjectValue::getStrRepre()
                    {
                      switch(type)
                        {
                          case INT:
                            return ::std::to_string(_value_i);
                          case OCTET_STR:
                            return _value_s;
                          case NULL_T:
                            return "";
                          default:
                            return "";
                        }
                    }

                  ::std::vector<BYTE> ObjectValue::getBinary()
                    {
                      return _value_b;
                    }
              }
        }
  }
