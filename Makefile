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
CXXFLAGS    = -std=c++11 -Wall -Wextra -pedantic
LDFLAGS     =
LDLIBS      =

# Target(s)
TARGET      = isaSnmpIfLog
BIN         = ${PREFIX}/bin
DESTDIR     = ${BIN}

# Source files
SOURCE      = source/main.cxx source/params.cxx source/packet.cxx \
			  source/manager.cxx source/log.cxx
INCLUDE     = include/packet.hxx include/params.hxx include/bitmap.hxx \
			  include/pdu.hxx include/pdu_bindings.hxx include/manager.hxx \
			  include/log.hxx

# This makefile for packaging
MAKEFILE    = Makefile

# This is required for CLion and also can check requirements
CMAKELISTS  = CMakeLists.txt

# Tarball to be delivered
TARBALL     = xcibul10.tar

# Build Directory - where object files will be
BUILD       = build
OBJECTS     = ${BUILD}/main.o ${BUILD}/params.o ${BUILD}/packet.o \
			  ${BUILD}/manager.o ${BUILD}/log.o

# Macros with params:
#   first   -> target
#   second  -> source
COMPILE.cxx = ${CXX} ${CXXFLAGS} -c -o
RM.rf       = -rm -rf
TAR.cfv     = tar cfv
INSTALL.x   = install -m +x

isaSnmpIfLog: ${BUILD} ${OBJECTS}
	${LD} ${OBJECTS} -o isaSnmpIfLog ${LDFLAGS} ${LDLIBS}

all: ${TARGET} ${TARBALL}

install: ${TARGET}
	${INSTALL.x} ${TARGET} ${BIN}/

${BUILD}:
	[ -d ${BUILD} ] || mkdir ${BUILD}

${BUILD}/params.o:  source/params.cxx include/params.hxx
	${COMPILE.cxx} ${BUILD}/params.o source/params.cxx

${BUILD}/packet.o:  source/packet.cxx include/packet.hxx include/bitmap.hxx
	${COMPILE.cxx} ${BUILD}/packet.o source/packet.cxx

${BUILD}/manager.o: source/manager.cxx include/manager.hxx include/packet.hxx \
					include/pdu.hxx include/pdu_bindings.hxx
	${COMPILE.cxx} ${BUILD}/manager.o source/manager.cxx

${BUILD}/log.o:     source/log.cxx include/log.hxx
	${COMPILE.cxx} ${BUILD}/log.o source/log.cxx

${BUILD}/main.o:    source/main.cxx include/packet.hxx include/params.hxx \
					include/bitmap.hxx include/pdu.hxx include/log.hxx \
					include/manager.hxx include/pdu_bindings.hxx
	${COMPILE.cxx} ${BUILD}/main.o source/main.cxx

.PHONY: clean arch tar uninstall test
tar: ${TARBALL}
arch: ${TARBALL}
clean:
	${RM.rf} ${BUILD} ${TARGET} ${TARBALL}

uninstall:
	${RM.rf} ${DESTDIR}/${TARGET}

test:
	./test.sh -v

${TARBALL}: ${SOURCE} ${MAKEFILE} ${CMAKELISTS} ${INCLUDE}
	${TAR.cfv} ${TARBALL} ${SOURCE} ${MAKEFILE} ${CMAKELISTS} ${INCLUDE}\
		manual.pdf README
