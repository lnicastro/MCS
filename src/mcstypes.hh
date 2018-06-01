// ----------------------------------------------------------------------^
// Copyright (C) 2004, 2005, 2006, 2007, 2008 Giorgio Calderone
// (mailto: <gcalderone@ifc.inaf.it>)
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


/* -------- FROM MYSQL DOCUMENTATION ---------
   buffer_type Value    SQL Type                C Type
MYSQL_TYPE_TINY		TINYINT		        char
MYSQL_TYPE_SHORT	SMALLINT		short int
MYSQL_TYPE_INT24        MEDIUMINT               int
MYSQL_TYPE_LONG		INT		        int
MYSQL_TYPE_LONGLONG	BIGINT		        long long int
MYSQL_TYPE_FLOAT	FLOAT		        float
MYSQL_TYPE_DOUBLE	DOUBLE		        double
MYSQL_TYPE_TIME		TIME		        MYSQL_TIME --> struct tm
MYSQL_TYPE_DATE		DATE		        MYSQL_TIME --> struct tm
MYSQL_TYPE_DATETIME	DATETIME		MYSQL_TIME --> struct tm
MYSQL_TYPE_TIMESTAMP	TIMESTAMP		MYSQL_TIME --> struct tm
MYSQL_TYPE_STRING	CHAR		
MYSQL_TYPE_VAR_STRING	VARCHAR		
MYSQL_TYPE_TINY_BLOB	TINYBLOB/TINYTEXT		
MYSQL_TYPE_BLOB		BLOB/TEXT		memory buffer
MYSQL_TYPE_MEDIUM_BLOB	MEDIUMBLOB/MEDIUMTEXT		
MYSQL_TYPE_LONG_BLOB	LONGBLOB/LONGTEXT
*/


#ifndef DEF_MCSTYPES_HH
#define DEF_MCSTYPES_HH


/*!
  \brief Enumeration of base type for Data.
 */
enum Types {
  TINY         ,          /*!<1 byte integer                         */
  SMALL        ,	  /*!<2 bytes integer                        */
  MEDIUM       ,	  /*!<3 bytes integer                        */
  INT          ,	  /*!<4 bytes integer                        */
  BIGINT       ,	  /*!<8 bytes integer                        */
  FLOAT        ,	  /*!<4 bytes floating point                 */
  DOUBLE       ,	  /*!<8 bytes floating point                 */
  STRING       ,	  /*!<Variable length string                 */
  TIME         ,	  /*!<Date and time information, with second resolution */
  TINY_BLOB    ,	  /*!<Binary data with length < 256          */
  BLOB         , 	  /*!<Binary data with length < 65536        */
  POINTER
};

typedef enum Types Types;

#endif
