/**
 * @file log.hxx
 * @brief Logging module with timestamp
 * @author Miroslav Cibulka - xcibul10
 * @details
 *    contains timestamp and logging class
 */


#pragma once

#include <iostream>

/**
 * @brief logging class for dumping data to stream
 */
class logging
  {
public:
    /**
     * @brief creates logging object from stream and specifies style of dumping
     */
    logging(
        std::ostream &out = ::std::cout,
        ::std::string stamp = get_stamp() + "; ");

    /**
     * @brief warn type of logging, creates specific logging object with
     *        better dump style
     */
    static logging warn();

    /**
     * @brief error type of logging, creates specific logging object with
     *        better dump style
     */
    static logging error();

    /**
     * @brief log type of logging, creates specific logging object with
     *        better dump style
     */
    static logging log();

    /**
     * @brief dumping operator
     */
    template<typename _ValT>
      logging &operator<<(_ValT &&v)
        { _out << v; return *this; }

    ~logging();

private:
    /** output stream -> default is ::std::cout */
    ::std::ostream &_out;

private:
    /** @return timestamp with millisecond-precision */
    static ::std::string get_stamp();
  };