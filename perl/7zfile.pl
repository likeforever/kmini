#usage: 7zfile.pl srcdir dstdir password
#作用：将srcdir目录下所有文件压缩成7z，并设置密码为password。所有压缩的文件都以数字编号，数字编号与真实文件名对应关系存在out.txt中。

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


