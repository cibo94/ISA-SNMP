/**
 * @file packet.hxx
 * @brief
 * @author Miroslav Cibulka - xcibul10
 * @details
 */

#pragma once

#include <bitset>
#include "bitmap.hxx"
#include "pdu.hxx"


namespace Packet
  {
    struct SNMPv2 :
        public BitMap
      {
        virtual ::std::string getStrRepre() override;

        virtual ::std::vector<BYTE> getBinary();

        void push_back(BitMap *dat);

        virtual ~SNMPv2();
      private:
        ::std::vector<::std::unique_ptr<BitMap>> data;
      };

    struct CommunityString :
        public BitMap
      {
        CommunityString(::std::string str);

        virtual ::std::string getStrRepre() override;

        virtual ::std::vector<BYTE> getBinary();

      private:
        ::std::string _str;
      };

    struct Version :
        public BitMap
      {
        Version(::std::string ver);

        virtual ::std::string getStrRepre() override;

        virtual ::std::vector<BYTE> getBinary();

    private:
        ::std::string _version;
      };
  }
