--TEST--
get core info
--SKIPIF--
--POST--
--GET--
--ENV--
--INI--
--FILE--
<?php 
$r = bartlby_new(getcwd() . "/bartlby.cfg");
$i = bartlby_get_info($r);
if($i) echo "OK";
?>
--EXPECT--
OK
