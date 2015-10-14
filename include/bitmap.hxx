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

typedef uint8_t BYTE;

static const ::std::string BINARY_DECODE_STRING = "0123456789ABCDEF";

enum DataTypesE : BYTE
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
    virtual ::std::vector<BYTE> getBinary() = 0;
    virtual ~BitMap() { };
  };

static inline ::std::vector<BYTE> StrToBin(
    ::std::string str)
  { return ::std::vector<BYTE>(::std::begin(str), ::std::end(str)); }

template<
    typename _IntegerT>
  static inline ::std::vector<BYTE> NumToBin(
      _IntegerT num)
    {
      BYTE *ptr_n = reinterpret_cast<BYTE *>(&num);
      if (!IS_BIG_ENDIAN)
        ::std::reverse(ptr_n, ptr_n + sizeof(_IntegerT));
      return ::std::vector<BYTE>(ptr_n, ptr_n + sizeof(_IntegerT));
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

static inline ::std::vector<BYTE> EncodeANS1(
    ::std::vector<::std::string> attrs)
  {
    ::std::vector<BYTE> ret;
    for (auto attr = attrs.begin() + 2; attr != attrs.end(); ++attr)
      {
        unsigned long num = ::std::stoul(*attr);
        ::std::vector<BYTE> temp;
        if (num > 127)
          while (num > 0)
            {
              temp.push_back((BYTE)(num | 0x80));
              num >>= 7;
            }
        else
          temp.push_back((BYTE) num);
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