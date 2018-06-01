// ----------------------------------------------------------------------^
// Copyright (C) 2004 -- 2011, Giorgio Calderone
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



/*!
  \file mcs.hh

  \brief Main include file for all MCS based applications.
*/


#ifndef DEF_MCS_HH
#define DEF_MCS_HH


//The symbol COMPILING_MCS is defined when we are compiling the MCS library
#undef COMPILING_MCS
#ifdef HAVE_CONFIG_H
  //We are compiling the MCS library
#define COMPILING_MCS
#else
  //We are compiling a MCS-based program
#endif //HAVE_CONFIG_H



//We need the following includes only when compiling the MCS library
#ifdef COMPILING_MCS
//--------------------------------------------------------------------
//System's library include

//Portability with old libc
// LN
#ifndef __APPLE__
#include <wait.h>
#endif

#include <sys/stat.h>
#include <math.h>
#include <stdint.h>
#include <stdarg.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>

//Portability with old libc
#include <unistd.h>

#include <fstream>
#endif  //COMPILING_MCS


//The following include is necessary for some network related type
#include <netinet/in.h>


//The following includes are probably needed by user programs, so we
//include them here instead of in the "COMPILING_MCS" section.
#include <string>
#include <iostream>
#include <vector>
using namespace std;

//#include <stdlib.h>
#include <cstdlib>

//LN
#include <cstdio>
#include <cstring>




//--------------------------------------------------------------------
//MCS configuration file.
#include "mcs_config.h"







/*Pierre's stuff*/

#define TBOLD "\x1B[1m"
#define REV  "\x1B[7m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"
#define CYAN  "\x1B[36m"
#define YELLOW  "\x1B[33m"
#define MAGENTA "\x1B[35m"
#define BEEP  "\x7"
#define BGBLUE  "\x1B[44m"
#define NORMAL "\x1B[0m"
#define CLEANS  "\x1B[2J"


#define MERROR std::cerr << RED <<   "\r(E)\t"<< GREEN << __FILE__ << "," << __LINE__ << NORMAL << "\t" <<TBOLD<<__FUNCTION__<<NORMAL << "\t" 
#define MINFO std::cout << BLUE <<   "\r(I)\t"<< GREEN << __FILE__ << "," << __LINE__ << NORMAL << "\t" <<TBOLD<<__FUNCTION__<<NORMAL << "\t" 
#define MWARN std::cout << YELLOW << "\r(W)\t"<< GREEN << __FILE__ << "," << __LINE__ << NORMAL << "\t" <<TBOLD<<__FUNCTION__<<NORMAL << "\t" 


#if ENABLE_MYSQL
//--------------------------------------------------------------------
//MySQL include, this will be needed by user programs until the
//"mysql_stmt_param_metadata" function is available.
#include <mysql.h>
#define MYSQL_SOCK NULL

#else
//If MySQL facilities are disabled we need the following declaration to
//compile correctly.
#define MYSQL_BIND char
#define my_bool bool

//This has been copied from MySQL's include file mysql_time.h
typedef struct st_mysql_time
{
    unsigned int  year, month, day, hour, minute, second;
} MYSQL_TIME;
#endif  //ENABLE_MYSQL



//OpenSSL include
#ifdef HAVE_OPENSSL_SSL_H
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif //HAVE_OPENSSL_SSL_H



#ifdef COMPILING_MCS

//--------------------------------------------------------------------
//PCRE include
#include <pcrecpp.h>
#endif  //COMPILING_MCS


/*!
   \namespace mcs

   \brief Namespace for MCS library.

   The namespace "mcs" contains all classes, functions and type declarations
   provided by the mcs library. Those names that cannot be protected inside
   namespaces, like symbols and macros, has the "MCS_" prefix.
*/
namespace mcs
{



//--------------------------------------------------------------------
//MCS constants

//!Default port on which the server is listening.
#define MCS_DEFAULT_PORT 6523


/*!
  \brief File chunk size.

  When a file is going to be sent or received it will be splitted in chunks of
  this size.
*/
#define MCS_DEFAULTCHUNKSIZE      16384


//!Default max number of clients connected.
#define MCS_DEFAULT_MAX_USERS 100


//!Default client connection timeout, in milliseconds: 10 minutes.
#define MCS_DEFAULT_CLIENT_TIMEOUT 10 * 60 * 1000
//                                 min  sec  msec

//!Max length of a message being sent between client and server.
#define MCS_COMMBUFSIZE    1024



#define MCS_SSLCERT     "mcscert.pem"
#define MCS_SSLPRIV     "mcspkey.pem"

//Grants constants
#define MCS_GRANT_NO_GRANTS        0
#define MCS_GRANT_LOGIN            1
#define MCS_GRANT_SQL_SCRIPTS      2
#define MCS_GRANT_SCRIPTS          4
#define MCS_GRANT_QUERY            8
#define MCS_GRANT_BATCH           16
#define MCS_GRANT_GET             32
#define MCS_GRANT_PUT             64
#define MCS_GRANT_SYS            128
#define MCS_GRANT_ADMIN          256
#define MCS_GRANT_ALL            511


//!MCS banner
#define MCS_BANNER "\nMy Customizable Server (MCS) ver. " PACKAGE_VERSION "\n"


#define MCS_MISSING_COPY_CONSTRUCTOR(CLASS) \
CLASS(const CLASS&);


#define MCS_MISSING_ASSIGNMENT_OPERATOR(CLASS)  \
CLASS& operator=(const CLASS&);


/*!
  \brief Values to be used in Buffer class constructor.
 */
enum BufferFreeOnDestroy
{
  AUTO_FREE, /*!<Automatically free allocated buffer. */
  DONT_FREE  /*!<Don't free allocated buffer. */
};



/*!
  \brief Values to be used with throwexc parameters.
*/
enum ThrowExceptions
{
  DONT_THROW,  /*!<Don't throw exception. */
  THROW        /*!<Throw an exception. */
};




//--------------------------------------------------------------------
//Protocol Specification:

//!Prefix of a message for the client, this is the first character.
#define MCS_PRE                         "#"

//!This character is used to split a message into "fields".
#define MCS_SEP                         "|"

//!Prompt sent after a command has been executed correctly.
#define MCS_PROMPT_OK           MCS_PRE "0--"

//!Prompt sent after a command raised one or more warnings.
#define MCS_PROMPT_WARN         MCS_PRE "0W-"

//!Prompt sent after a command raised one error.
#define MCS_PROMPT_ERROR        MCS_PRE "0E-"


//Protocol messages and numeric identifiers
#include <mcsmsg.hh>

//Protocol commands: (help lines MUST ends on column 93)

//!Command: No operation (dummy) command.
#define MCS_CMD_NOP                     "NOP"
#define MCS_CMD_NOP_HELP                "No operation (dummy) command."


//!Command: Return server status information.
#define MCS_CMD_CLIENT_INFO             "CLINFO"
#define MCS_CMD_CLIENT_INFO_HELP        "Return all client informations."


//!Command: Provide user name.
#define MCS_CMD_USERNAME                "USR"
#define MCS_CMD_USERNAME_HELP           "Supply user name.\n" \
                                        "USR <user_name>"

//!Command: Provide password.
#define MCS_CMD_PASSWORD                "PWD"
#define MCS_CMD_PASSWORD_HELP           "Supply password.\n"  \
                                        "PWD <password>"

//!Command: Provide the application (database) name.
#define MCS_CMD_DBNAME                  "DBN"
#define MCS_CMD_DBNAME_HELP             "Supply application (database) name.\n" \
                                        "DBN <application_name>"


//!Command: Finalize the authentication process and log in.
#define MCS_CMD_DBCONNECT               "CON"
#define MCS_CMD_DBCONNECT_HELP          "Finalize the authentication process and log in.\n" \
                                        "CON"

//!Command: Close the session.
#define MCS_CMD_CLOSECLIENT             "BYE"
#define MCS_CMD_CLOSECLIENT_HELP        "Close the session.\n" \
                                        "BYE"

//!Command: Retrieve the Client identifier.
#define MCS_CMD_CID                     "CID"
#define MCS_CMD_CID_HELP                "Retrieve the Client identifier.\n" \
                                        "CID"



/*!
  Command: Retrieve the record at a specified position, or at current
  position (if none is specified) of the last query executed.
*/
#define MCS_CMD_RECORD                  "FETCH"
#define MCS_CMD_RECORD_HELP             "Retrieve the record at a specified position, or at\n" \
                                        "current position (if none is specified) of the last\n" \
                                        "query executed.\n" \
                                        "FETCH [position]"


#if ENABLE_MYSQL
//Commands for DB access

//!Command: Execute queries on the database.
#define MCS_CMD_QUERY                   "QRY"
#define MCS_CMD_QUERY_HELP              "Execute queries on the database.\n" \
                                        "QRY <SQL query>"

//!Command: Retrieve a file with an ASCII dump of the entire set of records returned by the last query.
#define MCS_CMD_SENDQUERYRES            "QRES"
#define MCS_CMD_SENDQUERYRES_HELP       "Retrieve a file with an ASCII dump of the entire set\n" \
                                        "of records returned by the last query." \
                                        "QRES"


//!Command: Retrieve the list of tables actually present in the database.
#define MCS_CMD_TABLELIST               "TLIST"
#define MCS_CMD_TABLELIST_HELP          "Retrieve the list of tables actually present in the\n" \
                                        "database.\n" \
                                        "TLIST"

//!Command: Retrieve information about a table.
#define MCS_CMD_TABLEINFO               "TINFO"
#define MCS_CMD_TABLEINFO_HELP          "Retrieve information about a table.\n" \
                                        "TINFO"
#endif  //ENABLE_MYSQL




//Options (to be used with "-" like in a shell command)


//!Option: Save query result in FITS format.
#define MCS_OPT_SAVEQUERYFITS           "sqfits"
#define MCS_OPT_SAVEQUERYFITS_HELP      "Save query result in FITS format.\n " \
                                        "-sqfits"

//!Option: Save query result in ASCII format.
#define MCS_OPT_SAVEQUERYASCII          "sqascii"
#define MCS_OPT_SAVEQUERYASCII_HELP     "Save query result in ASCII format.\n"  \
                                        "-sqascii"


//!Option: Continue execution of commands even if an error occur.
#define MCS_OPT_FORCE                   "force"
#define MCS_OPT_FORCE_HELP              "Continue execution of commands even if an error\n" \
                                        "occurred.\n" \
                                        "-force"

//!Option: Turns all warning into errors, so that a warning can stop the execution.
#define MCS_OPT_ALL_ERRORS              "werr"
#define MCS_OPT_ALL_ERRORS_HELP         "Turns all warning into errors, so that a warning can\n" \
					"stop the execution.\n" \
                                        "-werr"

//!Option: Put a copy of thr recevied Data object in the "send" vector.
#define MCS_OPT_LOOP                    "loop"
#define MCS_OPT_LOOP_HELP               "Put a copy of thr received Data object in the \n" \
                                        "\"send\" vector.\n" \
                                        "-loop"

//!Option: Return a quick help for a command or an option.
#define MCS_OPT_HELP                    "help"


//!Command: Execute an external program, script, SQL or batch file.
#define MCS_CMD_EXEC                    "EXEC"
#define MCS_CMD_EXEC_HELP               "Execute an external program, SQL or batch file.\n" \
                                        "EXEC <alias> [[PAR1] [PAR2] ...]"

//!Command: Download a file from the work directory.
#define MCS_CMD_GET                     "GET"
#define MCS_CMD_GET_HELP                "Download a file from the work directory.\n" \
                                        "GET <filename>"

//!Command: Upload a file to the work directory
#define MCS_CMD_PUT                     "PUT"
#define MCS_CMD_PUT_HELP                "Upload a file to the work directory.\n" \
                                        "PUT <filename> <size>"

//!Command: Download Data objects.
#define MCS_CMD_GETDATA                 "GDATA"
#define MCS_CMD_GETDATA_HELP            "Download Data objects.\n" \
                                        "GDATA <position>"

//!Command: Upload Data objects.
#define MCS_CMD_PUTDATA                 "PDATA"
#define MCS_CMD_PUTDATA_HELP            "Upload Data objects.\n" \
                                        "PDATA <size>"





//--------------------------------------------------------------------
//MCS enumerations

/*!
  \brief Return value for MCS routines.

  This enumeration is often used as a return value for a function or
  method. It says if the function executed correctly (value OK) or
  not (values WARN, ERROR, FATAL). This enumeration is also used to
  specify an event type (see the Event class), which are also used to
  throw exceptions.

  The three values for errors should be used following this rule:

  - WARN a warning, should be used when the error is just an
  unexpected condition, and the function can do its task as if the
  error wasn't caught. If such an error occurr during the execution of
  a batch list of commands, the execution will continue until the end.

  - ERROR an application error, should be used when the function
  cannot do its task, but the error is due to a wrong call made by the
  user, or a wrong use of the software. In this case a message should
  be displayed at the user and the execution of the task should be
  terminated. This is what happened if the server is executing a list
  of batch commands.

  - FATAL a system error, should be used hen the function caught an
  error after a call to a system routine, or if something very wrong
  happened such a file that was expected to be available and it
  isn't. In this case the thread who caught the error should
  terminate.
*/
enum RetValue {
  OK,     /*!<No error.          */
  WARN,   /*!<Warning.	         */
  ERROR,  /*!<Application error. */
  FATAL   /*!<Fatal error.       */
};


#include "mcstypes.hh"


//--------------------------------------------------------------------
//MCS macro facilities

//!Macro to extract a C-style, null terminated string from a string object
#define csz            c_str()


//!Some systems doesn't have mempcpy, so we provide it here.
#define my_mempcpy(D, S, N) ((void *) ((char *) memcpy (D, S, N) + (N)))




//--------------------------------------------------------------------
//MCS utilities functions

//!To be used with subst(), doubles each backslash in "with". See subst().
#define MCS_SUBST_QUOTE_WITH 1

//!To be used with subst(), substitute only if "what" is at the beginning. See subst().
#define MCS_SUBST_LEADING    4

//!To be used with subst(), substitute only if "what" is at the end. See subst().
#define MCS_SUBST_TRAILING   8

/*!
  \brief Perform substitutions on a string.

  Substitutes "what" with "with" in the string "s" and return the
  result. The "what" parameter is intended as a regular expression.
  The parameter "op" can be one or more of MCS_SUBST_QUOTE_WITH,
  MCS_SUBST_LEADING, MCS_SUBST_TRAILING.
*/
string subst(string s, string what, string with, int op = 0);


//!Remove any leading or trailing blanks.
string trim(string s);

//!Remove any trailing newlines.
string chomp(string s);

//!Remove any tab.
string remTabs(string s);

//!Remove any leading character "p".
string remLeading(string& s, const char* p);

//!Remove any trailing character "p".
string remTrailing(string& s, const char* p);

/*!
  \brief Split a string into tokens.

  Return a vector of tokens starting from the string "s" and using
  "sep" as delimiter. Delimiters won't be included in any token. The
  "sep" parameter is intended as a regular expression.
 */
vector<string> split(string s, string sep = " ");

//!Extract the numeric code from a server reply.
int extractCode(const char* msg);

/*!
  \brief Check if a file or directory exists.

  \param fn Path to the filename or directory;
  \param size Upon return will contain the size (if "fn" referred to a file);

  \return True if the file or directory exists, false otherwise.
*/
bool File_Dir_Exist(string fn, unsigned int& size);

//!Convert an integer to a string.
string itos(int i);


/*!
  \brief Convert a string to an integer.

  \param s The string containing the number;

  If the conversion cannot be performed an exception will be thrown.

  \result The result of conversion.
 */
int stoi(string s);

/*!
  \brief Convert a string to an integer.

  \param s The string containing the number;

  \param errval If the conversion cannot be performed this parameter will be
  used as return value.

  \result The result of conversion or "errval".
 */
int stoi(string s, int errval);


//!Convert a boolean to a string containing "true" or "false".
string btos(bool b);

//!Join a vector of strings in a single string using newlines.
string vtos(vector<string> vec);

//!Convert an floating point number to a string.
string dtos(double f);

//!Return the current working dir.
string Pwd();

//!Return a string with an hex dump of the buffer pointed by "buf", with a length of "size".
string hexDump(const void* buf, unsigned int size);


#define MCS_MKDIR_UMASK 0
#define MCS_MKDIR_PROTECT S_IRUSR | S_IWUSR | S_IXUSR

//!Create a directory.
bool mkDir(string path, mode_t perm = 0, enum ThrowExceptions throwexc = THROW);


//!Read the file creation mask of the current process.
mode_t read_umask();

//!Removes a directory.
bool rmDir(string path, enum ThrowExceptions throwexc = THROW);

class Record;

//!Fill a vector with a list of file.
void ls2Record(string fn, Record& v);

//!Return the name of the type given in "type" and "isunsigned".
string Types2Str(Types type, bool isunsigned);

//!Tell if "type" is a variable length type.
bool VarLenType(Types type);

//!Tell if "type" is an integer type.
bool IntType(Types type);

//!Tell if "type" is a float type.
bool FloatType(Types type);

/*!
  \brief Copy a file from OLDNAME to NEWNAME.

  OLDNAME must be an absolute or relative path with a filename.

  NEWNAME can be an absolute or relative path with or without a
  filename (the file will be copied with its original name).

  Return 0 if successfull, otherwise return -1. In case of error a
  message will be printed in stderr.
*/
int copy(char* OLDNAME, char* NEWNAME);

/*!
  \brief Move a file from OLDNAME to NEWNAME.

  OLDNAME must be an absolute or relative path with a filename.

  NEWNAME can be an absolute or relative path with or without a
  filename (the file will be copied with its original name). If a
  relative path wich uses only the "." or ".."  paths and without a
  filename is used then it MUST terminates with a slash "/".

  Return 0 if successfull, otherwise return -1. In case of error a
  message will be printed in stderr.
*/
int move(char* OLDNAME, char* NEWNAME);


#if ENABLE_MYSQL
//!Return the name of the MySQL type given in "type".
string MYSQL2Str(enum_field_types type);


/*!
  \brief Convert a MySQL type into a MCS type.

  Convert the MySQL type "mtype" into a MCS type and put the result
  in the "type" parameter (which is a reference). If the conversion
  is possible will return true, false otherwise.
 */
bool MYSQL2Types(enum_field_types mtype, Types& type);

/*!
  \brief Convert a MCS type into a MySQL type.

  Convert the MCS type into a MySQL type and put the result in the
  "mtype" parameter (which is a reference). If the conversion is
  possible will return true, false otherwise.
*/
bool Types2MYSQL(Types& type, enum_field_types& mtype);

/*!
  \brief Convert a MCS type into a MySQL type.

   Return the name of the full MySQL type given the MCS type (reference)
   and the "isunsigned" parameters.
  If the conversion is not possible an empty string will be returned.
*/
string Types2MYSQLStr(Types& type, bool isunsigned);

#endif //ENABLE_MYSQL


#if ENABLE_CFITSIO
/*!
  \brief Convert a FITSIO type into a MCS type.

  Convert the FITSIO type "fits" into a MCS type and put the result in
  the "dbt" and "isunsigned" parameters (which are references). If
  the conversion is possible will return true, false otherwise.
*/
bool FITS2Types(int fits, Types& dbt, bool& isunsigned);

/*!
  \brief Convert a MCS type into a FITSIO type.

  Convert the MCS type into a FITSIO type and put the result in the
  "fits" parameter (which is a reference). If the conversion is
  possible will return true, false otherwise.
*/
bool Types2FITS(Types dbt, bool isunsigned, int& fits);

/*!
  \brief Convert a MCS type into a FITSIO type code.

  Convert the MCS type into a FITSIO type code and put the result in
  the "fits" parameter (which is a reference). If the conversion is
  possible will return true, false otherwise.
*/
bool Types2S_FITS(Types dbt, int len, bool isunsigned, string& fits);


//!Return a description of a FITS error.
string fitsError(int status);
#endif //ENABLE_CFITSIO



//--------------------------------------------------------------------
//!Size of the Event.vmsg static array.
#define MCS_VMSG_SIZE 1000

/*!
  \brief Hold informations about an event.

  An event occur in two cases:

  - when we want to log informations about something happened, for
  example a new user connected, a request made by users, etc...;

  - an error occur and an exception must be thrown.

  An Event object contains several informations about the event
  itself:

  - the source file and line at which the event occurred;

  - an event code, that is a numeric code which identify the event;

  - an event subcode, which is usually the return value of an external
  library function;

  - an event type, which says if the event is just a message or an
  error (see the RetValue enumeration);

  - a descriptive message.

  To create an Event object you should provide all these informations
  (source file name, source file line, event type and event code)
  using one of the constructors. You cannot specify the descriptive
  message directly in the constructor, instead there must be an entry
  in the Event.vmsg static array, whose index is given by the event
  code, which contain the message and eventually some "printf" format
  specifier to add information to the message. These extra information
  can be passed in the constructors, this is the reason why all
  constructors have the same first four parameters while the other
  changes, so that the Event class can be easily used in all
  situations.

  The Event.vmsg array is static, and contains all MCS predefined
  messages. It must be initialized calling the init() method (actually
  called by the constructor). The event codes for the MCS messages are
  defined as symbols with the "MSG_" prefix.

  To use this class with new user-defined messages you should follow
  this rule:

  - define new numeric symbols for any of your messages, starting from
    (MCS_LAST_MESSAGE_CODE + 1) for messages, from
    (MCS_LAST_ERROR_CODE + 1) for errors;

  - after you istantiated the MCS server, set the correct values in
    the Event.vmsg array so that it points to your message
    descriptions;

  An example follows:
  \code
  #define MY_MESSAGE MCS_LAST_MESSAGE_CODE + 1
  #define MY_ERROR   MCS_LAST_ERROR_CODE + 1

  .....

  void initialize_messages(char* vmsg[MCS_VMSG_SIZE]) {
    vmsg[MY_MESSAGE] = "My message: %s";
    vmsg[MY_ERROR  ] = "My error message: %s";
  }

  .....


  mcs::Event::custom_init_vmsg = &initialize_messages;
  \endcode

  Note that for any event there must be a numeric code identifier and
  a message in the corresponding position in the Event.vmsg array.

  The Event class has all private fields, so once you created the
  object you can't modify its content.

  Note that in the constructors parameter list there are always the
  arguments (atFile, atLine, type), in this order. You can avoid
  specifying any of these values using one of the MCS_OK, MCS_WARN,
  MCS_ERROR, MCS_FATAL or MCS_ macros.
*/
class Event
{
private:
  //!Source file.
  string latFile;

  //!Line number.
  int latLine;

  //!Temporary buffer.
  char buf[MCS_COMMBUFSIZE];

  //!Message.
  string lmsg;

  //!Event code;
  int lcode;

  //!Event subcode, usually an external library message code;
  int lsubcode;

  //!Event type.
  RetValue ltype;

  friend class UserThread;

  //!Static array of descriptive messages.
  static const char* vmsg[MCS_VMSG_SIZE];

  //!Initialize the vmsg array.
  static void init_vmsg();

  //!Flag to tell if the init() method has been already called.
  static bool flInitialized;

  void init(string atFile, unsigned int atLine, RetValue type, int code);

public:
  //!Pointer to a callback function to initialize custom messages in the vmsg static array.
  static void (*custom_init_vmsg) (const char* vmsg[MCS_VMSG_SIZE]);

  /*!
    \brief Constructor with three optional strings.

    This constructor set the subcode to zero.

    \param atFile Source file at which the event occurred;
    \param atLine Source file line at which the event occurred;
    \param type Event type;
    \param code Event code;
    \param s1 Optional first string to write in the message;
    \param s2 Optional second string to write in the message;
    \param s3 Optional third string to write in the message;
   */
  Event(string atFile, unsigned int atLine, RetValue type, int code,
	string s1 = "", string s2 = "", string s3 = "");

  /*!
    \brief Constructor with a string and an integer.

    This constructor set the subcode equal to the second parameter.

    \see Event(string, unsigned int, RetValue, int, string, string, string).
  */
  Event(string atFile, unsigned int atLine, RetValue type, int code,
	string s1, int i1);

  /*!
    \brief Constructor with an integer and a string.

    This constructor set the subcode to zero.

    \see Event(string, unsigned int, RetValue, int, string, string, string).
  */
  Event(string atFile, unsigned int atLine, RetValue type, int code,
	int i1, string s1 = "");


