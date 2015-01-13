--TEST--
get core info
--SKIPIF--
--POST--
--GET--
--ENV--
--INI--
--FILE--
<?php 
$r = @bartlby_new("/opt/bartlby/etc/bartlby.cfg");
$i = @bartlby_get_info($r);

include "skipif.php";

if($i) { 
	echo "OK";
} else {
	echo "CANNOT ATTACH TO SHM - bartlby running?";
}
?>
--EXPECT--
OK

