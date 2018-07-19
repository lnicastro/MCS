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
AC_INIT([mcs],
        [0.3.3-alpha7],
        [Giorgio Calderone <giorgio.calderone@inaf.it> Luciano Nicastro <luciano.nicastro@inaf.it>],
        [mcs])
INTERFACE_VERSION="4:1:0"

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER(src/mcs_config.h)
AM_INIT_AUTOMAKE([dist-bzip2])
m4_include([m4/ax_compare_version.m4])


#Can't use enable_debug, or enable_*, because these are internal variables for configure
myenable_all=0
myenable_debug=0
myenable_mysql=1      #Enabled by default
myenable_cfitsio=0
myenable_idl=0
myenable_python=0
myenable_php=0
myenable_perl=0
myenable_openssl=0
myenable_curl=0

mypath_cfitsio=""
IDL_BINDIR=""
mypath_python=""
mypath_pcre=""
mypath_mysql=""
mypath_php=""
mypath_perl=""
mypath_openssl=""
mypath_curl=""



#Custom command line arguments
AC_ARG_ENABLE(debug,
    [AC_HELP_STRING([--enable-debug], [Enable debugging options])],
    [if test "$enableval" = "no"; then myenable_debug=0; else myenable_debug=1; fi;],
    [])


AC_ARG_ENABLE(all,
    [AC_HELP_STRING([--enable-all], [Enable all facilities])],
    [if test "$enableval" = "no"; then myenable_all=0; else myenable_all=1; fi;],
    [])

if test $myenable_all -eq 1; then
  myenable_mysql=1
  myenable_cfitsio=1
  myenable_idl=1
  myenable_python=1
  myenable_php=1
  myenable_perl=1
  myenable_openssl=1
  myenable_curl=1
fi


#PCRE is mandatory, so we don't have enable-pcre
AC_ARG_WITH(pcre,
    [AC_HELP_STRING([--with-pcre=PATH], [PATH is the path to the PCRE directory])],
    [mypath_pcre="$withval"])


#CURL is now optional
AC_ARG_ENABLE(curl,
    [AC_HELP_STRING([--enable-curl], [Enable Curl facilities])],
    [if test "$enableval" = "no"; then myenable_curl=0; else myenable_curl=1; fi;],
    [])


AC_ARG_WITH(curl,
    [AC_HELP_STRING([--with-curl=PATH], [PATH is the path to the Curl directory])],
    [mypath_curl="$withval"; myenable_curl=1])



AC_ARG_ENABLE(mysql,
    [AC_HELP_STRING([--enable-mysql], [Enable MySQL facilities])],
    [if test "$enableval" = "no"; then myenable_mysql=0; else myenable_mysql=1; fi;],
    [])

AC_ARG_WITH(mysql,
    [AC_HELP_STRING([--with-mysql=PATH], [PATH is the path to the MYSQL directory])],
    [mypath_mysql="$withval"; myenable_mysql=1])


AC_ARG_ENABLE(openssl,
    [AC_HELP_STRING([--enable-openssl], [Enable Openssl facilities])],
    [if test "$enableval" = "no"; then myenable_openssl=0; else myenable_openssl=1; fi;],
    [])


AC_ARG_WITH(openssl,
    [AC_HELP_STRING([--with-openssl=PATH], [PATH is the path to the OPENSSL directory])],
    [mypath_openssl="$withval"; myenable_openssl=1])


AC_ARG_ENABLE(cfitsio,
    [AC_HELP_STRING([--enable-cfitsio], [Enable CFITSIO facilities])],
    [if test "$enableval" = "no"; then myenable_cfitsio=0; else myenable_cfitsio=1; fi;],
    [])


AC_ARG_WITH(cfitsio,
    [AC_HELP_STRING([--with-cfitsio=PATH], [PATH is the path to the CFITSIO directory])],
    [mypath_cfitsio="$withval"; myenable_cfitsio=1])


AC_ARG_ENABLE(idl,
    [AC_HELP_STRING([--enable-idl], [Enable IDL interface])],
    [if test "$enableval" = "no"; then myenable_idl=0; else myenable_idl=1; fi;],
    [])


AC_ARG_WITH(idl,
    [AC_HELP_STRING([--with-idl=PATH], [PATH is the path to the IDL directory])],
    [IDL_BINDIR="$withval"; myenable_idl=1])


AC_ARG_ENABLE(php,
    [AC_HELP_STRING([--enable-php], [Enable PHP interface])],
    [if test "$enableval" = "no"; then myenable_php=0; else myenable_php=1; fi;],
    [])

AC_ARG_WITH(php,
    [AC_HELP_STRING([--with-php=PATH], [PATH is the path to the PHP directory])],
    [mypath_php="$withval"; myenable_php=1])


AC_ARG_ENABLE(perl,
    [AC_HELP_STRING([--enable-perl], [Enable PERL interface])],
    [if test "$enableval" = "no"; then myenable_perl=0; else myenable_perl=1; fi;],
    [])

AC_ARG_WITH(perl,
    [AC_HELP_STRING([--with-perl=PATH], [PATH is the path to the PERL directory])],
    [mypath_perl="$withval"; myenable_perl=1])


