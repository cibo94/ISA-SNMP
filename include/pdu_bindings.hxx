/**
 * @file pdu_bindings.hxx
 * @brief PDU object bindings classes implementation
 * @author Miroslav Cibulka - xcibul10
 * @details
 *    This contains pdu object bindings which means
 *    name & value. Value may have different types like
 *    octet string or integer.
 */

#pragma once

#include "bitmap.hxx"


namespace Packet
  {
      namespace PDU
        {
            namespace Bindings
              {
                  /**
                   * @brief list of objects bindings
                   */
                  struct BindingList :
                      public BitMap
                    {
                      virtual ::std::string getStrRepre() override;

                      virtual BinaryVectorT getBinary();

                      /**
                       * @brief push_back binding
                       * @param dat is BitMap of binding
                       */
                      void push_back(BitMap *dat);

                      virtual ~BindingList();

                  private:
                      ::std::vector<::std::unique_ptr<BitMap>> objs;
                    };

                  struct Object :
                      public BitMap
                    {
                      Object(
                          ::std::string name,
                          BinaryVectorT value,
                          ::DataTypesE type);

                      virtual ::std::string getStrRepre();

                      virtual BinaryVectorT getBinary();

                      static ::std::unordered_map<
                          ::std::string, ::std::string> _decoded;

                      virtual ~Object();
                  private:
                      ::std::string _value_s;
                      ::std::string _name;

                      BinaryVectorT _value_b;
                      ::DataTypesE _type;
                    };
              }
        }
  }
