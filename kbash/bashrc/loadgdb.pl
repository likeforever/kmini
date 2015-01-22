#!/usr/bin/perl
$fn = $ENV{"HOME"}."/.gdbinit" ;
if (-e $fn){
	open $hfile,  $fn or die "open $fn failed! error:$!" ;
}else{
	open $hfile, ">", $fn or die "open $fn failed! error:$!" ;
}
while (<$hfile>) {
   if (/kbash_loadgdb/) {
	  $load = 1;
   }
}

if (1 != $load) {
   print $hfile "source ./kbash/bashrc/like.gdb #kbash_loadgdb\n";
   print $hfile "source ./kbash/bashrc/stl.gdb #kbash_loadgdb\n";
}

close $hfile;
