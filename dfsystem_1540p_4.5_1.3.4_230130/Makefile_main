CC = arm-linux-g++
AR = arm-linux-ar
TARGET = dfsystem.arm
SYSTEMHEADER = /opt/dfsystem/include/dfsystem/
SYSTEMLIB = /opt/dfsystem/lib/
TARGETSHARE = /opt/dfsystem/.df/
PMBUSHEADER = /opt/dfsystem/include/pmbus
DBGFLAGS = _LIBDFSYSTEM
#CXXFLAGS = -Wall -O2 -I. -D$(DBGFLAGS) -D_BUSTYPE
CXXFLAGS = -Wall -O2 -I. -I$(PMBUSHEADER) -D$(DBGFLAGS) -D_BUSTYPE -D_COMPILEMODE_=$(COMPILEMODE)
#LDFLAGS = -shared -o lib$(TARGET).so
LDFLAGS	= -shared -L/opt/dfsystem/lib -lpmbus.arm -o lib$(TARGET).so
OBJS = if.o \
	scramble.o \
	syscompiler.o \
	tgcompiler.o \
	pgcompiler.o \
	hwcontrol.o \
	swcontrol.o \
	deviceif.o

.cpp.o:
	$(CC) -c $(CXXFLAGS) -o $@ $<

All: $(TARGET)

all:
	make ARM=.arm
	make lib
	make diet

debug:
	make clean
	make CC=g++ DBGFLAGS=_DEBUG COMPILEMODE=1
	make diet
	make ARM=.arm DBGFLAGS=_DEBUG COMPILEMODE=2
	make diet

lib:
	rm -rf lib$(TARGET).a
	$(AR) rscv lib$(TARGET).a $(OBJS)
#	rm -rf lib$(TARGET).arm.a
#	$(AR) rscv lib$(TARGET).arm.a $(OBJS)

diet:
	rm -rf $(OBJS)

clean:
	make diet
	rm -rf libdfsystem.*

copy :
	cp struct.h template.h $(SYSTEMHEADER)../
	cp -rf *.h $(SYSTEMHEADER)
	cp -rf libdfsystem.* $(SYSTEMLIB)
	cp -rf libdfsystem.* $(TARGETSHARE)
	chown testfos:testfos $(SYSTEMLIB)libdfsystem.* $(TARGETSHARE)libdfsystem.*

$(TARGET): $(OBJS)
	$(CPLIB) $(CC) $(LDFLAGS) $(OBJS)
