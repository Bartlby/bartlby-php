<?

$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");

if(!$r) print("skip");


$pid_file=bartlby_config("/opt/bartlby/etc/bartlby.cfg", "pidfile_dir");
$pid_ar=@file($pid_file . "/bartlby.pid");
$pid_is=@implode($pid_ar, "");
if(!$pid_is || !file_exists("/proc/" . $pid_is . "/cmdline")) {
	print("skip Bartlby not running");
}