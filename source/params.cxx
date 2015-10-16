/**
 * @file params.cxx
 * @brief Argument parser
 * @author Miroslav Cibulka - xcibul10
 * @details Argument parser
 */

#include "../include/params.hxx"
#include <iostream>


extern "C" {
extern int optopt;
extern int opterr;
extern int optind;
extern char *optarg;
#include <stdarg.h>
}


::Params &::Params::addOption(
    char opt,
    ::std::string opt_long,
    ::std::function<void(::std::string)> func,
    ::Params::_ParamReqE requirement)
  {
    _long_names.push_back(opt_long);
    _long_options.push_back({_long_names.back().c_str(), requirement, NULL, opt});
    _options[opt] = func;
    _opts.push_back(opt);
    switch (requirement)
      {
        case _ParamReqE::Optional:
          _opts += ":";
        case _ParamReqE::Required:
          _opts += ":";
        default:
          break;
      }
    return *this;
  }

void ::Params::Parse(
    int argc,
    char **argv)
  {
    _long_options.push_back({0, 0, 0, 0});
    auto long_opts = _long_options.data();
    auto short_opts = _opts.c_str();
    int should_exit = 0;
    for (int ch = getopt_long(argc, argv, short_opts, long_opts, NULL);
         ch != -1;
         ch = getopt_long(argc, argv, short_opts, long_opts, NULL))
      {
        if (ch == '?')
          ++should_exit;
        else
          {
            ::std::string opt_arg(optarg == NULL ? "" : (const char *) optarg);
            _arg_map[ch] = opt_arg;
            _options[ch](opt_arg);
          }
      }
    if (should_exit)
      exit(should_exit);
    if (optind < argc - 1)
      {
        ::std::cerr << "Only one unbind program argument value is allowed!"
                    << ::std::endl;
        exit(1);
      }
    if (optind < argc)
      _arg_map[0] = argv[optind];
  }

::std::string &::Params::operator[](
    char c)
  {
    return _arg_map[c];
  }

bool Params::exists(char c)
  {
    return _arg_map.count(c) > 0;
  }
