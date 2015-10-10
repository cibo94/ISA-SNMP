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
    SNMPv2::SNMPv2()
      { }

    ::std::string SNMPv2::getStrRepre()
      {
        ::std::string ret;
        for (auto &dat: data)
          ret += dat->getStrRepre() + " ";
        return ret;
      }

    ::std::vector<BYTE> SNMPv2::getBinary()
      {
        ::std::vector<BYTE> ret;
        for (auto &dat:  data)
          ::JoinVectors(ret, dat->getBinary());
        return ret;
      }

    void SNMPv2::push_back(BitMap *dat)
      {
        data.emplace_back(dat);
      }
    
    SNMPv2::~SNMPv2() { }

    CommunityString::CommunityString(::std::string str) :
        _str(str)
      { }

    ::std::string CommunityString::getStrRepre()
      {
        return _str;
      }

    ::std::vector<BYTE> CommunityString::getBinary()
      {
        return ::StrToBin(_str);
      }

      Version::Version(::std::string ver) :
          _version(ver)
        { }

    ::std::string Version::getStrRepre()
      {
        return _version;
      }

    ::std::vector<BYTE> Version::getBinary()
      {
        return ::NumToBin((uint32_t)::std::stoi(_version));
      }

    namespace PDU
      {
        ::std::string SNMPv2::getStrRepre()
          {
            ::std::string ret;
            for (auto &dat: data)
              ret += dat->getStrRepre() + " ";
            return ret;
          }

        ::std::vector<BYTE> SNMPv2::getBinary()
          {
            ::std::vector<BYTE> ret;
            for (auto &dat:  data)
              JoinVectors(ret, dat->getBinary());
            return ret;
          }

        void SNMPv2::push_back(BitMap *dat)
          {
            data.emplace_back(dat);
          }

        SNMPv2::~SNMPv2() { }

        Type::Type(TypeE type) :
            _type(type)
          { }

        ::std::string Type::getStrRepre()
          {
            return _type_dec[_type];
          }

        ::std::vector<BYTE> Type::getBinary()
          {
            return ::NumToBin(static_cast<uint32_t>(_type));
          }

        RequestID::RequestID(uint32_t id) :
            _id(id)
          { }

        ::std::string RequestID::getStrRepre()
          {
            return BinToStr(getBinary());
          }

        ::std::vector<BYTE> RequestID::getBinary()
          {
            return NumToBin(_id);
          }

        NonRepeaters::NonRepeaters(uint32_t rep) :
            _reps(rep)
          { }

        ::std::string NonRepeaters::getStrRepre()
          {
            return ::std::to_string(_reps);
          }

        ::std::vector<BYTE> NonRepeaters::getBinary()
          {
            return NumToBin(_reps);
          }

        MaxRepeaters::MaxRepeaters(uint32_t rep) :
            _reps(rep)
          { }

        ::std::string MaxRepeaters::getStrRepre()
          {
            return ::std::to_string(_reps);
          }

        ::std::vector<BYTE> MaxRepeaters::getBinary()
          {
            return NumToBin(_reps);
          }

        namespace Bindings
          {
            Bind::Bind(BitMap *obj_name,
                       BitMap *obj_value) :
                _obj_name(obj_name),
                _obj_value(obj_value)
              { }

            ::std::string Bind::getStrRepre()
              {
                return _obj_name->getStrRepre() + " " +
                       _obj_value->getStrRepre() + " ";
              }

            ::std::vector<BYTE> Bind::getBinary()
              {
                ::std::vector<BYTE> ret;
                ::JoinVectors(ret, _obj_value->getBinary());
                ::JoinVectors(ret, _obj_name->getBinary());
                return ret;
              }
            
            Bind::~Bind() { }

            ObjectName::ObjectName(::std::string name) :
                _name(name)
              { }

            ::std::string ObjectName::getStrRepre()
              {
                return _name;
              }

            ::std::vector<BYTE> ObjectName::getBinary()
              {
                return StrToBin(_name);
              }

            ObjectValue::ObjectValue() :
                _value("<NO_VALUE>"), not_set(true)
              { }

            ObjectValue::ObjectValue(::std::string value) :
                _value(value), not_set(false)
              { }

            ::std::string ObjectValue::getStrRepre()
              { return _value; }

            ::std::vector<BYTE> ObjectValue::getBinary()
              {
                return not_set ? ::std::vector<BYTE>({0})
                                : ::StrToBin(_value);
              }
          }
      }
  }