  /*!
    \brief Constructor with two integers.

    This constructor set the subcode equal to the second parameter.

    \see Event(string, unsigned int, RetValue, int, string, string, string).
  */
  Event(string atFile, unsigned int atLine, RetValue type, int code,
	int i1, int i2);

  //!Destructor.
  ~Event();

  //!Returns the message.
  string msg();

  //!Returns the place where the error occurred formatted as FILE:LINE (CODE).
  string where();

  //!Returns the event code.
  int code();

  //!Return event subcode.
  int subcode();

  /*!
    \brief Set a subcode and return a reference to the object itself.

    In the Event class constructor there's no way to distinguish a
    parameter from the variable list from an optional subcode, so we
    couldn't insert the "subcode" parameter in the constructor. But
    this way throwing an exception with a subcode require the
    following code:
    \code
    Event e(__FILE__, __LINE__, ERROR, MSG_***, a, b, c,);
    e.subcode(sub);
    throw e;
    \endcode

    This method solve the problem returning a reference to the object
    itself:
    \code
    throw MCS_ERROR(MSG_***, a, b, c).subcode(sub);
    \endcode
  */
  Event& subcode(int subcode);

  //!Return the code and the message, formatted as MCS_PRE CODE
  //!MESSAGE.
  string codemsg();

  //!Source file name where the event occurred.
  string file();

  //!Source line number where the event occurred.
  unsigned int line();

  //!Return the event type.
  RetValue type();

  //!Return the msg in the i-th position of the vmsg array.
  static string static_vmsg(unsigned int i);
};


//!Facility to easily pass all necessary parameter to an Event constructor.
#define MCS_OK(A, rest...)     mcs::Event(__FILE__, __LINE__, mcs::OK,    A, ## rest)

//!Facility to easily pass all necessary parameter to an Event constructor.
#define MCS_WARN(A, rest...)   mcs::Event(__FILE__, __LINE__, mcs::WARN,  A, ## rest)

//!Facility to easily pass all necessary parameter to an Event constructor.
#define MCS_ERROR(A, rest...)  mcs::Event(__FILE__, __LINE__, mcs::ERROR, A, ## rest)

//!Facility to easily pass all necessary parameter to an Event constructor.
#define MCS_FATAL(A, rest...)  mcs::Event(__FILE__, __LINE__, mcs::FATAL, A, ## rest)

//!Facility to easily pass all necessary parameter to an Event constructor.
#define MCS_(TYPE, rest...)    mcs::Event(__FILE__, __LINE__, TYPE    , ## rest)


#ifndef DOXYGEN_SKIP
//--------------------------------------------------------------------
//MCS macro facilities for debug, see also the Debug class
#ifndef NOARGS
#define NOARGS
#endif  //NOARGS

#define MCS_DEBUG_ALLOC
#define MCS_DEBUG_SETUP(A, B)
#define MCS_DEBUG(A)
#define MCS_DEBUG_ENTER(A)
#define MCS_DEBUG_LEAVE(A)

#if ENABLE_DEBUG
#undef  MCS_DEBUG_ALLOC
#undef  MCS_DEBUG_SETUP
#undef  MCS_DEBUG
#undef  MCS_DEBUG_ENTER
#undef  MCS_DEBUG_LEAVE

#define MCS_DEBUG_ALLOC Debug _debug_
#define MCS_DEBUG_SETUP(A, B) _debug_.setDebug(B, this, A)
#define MCS_DEBUG(A)  if (_debug_.getDebugFlag()) { cout << _debug_.debug(__FILE__, __LINE__, __FUNCTION__) << A << endl; }
#define MCS_DEBUG_ENTER(A) MCS_DEBUG("-> "  A);
#define MCS_DEBUG_LEAVE(A) MCS_DEBUG("<- "  A);

#define MCS_DEBUG_NTHR 100

class Debug
{
private:
  bool active;
  string className;
  void* pThis;

  static pthread_t thrs[MCS_DEBUG_NTHR];
  static bool thrmap[MCS_DEBUG_NTHR];
  static int dummy;
  static int init();

public:
  Debug();
  ~Debug();

  string debug(const char* file, int line, const char* function);

  void setDebug(string className, void* pthis = 0, bool active = false);
  void setDebug();
  bool getDebugFlag();
};
#endif  //ENABLE_DEBUG
#endif  //DOXYGEN_SKIP






/*!
  \brief High level buffer.

  This class provide a buffer with several facilities:

  - dynamical allocation and resizing;
  - Read/write using ">>" and "<<" operators.
  - Availability of the "[ ]" operator to use the object as a simple
    array of pointers to char.
  - Append to the end of the buffer.


  This class can be used in two different way, each corresponding to a
  different constructor:

  - with a internal dynamically allocated buffer;
  - with a buffer provided by user;

  In the first case the allocation is performed automatically by the
  object, in the second case user must provide the pointer to the
  buffer and its size.


  An example of its usage follows:

  \code
  Buffer buffer(true); //The buffer will be freed when the object
                       //is destroyed.

  //Insert some data into the buffer.
  buffer(5) << "hello"; //Note that we passed in circle brackets the
                        //length of the data being inserted.

  //Append data.
  buffer(20) << ", my name is Giorgio";

  //Modify the first 5 bytes.
  buffer(0, 5) << "HELLO";

  //Note that unitl now we didn't wrote a terminating NULL character
  //in the buffer. This is needed only if we want to use the Buffer
  //object as a simple array of chars. This can be done in the
  //following way (note that we passed 2 as data length, one for the
  //character and one for the NULL):
  buffer(2) << "!";

  //Now simply print out the buffer content starting from the first
  //character.
  cout << buffer[0] << endl;

  //Print starting from the 8-th character.
  cout << buffer[7] << endl;

  //Copy from the 19-th character to the 25-th into another buffer.
  char tmp[100];
  buffer(18, 7) >> tmp;
  cout << tmp << endl;

  //Copy the entire content.
  buffer(buffer.size()) >> tmp;
  cout << tmp << endl;
  \endcode

 */
class Buffer {
private:
  //!Pointer to internal buffer.
  char* buf;

  //!Size of the buffer.
  unsigned int bufsize;

  //!Tell if the buffer should be freed once the object is destroyed.
  enum BufferFreeOnDestroy freebuffer;

  //!Tell if we are handling a user allocated buffer.
  bool extbuffer;

  //!If a window has been selected.
  bool select;

  //!Beginning of the window (0-based).
  unsigned int wstart;

  //!Length of the window.
  unsigned int wlen;

public:
  /*!
    \brief Constructor, buffer allocation is handled internally.

    \param freeBuffer If true the buffer will be freed once the object
    is destroyed, otherwise the buffer must be freed by the user.
  */
  Buffer(enum BufferFreeOnDestroy freeBuffer = AUTO_FREE);

  /*!
    \brief Constructor, buffer allocation handled by user.

    \param extbuf Pointer to the buffer;
    \param size Size of the buffer being pointed by "extbuf".
  */
  Buffer(void* extbuf, unsigned int size,
	 enum BufferFreeOnDestroy freeBuffer = DONT_FREE);

  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(Buffer);


  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(Buffer);

  //!Destructor, will free the buffer if "freeBuffer" is true.
  ~Buffer();


  void set(void* extbuf, unsigned int size, enum BufferFreeOnDestroy freeBuffer);


  /*!
    \brief Select a "window" in the buffer.

    A "window" is a part of the buffer which is to be read or written
    using a succesive call to the "<<" or ">>" operators.

    This operator is used to define such a window.

    \param start Beginning of the window (0-based);
    \param len Length of the window.
  */
  Buffer& operator()(unsigned int start, unsigned int len);


  /*!
    \brief Select a "window" in the buffer.

    This operator is similar to operator()(unsigned int, unsigned
    int) with the exception that in this case the start of the window
    is assumed to be at the end of the buffer when using the "<<"
    operator, at the beginning when using the ">>" operator.
  */
  Buffer& operator()(unsigned int len);


  /*!
    \brief Copy data from an external buffer.

    Data are copied into the selected window on internal buffer.

    \param extbuf The pointer to the external buffer from which data
    are to be copied.
  */
  Buffer& operator<<(const void* extbuf);

  Buffer& operator<<(istream& stream);

  /*!
    \brief Copy data into an external buffer.

    Data are copied into the external buffer from the selected window
    on internal buffer.

    \param extbuf The pointer to the external buffer to which data are
    to be copied.
   */
  Buffer& operator>>(void* extbuf);

  Buffer& operator>>(ostream& stream);

  operator void*() const { return buf; }


  //!Use the Buffer class an an array of pointers to char.
  char* operator[](unsigned int pos);


  /*!
    \brief Free allocated buffer.

    If we are using an internal buffer (i.e. the Buffer(num
    BufferFreeOnDestroy) constructor has been used) then the internally
    allocated buffer will be freed.
  */
  void free();


  /*!
    \brief Check size, and eventually enlarge allocated memory.

    If the buffer is handled internally then this method will check if
    it has enough space to hold "size" bytes, if not it reallocates
    the buffer to a bigger size.

    If the buffer used is handled by the user then it perform the same
    check but if the size is not enough an exception will be thrown.

    You usually don't need to call this method since it is
    automatically called by operators.

    \param Size of the buffer.
   */
  void resize(unsigned int size);

  //!Return size of the buffer.
  unsigned int size();
};


















#define MCS_SERIAL_UNKNOWN       0
#define MCS_SERIAL_BUFFER        1
#define MCS_SERIAL_FILENAME      2

//--------------------------------------------------------------------
//MCS Serializable class
/*!
  \brief Serialize memory buffers or files into chunks.

  This class is used from the Socket class to split data buffers into
  chunks of a maximum size, so they can be sent across the
  network. Objects that must be sent through the network must derive
  this class and eventually implement its virtual methods.

  Chunks are extracted from the source using the serialize_buffer()
  virtual method.

  This class has an internal type specification which is used to
  determine how to retrieve data, this specification can be one of
  MCS_SERIAL_BUFFER (for memory buffers), MCS_SERIAL_FILENAME (for
  files) and MCS_SERIAL_UNKNOWN (for custom data). In this last case
  the user must implement the serialize_unknown() and
  nextChunk_unknown() virtual methods to properly read data into the
  internal buffer.
*/
class Serializable
{
private:
  MCS_DEBUG_ALLOC;

  //!Pointer to temporary buffer containing the next chunk.
  char* buf;

  //!Maximum size of the chunk.
  unsigned int maxChunksize;

  //!Size of the actual chunk.
  unsigned int lsize;

  //!Filename of the file being sent, if type = MCS_SERIAL_FILENAME.
  string filename;

  //!Type of data to be sent.
  int type;

  //!Pointer to source buffer, if type = MCS_SERIAL_BUFFER.
  char* from;

  /*!
    \brief If true the external buffer will be automatically freed,
    used when type = MCS_SERIAL_BUFFER.
   */
  bool freeAfterUse;

  //!Pointer to an ifstream, if type = MCS_SERIAL_FILENAME.
  ifstream* stream;

  /*!
    \brief Common initialization.

    \param lmaxChunksize Max allowed size for a chunk.
  */
  void init(int type, unsigned int lmaxChunksize);

  char* userdata; //To be used with virtual_nextChunk

  bool firstTime; //To be used with virtual_nextChunk

  /*!
    \brief Custom routine to fill a buffer with a chunk.

    This virtual method should be overloadd in derived class, to
    fill the internal buffer when type = SERIAL_UNKNOWN. This method
    is called automatically by fillBuffer().

    \param buf Address of the buffer to fill with the chunk.
    \param chunksize Upon exit must contain the size of the chunk.
    \param firstTime Tell if it is the first time the method is called.
  */
  virtual char* nextChunk_unknown(char* userdata, char* buf,
				  unsigned int& chunksize, bool firstTime);

  virtual char* serialize_unknown();

  virtual bool serialize_buffer(char*& from, unsigned int& size);

  virtual string serialize_filename();

  void serialize();

  void serialize_end();

public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(Serializable);


  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(Serializable);

  /*!
    \brief Constructor, for custom data (type = SERIAL_UNKNOWN).

    \param lmaxChunksize Max allowed size for a chunk, default is
    MCS_CHUNKSIZE bytes.
  */
  Serializable(unsigned int lmaxChunksize = MCS_DEFAULTCHUNKSIZE);


  /*!
    \brief Constructor, for memory buffers (type = SERIAL_BUFFER).

    \param buf Address of the buffer containing data to be sent.
    \param size Size of the buffer.
    \param lmaxChunksize Max allowed size for a chunk, default is
    MCS_CHUNKSIZE.
  */
  Serializable(void* from, unsigned int size, bool freeAfterUse,
	       unsigned int lmaxChunksize = MCS_DEFAULTCHUNKSIZE);


  /*!
    \brief Constructor, for files (type = SERIAL_FILENAME).

    \param filename File to be sent.
    \param lmaxChunksize Max allowed size for a chunk, default is
    MCS_CHUNKSIZE.
  */
  Serializable(string filename, unsigned int lmaxChunksize = MCS_DEFAULTCHUNKSIZE);

  Serializable(int type, unsigned int lmaxChunksize = MCS_DEFAULTCHUNKSIZE);

    /*!
      \brief Destructor.
    */
    virtual ~Serializable();


//    /*!
//      \brief Fills a buffer with next chunk to be sent.
//
//      \param chunksize Upon exit contains the size of the chunk.
//      \return Address of the buffer containing the chunk, or NULL if there are
//      no more data.
//    */
//    char* fillBuffer(unsigned int& chunksize);

  void* nextChunk(unsigned int& chunksize);

  void* getEntireBuffer(unsigned int& size);

    //!Return the max size allowed for a chunk.
    unsigned int maxChunkSize();


//    /*!
//      \brief Reinitialize internal data.
//
//      Reinitialize internal data, so that next call to fillBuffer() will read
//      data from the beginning of the buffer or the file. If type =
//      SERIAL_UNKNOWN the next call to fillBuffer will have firstTime =
//      true.
//    */
//    void abort();


//    /*!
//      \brief Tells if the object is in use.
//
//      If a call to fillBuffer() has already be done, and data are not yet
//      finished this will return true. Otherwise return false.
//    */
//    bool inUse();


    /*!
      \brief Tells if we already know the size of entire block of data.

      If type = SERIAL_FILENAME or SERIAL_BUFFER, we already know the size
      of the entire block of data, and this return true. If type =
      SERIAL_UNKNOWN return false.
    */
    bool knowSize();

    /*!
      \brief If knowSize() is true, return how many chunks are
      required to send all data.
    */
    unsigned int nChunk();


    /*!
      \brief If knowSize() is true, return the size of the entire block of
      data.
    */
    unsigned int size();
};







//--------------------------------------------------------------------
/*!
  \brief Retrieve informations about network interfaces.

  This class can be used to obtain informations such as the name or the IP
  address of a network interface. It is used by the ServerSocket class to
  obtain a "struct sockaddr_in" needed to create a server socket.
 */
class NetInterface
{
protected:
  unsigned int lindex;

  int req(int ioctl_num, struct ifreq *ifr);
  int getflags();
  bool isup();
  void str_sockaddr(struct sockaddr* sa);
  void str_sockaddr_in(struct sockaddr_in* sin);

  //!Vector containing the name of all interfaces present in the system.
  vector<string> names;

public:
  /*!
    \brief Constructor.

    This constructor create a NetInterface objects linked to the
    interface named in the first parameter, so that any call to
    methods will return informations about this interface. If you want
    to know the names of the interfaces present in the system you
    should create the object without any parameter (deafult value for
    name is "lo"), then call the name() method with an index going
    from 0 to count().

    \param name Name of the interface, default is "lo" for the
    localhost interface (127.0.0.1).
   */
//LN-darwin
#ifdef __APPLE__
  NetInterface(string name = "lo0");
#else
  NetInterface(string name = "lo");
#endif

//NetInterface(string name = "127.0.0.1");

  //!Destructor.
  ~NetInterface();

  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(NetInterface);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(NetInterface);

  /*!
    \brief Return the name of an interface.

    If no argument is given will return the name of the actual
    interface, otherwise will return the name of the interface with
    the index equal to the argument. Note that the index must be less
    then count().

    \param index Interface index, default is -1 for actual interface.
  */
  string name(int index = -1);

  //!Return the index of the actual interface.
  unsigned int index();

  //!Return how many network interfaces are present in the system
  unsigned int count();

  /*!
    \brief Return the IP address of an interface.

    If no argument is given will return the address of the actual
    interface, otherwise will return the address of the interface with
    the index equal to the argument. Note that the index must be less
    then count().

    \param index Interface index, default is -1 for actual interface.
  */
  string ipaddress(int index = -1);
};







//--------------------------------------------------------------------
/*!
  \brief Retrieve informations about a network host.

  This class can be used to obtain informations such as the host name or the
  IP address for a network host. It is used by the Socket to obtain a "struct
  sockaddr_in" needed to connect to a remote host.
 */
class HostInfo
{
protected:
  struct sockaddr_in sin;
  string host;
  string ipaddr;
  void populate_sockaddr_in();

public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(HostInfo);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(HostInfo);

  /*!
    \brief Constructor.

    This constructor create a HostInfo object with informations about
    the host address given as parameter. The host address can be
    either an IP address or a valid alias if there is a working DNS in
    the system.

    \param host Address of the remote host.
   */
  HostInfo(string host);

  /*!
    \brief Constructor, to obtain information from an already connected socket.

    \param sockfd C socket descriptor.
  */
  HostInfo(int sockfd);

  //!Destructor.
  ~HostInfo();

  //!Return the host name.
  string hostname();

  //!Return the host IP address.
  string ipaddress();
};






//--------------------------------------------------------------------
/*!
  \brief Manage TCP sockets.

  This class implements a TCP socket. It has a timeout facility, as well as a
  mechanism to check that a connection is still open and ready to send/receive
  data. It is strongly recommended that the program ignores all SIGPIPE
  signals for this to work correctly, this can be done in the main program
  with:

  \code
  #include <signal.h>
  ...
  signal(SIGPIPE, SIG_IGN);
  \endcode
*/
class Socket : public HostInfo
{
private:
  MCS_DEBUG_ALLOC;

  //!C socket file descriptor.
  int sockfd;

  //!Set of socket file descriptors, used with select().
  fd_set fds;

#ifdef HAVE_OPENSSL_SSL_H
  //!Pointer to a SSL context object, provided in the constructor.
  SSL_CTX* ssl_ctx;

  //!Pointer to a SSL object.
  SSL *ssl;

  //!Pointer to a SSL BIO object.
  BIO *sbio;

  void initialize_ssl_bio(string keyfile);
#endif //HAVE_OPENSSL_SSL_H

  //!Port on which this socket will connect.
  unsigned short int port;

  //!True if the connection should be encrypted.
  bool use_ssl;

  //!Timeout interval for read operations.
  struct timeval readto;

  //!Timeout interval fro write operations.
  struct timeval writeto;

  /*!
    \brief Connect to the host.

    This method tries to connect to the host specified in the
    constructor, to the port specified as parameter.

    \note The timeout interval to connect to a remote host is not the
    one you specified in the constructor, but is set by the operating
    system.

    \param port Port to connect to.
    \return C socket file descriptor.
    \exception FATAL MSG_CANT_CONNECT_TO_HOST.
   */
  int socketToHost(unsigned short port);


  /*!
    \brief Send a chunk of binary data through the socket.

    This method send a chunk of binary data to another mcs::Socket
    object attached to the other side of the socket, using the MCS
    protocol. On the other side the Socket object should be waiting
    for data in the Socket.recvChunk() method.

    \param buf Pointer to data to send;
    \param size Size of the buffer.

    \exception WARN MSG_SEND_ABORT_BY_RECEIVER.
  */
  void sendChunk(void* buf, unsigned int size);


  /*!
    \brief Receive a chunk of binary data from the socket.

    This method receives a chunk of binary data from another
    mcs::Socket object attached to the other side of the socket, using
    the MCS protocol. On the other side the Socket object should send
    data with the Socket.sendChunk() method.

    \param buf Pointer to a buffer where store data, this buffer must
    be big at least "size" bytes;
    \param size Size of data received.

    \exception MCS_FATAL MSG_PROTOCOL;
    \exception MCS_FATAL MSG_NOT_ENOUGH_SPACE.
   */
  unsigned int recvChunk(void* buf, unsigned int size);

  /*!
    \brief Same as recvChunk(void*, unsigned int) but stores data in
    the AccumBuffer object passed as parameter.

  */
  unsigned int recvChunk(Buffer* buf);

protected:
  /*!
    \brief Check if you can send data through this socket.

    This is just a check that should be made just before any write to
    the socket.

    \param throwexc If DONT_THROW will return false instead of throwing an
    exception.

    \return True if its possible to write in the socket, false
    otherwise.

    \exception FATAL MSG_CALLING_SELECT;
    \exception FATAL MSG_TIME_OUT;
    \exception FATAL MSG_UNEXPECTED.

  */
  bool chkSend(enum ThrowExceptions throwexc = THROW);


  /*!
    \brief Check if you can read data from this socket.

    This is just a check that should be made just before any read from
    the socket. Depending on the throwexc it will throw a FATAL
    exception or return false if the remote socket has been closed or
    timed out.

    \param chkDataAvailable Will set timeout to zero, so that it will only
    check for data availability without waiting.

    \param throwexc If DONT_THROW will return false instead of throwing an
    exception.

    \return True if its possible to read from the socket, false
    otherwise.

    \exception FATAL MSG_CALLING_SELECT;
    \exception FATAL MSG_TIME_OUT;
    \exception FATAL MSG_CALLING_RECV;
    \exception FATAL MSG_CLOSED_BY_PEER;
    \exception FATAL MSG_UNEXPECTED.
  */
  bool chkRecv(bool chkDataAvailable = false, enum ThrowExceptions throwexc = THROW);




public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    This constructor is declared but not implemented. If you try to use it you
    will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(Socket);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(Socket);

  /*!
    \brief Constructor, to create a new connection.

    This creates a new socket connecting to a specified host and
    port. It is possible to specify read and write timeout for the
    newly created socket. This can throw Event exceptions if the
    connection cannot be made.

    \param host Host name to connect to;
    \param port Port to connect to;
    \param readTimeout Timeout for reading operations, in millisecond;
    \param writeTimeout Timeout for writing operations, in millisecond;
    \param ssl If the socket should use a secure connection.
  */
  Socket(string host, unsigned short int port=0,
	 unsigned int readTimeout = 1000,
	 unsigned int writeTimeout = 1000,
	 bool ssl = false);


  /*!
    \brief Constructor based on an already existent socket.

    This creates a new Socket object based on an already existent socket,
    provided, for example, from a ServerSocket object. It is possible to
    specify read and write timeout for the newly created socket.

    \param sockfd The C socket file descriptor;
    \param readTimeout Timeout for reading operations, in millisecond;
    \param writeTimeout Timeout for writing operations, in millisecond.
    \param ssl_ctx The SSL context object, if the socket should use a
    secure connection.
  */
  Socket(int sockfd,
	 unsigned int readTimeout = 1000,
	 unsigned int writeTimeout = 1000,
	 void* ssl_ctx = NULL);


  /*!
    \brief Destructor.

    This flushes the socket with a write timeout, then close the
    socket.
  */
  ~Socket();

  //!Close the socket
  void Close();

  /*!
    \brief Fill a "struct timeval" with the given interval.

    \param millisec Interval in milliseconds.
    \param time Address of a "struct timeval".
   */
  static void set_struct_timeval(unsigned int millisec, struct timeval* time);


  /*!
    \brief Reads data from the socket and write in a buffer.

    This will read count bytes from the socket and write in the memory
    buffer pointed by buf. It checks socket availability with
    chkRecv().

    \param buf Buffer to store data;
    \param count Number of bytes to read from the socket.

    \return Number of bytes actually read.

    \exception FATAL MSG_CLOSED_BY_PEER.
  */
  unsigned int read(void* buf, unsigned int count);


  /*!
    \brief Reads data from a buffer and write in the socket.

    This will read count bytes from the buffer pointed by buf and
    write in the socket. It checks socket availability with chkSend().

    \param buf Buffer from which read data to send;
    \param count Number of bytes to read from the buffer.

    \return Number of bytes actually written in the socket.

    \exception FATAL MSG_CALLING_SEND
  */
  unsigned int write(void* buf, unsigned int count);