AC_ARG_ENABLE(python,
    [AC_HELP_STRING([--enable-python], [Enable Python interface])],
    [if test "$enableval" = "no"; then myenable_python=0; else myenable_python=1; fi;],
    [])


AC_ARG_WITH(python,
    [AC_HELP_STRING([--with-python=PATH], [PATH is the path to the Python directory])],
    [mypath_python="$withval"; myenable_python=1])


AC_ARG_ENABLE(json,
    [AC_HELP_STRING([--enable-json], [Enable Json interface])],
    [if test "$enableval" = "no"; then myenable_json=0; else myenable_json=1; fi;],
    [])


AC_ARG_WITH(json,
    [AC_HELP_STRING([--with-json=PATH], [PATH is the path to the Json directory])],
    [mypath_json="$withval"; myenable_json=1])



AC_LANG(C++)

AC_DEFINE([CXX_HAS_BUGGY_FOR_LOOPS], [], [Description])
AC_DEFINE([CXX_HAS_NO_BOOL], [], [Description])
AC_DEFINE([NDEBUG], [], [Description])
AC_DEFINE([YOUR_OS], [], [Description])

#LF_CONFIGURE_CC
#LF_CONFIGURE_CXX
#LF_HOST_TYPE
#LF_SET_WARNINGS
AC_PROG_LIBTOOL


EXT_CPPFLAGS="-Wall $CPPFLAGS"
EXT_LDFLAGS="$LDFLAGS"

machtype=`uname -m | grep 64`
if ! test -z $machtype; then EXT_CPPFLAGS="-Wall $CPPFLAGS -DMACH_64"; fi

# LN: check malloc.h
myerr_malloc=""

AC_CHECK_HEADERS([malloc.h],   [], [myerr_malloc="cannot find file malloc.h"])

if test -z "$myerr_malloc"; then
  HAVE_MALLOC=1
  AC_MSG_NOTICE([])
  AC_MSG_NOTICE([malloc.h found.])
  AC_MSG_NOTICE([])
else
  HAVE_MALLOC=0
  AC_MSG_NOTICE([])
  AC_MSG_NOTICE([No malloc.h for this system.])
  AC_MSG_NOTICE([])
fi


#---------------------------------------------------------------------
# Checks for programs.
AC_PROG_CXX
#LT_INIT



#perl binary
AC_PATH_PROG([PERL], [perl], [])
if test -z "$PERL"; then AC_MSG_ERROR([cannot find required program: perl (vers. >= 5.8.8)]); fi

#SWIG
if test $myenable_php -eq 1 -o $myenable_perl -eq 1 -o $myenable_python -eq 1; then
  AC_PATH_PROG([SWIG], [swig], [])
  if test -z "$SWIG"; then AC_MSG_ERROR([cannot find required program: swig (vers. >= 1.3.37)]); fi
  SWIG_VER=`$SWIG -version | $PERL selectswig_ver.pl`
fi

AC_SUBST(SWIG_VER)

AC_CHECK_LIB([pthread], main, [], [AC_MSG_ERROR([cannot link library: pthread])])
AC_CHECK_LIB([pthread], pthread_mutexattr_settype,
			[AC_DEFINE([HAVE_PTHREAD_MUTEXATTR_SETTYPE], [], [HAVE_PTHREAD_MUTEXATTR_SETTYPE])])
AC_CHECK_LIB([pthread], pthread_mutexattr_setprotocol,
			[AC_DEFINE([HAVE_PTHREAD_MUTEXATTR_SETPROTOCOL], [], [HAVE_PTHREAD_MUTEXATTR_SETPROTOCOL])])


# LN modif 9/1/2013
# Check for PTHREAD_MUTEX_RECURSIVE_NP
AC_MSG_CHECKING([for PTHREAD_MUTEX_RECURSIVE_NP])

AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <pthread.h>
]], [[
  return (PTHREAD_MUTEX_RECURSIVE_NP);
]])], [
AC_MSG_RESULT([yes])
AC_DEFINE([HAVE_PTHREAD_MUTEX_RECURSIVE_NP], 1,
          [Define to 1 if you have the 'PTHREAD_MUTEX_RECURSIVE_NP' constant.])
], [
AC_MSG_RESULT([no])
])


# Check for PTHREAD_MUTEX_RECURSIVE
AC_MSG_CHECKING([for PTHREAD_MUTEX_RECURSIVE])

AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <pthread.h>
]], [[
  return (PTHREAD_MUTEX_RECURSIVE);
]])], [
AC_MSG_RESULT([yes])
AC_DEFINE([HAVE_PTHREAD_MUTEX_RECURSIVE], 1,
          [Define to 1 if you have the 'PTHREAD_MUTEX_RECURSIVE' constant.])
], [
AC_MSG_RESULT([no])
])


# Check for PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
AC_MSG_CHECKING([for PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP])

AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <pthread.h>
]], [[
  pthread_mutex_t m = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
  return 0;
]])], [
AC_MSG_RESULT([yes])
AC_DEFINE([HAVE_PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP], 1,
          [Define to 1 if you have the 'PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP' constant.])
], [
AC_MSG_RESULT([no])
])


#PCRE library
myerr_pcre=""

