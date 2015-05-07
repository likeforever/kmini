
use Cwd 'abs_path';
use File::Basename;
print "current script path:", dirname(abs_path($0)), "\n";
print "current script name:", basename($0), "\n";
print "current script file mtime:", GetFileTime($0);

sub GetFileTime {
        my $file = $_[0];
        my $mtime=(stat $file)[9] or die "(can't find file).$!";
        my @t = localtime $mtime; 
        my $date = sprintf "%02u.%02u.%02u_%02u_%02u_%02u", $t[5] % 100, $t[4] + 1, $t[3], $t[2], $t[1], $t[0]; 
		return $date;
}