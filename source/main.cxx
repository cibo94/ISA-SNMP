/**
 * @file main.cxx
 * @brief
 * @author Miroslav Cibulka - xcibul10
 * @details
 */

#include <iostream>
#include <thread>
#include "../include/params.hxx"
#include "../include/packet.hxx"
#include "../include/pdu.hxx"
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

bool is_number(const std::string& s)
  {
    return !s.empty() && std::find_if(
        s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
  }

::std::string pretty_hex_bin(::std::string str)
  {
    int i = 1;
    ::std::string out;
    for (auto &ch: str)
      {
        out.push_back(ch);
        if (i == 8)
          {
            out += "\n";
            i = 0;
          }
        i++;
      }
    return out;
  }

void messenger(::std::string community, ::Manager &manager)
  {
    auto snmp = manager.send(
        community, ::Packet::PDU::Type::GET_BULK_REQ,
        1, 1, 1).retrieve();
    if (snmp == nullptr)
      throw ::std::runtime_error(
          "Bad response from server!");
    ::std::cout << snmp->getStrRepre() << ::std::endl <<
    ::pretty_hex_bin(::BinToStr(snmp->getBinary())) <<
    ::std::endl;
  }

::std::vector<::std::thread> threads;

int main(
    int argc,
    char *argv[])
  {
    /* Default values of parameters */
    ::std::string interval = "100", community, address;
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

        /*  Parameter checks */
        if (community == "" || address == "")
          throw ::std::runtime_error(
              "Required parameters are community, address!");
        if (!is_number(interval))
          throw ::std::runtime_error(
              "Interval must be numeric!");

        /* Main loop for sending and retrieving SNMP messages */
        ::Manager manager(address, PORT);
        while (1)
          {
            threads.push_back(
                ::std::thread(messenger, community, ::std::ref(manager)));
            ::std::this_thread::sleep_for(
                ::std::chrono::milliseconds(
                    ::std::stoi(interval)));
          }
      }
    catch (::std::exception &ex)
      {
        ::std::cerr << ex.what() << ::std::endl;
      }
    return 0;
  }
