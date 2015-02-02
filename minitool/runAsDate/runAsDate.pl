use Time::Local;
use POSIX;
if(@ARGV<2){
	die "runAsDate yyyy/mm/dd app arg1 arg2....";
}

$time = time();
$time = strftime("%Y/%m/%d/", localtime($time));
system("echo $ARGV[0]|date");
shift @ARGV;
$cmdline = join ' ', @ARGV;
system($cmdline);
system("echo $time|date");
