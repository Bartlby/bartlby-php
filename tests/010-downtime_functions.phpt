--TEST--
Downtime Functions
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

$downtime_id = bartlby_add_downtime($r, $downtime_dummy);
if($downtime_id > 0) {
	echo "TRAP ADD OK\n";
}

$mod = $downtime_dummy;
$mod["downtime_notice"] = "renamed";

$rtc = bartlby_modify_downtime($r,  $downtime_id, $mod);
$get = bartlby_get_downtime_by_id($r, $downtime_id);
if($get["downtime_notice"] == "renamed") {
	echo "TRAP MODIFIED AND FETCHED\n";
}
bartlby_reload($r);
sleep(2);
$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");
$shm = bartlby_get_downtime($r, 0);
if($shm) {
	echo "TRAP FROM SHM OK\n";
}

$NN=$downtime_id+999;
$ret = bartlby_set_downtime_id($r, $downtime_id, $NN);
$get = bartlby_get_downtime_by_id($r, $ret);
if($ret == $NN) {
	echo "TRAP id changed to\n";
}



$ret = bartlby_delete_downtime($r, $NN);
if($ret >= 0) {
	echo "TRAP DEL OK\n";
}





bartlby_close($r);

?>
--EXPECT--
TRAP ADD OK
TRAP MODIFIED AND FETCHED
TRAP FROM SHM OK
TRAP id changed to
TRAP DEL OK


