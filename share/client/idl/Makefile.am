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
EXTRA_DIST = c2idl.pl incl.h idl2mcs.dlm

TEMPH = temp.h

if ENABLE_IDL
  AM_CFLAGS = @IDL_CFLAGS@ -Wno-missing-braces
  lib_LTLIBRARIES = idl2mcs.la
  idl2mcs_la_LDFLAGS = -module
  nodist_idl2mcs_la_SOURCES = idl2mcs.c
  idl2mcs_la_LIBADD = ../../../src/.libs/libmcs.la

  intrfdir = $(pkgdatadir)/client
  dist_intrf_DATA = idl2mcs.dlm

idl2mcs.c:
	$(CC) -DWANT_DESC -E ../../../src/mcs_c.pre.h | grep -v '#' > $(TEMPH)
	@PERL@ c2idl.pl $(TEMPH) idl2mcs.so > c2idl.out

endif

CLEANFILES = idl2mcs.c idl2mcs.pro c2idl.out $(TEMPH)