if test -z $mypath_pcre; then
  AC_PATH_PROG(PCRE_CONFIG, [pcre-config], [])
else
  AC_PATH_PROG(PCRE_CONFIG, [pcre-config], [], [$mypath_pcre/bin:$PATH])
fi

if test -z "$PCRE_CONFIG"; then
  AC_PATH_PROG(PCRE_CONFIG, [pcre-config], [/usr/local/bin:$PATH])
fi

if test -z "$PCRE_CONFIG"; then
   myerr_pcre="cannot find required program: pcre-config"
else
  PCRE_CFLAGS=`$PCRE_CONFIG --cflags | $PERL selectflags.pl`
  CPPFLAGS="$EXT_CPPFLAGS $PCRE_CFLAGS"

  AC_CHECK_HEADERS([pcre.h],   [], [myerr_pcre="cannot find file pcre.h"])
  AC_CHECK_HEADERS([pcrecpp.h],[], [myerr_pcre="cannot find file pcrecpp.h"])

  PCRE_LDFLAGS=`$PCRE_CONFIG --libs | $PERL selectflags.pl`
  LDFLAGS="$EXT_LDFLAGS $PCRE_LDFLAGS"

  PCRE_LIBS="pcrecpp"

  for l in $PCRE_LIBS; do
    AC_CHECK_LIB([$l], main, [], [myerr_pcre="cannot link library: $l"])
    if ! test -z "$myerr_pcre"; then break; fi
  done
fi

if test -z "$myerr_pcre"; then
  EXT_CPPFLAGS="$CPPFLAGS"
  EXT_LDFLAGS="$LDFLAGS"
  AC_MSG_NOTICE([])
  AC_MSG_NOTICE([PCRE library configured successfully.])
  AC_MSG_NOTICE([])
else
  $PERL requirePkg.pl PCRE http://www.pcre.org 6.4 pcre-6.4.tar.gz --with-pcre
  AC_MSG_ERROR([$myerr_pcre])
fi








#CURL library
myerr_curl=""

if test $myenable_curl -eq 1; then

echo "*** CURL ENABLED ***"
if test -z $mypath_curl; then
  AC_PATH_PROG(CURL_CONFIG, [curl-config], [])
else
  AC_PATH_PROG(CURL_CONFIG, [curl-config], [], [$mypath_curl/bin:$PATH])
fi

if test -z "$CURL_CONFIG"; then
  AC_PATH_PROG(CURL_CONFIG, [curl-config], [/usr/local/bin:$PATH])
fi

if test -z "$CURL_CONFIG"; then
   myerr_curl="cannot find required program: curl-config"
else
  CURL_CFLAGS=`$CURL_CONFIG --cflags | $PERL selectflags.pl`
  CPPFLAGS="$EXT_CPPFLAGS $CURL_CFLAGS"

  AC_CHECK_HEADERS([curl/curl.h],   [], [myerr_curl="cannot find file curl.h"])

  CURL_LDFLAGS=`$CURL_CONFIG --libs | $PERL selectflags.pl`
  LDFLAGS="$EXT_LDFLAGS $CURL_LDFLAGS"

  CURL_LIBS=`$CURL_CONFIG --libs | $PERL selectlibs.pl`

  for l in $CURL_LIBS; do
    AC_CHECK_LIB([$l], main, [], [myerr_curl="cannot link library: $l"])
    if ! test -z "$myerr_curl"; then break; fi
  done
fi

if test -z "$myerr_curl"; then
  EXT_CPPFLAGS="$CPPFLAGS"
  EXT_LDFLAGS="$LDFLAGS"
  AC_MSG_NOTICE([])
  AC_MSG_NOTICE([CURL library configured successfully.])
  AC_MSG_NOTICE([])
else
  $PERL requirePkg.pl CURL http://curl.haxx.se  7.12 curl-7.16.0.tar.bz2 --with-curl
  AC_MSG_ERROR([$myerr_curl])
fi

fi
AC_DEFINE_UNQUOTED([ENABLE_CURL], $myenable_curl, [ENABLE_CURL])
AM_CONDITIONAL(ENABLE_CURL, test $myenable_curl -eq 1)






#MYSQL library
myerr_mysql=""
if test $myenable_mysql -eq 1; then
  if test -z $mypath_mysql; then
    AC_PATH_PROG(MYSQL_CONFIG, [mysql_config], [])
  else
    AC_PATH_PROG(MYSQL_CONFIG, [mysql_config], [], [$mypath_mysql/bin:$PATH])
  fi

  if test -z "MYSQL_CONFIG"; then
    AC_PATH_PROG(MYSQL_CONFIG, [mysql_config], [], [/usr/local/mysql/bin:/usr/local/bin:$PATH])
  fi


  if test -z "MYSQL_CONFIG"; then
     myerr_mysql="cannot find required program: mysql-config"
  else
    MYSQL_CFLAGS=`$MYSQL_CONFIG --cflags | $PERL selectflags.pl`
    CPPFLAGS="$EXT_CPPFLAGS $MYSQL_CFLAGS"

    AC_CHECK_HEADERS([mysql.h],[], [myerr_mysql="cannot find file mysql.h"])

    MYSQL_LDFLAGS=`$MYSQL_CONFIG --libs | $PERL selectflags.pl`
    LDFLAGS="$EXT_LDFLAGS $MYSQL_LDFLAGS"

    MYSQL_LIBS=`$MYSQL_CONFIG --libs | $PERL selectlibs.pl`

    for l in $MYSQL_LIBS; do
      AC_CHECK_LIB([$l], main, [], [myerr_mysql="cannot link library: $l"])
      if ! test -z "$myerr_mysql"; then break; fi
    done
  fi

  if test -z "$myerr_mysql"; then
    EXT_CPPFLAGS="$CPPFLAGS"
    EXT_LDFLAGS="$LDFLAGS"
    AC_MSG_NOTICE([])
    AC_MSG_NOTICE([MYSQL library configured successfully.])
    AC_MSG_NOTICE([])
  else
    $PERL requirePkg.pl MYSQL http://www.mysql.com 5.1.20 mysql-5.1.20.tar.gz --with-mysql --disable-mysql
    AC_MSG_ERROR([$myerr_mysql])
  fi
