/**
 * @file params.hxx
 * @brief Argument parser
 * @author Miroslav Cibulka - xcibul10
 * @details Argument parser
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#ifdef __cplusplus
}
#endif

#include <unordered_map>
#include <vector>
#include <functional>


class Params
  {
public:
    enum _ParamReqE
      {
        Empty, Required, Optional
      };

    ::Params &addOption(
        char opt,
        ::std::string opt_long,
        ::std::function<void(::std::string)> func = [](::std::string) { },
        ::Params::_ParamReqE requirement = Empty);

    Params() = default;

    void Parse(
        int argc,
        char **argv);

    ::std::string &operator[](
        char c);

    bool exists(char c);

private:
    ::std::unordered_map<
        char,
        ::std::string> _arg_map;
    ::std::unordered_map<
        char,
        ::std::function<void(::std::string)>> _options;
    ::std::vector<struct ::option> _long_options;
    ::std::vector<::std::string> _long_names;
    ::std::vector<::std::string> _leftover_strings;
    ::std::string _opts;
  };
