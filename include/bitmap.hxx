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

typedef unsigned char BYTE;

static const ::std::string BINARY_DECODE_STRING = "0123456789ABCDEF";

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
  {
    ::std::vector<BYTE> ret(::std::begin(str), ::std::end(str));
    ret.push_back(0);
    return ret;
  }

template<
    typename _IntegerT>
  static inline ::std::vector<BYTE> NumToBin(
      _IntegerT num)
    {
      BYTE *ptr_n = reinterpret_cast<BYTE *>(&num);
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
    typename _VectorT>
  static inline _VectorT &JoinVectors(
      _VectorT &_out,
      _VectorT &&_in)
    {
      ::std::copy(::std::begin(_in),
                  ::std::end(_in),
                  ::std::back_inserter(_out));
      return _out;
    }