fi
AC_DEFINE_UNQUOTED([ENABLE_MYSQL], $myenable_mysql, [ENABLE_MYSQL])
AM_CONDITIONAL(ENABLE_MYSQL, test $myenable_mysql -eq 1)

MYSQL_VERSION=`$MYSQL_CONFIG --version | cut -d "." -f -2 | sed -e 's/\./_/g'`
AC_SUBST(MYSQL_VERSION)

CPPFLAGS="$CPPFLAGS -I$mypath_mysql_source"

AX_COMPARE_VERSION([$MYSQL_VERSION],[gt],[5.2])
if test "$ax_compare_version" = "true" ; then
  echo "Warning: MySQL version $MYSQL_VERSION (greater than 5.1). When compiling mysql use 'cmake -DWITH_DEBUG=0 ...'"
fi


#OPENSSL library
myerr_openssl=""
if test $myenable_openssl -eq 1; then
  if test -z $mypath_openssl; then
    AC_PATH_PROG(OPENSSL, [openssl], [])
  else
    AC_PATH_PROG(OPENSSL, [openssl], [], [$mypath_openssl/bin:$PATH])
  fi

  if test -z "OPENSSL"; then
    AC_PATH_PROG(OPENSSL, [openssl], [/usr/local/bin:$PATH])
  fi


  if test -z "OPENSSL"; then
     myerr_openssl="cannot find required program: openssl"
  else
    if ! test -z $mypath_openssl; then
      OPENSSL_CFLAGS="-I$mypath_openssl/include"
    fi
    CPPFLAGS="$EXT_CPPFLAGS $OPENSSL_CFLAGS"

    AC_CHECK_HEADERS([openssl/ssl.h],[], [myerr_openssl="cannot find file openssl/ssl.h"])
    AC_CHECK_HEADERS([openssl/err.h],[], [myerr_openssl="cannot find file openssl/err.h"])

    if ! test -z $mypath_openssl; then
      OPENSSL_LDFLAGS="-L$mypath_openssl/lib"
    fi
    LDFLAGS="$EXT_LDFLAGS $OPENSSL_LDFLAGS"

    OPENSSL_LIBS="ssl crypto"

    for l in $OPENSSL_LIBS; do
      AC_CHECK_LIB([$l], main, [], [myerr_openssl="cannot link library: $l"])
      if ! test -z "$myerr_openssl"; then break; fi
    done
  fi

  if test -z "$myerr_openssl"; then
    EXT_CPPFLAGS="$CPPFLAGS"
    EXT_LDFLAGS="$LDFLAGS"
    AC_MSG_NOTICE([])
    AC_MSG_NOTICE([OPENSSL library configured successfully.])
    AC_MSG_NOTICE([])
  else
    $PERL requirePkg.pl OPENSSL http://www.openssl.org 0.9.7 openssl-0.9.7d.tar.gz --with-openssl --disable-openssl
    AC_MSG_ERROR([$myerr_openssl])
  fi
fi
AC_DEFINE_UNQUOTED([ENABLE_OPENSSL], $myenable_openssl, [ENABLE_OPENSSL])
AM_CONDITIONAL(ENABLE_OPENSSL, test $myenable_openssl -eq 1)