  /*!
    \brief Reads from a socket until a newline is encountered.

    Internally calls chkRecv, so this can throw exceptions if the
    socket has been closed or the operation had timed out.

    \return a A string containing the data read without the newline.

    \warning Don't use this if you want to receive binary data.
  */
  string getline();


  /*!
    \brief Writes a string in the socket adding a newline.

    Internally calls chkSend, so this can throw exceptions if the
    socket has been closed.

    \param s The string containing the data to be send.

    \warning Don't use this if you want to send binary data.
  */
  void print(string s);


  /*!
    \brief Send a block of data through the socket.

    The data must be handled through a Serializable object or a derived class.

    \param from Addrees of a Serializable object used to send data.
   */
  void sendData(Serializable* from);


  /*!
    \brief Receive data and store in a buffer.

    If the buffer is not yet created it will be automatically
    allocated. In this case it is the user responsibility to free the
    buffer after use. Note that this buffer will contain all data, not
    just a chunk. The method will consider the buffer allocated if
    *buffer is not NULL, and will write in that buffer. The second
    parameter tells the size of the buffer, and must be large enough
    to store all data otherwise an exception will be thrown.

    \param buffer Address of a pointer to the buffer;
    \param maxsize Size of the buffer, if already allocated.

    \return Size of the data written in the buffer.

    \exception MCS_FATAL MSG_PROTOCOL;
    \exception MCS_FATAL MSG_NOT_ENOUGH_SPACE.
  */
  unsigned int recvData(char** buffer, unsigned int maxsize);


  /*!
    \brief Receive data and write to a file.

    \param filename File to be written.
    \return Size of the data written in the file.

    \exception MCS_FATAL MSG_PROTOCOL;
    \exception MCS_FATAL MSG_CANT_OPEN_FILE.
   */
  unsigned int recvData(string filename);

  /*!
    \brief Receive data and write to a file descriptor.

    \param filedes libc's file descriptor.
    \return Size of the data written in the file.

    \exception MCS_FATAL MSG_PROTOCOL;
   */
  unsigned int recvData(int filedes);

  /*!
    \brief Receive data and write to a ofstream object.

    \param stream C++ ofstream object.
    \return Size of the data written in the file.

    \exception MCS_FATAL MSG_PROTOCOL;
   */
  unsigned int recvData(ofstream& stream);
};





//--------------------------------------------------------------------
/*!
  \brief Manage TCP server sockets.

  This class implements a TCP server socket, that is an object that
  wait for a client connection to arrive, using the acceptConnection()
  method. When this connection is established the method returns the
  new socket descriptor.
 */
class ServerSocket : public NetInterface
{
private:
  //!Server socket descriptor.
  int sockfd;

  //!True if the connection should be encrypted.
  bool use_ssl;

#ifdef HAVE_OPENSSL_SSL_H
  //!Pointer to the global SSL context object.
  SSL_CTX* ssl_ctx;
#endif //HAVE_OPENSSL_SSL_H

public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(ServerSocket);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(ServerSocket);

  /*!
    \brief Constructor.

    This constructor create a server socket using the interface and
    port specified as arguments.

    \param interface Name of the network interface to be used as
    server;
    \param port Port on which the server should listen.

    \exception FATAL MSG_CALLING_BIND;
    \exception FATAL MSG_CALLING_LISTEN;
   */
  ServerSocket(string interface, unsigned short int port,
	       bool use_ssl = false, string sslcert = "", string sslpriv = "");

  //!Destructor.
  ~ServerSocket();

  /*!
    \brief Wait for an incoming connection.

    This method will wait until an incoming connection arrives or a timeout
    occur. When the connection arrives it returns the newly created C socket
    descriptor through the "newsock" parameter.

    \param newsock Upon return will contain the newly created C socket;
    \param millisec Timeout in millisec.

    \return True if a new connection arrived, false if a timeout occurred.
   */
  bool acceptConnection(int& newsock, unsigned int millisec);

  void* getSSLContext();
};





//--------------------------------------------------------------------
/*!
  \brief A simple class to implement "critical sections".

  Critical sections are portions of code which cannot be executed by
  two different threads simultaneously, that is one thread must wait
  that the other has finished before it can execute it. This is often
  useful to synchronize threads, or let them access shared
  variables. Critical sections are implemented in MCS using
  mutexes. For example if two threads would execute the same code,
  accessing the same variable, then you must implement a critical
  section like the following:

  \code
  int shared; //A variable shared between threads.
  Synchro syn; //Object used to implement the critical section.

  void function() {
    syn.enter();  //Enter the critical section

    ...           //Manipulate the shared variable

    syn.leave()   //Exit the critical section
  }
  \endcode

  In this code, when multiple threads are executing the function at
  the same time, then it is guaranteed that only one thread will
  execute the code in the critical section, while the other are
  suspended (and consume no CPU time).

  Anyway developer should pay attention because critical sections, or
  better a call to enter(), can be a potential deadlock, that is a
  point at which the thread remain suspended forever. Actually a
  thread can be killed (or better cancelled) by itself or by other
  threads only when it is executing a so called "cancellation point",
  typically I/O operations or some system call (like (sleep()). The
  system call to lock a mutex is not a cancellation point, so if the
  thread is suspended waiting to lock a mutex it cannot be cancelled,
  the only way to kill it is to kill the entire program using
  signals. On the other hand enter() and tryenter() are cancellation
  points, so if a thread received a cancellation request while it was
  suspended, this methods won't return at all. In other words, once a
  thread is waiting to enter a critical section that is ran by some
  other thread, it can't be cancelled until the other thread leave the
  section. For this reason every critical section must be balanced,
  that is it must have a starting point (using the enter() or
  tryenter() methods) and an ending point (using the leave() method).

  For these reasons developers should design critical sections
  appropriately, here we'll give some useful hints:

  - critical sections aren't useful in that code executed by only one
    thread;

  - critical sections are needed when multiple threads wants to modify
    a shared avariable at the same time;

  - critical sections aren't needed when multiple threads wants to
    read a shared avariable at the same time;

  - In critical sections you usually put a call to enter() at the
    beginning of the section and a leave() at the end, but this could
    not be enough to implement a balanced critical section, because
    the control flow can be interupted by several event:

    - a "return" instruction is encountered;
    - a "goto" instruction is encountered;
    - an exception is thrown;
    - the thread is cancelled (either by other threads or by itself);

    In the first three cases you MUST provide the balancing call to
    leave(), anyway we recommend not to use any "return" or "goto"
    instruction inside critical sections. To handle exceptions you can
    write a code like the following:

    \code
    enter();
    try {
       .... //Critical section
    }
    catch (Event e) {
      leave();
      throw e;
    }
    leave();
    \endcode

    As you can see there are one call to the enter() method, but two
    call to leave(), one for each possible exit point in the
    function. The code above can be easily implemented using the
    MCS_CRITICAL_SECTION macros. In the fourth case the Synchro object
    will leave the section automatically;


  Critical sections can be nested, that is a thread can enter a
  critical section any number of time, while the others are
  waiting. Each thread must also leave critical sections the same
  number of time. Other threads are allowed to enter a critical
  section once it is completely free.
*/
class Synchro
{
private:

  //  MCS_DEBUG_ALLOC;

//LN-darwin
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS - 200112L) >= 0L
/* POSIX Timeouts are supported - option group [TMO] */
 
  ///!Structure used to implement the cleanup handler.
  //  struct _pthread_cleanup_buffer _buf;
#endif

  //!The mutex used to do the job.
  pthread_mutex_t mutex;

  //!Attribute of the mutex (PTHREAD_MUTEX_RECURSIVE_NP).
  pthread_mutexattr_t attr;

  //!How many time the Mutex has been locked, or a thread entered a critical section.
  int Count;

  //!Whether this object is a "dummy" object or not.
  bool isActive;

public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
    MCS_MISSING_COPY_CONSTRUCTOR(Synchro);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
    MCS_MISSING_ASSIGNMENT_OPERATOR(Synchro);

  /*!
    \brief Constructor.

    This contructor initialize a "dummy" object, that is an object
    with synchronization disabled. To enable it call synchronize().

    That is because several class are derived from Synchro, but not
    all objects of those classes needs to be synchronized.
  */
  Synchro();

  /*!
    \brief Destructor.
  */
  ~Synchro();

  /*!
    \brief Enable or disable the synchronization feature.

    You must enable or disable the synchronization feature before
    callling any other method of this class, otherwise an exception
    will be thrown.

    \param setactive If false this became a "dummy" object, that is
    every call returns immediately without being synchronized.
   */
  void synchronize(bool setactive);

//!To be used with Synchro.enter(): enter a critical section.
#define MCS_SYNCHRO_LOCK        1

//!To be used with Synchro.enter(): try to enter a critical section.
#define MCS_SYNCHRO_TRY_LOCK    2

//!To be used with Synchro.enter(): try to enter a critical section with
//!timeout facility.
#define MCS_SYNCHRO_TRY_TIMED   3

  /*!
    \brief Enter, or try to enter a critical section.

    If other threads are locking the critical section the behaviour of
    this method depends on the first parameter:

    - MCS_SYNCHRO_LOCK, the thread will wait forever until the
      critical section became unlocked.

    - MCS_SYNCHRO_TRY_LOCK, the thread will try to enter the critical
      section, then return immediately.

    - MCS_SYNCHRO_TRY_TIMED, the thread will try to enter the critical
      section, if it cannot lock the section after the timeout has
      expired it will return.

    If you specify MCS_SYNCHRO_LOCK as first parameter, then the
    thread must lock the section before it can do anything else. Once
    the section has been locked this method will return, or the thread
    will be cancelled (this method won't return) if a cancellation
    request arrived in the meanwhile.

    \param op A code specifying the behaviour in case the section is
    already locked;

    \param timeout Use this only if op=MCS_SYNCHRO_TRY_TIMED, the
    timeout in milliseconds after which a try to lock the section will
    fail;

    \return True if the section has been locked, false otherwise. If
    op=MCS_SYNCHRO_LOCK this method will always return true or never
    return.
  */
  bool enter(int op = MCS_SYNCHRO_LOCK, unsigned int timeout = 0);

  //!Wrapper around enter(), using op=MCS_SYNCHRO_TRY_LOCK if the parameter is
  //!0, op=MCS_SYNCHRO_TRY_TIMED otherwise.
  bool tryenter(unsigned int timeout = 0);

  //!Return how many times the current thread has locked the section.
  int count();

  /*!
    \brief Leave a critical section.

    Leave a critical section (unlock the mutex). There must be a call
    to this method for each exit point in the critical section.

    \return How many times the thread should call this method to
    completely unlock the section.
   */
  int leave();

#ifdef ENABLE_CLEANUP_HANDLERS
  /*!
    \brief Used to implement a cleanup handler.

    This method unlock a section if a thread is cancelled during a
    critical section.

    \warning This method must never be called by users.
  */
 static void CH_leave(void* This);
#endif //ENABLE_CLEANUP_HANDLERS

  ///*!
  //  \brief Waits until some other thread locks and unlocks the same
  //  Synchro object.
  //
  //  The mutex MUST be locked before using wait() (with enter()) and unlocked
  //  after this method returns (with leave()).
  //
  //  When this method is called it immediately unlock the mutex, so
  //  that another thread can lock it.
  //
  //  If it return true then another thread has locked and unlocked the same
  //  Synchro object, if it returns false a timeout occurred before other
  //  threads can lock and unlock the mutex. in any case the mutex is now locked
  //  by the current thread, so you have to unlock it with leave().
  //
  //  An example follows:
  //  \code
  //  ...
  //  enter();
  //  bool b = wait(1000); //Leaving the critical section and wait
  //                       //for someone else to lock and unlock the
  //			 //same Synchro object, or a timeout of 1
  //			 //second occur.
  //
  //  if (b) {  //Another thread entered and leave a critical section
  //    ... //Do some work
  //  }
  //  else { //A timeout occurred
  //    ...
  //  }
  //  leave();
  //  \endcode
  //
  //  \param timeout Timeout in millisecond.
  //  \return False if a timeout or an interrupt occurred, true otherwise.
  //*/
  ////bool wait(unsigned int timeout);
};


//!Start a critical section, there must be no "return" or "goto" instruction
//!inside the section.
#define MCS_CRITICAL_SECTION_BEGIN  enter(); try {


//!End a critical section, there must be no "return" or "goto" instruction
//!inside the section.
#define MCS_CRITICAL_SECTION_END    } catch(Event e) { leave(); throw e; } leave();

#define MCS_CRITICAL_SECTION_END_RETURN(What...)  \
leave(); return What; }                           \
catch(Event e) { leave(); throw e; }


/*!
  \def MCS_CRITICAL_SECTION
  \brief Execute a list of commands inside a critical section.

  There must be no "return" or "goto" instruction inside the section.
*/
#define MCS_CRITICAL_SECTION(BLOCK...) \
MCS_CRITICAL_SECTION_BEGIN             \
  BLOCK;                               \
MCS_CRITICAL_SECTION_END







//--------------------------------------------------------------------
//!Thread state: the object has been created ut the separate thread has not been started.
#define MCS_STATE_CREATED                1

//!Thread state: the separate thread is executing the run() method.
#define MCS_STATE_RUNNING                2

//!Thread state: the exit() method is executing, the separate thread is going
//!to execute the final() method.
#define MCS_STATE_TERMINATING            3

//!Thread state: the separate thread had terminated its execution.
#define MCS_STATE_END                    4



/*!
  \brief A class to create separate threads.

  This class let you run code in a separate thread. You can derive
  this class and overload its virtual method run(), this will be the
  thread body being executed when you start the new thread.

  A thread can be started from the parent thread in two ways:

  - in the joinable state (using the start() method): this way another
    thread (for example the parent) can stop the newly created thread
    while it is running.

  - in the detached state (using the startDetached() method): this way
    the thread can only terminate by itself.

  The child thread will always execute three methods: initial(),
  run(), final(). These are virtual methods, so you can overload them
  in derived classes. The initial() method can be used to perform
  initialization tasks. It is useful because it is guarranteed that
  the start() (or startDetached()) method won't return until the
  initial() method has returned, so the main thread knows that the
  separate thread is already running. The run() method is the body of
  the separate thread execution. A thread can terminate in three
  different ways:

  - the control reach the end of the overloaded run() method;

  - an exception is thrown inside the overloaded run() method;

  - the stop() method is called from another object/thread (applicable
    only if the thread has been started in the joinable state).

  The final() method can be used to free resources allocated by the
  separate thread. This method is useful because a thread can
  terminate either by returning from the run() method, or because it
  is stopped from another thread.  In
  any case the final() method will be executed in the separate
  thread, so it can be used to free allocated resources.

  If the adress of a parent object has been provided in the
  constructor call, then it will be notified of thread termination
  (see notify() method), after the final() method has been executed.

  This class provide the state() method which tells in which "state"
  of its life-cycle a thread is. The value of the state can be one of
  the follows:

  - MCS_STATE_CREATED: the object has been created but the separate
    thread has not been started.

  - MCS_STATE_RUNNING: the separate thread has executed initial()
    method and it is executing the run() method.

  - MCS_STATE_TERMINATING: the run() method has returned or the stop()
    method has been called, and the thread is going to execute the
    final() method and eventually notify its parent of thread
    termination.

  - MCS_STATE_END: the separate thread has executed the final()
    method, (eventually) notified its parent and terminated its
    execution.
*/




class Thread
{
private:

  //  MCS_DEBUG_ALLOC;

  static void* RunThread(void* args);

  //!Reference to parent Thread object, if any is given in the constructor.
  Thread* lparent;

  //!System's thread identifier.
  pthread_t lthrID;

  //!General purpose Thread identifier.
  int lid;

  //!State of the object, see state() method.
  int lstate;

  //Synchronization for lstate variable.
  //  Synchro syn_lstate;

  /*!
    \brief Static wrapper around final().

    This method is automatically called once another thread tries to
    terminate the thread.
  */
  static void cleanup_Handler(void* This);

  /*!
    \brief Starting point for the separate thread.

    This method calls the appropriate virtual method run().
  */

  //!Check if the thread is already terminating (true) or not (false).
  bool checkTerminating();

  //!Tell if the thread is to be created in the detached state.
  bool detached;

  //!Tell if the object should delete himself once the thread is
  //!terminated.
  bool selfDelete;

protected:
  /*!
    \brief A method called from child threads to notify their termination.

    If the address of a parent thread is provided in the constructor
    then the child can notify its parent of its termination through
    this method.

    Note that this method will always be ran inside the child thread.

    This method can be used to destroy Thread objects once their
    thread is terminated, for example:

    \code
    void MyThread::notify(int id, Thread* ref)
    {
      cout << "Thread " << id << " is terminated." << endl;
      delete ref;
    }
    \endcode

    \param id Thread identifier of the terminating thread.
    \param ref Address of the Thread object whose thread is terminated.
  */
  virtual void notify(int id, Thread* ref);


  /*!
    \brief Initialization method.

    This method is executed inside the newly created thread once the
    start() method is called from parent thread.

    You can use this method to perform initializations, it is
    guarranteed that the start() method won't return until this method
    is completely executed.
  */
  virtual void initial();


  /*!
    \brief Finalization method.

    This method is executed inside the separate thread once the thread
    has terminated (the run() method has returned or another thread
    called the stop() method).

    You can use this method to perform finalizations like memory cleanup,
    objects destructions, etc...

    \warning Don't use this method to destroy the Thread object,
    instead use the notify() method of parent's thread, or start the
    thread in the detached state with the "selfDelete" flag set to
    true.
  */
  virtual void final();


  /*!
    \brief Body of the thread execution.

    When the new thread starts it will automatically execute the code
    in the run() method. This is the method you should overload in
    derived class to do the work you need.

    Thread will be terminated once this method returns.
  */
  virtual void run();

  /*!
    \brief Set cancellation state for current thread.
   */
  void set_cancel_state(bool cancel);

  /*!
    \brief Test if a cancellation request is pending.
  */
  void test_cancel();

  //!Last error.
  Event* lerror;

public:
  //!Return last error message.
  Event* error();

  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you
    try to use it you will get a compilation error.
  */

  //  MCS_MISSING_COPY_CONSTRUCTOR(Thread);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try
    to use it you will get a compilation error.
  */


  //  MCS_MISSING_ASSIGNMENT_OPERATOR(Thread);

  /*!
    \brief Constructor.

    \param id Thread general purpose identificator, this can be
    retrieved with the id() method.

    \param parent Address of a parent Thread object, this will be used
    to notify the thread termination. If NULL is passed no
    notification will occur.
  */
  Thread(int id = 0, Thread* parent = NULL);

  /*!
    \brief Destructor.

    You should avoid destroying Thread objects unless you are sure
    that the separate thread has terminated.

    A good choice to destroy Thread objects is to place a "delete"
    statement inside the notify() method, otherwise you can start the
    thread in the detached state (with the startDetached() method) and
    set the "selfDelete" flag to true.
  */
  virtual ~Thread();

  /*!
    \brief Start a new thread in the joinable state.

    If you start the thread using this method then you can stop the
    thread execution using the stop() method.

    This method will return only after the initial() method has been
    executed.

    \exception MCS_FATAL MSG_CALLING_PTHREAD_CREATE;
  */
  void start();


  /*!
    \brief Start a new thread in the detached state.

    If you start the thread using this method then you cannot stop the
    thread execution, it will terminate by itself.

    This method will return only after the initial() method has been
    executed.

    \param selfDelete If set to true once the thread is terminated the
    object will be automatically destroyed.

    \exception MCS_FATAL MSG_CALLING_PTHREAD_CREATE;
  */
  void startDetached(bool selfDelete = false);


  /*!
    \brief Stop thread execution.

    This method has an effect only if the thread has been started in
    the joinable state (using the start() method).

    If the child can notify its termination it will do it before this
    method returns.
  */
  void stop();

  /*!
    \brief Returns the Thread object identificator.

    This is a general purpose identificator, it's not used inside the
    class so user can give it any value (using the constructor).

    \return Thread object identificator.
  */
  int id();

  /*!
    \brief Returns the address of the parent.
  */
  Thread* parent();


  /*!
    \brief Return the state of the thread.

    The thread can be if four different states:
    - MCS_STATE_CREATED;
    - MCS_STATE_RUNNING;
    - MCS_STATE_TERMINATING;
    - MCS_STATE_END;

    \return The state of the thread.
  */
  int state();
};



template<class BASE>
class ThreadSpecificData
{
private:
  pthread_key_t key;
  int ltag;

  static void generic_destructor(void* p)
  { delete ((BASE*) p); }

  void (*destructor)(void*);

public:
  ThreadSpecificData(void (*ext_destructor)(void*) = NULL)
  {
    destructor = generic_destructor;
    if (ext_destructor)
      destructor = ext_destructor;

    pthread_key_create(&key, destructor);
  }

  ~ThreadSpecificData()
  { pthread_key_delete(key); }

  void clear()
  {
    void* p = getp();
    if (p)
      (destructor)(p);

    pthread_setspecific(key, NULL);
  }

  void init(BASE* p = NULL, int tag = 0)
  {
    clear();

    if (! p)
      p = new BASE();

    pthread_setspecific(key, p);
    ltag = tag;
  }

  int tag()
  { return ltag; }

  BASE* operator->() const
  { return  ((BASE*) pthread_getspecific(key)); }

  BASE* getp()
  { return  ((BASE*) pthread_getspecific(key)); }
};





class ThreadFunc : public Thread
{
private:
  int    (*start_routine1)(void*);
  Event* (*start_routine2)(void*);
  void* arg;
  int ret;

  void run();

public:
  //!Constructor, run a user function in a separate thread.
  ThreadFunc(int    (*start_routine)(void*), void* arg);
  ThreadFunc(Event* (*start_routine)(void*), void* arg);

  int retcode();
};



/*!
  \brief A millisecond resolution sleep function

  The libc provides a second and a nanosecond resolution sleep function. This
  one provides a millisecond resolution, and is probably the most useful.
*/
void sleep_ms(unsigned int millisec);



/*!
  \brief Enumerate operational mode for a DateTime object.

  Specify if datetime values being read/written into a DateTime object are
  expressed in UTC or in local time zone.
*/
enum TimeMode {
  UTC,
  LOCAL
};





time_t my_timelocal(struct tm* tm);




/*!
  \brief A class to hold date time information.

  This class can be used to store a datetime value. Data are internally stored
  as the number of seconds elapsed since 1970-01-01 00:00:00 UTC, that is as a
  time_t value.

  This class is also able to perform conversions between different
  data type related to temporal information: time_t, struct tm and
  string representation of a datetime.

  The setTimeMode() method can be used to specify if input values
  should be considered to be expressed in UTC or in local time zone.

  A MYSQL_TIME structure can be linked with this class that will be
  automatically updated each time the DateTime object is updated and
  vice-versa.
*/
class DateTime {

private:
  //!Internal storage (seconds elapsed since 1970-01-01 00:00:00 UTC).
  time_t time;

  //!Operational time mode.
  enum TimeMode timemode;

  //!Pointer to linked MYSQL_TIME structure.
  MYSQL_TIME* mysql;

  /*!
    \brief Parse a string to extract datetime information.

    The parsed values will be written in the struct tm pointed by the second
    parameter. Only the following fields will be used:
    - tm_mday;
    - tm_mon;
    - tm_year;
    - tm_sec;
    - tm_min;
    - tm_hour;
  */
  static const char* parseTime(const char* s, struct tm* tm);


  //!Update linked MYSQL_TIME structure (if any) with internal value.
  void to_MYSQL_TIME();


  /*!
    \brief Return stored time.

    Return the time_t representation of the linked MYSQL_TIME structure. If
    there's no linked MYSQL_TIME structure this function simply returns the
    value of the "time" private field.
  */
  time_t getTime() const;


public:
  //!Constructor.
  DateTime();

  /*!
    \brief Set a pointer to a MYSQL_TIME structure.

    The MYSQL_TIME structure pointed will be automatically updated each time
    the DateTime object is modified. Also any modification in the MYSQL_TIME
    structure will be reflected in the DateTime object's value.
  */
  void setMysqlBuffer(MYSQL_TIME* mysql);

