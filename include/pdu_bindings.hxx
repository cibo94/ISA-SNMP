/**
 * @file pdu_bindings.hxx
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
        namespace Bindings
          {
            struct Bind :
                public BitMap
              {
                Bind(BitMap *obj_name,
                     BitMap *obj_value);

                virtual ::std::string getStrRepre() override;

                virtual ::std::vector<BYTE> getBinary();
                
                virtual ~Bind();
              private:
                ::std::unique_ptr<BitMap>_obj_name;
                ::std::unique_ptr<BitMap>_obj_value;
              };

              struct ObjectName :
                public BitMap
              {
                ObjectName(::std::string name);

                virtual ::std::string getStrRepre();

                virtual ::std::vector<BYTE> getBinary();

              private:
                ::std::string _name;
              };

            struct ObjectValue :
                public BitMap
              {
                ObjectValue();

                ObjectValue(::std::string value);

                virtual ::std::string getStrRepre() override;

                virtual ::std::vector<BYTE> getBinary();

              private:
                ::std::string _value;
                bool not_set;
              };
          }
      }
  }
