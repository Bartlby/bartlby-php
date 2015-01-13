--TEST--
get SHM Signature
--SKIPIF--
<? include "skipif.php"; ?>
--POST--
--GET--
--ENV--
--INI--
--FILE--
<?php 
$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");
$i=bartlby_check_shm_size($r);
if($i) echo "OK";
?>
--EXPECT--
OK