  /*!
    \brief Set operational time mode.

    The time mode controls the behaviour of the DateTime object in the
    following cases:

    - when converting from/to a string representation;
    - when converting from/to a broken down representation (that is a struct
      tm);
    - when converting from/to a MYSQL_TIME structure;

    When the UTC mode is used each value is expressed in UTC, if the LOCAL
    mode is used each value is expressed in local time zone.
  */
  void setTimeMode(enum TimeMode tm);


  //!Set the current datetime value.
  void now();


  //!Assign a time_t value.
  void settval(time_t t);

  //!Wrapper to settval(time_t)
  DateTime& operator=(time_t t)
    { settval(t); return *this; }


  /*!
    \brief Parse a string to extract a datetime value.
   */
  void setsval(string s);

  //!Wrapper to setsval(string)
  DateTime& operator=(string s)
    { setsval(s); return *this; }

  /*!
    \brief Assign a struct tm value.

    Actually the only used fields are:
    - tm_sec (seconds, 0:59);
    - tm_min (minutes, 0:59);
    - tm_hour (hours, 0:23);
    - tm_mday (day of the month, 1:31);
    - tm_mon (month, 0:11);
    - tm_year, (number of years since 1900);
    - tm_isdst (flag indicating whether DST is in effect at the time
    described, 0:1).

    \sa setTimeMode().
   */
  void settmval(struct tm& ltm);

  //!Wrapper to settmval()
  DateTime& operator=(struct tm& ltm)
    { settmval(ltm); return *this; }


  //!Retrieve a time_t value.
  time_t tval() const;

  //!Wrapper to tval().
  operator time_t() const
    { return tval(); }


  //!Retrieve a struct tm object.
  struct tm tmval() const;

  //!Wrapper to tval().
  operator struct tm() const
    { return tmval(); }

  //!Retrieve a string representation of the datetime value.
  string sval() const;

  //!Wrapper to sval().
  operator string() const
    { return sval(); }
};



#define MCS_ID_UNKNOWN  -1
#define MCS_ID_LOCAL    -2
#define MCS_ID_CLIENT   -3


//  Another wide use of the Data class is to to set/retrieve data of
//  prepared statement executions on the database. The Query class has
//  two fundamental methods that returns address of Data objects:
//  param() (to input data on prepared statement) and field() (to
//  retrieve data from an executed query on the database.



//Max number of dimensions in a Data multi-dimensional array. Note
//that this number influences the declaration and implementation of
//Data::array and Data::operator=.
#define MCS_DATA_NDIM   15



//--------------------------------------------------------------------
/*!
  \brief A general purpose data type.

  This class let you create a high-level object that has its own base type,
  but is able to perform conversion to a different data type. Available base
  types are those of the mcs::Types enumeration, namely: integers, floating
  point, string, char*, struct tm*, time_t, memory buffers and pointers. Some
  base types (STRING, TINY_BLOB and BLOB) are variable length objects, that is
  their length varies between zero and a maximum specified in the
  constructor. All other base types are fixed length objects. All integer
  types can be either signed or unsigned. Conversion between different types
  follow standard rules (described below). If the conversion cannot be
  performed an exception is thrown.

  This class is widely used inside MCS code due to its ease of use. Some
  examples follows:

  \code
    Data di(INT);        //Data object with base type INT
    Data ds(STRING, 5);  //Data object with base type STRING (max length 5)

    //Store some values
    di = 1;         //store 1
    di = 2.7;       //store 2, note that the value is truncated
    di = "3";       //store 3
    di = "mcs";     //throws an exception because base type is an integer
    ds = "mcs";     //now its ok because base type is STRING
    ds = "giorgio"; //throws an exception because the string is too long

    //Retrieve values into different data type variables
    struct tm tm;
    di.tval(&tm);

    time_t           tt = di.tval();
    int              ii = di.ival();
    unsigned int     ui = di.uival();
    long long int    il = di.lval();
    unsigned long long int ul = di.ulval();
    float            ff = di.fval();
    string           ss = di.sval();

    char cc[10];
    di.cval(cc, 10);
  \endcode

  The Data class features several constructors to allow writing "easy to read"
  code as well as several methods to perform conversion to/from any allowed
  data types (see all *val, and set*val methods). Also several assignment
  operators wraps the set*val methods. Furthermore a Data object can contain
  either a single value (scalar) or a multi-dimensional array of the same base
  data type values (see resize(), array(), dim()).

  Each Data object has auxiliary informations:

  - a "length" when the base type is variable length (length());
  - a "maximum length" which is the size of internal buffer (maxLength());
  - a "name" (name(), setName());
  - a NULL flag (setNull(), isNull());
  - a tag (getTag(), setTag());

  The Data object can be used to access value of a database table, in this
  case the isAutoIncrement() and isNull() methods provide further information
  about the underlying database field.

  Finally, the Data class extends the Serialize class so it can be serialized
  to be sent through the network or saved on disk.
*/
class Data : public Serializable
{
private:
  MCS_DEBUG_ALLOC;

  void reallocBuffer();

#if ENABLE_MYSQL
  //!Internal reference to MYSQL_BIND structure.
  MYSQL_BIND* lbind;
#endif

  //!Object base type.
  Types ltype;

  //!Object name.
  string lname;

  //!Flags (EXPERIMENTAL).
  unsigned int lflags;

  //!Max number of bytes that can be stored in the buffer (size of the
  //!buffer).
  unsigned short int lmaxlength;

  /*!
    \brief Actual length of real data in the buffer, useful only when
    the base type is a variable length type.
  */
  unsigned long llength;

  //!Base type is unsigned.
  bool lisunsigned;

  //!The object has a null value.
  my_bool lisnull;

  //!If the AUTO_INCREMENT flag is true.
  bool lautoincr;

  //!Tag for user convenience.
  unsigned char tag;

  //!Data buffer.
  char* buf;

  //!Size of data buffer.
  unsigned int bufsize;

  //!Format to handle datetime in sprintf/sscanf calls.
  static const char* dtfmt;

  //!Format to handle date in sprintf/sscanf calls.
  static const char* dafmt;

  //!Format to handle time in sprintf/sscanf calls.
  static const char* tmfmt;

  //!Format to handle integers in sprintf/sscanf calls.
  static const char* ifmt;

  //!Format to handle long integers in sprintf/sscanf calls.
  static const char* lfmt;

  //!Format to handle float in sprintf/sscanf calls.
  static const char* ffmt;

  //!Format to handle double float in sprintf/sscanf calls.
  static const char* dfmt;

  DateTime dt;


  //!Initialize internal structures.
  void init(MYSQL_BIND* bind, Types type, const char* name="",
	    unsigned short int maxLength=0, bool isunsigned=false,
	    unsigned int flags = 0);

  ////!Convert a MYSQL_TIME structure to a C struct_tm.
  //void MySQL_TIME_2_time_t(MYSQL_TIME* mtime, time_t* t);
  //
  ////!Convert a C struct_tm structure to a MYSQL_TIME structure.
  //void time_t_2_MySQL_TIME(time_t* t, MYSQL_TIME* mtime);

  /*!
    \brief Prepare a buffer with all informations contained in the object.

    Once the buffer is prepared you can rebuild an identical object
    (except for the bind information) using the Data(void*)
    cosntructor.

    If "*lbuf" is NULL then the buffer will be allocated, otherwise
    "size" must be set to the length of the buffer pointed by "*lbuf";

    \param lbuf Address of a pointer to a buffer;
    \param size Size of the buffer if it should not be allocated.

    \return Size of the data in the buffer.

    \exception ERROR MSG_NOT_ENOUGH_SPACE.
  */
  bool serialize_buffer(char*& from, unsigned int& size);

  //!The userid of the thread who send this object.
  int id_source;

  //!The userid of the thread that will receive this object.
  int id_dest;

  /*!
    \brief Multi-dimensional array specification.

    Higher 4 bits: varyingDim, Lower 4 bits: howManyDim
   */
  unsigned char ldimspec;

  //!Size of each dimension.
  unsigned short int ldim[MCS_DATA_NDIM];

  //!Multiplicative coefficients to calculate arrpos.
  unsigned short int mult[MCS_DATA_NDIM];

  //!Array size, i.e. how many cells are in the array.
  unsigned int arrsize;

  //!Last selected array cell.
  unsigned int arrpos;

public:
  //!Return how many bytes require the object to be serialized.
  unsigned int objSize();


  //!Build Data object of base type STRING and length zero.
  Data();

  //!Copy constructor. Note that this will not copy any bind information.
  Data(const Data& from);


  /*!
    \brief Constructor with specification of base type.

    With this constructor the object will have the base type specified
    in the parameters but won't have any value stored in the internal
    buffer, and no bind informations.

    \param type Base type for the object;

    \param maxLength Max length of the object if "type" is a variable
    length type;

    \param isunsigned Unsigned flag of the object, default is false
    (so that the data is signed);
   */
  Data(Types type, unsigned short int maxLength = 0,
       bool isunsigned = false, string dimSpec = "");


  /*!
    \brief Constructor used to bind the object with database data.

    \param bind A bind structure address, retrieved from a Query
    object. It can be NULL, if the object is not intended for db use;

    \param type Base type for the object;

    \param name Name of the object, this can be NULL for all practical
    use, it becomes important when the object is created inside a Query
    object because you can specifiy the name of a parameter or a field
    instead of its positional index;

    \param maxLength Size of internal buffer when base type is STRING;

    \param isunsigned Unsigned flag of the object, default is false
    (so that the data is signed);

    \param flags Database flags for the field;

    \param tag Set a new value to internal tag.
  */
  Data(MYSQL_BIND* bind = NULL, Types type = STRING,
       const char* name="", unsigned short int maxLength=0,
       bool isunsigned=false, unsigned int flags = 0,
       unsigned char tag = 0);


  /*!
    \brief Build an object with base type INT.

    \param v Value to be stored in the object;
    \param tag Optional tag for the object.
  */
  Data(int                    v, unsigned char tag = 0);

  /*!
    \brief Build an object with base type BIGINT.

    \param v Value to be stored in the object;
    \param tag Optional tag for the object.
  */
  Data(long long int          v, unsigned char tag = 0);

  /*!
    \brief Build an object with base type DOUBLE.

    \param v Value to be stored in the object;
    \param tag Optional tag for the object.
  */
  Data(double                 v, unsigned char tag = 0);

  /*!
    \brief Build an object with base type STRING.

    The max length is that of the parameter "v".

    \param v Value to be stored in the object;
    \param tag Optional tag for the object.
  */
  Data(string                 v, unsigned char tag = 0);

  /*!
    \brief Build an object with base type TIME.

    \param v Value to be stored in the object;
    \param tag Optional tag for the object.
  */
  Data(struct tm             v, unsigned char tag = 0);

  /*!
    \brief Build an object with base type TIME.

    \param v Value to be stored in the object;
    \param tag Optional tag for the object.
  */
  Data(time_t                 v, unsigned char tag = 0);

  /*!
    \brief Build an object with base type BLOB.

    The max length is given by the parameter "size".  This constructor
    also copies "size" bytes from the source "lbuf" to the internal
    buffer.

    \param lbuf Pointer to the buffer in memory.
    \param size Size of the BLOB.
    \param tag Optional tag for the object.
  */
  Data(void* lbuf, unsigned int size, unsigned char tag = 0);


  /*!
    \brief Build a Data object from a buffer prepared with
    Data.prepareBuffer().

    If the argument is NULL, an empty Data object will be created, with the
    isNull flag to true.
   */
  Data(void* lbuf);

  //!Destructor Frees internal buffer.
  ~Data();

  /*!
    \brief Set the object name to an empty string.

    When sent through the network the object will carry its name, consuming
    bandwidth. With this method you can save some bytes during trasmission.
  */
  void emptyName();


  //!Return the name of the object.
  string name();

  //Set the name of the object.
  void setName(string name);

  ////!Return the flags of the field.
  //unsigned int flags();

  //!Return the base type of the object.
  Types type();

  //!Return the size of the internal buffer.
  unsigned short int maxLength();

  //!Return the actual length of the data in the internal buffer.
  unsigned short int length();

  //!Tell if the object contains unsigned integers.
  bool isUnsigned();

  //!Tells if no value is stored.
  bool isNull();

  //!Tells if the database field is an auto increment field.
  bool isAutoIncrement();

  //!Return a pointer to the internal buffer, use this at your own risk.
  void* buffer() const;

  /*!
    \brief Convert internal data to a integer value.

    \exception ERROR MSG_CONVERSION_STRING_INT;
    \exception ERROR MSG_CONVERSION_BLOB_INT.
  */
  int ival() const;

  /*!
    \brief Convert internal data to an unsigned integer value.

    \exception ERROR MSG_CONVERSION_STRING_INT;
    \exception ERROR MSG_CONVERSION_BLOB_INT.
  */
  unsigned int uival() const;

  /*!
    \brief Convert internal data to a long long integer value.

    \exception ERROR MSG_CONVERSION_STRING_INT;
    \exception ERROR MSG_CONVERSION_BLOB_INT.
  */
  long long int lval() const;

  /*!
    \brief Convert internal data to an unsigned long long integer value.

    \exception ERROR MSG_CONVERSION_STRING_INT;
    \exception ERROR MSG_CONVERSION_BLOB_INT.
  */
  unsigned long long int ulval() const;

  /*!
    \brief Convert internal data to a floating point value.

    \exception ERROR MSG_CONVERSION_STRING_FLOAT;
    \exception ERROR MSG_CONVERSION_BLOB_FLOAT.
  */
  float fval() const;

  /*!
    \brief Convert internal data to a double (8 bytes) floating point value.

    \exception ERROR MSG_CONVERSION_STRING_FLOAT;
    \exception ERROR MSG_CONVERSION_BLOB_FLOAT.
  */
  double dval() const;

  /*!
    \brief Convert internal data to a string object.

    \param addWhiteSpaces If true a number of spaces will be added to
    pad to maxLength bytes.
  */
  string sval(bool addWhiteSpaces = false) const;

  /*!
    \brief Convert internal data to a NULL terminated string and store it in a
    buffer.

    \param c Address of the buffer.
    \param maxlength Max number of bytes that can be copied in the
    buffer.

    \return Number of bytes copied in the buffer, without accounting
    for the NULL character.
   */
  int cval(char* c, int maxlength) const;

  /*!
    \brief Convert internal data to a time_t value.
  */
  time_t tval() const;

  /*!
    \brief Convert internal data to a struct tm and store it in a buffer.

    \param t Address of the buffer.

    \exception ERROR MSG_CONVERSION_BLOB_DATETIME
  */
  void tval(struct tm* t) const;


  void* pval() const;

  void setpval(void* p);


  //!Set null flag. Following call to isNull() returns the value used here as parameter.
  void setNull(bool null = true);

  //!Convert int value to base type and store in internal buffer.
  void setival(int v);

  //!Convert unsigned int value to base type and store in internal buffer.
  void setuival(unsigned int v);

  //!Convert long int value to base type and store in internal buffer.
  void setlval(long long int v);

  //!Convert unsigned long int value to base type and store in internal buffer.
  void setulval(unsigned long long int v);

  //!Convert double value to base type and store in internal buffer.
  void setdval(double v);

  //!Convert string value to base type and store in internal buffer.
  void setcval(const char* v);

  //!Convert string value to base type and store in internal buffer.
  void setsval(string v);

  //!Copy "size" bytes from the address given in "lbuf" parameter.
  void setblob(void* lbuf, unsigned int size);

  //!Convert "struct tm" value to base type and store in internal buffer.
  void settimeval(struct tm v);

  //!Convert time_t value to base type and store in internal buffer.
  void settimeval(time_t v);

  //!Convert current time value to base type and store in internal buffer.
  void settimenow();

  void setTimeMode(enum TimeMode tm);

  //Wrapper to setsval(int).
  //void setval(string v);

  //Wrapper to setival(int).
  //void setval(int v);

  //Wrapper to setdval(int).
  //void setval(double v);

  //Wrapper to setcval(int).
  //void setval(const char* v);

  /*!
    \brief Create or resize a multi-dimensional array.

    The array can have as much as MCS_DATA_NDIM dimensions (actually
    15), and each dimension can have as much as 65535 positions.

    When you call this method you must specify:
    - the number of dimensions for the array;
    - the length of each dimensions;
    - optionally if the last dimension is variable in length.

    All this informations are given in a string formatted using the
    VOTable specifications
    (http://www.ivoa.net/Documents/latest/VOT.html) for the
    "arraysize" attribute of a "FIELD" node.

    Some examples are:
    - "10", one dimension of length 10;
    - "10x3", two dimensions of length 10 and 3 respectively;
    - "10x3x5*", three dimensions of of length 10, 3 and 5
    respectively, but the last one is variable in length.

    \note If you try to call this method on a Data object bind to a
    database field (that is a Data object returned by a Query object)
    an exception will be thrown.
   */
  void resize(string dimSpec);

  /*!
    \brief Resize variable length dimension.

    If a dimension is allowed to vary in length (see resize()) you can
    change its length with this method. If you increase the size of
    the dimension then all values already present in the array will be
    preserved and new values will be set to zeroes.

    If no dimension is allowed to vary then an exception will be
    thrown.
   */
  void resizeVaryingDim(unsigned short int newsize);

  /*!
    \brief Return length of a dimension

    The "d" parameter must be in the range 1..MCS_DATA_NDIM.
  */
  unsigned short int dim(int d);


  //How many dimensions, 0 means scalar value

  /*!
    \brief Return how many dimensions are in the array.

    0 means a scalar value, 1 means a one dimensional array (simple
    array), etc...
  */
  unsigned int howManyDim();

  /*!
    \brief Return the index of the dimension that may vary.

    The last dimension of the multi-dimensional array may vary in
    length (see resizeVaryingDim()) if it is allowed to do so. This
    method returns the index of the dimension which is allowed to
    vary in the 1..MCS_DATA_NDIM range, or 0 if no dimension can
    change.
  */
  unsigned int varyingDim();

  /*!
    \brief Return size of array.

    That is how many cells are in the array.
   */
  unsigned int arraySize();


  /*!
    \brief Select a cell from the array.

    Indexes are all 0-based, so each index must be in the range
    0..dim(d) where d is the dimension we are considering.
  */
  unsigned int array(unsigned short int i1 = 0,
		     unsigned short int i2 = 0,
		     unsigned short int i3 = 0,
		     unsigned short int i4 = 0,
		     unsigned short int i5 = 0,
		     unsigned short int i6 = 0,
		     unsigned short int i7 = 0,
		     unsigned short int i8 = 0,
		     unsigned short int i9 = 0,
		     unsigned short int i10 = 0,
		     unsigned short int i11 = 0,
		     unsigned short int i12 = 0,
		     unsigned short int i13 = 0,
		     unsigned short int i14 = 0,
		     unsigned short int i15 = 0);

  /*!
    \brief Same as array().
  */
  Data& operator()(const int i1 = 0,
		   const int i2 = 0,
		   const int i3 = 0,
		   const int i4 = 0,
		   const int i5 = 0,
		   const int i6 = 0,
		   const int i7 = 0,
		   const int i8 = 0,
		   const int i9 = 0,
		   const int i10 = 0,
		   const int i11 = 0,
		   const int i12 = 0,
		   const int i13 = 0,
		   const int i14 = 0,
		   const int i15 = 0);

  //!Wrapper assignment operator to setival(int).
  Data& operator=(const int v)
    { setival(v); return *this; }

  //!Wrapper assignment operator to setuival(unsigned int).
  Data& operator=(const unsigned int v)
    { setuival(v); return *this; }

  //!Wrapper assignment operator to setlval(long long int).
  Data& operator=(const long long int v)
    { setlval(v); return *this; }

  //!Wrapper assignment operator to setulval(unsigned long long int).
  Data& operator=(const unsigned long long int v)
    { setulval(v); return *this; }

  //!Wrapper assignment operator to setdval(double).
  Data& operator=(const double v)
    { setdval(v); return *this; }

  //!Wrapper assignment operator to setsval(string)
  Data& operator=(const char*  v)
    { setsval(v); return *this; }

  //!Wrapper assignment operator to setsval(string).
  Data& operator=(const string v)
    { setsval(v); return *this; }

  //!Wrapper assignment operator to settimeval(struct tm)
  Data& operator=(const struct tm v)
    { settimeval(v); return *this; }

  //!Wrapper assignment operator to settimeval(time_t)
  Data& operator=(const time_t v)
    { settimeval(v); return *this; }

  //!Wrapper cast operator to ival()
  operator const int                    () const { return ival     (); }

  //!Wrapper cast operator to uival()
  operator const unsigned int           () const { return uival    (); }

  //!Wrapper cast operator to lval()
  operator const long long int          () const { return lval     (); }

  //!Wrapper cast operator to ulval()
  operator const unsigned long long int () const { return ulval    (); }

  //!Wrapper cast operator to dval()
  operator const float                  () const { return dval     (); }

  //!Wrapper cast operator to dval()
  operator const double                 () const { return dval     (); }

  //!Wrapper cast operator to pval()
  operator const char*                  () const { return (char*) pval(); }

  //!Wrapper cast operator to sval()
  operator const string                 () const { return sval     (); }

  //!Wrapper cast operator to tval(struct tm)
  operator const struct tm              () const { struct tm v; tval(&v); return v; }

  //!Wrapper cast operator to tval()
  operator const time_t                 () const { return tval     (); }


  /*!
    \brief To assign the value of a Data object to another Data object.

    This operator is used to copy the value of a Data object to
    another Data object. Note that, as in any other use of the
    assignment operator, only the value is copied, not all the other
    settings of a Data object. This operator will be called each time
    you assign a Data object to another Data object. In the particular
    cas in which you are creating a Data object with an assignment the
    copy constructor will be called instead of this.
  */
  Data& operator=(Data& d);

  //!Fills the "struct tm*" passed as argument with the current local datetime.
  //static void default_struct_tm(struct tm* t);

  /*!
    \brief Parse string to extract date time information.

    The date time format must be one of:
      YYYY-MM-DD HH:MM:SS
      YYYY-MM-DD
      HH:MM:SS
    otherwise an exception will be thrown.

    \param s String to be parsed.
    \param ts Address of a struct tm that will be filled with parsed
    information.

    \exception ERROR MSG_CONVERSION_DATETIME.
  */
  static void parseTime(string s, struct tm* ts);

  /*!
    \brief Return the minimum integer value for the base type
    specified.

    \param ltype Base type for which compute the min value.
    \param flunsigned If the value should be considered unsigned.

    \return The min value for a Data object of base type ltype and
    unsignedness equal to flunsigned.

    If flunsigned is false this function always returns zero.

    \sa enum Types.
   */
  static long long int MinValue(Types ltype, bool flunsigned);

  /*!
    \brief Return the maximum integer value for the base type
    specified.

    \param ltype Base type for which compute the max value.
    \param flunsigned If the value should be considered unsigned.

    \return The max value for a Data object of base type ltype and
    unsignedness equal to flunsigned.

    \sa enum Types.
   */
  static long long int MaxValue(Types ltype, bool flunsigned);

  //!Returns a string representation of the object, for debug purpose.
  string print();

  //!Set a new value to internal tag.
  void setTag(unsigned char tag);

  //!Retrieve the value of the tag.
  unsigned char getTag();

  int getSourceID();
  int getDestID();
  void setSourceID(int id);
  void setDestID(int id);
};



//!Size of allocation block for the Dynamic_Array class.
#define DYNAMIC_ARRAY_DEFAULT_STEP 10



/*!
  \brief A thread safe, template class to handle an array of objects.

  This class is thread safe in the sense that it can be synchronized
  betweem different thread, using the parent class Synchro. This class
  can be used as an array (using the bracket operator []) or like a
  FIFO structure (using the push(), pop(), peek() methods).
*/
template<class BASE>
class Dynamic_Array : public Synchro {
private:
  //!Array of pointer to template objects.
  BASE** arr;

  //!How many blocks are allocated.
  int step_alloc;

  //!How many elements are in the array.
  int lcount;

  /*!
    \brief Alloc necessary memory to store data in the array.

    Check that the current allocated memeory is big enough to contain
    as many pointer to template objects as passed in parameter. If the
    memory is not enough it realloc the memory to make it bigger.
  */
  void check_allocation(int count);

protected:
  Dynamic_Array<BASE>& array;

public:
  /*!
    \brief Constructor with optional synchronization.

    \param synchro If true the array will use synchronization.
  */
  Dynamic_Array(bool synchro);

  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you
    try to use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(Dynamic_Array);


