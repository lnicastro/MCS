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


server: server.o
	$(CCPP) -o $@ $@.o $(LDFLAGS)

parsetime: parsetime.o
	$(CCPP) -o $@ $@.o $(LDFLAGS)

commandparser: commandparser.o
	$(CCPP) -o $@ $@.o $(LDFLAGS)

db: db.o
	$(CCPP) -o $@ $@.o $(LDFLAGS)

thread: thread.o
	$(CCPP) -o $@ $@.o $(LDFLAGS)

data: data.o
	$(CCPP) -o $@ $@.o $(LDFLAGS)

client: client.o
	$(CCPP) -o $@ $@.o $(LDFLAGS)

record: record.o
	$(CCPP) -o $@ $@.o $(LDFLAGS)

urlreader: urlreader.o
	$(CCPP) -o $@ $@.o $(LDFLAGS)

fitsreader: fitsreader.o
	$(CCPP) -o $@ $@.o $(LDFLAGS)

piperw: piperw.o
	$(CCPP) -o $@ $@.o $(LDFLAGS)

c_bind: c_bind.o
	$(CC) -o $@ $@.o $(LDFLAGS)



FC ?= gfortran
f_bind: f_bind.F
	gfortran `mcs-config --cflags` -Wno-unused-variable -g -O0 -c f_bind.F
	gfortran -o f_bind f_bind.o `mcs-config --libs`





test:  test_commandparser test_db test_thread test_data \
       test_record test_parsetime test_piperw test_urlreader \
       test_fitsreader test_server test_client test_c_bind \
       test_f_bind
	killmcs test

test_commandparser: commandparser
	./commandparser

test_db: db
	./db

test_thread: thread
	./thread

test_data: data
	./data

test_record: record
	./record

test_parsetime: parsetime
	./parsetime

test_piperw: piperw
	./piperw

test_urlreader: urlreader
	./urlreader http://ross.iasfbo.inaf.it/MCS/urlreader_test > aux/urlreader_test
	diff aux/urlreader_test aux/urlreader_test.cmp


test_fitsreader: fitsreader
	./fitsreader `cat fitslist | perl -pe 'chomp; print " aux/";'` | grep -v 'Reading:' > aux/fitsreader.log
	diff aux/fitsreader.cmp aux/fitsreader.log


test_server: server
	./server

test_client: client
	./client

test_c_bind: c_bind
	./c_bind

test_f_bind: f_bind
	./f_bind
