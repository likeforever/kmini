use Date::Calc qw(:all); #对日期进行各种加减等比较

$st1 = Date_to_Days(2014,3,2);
$st2 = Date_to_Days(2013,8,2);
if($st1<$st2){
	$compare=1;
}else{
	$compare=0;
}
print "date to num: $st1, $st2. compare: $compare\n\n";

print "test add date\n";
for($i = 0; $i < 3; $i++){
	my ($year, $mon, $day) = Add_Delta_Days(2014, 3, 2, $i*7); 
	print "$year:$mon:$day\n";
}