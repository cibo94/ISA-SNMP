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
            /** Is bitmap (e.g. map/vector of bits) with string representation
             * This does not contains any real bits
             */
            struct SNMPv2 :
                public BitMap
              {
                /** Requires appropriate type field */
                SNMPv2(BitMap *_T);

                virtual ::std::string getStrRepre() override;

                virtual BinaryVectorT getBinary() override;

                /** Pushes bit-data in form of bitmap (like version or like that) */
                void push_back(BitMap *dat);

                virtual ~SNMPv2();

            private:
                /** Version of type of SMNPv2 packet (e.g. GET_REQ)*/
                BitMap *__type;

                /** Actual data */
                ::std::vector<::std::unique_ptr<BitMap>> data;
              };

            struct Type :
                public BitMap
              {
                /** Types of SNMPv2 packet */
                enum TypeE :
                    ByteT
                  {
                    /** Get request will send request to agent and he will
                     * return actual data about requesting object
                     */
                    GET_REQ = 0
                    /** Get next request will send request for object
                     * and gets info about next object
                     */
                    , GET_NEXT_REQ
                    /** Is type of replied message from agent */
                    , GET_RESP
                    /** ??? */
                    , SET_REQ
                  };

                /** Represents binary representation of SNMPv2 packet type
                 * @see TypeE
                 */
                Type(TypeE type);

                virtual ::std::string getStrRepre() override;

                virtual BinaryVectorT getBinary() override;

                virtual ~Type();
            public:
                /** Lookup table TypeE -> Binary */
                static BinaryVectorT _type_byte;

                /** Lookup table vector of Binary -> TypeE */
                static ::std::unordered_map<ByteT, TypeE> _byte_type;
            private:
                TypeE _type;
              };

            /** Request ID of SNMPv2 packet - transaction */
            struct RequestID :
                public BitMap
              {
                /** needs to be defined */
                RequestID(uint32_t id);

                virtual ::std::string getStrRepre() override;

                virtual BinaryVectorT getBinary() override;

                virtual ~RequestID();
            private:
                uint32_t _id;
              };

            /** Describes Errors in SNMPv2 requests/responses */
            struct Error :
                public BitMap
              {
                /** Errors enumerator - not used really (maybe later) */
                enum ErrorsE :
                    ByteT
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

                /** @see ErrorsE */
                Error(ErrorsE rep);

                virtual ::std::string getStrRepre() override;

                virtual BinaryVectorT getBinary() override;

                virtual ~Error();
            private:
                /** Default is no error flag */
                ErrorsE _err_code = noError;

                /** Lookup table @see ErrorsE */
                const ::std::vector<::std::string> error_table = {
                    "No error", "Too big", "No such name", "Bad value",
                    "Read only", "Generation error", "No access", "Wrong type",
                    "Wrong length", "Wrong encoding", "Wrong value",
                    "No creation", "Inconsistent value", "Resource unavailable",
                    "Commit failed", "Undo failed", "Authorization error",
                    "Not writable", "Inconsistent name"
                };
              };

            /** Index of which error occurred */
            struct ErrorIndex :
                public BitMap
              {
                /** needs to be defined @see Error */
                ErrorIndex(uint32_t rep);

                virtual ::std::string getStrRepre() override;

                virtual BinaryVectorT getBinary() override;

                virtual ~ErrorIndex();
            private:
                /** Default is no position */
                uint32_t _index;
              };
        }
  }
