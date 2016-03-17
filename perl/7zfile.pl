#usage: 7zfile.pl srcdir dstdir password
#���ã���srcdirĿ¼�������ļ�ѹ����7z������������Ϊpassword������ѹ�����ļ��������ֱ�ţ����ֱ������ʵ�ļ�����Ӧ��ϵ����out.txt�С�

use File::Find;
use File::Basename;
use File::Copy;

$srcDir=$ARGV[0];
$srcDirDir[0]=$srcDir;
$dstDir = $ARGV[1];
$password = $ARGV[2];
$index = 1;
open $outfn, ">$dstDir/out.txt" or die "open failed out.txt";
find(\&enumdir, @srcDirDir);

sub enumdir {
        my $filename = $_;
		my $relativeFullname = $File::Find::name;
		if(-f $relativeFullname){
			$zname = "$index.7z";
			$cmd = "7z a -t7z -p$password -mhe $dstDir/$zname \"$relativeFullname\"";
			print $cmd, "\n";
			system($cmd);
			$index++;
			print $outfn "$zname:\t\t$filename\n";
		}

}


