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
EXTRA_DIST = python2mcs.i searchInstallPath.py

if ENABLE_PYTHON
  AM_CFLAGS = @PYTHON_CFLAGS@
  lib_LTLIBRARIES = libpython2mcs.la
  nodist_libpython2mcs_la_SOURCES = python2mcs.c
  libpython2mcs_la_LIBADD = ../../../src/.libs/libmcs.la
  libpython2mcs_la_LDFLAGS = -module -undefined dynamic_lookup -flat_namespace -undefined suppress

#libpython2mcs_la_LDFLAGS = -module -flat_namespace -undefined suppress
#  libpython2mcs_la_LDFLAGS = -dynamclib -module -Xlinker -flat_namespace -undefined suppress

  CLEANFILES = python2mcs.c python2mcs.py

  intrfdir = `python searchInstallPath.py`
#  dist_intrf_DATA = python2mcs.py

python2mcs.c:
	@SWIG@ -includeall -I../../../src -python -o python2mcs.c python2mcs.i
#v0.3.3-alpha7
#	cat python2mcs.c | perl -pe 's/Python.h/python\/Python.h/;' > tmp
#	mv tmp python2mcs.c

install-exec-hook:
	echo python2mcs > $(intrfdir)/python2mcs.pth
	mkdir -p $(intrfdir)/python2mcs
	cp python2mcs.py $(intrfdir)/python2mcs
	ln -fs $(prefix)/lib/libpython2mcs.so $(intrfdir)/python2mcs/_python2mcs.so

endif
