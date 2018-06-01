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


if ($#ARGV != 0) { die; }

my $arch_64bit = 0;
if ($ARGV[0] =~ /64/ || $ARGV[0] =~ /darwin/) {  # Mac OS assumed 64bit
    $arch_64bit = 1;
}
shift(@ARGV);

my $syslen_mult = 1;
if ($arch_64bit) {
    $syslen_mult = 2;
}


sub FortranIfy {
    my $l = shift(@_);
    chomp $l;
    s/ +$//g;

    my $ret = "";
    while (length($l) > 60) {
	$ret .= substr($l, 0, 60) . "\n     .";
	$l = substr($l, 60);
    }
    $ret .= $l;
    return $ret;
}


sub checkPointer {
    my $l = shift(@_);
    my $p = index($l, '*', 0);
    if ($p >= 0) {
	return 1; }
    else {
	return 0; }
}

sub remStar {
    my $l = shift(@_);
    $_ = $l;
    s/\*//g;
    return $_;
}


sub printLen {
    my $l = shift(@_);
    my $a = sprintf("%d", $l * $syslen_mult);
    return $a;
}


sub FortranType {
    my $l = shift(@_);
    my $pointer = checkPointer($l);
    $l = remStar($l);
    my $i;

    if ($pointer) {
	if ($l eq 'char') {
	    return 'CHARACTER*(*)'; }
	else {
	    return "INTEGER*" . printLen(4); }
	}
    else {
	if ($l eq 'void') {
	    die;
	}
	if ($l eq 'short int') {
	     return 'INTEGER*4';
	}
	if ($l eq 'unsigned short int') {
	    return 'INTEGER*4';
	}
	if ($l eq 'short') {
	     return 'INTEGER*4';
	}
	if ($l eq 'unsigned short') {
	    return 'INTEGER*4';
	}
	if ($l eq 'int') {
	    return 'INTEGER*4';
	}
	if ($l eq 'unsigned int') {
	    return 'INTEGER*8';
	}
	if ($l eq 'long int') {
	    return 'INTEGER*4';
	}
	if ($l eq 'unsigned long int') {
	    return 'INTEGER*8';
	}
	if ($l eq 'long') {
	    return 'INTEGER*4';
	}
	if ($l eq 'unsigned long') {
	    return 'INTEGER*8';
	}
	if ($l eq 'long long int') {
	    return 'INTEGER*8';
	}
	if ($l eq 'unsigned long long int') {
	    return 'INTEGER*8';
	    #die; #This is because unsigned integers in 8 bytes are not
	         #present in any language
	}
	if ($l eq 'float') {
	    return 'REAL*4';
	}
	if ($l eq 'double') {
	    return 'REAL*8';
	}
	if ($l eq 'char') {
	    return 'INTEGER*4';
	}
	if ($l eq 'time_t') {
	    return 'INTEGER*4';
	}
	if ($l eq 'Types') {
	    return 'INTEGER*4';
	}
	else {
	    die "Unsupported type: $l\n";
	}
    }
}




sub CType {
    my $l = shift(@_);
    my $pointer = checkPointer($l);
    $l = remStar($l);
    my $i;

    if ($pointer) {
	if ($l eq 'char') {
	    return 'char*'; }
	else {
	    return 'int*'; }
    }
    else {
	if ($l eq 'void') {
	    die;
	}
	if ($l eq 'short int') {
	     return 'int';
	}
	if ($l eq 'unsigned short int') {
	    return 'int';
	}
	if ($l eq 'short') {
	     return 'int';
	}
	if ($l eq 'unsigned short') {
	    return 'int';
	}
	if ($l eq 'int') {
	    return 'int';
	}
	if ($l eq 'unsigned int') {
	    return 'long long int';
	}
	if ($l eq 'long int') {
	    return 'int';
	}
	if ($l eq 'unsigned long int') {
	    return 'long long int';
	}
	if ($l eq 'long') {
	    return 'int';
	}
	if ($l eq 'unsigned long') {
	    return 'long long int';
	}
	if ($l eq 'long long int') {
	    return 'long long int';
	}
	if ($l eq 'unsigned long long int') {
	    return 'unsigned long long int'; #die;
	}
	if ($l eq 'float') {
	    return 'float';
	}
	if ($l eq 'double') {
	    return 'double';
	}
	if ($l eq 'char') {
	    return 'int';
	}
	if ($l eq 'time_t') {
	    return 'int';
	}
	if ($l eq 'Types') {
	    return 'int';
	}
	else {
	    die "Unsupported type: $l\n";
	}
    }
}


print "#include <stdlib.h>\n";
print "#include <string.h>\n";
print "#include <time.h>\n\n";
print "#include <mcs_c.h>\n\n";

print "static char* String_F2C(char* ss, int len)\n";
print "{\n";
print "    unsigned int i;\n";
print "    char* p = malloc(len + 1);\n";
print "    p[len] = 0;\n";
print "    memcpy(p, ss, len);\n";
print "    if (len > 0) {\n";
#Trim all trailing blanks
print "      for (i=len-1; i>=0; i--) {\n";
print "        if (p[i] != ' ')  break;\n";
print "        else p[i] = 0;\n";
print "      }\n";
print "    }\n";
print "    return p;\n";
print "}\n";
print "\n";
print "\n";
print "static int String_C2F(char* src, char*dst, int len)\n";
print "{\n";
print "    int i;\n";
print "\n";
print "    for (i=0; i<len; i++ )\n";
print "	       if (! src[i]) break;\n";
print "        else dst[i] = src[i];\n";
print "\n";
print "    for (; i<len; i++)\n";
print "	       dst[i] = ' ';\n";
print "\n";
print "    return 1;\n";
print "}\n";
print "\n";
print "\n";


open ($outf, "> mcs_fortran.inc");
open ($outffacility, "> mcs_facility.inc");

$pre = "      ";


while (<>) {
    s/\t+/ /g;       #multiple tabs
    s/;+/;/g;        #multiple ;
    s/; +/;/g;       # ; with blanks
    s/^ +| +$//g;    #leading and trailing blanks
    s/ +/ /g;        #multiple blanks
    s/\n//g;         #newlines
    s/^ | $//g;      #leading and trailing blanks, again



    if ($_ =~ /^function.*/) {
	s/^function *//g;
	$line = $_;
	
	@v = split(/\(/, $line);
	$pars = $v[1];
	$_= $pars;
	s/\);//g;
	s/^ +| +$//g;
	@pars = split(/ *, */, $_);

	@v = split(/ +/, $v[0]);

	$funcName = pop(@v);
	$retType = join(' ', @v);
	


	#Print the function template
	print "//$retType $funcName(" . join(', ', @pars) . ");\n";


	#All functions are void, return values are passed by
	#reference as last parameter
	print "void wf77_" . lc($funcName)  . "_(";
	
	#List of parameters
	for ($i=0; $i<=$#pars; $i++) {
	    $par = $pars[$i];
	    @v = split(/ +/, $par);
	    $parName[$i] = pop(@v);
	    $parName[$i] =~ s/^_//g;

	    $parType[$i] = join(' ', @v);  #print $parType[$i] . "|\t\t|" . $parName[$i] . "|\n";

	    if ($i > 0) { print ", "; }

	    if ($parType[$i] eq "char*") {
		print CType($parType[$i]) . " " . $parName[$i];
		print ", int* $parName[$i]_flen";
	    }
	    else {
		print CType($parType[$i]) . "* " . $parName[$i];
	    }
	}
	
	#Add an extra parameter for the return value
	if ($retType ne 'void') {
	    if ($#pars >= 0) { print ", "; }		
	    if ($retType eq "char*") {
		print "$retType RET, int* RET_len";
	    }
	    else {
		print CType($retType) . "* RET";
	    }
	}


	#End of declaration, function body begins
	print  ") {\n";

	#Handle parameter strings
	$tobeFreed = "";
	for ($i=0; $i<=$#pars; $i++) {
	    if ($parType[$i] eq "char*") {
		print "  char* C_$parName[$i] = " .
		    "String_F2C($parName[$i], *$parName[$i]_flen);\n";
		
		$tobeFreed .= "  free(C_$parName[$i]);\n";
	    }
	}


	#Prepare the return value
	if ($retType ne "void") {
	    if ($retType eq "char*") {
		print "  char* RETSTR = ";
	    }
	    else {
		if ($retType eq "IFD_STATUS*") {
                    print "  *RET = (int *) ";
                }
                else {
                    print "  *RET = ";
                }
	    }
	}
		

	#Call the C function
	print "  $funcName(";
	for ($i=0; $i<=$#pars; $i++) {
	    if ($i > 0) { print ", "; }


	    if ($parType[$i] eq "char*") {
		print "C_$parName[$i]";
	    }
	    else {
		print "* (($parType[$i]*) $parName[$i])";
	    }
	}
	print ");\n";
	
	#Eventually free memory
	print $tobeFreed;
	

	#Eventually the string return value
	if ($retType eq "char*") {
	    if ($funcName ne "ifd_last_error") {
		# Thread safe version:  print "  if (! ifd_got_error(* ((IFD_STATUS**) st)))\n";
		print "  if (! ifd_got_error())\n";
		print "    String_C2F(RETSTR, RET, *RET_len);\n";
		print "  else\n";
		print "    String_C2F(\"\", RET, *RET_len);\n";
	    }
	    else {
		print "  String_C2F(RETSTR, RET, *RET_len);\n";
	    }
	}
	print "}\n\n\n";



	#Now write the FORTRAN wrapper
	$line = "";
	if ($retType eq "void") {
	    $line .= $pre . "SUBROUTINE ";
	}
	else {
	    $line .= $pre . FortranType($retType) . " FUNCTION ";
	}
	$line .= $funcName . "(";
	for ($i=0; $i<=$#pars; $i++) {
            if ($i > 0) { $line .= ", "; }
	
	    $line .= $parName[$i];
	}

	$line .= ")";
	print $outf FortranIfy($line) . "\n";
	
	#Argument declaration
	for ($i=0; $i<=$#pars; $i++) {
	    $line = $pre . FortranType($parType[$i]) . " $parName[$i]";
	    print $outf FortranIfy($line) . "\n";
	}


	#Call the C function wf77_*
	$line = $pre . "CALL ";
		
	$line .= "wf77_$funcName(";
        for ($i=0; $i<=$#pars; $i++) {
            if ($i > 0) { $line .= ", "; }

            $line .= $parName[$i];

	    if ($parType[$i] eq 'char*') {
		$line .= ", LEN($parName[$i])";
	    }
        }

	if ($retType ne "void") {
	    if ($#pars >= 0) { $line .= ", "; }
	    $line .= "$funcName";

	    if ($retType eq 'char*') {
		$line .= ", LEN($funcName)";

		#We write this because these functions are decalared
		#as: CHARACTER*(*) , so we must give them a length,
		#otherwise this must be done by the user.
		print $outffacility
		    FortranIfy($pre . "CHARACTER*200 $funcName")  . "\n";
	    }
	    else {
		#We write this because when the user calls these
		#functions their return type will be assumed depending
		#on the initial letter.
		print $outffacility
                    FortranIfy($pre . FortranType($retType) .
			       " $funcName")  . "\n";
	    }
	}

	$line .= ")";
	print $outf FortranIfy($line) . "\n";
	print $outf $pre . "RETURN\n";
	print $outf $pre . "END\n\n\n\n";
    }
    else {
	if ($_ =~ /^enum.*/) {
	    $line = $_;
	    $first = 0;
	    if ($line =~ /enum +([\w]+) *{/) {
		$enumName = $1;
		
		$line = "";
		@v = ();
		while (<>) {
		    if ($_ =~ /}/) { last; }
		    chomp;
		    s/^ +| +$//g;
		    s/ +/ /g;
		    s/^ | $//g;
   		    push @v, $_;
		    $line .= $_;
		    #print stderr "----------------------------: $#v $_\n";
 	        }

		$tmp = $pre . "INTEGER $line";
		print $outffacility FortranIfy($tmp) . "\n\n";
		
		$i=0;

		$tmp = $pre . "PARAMETER(";
		foreach $val (@v) {
		    $val =~ s/ *//g;
		    $val =~ s/,//g;
		
		    $tmp .= "$val = $i,";
		    $i++;
		}
		$tmp =~ s/,$//;
		$tmp .= ")";
		print $outffacility FortranIfy($tmp) . "\n";
	    }
	}
    }
}


#if ($syslen_mult > 2) {
#    print $outf "#define PTR INTEGER*4\n";
#}
#else {
#    print $outf "#define PTR INTEGER*8\n";
#}

close $outf;
