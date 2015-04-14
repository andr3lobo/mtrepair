#!/usr/bin/perl

# CoverageByTestSet.pl <base-dir> <base-script-dir> <pool> <#begin> <#end> <file-name> <session-name>

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
	print "ERROR: seven arguments nedded...\n$0 <base-dir> <base-script-dir> <pool> <#begin> <#end> <file-name> <session-name>\n";
	exit;
}

#print (join(",",@ARGV),"\n");

my $BASEDIR = $ARGV[0];
my $SCRIPTDIR = $ARGV[1];
my $POOL = $ARGV[2];
my $begin = $ARGV[3];
my $end   = $ARGV[4];
my $PROG = $ARGV[5];
my $SESSION = $ARGV[6];

my $DIRTEST = "$BASEDIR/$PROG/$SESSION";

chdir $DIRTEST or die "Can't locate $DIRTEST\n";

if (! (-e "$PROG-gcov.exe") ) {
	system("gcc --coverage $PROG-gcov.c -o $PROG-gcov.exe -w");
}

#Running program for each test case
my $tc;

system("mkdir ../result");

print("Running program...\n");
for($tc = $begin; $tc <= $end; $tc++) {
	#Running
	if ( (-e "./$POOL/tec$tc.tes") ) {
		print("./$PROG-gcov.exe `cat ./$POOL/input$tc.tes` < ./$POOL/tec$tc.tes");
		system("./$PROG-gcov.exe `cat ./$POOL/input$tc.tes` < ./$POOL/tec$tc.tes");
	} else {
		print("./$PROG-gcov.exe `cat ./$POOL/input$tc.tes` < /dev/null");
		system("./$PROG-gcov.exe `cat ./$POOL/input$tc.tes` < /dev/null");
	}
	print(".");
}
print("Done running program.\n");

#Collecting coverage data
system("lcov -t \"Cobertura Test Set\" -o $PROG-gcov.info -c -d .");

#Generating report
system("genhtml -o report $PROG-gcov.info");

#Generating graph
#system("$SCRIPTDIR/PARSERsemINTERF/grafos $PROG-gcov");

#Moving files
system("mkdir ../result/0-ts");
system("mv report *.gcda *.info ../result/0-ts/");

print("\n");
