# ----------------------------------------------------------------------^
# Copyright (C) 2004, 2005, 2006, 2007 Giorgio Calderone <gcalderone@ifc.inaf.it>
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
# CONSTANTS
$FUNC    = "function";
$STRUCT  = "struct"  ;
$TYPEDEF = "typedef" ;

$lib = "";      #Global variable: name of the library

$typedefs = ""; #Global array: typedefs
$strudefs = ""; #Global array: structures defined
$enumdefs = ""; #Global array: enumerations defined, mapped to UINT


$funclist = "";
$prolist = "";
$dlmlist = "";


sub clean {
    my $l = shift(@_);
    $_ = $l;
    s/ +/ /g;
    s/\t+/ /g;
    s/\n//g;
    s/^ | $//g;
    return $_;
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


sub idlConvertFunc {
    my $l = shift(@_);
    my $pointer = checkPointer($l);
    $l = remStar($l);
    my $i;

    if ($pointer) {
	if ($l eq 'char') {
	    return 'STRING'; }
	else {
	    return 'ULONG';}
    }
    else {
	if ($l eq 'void') {
	    return 'LONG';
	}
	if ($l eq 'short int') {
	     return 'CONV_INT';
	}
	if ($l eq 'unsigned short int') {
	    return 'CONV_UINT';
	}
	if ($l eq 'short') {
	     return 'CONV_INT';
	}
	if ($l eq 'unsigned short') {
	    return 'CONV_UINT';
	}
	if ($l eq 'int') {
	    return 'LONG';
	}
	if ($l eq 'unsigned int') {
	    return 'ULONG';
	}
	if ($l eq 'long int') {
	    return 'LONG';
	}
	if ($l eq 'unsigned long int') {
	    return 'ULONG';
	}
	if ($l eq 'long') {
	    return 'LONG';
	}
	if ($l eq 'unsigned long') {
	    return 'ULONG';
	}
	if ($l eq 'long long int') {
	    return 'LONG64';
	}
	if ($l eq 'unsigned long long int') {
	    return 'ULONG64';
	}
	if ($l eq 'float') {
	    return 'FLOAT';
	}
	if ($l eq 'double') {
	    return 'DOUBLE';
	}
	if ($l eq 'char') {
	    return 'BYTE';
	}
	if ($l eq 'time_t') {
	    return 'ULONG';
	}
	else {
	    for ($i=0; $i<=$#enumdefs; $i++) {
		if ($l eq $enumdefs[$i]) {
		    return 'CONV_UINT';
		}
	    }
	    die "Unsupported type: $l|\n";
	}
    }
}





sub Ctype2IDLtype {
    my $l = shift(@_);
    my $pointer = checkPointer($l);
    $l = remStar($l);
    my $i;

    if ($pointer) {
	if ($l eq 'char') {
	    return 'STRING'; }

	if ($l eq 'void') {
	    return 'PTR'; }
    }
    else {
	if ($l eq 'short int') {
	     return 'INT';
	}
	if ($l eq 'unsigned short int') {
	    return 'UINT';
	}
	if ($l eq 'short') {
	     return 'INT';
	}
	if ($l eq 'unsigned short') {
	    return 'UINT';
	}
	if ($l eq 'int') {
	    return 'LONG';
	}
	if ($l eq 'unsigned int') {
	    return 'ULONG';
	}
	if ($l eq 'long int') {
	    return 'LONG';
	}
	if ($l eq 'unsigned long int') {
	    return 'ULONG';
	}
	if ($l eq 'long') {
	    return 'LONG';
	}
	if ($l eq 'unsigned long') {
	    return 'ULONG';
	}
	if ($l eq 'long long int') {
	    return 'LONG64';
	}
	if ($l eq 'unsigned long long int') {
	    return 'ULONG64';
	}
	if ($l eq 'float') {
	    return 'FLOAT';
	}
	if ($l eq 'double') {
	    return 'DOUBLE';
	}
	if ($l eq 'char') {
	    return 'BYTE';
	}
	if ($l eq 'time_t') {
	    return 'ULONG';
	}
	else {
	    for ($i=0; $i<=$#enumdefs; $i++) {
		if ($l eq $enumdefs[$i]) {
		    return 'UINT';
		}
	    }
	
	}
    }

    die "Ctype2IDLtype: Unsupported type: $l|\n";
}




sub IDLtype2UnionValue {
    my $l = shift(@_);

    if ($l eq 'BYTE') {
	return "c";
    }
    if ($l eq 'INT') {
	return "i";
    }
    if ($l eq 'UINT') {
	return "ui";
    }
    if ($l eq 'LONG') {
	return "l";
    }
    if ($l eq 'ULONG') {
	return "ul";
    }
    if ($l eq 'LONG64') {
	return "l64";
    }
    if ($l eq 'ULONG64') {
	return "ul64";
    }
    if ($l eq 'FLOAT') {
	return "f";
    }
    if ($l eq 'DOUBLE') {
	return "d";
    }
    if ($l eq 'STRING') {
	return "str";
    }
    if ($l eq 'PTR') {
	return "ptrint";
    }


    die "IDLtype2UnionValue: Unsupported type: $l|\n";
}



sub IDLtype2ReturnFunc {
    my $l = shift(@_);

    if ($l eq 'BYTE') {
	return "IDL_GettmpByte";
    }
    if ($l eq 'INT') {
	return "IDL_GettmpInt";
    }
    if ($l eq 'UINT') {
	return "IDL_GettmpUInt";
    }
    if ($l eq 'LONG') {
	return "IDL_GettmpLong";
    }
    if ($l eq 'ULONG') {
	return "IDL_GettmpULong";
    }
    if ($l eq 'LONG64') {
	return "IDL_GettmpLong64";
    }
    if ($l eq 'ULONG64') {
	return "IDL_GettmpULong64";
    }
    if ($l eq 'FLOAT') {
	return "IDL_GettmpFloat";
    }
    if ($l eq 'DOUBLE') {
	return "IDL_GettmpDouble";
    }
    if ($l eq 'STRING') {
	return "IDL_StrToSTRING";
    }
    if ($l eq 'PTR') {
	return "IDL_GettmpPtr";
    }


    die "IDLtype2ReturnFunc: Unsupported type: $l|\n";
}




sub CconvChar {
    my $l = shift(@_);
    my $pointer = checkPointer($l);
    $l = remStar($l);
    my $i;

    if ($pointer) {
	if ($l eq 'char') {
	    return '%s'; }
	else {
	    return '%d';}
    }
    else {
	if ($l eq 'void') {
	    return '%d';
	}
	if ($l eq 'float') {
	    return '%f';
	}
	if ($l eq 'double') {
	    return '%lf';
	}
	if ($l eq 'time_t') {
	    return '%ld';
	}
	else {
	    return '%d';
        }
    }
}


sub parseFunction {
    my $def = shift(@_);
    my @v = split(/\(/, $def);
    my $s = "";

    my $pars = $v[1]; #all parameters

    @v = split(/ +/, $v[0]);
    my $funcName = clean(pop(@v));
    my $funcType = clean(join(' ', @v));

    $_ = $pars;
    s/\)//g;
    s/ +/ /g;
    @v = split(/,/, $_);
    my $parcount = $#v + 1;

    my $parsType = "";
    my $parsName = "";
    my $i;

    for ($i=0; $i<$parcount; $i++) {
	my @vv = split(/ +/, $v[$i]);
	$parsName[$i] = clean(pop(@vv));
	$parsType[$i] = clean(join(' ', @vv));
    }

    print "FUNCTION: |$funcName|\n";
    print "RETURN:   |$funcType|\n";
    for ($i=0; $i<$parcount; $i++) {
	print "PAR $i: |$parsType[$i]|  |$parsName[$i]|\n";
    }
    print "\n\n";

    #C wrapper

    #Variable to be returned, if any
    $CretType = $funcType;
    if ($CretType eq 'void') {
	print OutSrc "static void idl_$funcName(int argc, IDL_VPTR *argv) {\n";
	push @prolist, "{ (IDL_SYSRTN_GENERIC) idl_$funcName, \"" . uc($funcName) . "\", $parcount, $parcount, 0, 0}";
	$dlmlist .= "PROCEDURE " . uc($funcName) . " $parcount $parcount\n";
    }
    else {
	print OutSrc "static IDL_VPTR idl_$funcName(int argc, IDL_VPTR *argv) {\n";
        print OutSrc "  static IDL_VARIABLE idlret;\n";
	print OutSrc "  static $CretType ret;\n";
	push @funclist, "{idl_$funcName, \"" . uc($funcName) . "\", $parcount, $parcount, 0, 0}";
	$dlmlist .= "FUNCTION " . uc($funcName) . " $parcount $parcount\n";
    }

    print OutSrc "  static IDL_VPTR p[$parcount];\n";
    print OutSrc "  static int i;\n\n";

    #Check number of arguments
    print OutSrc "  if (argc != $parcount) {\n";
    print OutSrc "    snprintf(msg, MSG_LEN, wrongnum, $parcount);\n";
    print OutSrc "    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);\n";
    print OutSrc "  }\n\n";


    #Check that each argument is a scalar
    #for ($i=0; $i<$parcount; $i++) {
    #	$idltype = Ctype2IDLtype($parsType[$i]);
    #	print OutSrc "  if ((argv[$i]->type != IDL_TYP_$idltype)   ||\n";
    #	print OutSrc "      (argv[$i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT))) {\n";
    #	print OutSrc "    snprintf(msg, MSG_LEN, wrongpar, $i, \"$idltype\");\n";
    #	print OutSrc "    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);\n";
    #	print OutSrc "  }\n";
    #}

    print OutSrc "  for (i=0; i<$parcount; i++) \n";
    print OutSrc "    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {\n";
    print OutSrc "      snprintf(msg, MSG_LEN, wrongpar, i, \"$idltype\");\n";
    print OutSrc "      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);\n";
    print OutSrc "    }\n";


    #Conversion of argument to correct type
    print OutSrc "\n";
    for ($i=0; $i<$parcount; $i++) {
	$l = "IDL_TYP_" . Ctype2IDLtype($parsType[$i]);
	print OutSrc "  p[$i] = (argv[$i]->type == $l ? argv[$i] : IDL_BasicTypeConversion(1, &(argv[$i]), $l));\n";
    }
    print OutSrc "\n";


    if ($CretType eq 'void') {
	print OutSrc "  ";
    }
    else {
	print OutSrc "  ret = "
    }

    print OutSrc "$funcName(";
    for ($i=0; $i<$parcount; $i++) {
	if ($i > 0) { print OutSrc ",\n            "; }
	print OutSrc "($parsType[$i]) ";

	if (Ctype2IDLtype($parsType[$i]) eq "STRING") {
	    print OutSrc "IDL_STRING_STR(&(";
	}

	print OutSrc "p[$i]->value.";
	print OutSrc IDLtype2UnionValue(Ctype2IDLtype($parsType[$i]));

	if (Ctype2IDLtype($parsType[$i]) eq "STRING") {
	    print OutSrc "))";
	}
    }
    print OutSrc ");\n\n";


    #Eventually free temporary variables
    print OutSrc "  for (i=0; i<$parcount; i++) \n";
    print OutSrc "    if (p[i] != argv[i])\n";
    print OutSrc "      IDL_Deltmp(p[i]);\n\n";

    #print OutSrc "  if (ifd_got_error()) {\n";
    #print OutSrc "    snprintf(msg, MSG_LEN, mcserr, ifd_last_error());\n";
    #print OutSrc "    IDL_Message(IDL_M_GENERIC, IDL_MSG_INFO, msg);\n";
    #print OutSrc "  }\n\n";


    if ($CretType ne 'void') {
	$idlrettype = Ctype2IDLtype($CretType);
	
	if ($idlrettype eq "STRING") {
	    print OutSrc "  idlret.type = IDL_TYP_STRING;\n";
	    print OutSrc "  idlret.flags = IDL_V_DYNAMIC;\n";
	    print OutSrc "  IDL_StrStore(&(idlret.value.str), ret);\n";
	}
	else {
	    print OutSrc "  idlret.type = IDL_TYP_$idlrettype;\n";
	    print OutSrc "  idlret.flags = 0;\n";
	    print OutSrc "  idlret.value." . IDLtype2UnionValue($idlrettype) . " = ";

	    if ($idlrettype eq "PTR") {
		print OutSrc "  (IDL_PTRINT) ";
	    }
	    print OutSrc "  ret;\n";
	}
	print OutSrc "  return &idlret;\n";
    }
    print OutSrc "}\n\n\n";
}


sub parseStruct {
    my $def = shift(@_);
    my @v = split(/{|}/, $def);

    $structName = shift(@v);
    $memb = clean(join(' ', @v));
    @v = split(/;/, $memb);

    my $membcount = $#v + 1;
    my $smType = "";
    my $smName = "";
    my $i;
    for ($i=0; $i<$membcount; $i++) {
        my @vv = split(/ +/, $v[$i]);
        $smType[$i] = clean(pop(@vv));
        $smName[$i] = clean(join(' ', @vv));
    }

    print "STRUCT: |$structName|\n";
    for ($i=0; $i<$membcount; $i++) {
	print "FIELD $i: |$smName[$i]|  |$smType[$i]|\n";
    }
    print "\n\n";

}

sub parseTypedef {
    my $def = shift(@_);
    my @v = split(/ +/, $def);
    my $typeType = clean(pop(@v));
    my $typeName = clean(pop(@v));
    print "TYPEDEF: |$typeType| --> |$typeName|\n\n";
}


sub parseEnum {
    my $def = shift(@_);
    my @v = split(/ +/, $def);
    my $enumName = shift(@v);
    my $enumMemb = join(' ', @v);
    @enumMemb = split(/, +/, $enumMemb);
    my $membcount = $#enumMemb ;
    print "ENUM: $enumName {\n";
    for ($i=0; $i<=$membcount; $i++) {
	$_ = $enumMemb[$i];
	s/\{//g;
	s/\}//g;
	$enumMemb[$i] = clean($_);
	print "  $enumMemb[$i] \n";
    }
    print "}\n\n";
    @enumdefs[++$#enumdefs] = $enumName;

    for ($i=0; $i<=$membcount; $i++) {
	$funcName = $enumName . "_" . $enumMemb[$i];

        print OutSrc "static IDL_VPTR idl_$funcName(int argc, IDL_VPTR *argv) {\n";
	print OutSrc "  static IDL_VARIABLE idlret = { IDL_TYP_UINT, 0, 0, $i};\n";
	print OutSrc "  return &idlret;\n}\n\n";

        push @funclist, "{idl_$funcName, \"" . uc($funcName) . "\", 0, 0, 0, 0}";
        $dlmlist .= "FUNCTION " . uc($funcName) . " 0 0\n";
    }
}


sub parseLine {
    my $instr = clean(shift(@_));

    my @v = split(/ +/, $instr);

    my $spec = shift(@v);
    my $def = join(' ', @v);

    my $i = rindex($dev, ';');
    if ($i == length($dev)-1) {
    	$def = substr($def, 0, $i);
    }
	
    print "$spec $def \n";
    if ($spec =~ /function */) {
	parseFunction($def);
    }
    elsif ($spec =~ /struct */) {
	parseStruct($def);
    }
    elsif ($spec =~ /typedef */) {
	parseTypedef($def);
    }
    elsif ($spec =~ /enum */) {
	parseEnum($def);
    }
    #else  {
    #	print  "Unknown specifier: $spec";
    #}
}


sub countBracket {
    my $l = shift(@_);
    my $count = 0;
    my $p = 0;
    $p = index($l, '{', 0);
    while ($p >= 0) {
	$p = index($l, '{', $p+1);
	$count++;
    }

    $p = 0;
    $p = index($l, '}', 0);
    while ($p >= 0) {
	$p = index($l, '}', $p+1);
	$count--;
    }
    return $count;
}



sub parse {
    my $in = shift(@_);
    my $v;
    my $bracket = 0;
    my $toParse = "";
    my $i;

    while (<$in>) {
	chomp;
	if ($_ ne "") {
	    s/\/\/.*//g;     #one line comments
	    s/\t+/ /g;       #multiple tabs
	    s/;+/;/g;        #multiple ;
	    s/; +/;/g;       # ; with blanks
	    s/\t+/ /g;       #tab to blank
	    s/ +/ /g;        #multiple blanks
	    s/^ +| +$//g;    #leading and trailing blanks
	    s/\n//g;         #newlines
	    s/^ +| +$//g;    #leading and trailing blanks
	    s/^ | $//g;      #leading and trailing blanks, again
	
	    $v = clean($_);
	    $bracket += countBracket($v);
	    $toParse = $toParse . $v . ' ';
	    if (($bracket == 0)   &&   (index($v, ';') >=0)) {
		parseLine($toParse);
		$toParse = "";
	    }
	}
    }
}



# ---- MAIN ----
$title = 'c2idl - C to IDL interface builder, ver. 0.3.3-alpha7' .
    "\nGiorgio Calderone - gcalderone\@ifc.inaf.it\n\n";

$head = "";
$outf = "";

print $title;

foreach $v (@ARGV) {
    if (($v eq "--ver")  ||  ($v eq "-v")) {
	exit;
    }
    if (($v eq "--help")  ||  ($v eq "-h")) {
	print "Usage: $0 <header> <library>";
	print "";
	print "-h | --help,    shows this usage message";
	print "-v | --ver,     shows number version";
	print "";
	print "<header> is the header file of the library,";
	print "<library> is the library which contains the functions,";
	print "[output file] is the name of output file, if missing <header>.pro will be used" ;
	exit;
    }
    if ($v =~ /^\-+/)      { die "Unknown option: $v"; }
    else {
	if ($head eq "")       { $head       = $v; next; }
	if ($lib eq "")        { $lib        = $v; next; }
	else                   { $outf       = $v; next; }
    }
}


if ($head eq ""   ||   $lib eq "")  {
    die  "Header or library file name is missing. Try $0 --help";
}


$_ = $lib;
s/\.a|\.so//g;
$libname = $_;

$outfsrc = "$libname.c";
$dlmfsrc =  "$libname.dlm";

open($in, "$head") or die "Can't open file for input: $head";

open(OutSrc, "> $outfsrc") or die "Can't open file for output: $outfsrc";
open(DlmSrc, "> $dlmfsrc") or die "Can't open file for output: $dlmfsrc";




print OutSrc "#include <incl.h>\n";
print OutSrc "#include <stdlib.h>\n";
print OutSrc "#include <idl_export.h>\n";
print OutSrc "\n";

print OutSrc "#define MSG_LEN 80\n";
print OutSrc "static char msg[MSG_LEN];\n";
print OutSrc "static const char* wrongnum = \"Wrong number of parameters, should be %d, got %d\";\n";
print OutSrc "static const char* wrongpar = \"Wrong type for parameter %d, should be %s\";\n";
#print OutSrc "static const char* mcserr   = \"MCS: %s\";\n";
print OutSrc "\n";
print OutSrc "\n";


parse $in;


@prolist  = sort {uc($a) cmp uc($b)} @prolist;
@funclist = sort {uc($a) cmp uc($b)} @funclist;

print OutSrc "int IDL_Load(void) {\n";
print OutSrc "  static IDL_SYSFUN_DEF2 function_addr[] = {\n";
foreach $l (@funclist) {
    print OutSrc "$l, \n";
}
print OutSrc "};\n\n";
print OutSrc "  static IDL_SYSFUN_DEF2 procedure_addr[] = {\n";
foreach $l (@prolist) {
    print OutSrc "$l, \n";
}
print OutSrc "};\n\n";

print OutSrc "  return IDL_SysRtnAdd(function_addr, TRUE, IDL_CARRAY_ELTS(function_addr))\n";
print OutSrc "      && IDL_SysRtnAdd(procedure_addr, FALSE, IDL_CARRAY_ELTS(procedure_addr));\n}\n";


print DlmSrc "MODULE $libname\n";
print DlmSrc "DESCRIPTION Interface to the MCS library\n";
print DlmSrc "VERSION 0.3.3-alpha7\n";
print DlmSrc "SOURCE Giorgio Calderone (gcalderone\@ifc.inaf.it)\n";
print DlmSrc $dlmlist;


close OutSrc;
close DlmSrc;
