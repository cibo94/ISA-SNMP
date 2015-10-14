/**
 * @file pdu.hxx
 * @brief
 * @author Miroslav Cibulka - xcibul10
 * @details
 */

#pragma once

#include <unordered_map>
#include "bitmap.hxx"


namespace Packet
  {
      namespace PDU
        {
            struct Type;

            struct SNMPv2 :
                public BitMap
              {
                SNMPv2(Type *_T);

                virtual ::std::string getStrRepre() override;

                virtual ::std::vector<BYTE> getBinary() override;

                void push_back(BitMap *dat);

                virtual ~SNMPv2();

            private:
                Type *__type;
                ::std::vector<::std::unique_ptr<BitMap>> data;
              };

            struct Type :
                public BitMap
              {
                enum TypeE :
                    BYTE
                  {
                    GET_REQ = 0
                    , GET_NEXT_REQ
                    , GET_RESP
                    , SET_REQ
                  };

                Type(TypeE type);

                virtual ::std::string getStrRepre() override;

                virtual ::std::vector<BYTE> getBinary() override;

            public:
                static ::std::vector<BYTE> _type_byte;
                static ::std::unordered_map<BYTE, TypeE> _byte_type;
            private:
                TypeE _type;
                static ::std::vector<::std::string> _type_dec;
              };

            struct RequestID :
                public BitMap
              {
                RequestID(uint32_t id);

                virtual ::std::string getStrRepre() override;

                virtual ::std::vector<BYTE> getBinary() override;

            private:
                uint32_t _id;
              };

            struct Error :
                public BitMap
              {
                enum ErrorsE : BYTE
                  {
                    noError = 0
                    , tooBig = 1
                    , noSuchName = 2
                    , badValue = 3
                    , readOnly = 4
                    , genErr = 5
                    , noAccess = 6
                    , wrongType = 7
                    , wrongLength = 8
                    , wrongEncoding = 9
                    , wrongValue = 10
                    , noCreation = 11
                    , inconsistentValue = 12
                    , resourceUnavailable = 13
                    , commitFailed = 14
                    , undoFailed = 15
                    , authorizationError = 16
                    , notWritable = 17
                    , inconsistentName = 18
                  };

                Error(ErrorsE rep);

                virtual ::std::string getStrRepre() override;

                virtual ::std::vector<BYTE> getBinary() override;

            private:
                ErrorsE _err_code = noError;
                const ::std::vector<::std::string> error_table = {
                    "No error", "Too big", "No such name", "Bad value",
                    "Read only", "Generation error", "No access", "Wrong type",
                    "Wrong length", "Wrong encoding", "Wrong value",
                    "No creation", "Inconsistent value", "Resource unavailable",
                    "Commit failed", "Undo failed", "Authorization error",
                    "Not writable", "Inconsistent name"
                };
              };

            struct ErrorIndex :
                public BitMap
              {
                ErrorIndex(uint32_t rep);

                virtual ::std::string getStrRepre() override;

                virtual ::std::vector<BYTE> getBinary() override;

            private:
                uint32_t _index;
              };
        }
  }
