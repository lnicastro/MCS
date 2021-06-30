# See/use Makefile_test to compile test programs
# 
CC ?= gcc
CCPP ?= g++

FFLAGS = `mcs-config --cflags`
CFLAGS = `mcs-config --cflags`
CXXFLAGS = `mcs-config --cflags`
LDFLAGS = `mcs-config --libs`
LIBS = `mcs-config --libs`


TEST = server parsetime commandparser db thread data \
       client record urlreader fitsreader piperw \
       c_bind f_bind

all: $(TEST)

clean:
	rm -f $(TEST) *.o

