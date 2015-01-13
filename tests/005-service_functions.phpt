--TEST--
Service Functions
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
$i=bartlby_check_shm_size($r);

$service_id = bartlby_add_service($r, $service_dummy);
if($service_id > 0) {
	echo "SERVICE ADD OK\n";
}

$mod = $service_dummy;
$mod["service_name"] = "renamed";

$rtc = bartlby_modify_service($r,  $service_id, $mod);
$get = bartlby_get_service_by_id($r, $service_id);
if($get["service_name"] == "renamed") {
	echo "SERVICE MODIFIED AND FETCHED\n";
}
bartlby_reload($r);
sleep(2);
$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");
$shm = bartlby_get_service($r, 0);
if($shm) {
	echo "SERVICE FROM SHM OK\n";
}

$NN=$service_id+999;
$ret = bartlby_set_service_id($r, $service_id, $NN);
$get = bartlby_get_service_by_id($r, $ret);
if($ret == $NN) {
	echo "SERVICE id changed to\n";
}


$ret = bartlby_delete_service($r, $NN);
if($ret >= 0) {
	echo "SERVICE DEL OK\n";
}

//TOGGLES
bartlby_toggle_service_notify($r, 0, 0);
bartlby_toggle_service_handled($r, 0, 0);
bartlby_toggle_service_active($r, 0, 0);
bartlby_service_set_interval($r, 0, 100,0);


//SET STATE VIA ARRAY - and get 0 as expected result
bartlby_bulk_service_active($r, array(0=>0), 0, 0);
bartlby_bulk_service_notify($r, array(0=>0), 0, 0);
bartlby_bulk_force_services($r, array(0=>0));





bartlby_close($r);

?>
--EXPECT--
SERVICE ADD OK
SERVICE MODIFIED AND FETCHED
SERVICE FROM SHM OK
SERVICE id changed to
SERVICE DEL OK