#CFITSIO library
myerr_cfitsio=""
if test $myenable_cfitsio -eq 1; then

  if test -z $mypath_cfitsio; then
    if test -d /usr/include/cfitsio; then
      mypath_cfitsio="/usr/include/cfitsio";
      CFITSIO_CFLAGS="-I$mypath_cfitsio"
    fi
  fi
  if test -z $mypath_cfitsio; then
    if test -d /usr/local/cfitsio; then
      mypath_cfitsio="/usr/local/cfitsio";
    fi
    if test -d /opt/local/cfitsio; then
      mypath_cfitsio="/opt/local/cfitsio";
    fi
    if ! test -z $mypath_cfitsio; then
      CFITSIO_CFLAGS="-I$mypath_cfitsio/include"
    fi
  fi
  
  CPPFLAGS="$EXT_CPPFLAGS $CFITSIO_CFLAGS"

  AC_CHECK_HEADERS([fitsio.h],[], [myerr_cfitsio="cannot find file fitsio.h"])


  if ! test -z $mypath_cfitsio; then
    CFITSIO_LIB="$mypath_cfitsio/lib"
    CFITSIO_LDFLAGS="-L$CFITSIO_LIB -Wl,-rpath,$CFITSIO_LIB"
  fi
  LDFLAGS="$EXT_LDFLAGS $CFITSIO_LDFLAGS"

  CFITSIO_LIBS="cfitsio"

  for l in $CFITSIO_LIBS; do
    AC_CHECK_LIB([$l], main, [], [myerr_cfitsio="cannot link library: $l"])
    if ! test -z "$myerr_cfitsio"; then break; fi
  done

  if test -z "$myerr_cfitsio"; then
    EXT_CPPFLAGS="$CPPFLAGS"
    EXT_LDFLAGS="$LDFLAGS"
    AC_SUBST(CFITSIO_LDFLAGS)
    AC_MSG_NOTICE([])
    AC_MSG_NOTICE([CFITSIO library configured successfully.])
    AC_MSG_NOTICE([])
  else
    $PERL requirePkg.pl CFITSIO http://heasarc.gsfc.nasa.gov/docs/software/fitsio/fitsio.html 3.340 cfitsio3340.tar.gz --with-cfitsio --disable-cfitsio
    AC_MSG_ERROR([$myerr_cfitsio])
  fi

#JSONLIB - only used in FITS Reader
  AC_CONFIG_SUBDIRS([contrib/libjson])
  JSON_LIBS="json"
fi
AC_DEFINE_UNQUOTED([ENABLE_CFITSIO], $myenable_cfitsio, [ENABLE_CFITSIO])
AM_CONDITIONAL(ENABLE_CFITSIO, test $myenable_cfitsio -eq 1)





#PHP library
myerr_php=""
if test $myenable_php -eq 1; then
  if test -z $mypath_php; then
    AC_PATH_PROG(PHP_CONFIG, [php-config], [])
    AC_PATH_PROG(PHP, [php], [])
  else
    AC_PATH_PROG(PHP_CONFIG, [php-config], [], [$mypath_php/bin:$PATH])
    AC_PATH_PROG(PHP, [php], [], [$mypath_php/bin:$PATH])
  fi

  if test -z "$PHP_CONFIG"; then
    AC_PATH_PROG(PHP_CONFIG, [php-config], [/usr/local/bin:$PATH])
    AC_PATH_PROG(PHP, [php], [/usr/local/bin:$PATH])
  fi


  if test -z "$PHP_CONFIG"; then
     myerr_php="cannot find required program: php-config"
  else
    PHP_CFLAGS=`$PHP_CONFIG --includes | $PERL selectflags.pl`
    CPPFLAGS="$EXT_CPPFLAGS $PHP_CFLAGS"

    PHP_EXTDIR=`$PHP_CONFIG --extension-dir`

    #if test -z "$PHP_EXTDIR"; then
    #  if ! test -z $mypath_php; then
    #    PHP_EXTDIR="$mypath_php/extensions"
    #  fi
    #fi

# include_path
    #PHP_INCPATH=`$PHP -i | grep include_path | awk -F '=>' '{s1=index($2,":"); p=substr($2,s1+1,length($2)-s1-1); s2=index(p,"":""); if (s2 > 0) print substr(p,1,s2-1); else print p;}'`
     PHP_INCPATH=`$PHP -i | grep include_path | $PERL selectphp_path.pl`

    #if test -d $PHP_EXTDIR; then 	
      AC_CHECK_HEADERS([zend.h             ],[], [myerr_php="cannot find file zend.h"])
      AC_CHECK_HEADERS([zend_API.h         ],[], [myerr_php="cannot find file zend_API.h"])
      AC_CHECK_HEADERS([php.h	         ],[], [myerr_php="cannot find file php.h"])
    #else
    #  myerr_php="Cannot determine PHP extension dir!"
    #fi
  fi

  if test -z "$myerr_php"; then
    AC_MSG_NOTICE([])
    AC_MSG_NOTICE([PHP package configured successfully.])
    AC_MSG_NOTICE([])
  else
    $PERL requirePkg.pl PHP http://www.php.net 5.0.5 php-5.0.5.tar.gz --with-php --disable-php
    AC_MSG_ERROR([$myerr_php])
  fi
fi
AC_SUBST(PHP_CFLAGS)
AC_SUBST(PHP_EXTDIR)
AC_SUBST(PHP_INCPATH)
AC_DEFINE_UNQUOTED([ENABLE_PHP], $myenable_php, [ENABLE_PHP])
AM_CONDITIONAL(ENABLE_PHP, test $myenable_php -eq 1)













#PERL library
myerr_perl=""
if test $myenable_perl -eq 1; then
  if test -z $mypath_perl; then
    mypath_perl=`perl -MConfig -e 'print $Config{archlib}'`
  fi

  mypath_perl="$mypath_perl/CORE"

