--TEST--
Worker Thread Info
--SKIPIF--
<? include "skipif.php"; ?>
--POST--
--GET--
--ENV--
--INI--
--FILE--
<?php 
$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");


$thread_count = bartlby_get_thread_count($r);
if($thread_count >= 0) {
	$info = bartlby_get_thread_info($r, 0);
	if($info["pid"] > 0) {
			echo "OK\n";
	}
} else {
	echo "OK\n";
}


bartlby_close($r);
?>
--EXPECT--
OK

