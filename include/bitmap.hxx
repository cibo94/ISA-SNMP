/**
 * @file bitmap.hxx
 * @brief
 * @author Miroslav Cibulka - xcibul10
 * @details
 */

#pragma once

#include <vector>
#include <algorithm>
#include <memory>
#include <iterator>

extern "C" {
#include <stdint.h>
}

#define IS_BIG_ENDIAN ((bool)((*(uint16_t *)"\0\xff") < 0x100))

typedef ::uint8_t ByteT;
typedef ::std::vector<ByteT> BinaryVectorT;


static const ::std::string BINARY_DECODE_STRING = "0123456789ABCDEF";

enum DataTypesE :
    ByteT
  {
    INT = 0x02, OCTET_STR = 0x04, NULL_T = 0x05, OBJECT_IDENT = 0x06
    , SEQ = 0x30, GET_REQ_PDU = 0xA0, GET_RESP_PDU = 0xA2, SET_REQ_PDU = 0xA3
  };

struct BitMap
  {
    BitMap() = default;
    BitMap(BitMap &&) = default;
    virtual BitMap &operator=(BitMap &&) = default;
    virtual ::std::string getStrRepre() = 0;
    virtual BinaryVectorT getBinary() = 0;
    virtual ~BitMap() { };
  };

static inline BinaryVectorT StrToBin(
    ::std::string str)
  { return BinaryVectorT(::std::begin(str), ::std::end(str)); }

template<
    typename _IntegerT>
  static inline BinaryVectorT NumToBin(
      _IntegerT num)
    {
      ByteT *ptr_n = reinterpret_cast<ByteT *>(&num);
      if (!IS_BIG_ENDIAN)
        ::std::reverse(ptr_n, ptr_n + sizeof(_IntegerT));
      return BinaryVectorT(ptr_n, ptr_n + sizeof(_IntegerT));
    }

template<
    typename _BinaryT>
  static inline ::std::string BinToStr(
      _BinaryT &&binary)
    {
      ::std::string ret;
      ret.reserve(binary.size() * 2);
      for (auto &bin : binary)
        {
          ret.push_back(BINARY_DECODE_STRING[(bin >> 4) & 0xF]);
          ret.push_back(BINARY_DECODE_STRING[bin & 0xF]);
        }
      return ret;
    }

template<
    typename _VectorTIn,
    typename _VectorTOut>
  static inline _VectorTOut &JoinVectors(
      _VectorTOut &_out,
      _VectorTIn &&_in)
    {
      ::std::copy(::std::begin(_in),
                  ::std::end(_in),
                  ::std::back_inserter(_out));
      return _out;
    }

static inline ::std::vector<::std::string> SplitStr(
    ::std::string str,
    char del)
  {
    ::std::vector<::std::string> ret;
    ret.push_back("");
    for (auto &ch: str)
      if (ch != del)
        ret.back().push_back(ch);
      else
        ret.push_back("");
    return ret;
  }

static inline BinaryVectorT EncodeANS1(
    ::std::vector<::std::string> attrs)
  {
    BinaryVectorT ret;
    for (auto attr = attrs.begin() + 2; attr != attrs.end(); ++attr)
      {
        unsigned long num = ::std::stoul(*attr);
        BinaryVectorT temp;
        if (num > 127)
          while (num > 0)
            {
              temp.push_back((ByteT)(num | 0x80));
              num >>= 7;
            }
        else
          temp.push_back((ByteT) num);
        ::std::reverse(temp.begin(), temp.end());
        ::JoinVectors(ret, temp);
      }
    return ret;
  }

static inline ::std::vector<::std::string> StrSplit(
    ::std::string str,
    char d)
  {
    ::std::vector<::std::string> ret {""};
    for (auto ite = str.begin();
         ite != str.end();
         ++ite)
      if (*ite == d)
        ret.push_back("");
      else
        ret.back().push_back(*ite);
    return ret;
  }

static inline ::std::string StrJoin(
    ::std::vector<::std::string>::iterator str_B,
    ::std::vector<::std::string>::iterator str_E,
    char del)
  {
    ::std::string ret;
    while (str_B != str_E - 1)
      (ret += *(str_B++)).push_back(del);
    ret += *str_B;
    return ret;
  }

template<
    typename _VectorT>
  static inline _VectorT &InsertChar(
      _VectorT &vec,
      char c)
    {
      if (vec.size() > 0)
        for (int i = vec.size() - 1; i > 0; --i)
          if (i % 2 == 0)
            vec.insert(vec.begin() + i, c);
      return vec;
    }


static inline ::std::string decodeObjectName(
    BinaryVectorT &msg)
  {
    ::std::vector<uint32_t> data {
        (uint32_t)(msg[0]/40), (uint32_t)(msg[0]%40)
    };
    bool was_m_b = false;
    for (auto ite = msg.begin() + 1;
         ite != msg.begin() + msg.size();
         ++ite)
      {
        if ((*ite) & 0x80)
          {
            if (!was_m_b) data.push_back(0);
            data.back() <<= 7;
            data.back() |= (*ite) & 0x7F;
            was_m_b = true;
          }
        else
          {
            data.push_back(*ite);
            was_m_b = false;
          }
      }
    ::std::string str_vec;
    auto bin = data.begin();
    for (;
        bin != data.end() - 1;
        ++bin)
      str_vec += ::std::to_string(*bin) + ".";
    str_vec += ::std::to_string(*bin);
    return str_vec;
  }