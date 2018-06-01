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

EXTRA_DIST = admin.tex mcs.tex old_mcs.tex Doxyfile                     \
	     mcslogo.pdf mcslogo.gif mcslogo_sss_c.gif mcslogo_ssst.gif \
	     mcslogo_sst.gif mcslogo_st.gif mcs-foot.html.in            \
             mcslogo_ssss_c.gif includes html


doxy: Doxyfile
	mkdir -p html
	cp mcslogo_sst.gif mcslogo_ssss_c.gif html
	cat mcs-foot.html.in | sed "s/\@VERSION\@/`cat ../version`/g" | \
	                       sed "s/\@TIMESTAMP\@/`date -u`/g" > mcs-foot.html
	doxygen

mcs.pdf: mcs.tex
	make -C includes pdf
	pdflatex mcs.tex
	pdflatex mcs.tex


prepdist: doxy mcs.pdf


docdir = $(pkgdatadir)/doc
dist_doc_DATA = mcs.pdf

install-data-hook:
	cp -r html $(docdir)

uninstall-hook:
	rm -rf  $(docdir)/html
