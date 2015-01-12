--TEST--
get core info
--SKIPIF--
--POST--
--GET--
--INI--
--FILE--
<?php 
$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");
$i = bartlby_get_info($r);
if($i) echo "OK";
?>
--EXPECT--
OK