# LN add
  if test -z $myccflags_perl; then
    myccflags_perl=`perl -MConfig -e 'print $Config{ccflags}'`
  fi

  PERL_CFLAGS="-I$mypath_perl $myccflags_perl"
  CPPFLAGS="$EXT_CPPFLAGS $PERL_CFLAGS"

  AC_CHECK_HEADERS([EXTERN.h      ],[], [myerr_perl="cannot find file Extern.h"])
  AC_CHECK_HEADERS([perl.h	  ],[], [myerr_perl="cannot find file perl.h"])
  AC_CHECK_HEADERS([XSUB.h        ],[], [myerr_perl="cannot find file XSUB.h"])

  if test -z "$myerr_perl"; then
    AC_MSG_NOTICE([])
    AC_MSG_NOTICE([PERL package configured successfully.])
    AC_MSG_NOTICE([])
  else
    $PERL requirePkg.pl PERL http://www.perl.org 5.8.8 perl-5.8.8.tar.gz --with-perl --disable-perl
    AC_MSG_ERROR([$myerr_perl])
  fi
fi
AC_SUBST(PERL_CFLAGS)
AC_SUBST(PERL_EXTDIR)
AC_DEFINE_UNQUOTED([ENABLE_PERL], $myenable_perl, [ENABLE_PERL])
AM_CONDITIONAL(ENABLE_PERL, test $myenable_perl -eq 1)









#IDL library
# LN
myerr_idl=""
VALID_IDL=
ARCHFLAGS=

if test $myenable_idl -eq 1; then
#  if test -z $mypath_idl; then
#    mypath_idl="/usr/local/rsi/idl"  #Default search path

	if test -z "$IDL_BINDIR" ; then
#no location was presented as an option -> search the usual suspects...
	  IDL_PATH=/usr/local/harris/idl/bin:/usr/local/exelis/idl/bin:/usr/local/itt/idl/bin:/usr/local/rsi/idl/bin:/usr/local/pkg/graphics/rsi/idl/bin:/Applications/harris/idl/bin:/Applications/exelis/idl/bin:/Applications/exelis/idl/idl/bin:/Applications/rsi/idl/bin:/Applications/itt/idl/bin:/usr/local/idl/bin:/Applications/itt/idl/idl/bin:/usr/local/itt/idl/idl/bin
	  AC_PATH_PROGS([IDL_BIN], [idl],[],[$IDL_PATH])
	  if test -z "$IDL_BIN" ; then
#binaries not found while they were required through the enable-idl option
		AC_MSG_ERROR([IDL binaries not found in $IDL_PATH. Use --with-idl to set the location of the IDL root dir.])
	  else
		#binaries were found...
	#	AC_MSG_RESULT([yes, in $IDL_PATH])
		IDL_BINDIR=`AS_DIRNAME([$IDL_BIN])`
	  fi
	else
	#test if the user-supplied value contains the idl binary
	  AC_PATH_PROGS([IDL_BIN], [idl],[],[$IDL_BINDIR])
	  if test -z "$IDL_BIN" ; then
		AC_MSG_ERROR([IDL binaries not found. Use --with-idl to set the location of the IDL root dir.])
		VALID_IDL=no
	  fi
	fi

	#moving on...
	if test -n "$IDL_BINDIR" && test "x$VALID_IDL" != xno ; then
	IDL_DIR=`AS_DIRNAME([$IDL_BINDIR])`
	#if this test succeeds then using version 5.6 or greater
	IDL_INCDIR="$IDL_DIR/external/include"
	AC_CHECK_FILE(["$IDL_INCDIR/idl_export.h"],[IDL_HASINC=yes],[IDL_HASINC=no])

	#for older versions this would yield the header file
	if test "x$IDL_HASINC" = xno ; then
		IDL_INCDIR="$IDL_DIR/external"
		AC_CHECK_FILE(["$IDL_INCDIR/export.h"],[IDL_HASINC=yes],[IDL_HASINC=no])
	fi
	if test "x$IDL_HASINC" = xno ; then
		AC_MSG_WARN([IDL header file was not found in $IDL_INCDIR. This most likely indicates a problem with the IDL installation.])
		VALID_IDL=no
	fi

	#check for path to libidl.so
	if test "x$VALID_IDL" != xno ; then
  	case "$host" in

	    x86_64-*-linux*)
	      IDL_LIBBIN=$IDL_BINDIR/bin.linux.x86_64
	      #IDL_CFLAGS="-fPIC"
	      IDL_LD_FLAGS="-shared -Xlinker -Bsymbolic -Xlinker --warn-once -Xlinker -noinhibit-exec"
	      ;;

	    *-linux*)
	      IDL_LIBBIN=$IDL_BINDIR/bin.linux.x86
              #IDL_CFLAGS="-fPIC"
                     IDL_LD_FLAGS="-shared -Xlinker -Bsymbolic -Xlinker --warn-once -Xlinker -noinhibit-exec"
	      ;;

	    *-sgi*)
