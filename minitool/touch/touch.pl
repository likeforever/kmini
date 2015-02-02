use File::Touch;

foreach (@ARGV)	{
	touch $_;
}
