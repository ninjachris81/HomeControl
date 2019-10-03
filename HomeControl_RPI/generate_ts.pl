use strict;
use warnings;

my $filecontent = '';

my $filename = $ARGV[0];
my $outfile = $ARGV[1];

open(my $fh, '<:encoding(UTF-8)', $filename) or die "Could not open file '$filename' $!";

while (my $row = <$fh>) {
  $filecontent .= "$row";
  #print "$row\n";
}

close($fh);

#print "$filecontent\n";

open(my $fh2, '>', $outfile) or die "Could not open file '$outfile' $!";

while ($filecontent =~ /(.*_LABEL_.*)/g) {
	my $line = $1;
	
	my @tokens = $line =~ /(#define)\s{1}(.*_LABEL_.*)\s{1}(\"{1}.*\"{1})/g;
	
	print $fh2 "#define CONST_$tokens[1] QT_TRANSLATE_NOOP(\"Constants\", $tokens[2]);\n";
}

close $fh2;