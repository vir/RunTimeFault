#!perl

use Encode;

my($cp) = @ARGV;
die "Usage: $0 866\n" unless $cp;

print "const long encoding_$cp\[\] = {\n";
for(my $i = 0x80; $i < 0x100; ++$i) {
	my $c = chr($i);
	Encode::from_to($c, "cp$cp", 'ucs4be');
	printf "\t0x%08X,", unpack("N", $c);
	print "\n" if 0x03 == ($i&0x03);
}
print "};\n\n";
