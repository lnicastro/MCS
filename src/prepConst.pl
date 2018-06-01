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

$errmax = 0;
$msgmax = 0;

warn "#include \"mcs.hh\"\n";
warn "void mcs::Event::init_vmsg() {\n";

while (<>) {
    chomp;
    s/\t+/ /g;
    if ($_ =~ /^\/\//) {
	;
    }
    else {
	@vec = split(' ');

	$name = shift(@vec);
	$ival = shift(@vec);
	$sval = sprintf("%03d", $ival);
	$msg  = join(' ', @vec);
	
	if ($ival < 100) {
	    if ($ival > $msgmax) {
		$msgmax = $ival;
	    }
	}
	else {
	    if ($ival > $errmax) {
		$errmax = $ival;
	    }
	}

	if ($msg =~ /\w|\"/) {
	    print "#define MSG_$name   $ival\n";
	    warn "vmsg[MSG_$name] = $msg;\n";
	}
    }
}
warn "}\n\n\n";
print "#define MCS_LAST_MESSAGE_CODE $msgmax\n";
print "#define MCS_LAST_ERROR_CODE $errmax\n";

