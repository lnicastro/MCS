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
#EXTRA_DIST = test examples
EXTRA_DIST = test

# The order is important: first scripts (to create mcs-config), then others
SUBDIRS =  scripts client examples


#dist-hook:
prepdist:
	make -C examples clean
	make -C test     clean

clean-local:
	make -C examples clean
	make -C test     clean


install-data-hook:
	mkdir -p ${prefix}/share/mcs

#	cp -r test     ${prefix}/share/mcs
#	cp -r examples ${prefix}/share/mcs
#	cp -r client   ${prefix}/share/mcs
