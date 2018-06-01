// ----------------------------------------------------------------------^
// Copyright (C) 2004, 2005, 2006, 2007 Giorgio Calderone <gcalderone@ifc.inaf.it>
// 
// This file is part of MCS.
// 
// MCS is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// MCS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with MCS; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// 
// ----------------------------------------------------------------------$

/*!
  \mainpage My Customizable Server (MCS)

  \htmlonly
  <a href="http://ross.iasfbo.inaf.it/mcs/" target="_blank">
    <img src="mcslogo_sst.gif" alt="mcslogo" align="left" border="0">
  </a>
  \endhtmlonly

  MCS is a collection of high level classes and functions designed to
  easily implement the following kind of applications:

  - multi thread applications;

  - network applications (through TCP);

  - database (MySQL) applications;

  - information servers;

  - FITS file access.

  MCS contains already a working information server, able to serve several
  client simultaneosly and give a database access.

  If you want to rapidly develop your application without worrying
  about networking, threading, and database code, MCS does the hard
  work for you, but doesn't pretend that you are a good low level
  programmer, or even that you know what is a socket. Furthermore MCS
  offers not only a number of C++ wrappers around the system's
  library, but a set of high level classes aimed to solve the more
  common problems.

  This site contains MCS's reference documentation for all classes and
  functions. For an overview of the MCS facilities and description of
  basic topics check the documentation in \htmlonly <a
  href="mcs.pdf">pdf format</a> \endhtmlonly.  Latest release of the
  MCS package can be found \htmlonly <a
  href="mcs-0.3.3-alpha7.tar.gz">here</a> \endhtmlonly (see
  \htmlonly <a href="ChangeLog">ChangeLog</a> \endhtmlonly).


  \b NOTE: from version 0.3.2-alpha1 the MyRO, DIF and VO tables
  facilities are also distributed as stand-alone packages. (See \ref
  links).

  MCS is continuosly evolving through the addition of new feature and
  correction of bugs, so come back soon for newer release. If you wish
  to contribute to the MCS development, or you think that
  MCS is lacking some important feature, or even if you found a bug,
  please let us know.

  MCS was written by Giorgio Calderone <gcalderone@ifc.inaf.if> and
  Luciano Nicastro <nicastro@iasfbo.inaf.it>. Aldo Spizzichino
  <spizzichino@iasfbo.inaf.it> created the MCS logo
  (each software package out there has a little animal as logo, our is
  a cat).



  \page links Links

  - \b MyRO (My Record Oriented privilege system):
    http://ross.iasfbo.inaf.it/myro

  - \b DIF (Dynamic Indexing Facility):
    http://ross.iasfbo.inaf.it/dif

  - \b VOTPP (VOTable C++ Parser): http://ross.iasfbo.inaf.it/votpp

  - A simple demostration suite for the MCS library: \htmlonly <a
    href="http://ross.iasfbo.inaf.it/demo_mcs/">demo_mcs</a>
    \endhtmlonly

  - IDL user contributed library - version 0.3d: \htmlonly <a
    href="../mcs-download/mcs_usrlib.pro">mcs_usrlib.pro</a>
    \endhtmlonly

  - IDL HEALPix contributed library - version 0.4a: \htmlonly <a 
    href="../mcs-download/mcs_healplib.pro">mcs_healplib.pro</a>
    \endhtmlonly

  - Fortran user contributed library with demo - version 0.1b:
    \htmlonly <a 
    href="../mcs-download/mcs_fdemos_v01c_20100625.tar.gz">mcs_fdemos</a>
    \endhtmlonly
*/
