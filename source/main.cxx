/**
 * @file main.cxx
 * @brief Main program
 * @author Miroslav Cibulka - xcibul10
 * @details Main program
 */

#include <iostream>
#include <thread>
#include "../include/params.hxx"
#include "../include/packet.hxx"
#include "../include/pdu.hxx"
#include "../include/manager.hxx"
#include "../include/log.hxx"


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
        s.begin(), s.end(),
        [](char c) { return !std::isdigit(c); }) == s.end();
  }

::std::string pretty_hex_bin(::std::string str)
  {
    int i = 1;
    ::std::string out;
    for (auto &ch: str)
      {
        out.push_back(ch);
        if (!(i % 2))
          out.push_back(' ');
        if (i == 8)
          {
            out += "\n";
            i = 0;
          }
        i++;
      }
    return out;
  }

void messenger(
    ::std::string community,
    ::Manager &manager)
  {
    using namespace ::Packet::PDU;

    ::std::string first = "1.3.6.1.2.1.2.2";
    ::std::string obj = first;
    /* WTF - after exception logs were multiplied */
    ::std::string repz;
    ::std::vector<::std::string> rets;
    ::std::string last_base_obj = first;
    int i = 0;
    while (true)
      {
        try
          {
            auto snmp = manager.send(
                community, ::Packet::PDU::Type::GET_NEXT_REQ, 0x42,
                Error::noError, 0, &obj, {}, ::DataTypesE::NULL_T).retrieve();
            if (repz == obj)
              continue;
            if (obj.find(first) == obj.npos)
              break;
            if (snmp == nullptr)
              throw ::std::runtime_error(
                  "No response from server!");

            //::logging::log() << snmp->getStrRepre();

            auto toks = StrSplit(obj, '.');
            ::std::string base_obj = StrJoin(toks.begin(), toks.end() - 1, '.');
            if (base_obj != last_base_obj)
              {
                i = 0;
                last_base_obj = base_obj;
              }

            if (rets.size() <= (unsigned long)i)
              rets.resize((size_t) i + 1);

            rets[i] += " " + snmp->getStrRepre() + ";";
            repz = obj;
            i++;
          }
        catch (::std::exception &ex)
          {
            ::logging::error() << ex.what();
          }
      }
    ::logging() << StrJoin(rets.begin(), rets.end(), ' ');
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
        auto inter = ::std::stoull(interval);
        ::std::vector<::std::thread> threads;
        while (1)
          {
            ::messenger(community, manager);
            ::std::this_thread::sleep_for(
                ::std::chrono::milliseconds(inter));
          }
      }
    catch (::std::exception &ex)
      {
        ::std::cerr << ex.what() << ::std::endl;
      }
    return 0;
  }
