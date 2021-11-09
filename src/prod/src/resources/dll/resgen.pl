use strict;
use warnings;

my $in = shift(@ARGV);
my $out = shift(@ARGV);

open (my $if, "<", $in) or die "can't read from $in: $!";
open (my $of, ">", $out) or die "can't write to $out: $!";

print $of "//------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------

// *** DO NOT EDIT: This file was generated by \"resgen.pl\" ***

#include <windows.h>
#include <inc\\resourceids.h>
#include \"fabricverp.h\"

#include \"resources.h\"

#include \"common.ver\"
#include \"Microsoft-ServiceFabric-Events.rc\"
#include \"WFPerf.rc\"

#pragma code_page(1252)

STRINGTABLE LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_US
BEGIN
";

while (<$if>)
{
    if (/^#define\s*(IDS_\w*).*?\/\/\s*(.*)$/) 
    { 
        print $of "    $1 L\"$2\"\n"; 
    }
    elsif (/^#define\s*IDS_.*RESOURCE_ID/)
    {
        # intentional no-op for recognized component definition
    }
    elsif (/^#define\s*IDS_/)
    {
        # looks like a resource definition but doesn't match
        # the expected pattern - it's probably an error
        die "invalid resource definition: \"$_\"";
    }
}

print $of "END";
