--TEST--
Get SHM object by ID
--SKIPIF--
<? include "skipif.php"; ?>
--POST--
--GET--
--ENV--
--INI--
--FILE--
<?php 
include "dummy_data.inc.php";
$OUT="";
$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");


$service_id = bartlby_add_service($r, $service_dummy);


bartlby_reload($r);
sleep(2);
$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");


$svc_shm = bartlby_get_object_by_id($r, BARTLBY_OBJECT_SERVICE, $service_id);

if($svc_shm["service_id"] != $service_id) {
	 echo "FAIL";
	var_dump($svc_shm);
}


bartlby_close($r);
echo "OK\n";
?>
--EXPECT--
OK

