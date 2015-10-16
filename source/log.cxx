/**
 * @file log.cxx
 * @brief Logging module with timestamp
 * @author Miroslav Cibulka - xcibul10
 * @details
 *    contains timestamp and logging class
 */


#include "../include/log.hxx"
#include <chrono>
#include <iomanip>
#include <chrono>
#include <sstream>


::logging::logging(
    std::ostream &out,
    ::std::string stamp) :
    _out(out)
  { _out << stamp; }

logging logging::warn()
  { return logging(::std::cerr, "WARNING {" + get_stamp() + "} "); }

logging logging::error()
  { return logging(::std::cerr, "ERROR {" + get_stamp() + "} "); }

logging logging::log()
  { return logging(::std::cout, "LOG: {" + get_stamp() + "} "); }

::logging::~logging()
  {
    _out << ::std::endl;
  }

::std::string logging::get_stamp()
  {
    ::tm localTime;
    ::std::chrono::system_clock::time_point t = ::std::chrono::system_clock::now();
    ::time_t now = std::chrono::system_clock::to_time_t(t);
    ::localtime_r(&now, &localTime);
    const std::chrono::duration<double> tse = t.time_since_epoch();
    ::std::chrono::seconds::rep milliseconds = ::std::chrono::duration_cast<
        ::std::chrono::milliseconds>(tse).count() % 1000;
    char buffer[26];
    struct ::tm *tm_info;
    ::time(&now);
    tm_info = ::localtime(&now);
    ::std::string ret;
    ::strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    ret = buffer;
    ::std::ostringstream ss;
    ss << ::std::setfill('0') << ::std::setw(3) << ::std::to_string(milliseconds);
    ret = ret + "." + ss.str();
    return ret;
  }