  //!Assignment operator.
  Dynamic_Array& operator=(Dynamic_Array& from);


  //!Destructor, this also destroy all objects in the array.
  ~Dynamic_Array();

  /*!
    \brief Returns number of elements in the array.

    \return Number of elements in the array.
  */
  int count();

  /*!
    \brief Tells if there are objects in the array.

    Note that in a multithreaded environment it is not sure that if this
    method returns true a call to pop() or the operator[] methods will
    really return an object. That's because in the meanwhile another
    thread can modify the array content.

    \return true if there are objects in the queue, false otherwise.
  */
  bool ready();

  /*!
    \brief Push an existing element at the end of the array.

    This method stores the address passed as parameter in its internal
    structure.

    \param d Address of the object to push in the array.

    \warning The object whose address is passed as parameter will be
    automatically deleted from the destructor of Dynamic_Array. So
    this object must have been allocated in the heap and you must not
    destroy it, otherwise errors like "double free or corruption" or
    segmentation fault can occur. A typical misusing of this method is
    like the following:

    \code
    Data* aa = new Data(1);

    Dynamic_Array<Data> vec1;
    Dynamic_Array<Data> vec2;

    vec1.push(aa);
    vec2.push(aa);
    \endcode

    In this case two Dynamic_Arrays have the same object stored in
    their internal structure, so when these arrays will be destroyed
    the object will be destroyed twice.
  */
  void push(BASE* d);

  /*!
    \brief Push a copy of an existing element at the end of the array.

    Passing a reference you are really storing in the array a copy of
    your object, so the warning given in push(BASE*) doesn't apply
    here.

    \param d Reference to an element object.
  */
  void push(BASE& d);

  /*!
    \brief Extracts an element from the array.

    The object is also removed from the array, if you want to maintain
    this object you should use the operator[] or the peek() method. If
    the array is empty an exception will be thrown.

    \param x Positional index of the element to be retrieved (0 <= x <
    count()).

    \return The element object extracted from the array.
  */
  BASE pop(int pos = 0);

  /*!
    \brief The operator[] for the array.

    With this operator you can use the object it was a C-style
    array. If you require an index out of the range [0..count()] an
    exception will be thrown.

    Note that this method returns a reference to the object, so any
    modification you made will be stored in the array. Note also that
    this is not a thread-safe method (the Synchro beyond this object
    is not locked during the execution of the method). So in a
    multi-threaded program you should use this method as follows:

    \code
    Dynamic_Array q;

    ...

    q.enter();
    q[0] = 1.2,
    q.leave();
    \endcode

    \param x Positional index of the element to be retrieved (0 <= x <
    count()).

    \return A reference to the object.

  */
  BASE& operator[](int pos);

  /*!
    \brief Retrieve an element from the array but don't extract it.

    This returns a copy of an element from the array. The object is not removed
    from the array, if you want to remove this object you should use
    the pop() method. If the queue is empty an exception will be
    thrown.

    \param x Positional index of the element to be retrieved (0 <= x <
    count()).

    \return A copy of the object extracted from the queue.
  */
  BASE peek(int pos);

  /*!
    \brief Empty the array.

    This destroy all objects in the array.
  */
  void clear();
};


//Methods implementation of template classes must be in the header file
template<class BASE>
void mcs::Dynamic_Array<BASE>::check_allocation(int count) {
  unsigned int size;
  static int max_size = Data::MaxValue(SMALL, true);

  if (count >= max_size)
    throw MCS_ERROR(MSG_INDEX_OUT_RANGE, count, max_size);

  if (count > (step_alloc * DYNAMIC_ARRAY_DEFAULT_STEP)) {
    step_alloc++;
    size = sizeof(BASE*) * step_alloc * DYNAMIC_ARRAY_DEFAULT_STEP;

    if (step_alloc == 1) //First allocation
      arr = (BASE**) malloc(size);
    else
      arr = (BASE**) realloc(arr, size);
  }
}


template<class BASE>
mcs::Dynamic_Array<BASE>::Dynamic_Array(bool synchro) :
  Synchro(), array(*this)
{
  synchronize(synchro);
  lcount = 0;
  step_alloc = 0;
  arr = NULL;
}


template<class BASE>
mcs::Dynamic_Array<BASE>::~Dynamic_Array() {
  clear();
}



template<class BASE>
mcs::Dynamic_Array<BASE>&
mcs::Dynamic_Array<BASE>::operator=(Dynamic_Array<BASE>& from) {
  int i;

  clear();
  for (i=0; i<from.count(); i++)
    push(from[i]);

  return *this;
}


template<class BASE>
void mcs::Dynamic_Array<BASE>::push(BASE* d) {
  MCS_CRITICAL_SECTION_BEGIN;

  check_allocation(lcount + 1);
  arr[lcount++] = d;

  MCS_CRITICAL_SECTION_END;
}


template<class BASE>
void mcs::Dynamic_Array<BASE>::push(BASE& d) {
  MCS_CRITICAL_SECTION_BEGIN;

  check_allocation(lcount + 1);
  arr[lcount++] = new BASE(d);

  MCS_CRITICAL_SECTION_END;
}



//#define EVENT_IS_THROWN(INSTR...)
//try { INSTR;}  catch(Event e) { leave(); throw; }
//{ bool _EVENT_IS_THROWN



template<class BASE>
BASE mcs::Dynamic_Array<BASE>::pop(int pos) {
  int i;

  MCS_CRITICAL_SECTION_BEGIN;

  BASE d = BASE(operator[](pos));

  if (pos < lcount) {
    delete arr[pos];

    lcount--;
    for (i=pos; i<lcount; i++)
      arr[i] = arr[i+1];
  }

  MCS_CRITICAL_SECTION_END_RETURN(d);
}


template<class BASE>
BASE& mcs::Dynamic_Array<BASE>::operator[](int pos) {
  if (pos < 0)
    pos += lcount;

  if (pos < 0)
    throw MCS_ERROR(MSG_INVALID_POSITION, pos);

  if (pos >= lcount)
    throw MCS_ERROR(MSG_INDEX_OUT_RANGE, pos, lcount);

  return *(arr[pos]);
}


template<class BASE>
BASE mcs::Dynamic_Array<BASE>::peek(int pos)
{
  MCS_CRITICAL_SECTION_BEGIN;

  BASE d = operator[](pos);

  MCS_CRITICAL_SECTION_END_RETURN(d);
}


template<class BASE>
int mcs::Dynamic_Array<BASE>::count() {
  return lcount;
}


template<class BASE>
bool mcs::Dynamic_Array<BASE>::ready() {
  return (bool) (lcount > 0);
}


template<class BASE>
void mcs::Dynamic_Array<BASE>::clear() {
  int i;

  MCS_CRITICAL_SECTION_BEGIN;

  for (i=0; i<lcount; i++)
    delete arr[i];

  if (arr) {
    free(arr);
      arr = NULL;
  }

  lcount = 0;
  step_alloc = 0;

  MCS_CRITICAL_SECTION_END;
}




////!To be used with Record.move(), move the "from" object to the end.
//#define MCS_VECTOR_MOVE_TO_END -1

/*!
  \brief A dynamic array of Data objects.

  This class handles an array of Data objects, that are the most
  common way of passing information in MCS, due to their interface
  able to convert information between different types. This class is
  thread safe in the sense that it can be used from different threads
  without worrying about synchronization. You only have to take care
  when you use the operator[].

  This class is simply a wrapper around a Dynamic_Array<Data> object,
  it is mainly used because object of this class can be serialized
  (with the prepareBuffer() and one of the constructor), and because
  it can use a name map.
*/
class Record : public Serializable, public Synchro
{
private:
  MCS_DEBUG_ALLOC;

  //!Array of fields
  Dynamic_Array<Data> array;

  //!Array of mapped fields.
  Dynamic_Array<int> lmap;

  //!String representation of the map.
  string smap;

  /*!
    \brief Prepare a buffer with all informations contained in the object.

    Once the buffer is prepared you can rebuild an identical object
    using the Record(void*) cosntructor.

    If "*lbuf" is NULL then the buffer will be allocated, otherwise
    "size" must be set to the length of the buffer pointed by "*lbuf";

    \param lbuf Address of a pointer to a buffer;
    \param size Size of the buffer if it should not be allocated.

    \return Size of the data in the buffer.

    \exception ERROR MSG_NOT_ENOUGH_SPACE.

    \return Size of the buffer.
  */
  bool serialize_buffer(char*& from, unsigned int& size);



public:
  //!Copy constructor, will copy all data objects.
  Record(Record& from);

  //!Assignment operator, will copy all data objects.
  Record& operator=(Record& from);

  /*!
    \brief Set all Data object's name to an empty string.

    When sent through the network the object will carry its name, consuming
    bandwidth. With this method you can save some bytes during trasmission.
  */
  void emptyName();

  /*!
    \brief Set all Data object's name to null values.
  */
  void setNull();


  //!Constructor, build an empty record. The parameter is passed to the underlying Dynamic_Array object.
  Record(bool synchro = false);


  /*!
    \brief Build a Record object from a buffer prepared with
    Record.prepareBuffer().

    The object won't be synchronized.

    \param lbuf Pointer to the buffer in memory.
  */
  Record(void* lbuf, bool synchro = false);


  /*!
    \brief Destructor.

    It calls clean() to destroy all objects.

    \sa clean()
  */
  ~Record();

  //!Wrapper around Dynamic_Array.count.
  int count();

  //!Wrapper around Dynamic_Array.push.
  void addField(Data* d);

  //!Wrapper around Dynamic_Array.push.
  void addField(Data& d);

  /*!
    \brief Push a new Data object with base type STRING in the array.

    The new element contains the data passed as the "s" parameter.

    \param s The data that will be contained in the new object.
    \param tag Set a new value to internal tag.
  */
  void addField(string s, char tag = 0);

  /*!
    \brief Push a new Data object with base type INT in the array.

    The new element contains the data passed as the "i" parameter.

    \param i The data that will be contained in the new object.
    \param tag Set a new value to internal tag.
  */
  void addField(int i, char tag = 0);

  /*!
    \brief Push a new Data object with base type BIGINT in the array.

    The new element contains the data passed as the "i" parameter.

    \param i The data that will be contained in the new object.
    \param tag Set a new value to internal tag.
  */
  void addField(long long int i, char tag = 0);

  /*!
    \brief Push a new Data object with base type DOUBLE in the array.

    The new element contains the data passed as the "f" parameter.

    \param f The data that will be contained in the new object;
    \param tag Set a new value to internal tag.
  */
  void addField(double f, char tag = 0);

  Data pop(int x = 0);

  /*!
    \brief Return the index of the first Data objects whose name is
    "name".

    This method work correctly only if all Data objects in the array
    has a name.

    \param name The field name to search for.

    \param throwexc If DONT_THROW will return -1 instead of throwing an
    exception.

    \return Index of the field if found, or -1 if not found.

    \exception ERROR MSG_MISSING_FIELD.
  */
  int posWhoseNameIs(string name , enum ThrowExceptions throwexc = THROW);


  /*!
    \brief Creates a new field map from the given string.

    It often happens that user want to access a record with field names rather
    than indexes. This can be done with the fieldName() method. On the other
    hand using this method in a iteration can really slow down the execution
    of a program. With this method you can remap the list of indexes using
    field names, then access the fields with the integer provided by the
    map. You create the map giving a string containing, in the order you wish,
    the names of the fields. You don't need to citate all the fields in the
    list, just the ones you want to remap. Fields name must be separated by
    spaces.

    \param s String containing the map.

    An example follows:
    \code

    //Prepare and execute a query...
    Query qry;
    qry.prepare(...);
    qry.execute();

    //Suppose the query returned a result-set with the field named
    //"Field1", "Field2" and "Field3" in this order. Now we want to
    //reverse the order:
    qry.newFieldMap("Field3 Field2 Field1");

    //Direct access to the fields....
    qry.field(0)  //Field1
    qry.field(1)  //Field2
    qry.field(2)  //Field3

    //Access through the map....
    qry.mfield(0)  //Field3
    qry.mfield(1)  //Field2
    qry.mfield(2)  //Field1

    \endcode

    To restore the original order simply call this method without arguments.
  */
  void setFieldMap(string s = "");

  //!Copy the field map from one record to another.
  void setFieldMap(Record& rec);

  void clear();


  /*!
    Note that this returns a reference to the object, so that you can
    use the Data assignment operator like this:

    \code
    Record q;

    q.addField();
    q[0] = "a string";
    q[0] = 12;
    ...
    \endcode
   */
  Data& operator[](string name);

  Data& operator[](int pos);

  //!Returns the Data object in the vector whose name is "name".
  Data field(string name);

  Data field(int pos);


  //!Return how many bytes require the object to be serialized.
  unsigned int objSize();

  //!Return record content as a unique string.
  string asString(string sep = "\t");
  string asStringNames(string sep = "\t");
  string asStringTypes(string sep = "\t");


  ///*!
  //  \brief Move element in the Record from one position to another.
  //
  //  Both indexes must be allowed ones, and "from" must be less than
  //  "to". All elements after "from" will be shifted one position
  //  towards the exit of the queue. This is a thread safe method.
  //
  //  \param from Source position;
  //  \param to Destination position.
  //
  //  \exception ERROR MSG_INDEX_OUT_RANGE.
  // */
  //void move(int from, int to);
};











/*!
  \brief Flag for RecordSet::init().

  If this flag is set then all records will be fetched when the
  startFetch() method is called.

  Setting this flag implies that the MCS_RS_RANDOM and
  MCS_RS_KNOW_NROWS flags are also set.
 */
#define MCS_RS_ACCUM        1


/*!
  \brief Flag for RecordSet::init().

  If this flag is set then all records in the set will have the same
  structure based on the metarec object. When a call to the overrided
  fetch() method is performed the internal Record object (accessible
  throught the rec() method) will already contain an empty structure
  which is a copy of the metarec object.
*/
#define MCS_RS_USEMETAREC   2

/*!
  \brief Flag for RecordSet::init().

  If this flag is set then the number of records in the set is known
  and can be retrieved through the nRows() method.
 */
#define MCS_RS_KNOW_NROWS   4

/*!
  \brief Flag for RecordSet::init().

  If this flag is set then the object provide a random access to the
  set, that is the methods setFirst, setLast, setPos, etc... can be
  used to locte a particular record.
*/
#define MCS_RS_RANDOM       8


/*!
  \brief Flag for RecordSet::init().

  If this flag is set then the RecordSet object will work in "insert"
  mode, that is all records will be inserted in the set using the
  insert() method.

  Setting this flag implies that the MCS_RS_ACCUM, MCS_RS_RANDOM and
  MCS_RS_KNOW_NROWS flags are also set.

 */
#define MCS_RS_INSERT      16




/*!
  \brief The base class that implement the data abstraction layer.

  This class provide the basic framework of the data abstraction
  layer, in particular it offer to the user a uniform interface to
  access the Record objects read from the source. Each class used as a
  data source through the data abstraction layer must derive this
  class.

  This class basically represent a collection (or set) of
  records. When the set is not empty there is always a "current"
  record, available through the rec() method. To change the current
  record you can use the setNext() method to move to the next
  record. Depending on the set there are also other method to browse
  the set (setFirst(), setPrev(), setLast(), setWhere()).

  This class can work in two "modes":

  - insert mode: in this case the class can be used "as is", without
    being derived. Records can be inserted into the set using the
    insert() method.

  - fetch mode: in this case the class must be derived and the fetch()
    virtual method must be overrided.
 */
class RecordSet {
private:
  //!General purpose identificator.
  short int lid;

  //!Flags used in the init() method
  unsigned char code;

  //!If the number of records is known.
  bool lknow_nrows;

  //!Number of records, to be used only if lknow_nrows is true.
  unsigned int lnrows;

  //!If all records has been fetched during the init() call.
  bool laccum;

  //!If random access (setFirst, setLast, setPos) is allowed.
  bool lrandom;

  //!If all records share the same meta structure.
  bool lusemetarec;

  //!If true the fetch mechanism will be used.
  bool lfetch;

  //!If true the insert mechanism will be used.
  bool linsert;

  //!Current position in the recordset.
  unsigned int lpos;

  //!Internal position in the Dynamic_Array object.
  unsigned int current;

  //!Record of meta structure, to be used only if lusemetarec is true.
  Record lmetarec;

  //!If the current position (lpos) is at the end of the set.
  bool leof;

  //!Internal Record set.
  Dynamic_Array<Record> rs;

  /*!
    \brief Creates a new Record object in the heap.

    If lusemetarec is true then the record will be created with the
    same meta structure as the lmetarec object.

    Users can override this method to provide a customized build
    Record object.

    This method is automatically called by internal_fetch() when a new
    record is required.;
   */
  virtual Record* newRecord();

  //Clear the entire Record set and the lmetarec object.
  void clear();

  /*!
    \brief Fetch a new Record object in the set.

    If laccum is true a new Record object will be created for each
    effective record, otherwise a single Record object will be used
    (rs[0]). Users should not override this method because it is a
    very important one, instead override fetch().
   */
  bool internal_fetch(unsigned int newpos);


protected:
  /*!
    \brief Initailize the Record set.

    This metod must be called from a derived class to provide
    information about the Record set to the parent one.

    The first argument is a combination of one or more of the
    following flags:

    - MCS_RS_ACCUM
    - MCS_RS_USEMETAREC
    - MCS_RS_KNOW_NROWS
    - MCS_RS_RANDOM
    - MCS_RS_INSERT

    If you want to use the meta informations you should provide them
    before calling this method, through the metarec() method.

    \param accum If all records must be fetched during this call;
    \param usemetarec If the meta informations has been provided;
    \param know_nrows If the number of records is known;
    \param random If the random access to the set is allowed;
    \param nrows The number of rows, if known.
   */
  void init(unsigned char code, unsigned int nrows = 0, Record* meta = NULL,
	    short int id = 0);

  //!Must be called when record fetching is possible.
  void startFetch();

  /*!
    \brief The virtual method to fetch new records.

    This method is automatically called from internal_fetch() to fetch
    new records. It must be overrided in a derived class, otherwise an
    exception will be thrown. If the "accum" parameter in the init()
    call was true, then all records will be fetched during the init()
    call, otherwise they'll be fetched when they are needed.

    The record to be fetched is the one in the position (of the set)
    indicated by the "newpos" parameter. Anyway not all Record set can
    provide random access, so if you pass "random = false" in the
    init() call you are guaranteed that "newpos" will be incremented
    by one in each call, starting from zero. If the random access is
    allowed you will know if the record to be fetched is really the
    next in the set from the second parameter. When this is false the
    next record in the set is to be fetched, when this is true the
    position requested is random, that is not predictable.

    \param newpos The position of the record to be fetched;
    \param random If true a random access is requested.

    \return False if there are no more records to be fetched, true
    otherwise.
   */
  virtual bool fetch(unsigned int newpos, bool random);

  //!Virtual method to provide dump facility to a file.
  virtual void hk_dump(string fn);


public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(RecordSet);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(RecordSet);

  /*!
    \brief Build an empty record set.

    Before fetching records you should call init(), passing all
    necessary parameters.
  */
  RecordSet();

  //!Destructor.
  virtual ~RecordSet();



  /*!
    \brief Set up a field map for each record in the recordset.

    This method can be used only on those recordsets which have a
    "meta" record, that is MCS_RS_USEMETAREC has been used while
    calling init(). With this method you set a field map in the "meta"
    recordset which will be automatically replicated in all records
    belonging to the set.
  */
  void setFieldMap(string s = "");


  /*!
    \brief Insert a new record in the set.

    The insert() method provide an alternative method to fetch() to
    insert records in the set. With this method you can insert Record
    objects allocated in the heap. These objects will not be copied and
    will be destroyed by the RecordSet object.

    \param rec Address to a Record object.
  */
  void insert(Record* rec);

  /*!
    \brief Copy a new record in the set.

    The insert() method provide an alternative method to fetch() to
    insert records in the set. With this method you can insert a copy
    of a Record object. These objects will be copied in the set as a
    new record.

    \param rec Reference to a Record object.
  */
  void insert(Record& rec);


  /*!
    \brief Set the record pointer to the first position.

    \return False if there are no more records to be fetched, true
    otherwise (! eof).

    \exception ERROR MSG_NOT_A_RANDOM_ACCESS_RECORDSET.
  */
  bool setFirst();

  /*!
    \brief Set the record pointer to the last position.

    \return False if there are no more records to be fetched, true
    otherwise (! eof).

    \exception ERROR MSG_NOT_A_RANDOM_ACCESS_RECORDSET;
    \exception ERROR MSG_DONT_KNOW_NROWS.
  */
  bool setLast();

  /*!
    \brief Set the record pointer to the next position.

    \return False if there are no more records to be fetched, true
    otherwise (! eof).

    \exception ERROR MSG_ALREADY_AT_END_OF_FILE;
  */
  bool setNext();

  /*!
    \brief Set the record pointer to the previous position in the set.

    \return Always true.

    \exception ERROR MSG_NOT_A_RANDOM_ACCESS_RECORDSET;
    \exception ERROR MSG_DONT_KNOW_NROWS.
  */
  bool setPrev();

  /*!
    \brief Set the record pointer to the position which satisfies a
    string condition.

    The condition is that the value on i-th field must be equal to the
    "equalTo" string. The index "i" must be in the allowed range (0<=
    i < nFields()).

    \param i Index of field on which perform the check;
    \param equalTo Value to compare with the content of the field.

    \return True if the record was found, false otherwise, in this
    latter case the record pointer remain unchanged.

    \exception ERROR MSG_NO_SAME_META_INFO;
    \exception ERROR MSG_INDEX_OUT_RANGE.
  */
  bool setWhere(int i, string equalTo);

  /*!
    \brief Set the record pointer to the position which satisfies an
    integer condition.

    The condition is that the value on i-th field must be equal to the
    "equalTo" integer. The index "i" must be in the allowed range (0<=
    i < nFields()).

    \param i Index of field on which perform the check;
    \param equalTo Value to compare with the content of the field.

    \return True if the record was found, false otherwise, in this
    latter case the record pointer remain unchanged.

    \exception ERROR MSG_NO_SAME _META_INFO;
    \exception ERROR MSG_INDEX_OUT_RANGE.
  */
  bool setWhere(int field, int equalTo);

  /*!
    \brief Set the record pointer to the i-th position.

    Index must be in the allowed range (0 <= i < nRows()). Throw a
    Event exception if i is beyond eof() or if the record set doen'st
    allow random access.

    \param i New position.

    \return False if there are no more records to be fetched, true
    otherwise (! eof).

    \exception ERROR MSG_NOT_A_RANDOM_ACCESS_RECORDSET;
    \exception ERROR MSG_DONT_KNOW_NROWS;
    \exception ERROR MSG_INDEX_OUT_RANGE.
  */
  bool setPos(unsigned int i);

  void dump(string fn);

  bool know_nRows();
  unsigned int nRows();

  //!Returns number of fields.
  int nFields();

  unsigned int pos();
  bool eof();
  bool alwaysSameStructure();

  Record& rec();
  Record& metarec();

  Record* prepRecToSend();
};





//--------------------------------------------------------------------
/*!
  \brief Read and write configuration files.

  This class can be used to read and write configuration files like
  those known as INI files in the Windows world.

  Supported files can have sections (using the "[]" characters) and
  comments (from the "#" character to the new line).

  Once the file have been parsed (by the constructor) values can be
  retrieved as STRING Data objects, as well as string or integers.
 */
class Conf
{
private:
  //!Path to the configuration file.
  string filename;

  //!Internal vector containing section names.
  vector<string> sections;

  //!Internal vector containing keys names.
  vector<string> keys;

  //!Internal vector containing values names.
  vector<string> values;

  //!Internal vector containing comments, this is used in the save() method.
  vector<string> comments;

  /*!
    \brief Search the index at which section and key are found.

    \param section Section to search for;
    \param key Key to search for.

    \return Index of the section/key pair, or -1 if not found.
   */
  int index(string section, string key);

  Data lastval;

public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(Conf);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(Conf);

  /*!
    \brief Constructor, parse a configuration file.

    \param filename Path to the configuration file.
   */
  Conf(string filename = "");

  //!Destructor.
  ~Conf();

  void open(string filename);

