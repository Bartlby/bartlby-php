--TEST--
Servicegroup Functions
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

$servicegroup_id = bartlby_add_servicegroup($r, $servicegroup_dummy);
if($servicegroup_id > 0) {
	echo "SERVICEGROUP ADD OK\n";
}

$mod = $servicegroup_dummy;
$mod["servicegroup_name"] = "renamed";

$rtc = bartlby_modify_servicegroup($r,  $servicegroup_id, $mod);
$get = bartlby_get_servicegroup_by_id($r, $servicegroup_id);
if($get["servicegroup_name"] == "renamed") {
	echo "SERVICEGROUP MODIFIED AND FETCHED\n";
}
bartlby_reload($r);
sleep(2);
$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");
$shm = bartlby_get_servicegroup($r, 0);
if($shm) {
	echo "SERVICEGROUP FROM SHM OK\n";
}

$NN=$servicegroup_id+999;
$ret = bartlby_set_servicegroup_id($r, $servicegroup_id, $NN);
$get = bartlby_get_servicegroup_by_id($r, $ret);
if($ret == $NN) {
	echo "SERVICEGROUP id changed to\n";
}



bartlby_toggle_servicegroup_notify($r, 0, 0);
bartlby_toggle_servicegroup_active($r, 0, 0);

$ret = bartlby_delete_servicegroup($r, $NN);
if($ret >= 0) {
	echo "SERVICEGROUP DEL OK\n";
}





bartlby_close($r);

?>
--EXPECT--
SERVICEGROUP ADD OK
SERVICEGROUP MODIFIED AND FETCHED
SERVICEGROUP FROM SHM OK
SERVICEGROUP id changed to
SERVICEGROUP DEL OK


