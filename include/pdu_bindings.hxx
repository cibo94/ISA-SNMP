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

                  struct BindingList :
                      public BitMap
                    {
                      virtual ::std::string getStrRepre() override;

                      virtual ::std::vector<BYTE> getBinary();

                      void push_back(BitMap *dat);

                      virtual ~BindingList();

                  private:
                      ::std::vector<::std::unique_ptr<BitMap>> objs;
                    };

                  struct Bind :
                      public BitMap
                    {
                      Bind(
                          BitMap *obj_name,
                          BitMap *obj_value);

                      virtual ::std::string getStrRepre() override;

                      virtual ::std::vector<BYTE> getBinary();

                      virtual ~Bind();

                  private:
                      ::std::unique_ptr<BitMap> _obj_name;
                      ::std::unique_ptr<BitMap> _obj_value;
                    };

                  struct ObjectName :
                      public BitMap
                    {
                      ObjectName(::std::string name);

                      virtual ::std::string getStrRepre();

                      virtual ::std::vector<BYTE> getBinary();

                      static ::std::unordered_map<
                          ::std::string, ::std::string> _decoded;

                      ::std::string _name;
                    };

                  struct ObjectValue :
                      public BitMap
                    {
                      ObjectValue(::std::string value);

                      ObjectValue(int value);

                      ObjectValue();

                      virtual ::std::string getStrRepre() override;

                      virtual ::std::vector<BYTE> getBinary();

                  private:
                      ::std::string _value_s;
                      int _value_i;

                      ::std::vector<BYTE> _value_b;
                      ::DataTypesE type;
                    };
              }
        }
  }
