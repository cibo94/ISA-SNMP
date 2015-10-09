/**
 * @file packet.hxx
 * @brief
 * @author Miroslav Cibulka - xcibul10
 * @details
 */

#pragma once

#include "pdu.hxx"


/**
 * @brief Container
 */
class Manager
  {
public:

    Manager(::Packet::SNMPv2 *snmp) :
        _snmp(snmp)
      { }

    ::std::string toString() noexcept
      { return _snmp->getStrRepre(); }

    ::std::vector<BYTE> toBinary() noexcept
      { return _snmp->getBinary(); }

    ~Manager()
      { delete _snmp; }

    ::Packet::SNMPv2 *_snmp;
  };

static Manager createManager(
    ::std::string Community,
    ::Packet::PDU::Type::TypeE PDUType,
    ::uint32_t RequestID,
    ::uint32_t NonRepeaters,
    ::uint32_t MaxRepeaters)
  {
    ::Packet::SNMPv2 *_snmp = new ::Packet::SNMPv2();
    ::Packet::PDU::SNMPv2 *_pdu = new ::Packet::PDU::SNMPv2();
    _snmp->push_back(new ::Packet::CommunityString(Community));
    _snmp->push_back(_pdu);
    _pdu->push_back(new ::Packet::PDU::Type(PDUType));
    _pdu->push_back(new ::Packet::PDU::RequestID(RequestID));
    _pdu->push_back(new ::Packet::PDU::NonRepeaters(NonRepeaters));
    _pdu->push_back(new ::Packet::PDU::MaxRepeaters(MaxRepeaters));
    return Manager(_snmp);
  }
