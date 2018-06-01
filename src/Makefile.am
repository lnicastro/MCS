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
EXTRA_DIST = mcsifd.h mcs_c.pre.h beautify.pl \
             prepConst.pl mcsmsg.pre c2fortran.pl doc.h

mcsmsg.hh: mcsmsg.pre prepConst.pl
	cat mcsmsg.pre | @PERL@ prepConst.pl  > mcsmsg.hh 2> mcsmsg.cc

mcs_fortran.c: Makefile mcs.hh mcs_c.h
	$(CC) -DWANT_DESC -E -P mcs_c.pre.h | grep -v '#' | @PERL@ c2fortran.pl $(build_cpu)_$(build_os) >$@

mcs_c.h: mcs_c.pre.h mcsifd.h
	$(CXX) -x c++ -DWANT_HEAD -E -CC mcs_c.pre.h  | @PERL@ beautify.pl > mcs_c.h

mcs_c.cc: mcs_c.pre.h mcsifd.h
	$(CXX) -x c++ -DWANT_CODE -E -CC mcs_c.pre.h  | @PERL@ beautify.pl > mcs_c.cc

lib_LTLIBRARIES = libmcs.la

include_HEADERS =                    \
		   mcs_config.h      \
		   mcs.hh            \
		   mcsmsg.hh         \
		   mcstypes.hh       \
                   mcs_c.h           \
                   mcs_fortran.inc   \
                   mcs_facility.inc

libmcs_la_LDFLAGS = -version-info @INTERFACE_VERSION@
libmcs_la_CXXFLAGS = -I../contrib/libjson

libmcs_la_SOURCES =                  \
		   mcs_config.h      \
		   mcs.hh            \
		   mcsmsg.hh         \
		   mcsmsg.cc         \
		   mcstypes.hh       \
                   mcs_c.h           \
                   mcs_c.cc          \
		   Utils.cc          \
		   Readers.cc        \
		   Writers.cc        \
		   Event.cc          \
		   Serializable.cc   \
		   Socket.cc         \
		   Thread.cc         \
		   Data.cc           \
		   Record.cc         \
		   Conf.cc           \
		   CommandParser.cc  \
		   Db.cc             \
		   BaseThread.cc     \
		   UserThread.cc     \
		   LocalThread.cc    \
		   Env.cc            \
		   Server.cc         \
                   Client.cc         \
		   Main.cc


nodist_libmcs_la_SOURCES = mcs_fortran.c
CLEANFILES = mcs_fortran.c


bin_PROGRAMS = mcsexec b64codec killmcs

mcsexec_SOURCES = mcsexec.cc

b64codec_SOURCES = b64codec.cc
b64codec_LDADD = ../contrib/libjson/.libs/libmcsjson.la .libs/libmcs.la

if ENABLE_OPENSSL
   bin_PROGRAMS += mcscreatekey

   mcscreatekey_SOURCES = mcscreatekey.cc
   mcscreatekey_LDADD = ../contrib/libjson/.libs/libmcsjson.la .libs/libmcs.la
endif

killmcs_SOURCES = killmcs.cc
killmcs_LDADD = ../contrib/libjson/.libs/libmcsjson.la .libs/libmcs.la