#	      IDL_LIBBIN=$IDL_BINDIR/bin.sgi
	      AC_MSG_WARN([IRIX is currently not supported. Disable the bindings with --disable-idl])
		VALID_IDL=no
	      ;;
	    x86_64-*-darwin*)
		if test -d $IDL_BINDIR/bin.darwin.x86_64 ; then
	      		IDL_LIBBIN=$IDL_BINDIR/bin.darwin.x86_64
	      		#IDL_CFLAGS="-fPIC"
	      		IDL_LD_FLAGS="-Xlinker -flat_namespace -Xlinker -undefined -Xlinker suppress -Xlinker -bundle"
		else
			AC_MSG_WARN([You appear to be running an IDL version on Mac OS X Snow Leopard that does not have the required 64-bit IDL libraries. Please upgrade your IDL installation to at least version 7.1])
		VALID_IDL=no
	        fi
	      ;;



	    i386-*-darwin*)
    		IDL_LIBBIN=$IDL_BINDIR/bin.darwin.i386
		#IDL_CFLAGS="-fPIC"
	      	IDL_LD_FLAGS="-Xlinker -flat_namespace -Xlinker -undefined -Xlinker suppress -Xlinker -bundle"
	      ;;

	    ppc*-darwin*)
	      IDL_LIBBIN=$IDL_BINDIR/bin.darwin.ppc
	      #IDL_CFLAGS="-fPIC"
	      IDL_LD_FLAGS="-Xlinker -flat_namespace -Xlinker -undefined -Xlinker suppress -Xlinker -bundle"
	      ;;
	   *-solaris-*)
	      AC_MSG_WARN([Solaris is currently not supported. Disable the bindings with --disable-idl])
		VALID_IDL=no
	      ;;

	   *)
	      AC_MSG_WARN([Could not detect platform for IDL.])
		VALID_IDL=no
	      ;;

	  esac

	if test "x$VALID_IDL" != xno ; then
#	AC_ARG_WITH([rsiidl-libdir],[AS_HELP_STRING([--with-rsiidl-bindir],[set location of the rsi-idl libraries])],[IDL_LIBBIN=$withval],[IDL_LIBBIN="$IDL_LIBBIN"])
	AC_CHECK_FILE([$IDL_LIBBIN/libidl.so],[IDL_HASBIN_SO=yes],[IDL_HASBIN_SO=no])	
	AC_CHECK_FILE([$IDL_LIBBIN/libidl.dylib],[IDL_HASBIN_DY=yes],[IDL_HASBIN_DY=no])	
	if test "x$IDL_HASBIN_SO" = xno && test "x$IDL_HASBIN_DY" = xno  ; then
		AC_MSG_WARN([libidl.so or libidl.dylib could not be found. This means that you have either a corrupt or custom installation of idl. In the first case, set --with-rsiidl-libdir to match the location of the IDL libraries])
		VALID_IDL=no
	fi
	if test "x$VALID_IDL" != xno ; then
	VALID_IDL=yes
	AC_PROG_MKDIR_P
	AC_PROG_INSTALL
	AC_SUBST(IDL_BINDIR)
	AC_SUBST(IDL_DIR)
	AC_SUBST(IDL_INCDIR)
	AC_SUBST(IDL_LIBBIN)
  IDL_CFLAGS="-I$IDL_INCDIR"
	AC_SUBST(IDL_CFLAGS)
	AC_SUBST(IDL_LD_FLAGS)
	AC_MSG_NOTICE([Building with IDL bindings])
	fi

	else
	  $myerr_idl="error with IDL bindings"
	fi
  fi
  fi
fi #test "x$enable_idl" != xno

AC_SUBST(ARCHFLAGS)

AM_CONDITIONAL([ENABLE_IDL],[test x$VALID_IDL = xyes])





#  fi

#  IDL_CFLAGS="-I$IDL_BINDIR/external/include"
  CPPFLAGS="$EXT_CPPFLAGS -I$IDL_INCDIR $IDL_CFLAGS"

#  AC_CHECK_HEADERS([idl_export.h],[], [myerr_idl="cannot find file idl_export.h"])

  if test -z "$myerr_idl"; then
    AC_MSG_NOTICE([])
    AC_MSG_NOTICE([IDL package configured successfully.])
    AC_MSG_NOTICE([])
  else
    echo "MCS require the IDL development package (http://www.ittvis.com/idl/, version 5.6 or later)."
    echo ""
    echo "Either the package is not installed, or it is located in a non standard path."
    echo ""
    echo "If the package is not installed you should contact ITT to obtain a copy."
    echo ""
    echo ""
    echo "If the package is already installed in a non standard location you can provide"
    echo "the correct path with:"
    echo "  ./configure --with-idl=<PATH>"
    echo ""
    echo "If you don't want the IDL facilities to be active type:"
    echo "  ./configure --disable-idl"
    echo ""
    AC_MSG_ERROR([$myerr_idl])
  fi
#fi
#AC_SUBST(IDL_CFLAGS)
#AC_DEFINE_UNQUOTED([ENABLE_IDL], $myenable_idl, [ENABLE_IDL])
#AM_CONDITIONAL(ENABLE_IDL, test $myenable_idl -eq 1)