  /*!
    \brief Search for the specified section/key value.

    \param section Section to search for;
    \param key Key to search for.
    \param throwexc If THROW will throw an exception instead of returning
    an empty string.

    \return True if the value has been found, false otherwise.

    \exception ERROR MSG_ENTRY_UNAVAILABLE.
  */
  bool search(string section, string key, enum ThrowExceptions throwexc = DONT_THROW);

  //!Return a Data object containing the last value found.
  /*!
    \brief Search for the specified section/key value and return it in
    a Data object.

    If the value is not found an exception will be thrown. If both
    parameters are empty string the retunred Data object contain the
    last requested value.

    \param section Section to search for;
    \param key Key to search for.

    \return The value requested inside a Data object if found.

    \exception ERROR MSG_ENTRY_UNAVAILABLE.
   */
  Data& val(string section = "", string key = "");

  /*!
    \brief Search for the specified section/key value and return it as
    a string.

    If the value is not found an exception will be thrown.

    \param section Section to search for;
    \param key Key to search for.

    \return The value requested as a string if found.

    \exception ERROR MSG_ENTRY_UNAVAILABLE.
   */
  string sval(string section, string key);

  /*!
    \brief Search for the specified section/key value and return it as
    a string.

    Return a string containing the requested value if found, the
    default value passed in the last parameter otherwise.

    \param section Section to search for;
    \param key Key to search for;
    \param defval Default value.

    \return The value requested as a string if found, the default
    value otherwise.
   */
  string sval(string section, string key, string defval);



  /*!
    \brief Search for the specified section/key value and return it as
    an integer.

    If the value is not found an exception will be thrown.

    \param section Section to search for;
    \param key Key to search for.

    \return The value requested as an integer if found.

    \exception ERROR MSG_ENTRY_UNAVAILABLE.
   */
  int ival(string section, string key);

  /*!
    \brief Search for the specified section/key value and return it as
    an integer.

    Return the containing the requested value as an integer if found,
    the default value passed in the last parameter otherwise.

    \param section Section to search for;
    \param key Key to search for;
    \param defval Default value.

    \return The value requested as an integer string if found, the
    default value otherwise.
   */
  int ival(string section, string key, int defval);


  /*!
    \brief Search for the specified section/key value and return it as
    a long integer.

    If the value is not found an exception will be thrown.

    \param section Section to search for;
    \param key Key to search for.

    \return The value requested as an integer if found.

    \exception ERROR MSG_ENTRY_UNAVAILABLE.
   */
  long long int lval(string section, string key);

  /*!
    \brief Search for the specified section/key value and return it as
    a long integer.

    Return the containing the requested value as an integer if found,
    the default value passed in the last parameter otherwise.

    \param section Section to search for;
    \param key Key to search for;
    \param defval Default value.

    \return The value requested as an integer string if found, the
    default value otherwise.
   */
  long long int lval(string section, string key, int defval);


  /*!
    \brief Set a new value for the specified section/key pair.

    If the section/key pair was already present its value will be
    substituted with the new one, otherwise it will be created.

    The changes you made to the values won't be written to the
    configuration file until you call the save() method.

    \param section Section you wish to write to;
    \param key Key you wish to write to;
    \param val New value for the section/key pair;
    \param comment Optional comment to the value.
   */
  void setval(string section, string key, string val, string comment = "");

  /*!
    \brief Set a new value for the specified section/key pair.

    If the section/key pair was already present its value will be
    substituted with the new one, otherwise it will be created.

    The changes you made to the values won't be written to the
    configuration file until you call the save() method.

    \param section Section you wish to write to;
    \param key Key you wish to write to;
    \param val New value for the section/key pair;
    \param comment Optional comment to the value.
   */

  void setval(string section, string key, long long int val, string comment = "");
  /*!
    \brief Set a new value for the specified section/key pair.

    If the section/key pair was already present its value will be
    substituted with the new one, otherwise it will be created.

    The changes you made to the values won't be written to the
    configuration file until you call the save() method.

    \param section Section you wish to write to;
    \param key Key you wish to write to;
    \param val New value for the section/key pair;
    \param comment Optional comment to the value.
   */
  void setval(string section, string key, int val, string comment = "");

  /*!
    \brief Set a new value for the specified section/key pair.

    If the section/key pair was already present its value will be
    substituted with the new one, otherwise it will be created.

    The changes you made to the values won't be written to the
    configuration file until you call the save() method.

    \param section Section you wish to write to;
    \param key Key you wish to write to;
    \param val New value for the section/key pair;
    \param comment Optional comment to the value.
   */
  void setval(string section, string key, Data* val, string comment = "");


  /*!
    \brief Save all the values in a configuration file.

    If no parameter is given the path of the last configuration file
    opened will be used.

    \param filename Path to the configuration file.

    \exception ERROR MSG_CANT_OPEN_FILE.
   */
  void save(string filename = "");
};







//--------------------------------------------------------------------
//!Max number of tokens for a command line
#define MCS_CMD_MAX_TOKENS 100

/*!
  \brief Parse command lines.

  This class parses a MCS command line (very close to the one used in
  a unix like shell) into its tokens.

  In a command line tokens are separated by one or more blanks or
  tabs. A token can also contains blanks, because everything enclosed
  in double quotes is taken as a single token, in this case the double
  quotes are removed from the token. Double quotes can be escaped by a
  backslash, this way becoming part of the token. Also backslashes can
  be escaped with another backslash. Tokens are available through the
  tokenc() and token() methods.

  The first token is considered the command, returned by the cmd()
  method.

  After being parsed the tokens are separated into options and
  arguments. A token is an options if it start with a minus "-"
  character like the options in a unix shell, and can have an optional
  value (following the "=" chatracter). Argument are all the other
  tokens. Options are available through the optc(), opt(), givenOpt()
  methods. Argument to options are available through the optArg()
  method. Arguments are available through the argc() and arg()
  methods.

  This class can also be used to read a MCS batch file, or SQL file
  and perform parameter substitutions. In these scripts an instruction
  terminate with a semicolon (";") character. Comments are everything
  between the "#" character and a newline. Parameters are substituted
  using the $1, $2, etc... placeholders. The replPars_onFile() and
  replPars() to this job. Consider the following script:

  \code
  #This is a comment

  line 1;
  line 2;  #A line with a comment
  line $1;
  line $2; #A line with a comment ;

  line splitted
      in two $3;

  line splitted   #with comment on the first line
      in two $3;  #and on the second !"£$%&;/()='ì
  \endcode

  And a command line:

  \code
  script one two three
  \endcode

  where "script" is the name of the script. A call to
  replPars_onFile() will return a vector of strings as follows:

  \code
  line 1
  line 2
  line one
  line two
  line splitted in two three
  line splitted in two three
  \endcode
*/
class CommandParser {
private:
  MCS_DEBUG_ALLOC;

  //!Command line parsed.
  string origcmdline;

  //!First token, that is the command.
  string lcmd;

  //!vector of tokens.
  vector<string> tokens;

  /*!
    \brief vector of "rests".

    For ach token there is a "rest", that is the rest of the command
    line starting from the token. This string is called the "rest" of
    the token.
  */
  vector<string> rest;

  //!vector of arguments.
  vector<string> args;

  //!vector of options.
  vector<string> opts;

  //!vector of argument to options.
  vector<string> optargs;

  //!Array of arguments stored as Data objects.
  Data* md_args[MCS_CMD_MAX_TOKENS];

  //!Array of arguments to options stored as Data objects.
  Data* md_optargs[MCS_CMD_MAX_TOKENS];

  /*!
    \brief Cleans a string.

    Remove tabs, newline, carriage-return, multiple spaces and
    trailing ";".
  */
  static string clean(string c);

  //!Destroy all Data objects containing arguments.
  void clearArgs();

public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(CommandParser);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(CommandParser);

  //!Constructor.
  CommandParser();

  //!Destructor, this destroy all Data objects containing arguments.
  ~CommandParser();

  //!Return number of tokens.
  int tokenc();

  /*!
    \brief Return token at i-th position.

    The index must be in the allowed range (0 <= i < tokenc())

    \param i Index of the token to be returned.

    \return A string containing the token at i-th position.

    \exception ERROR MSG_INDEX_LT_ZERO.
    \exception ERROR MSG_INDEX_OUT_RANGE.
   */
  string token(int i);

  /*!
    \brief Return the "rest" relative to token at i-th position.

    The index must be in the allowed range (0 <= i < tokenc())

    \param i Index of the "rest" to be returned.

    \return A string containing the "rest" at i-th position.

    \exception ERROR MSG_INDEX_LT_ZERO.
    \exception ERROR MSG_INDEX_OUT_RANGE.

    \sa rest.
   */
  string line_afterToken(int i);


  /*!
    \brief Return argument at i-th position.

    The index must be in the allowed range (0 <= i < argc()). Note
    that i-th position, doesn't mean that the argument is at the i-th
    position in the command line. Those are tokens.

    \param i Index of the argument to be returned.

    \return A reference to a STRING Data object containing the
    argument at i-th position.

    \exception ERROR MSG_INDEX_LT_ZERO.
    \exception ERROR MSG_INDEX_OUT_RANGE.
   */
  Data& arg(int i);

  //!Return argument at i-th position as a string, see arg(int).
  string sarg(int i);


  //!Return number of arguments.
  int argc();


  /*!
    \brief Return option at i-th position.

    The index must be in the allowed range (0 <= i < optc()). Note
    that i-th position, doesn't mean that the option is at the i-th
    position in the command line. Those are tokens.

    \param i Index of the option to be returned.

    \return A string containing the option at i-th position, without
    the minus ("-") character.

    \exception ERROR MSG_INDEX_LT_ZERO.
    \exception ERROR MSG_INDEX_OUT_RANGE.
   */
  string opt(int i);

  //!Return number of options.
  int optc();


  /*!
    \brief Return argument to option at i-th position.

    Argument to options are the part of the option token after the
    equal "=" character. If the equal character is not present in the
    option the corresponding argument is an empty string.

    The index must be in the allowed range (0 <= i < optc()). Note
    that i-th position, doesn't mean that the option is at the i-th
    position in the command line. Those are tokens.

    \param i Index of the option to be returned.

    \return A string containing the argument to the option at i-th
    position, without the equal ("=") character.

    \exception ERROR MSG_INDEX_LT_ZERO.
    \exception ERROR MSG_INDEX_OUT_RANGE.
   */
  Data& optarg(int i);

  //!Parse a command line into tokens, arguments and options.
  void parseCmd(string c);

  //!Parse a command line using standard argc and argv parameters.
  void parseCmd(int argc, char* argv[]);



  //!Returns the entire command line.
  string cline();

  //!Returns the first token, that is the command.
  string cmd();

  //!Returns all arguments (not tokens!) as a string.
  string allargs();

  //!Check if the string in the parameter is the same as the command (case insensitive).
  bool cmpCmd(string cmd);

  //!Check if a certain option has been given.
  bool givenOpt(string opt);

  //!Check if a certain option has been given and return the index in the "i" parameter.
  bool givenOpt(string opt, int& i);


  /*!
    \brief Parse a MCS script.

    This method reads the file given as parameter, and parse it into
    single instructions, that is string terminating with a semicolon
    ";" character. Then it passes the instruction to the replPars()
    method to substitute the parameters. The returned strings are
    stored in a vector that is returned after all the content of the
    file have been parsed.

    \param fn File to parse.

    \return vector of string containing instructions.

    \exception ERROR MSG_CANT_OPEN_FILE.
   */
  vector<string> replPars_onFile(string fn);


  /*!
    \brief Perform parameter substitutions.

    After a command line have been parsed (with parseCmd()) we have at
    our disposal a number of argument. This method substitute every
    occurrence of a placeholder of the type $1, $2, etc..., in "str"
    parameter with the corresponding parameter. It also remove
    comments.

    \param str The string in which perform parameter substitutions.
    \return The final string.
   */
  string replPars(string str);
};






#if ENABLE_MYSQL
//--------------------------------------------------------------------
/*!
  \brief Handle database connection.

  You can use this class to connect to a database. To perform queries
  on the database you should use a Query object, whose methods require
  a DBConn object as parameter.
*/
class DBConn
{
private:
  MCS_DEBUG_ALLOC;

  //!MYSQL connection structure.
  MYSQL* lconn;

  //!True if the connection has been initialized.
  bool lconnInitialized;

  //!True if the connection has been opened.
  bool lconnOpened;

  //!User name.
  string luser;

  //!User password.
  string lpass;

  //!Database name to connect to.
  string ldb  ;

  //!Host running database server.
  string lhost;

  friend class Query;

public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(DBConn);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(DBConn);

  //!Constructor.
  DBConn();

  //!Destructor, calls close().
  ~DBConn();

  /*!
    \brief Connect to a databae server.

    \param user User name.
    \param pass User password.
    \param db   Database name to connect to.
    \param host Host running database server, if empty localhost will
    be used.

    \exception ERROR MSG_CANT_ALLOCATE_MYSQL;
    \exception ERROR MSG_CANT_OPEN_MYSQL_CONNECTION.

    \sa DBConn_connect().
  */
  void connect(string user, string pass, string db, string host="");

  //!Closes a db connection.
  void close();

  //!Tell if a connection to the server is opened.
  bool isOpen();

  //!Returns the same value as mysql_thread_id.
  unsigned long id();

  /*!
    \brief Creates a new DB connection with the same parameters.

    A DBConn object can maintain only one prepared statement at a
    time. So if you want to use several prepared statement you must
    use different DBConn objects. With this method you istantiate on
    the heap a new DBConn object and call its connect method() with
    the same parameters used to connect with the actual object. It's
    user responsibility to delete objects created with this method.

    \return Address of the newly created DBConn object.

    \exception ERROR MSG_CANT_OPEN_MYSQL_CONNECTION.
  */
  DBConn* newDBConn();
};






//--------------------------------------------------------------------
/*!
  \brief Execute queries on the database.

  You can execute two types of query with this class:
    - simple queries (see simpleQuery())
    - prepared statement (prepare(), execute())

  The result-set (if any is present) will be returned as a whole inside a
  vector<string> object in the first case, as a DBRecordSet object in the
  second (see recordSet() method). With prepared statement you can also
  prepare a parametric query, then execute it any number of time changing just
  the parameters.
*/
class Query : public RecordSet
{
private:
  MCS_DEBUG_ALLOC;

  //!Internal reference to the DBConn object used to connect to db.
  DBConn* ldbc;

  //!Flag to indicate that a new DBConn object has been created.
  bool lhandleNewDBConn;

  //!Mysql connection to use.
  MYSQL* lconn;

  //!True if the statement has been initialized.
  bool gotStmtInitialized;

  //!True if we got a result from the last query.
  bool gotResult;

  //!Rows affected by the last INSERT-LIKE query.
  unsigned int laffectedRows;

  //!Last generated id on an AUTO_INCREMENT column.
  long long int lastid;

  /*!
    \brief Record of binded Data objects for input (parameters).
    \warning see warning in prepare().
  */
  Record lparam;


  /*!
    \brief Prints all the result set in a vector of string.

    The index is ordered by field, then for row, so the index of the
    r-th record and f-th field is: r*nfields + f. The number of
    element in the vector is (r+1) * f, because the first row contains
    the field names.

    \param res Address of the MYSQL_RES containing the result-set.
    \param nrows Reference to an integer that will contain the number
    of rows in the result-set.
    \param nfields Reference to an integer that will contain the
    number of fields in the result-set.

    \return A vector<string> with all the information in the
    result-set.
  */
  vector<string> printResultSet(unsigned int& nrows,
				unsigned int& nfields,
				MYSQL_RES* res = NULL);

  /*!
    \brief Mysql statement structure.
    \warning see warning in prepare().
  */
  MYSQL_STMT* lstmt;

  /*!
    \brief Record of binding structure for input (parameters).
    \warning see warning in prepare().
  */
  MYSQL_BIND* lbparam;

  int nlparam;


  //!Record of binding structure for output.
  MYSQL_BIND* lbrec;

  bool fetch(unsigned int newpos, bool random);

  Record* myrec;

  Record* newRecord();

  /*
    IMPORTANT NOTE: lbparam and lparam are protected because the Table class
    needs them to bind parameters. Anyway this should be automatically done
    via mysql_stmt_param_metadata, but it is not yet implemented. When this
    routine will became available this two variables will become private, so
    users should use the method "param" wherever possible.
  */

  /*!
    \brief Bind parameters.

    \exception ERROR MSG_BIND_FAILED.
  */
  void bind();

  string SQL;

  void parseFieldList(int op, string& fields, string& values, int& autoIncr);


public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(Query);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(Query);

  /*!
    \brief Constructor.

    This creates a new Query object that connects to the db server
    using the DBConn object used as parameter. If the second parameter
    is true then the newDBConn() method will be called to create a new
    DBConn object. In this case the newly created object is available
    only inside the Query object, so it will be destroyed inside the
    Query destructor.

    \param lconn DBConn object used to connect to db server;
    \param call_newDBConn Flag to eventually call the
    newDBConn() method on lconn.
  */
  Query(DBConn* lconn, bool call_newDBConn = false);

  /*!
    brief Destructor.

    This call the close() method and eventually destroy the
    DBConn object (if lhandleNewDBConn is true).
  */
  ~Query();

  /*!
    \brief Wrapper aropund prepare() and execute().

    \param SQL The query to execute;
    \param StoreResult Flag passed to execute().
   */
  void query(string SQL, bool StoreResult = false);


  /*!
    \brief Execute a query and return result in a vector of strings.

    This executes the query using mysql_query, no placeholders can be
    present in the query. Then it stores the result on a
    vector<string> object which contains the entire result-set. An
    example to print all the contents of a table follows:

    \code
    DBConn conn();
    conn.connect("user, "pass", "dbname");

    Query qry(&conn, false);
    int nrecs, nfields, i, j;

    vector<string> res = qry.simpleQuery("SELECT * FROM MyTable",
                                         nrecs, nfields);

    for (i=0; i<nrecs; i++) {
      for (j=0; j<nfields; j++)
        cout << res[i*nfields + j];

      cout << endl;
    }
    \endcode

    Remember that all the result-set will be retrieved, so
    it is better to specify a LIMIT clause on the query.

    \param SQL The query to execute.
    \param nrows Reference to an integer that will contain the number of
    rows in the result-set.
    \param nfields Reference to an integer that will contain the
    number of fields in the result-set.

    \return A vector<string> containing all the result set as returned
    by printResultSet().

    \exception ERROR MSG_MYSQL_ERROR.
  */
  vector<string> simpleQuery(string SQL,
			     unsigned int& nrows, unsigned int& nfields);


  //!Wrapper for simpleQuery(string, unsigned int&, unsigned int&).
  vector<string> simpleQuery(string SQL);


  /*!
    \brief Prepare the SQL statement.

    Before executing a prepared statement you must prepare it. Any call to
    this method will destroy all parameter binding, result-set and prepared
    statement previously present.

    To prepare a query with input parameters you should call the method
    prepare_with_parameters() instead of this.

    After this method has executed you can use the recordSet() method to get
    access to the recordset which already contain all fields, but no data.

    \param SQL The query to be prepared.

    \exception ERROR MSG_EMPTY_STRING;
    \exception ERROR MSG_OUT_OF_MEMORY;
    \exception ERROR MSG_MYSQL_ERROR;
    \exception ERROR MSG_TYPE_NOT_HANDLED.
  */
  void prepare(string SQL="");

#define MCS_PAB_INSERT 1
#define MCS_PAB_UPDATE 2
#define MCS_PAB_REPLACE 3


  /*!
    \brief Prepare a query with input parameters.

    Due to the fact that the mysql_stmt_param_metadata() function is not yet
    implemented, not all the queries with input parameters can be prepared, but
    only those queries which performs simple insert or update operations on
    one table. See the following code for an example:

    \code
    //Suppose the the table 'mytable' has been created with the
    //following SQL istruction:
    //
    //CREATE TABLE MYTABLE (C CHAR(10), I INTEGER, F FLOAT, D
    //DATETIME)

    //Prepare an insert query with four parameters.
    qry->prepare_with_parameters(MCS_PAB_INSERT, "C I F D", "MYTABLE");

    //Set the values
    qry->param()[0] = "hello";
    qry->param()[1] = 12;
    qry->param()[2] = 3.14156;
    qry->param()[3] = "2006-03-03 12:00:00";

    //Execute the query
    qry->execute();
    \endcode

    The first parameter identify which kind of query you want to
    execute:
    MCS_PAB_INSERT  ==> insert queries;
    MCS_PAB_REPLACE ==> replace queries;
    MCS_PAB_UPDATE  ==> update queries;

    \param op One of MCS_PAB_INSERT, MCS_PAB_REPLACE or MCS_PAB_UPDATE;
    \param fields List of fields to insert or update, separated by
    blanks;
    \param table Table name;
    \param where Where clause, used only if op = MCS_PAB_UPDATE.
   */
  void prepare_with_parameters(int op, string fields, string table,
							   string where="", int nrec=1);


  /*!
    \brief Wrapper around prepare_with_parameters(int, string, string,
    string).

    \param op One of MCS_PAB_INSERT, MCS_PAB_REPLACE or MCS_PAB_UPDATE;
    \param fields Array of pointer to null terminated strings, each
    containing a field name;
    \param nfields Element in the "field" array;
    \param table Table name;
    \param where Where clause, used only if op = MCS_PAB_UPDATE.
   */
  void prepare_with_parameters(int op, char** fields, int nfields,
							   string table, string where="", int nrec=1);


  /*!

    \brief Returns number of affected records by the last execute()
    call.

    This has meaning only for non-SELECT queries.
  */
  unsigned int nAffectedRows();


  //!Returns a reference to a Record object containing all the input parameters.
  Record& param();

  ///*!
  //  \brief Returns the current record set.
  //*/
  //DBRecordSet* result();

  /*!
    \brief Return the value of a field on a record satisfying a where
    condition.

    This method execute a lookup on the table "table", for a record
    that satisfy the condition "where". Then if this record is found
    it returns the value of the field "field" as a Data reference,
    otherwise it throws an exception.

    \param field The field whose value is to be returned.
    \param table The table to be used for search.
    \param where The condition that should be satisfied by the record.

    \return Reference to a Data object containing the value of
    "field".

    \exception ERROR MSG_NO_RESULT.
  */
  Data& lookup(string field, string table, string where = "");


  /*!
    \brief Execute the prepared statement.

    This will thrown Event exceptions if the statement hasn't be
    prepared yet (see prepare()) or if an error during execution
    occur.

    If the prepared statement is a SELECT query the server can execute
    it in two ways:

    - read all the record set at once, then send all data to the
      client;
    - read just the first record and send it to the client, the
      remnant records will be read when needed;

    You choose which method use by the "StoreResult" parameter, if it
    is true the first method will be used, the second otherwise.

    In both cases the Query class behave almost the same, except that
    in the first case the RecordSet::know_nRows() method will return
    true (and RecordSet::nRows() will return the number of records),
    in the second case will return false.

    \param StoreResult If true storeResult() will automatically be
    called.

    \exception ERROR MSG_STMT_NOT_YET_PREPARED;
    \exception ERROR MSG_MYSQL_ERROR;
    \exception ERROR MSG_BIND_FAILED.
  */
  void execute(bool StoreResult = false);


  bool gotRecordSet();


  //!Close the statement and frees anything has been allocated.
  void close();


  /*!
    \brief Reads the list of tables in the current database.

    The tables list is then stored in the public vector<string>
    tableList.
  */
  void readTableList();

  //!A vector<string> containing the table list.
  vector<string>tableList;

  /*!
    \brief Retrieve information about a table.

    Informations are returned as a vector<string> containing the same
    data that would be returned by a mysql client if you give the
    command "DESCRIBE ...".

    \param tbl Name of the table.

    \return A vector<string> describing the table.
  */
  vector<string> tableInfo(string tbl);


  //!Returns a vector of strings containing details of query execution.
  vector<string> ExecutionDetails(string pre = "");


  /*!
    \brief Fills a buffer with a result-set.

    This method can be used to obtain a printable dump of the result
    set. The fields are separated by tabs, the records are separated
    by newlines. Successive calls are necessary because the buffer has
    a maximum size equal to chunksize. As an example see the
    Result2Ascii() method. When the reference to "chunksize" return
    zero means that all the result set has been dumped.

    \param buf Pointer to buffer to fill;
    \param chunksize Size of the buffer, upon exit it will contain the
    number of bytes actually written in the buffer.
    \param firstTime A boolean variable to be set to true the first
    time na dto false all the rest.

    \exception ERROR MSG_NO_RESULT;
    \exception ERROR MSG_NOT_ENOUGH_SPACE.
  */
  void customFillBuffer(char* buf, unsigned int& chunksize, bool firstTime);

