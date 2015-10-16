/**
 * @file packet.hxx
 * @brief
 * @author Miroslav Cibulka - xcibul10
 * @details Packet implementation
 *    Contains whole structure of SNMPv2 packet:
 *       Version
 *       Community String
 *       PDU
 *         -> Type
 *         -> Request ID
 *         -> Error
 *         -> Error index
 *         -> Bindings
 *            --> Object
 *                ---> Object name
 *                ---> Object value
 *            ...
 */

#pragma once

#include <bitset>
#include "bitmap.hxx"
#include "pdu.hxx"


namespace Packet
  {
      /**
       * @brief SNMPv2 header
       */
      struct SNMPv2 :
          public BitMap
        {
          virtual ::std::string getStrRepre() override;

          virtual BinaryVectorT getBinary();

          void push_back(BitMap *dat);

          virtual ~SNMPv2();

      private:
          ::std::vector<::std::unique_ptr<BitMap>> data;
        };

      /**
       * @brief Community string
       */
      struct CommunityString :
          public BitMap
        {
          /**
           * @param str community
           */
          CommunityString(::std::string str);

          virtual ::std::string getStrRepre() override;

          virtual BinaryVectorT getBinary();

          virtual ~CommunityString();

      private:
          ::std::string _str;
        };

      /**
       * @brief Version (1,2,3)
       */
      struct Version :
          public BitMap
        {
          /** In string format */
          Version(::std::string ver);

          virtual ::std::string getStrRepre() override;

          virtual BinaryVectorT getBinary();

          virtual ~Version();

      private:
          ::std::string _version;
        };
  }
