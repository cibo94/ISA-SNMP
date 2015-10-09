/**
 * @file main.cxx
 * @brief
 * @author Miroslav Cibulka - xcibul10
 * @details
 */

#include <iostream>
#include "../include/params.hxx"
#include "../include/packet.hxx"
#include "../include/pdu.hxx"
#include "../include/pdu_bindings.hxx"
#include "../include/manager.hxx"


#define HELP_TEXT \
"                      SNMP Manager\n" <<\
"                      ============\n" <<\
"\n" <<\
"  ./isaSnmpIfLog [-i interval] -c community_string AGENT\n" <<\
"\n" <<\
" Parameters:\n" <<\
"     -i, --interval:           interval mezi jednotlivými vyčítáními \n" <<\
"                               hodnot v ms, implicitní hodnota 100ms\n" <<\
"     -c, --community_string:   community string pro přístup \n" <<\
"                               k parametrům na SNMP agentu\n" <<\
"     AGENT:                    doménové jméno nebo IP adresa SNMP agenta\n"


#define PORT 161
#define TRAP_PORT 162


int main(
    int argc,
    char *argv[])
  {
    ::std::string interval, community, address;
    ::Params _p;
    _p.addOption(
        'h', "help", [](::std::string)
      { ::std::cout << HELP_TEXT; exit(0); }).addOption(
        'i', "interval", [&interval](::std::string _s)
      { interval = _s; }, ::Params::Required).addOption(
        'c', "community_string", [&community](::std::string _s)
      { community = _s; }, ::Params::Required);
    try
      {
        _p.Parse(argc, argv);
        address = _p[0];
        if (interval == "" || community == "" || address == "")
          throw ::std::runtime_error(
              "Required parameters are interval, community, address!");
        ::Manager manager = createManager(
            community, ::Packet::PDU::Type::GET_REQ, 0xABCDEF, 42, 42);
        ::std::cout << manager.toString() << ::std::endl;
        int i = 1;
        for (auto &ch: BinToStr(manager.toBinary()))
          {
            ::std::cout << ch;
            if (i == 8)
              {
                ::std::cout << ::std::endl;
                i = 0; 
              }
            i++;
          }
      }
    catch (::std::exception &ex)
      {
        ::std::cerr << ex.what() << ::std::endl;
      }
    return 0;
  }
