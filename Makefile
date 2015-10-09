# @author Miroslav Cibulka - xcibul10
# @details
#     Build system for project SNMP manager
#       Build:
#         make
#       Test:
#         make test
#       Install:
#         sudo make install PREFIX=/usr

# With LD you are missing essential libraries to be linked
LD          = ${CXX}

# Flags
CXXFLAGS    = -std=c++14 -Wall -Wextra -pedantic -g
LDFLAGS     =
LDLIBS      =

# Target(s)
TARGET      = isaSnmpIfLog
BIN         = ${PREFIX}/bin
INSTALLED   = ${BIN}/isaSnmpIfLog

# Source files
SOURCE      = source/main.cxx source/params.cxx source/packet.cxx

# This makefile for packaging
MAKEFILE    = Makefile

# Tarball to be delivered
TARBALL     = xcibul10.tar

# Build Directory - where object files will be
BUILD       = build
OBJECTS     = ${BUILD}/main.o ${BUILD}/params.o ${BUILD}/packet.o

# Macros with params:
#   first   -> target
#   second  -> source
COMPILE.cxx = ${CXX} ${CXXFLAGS} -c -o
RM.rf       = -rm -rf
TAR.cf      = tar cf
INSTALL.x   = install -m +x

isaSnmpIfLog: ${BUILD} ${OBJECTS}
	${LD} ${OBJECTS} -o isaSnmpIfLog ${LDFLAGS} ${LDLIBS}

all: ${TARGET} ${TARBALL}

install: ${TARGET}
	${INSTALL.x} ${TARGET} ${BIN}/

${BUILD}:
	[ -d ${BUILD} ] || mkdir ${BUILD}

${BUILD}/params.o: source/params.cxx include/params.hxx
	${COMPILE.cxx} ${BUILD}/params.o source/params.cxx

${BUILD}/packet.o: source/packet.cxx include/packet.hxx include/bitmap.hxx
	${COMPILE.cxx} ${BUILD}/packet.o source/packet.cxx

${BUILD}/main.o: source/main.cxx include/packet.hxx include/params.hxx \
				include/bitmap.hxx  include/pdu.hxx  include/pdu_bindings.hxx
	${COMPILE.cxx} ${BUILD}/main.o source/main.cxx

.PHONY: clean arch tar uninstall test
tar: ${TARBALL}
arch: ${TARBALL}
clean:
	${RM.rf} ${BUILD} ${TARGET} ${TARBALL}

uninstall:
	${RM.rf} ${INSTALLED}

test:
	./test.sh -v

${TARBALL}: ${SOURCE} ${MAKEFILE}
	${TAR.cf} ${TARBALL} ${SOURCE} ${MAKEFILE}
