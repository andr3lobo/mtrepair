#!/usr/bin/perl

# DeadLiveMutantsByTestCase.pl <base-dir> <base-script-dir> <#begin> <#end> <file-name> <session-name>

use POSIX qw(strftime);
use strict;

my $now_string = strftime "%a %b %e %H:%M:%S %Y", gmtime;

print <<EOF;
#####################################################
# Execution of $0 script
# at $now_string
#####################################################
EOF

if ($#ARGV != 5) {
	print "ERROR: six arguments nedded...\n$0 <base-dir> <base-script-dir> <#begin> <#end> <file-name> <session-name>\n";
	exit;
}

#print (join(",",@ARGV),"\n");

my $BASEDIR = $ARGV[0];
my $SCRIPTDIR = $ARGV[1];
my $begin = $ARGV[2];
my $end   = $ARGV[3];
my $PROG = $ARGV[4];
my $SESSION = $ARGV[5];

my $DIRTEST = "$BASEDIR/$PROG/$SESSION";

chdir $DIRTEST or die "Can't locate $DIRTEST\n";

#Disabling all test cases
system("tcase -i $SESSION");
system( "exemuta -exec -trace $SESSION" );

#Running program for each test case
my $tc;

for($tc = $begin; $tc <= $end; $tc++) {
	#Enabling a single test case
	system("tcase -e -f $tc -t $tc $SESSION");
	system( "exemuta -exec -trace $SESSION" );

	#Generating report
	system( "report -tcase -L 511 -S $SESSION-tc-$tc $SESSION" );

	#Collecting the number of live mutants
	system( "muta -l $SESSION | $SCRIPTDIR/vivos > tc-$tc-vivos" );

	#Collecting the number of dead mutants
	system( "muta -l $SESSION | $SCRIPTDIR/mortos > tc-$tc-mortos" );

	#Disabling all test case
	system("tcase -i $SESSION");
	system( "exemuta -exec -trace $SESSION" );

	#Moving files
	system( "mv tc-$tc-vivos tc-$tc-mortos $SESSION-tc-$tc* ../result/tc-$tc" );
	print(".");
}

#Enabling all test case
system("tcase -e -f $begin -t $end $SESSION");
system( "exemuta -exec -trace $SESSION" );

print("\n");
