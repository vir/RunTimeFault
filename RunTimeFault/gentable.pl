#!perl

use Encode;

my($cp) = @ARGV;
die "Usage: $0 866\n" unless $cp;

print "const char* encoding_$cp\[\] = {\n";
for(my $i = 0x80; $i < 0x100; ++$i) {
	my $c = chr($i);
	Encode::from_to($c, "cp$cp", 'utf8');
	my @b = split '', $c;
	print qq{\t"};
	printf qq{\\x%02X}, ord($_) foreach @b;
	print qq{",};
	print "\n" if 0x03 == ($i&0x03);
}
print "};\n\n";
