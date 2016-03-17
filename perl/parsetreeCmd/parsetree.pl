$fn = $ARGV[0];
open $hFile, $fn or die "open $fn failed \n";
open $dst, ">$fn.out" or die "open out failed\n";
$start = 0;
while(<$hFile>){
	chomp;
	if(/^[A-Z]:.*/){
		$start = 1;
	}elsif(0 == $start){
		next;
	}
	$line = $_;
	$line =~ s/\xA2\x78/ /g;
	$line =~ s/\xA2\x75/ /g;
	$line =~ s/\xA2\x77/ /g;
	$line =~ s/\xA2\x7C/ /g;
	
	if($line =~ /(\s*)(.*)$/){
		 $spaceSize = length($1);
		 $curfn = $2;
	}
	
	if(0 == length($curfn)){
		next;
	}
	
	$pathNum = @fullPath;
	if(0 == $pathNum){
		push @fullPath, [$line, $spaceSize];	
		next;
	}
	
	for($i = 0; $i < $pathNum; $i++){
		if($fullPath[$i][1] >= $spaceSize){
			last;
		}
	}
	splice @fullPath, $i, -1;
	push @fullPath, [$curfn, $spaceSize];
	for($i = 0; $i < @fullPath; $i++){
		$fullpathName .= "$fullPath[$i][0]\\";
	}
	
	$fullpathName =~ s/\\\\/\\/g;
	$fullpathName = substr($fullpathName, 0, -1);
	#$fullpathName = substr($fullpathName, 2);
	print $dst $fullpathName, "\n";
	$fullpathName = "";
}
