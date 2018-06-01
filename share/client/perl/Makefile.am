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
EXTRA_DIST = perl2mcs.i

if ENABLE_PERL
  AM_CFLAGS = @PERL_CFLAGS@
  lib_LTLIBRARIES = libperl2mcs.la
  nodist_libperl2mcs_la_SOURCES = perl2mcs.c
  libperl2mcs_la_LIBADD = ../../../src/.libs/libmcs.la

  CLEANFILES = perl2mcs.c perl2mcs.pm

  intrfdir = $(pkgdatadir)/client
#  dist_intrf_DATA = perl2mcs.pm

perl2mcs.c:
	@SWIG@ -includeall -I../../../src -perl -o perl2mcs.c perl2mcs.i
endif
