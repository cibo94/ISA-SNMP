/**
 * @file pdu.hxx
 * @brief
 * @author Miroslav Cibulka - xcibul10
 * @details
 */

#pragma once

#include "bitmap.hxx"


namespace Packet
  {
    namespace PDU
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

        struct Type :
            public BitMap
          {
            enum TypeE : BYTE
              {
                GET_REQ, GET_NEXT_REQ, GET_RESP, SET_RESP,
                OBSOLETE, GET_BULK_REQ, INFORM_REQ, TRAP
              };

            Type(TypeE type);

            virtual ::std::string getStrRepre() override;

            virtual ::std::vector<BYTE> getBinary();

          private:
            TypeE _type;
            const ::std::vector<::std::string> _type_dec = {
              "getRequest", "GetNextRequest", "GetResponse",
              "SetResponse", "obsolete", "GetBulkRequest",
              "InformRequest", "SNMPv2-TRAP"
            };
          };

        struct RequestID :
            public BitMap
          {
            RequestID(uint32_t id);

            virtual ::std::string getStrRepre() override;

            virtual ::std::vector<BYTE> getBinary();

          private:
            uint32_t _id;
          };

        struct NonRepeaters :
            public BitMap
          {
            NonRepeaters(uint32_t rep);

            virtual ::std::string getStrRepre() override;

            virtual ::std::vector<BYTE> getBinary();

          private:
            uint32_t _reps;
          };

          struct MaxRepeaters :
            public BitMap
          {
            MaxRepeaters(uint32_t rep);

            virtual ::std::string getStrRepre() override;

            virtual ::std::vector<BYTE> getBinary();

          private:
            uint32_t _reps;
          };
      }
  }
