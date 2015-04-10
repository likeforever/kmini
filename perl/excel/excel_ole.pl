use Win32::OLE;   #导入win32 OLE包
use Win32::OLE::Variant; #这个是必要的，有这个excel如日历格式这些才会自动转化为字符串
use Cwd;

eval {$app_xls = Win32::OLE->GetActiveObject('Excel.Application')};
	die "Excel not installed" if $@;
unless (defined $app_xls) {
	$app_xls = Win32::OLE->new('Excel.Application', sub {$_[0]->Quit;})
			or die "Oops, cannot start Excel";
}
$app_xls->{DisplayAlerts} = "False";  #关掉excel的提示，比如是否保存修改之类的

my $dir = getcwd();
$dst_name = $dir."/task.xls";
print "open excel:$dst_name\n";
my $dst_book = $app_xls->WorkBooks->Open($dst_name);
my $dst_sheet = $dst_book->Worksheets(1);#内部第1个sheet

$date = $dst_sheet->Cells(1,5)->{Value}; #测试读取
print "cell 1, 5: $date\n";

$dst_sheet->Cells(1,6)->{Value} = "testwrite"; #测试写入

$saveName = $dst_name."_new.xls";
print "save excel to : $saveName\n";
$saveName =~ s/\//\\/g; #saveas的文件名必须是反斜杠，否则保存不了，还要提示错误
$dst_book->SaveAs($saveName); #保存所做修改，用saveas可避免excel弹出其它对话框,比如什么兼容性这些的
$app_xls->{DisplayAlerts} = 'True'; #恢复提示

undef $dst_book;
undef $app_xls;  #关掉所打开的excel应用