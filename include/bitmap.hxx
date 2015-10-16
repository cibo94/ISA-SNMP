/**
 * @file bitmap.hxx
 * @brief Support module
 * @author Miroslav Cibulka - xcibul10
 * @details
 *    supporting module for SNMPv2 packet, contains
 *    function, base classes, global enumerators, typedefs, macros ...
 */

#pragma once

#include <vector>
#include <algorithm>
#include <memory>
#include <iterator>

extern "C" {
#include <stdint.h>
}

/** Checks if system is big or little endian */
#define IS_BIG_ENDIAN ((bool)((*(uint16_t *)"\0\xff") < 0x100))

/** Byte is eight-bit number */
typedef ::uint8_t ByteT;

/** Binary vector is vector of bytes */
typedef ::std::vector<ByteT> BinaryVectorT;

/** Hex lookup table for decoding binary */
static const ::std::string BINARY_DECODE_STRING = "0123456789ABCDEF";

/**
 * Basic data types with codes
 */
enum DataTypesE :
    ByteT
  {
    INT = 0x02, OCTET_STR = 0x04, NULL_T = 0x05, OBJECT_IDENT = 0x06
    , SEQ = 0x30, GET_REQ_PDU = 0xA0, GET_RESP_PDU = 0xA2, SET_REQ_PDU = 0xA3
  };

struct BitMap
  {
    /** Default cons */
    BitMap() = default;

    /** Default move cons */
    BitMap(BitMap &&) = default;

    /** Default move operator */
    virtual BitMap &operator=(BitMap &&) = default;

    /**
     * @brief Gets string representation of bitmap
     * @return string
     */
    virtual ::std::string getStrRepre() = 0;

    /**
     * @brief Gets binary representation of bitmap
     * @return binary vector
     */
    virtual BinaryVectorT getBinary() = 0;

    /**
     * @brief Needs to be virtual because it won't deallocate alone and
     *        some of instances may contains containers
     */
    virtual ~BitMap() { };
  };

/**
 * @brief Converts string to binary without zero at the end
 * @param str is input string
 * @return binary vector
 */
static inline BinaryVectorT StrToBin(
    ::std::string str)
  { return BinaryVectorT(::std::begin(str), ::std::end(str)); }

/**
 * @brief Converts Number to binary
 * @tparam _IntegerT is integer type with n-bytes - size does matters
 * @param num is number
 * @return vector created from this number
 */
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

/**
 * @brief Converts binary to string in hex format
 * @tparam _BinaryT is type of binary vector
 * @param binary is binary vector
 * @return string of hex dump
 */
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

/**
 * @brief Joins 2 vectors of same type
 * @tparam _VectorTIn is input vector
 * @tparam _VectorTOut is output vector
 * @param _out is output vector - unique reference
 * @param _in is input vector - reference
 * @return _out
 */
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

/**
 * @brief encodes vector of strings into ANS.1 coding binary format
 * @params attrs is vector of attributes (e.g. 1.0.1.0.2.85.4)
 * @return binary vector
 */
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

/**
 * @brief split strings into vector via delimiter
 * @param str is string
 * @param d is delimiter that won't be in vector
 * @return split string
 */
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

/**
 * @brief joins vector of strings into one and insert between them delimiter
 * @param str_B is iterator of string begining
 * @param str_E is iterator of string end
 * @param del is delimiter that will be inserted between strings
 * @return joined string
 */
static inline ::std::string StrJoin(
    ::std::vector<::std::string>::iterator str_B,
    ::std::vector<::std::string>::iterator str_E,
    char del = 0)
  {
    ::std::string ret;
    while (str_B != str_E - 1)
      if (del)
        (ret += *(str_B++)).push_back(del);
      else
        ret += *(str_B++);
    ret += *str_B;
    return ret;
  }

/**
 * @brief inserts char into vector of char or string
 * @tparam _VectorT is type of vector - may be string or vector of chars
 * @tparam _PartitionSize is size of chunks between char will be inserted
 * @param vec of chars
 * @param c is character that will be added between chunks
 */
template<
    typename _VectorT,
    int _PartitionSize = 2>
  static inline _VectorT &InsertChar(
      _VectorT &vec,
      char c)
    {
      if (vec.size() > 0)
        for (int i = vec.size() - 1; i > 0; --i)
          if (i % _PartitionSize == 0)
            vec.insert(vec.begin() + i, c);
      return vec;
    }

/**
 * @brief Decodes object name from ANS.1 coding
 * @param msg are binary data
 */
static inline ::std::string decodeObjectName(
    BinaryVectorT &msg)
  {
    ::std::vector<uint32_t> data {
        (uint32_t)(msg[0]/40), (uint32_t)(msg[0]%40)
    };
    /*if number was multi-byte or single*/
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