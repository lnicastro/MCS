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
<?php

require("php2mcs.php");

function check_errors() {
    if (ifd_got_error()) {
	print "ERROR: " . ifd_last_error() . "\n";
	exit(1);
    }
}


function print_msgs($cli) {
    check_errors();
    while ( (! ifd_got_error())   and
	    (Record_count(Client_msg($cli)) > 0)) {

	print " --> " .
	    Data_sval(
		Record_pop(
		    Client_msg($cli),
                0)
            )
	    . "\n";
    }
    Record_clear(Client_msg($cli));
}




$null = ifd_null();

print 'Testing class DBConn...\n';
$p = new_DBConn(ifd_null());
print 'DB connection is open: ' . DBConn_isOpen($p) . "\n";

print 'Connecting... \n ';
DBConn_connect($p, 'mcstest', 'mcstest', 'test', '');
print 'DB connection is open: ' . DBConn_isOpen($p) . "\n";
check_errors();

print 'Testing class Query...';
$dbq = new_Query(ifd_null(), $p, 0);

$query = "SELECT * FROM mcstest";
print "Preparing query: " . $query . "\n";
Query_prepare($dbq, $query);

print "Executing query...\n";
Query_execute($dbq, 1);

print "Number of returned rows: " . RecordSet_nRows($dbq) . "\n";
print "Number of affected rows: " . Query_nAffectedRows($dbq) . "\n";

Query_close($dbq);

check_errors();
ifd_reset_error();
del_Query($dbq);

ifd_reset_error();
del_DBConn($p);






print "Creating a Client object:\n";
$cnf = new_Conf(ifd_null(), "mcs.conf");
$port = Conf_ival($cnf, "test", "PORT");
$ssl = Conf_ival($cnf, "test", "use_ssl");
$cli = new_Client(ifd_null(), "", "localhost", $port, $ssl, 60000);


print "Login...\n";
if (Client_login($cli, "mcstest", "mcstest", "test")) {
  print "OK!\n";
}
else {
  print "Failed!\n";
  return;
}


print "Execute a query: SELECT * FROM mcstest\n";
$cmd = "QRY SELECT * FROM mcstest";
if (! Client_exec($cli, $cmd, $null)) {
    print_msgs($cli);
    return;
}

print "print messages received 'til now\n";
print_msgs($cli);

#Retrieve number of rows
$nrows = RecordSet_nRows($cli);

#Print all result from query
for ($i=0; $i<$nrows;  $i++) {
  #Retrieve $i-th record
  RecordSet_setPos($cli, $i);
  $rec = RecordSet_rec($cli);

  if ($i == 0) { #Print head
    print "\n\nRecord details:\n";
    for ($j=0; $j<Record_count($rec); $j++) {
      if (Data_type(Record_field($rec, $j)) != BLOB) {
        print Data_name(Record_field($rec, $j)) . "\t";
      }
    }
    print "\n";
  }

  for ($j=0; $j<Record_count($rec); $j++) {
    if (Data_type(Record_field($rec, $j)) != BLOB) {
      print Data_sval(Record_field($rec, $j)) . "\t";
    }
  }
  print "\n";
}
print "\n";

print_msgs($cli);

ifd_reset_error();
print "Deleting Client object\n";
del_Client($cli);

return;

?>