  /*!
    \brief Writes a table into an ASCII file using the
    customFillBuffer() method.

    \exception ERROR MSG_CANT_OPEN_FILE;
  */
  void Result2Ascii(string fn);

  /*!
   \brief Returns the last value generated for an AUTO_INCREMENT column.

   Use this function after you have performed an INSERT statement into
   a table that contains an AUTO_INCREMENT field.

   The return value is undefined if last executed statement wasn't an
   INSERT statement.
  */
  long long int last_id();
};







//--------------------------------------------------------------------
/*!
  \brief A class to directly set and retrieve value from a database
  table.

  This class gives you direct access to the value of a table. Direct
  access means that you can use setFirst(), setLast(), setWhere()
  method of Query to find a specific record. While you can retrieve
  the values from the table with the usual rec() method you can also
  update the table. To update you need to move the record pointer to
  the record you want to update, then insert the new values using the
  rec() method, and call the update() method. Finally you can insert
  new records setting values in the internal array of Data through the
  newRec() method, and call insert();

  \warning For this class to work correctly the table must have a
  unique key on one integer field.
*/
class Table : public Query
{
private:
  MCS_DEBUG_ALLOC;

  //!Table name.
  string ltable;

  //!Name of the index field.
  string lfieldkey;

  //!Position of the index field.
  unsigned int posfieldkey;

  //!Array of values for insert.
  Record newrec;

  void insert_or_update(int op);

public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(Table);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(Table);

  /*!
    \brief Constructor.

    \param db DBConn object used to connect to db server;
    \param table Table name;
    \param fieldkey Field index which is a unique key in the table.
   */
  Table(DBConn* db, string table, string fieldkey);

  //!Destructor.
  ~Table();

  //!Reload the table.
  void loadTable();

  /*!
    \brief Return a reference to a Record object for insert.
   */
  Record& newRec();

  /*!
    \brief Insert a new record.

    Values are those set with the newRec() methods.
  */
  void insert();

  /*!
    \brief Insert (or replace) a new record.

    Values are those set with the newRec() methods.
  */
  void replace();


  /*!
    \brief Update the current record.

    Values are those actually found in the Record returned by the
    rec() method.
  */
  void update();
};
#endif





//Forward declaration
class Env;
class Server;



//
//--------------------------------------------------------------------
/*!
  \brief Base class for MCS threaded objects.

  This is the basic class for all threaded objects inside MCS. It
  offers a log facility as well as a static access to a Env object.
*/
class BaseThread : public Thread {
private:
  MCS_DEBUG_ALLOC;

  //!Thread identifier stored as a null terminated string.
  char tID[4];

protected:
  /*!
    \brief Logging facility.

    \param e Event object containing information about an event.

    \return The event type (Event.type()).
  */
  RetValue Log(Event e);

  //!Pointer to the actual Env object, this can be seen in all threaded object.
  static Env* env;

public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(BaseThread);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(BaseThread);

  /*!
    \brief Constructor invoked from ClienThread objects.

    ClienThread objects are identified by a numeric identifier.

    \param parent Parent thread;
    \param lID Numeric identifier.
  */
  BaseThread(Thread* parent, int lID);

  /*!
    \brief Constructor invoked from other objects.

    The other objects are identified by a mnemonic string identifier.

    \param parent Parent thread;
    \param ltID String identifier.
  */
  BaseThread(Thread* parent, const char *ltID);

  //!Destructor.
  ~BaseThread();

  //!Return the thread identifier.
  const char *tid();


  /*!
    \brief Determine the file type of an external program or script.

    This method is used when a user issue a MCS_CMD_EXEC command, to
    execute an external program or script, to determine the file
    type. Return value can be one of the following:

    - MCS_FT_UNKNOWN;
    - MCS_FT_BATCH;
    - MCS_FT_SQL;
    - MCS_FT_SCRIPT;
    - MCS_FT_BIN.

    \param fn Path to file.

    \return file type.

    \exception ERROR MSG_CANT_OPEN_FILE.
  */
  static int fileType(string fn);

  /*!
    \brief Check if an external program or script is registered in the configuration file.

    The name (as registered in the configuration file) must be given
    as parameter. Upon return the parameter will contain the complete
    path to the real file.

    An exception will be thrown if the external program or script is
    not registered in the configuration file or if the file type (as
    returned by ()) is fileType MCS_FT_UNKNOWN.

    \return The value returned by fileType().
  */
  int chkExt(string& s);

  /*!
    \brief Execute an external program or script in a dedicated environment.

    This routine will create an environment with the following
    variables, the content of which can be specified via this method's
    parameters:

      - PWD
      - MCS_CID
      - MCS_USER
      - MCS_PASS
      - MCS_DBNAME

    Then the external program or script will be executed in the
    directory specified in the "wpath" parameter, and its standard
    output and error will be captured in the files specified in the
    "fout" and "ferr" parameters.

    Scripts will be executed under the "sh" shell. The "wpath",
    "fout", "ferr" parameters can be either absolute paths or path
    relative to the current working dir, that is what is returned by
    mcs::Pwd().

    \param fn Complete path to the program or script;
    \param pars Arguments to the program or script;
    \param wpath Directory where the program or script will be
    executed, also content of the PWD environment variable;
    \param thrID Content of the MCS_CID environment variable;
    \param user Content of the MCS_USER environment variable;
    \param pass Content of the MCS_PASS environment variable;
    \param dbname Content of the MCS_DBNAME environment variable;
    \param fout Path to the file that will contain the standard output
    result;
    \param ferr Path to the file that will contain the standard error
    result;

    \return The exit code of the program or script;
   */
  static int spawn(string fn, string pars,
		   string wpath = ".",
		   string thrID = "x",
		   string user = "x",
		   string pass = "x",
		   string dbname = "x",
		   string fout = "out",
		   string ferr = "err");
};




//--------------------------------------------------------------------
//!Return value for BaseThread.fileType(), unknown type.
#define MCS_FT_UNKNOWN  0

//!Return value for BaseThread.fileType(), batch file.
#define MCS_FT_BATCH    1

//!Return value for BaseThread.fileType(), SQL file.
#define MCS_FT_SQL      2

//!Return value for BaseThread.fileType(), script file.
#define MCS_FT_SCRIPT   3

//!Return value for BaseThread.fileType(), binary file.
#define MCS_FT_BIN      4



/*!
  \brief Class holding information about a client connection.

  A reference to such an object can be obtained from the UserThread::info()
  method to obtain information about that client connection.
*/
class ClientInfo : public Record
{
public:
  //!Client ID, the same as the one returned with the MCS_CMD_CID command.
  Data& id();

  //!IP address of client.
  Data& ipaddress();

  //!Hostname of client.
  Data& hostname();

  //!Time at which the client connected.
  Data& timeConnetcted();

  //!User name.
  Data& username();

  //!Last command executed.
  Data& lastCommand();

  //!Time at which the last command was executed.
  Data& timeLastCommand();

  //!Number of commands executed.
  Data& commandExecuted();

  //!True if the client has logged in.
  Data& logged();

public:
  //!Constructor.
  ClientInfo();
};





/*!
  \brief The server side client thread.

  This class provide the server functionality for a single client
  connected. Every time a new client is connected a new object based
  on this class (or a derived one) will be generated by the Server
  object.

  The main methods are run() and exec(), the first is the body of the
  thread which reads command from the socket, the second is the
  execution unit of those commands.

  This class can be derived to customize MCS behaviour. To do so
  simply overload one or more of the "hook" methods:

  - hk_connect(), called when a new user connects to the server;
  - hk_auth(), called when the user issue the MCS_CMD_DBCONNECT command;
  - hk_exec(), called before executing the command;
  - hk_postexec(), called after the command has been executed;
  - hk_disconnect(), called when the user disconnect.

  These methods are virtual, so you can overload them to customize
  the server, see the following code for an example. Suppose you want
  to implement a custom command, you should derive the UserThread
  class and overload the hk_exec() method:

  \code
  class MyClient : public UserThread
  {
    //Constructors MUST have the same parameters.
    MyClient(Thread* parent, int lID, int newsock) :
            UserThread(parent, lID, newsock);
    {}

    ~MyClient() {}

    RetValue hk_exec(CommandParser* cmd, bool& cmd_executed)
    {
      cmd_executed = false;
      if (cmd->cmpCmd("MyCustomCommand")) {
        cmd_executed = true;  //we are executing the command here
        //... do the job ...
      }
    }
  }
  \endcode

  Once you define this class you should also derive the Server class
  with the MCS_CUSTOM_SERVER macro, so that it istantiate the correct
  class when a client connects (see the Server class
  description). Virtual methods aren't the only way to customize MCS,
  see also the callback functions in the Server class.
 */
class UserThread : public BaseThread {
private:
  MCS_DEBUG_ALLOC;

  //!C socket descriptor, used to initialize the Socket object.
  int csocket;

  //!Structure containing client informations.
  ClientInfo linfo;

  RecordSet* rs;

  bool deleters;

  Synchro syn;

protected:
  //!Socket to client.
  Socket* sock;

  //!The userid of current user.
  int luserid;

  //!Nested batch level.
  int batchlevel;

  //!Tells if client has logged in correctly.
  bool loginok;

  //!User grants.
  int grants;

  //!Default output file.
  string fnout;

  //!Default error file.
  string fnerr;

  //!User name.
  string user;

  //!Password.
  string pass;

  //!Database name, usually the same as Env.appname.
  string dbname;

  //!Host running database server.
  string dbhost;

#if ENABLE_MYSQL
  //!Object to access DB.
  DBConn db;

  //!Query object.
  Query* query;
#endif

  void setActiveRS(RecordSet* rs, bool delWhenFinished = true);

  /*!
    \brief Return the path of the work directory followed by the path
    given in parameter.

    No blanks, quotes or backslashes allowed.

    \param fn Relative path.

    \return An absolute path in the "work" directory.
   */
  string wpath(string fn = "");


  /*!
    \brief Send strings to client.

    The parameter is splitted in at newlines, then sent to client one
    by one  using the MSG_OUT code.

    \param str Strings to be sent.

    \return OK.
  */
  RetValue sendStrings(string str);


  /*!
    \brief Send a message to client.

    \param e Event object containing information about an event;
    \param log If true the message will also be logged.
  */
  RetValue Send(Event e, bool log = true);

  /*!
    \brief Send a Data object to client.

    \param data Reference to a Data object data to be sent.
  */
  RetValue Send(Data& data);

  /*!
    \brief Send a Record object to client.

    \param vec Reference to a Record object data to be sent.
  */
  RetValue Send(Record& vec);

  /*!
    \brief Send a file to the client.

    The file must be present in the work directory. See wpath().

    \param filename Name of the file to be sent.
    \param path Path to the file, including working dir.
  */
  RetValue Send(string filename, string path);

  /*!
    \brief Send a vector of strings to the client.

    Strings will be sent using the MSG_OUT code.

    \param vec Reference to a vector of strings to be sent.
  */
  RetValue Send(vector<string> vec);


#if ENABLE_MYSQL
  /*!
    \brief Send query results as it was a file created with the
    Query.Result2Ascii() method.

    If an exception is raised during the execution it will be catched,
    then the message will be sent to the user as ane error, and the
    Event type will be returned. If the raised exception is of type
    FATAL it will be rethrown.

    \return WARN if there was no result set, event type on a catched
    exception, OK otherwise.
   */
  RetValue sendQueryRes();
#endif


  /*!
    \brief Send a prompt to client.

    Actually we have three type of prompt:

    - MCS_PROMPT_OK;
    - MCS_PROMPT_WARN;
    - MCS_PROMPT_ERROR.

    so the user can see if the last command executed successfully or
    not.

    \param ret A RetValue to determine the prompt to send.
   */
  void prompt(RetValue ret);


  /*!
    \brief The body of the thread.

    Will send a welcome message followed by a prompt when the object
    is started, then read line by line commands from the socket and
    execute them with the exec() method, until the socket is closed, a
    FATAL exception is raised, or someone kills this thread.
   */
  void run();


  /*!
    \brief Virtual method called when a new user connects to the
    server.

    This method calls the cb_connect() and cbwa_connect() callback
    functions if defined.

    If you overload this method the callback functions won't be called
    anymore. In this case if the return value is different from OK the
    thread would die immediately.

    \return The return value from cb_connect(), or OK if no callback defined.
  */
  virtual RetValue hk_connect();


  /*!
    \brief Virtual method called when the user issue the
    MCS_CMD_DBCONNECT command.

    This method calls the cb_auth() and cbwa_auth() callback functions
    if defined.

    If you overload this method the callback functions won't be called
    anymore. In this case if the return value is different from OK the
    authentication process will fail, and you should send an error
    message to the client using the Send() method.

    \param grants Upon return should bear the grants for the user;
    \param loginok Upon return is a flag saying if the user has logged
    in.

    \return The return value from cb_auth(), or OK if no callback defined.
  */
  virtual RetValue hk_auth(int& grants, bool& loginok);


  /*!
    \brief Virtual method called before executing a command.

    This method calls the cb_exec() and cbwa_exec() callback functions
    if defined.

    If you overload this method the callback functions won't be called
    anymore. In this case if the "cmd_executed" flag is true the
    exec() method will return immediately with the overloaded method's
    return value.

    \param cmd Address of the actual CommandParser object;
    \param cmd_executed Upon return tells if the command has been
    already executed or not.

    \return The return value from cb_exec(), or OK if no callback defined.
  */
  virtual RetValue hk_exec(CommandParser* cmd, bool& cmd_executed);


  /*!
    \brief Virtual method called after a command has been executed.

    This method calls the cb_postexec() and cbwa_postexec() callback
    functions if defined.

    If you overload this method the callback functions won't be called
    anymore.

    \param cmd Address of the actual CommandParser object;
    \param ret Return value of the exec() method.
  */
  virtual void hk_postexec(CommandParser* cmd, RetValue ret);


  /*!
    \brief Virtual method called when the user disconnect.

    This method calls the cb_disconnect callback function if defined.

    If you overload this method the callback functions won't be called
    anymore.
  */
  virtual void hk_disconnect();


  /*!
    \brief Execution method for user's command.

    This method execute user's command. It also call the necessary
    virtual methods, and even itself in case of a batch execution.

    The command can be given entirely in the first parameter, or even
    using the second. In any case the executed command will be "cmd
    pars".

    This method can throw several exceptions which are catched by the
    run() method. If a FATAL exception is raised the run() method will
    exit killing the thread.

    \param cmd String with the command;
    \param pars Optional parameters.

    \return value telling if the command has been executed correctly.

    \note This method will run in a critical section to be synchronized with
    the execution of the wakeUpClient() method.
   */
  RetValue exec(string cmd, string pars = "");


  /*!
    \brief Return the address of the Server object which generates
    this Thread.
   */
  Server* parent();


  //!Used to send data object to other threads.
  void send2OtherThread(Data& d, int destid);

public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(UserThread);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(UserThread);

  /*!
    \brief Constructor, called by a Server object when a new client is
    connected.

    \param parent Address of the parent Server object;
    \param lID Thread identifier;
    \param newsock The new C socket descriptor to be used by the new
    thread.
  */
  UserThread(Thread* parent, int lID, int newsock);

  //!Destructor.
  ~UserThread();

  //!Return internal ClientInfo structure.
  ClientInfo& info();

  /*!
    \brief Send a message to the client.

    Usually the this object is waiting to receive a request from the
    client on the other side of the socket. But sometime the
    LocalThread (or derived class) object needs to force the client to
    do something (see the LocalThread.run() method), in this case it
    can call this method to send a message to the client. The message
    sent will be the one contained in the Event object passd as
    parameter. If the parameter is null the message sent will be
    MSG_WAKE_UP.

    Using this method makes sense only if on the other side of the
    socket there is a ClientThread object (see relative
    documentation), only in this case the client will react to this
    message.
  */
  void wakeUpClient(Event* e = NULL);

  //!Record of Data objects to be dispatched to be sent to client.
  Record send;

  //!Record of Data objects received from the client or other threads.
  Record recv;

  //!Return the user name used to authenticate the client.
  string userName();

  int userID();
};




// -------------------------------------------------------------------
/*!
  \brief Server side administrative thread.

  An object of this class will be automatically istantiated from the Server
  object. If this class is not derived its run() method (running in its own
  thread) will forward any Data object in the "send" Record to the
  corresponding UserThread object. You can also derive this class to do all
  the necessary "local" (which means server-side work such as database
  insertion or mainteinance, software updates, or everything is necessary to
  your information service.

  This class can also be used to monitor clients activities using the hk_log()
  method, which is a virtual method called each time the BaseThread.Log() is
  executed. Since all clients activities are logged you can overload this
  method to perform monitoring.
 */
class LocalThread : public BaseThread
{
private:
  MCS_DEBUG_ALLOC;

  /*!
    \brief Thread body.

    This method can be overloadd to  customize the "local" behaviour of the
    server. In this class then run() method wait until there are some Data
    object in the "send" vector. When an object appear it will call the
    dataDispatcher() method.
   */
  virtual void run();


protected:
  UserThread** client;

  //!Returns the Server.pClient variable.
  UserThread** serverClients();

  /*!
    \brief Dispatch Data objects in the "send" vector to UserThread object's
    "send" vectors.

    This method will extract objects from the "send" vector and put them in
    the "send" vector of the corresponding UserThread object (see mcsid()
    method).
   */
  void dataDispatcher();

  /*!
    \brief Search for a client ID (CID) associated with a destination ID.

    Client ID (CID) are the positions in the Server.pClient array that holds
    the address of UserThread objects associated with the client. Each Data
    object has a destination ID (Data.getDest() method) and each UserThread
    object has a destination ID (UserThread.getDestinationId() method). This
    method search the client ID (CID) associated with the UserThread object
    whose destination ID is equal to the destination ID of the Data object
    passed as parameter.

    \param d Data object;
    \param id Address of an object where the client ID (CID) will be stored.

    \return True if the method found at least one UserThread object with the
    required destination ID, false otherwise.
  */
  bool userID2clientID(int userid, int* cid);


public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(LocalThread);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(LocalThread);

  /*!
    \brief Constructor.

    This method simply pass the parameter to its parent class
    constructor (BaseThread).

    \param parent Address of the parent Server object;
    \param lID Thread identifier;
  */
  LocalThread(Thread* parent);


  //!Destructor.
  ~LocalThread();


  /*!
    \brief Virtual method called each time the BaseThread.Log() method
    is executed.

    This method calls the cb_log() and cbwa_log() callback functions if
    defined. If you overload this method the callback functions won't be
    called anymore.

    This method can be overloaded to monitor client activities, since
    each command and server reply are logged.

    \param p Address of the UserThread object whose logging;
    \param e Event object describing what has been logged.
  */
  virtual void hk_log(UserThread* p, Event e);

  //!Record of Data objects received from other threads.
  Record recv;
};




//--------------------------------------------------------------------
/*!
  \class Env
  \brief Hold all environment variables.

  This class reads a configuration file named "mcs.conf" that must be
  in the working directory of execution, which contains all
  environment variables for the execution of a MCS-based
  application. You can see an example of a configuration file in the
  MCS distribution tree "share/test/mcs.conf". The file will be read
  using the Conf class. If one of the environment variable, or the
  entire file, aren't found then a default value will be assumed.

  An Env object (or a derived one) must be istantiated in the main
  program, then used as parameter for the Server constructor.

  \sa Server
 */
class Env {
private:
#if ENABLE_MYSQL
  DBConn db;
#endif //ENABLE_MYSQL

  //!Check the entire MCS tree.
  void chkTree();

  /*!
    \brief Load configuration entries.
  */
  void LoadConf();

  /*!
    \brief Dump environment to configuration file.
  */
  void DumpConf();


public:
  /*!
    \brief Env constructor.

    \param app Application name, configuration entries will be
    searched under a "MCS_Generic" section for first, then under a
    section whose name is equal to tis parameter;
    \param inipath Path for configuration file, default is "./mcs.conf".
   */
  Env(string app = "mcs", string inipath = "mcs.conf");

  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(Env);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(Env);

  //!Destructor.
  ~Env();

  LocalThread* local;

  //!Pointer to the Conf object used to read the configuration file.
  Conf* cnf;

  //!Application name.
  string appname;

  //!Application version.
  string appvers;

  //!Main server path (APPD).
  string path;

  //!Application ini file.
  string inipath;

  //!Host name.
  string localhost;

  //!Server interface name.
  string interface;

  //!Server port number.
  int port;

  ////!Tells if the MCS server is running.
  //bool server_running;

  Server* server;

  //!How many clients can be connected at the same time.
  int max_users;

  //!When the local thread dies the server will be stopped.
  bool cl_local_kills_mcs;

  //!Activate DB facilities.
  bool cl_have_db;

  //!Activate custom authentication.
  bool cl_custom_auth;

  //!Use log file.
  bool cl_logfile;

  //!Use private (per user) working dir, otherwise use path.
  bool cl_work;

  //!Work path is created using cid if true, otherwise with the user name.
  bool cl_work_cid;

  //!Copy log on stdout.
  bool cl_logstdout;

  //!Create local thread.
  bool cl_createlocal;

  //!Resart local thread when it dies.
  bool cl_restartlocal;

  //!Clean work dir on user logout.
  bool cl_clean_logout;

  //!Read grants from the DB table (deprecated).
  bool cl_read_grants;

  //!Auto execute batch file "auto".
  bool cl_autoexec;

  //!Use secure connections.
  bool cl_use_ssl;

  //!Path to the SSL certificate file, used if cl_use_ssl = 1.
  string sslcert;

  //!Path to the SSL private key file, used if cl_use_ssl = 1.
  string sslpriv;

  //!Stream to write the log file.
  ofstream* flog;

  //!User name for the local thread access to DB.
  string db_user;

  //!Password.
  string db_pass;

  //!Database name.
  string db_name;

  //!Host running database server.
  string db_host;

  //!Timeout for client operations, in millisecond.
  unsigned int timeout;

  //!Default size of a chunk of a Serializable object, that is chunks that goes through the network.
  int cl_chunksize;

  //!Pointer to the global SSL context object, used if cl_use_ssl is true.
  void* ssl_ctx;
};












