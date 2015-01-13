--TEST--
Servergroup Functions
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

$servergroup_id = bartlby_add_servergroup($r, $servergroup_dummy);
if($servergroup_id > 0) {
	echo "SERVERGROUP ADD OK\n";
}

$mod = $servergroup_dummy;
$mod["servergroup_name"] = "renamed";

$rtc = bartlby_modify_servergroup($r,  $servergroup_id, $mod);
$get = bartlby_get_servergroup_by_id($r, $servergroup_id);
if($get["servergroup_name"] == "renamed") {
	echo "SERVERGROUP MODIFIED AND FETCHED\n";
}
bartlby_reload($r);
sleep(2);
$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");
$shm = bartlby_get_servergroup($r, 0);
if($shm) {
	echo "SERVERGROUP FROM SHM OK\n";
}

$NN=$servergroup_id+999;
$ret = bartlby_set_servergroup_id($r, $servergroup_id, $NN);
$get = bartlby_get_servergroup_by_id($r, $ret);
if($ret == $NN) {
	echo "SERVERGROUP id changed to\n";
}



bartlby_toggle_servergroup_notify($r, 0, 0);
bartlby_toggle_servergroup_active($r, 0, 0);

$ret = bartlby_delete_servergroup($r, $NN);
if($ret >= 0) {
	echo "SERVERGROUP DEL OK\n";
}





bartlby_close($r);

?>
--EXPECT--
SERVERGROUP ADD OK
SERVERGROUP MODIFIED AND FETCHED
SERVERGROUP FROM SHM OK
SERVERGROUP id changed to
SERVERGROUP DEL OK


