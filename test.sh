#!/bin/bash

VERBOSE=0
MAKE=make
CFLAGS="-std=c++11 -Wall -Wextra -pedantic -g"
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
    ${MAKE}
  }

counter=0

function run_test()
  {
		valgrind ${PROG} $@
		counter=$((counter+1))
  }

arg_parse $@
compile
run_test