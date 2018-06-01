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
#include "mcs.hh"
void mcs::Event::init_vmsg() {
vmsg[MSG_LOG_COMMAND] = "< %s";
vmsg[MSG_OK] = "OK.";
vmsg[MSG_WELCOME] = "Welcome at %s running %s ver. %s";
vmsg[MSG_WELCOMEUSER] = "Welcome user %s.";
vmsg[MSG_PASSWORD] = "OK.";
vmsg[MSG_DBNAME] = "Database changed to %s";
vmsg[MSG_DBHOST] = "Database host changed to %s";
vmsg[MSG_CONNECTED] = "Login successfull (%s)";
vmsg[MSG_DB_ROWSAFF] = "Rows affected: " MCS_SEP "%d" MCS_SEP;
vmsg[MSG_SENDFILE_START] = "Sending file " MCS_SEP "%s" MCS_SEP;
vmsg[MSG_SENDDATA_START] = "Sending data " MCS_SEP "%d" MCS_SEP;
vmsg[MSG_RECVFILE_START] = "OK, receiving file %s";
vmsg[MSG_RECVDATA_START] = "OK, receiving data";
vmsg[MSG_FILE_STOP] = "File received: %s";
vmsg[MSG_DATA_STOP] = "End of data.";
vmsg[MSG_OUT] = "%s";
vmsg[MSG_WAKE_UP] = "Wake up!";
vmsg[MSG_USERTHREAD_TERMINATING] = "Client thread is terminating...";
vmsg[MSG_CLIENT_ID] = "Client ID: " MCS_SEP "%d" MCS_SEP ", chunk size: " MCS_SEP "%d" MCS_SEP;
vmsg[MSG_BATCH_START] = "BATCH(%d) exec: %s";
vmsg[MSG_BATCH_STOP] = "BATCH(%d) finish.";
vmsg[MSG_DB_QRY_OK] = "Query executed successfully.";
vmsg[MSG_SCRIPT_START] = "Executing script: %s";
vmsg[MSG_SCRIPT_STOP] = "Script execution ended.";
vmsg[MSG_SQL_START] = "Executing SQL script: %s";
vmsg[MSG_SQL_STOP] = "SQL script execution ended.";
vmsg[MSG_DQR_START] = "Executing Data Query: %s";
vmsg[MSG_DQR_STOP] = "Rows returned: " MCS_SEP "%d" MCS_SEP;
vmsg[MSG_DB_FIELDS_DESCR] = "Sending fields details " MCS_SEP "%d" MCS_SEP;
vmsg[MSG_DB_FIELDS_DESCR2] = "End fields details";
vmsg[MSG_SYS_EXIT_STATUS] = "Exit status: %d";
vmsg[MSG_USER_HAS_GRANTS] = "User %s has grants : %d.";
vmsg[MSG_THREAD_CREATE] = "%s\t\t\t\tcreated";
vmsg[MSG_THREAD_DESTROY] = "%s\t\t\t\tdestroyed";
vmsg[MSG_SERVER_DIR] = "APPD set to: %s";
vmsg[MSG_SERVER_RUNNING] = "MCS Server running on %s:%d";
vmsg[MSG_CREATING_LOCAL] = "Creating new LocalThread...";
vmsg[MSG_CREATING_CLIENT] = "Creating new UserThread on ID=%d...";
vmsg[MSG_LOCAL_THREAD_DIED] = "Local thread died, shutting down server...";
vmsg[MSG_KILLING_CLIENT] = "Killing client %d...";
vmsg[MSG_KILLING_LOCAL] = "Killing local thread...";
vmsg[MSG_KILLING_ALL_CLIENTS] = "Killing all clients...";
vmsg[MSG_COPYING_DATA_OBJECTS] = "Copying Data objects in %s";
vmsg[MSG_NEW_RECORDSET] = "New RecordSet ready";
vmsg[MSG_FETCH_EOF] = "End of file reached";
vmsg[MSG_BYE] = "Bye.";
vmsg[MSG_DUMMY] = "";
vmsg[MSG_CANT_OPEN_FILE] = "Can't open file %s";
vmsg[MSG_INVALID_SWITCH] = "Invalid value for switch: %d";
vmsg[MSG_METHOD_MUST_BE_OVERLOADED] = "This virtual method needs to be overloaded: %s";
vmsg[MSG_NET_INTERFACE_NAME_NOT_EXISTS] = "Interface named %s doesn't exists";
vmsg[MSG_NET_INTERFACE_ID_NOT_EXISTS] = "Interface with index %d doesn't exists (total %d)";
vmsg[MSG_NET_INTERFACE_NOT_ACTIVE] = "Interface %s is not active";
vmsg[MSG_RETRIEVING_INTERFACE_FLAGS] = "Can't get flags for interface %s";
vmsg[MSG_RETRIEVING_HOSTNAME] = "Error calling gethostbyname(): %s";
vmsg[MSG_RETRIEVING_PEERNAME] = "Error calling getpeername(): %s";
vmsg[MSG_RETRIEVING_HOST_INFO] = "Error calling gethostbyaddr(): %s";
vmsg[MSG_CANT_CONNECT_TO_HOST] = "Can't connect to host: %s";
vmsg[MSG_TIME_OUT] = "Connection timed out";
vmsg[MSG_UNEXPECTED] = "Unexpected error. %s";
vmsg[MSG_CALLING_SELECT] = "Error calling select(): %s";
vmsg[MSG_CALLING_RECV] = "Error calling recv(): %s";
vmsg[MSG_CALLING_SEND] = "Error calling send(): %s";
vmsg[MSG_CLOSED_BY_PEER] = "Connection closed by peer";
vmsg[MSG_CALLING_BIND] = "Error calling bind(): %s";
vmsg[MSG_CALLING_LISTEN] = "Error calling listen(): %s";
vmsg[MSG_CALLING_PTHREAD_CREATE] = "Error calling pthread_create()";
vmsg[MSG_OUT_OF_MEMORY] = "Out of memory";
vmsg[MSG_DIRECTORY_NOT_FOUND] = "Directory not found: %s";
vmsg[MSG_CANT_MKDIR] = "Can't create directory: %s";
vmsg[MSG_CANT_RMDIR] = "Can't remove directory: %s";
vmsg[MSG_CANT_MKFIFO] = "Can't create FIFO file";
vmsg[MSG_CALLING_PIPE] = "Error calling pipe()";
vmsg[MSG_ERROR] = "Error: %s";
vmsg[MSG_SYNTAX] = "Syntax error: %s";
vmsg[MSG_WRONG_DIM_SPEC] = "Wrong index for dim %d: %d";
vmsg[MSG_TYPE_NOT_HANDLED] = "Data type not handled on field %d: %d";
vmsg[MSG_CONVERSION_STRING_INT] = "Conversion error, STRING to INT";
vmsg[MSG_CONVERSION_BLOB_INT] = "Conversion error, BLOB to INT";
vmsg[MSG_CONVERSION_STRING_FLOAT] = "Conversion error, STRING to FLOAT";
vmsg[MSG_CONVERSION_BLOB_FLOAT] = "Conversion error, BLOB to FLOAT";
vmsg[MSG_CONVERSION_DATETIME] = "Conversion error (not a date and/or time): %s";
vmsg[MSG_CONVERSION_BLOB_DATETIME] = "Conversion error, BLOB to DATETIME";
vmsg[MSG_CONVERSION_INT_BLOB] = "Conversion error, INTEGER to BLOB";
vmsg[MSG_CONVERSION_FLOAT_BLOB] = "Conversion error, DOUBLE to BLOB";
vmsg[MSG_CONVERSION_STRING_TOO_LONG] = "Conversion error: string too long";
vmsg[MSG_CONVERSION_STRING_BLOB] = "Conversion error, STRING to BLOB";
vmsg[MSG_CONVERSION_TIME_LINT] = "Conversion error, TIME to number shorter than INT";
vmsg[MSG_CONVERSION_NAN] = "Conversion error: not a number";
vmsg[MSG_NOT_A_BLOB] = "Not a BLOB object";
vmsg[MSG_BLOB_TOO_BIG] = "BLOB too big: %d";
vmsg[MSG_QUEUE_FULL] = "Queue is full";
vmsg[MSG_INDEX_OUT_RANGE] = "Index out of range: %d >= %d";
vmsg[MSG_MISSING_FIELD] = "Missing field: %s";
vmsg[MSG_INDEX_UNAVAILABLE] = "Index unavailable: %d";
vmsg[MSG_ENTRY_UNAVAILABLE] = "Entry not available: %s/%s";
vmsg[MSG_INDEX_LT_ZERO] = "Index less than zero: %d";
vmsg[MSG_SEND_ABORT_BY_RECEIVER] = "Sending aborted by receiver";
vmsg[MSG_PROTOCOL] = "Communication protocol error: " MCS_SEP "%s" MCS_SEP;
vmsg[MSG_NOT_ENOUGH_SPACE] = "Not enough space in buffer: %d > %d";
vmsg[MSG_CANT_ALLOCATE_MYSQL] = "Can't allocate MySQL connection handler";
vmsg[MSG_CANT_OPEN_MYSQL_CONNECTION] = "Can't connect to MySQL server: %s";
vmsg[MSG_USE_ONLY_WITH_STORERESULT] = "%s can be used only with storeResult()";
vmsg[MSG_MYSQL_ERROR] = "MySQL: %s";
vmsg[MSG_INVALID_POSITION] = "Invalid position: %d";
vmsg[MSG_BIND_FAILED] = "Bind failed: %s";
vmsg[MSG_STMT_NOT_YET_PREPARED] = "Statement not yet prepared";
vmsg[MSG_EMPTY_STRING] = "Empty string";
vmsg[MSG_CANT_GET_INFO_TABLE] = "Cannot get info for table: %s";
vmsg[MSG_CANT_LOCK_TABLE] = "Cannot lock table %s insert";
vmsg[MSG_CANT_UNLOCK_TABLE] = "Cannot unlock tables";
vmsg[MSG_STMT_WRONG_NUM_PARAM] = "Wrong number of parameter in prepared statement";
vmsg[MSG_NO_RESULT] = "No result available";
vmsg[MSG_NO_FIELDS] = "No fields in result";
vmsg[MSG_FITS_ERROR] = "FITS: %s";
vmsg[MSG_FITS_NOT_COMPILED] = "CFITSIO feature hasn't been compiled";
vmsg[MSG_UNKNOWN_FILE_TYPE] = "Unknown file type: %s";
vmsg[MSG_NOT_LOGIN] = "You didn't log in";
vmsg[MSG_MYSQL_DISABLED] = "MySQL feature is not enabled";
vmsg[MSG_GRANTS_LOGIN] = "You're not allowed to log in.";
vmsg[MSG_GRANTS_DB_SCRIPT] = "You're not allowed to execute DB scripts.";
vmsg[MSG_GRANTS_SCRIPT] = "You're not allowed to execute shell scripts.";
vmsg[MSG_GRANTS_QUERY] = "You're not allowed to execute direct queries.";
vmsg[MSG_GRANTS_BATCH] = "You're not allowed to execute BATCH commands.";
vmsg[MSG_GRANTS_ADMIN] = "You're not allowed to use administrative commands.";
vmsg[MSG_GRANTS_GET] = "You're not allowed to use retrieve files.";
vmsg[MSG_GRANTS_CANTGET_GRANTS] = "Can't retrieve grants information.";
vmsg[MSG_GRANTS_SYS] = "You're not allowed to issue system commands";
vmsg[MSG_GRANTS_PUT] = "You're not allowed to upload files.";
vmsg[MSG_DB_ERR_NOTACTIVATED] = "DB facility is not active";
vmsg[MSG_DB_NOT_COMPILED] = "MySQL feature hasn't been compiled";
vmsg[MSG_SCRIPT_ERROR] = "Program quit with code" MCS_SEP "%d" MCS_SEP ", see err file";
vmsg[MSG_NO_MORE_CLIENT_ALLOWED] = "No more clients allowed";
vmsg[MSG_OBJECT_MISSING] = "Object missing";
vmsg[MSG_SIZE_CHUNK] = "Size of a chunk cannot be less than one: %d";
vmsg[MSG_ALREADY_CONNECTED] = "You already logged in.";
vmsg[MSG_EXT_BIN_UNEXPECTED] = "Program abnormal termination";
vmsg[MSG_NOT_CONNECTED] = "Socket not connected";
vmsg[MSG_EXT_NOT_AVAILABLE] = "External program or script not available: %s";
vmsg[MSG_CHARACTER_NOT_ALLOWED] = "Character not allowed: '%s'";
vmsg[MSG_CANT_SET_SYNCHRO_STATE] = "Can't set synchro state now";
vmsg[MSG_NOT_A_RANDOM_ACCESS_RECORDSET] = "Can't use method %s on a non random access recordset";
vmsg[MSG_ALREADY_AT_BEGIN_OF_FILE] = "Already at begin of file";
vmsg[MSG_ALREADY_AT_END_OF_FILE] = "Already at end of file";
vmsg[MSG_NO_METAREC] = "No meta information has been given";
vmsg[MSG_DONT_KNOW_NROWS] = "Don't know how many records in the set";
vmsg[MSG_NO_SAME_META_INFO] = "Records doesn't have the same meta structure";
vmsg[MSG_INSERT_NOT_ALLOWED] = "Can't insert records while in fetch mode";
vmsg[MSG_FETCH_ERROR] = "Error while fetching record: %s";
vmsg[MSG_INCOMPATIBLE_SERIAL] = "Incompatible Serializable objects: %d > %d";
vmsg[MSG_CANT_READ_CERT] = "Can't read certificate file: %s";
vmsg[MSG_CANT_READ_KEY] = "Can't read key file: %s";
vmsg[MSG_SSL_CONNECT_ERROR] = "SSL connect error";
vmsg[MSG_SSL_ACCEPT_ERROR] = "SSL accept error";
vmsg[MSG_UNKNOWN_USERID] = "Unknown user id: %d";
vmsg[MSG_YET_RUNNING] = "The thread is yet running";
vmsg[MSG_FILE_ALREADY_EXISTS] = "File already exists: %s";
vmsg[MSG_PIPE_YET_OPENED] = "Pipe is already opened, call \"closeRead()\" first.";
vmsg[MSG_CANT_EXECUTE_FILE] = "Can't execute file: %s";
vmsg[MSG_CANT_BIND_AND_HAVE_DIMSPEC] = "The object can't be bind and have a dim spec";
vmsg[MSG_TOO_MANY_DIMSPEC] = "Too many dim spec, max is %d";
vmsg[MSG_VARYING_MUST_BE_LAST] = "There can be only one dynamic dimension and it must be the last";
vmsg[MSG_NO_DIM_ALLOWED_TO_CHANGE] = "No dim is allowed to change";
vmsg[MSG_NO_ABSOLUTE_PATH] = "Absolute paths are not allowed: %s";
vmsg[MSG_NO_DOUBLEDOT_PATH] = "The \"..\" string is not allowed: %s";
vmsg[MSG_CANT_FREE_EXTERNAL_BUF] = "Cannot free an externally-handled buffer";
vmsg[MSG_NO_WINDOWS_SELECTED] = "No window has been selected";
vmsg[MSG_PIPE_NOT_CREATED] = "Pipe has not been created.";
vmsg[MSG_VOT_UNHANDLED_CODE] = "Unhandled code: %d";
vmsg[MSG_VOT_PARSE_FAILED] = "Parse failed on file: %s";
vmsg[MSG_VOT_CANT_OPEN_FILE] = "Can't open file: %s";
vmsg[MSG_VOT_UNEXPECTED_NODE_TYPE] = "Unexpected node type: %d";
vmsg[MSG_VOT_NOT_AN_INTEGER] = "Not an integer value: %s";
vmsg[MSG_VOT_INVALID_NODETYPE] = "Invalid element type: %s";
vmsg[MSG_VOT_INVALID_DATATYPE] = "Invalid Datatype: %s";
vmsg[MSG_VOT_INVALID_ENCODINGTYPE] = "Invalid EncodingType: %s";
vmsg[MSG_VOT_INVALID_RESOURCE_TYPE] = "Invalid Resource_type: %s";
vmsg[MSG_VOT_INVALID_FIELD_TYPE] = "Invalid Field_type: %s";
vmsg[MSG_VOT_INVALID_VALUES_TYPE] = "Invalid Values_Type: %s";
vmsg[MSG_VOT_INVALID_LINK_CONTENT_ROLE] = "Invalid Link_content_role: %s";
vmsg[MSG_VOT_INVALID_STREAM_TYPE] = "Invalid Stream_type: %s";
vmsg[MSG_VOT_INVALID_STREAM_ACTUATE] = "Invalid Stream_actuate: %s";
vmsg[MSG_VOT_INVALID_COOSYS_SYSTEM] = "Invalid Coosys_system: %s";
vmsg[MSG_VOT_INVALID_INDEX] = "Invalid index: %d";
vmsg[MSG_VOT_UNEXPECTED_ATTRIBUTE] = "Unexpected attribute: %s";
vmsg[MSG_VOT_ROOT_IS_NOT_VOTABLE] = "Root node is not a VOTABLE element: %s";
vmsg[MSG_CURL_ERROR] = "Curl error: %s";
vmsg[MSG_CURL_INIT] = "Cannot initialize CURL library";
}