//--------------------------------------------------------------------
/*!
  \brief Main server class for a MCS-based application.

  Every MCS-based application must use an object of this class, or a
  derived one as main server.

  When started it create a LocalThread object (using the
  newLocalThread() method), then it acts like an usual TCP server
  listening on a specified port and waiting for a connection. When a
  new connection arrives it create a new UserThread object (using
  the newUserThread() method) that runs on a separate thread and
  handle the connection. This object will terminate spotaneously and
  destroy himself if a FATAL error is raised or if the LocalThread
  terminate and the Env.cl_root_kills_mcs variable is true. The main
  program can also terminate this object by calling its exit()
  method. Remember that you must create the Server object in the heap
  memory and never try to destroy it, it will destroy by himself, That
  is because it is an object derived from Thread (see Thread).

  To customize MCS you should derive the UserThread and/or
  LocalThread classes, and use a server object which istantiate the
  derived classes instead of the standard ones. For this reason, the
  newLocalThread() and newUserThread() methods are virtual, so you
  can derive the Server class and overload these methods to use the
  UserThread and/or LocalThread derived classes. Suppose you
  implemented a class named MyClient derived from UserThread, and a
  class named MyLocal derived from LocalThread, here's an example of
  the code you should write to correctly derive the Server class:

  \code
  class MyServer: public Server
  {
  public:
    MyServer(Env* lenv): Server(lenv) {}

    ~MyServer() {}

    UserThread* newUserThread(int lID, int newsock) {
      return new MyClient(this, lID, server);
    }

    LocalThread* newLocalThread() {
      return new MyLocal(this, "LLL");
    }
  };

  MyServer* srv = new MyServer(...);
  \endcode

  Since overriding the newLocalThread() and newUserThread() methods
  are the only reason to derive the Server class, we provide an
  easiest way to reach the same goal, that is using the
  MCS_CUSTOM_SERVER macro. The code shown above can be substituted
  with:

  \code
  MCS_CUSTOM_SERVER(MyServer, MyClient, MyLocal);
  MyServer* srv = new MyServer(...);
  \endcode

  To create a working server you must first create an Env object
  containing all information needed to run the server, then you can
  create the server object. A typical usage is as follows:

  \code
  //Define a new server class, with derived UserThread and
  //LocalThread classes.
  MCS_CUSTOM_SERVER(MyServer, MyClient, MyLocal);

  //Create a Env object which reads the configuration file.
  Env* env=new Env("myApp");

  //Create a server object...
  MyServer* srv = new MyServer(env);

  //...and start it.
  srv->start();
  \endcode

  We provide a facility function (mcsstart()) that perform all these
  steps. You can substitute the code above with:

  \code
  Env* = mcsstart("myApp");
  \endcode

  This class also provide access to the "callback functions"
  customizing facilities. Using callback functions you can customize
  the server behaviour without deriving the UserThread class (as you
  would do if you want to use the "hooks" methods, instead of
  callback). Callback functions are C++ functions (not methds!) with
  well determined parameters and return value, which are called
  automatically by the "hooks" (note that if you overload those method
  the callback functions won't be called anymore). In the Server class
  there are a number of variables whose name start with "cb_" or
  "cbwa_", which are pointer to functions. There are one "cb_" and one
  "cbwa_" variable for each "hook" method (described in the relative
  classes LocalThread and UserThread).

  Suppose you want a function to be executed each time a line of log
  is written, you could of course derive the LocalThread class and
  overload the hk_log() method. Another way is to set a callback
  function for that hook, here's an example:
  \code
  //Define the callback function
  void myCallbackFunction(int logID, int  tID, Event e)
  {
    //... Do the job ...
  }

  //Istantiate an Env and a  Server (or a derived) object.
  Env* env = new Env("myApp");
  Server* srv = new Server(env);

  //Before starting the server set the appropriate callback variable.
  srv->cb_log = &myCallbackFunction;

  //Finally start the server
  srv->start;
  \endcode

  Note that the callback function has the same parameters and return
  values as the corresponding hook method. Note also that here we used
  the "cb_log" variable, there is also a "cbwa_log" variable but this,
  as well as all the other "cbwa_" variables, are pointers to void
  functions without arguments (the "wa" means just "without
  arguments").

You can use callback functions instead
  \sa LocalThread
  \sa UserThread
  \sa Env
 */
class Server : public BaseThread
{
private:
  MCS_DEBUG_ALLOC;

  //!Array of pointers to UserThread objects.
  UserThread** pClient;

  friend class LocalThread; //To give access to pClient

  /*!
    \brief Server thread body.

    Initialize the server socket, then wait for a new connection. When
    a new connection arrives call the virtual newUserThread() method
    to istantiate a new client thread that can handle that client
    connection.
  */
  void run();

  //!Search for a free client identifier when a new client tries to
  //!connect. See Env.max_users.
  int find_free_id();

protected:
  /*!
    \brief Called when a child thread terminate, used to free
    resources.

    \note If LocalThread object notify its termination and the
    Env.cl_root_kills_mcs option is true then this method will call
    Server.exit(), terminating the ewhole server.
  */
  void notify(int id, Thread* ref);

  //!Search for a free identifier and creates a new UserThread object.
  void newClient(int newsock);

  ////!Frees the resources allocated by the child with specified
  ////!identifier.
  //void freeClient(int i);

  //!Kills all UserThread and the LocalThread objects.
  void killAllClients();

  /*!
    \brief Virtual method called when a new user connects to the
    server.

    This method calls the cb_newClient() callback functions if defined.  If
    you overload this method the callback functions won't be called anymore.

    This method work the same as the hk_connect() virtual method, but it is
    used inside the Server class instead of UserThread.

    If you overload this method the callback functions won't be called
    anymore.

    \param i Index of the newly created UserThread object.
  */
  virtual void hk_newClient(int i);

  //!Kill client with specified identifier.
  void killClient(int i);


  /*!
    \brief Wrapper around UserThread constructors.

    Parameters are passed directly to the constructor. This method can
    be overload to istantiate object of a UserThread derived class.
  */
  virtual UserThread* newUserThread(int lID, int newsock);

  /*!
    \brief Wrapper around LocalThread constructors.

    This method can be overload to istantiate object of a LocalThread derived
    class.
  */
  virtual LocalThread*  newLocalThread();

public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(Server);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(Server);

  /*!
    \brief Constructor.

    \param lenv Address to a valid Env object;
    \sa Env
   */
  Server(Env* lenv);

  //!Destructor.
  ~Server();

  /*!
    \brief Return a RecordSet object containing informations
    about all clients connections.

    This method return the address of a RecordSet object allocated in
    the heap. Users should delete this object after use.
  */
  RecordSet* getAll_ClientInfo();

  //!Pointer to a callback function, called by hk_log().
  void     (*cb_log)         (UserThread* p, Event e);

  //!Pointer to a callback function, called by hk_connect().
  RetValue (*cb_connect)     ();

  //!Pointer to a callback function, called by hk_auth().
  RetValue (*cb_auth)        (int&, bool&);

  //!Pointer to a callback function, called by hk_exec().
  RetValue (*cb_exec)        (CommandParser*, bool&_executed);

  //!Pointer to a callback function, called by hk_postexec().
  void     (*cb_postexec)    (CommandParser*, RetValue);

  //!Pointer to a callback function, called by hk_disconnect().
  void     (*cb_disconnect)  ();

  //!Pointer to a void callback function without arguments, called by hk_log().
  void     (*cbwa_log)       ();

  //!Pointer to a void callback function without arguments, called by hk_connect().
  void     (*cbwa_connect)   ();

  //!Pointer to a void callback function without arguments, called by hk_auth().
  void     (*cbwa_auth)      ();

  //!Pointer to a void callback function without arguments, called by hk_exec().
  void     (*cbwa_exec)      ();

  //!Pointer to a void callback function without arguments, called by hk_postexec().
  void     (*cbwa_postexec)  ();

  //!Pointer to a void callback function, called by hk_newClient().
  void     (*cb_newClient)   (int i);

  //!Record of Data objects to be sent to various threads.
  Record dispatch;
};




/*!
  \brief Start a MCS server.

  This function start a MCS Server (or a derived object) creating the
  necessary Env object, and eventually catching exceptions. If an
  exception is raised the message will be written on standard error.

  This funtion return the address of an Env object, or NULL if an
  error occurred. The Env object needs to be destroyed when the
  program exit.

  A typical use of this function is in the main program, you can start
  a MCS server without derived classses with the following code:
  \code
  #include <mcs.hh>
  using namespace mcs;

  Env* env = mcsStart("myApp");
  \endcode

  Instead, if you implemented some derived classes, you should change
  the code as follows:
  \code
  #include <mcs.hh>
  using namespace mcs;

  //...derived class implementation.

  //Derive the Server class, see relative documentation.
  MCS_CUSTOM_SERVER(MyServer, MyClient, MyLocal);

  int main(int argc, char* argv[])
  {
    Env* env = mcsCustomStart("myApp");
  }
  \endcode

  The mcsCustomStart() function has been implemented by the call to
  the MCS_CUSTOM_SERVER macro.

  \param appname Application name, will be passed to the Env
  constructor.

  \param inipath Configuration file path, will be passed to the Env
  constructor.

  \return Address of the newCustomServer function if defined, users
  should not use this argument.
 */
Env* mcsStart(string appname, string inipath = "",
	      Server* (*cb_newServer)(Env*) = NULL);


/*!
  \brief Wait for a MCS server termination.

  The Server termination will occur if the object terminate
  spontaneously (see Sever class documentation). This function install
  a signal handler for the SIGINT, SIGQUIT and SIGTERM signals, so
  termination will occur also if one of these signals is received by
  the application.

  This function simply waits for the server termination, once this occurr it
  destroy the Env object and then returns. A typical use of this function is
  in the main program, toghether with the mcsStart() function:

  \code
  #include <mcs.hh>
  using namespace mcs;

  int main(int argc, char* argv[])
  {
    mcsStart("myApp");  //...or mcsCustomStart()
  }
  \endcode

  \param env Address of a valid Env object.

  \note If you run the example shown above in a terminal, and you
  press CTRL-C the application will receive a SIGINT signal, so it
  will terminate the server, then exit.
 */
//void mcsWait(Env* env);


#define MCS_CUSTOM_USER(CLASS)                                        \
class CLASS : public mcs::UserThread			              \
{								      \
public:								      \
    CLASS(mcs::Thread* p, int id, int sock) :		              \
	UserThread(p, id, sock) {}			              \
    ~CLASS() {}
#define MCS_CUSTOM_USER_END(CLASS)  };


#define MCS_CUSTOM_LOCAL(CLASS)                                       \
class CLASS : public mcs::LocalThread				      \
{								      \
public:								      \
    CLASS(mcs::Thread* p) :                                           \
	LocalThread(p) {}				              \
    ~CLASS() {}
#define MCS_CUSTOM_LOCAL_END(CLASS)  };


/*!
  \brief Macro to define a Server derived class, that works with
  UserThread and LocalThread derived objects.

  The UserThread and LocalThread derived classes must have the same
  constructors as the parents classes. Remember that this class simply
  defines a new class, it doesn't istantiate any object.

  \param CLASS  The newly defined class name;
  \param CLIENT The UserThread derived class name;
  \param LOCAL  The LocalThread derived class name.
  \param EXTRA  Extra code added inside the server class definition.
 */
#define MCS_CUSTOM_SERVER(CLIENT, LOCAL)                              \
mcs::Server* newCustomServer(mcs::Env* env);                          \
								      \
class mcsCustomServer : public mcs::Server                            \
{                             					      \
private:                                                              \
    mcs::UserThread* newUserThread(int id, int sock) {                \
	return new CLIENT(this, id, sock);			      \
    }								      \
								      \
    mcs::LocalThread* newLocalThread() {                              \
	return new LOCAL(this);            			      \
    }								      \
								      \
public: 							      \
    mcsCustomServer(mcs::Env* lenv): Server(lenv) {}		      \
								      \
    ~mcsCustomServer() {}                                             \
};                                                                    \
                                                                      \
mcs::Server* newCustomServer(Env* env)                                \
{  return new mcsCustomServer(env); }                                 \
                                                                      \
mcs::Env* mcsCustomStart(string appname, string inipath = "")         \
{ return mcsStart(appname, inipath, &newCustomServer); }



//--------------------------------------------------------------------
/*!
  \brief Connect to a MCS server as a client.

  This class let you connect to a MCS server as a client, execute
  command on it, upload and download files and binary data. A typical
  usage is as follow:

  \code
  #include <mcs.hh>
  using namespace mcs;

  ...

  try {
    //Connect to the server.
    Client cli = new Client("", "Host_running_a_MCS_server");

    //Login
    cli->login(user, pass, dbna);

    //Retrieve the Client Identifier
    cli->exec("cid");
    int cid = cli->recv.pop().ival();

    //Query a table in the database
    if (! cli->exec(MCS_CMD_QUERY + string(" SELECT * FROM MyTable")))
    {
      //The server reported an error
      while (cli->msg.ready())
        cout << cli->msg.pop().sval()  << endl;
      exit(1);
    }

    //Retrieve the first record
    cli->exec(MCS_CMD_RECORD + string (" 0"));

    //Use values in field...
    cli->record()[0].ival();
    cli->record().field("MyField").sval();
    //...

    //Create an INTEGER Data object to send to server.
    Data aa(12);
    cli->exec(MCS_CMD_PUTDATA, &aa);

    //Retrieve a Data object from the server.
    cli->exec(MCS_CMD_GETDATA);
    Data bb = cli->recv.pop();

    //Upload a file
    cli->exec(MCS_CMD_PUT + string(" FileToUpload");

    //Download a file
    cli->exec(MCS_CMD_GET + string(" FileToDownload");

    //Close connection
    cli->exec(MSG_BYE);
  }
  catch (Event e) {
    cout << e.msg() << endl;
  }
  \endcode
 */
class Client :  public Socket, public RecordSet
{
protected:
  //!Local path to read/write files.
  string lpath;

  //!Flag telling if the client is connected or not.
  bool connected;

  //!Clear all records.
  void clearRecords();

  bool writetofile;

  int fileDescriptor;

  bool fetch(unsigned int newpos, bool random);

  Record* lrecord;

  //Server side Client Identificator.
  int lcid;

  //Server side chunk size.
  int lchunksize;

public:
  /*!
    \brief Declared to avoid using of default copy constructor.

    \warning This constructor is declared but not implemented. If you try to
    use it you will get a compilation error.
  */
  MCS_MISSING_COPY_CONSTRUCTOR(Client);

  /*!
    \brief Declared to avoid using of default assignment operator.

    \warning This operator is declared but not implemented. If you try to use
    it you will get a compilation error.
  */
  MCS_MISSING_ASSIGNMENT_OPERATOR(Client);


  /*!
    \brief Record containing all reply's numeric codes sent by the
    server while executing the last command.
  */
  Record code;

  /*!
    \brief Record containing all reply's messages sent by the server
    while executing the last command.
  */
  Record msg;

  /*!
    \brief Record containing all output lines (code: MSG_OUT) sent by
    the server while executing the last command.
  */
  Record out;

  //!Record containing all Data objects sent by the server.
  Record recv;

  //!Record containing all auxiliary fields sent by the server.
  Record aux;

  /*!
    \brief Constructor.

    \param path Local directory;
    \param server Address of a host running a MCS server;
    \param port Port in which the MCS service is listening;
    \param synchro If all Record objects must be synchronized (useful
    in multithreading application);
    \param SSL If the TCP connection should be protected with SSL;
    \param timeout Time to wait for server response (in milliseconds).
   */
  Client(string path, string server, int port=MCS_DEFAULT_PORT,
	 bool synchro = false, bool SSL = false,
	 unsigned int timeout = MCS_DEFAULT_CLIENT_TIMEOUT);

  //!Destructor.
  virtual ~Client();


  /*!
    \brief Log in to a MCS server.

    You must already been connected (see the constructor) to the host
    before executing this method.

    \param user User name;
    \param pass Password;
    \param db Database name, default is the application name.

    \return True if logged in, false otherwise.
   */
  bool login(string user, string pass, string db = "");

  /*!
    \brief Execute commands on the server.

    \param cmd The command to execute;
    \param Data If the command is MCS_CMD_PUTDATA you should supply
    an address to a valid Data object;
   */
  bool exec(string cmd, Data* data = NULL);

  void writeToFile(int des);

#define MCS_CLIENT_ERROR       1
#define MCS_CLIENT_DATA        2
#define MCS_CLIENT_FILE        3
#define MCS_CLIENT_DISCONNECT  4
  virtual void event(int op, Data d, int code = 0);

  //!Tell if the object is connected to the MCS server.
  bool isConnected();

  //!Return the server side client identificator.
  int cid();

  //!Return the server side chunk size used to send data across network.
  int chunksize();
};



/*!
  \brief A high level class to use system pipes.

  This class can be used to perform communication between threads using system
  pipes. There must always be two threads: a "producer" (write) and a
  "consumer" (read) and the same Pipe object must be accessible from both
  threads.

  The communication is initiated by one of the threads through the create() or
  createNamed() methods, while the other is waiting until the isReady() method
  returns true (and maybe using sleep_ms()). When the create() method is used
  an unnamed pipe will be created and both threads will read/write using file
  descriptors. If createNamed() method is used then a FIFO special file will
  be created and (only) one thread will access the pipe as if it was a normal
  file (that is using open, or fopen, etc...). The filename for the FIFO is
  generated automatically and can be retrieved wih the filename() method.

  The consumer thread obtain the file descriptor to read using the openRead()
  method, the producer thread obtain the file descriptor to write using the
  openWrite() method. To each openRead() (or openWrite()) call must correspond
  one closeRead() (or closeWrite()) call.

  If the producer thread is using the file descriptor obtained through
  openWrite() then it can check whether the other side of the pipe (namely the
  consumer side) is still open by calling the consumerHasGone() method.
 */
class Pipe {
private:
  //!To protect the filecount variable.
  Synchro synchro;

  //!Used to generate a unique filename.
  static int filecount;

  //!Name of the FIFO file.
  string pipefn;

  //!Pipe file descriptors (0 read side, 1 write side).
  int pipefd[2];

  /*!
    \brief Tell if a pipe has been created.

    A pipe will be created when a create() or createNamed() method is
    invoked. Note that if a file is in the local filesystem the pipe
    won't be created.
   */
  bool flcreated;

  //!If it is a named pipe (a FIFO file) or not.
  bool named;

public:
  //!Constructor.
  Pipe();

  //!Destructor.
  ~Pipe();

  //!Returns true if the pipe has been set-up (through the create() or
  //!createNamed() methods).
  bool isReady();

  //!If the pipe has been set-up through the createNamed() method this returns
  //!the generated file name.
  string filename();


  /*!
    \brief Tell if the consumer thread is still reading.

    This method can be used only if the producer thread is using the file
    descriptor obtained through openWrite(). In this case the producer should
    check that the consumer is still reading before each write attempt.
   */
  bool consumerHasGone();


  //!Create an unnamed pipe, both threads will read/write using file
  //!descriptors.
  void create();

  //!Create a named pipe, that is a FIFO special file and returns the filename.
  string createNamed();

  /*!
    \brief Return the file descriptor for the consumer side of the pipe.
  */
  int openRead();

  /*!
    \brief Return the file descriptor for the producer side of the pipe.
  */
  int openWrite();

  /*!
    \brief Close the file descriptor associated with the consumer side of the
    pipe.
  */
  void closeRead();

  /*!
    \brief Close the file descriptor associated with the producer side of the
    pipe.
  */
  void closeWrite();
};





#if ENABLE_CURL
/*!
  \brief A class that provide a uniform access to file retrieving.

  This class is capable (through CURL) to provide access to files both
  on local or remote file systems.
*/
class URLReader : Pipe
{
private:
  //!True if curl_global_init has been called (must be called once).
  static bool fl_curl_global_init;

  //!URL requested.
  string lurl;

  //!If the URL refer to a resource on local filesystem.
  bool local;

  static size_t cb_write(void *ptr, size_t size, size_t nmemb, void *This);

  ThreadFunc* thr;
  static int thread_run(void* p);
  void thread_fetch();

protected:
  /*!
    \brief Writes a chunk of data into the pipe.

    This method is called each time a new chunk of data is ready to be written
    in the write side of the pipe. This method can be ovverriden to implement
    custom filtering.
  */
  virtual unsigned int Write(void *ptr, unsigned int size, unsigned int nmemb);

  void flush();

public:
  //!Constructor.
  URLReader();

  //!Destructor.
  virtual ~URLReader();

  //!Obtain a file descriptor to read the file passed as parameter.
  int OpenAsFD(string url);

  //!Create a FIFO on file system to read from the specified file.
  const char* OpenAsFifo(string url);

  void Download(string url, string fn);


  /*!
    \brief Read from the resource.
  */
  int Read(char* buf, int maxlen);

  /*!
    \brief Terminates a file retrieval.

    This method closes the open file descriptor (if any) and terminates an
    open connection. Note that this method must be called once for each
    OpenAsFD() or OpenAsFile() call.

    This method will raise an exception if an error occurred during file
    retrieval.
  */
  void Close();

  string url();

  //Event* thread_error();

  static bool chkLocal(string& url);
};
#endif



#if ENABLE_CFITSIO

/*!
  \brief Read a FITS file.

  The documentation for this class has yet to be written.
 */
class FITSReader:  public RecordSet
{
private:
  void* fptr;
  long int nrows;
  int ncols;
  bool local;
  int nhdu;
  bool fetch(unsigned int newpos, bool random);

  Buffer memfile;
  void*  memfilep;
  size_t memfilesize;

public:
  FITSReader();
  ~FITSReader();

  void open(Buffer& buf);
  void open(string fn);
  void close();

  Record header;
  Record header_comments;

  void selectHDU(int hdunum);
  bool selectNextHDU();

  void selectHDU(string name, int extver = 0);
  int currentHDU();
  int HDUCount();

  bool ffile_is_compressed(string fn);

  void read_json_header(const string& json_string);
};


/*!
  \brief Write a fits file.

  This class has yet to be implemented.
*/

class FITSWriter:  public Record
{
private:
  void* fptr;

public:
  FITSWriter();
  ~FITSWriter();

  void open(string fn, bool compressed = false);
  void close();

  void newKeyword(string key, string value, string comment = "");
  void newBinTable(Record& meta, string name = "");
  void newAsciiTable(int nfields, string name = "");
  void newImage(Types bitpix, unsigned int naxis, unsigned int naxes,
		void* buf, string name = "");

  //The "buf" parameter is used only to write a image HDU.
  void write(void* buf);
};
#endif //ENABLE_CFITSIO


//class FileReader : public RecordSet
//{
//private:
//  URLReader input;
//  int fd;
//
//  Record master;
//  string sep;
//  string eor;
//
//  Buffer buf;
//
//  bool fetch(unsigned int newpos, bool random);
//
//public:
//  FileReader();
//  ~FileReader();
//
//  void open(string url);
//  void close();
//
//  void setMaster(Record& rec);
//  void setMaster(Record& rec, string sep);
//  void setMaster(Record& rec, string sep, string eor);
//};



#define MCS_B64_DEF_LINE   72
#define MCS_B64_MIN_LINE    4

#define MCS_B64_ENCODE         0
#define MCS_B64_DECODE         1
#define MCS_B64_DECODEINPLACE  2

/*!
  \brief A Base 64 encoder/decoder.

  For an example on how to use this class see the b64codec.cc file.
*/
class B64_Codec {
private:
  /*
  ** Translation Table as described in RFC1113
  */
  static const char* cb64;

  /*
  ** Translation Table to decode (created by author)
  */
  static const char* cd64;

  //!Pointer to allocated internal buffer.
  char* bufAllocated;

  //!Size of allocated buffer.
  unsigned int bufsize;

  //!Allocate "Length" bytes in internal buffer.
  void requireSpace(unsigned int Length);

  //!Free internal buffer.
  void freeBuffer();

  //!Pointer to output buffer.
  char* buf_out;

  //!Size of data in output buffer.
  unsigned int bufused;

  //!Cursor for input buffer.
  char* pin;

  //!Cursor for output buffer.
  char* pout;

  //!Pointer to the end of input buffer.
  char* EOIB;

  //!Required linesize (when encoding).
  unsigned int linesize;

  //!Blocks of base64 data already written
  unsigned int blocksout;

  unsigned char precData[4];
  int precDataCount;

  //!Tell if we are at the end of input buffer.
  bool eob();

  //!Read one byte from input buffer.
  unsigned char readData();

  //!Write one byte to output buffer.
  void writeData(unsigned char d);

  //!Encode a block of 3 binary bytes into 4 'Base64' bytes.
  void encodeblock(unsigned char in[3], unsigned char out[4], int len);

  //!Decode a block of 4'Base64' bytes into 3 binary bytes.
  void decodeblock(unsigned char in[4], unsigned char out[3]);

public:

  //!Constructor.
  B64_Codec();

  //!Destructor.
  ~B64_Codec();

  //DO NOT SET par_buf_out = buf_in (sigsegv)

  /*!
    \brief Encode a block of data.

    If par_buf_out is NULL an internal buffer will be automatically
    allocated for encoded data. This buffer can be reached through the
    buffer() method.

    \warn If buffers pointed by buf_in and par_buf_out overlaps the
    result are unpredictable.
  */
  unsigned int encode(char* buf_in, int Length, char* par_buf_out = NULL,
		      unsigned int linesize = MCS_B64_DEF_LINE);

  //Decode may happen also in the same buffer (set par_buf_out =
  //buf_in).

  /*!
    \brief Decode a block of data.

    If par_buf_out is NULL an internal buffer will be automatically
    allocated for decoded data. This buffer can be reached through the
    buffer() method.

    \warn If buffers pointed by buf_in and par_buf_out overlaps the
    result are unpredictable.
  */
  unsigned int decode(char* buf_in, int Length, char* par_buf_out = NULL);

  char* buffer();
  unsigned int bufUsed();
};



#define MCS_SELECT_READ 0
#define MCS_SELECT_WRITE 1


int Select(int fd, unsigned int sec_timeout, unsigned int usec_timeout, int op);
int Select(int fd[], int nfd, unsigned int sec_timeout, unsigned int usec_timeout, int op);



} //Namespace mcs
#endif //DEF_MCS_HH
