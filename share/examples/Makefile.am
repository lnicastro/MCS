# ----------------------------------------------------------------------^
# Copyright (C) 2004, 2005, 2006, 2007, 2008 Giorgio Calderone
# (mailto: <gcalderone@ifc.inaf.it>)
# 
# This file is part of MCS.
# 
# MCS is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# MCS is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with MCS; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
# 
# ----------------------------------------------------------------------$

bin_PROGRAMS = data data1 db1 db2 server1 client1 client_c \
	client buffer pipe command_parser conf datetime \
	fitsimg fits2dbt

data_SOURCES = data.cc
data1_SOURCES = data1.cc
db1_SOURCES = db1.cc 
db2_SOURCES =  db2.cc
server1_SOURCES = server1.cc
client1_SOURCES = client1.cc
client_c_SOURCES = client_c.c
client_SOURCES = client.cc
buffer_SOURCES = buffer.cc
pipe_SOURCES = pipe.cc
command_parser_SOURCES = command_parser.cc
conf_SOURCES = conf.cc
datetime_SOURCES = datetime.cc
fitsimg_SOURCES = fitsimg.cc
fits2dbt_SOURCES = fits2dbt.cc

#AM_LDFLAGS = `mcs-config --libs`
#AM_LDFLAGS = `/bin/sh ../scripts/mcs-config --libs`

data_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la
data1_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la
db1_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la
db2_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la
server1_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la
client1_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la
client_c_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la
client_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la
buffer_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la
pipe_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la
command_parser_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la
conf_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la
datetime_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la
fitsimg_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la
fits2dbt_LDADD = ../../contrib/libjson/.libs/libmcsjson.la ../../src/.libs/libmcs.la

CLEANFILES = $(bin_PROGRAMS)

