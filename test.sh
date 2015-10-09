#!/bin/bash

VERBOSE=0
MAKE=make
CFLAGS="-std=c++14 -Wall -Wextra -pedantic -g"
PROG=isaSnmpIfLog
LDFLAGS=""
LDLIBS=""

function print_help()
  {
    echo "           Test suite for SNMP Manager"
    echo "           ==========================="
    echo
    echo "  ./test.sh [-v] [-e EXE] [-h]"
    echo
    echo " Parameters:"
    echo "    -v:      Verbose tests"
    echo "    -h:      Print this help"
    echo "    -e EXE:  Use executable as tested SNMP manager"
    exit 0
  }

function arg_parse()
  {
    while getopts "ve:h" opt
      do
        case ${opt} in
          v)
            VERBOSE=1
            ;;
          h)
            print_help
            ;;
          e)
            PROG=${OPTARG}
            ;;
        esac
      done
  }

function compile()
  {
    make
    rm build/main.o
    echo "g++ ${1} ${CFLAGS} -c -o build/test.o"
    g++ ${1} ${CFLAGS} -c -o build/test.o
    echo "g++ " build/*.o " ${LDFLAGS} ${LDLIBS} -o test.run"
    g++ build/*.o ${LDFLAGS} ${LDLIBS} -o test.run
  }

function run_test()
  {
    echo "  TEST SUITE ${1}"
    if [[ "${VERBOSE}" -eq "0" ]]
      then
        compile $1 >/dev/null 2>&1
      else
        compile $1
      fi
    if ./test.run
      then
        echo -e "  TEST SUITE '${1}' \e[0;32mPASSED\e[0m!"
      else
        echo -e "  TEST SUITE '${1}' \e[0;31mFAILED\e[0m!"
      fi
  }

arg_parse $@
printf '=%.0s' {1..$(tput cols)}
echo
run_test tests/test_package.cxx
printf '=%.0s' {1..$(tput cols)}
echo
