use File::Find;
use File::Basename;
use File::Copy;

$srcDir=$ARGV[0];
$srcDirDir[0]=$srcDir;
print "========ENUM DIR===============\n";
find(\&enumdir, @srcDirDir);

sub enumdir {
        my $filename = $_;
		my $relativeFullname = $File::Find::name;
		my $isfile = -f $relativeFullname;
		if(!defined($isfile)){
			$isfile = 0;
		}
        print "find name: $filename, relativeFullname: $relativeFullname, isfile: $isfile\n";
}


