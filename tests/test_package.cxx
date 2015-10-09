
#include "../include/packet.hxx"
#include <iostream>


template< size_t _N>
  ::std::vector<::std::string> get_str(
      ::Packet<_N> _pck)
    {
      ::std::vector<::std::string> _out;
      ::std::string matrix = _pck.getPacket().to_string();
      auto beg = matrix.begin();
      for (size_t i = 0; i < _N + 6; ++i, beg += 32)
        _out.emplace_back(beg, beg + 32);
      return _out;
    }

template<size_t _N>
  bool check_vector(::std::vector<::std::string> _rows)
    {
      int ret = 0;
      for (auto s: _rows)
        {
          ::std::bitset<32> sSet (s);
          if (sSet.to_ulong() == _N)
            {
              ::std::cout << "Comparing '" << s << "' with '"
              << ::std::bitset<32>(_N).to_string() <<
              "' ... \033[0;32mOK\033[0m" << ::std::endl;
            }
          else
            {
              ::std::cerr << "Comparing '" << s << "' with '"
              << ::std::bitset<32>(_N).to_string() <<
              "' ... \033[0;31mERR\033[0m" << ::std::endl;
              ret++;
            }
        }
        return !ret;
    }

template<size_t _N>
  bool _test(::Packet<_N> _pck)
    {
      _pck.setVersion(_N);
      _pck.setCommunityString(_N);
      _pck.setPDUType(_N);
      _pck.setRequestID(_N);
      _pck.setErrorStatus(_N);
      _pck.setErrorIndex(_N);
      for (size_t n = 0; n < _N; ++n)
        _pck.setObjectNameNValue(n, _N);
      return check_vector<_N>(get_str(_pck));
    }

bool _and()
  { return true; }

template<
    typename _BoolT,
    typename ..._BoolsT>
  bool _and(_BoolT _b, _BoolsT ..._bs)
    { return _b && _and(_bs...); }

template<
    size_t ..._N>
  bool test()
    { return _and(_test(::Packet<_N>())...); }

int main()
  { return !test<1,2,3,4,5,6,7,8,9,10,11,12,13,14>(); }
