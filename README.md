# MCS
![MCS_logo](doc/mcslogo_sst.gif)
MCS is a collection of high level classes and functions designed to easily implement the following kind of applications:

1. multi thread applications;
2. network applications (through TCP);
3. database (MySQL) applications;
4. information servers;
5. FITS file access.

MCS contains already a working information server, able to serve several client simultaneosly and give a database access.

If you want to rapidly develop your application without worrying about networking, threading, and database code,
MCS does the hard work for you, but doesn't pretend that you are a good low level programmer, or even that you know what is a socket.
Furthermore MCS offers not only a number of C++ wrappers around the system's library, but a set of high level classes aimed to solve the more common problems.

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

## Compile and install

To avoid autotools requirements with a message like this:
```
...
MCS/config/missing: line 81: aclocal-1.16: command not found
WARNING: 'aclocal-1.16' is missing on your system.
...
```

follow this command sequence:

```
touch configure aclocal.m4 Makefile.in
./configure [... see options ...]
make
sudo make install
```

Also do not forget, at first installation, to make the shared library visible to other packages via
```
(Linux)  sudo ldconfig /usr/local/lib
or
(Mac OS) sudo update_dyld_shared_cache
```

Tested on Linux and Mac OS with Xcode+MacPorts.

See the [documentation](doc/mcs.pdf).
