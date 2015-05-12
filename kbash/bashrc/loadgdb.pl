#!/usr/bin/perl
$fn = $ENV{"HOME"}."/.gdbinit" ;
if (-e $fn){
	open $hfile,  "$fn" or die "open $fn failed! error:$!" ;
}else{
	open $hfile, ">", $fn or die "open $fn failed! error:$!" ;
}
while (<$hfile>) {
   if (/kbash_loadgdb/) {
	  $load = 1;
   }
}

if (1 != $load) {
   close $hfile;
   open $hfile, ">>", $fn or die "open $fn failed! error:$!" ;
   print $hfile "#kbash_loadgdb\n";
   print $hfile "source ~/kbash/bashrc/like.gdb\n";
   print $hfile "source ~/kbash/bashrc/stl.gdb\n";
}

close $hfile;