#PYTHON library
if test $myenable_python -eq 1; then

  if test -z $mypath_python; then
    AC_PATH_PROG(PYTHON_CONFIG, [python-config], [])
  else
    AC_PATH_PROG(PYTHON_CONFIG, [python-config], [], [$mypath_python/bin:$PATH])
  fi

  if test -z "$PYTHON_CONFIG"; then
    AC_PATH_PROG(PYTHON_CONFIG, [python-config], [/usr/local/bin:$PATH])
  fi

  if test -z "$PYTHON_CONFIG"; then
     mypath_python="/usr/local/python"
     PYTHON_CFLAGS="-I$mypath_python/include"
  else
    PYTHON_CFLAGS=`$PYTHON_CONFIG --includes | $PERL selectflags.pl`
  fi

  CPPFLAGS="$EXT_CPPFLAGS $PYTHON_CFLAGS"
#v0.3.3-alpha7
#AC_CHECK_HEADERS([python/Python.h],[], [myerr_python="cannot find file Python.h"])
  AC_CHECK_HEADERS([Python.h],[], [myerr_python="cannot find file Python.h"])

  if test -z "$myerr_python"; then
    AC_MSG_NOTICE([])
    AC_MSG_NOTICE([PYTHON package configured successfully.])
    AC_MSG_NOTICE([])
  else
    $PERL requirePkg.pl PYTHON http://www.python.org 2.3 python-2.3.tar.gz --with-python --disable-python
    AC_MSG_ERROR([$myerr_python])
  fi
fi
AC_SUBST(PYTHON_CFLAGS)
AC_DEFINE_UNQUOTED([ENABLE_PYTHON], $myenable_python, [ENABLE_PYTHON])
AM_CONDITIONAL(ENABLE_PYTHON, test $myenable_python -eq 1)




#Restore original CPPFLAGS
CPPFLAGS="$EXT_CPPFLAGS"




if test $myenable_debug -eq 1; then
  #Remove debug and optimization flags from CPPFLAGS, their place is in the CXXFLAGS
  CPPFLAGS=`echo "$CPPFLAGS" | $PERL -pe 's/(\A|\W)-g(\Z|\W)/ /g; s/(\A|\W)-O\d??(\Z|\W)/ /g';`

  CXXFLAGS=`echo "$CXXFLAGS" | $PERL -pe 's/(\A|\W)-g(\Z|\W)/ /g; s/(\A|\W)-O\d??(\Z|\W)/ /g';`
  CXXFLAGS="$CXXFLAGS -g -O0"
  CFLAGS=`echo "$CFLAGS" | $PERL -pe 's/(\A|\W)-g(\Z|\W)/ /g; s/(\A|\W)-O\d??(\Z|\W)/ /g';`
  CFLAGS="$CFLAGS -g -O0"
fi
AC_DEFINE_UNQUOTED([ENABLE_DEBUG], $myenable_debug, [ENABLE_DEBUG])


MCS_VERSION="$PACKAGE_VERSION"
mcs_cflags="$CPPFLAGS $CXXFLAGS"
mcs_libs="-lmcsjson -lmcs"
#MCS_LIBDEPS="-L/$prefix/lib -L${prefix}/lib"

AC_DEFINE_DIR([DATADIR], [datadir/mcs], [Share directory for MCS])
AC_DEFINE_DIR([BINDIR], [bindir], [Binary installation directory])
AC_CONFIG_LIBCONFIG_IN([mcs],[share/scripts])

AC_SUBST(MCS_CONFIG)
AC_SUBST(INTERFACE_VERSION)

#   extra/Makefile
AC_CONFIG_FILES([
   Makefile
   version
   contrib/Makefile
   contrib/libjson/Makefile
   src/Makefile
   src/doc.h
   doc/Makefile
   doc/Doxyfile
   share/Makefile
   share/client/Makefile
   share/client/idl/Makefile
   share/client/idl/c2idl.pl
   share/client/php/Makefile
   share/client/python/Makefile
   share/client/perl/Makefile
   share/scripts/Makefile
   share/scripts/mcs-config
   share/scripts/idl2mcs
   share/examples/Makefile
])
AC_OUTPUT




cat << myEOF


MCS (My Customizable Server), ver. $PACKAGE_VERSION, intrf. ver. $INTERFACE_VERSION
-----------------------------------------------------------
Reference site: http://ross.iasfbo.inaf.it/mcs
Bug report: $PACKAGE_BUGREPORT


Configuration summary:
----------------------
Installation prefix: $prefix
Docs directory: `/bin/sh share/scripts/mcs-config --datadir`/doc
Debug: $myenable_debug


CPPFLAGS: $CPPFLAGS

CXXFLAGS: $CXXFLAGS

LDFLAGS: $LDFLAGS

LIBS: $LIBS


External programs and libraries:
--------------------------------
pcre-config program:  $PCRE_CONFIG

curl facilities:      $myenable_curl
curl-config program:  $CURL_CONFIG

MySQL facilities:     $myenable_mysql
mysql_config program: $MYSQL_CONFIG

OpenSSL support:      $myenable_openssl

CFITSIO facilities:   $myenable_cfitsio
CFITSIO path:         $mypath_cfitsio
		
IDL interface:        $myenable_idl
IDL path:             $IDL_BINDIR
		
Python interface:     $myenable_python
Python path:          $mypath_python
		
PHP interface:        $myenable_php
php-config program:   $PHP_CONFIG

Perl interface:       $myenable_perl
Perl path:            $mypath_perl


The package has been configured correctly (see summary above).
Type 'make' to compile, and 'make install' to install.
myEOF

