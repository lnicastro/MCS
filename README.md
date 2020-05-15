# MCS
![MCS_logo](doc/mcslogo_sst.gif)
MCS is a collection of high level classes and functions designed to easily implement the following kind of applications:

1. multi thread applications;
2. network applications (through TCP);
3. database (MySQL / MariaDB) applications;
4. information servers;
5. FITS file access.

MCS contains already a working information server, able to serve several clients simultaneosly and give a database access.

If you want to rapidly develop your application without worrying about networking, threading, and database code,
MCS does the hard work for you, but doesn't pretend that you are a good low level programmer, or even that you know what is a socket.
Furthermore MCS offers not only a number of C++ wrappers around the system's library, but also a set of high level classes aimed to solve the more common problems.

## Requirements
1. MySQL / MariaDB (libmysqlclient-dev)
2. make or gmake
3. Perl
4. PCRE (libpcre++-dev)

MySQL 8 requires
- libssl-dev

Optional but suggested:
- cfitsio (libcfitsio-dev)
- curl (libcurlpp-dev)
- swig

You will also need `pdflatex` to recreate `mcs.pdf` (can be disabled in `doc/Makefile.in`).

## Compile and install

```
./configure [... see options ...]
make
sudo make install
```

At first installation, do not forget to make the shared library visible to other packages via
```
(Linux)  sudo ldconfig /usr/local/lib
or
(Mac OS) sudo update_dyld_shared_cache
```

Tested on Linux and Mac OS with Xcode+MacPorts.

See the [documentation](doc/mcs.pdf) or the [Doxygen reference doc](http://ross.oas.inaf.it/mcs/). Note: not updated for the last versions. 

## Known issues

- PCRE on Mac OS

systems with a recent version of Xcode (4.2 or later) use `clang` as default compiler, whereas MCS requires `gcc`.
Apparently, at some point, the MacPorts installed PCRE libraries became incompatible with calls from `gcc` (at least v.6) compiled libraries (or buggy) and at runtime one can get an error like this:
```
dyld: lazy symbol binding failed: Symbol not found: __ZN7pcrecpp2RE4InitERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEPKNS_10RE_OptionsE
  Referenced from: /usr/local/lib/libmcs.4.dylib
  Expected in: flat namespace
```

The easiest solution is to use a local compiled PCRE library: download and configure it choosing a convenient directory and use the `--with-pcre=` option when configuring MCS. For example, in a directory if your choice:
```
wget https://ftp.pcre.org/pub/pcre/pcre-8.42.tar.bz2
tar jxvf pcre-8.42.tar.bz2
cd pcre-8.42
./configure --prefix=/usr/local/pcre
make
make install
```

Then go into the MCS source directory and run (e.g.):
```
./configure --enable-cfitsio --enable-idl --with-pcre=/usr/local/pcre
```

- `autotools` requirements

If you download the code via ``git clone``, and see a message like this:
```
...
MCS/config/missing: line 81: aclocal-1.16: command not found
WARNING: 'aclocal-1.16' is missing on your system.
...
```

then, before the `configure` and `make` commands, execute:

```
touch configure aclocal.m4 Makefile.in src/mcs_config.h.in
```

For any other issue please contact the authors.

## IDL contributed libraries

In order to help IDL users to write code querying MySQL/MariaDB tables, in the directory `contrib/libidl` you can find two user contributed libraries and some demo programs.
You can edit and adapt them to better meet your needs. If you have request of changes / additions or you want to share your code, just let us know. 

## Fortran contributed library

TBD. See MCS reference website.
