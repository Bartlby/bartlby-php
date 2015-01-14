--TEST--
Cleanup Test Data
--SKIPIF--
<? include "skipif.php"; ?>
--POST--
--GET--
--ENV--
--INI--
--FILE--
<?php 

$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");
$c = bartlby_cleanup_tests($r);
if($c <= 0) echo "ERRROR";
echo "OK\n";
bartlby_close($r);

?>
--EXPECT--
OK

