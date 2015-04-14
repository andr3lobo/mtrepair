#!/usr/bin/perl

# ImportingExecutingTestSet.pl <base-dir> <pool> <type> <#begin> <#end> <file-name> <session-name>

use POSIX qw(strftime);
use strict;

my $now_string = strftime "%a %b %e %H:%M:%S %Y", gmtime;

print <<EOF;
#####################################################
# Execution of $0 script
# at $now_string
#####################################################
EOF

if ($#ARGV != 6) {
	print "ERROR: seven arguments nedded...\n$0 <base-dir> <pool> <type> <#begin> <#end> <file-name> <session-name>\n";
	exit;
}

#print (join(",",@ARGV),"\n");

my $BASEDIR = $ARGV[0];
my $POOL = $ARGV[1];
my $TYPE = $ARGV[2];
my $begin = $ARGV[3];
my $end   = $ARGV[4];
my $PROG = $ARGV[5];
my $SESSION = $ARGV[6];

my $DIRTEST = "$BASEDIR/$PROG/$SESSION";

chdir $DIRTEST or die "Can't locate $DIRTEST\n";

#Importing test cases
if ( $TYPE eq "ascii" ) {
	print( "\nImporting test cases from ascii files\n" );
	system( "tcase -$TYPE -DD ./$POOL -I param -E $PROG.exe -EE ${PROG}_inst -trace -f $begin -t $end -v . $SESSION" );
} elsif ( $TYPE eq "poke" ) {
	print( "\nImporting test cases from poke-tool files\n" );
	system( "tcase -$TYPE -DD ./$POOL -E $PROG.exe -EE ${PROG}_inst -trace -f $begin -t $end -v . $SESSION" );
} else {
	print( "ERROR: wrong type. It should be ascii or poke.\n" );
	exit;
}

#Executing test cases
print( "\nExecuting test cases\n" );
system( "exemuta -exec -v . -trace $SESSION" );

#Generating testing report
print( "\nGenerating testing report\n" );
system( "report -tcase -L 511 -S $SESSION-$POOL-$begin-$end $SESSION" );
system( "report -trace -L 1 -S $SESSION-$POOL-$begin-$end-trace-L1 $SESSION" );
system( "report -trace -L 2 -S $SESSION-$POOL-$begin-$end-trace-L2 $SESSION" );

