//
// Created by onionka on 13/10/15.
//

#pragma once

#include <iostream>


class logging
  {
public:
    logging(
        std::ostream &out = ::std::cout,
        ::std::string stamp = ::std::string("[") + get_stamp() + "] ");

    static logging warn();

    static logging error();

    static logging log();

    template<typename _ValT>
      logging &operator<<(_ValT &&v)
        { _out << v; return *this; }

    ~logging();

private:
    ::std::ostream &_out;

private:
    static ::std::string get_stamp();
  };