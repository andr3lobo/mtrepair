#!/usr/bin/perl

# CreatingTestSession.pl <base-dir> <type-session> <file-name> <session-name>

use POSIX qw(strftime);
use strict;

my $now_string = strftime "%a %b %e %H:%M:%S %Y", gmtime;

print <<EOF;
#####################################################
# Execution of $0 script
# at $now_string
#####################################################
EOF

if ($#ARGV != 3) {
	print "ERROR: four arguments nedded...\n$0 <base-dir> <type-session> <file-name> <session-name>\n";
	exit;
}

my $BASEDIR = $ARGV[0];
my $TYPE = $ARGV[1];
my $PROG = $ARGV[2];
my $SESSION = $ARGV[3];

my $DIRTEST = "$BASEDIR/$PROG/$SESSION";

chdir $DIRTEST or die "Can't locate $DIRTEST\n";

if (! (-e "$PROG.exe") ) {
	system("gcc $PROG.c -o $PROG.exe -w");
}

#Creating the test session
print ( "\nCreating a test session of type $TYPE\n" );
system( "test-new -$TYPE -S $PROG -E $PROG.exe -C \"gcc $PROG.c -o $PROG.exe -w\" $SESSION" );

#Instrumenting and compiling the original program
print( "\nInstrumenting and compiling the original program\n" );
system( "instrum -EE $SESSION __$PROG" );
system( "gcc __${PROG}_inst.c __${PROG}_pp.c -o ${PROG}_inst -w -I$ENV{PROTEUMIMHOME}" );

# Creating all mutants
print ( "\nCreating all mutants\n" );
system( "muta-gen -u- 100 0 $SESSION" );

# Checking the equivalent mutants
if (-e "$PROG-equivs") {
	print ( "\nChecking equivalent mutants\n" );
	open(FD, "< $PROG-equivs");
	my $cont = 0;
	while ( <FD> ){
		m/(\d+)/;
		print (".");
		system("muta -equiv -f $1 -t $1 $SESSION");
		$cont++;
	}
	printf("\n%d Mutants checkeq as equivalent.\n", $cont);
}

#Generating a report for an empty test session
print( "\nGenerating a report for an empty test session\n" );
system( "exemuta -exec -trace $SESSION" );
system( "report -tcase -L 511 -S $SESSION-begin $SESSION" );
