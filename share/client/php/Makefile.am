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

EXTRA_DIST = php2mcs.i php_php2mcs.h

#Warning1 Warning2


if ENABLE_PHP
  AM_CFLAGS = $(CFLAGS) @PHP_CFLAGS@ \
              -Wno-unused-variable -Wno-unused-label -Wno-unused-function -fno-strict-aliasing

  lib_LTLIBRARIES = php_php2mcs.la
  php_php2mcs_la_LDFLAGS = -module
  nodist_php_php2mcs_la_SOURCES = php2mcs.c
  php_php2mcs_la_LIBADD = ../../../src/.libs/libmcs.la

  CLEANFILES = php2mcs.c php2mcs.php

  intrfdir = $(pkgdatadir)/client
#dist_intrf_DATA = php2mcs.php

php2mcs.c: Makefile
	@if test "@SWIG_VER@" \> "1.3.36" ; then \
	  echo SWIG version greater than 1.3.36. Using PHP 5.; \
	  @SWIG@ -includeall -I../../../src -noproxy -php -o php2mcs.c php2mcs.i; \
	else \
	  echo SWIG version lower than 1.3.37. Using PHP 4.; \
	  @SWIG@ -includeall -I../../../src -noproxy -php4 -o php2mcs.c php2mcs.i; \
	fi


install-exec-hook:
	mkdir -p @PHP_EXTDIR@
	ln -fs $(libdir)/php_php2mcs.so @PHP_EXTDIR@/php2mcs.so
#cp php2mcs.php $(intrfdir)
	@if test "x@PHP_INCPATH@" = x./ ; then \
	  echo WARNING! ; \
	  echo PHP has no include dir defined. php2mcs.php goes into $(intrfdir); \
	  echo ; \
	else \
	  ln -fs $(intrfdir)/php2mcs.php @PHP_INCPATH@/php2mcs.php ; \
	fi

#ln -s $(intrfdir)/php2mcs.php @PHP_EXTDIR@/../../php2mcs.php

endif
