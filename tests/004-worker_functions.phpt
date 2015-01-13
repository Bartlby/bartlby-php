--TEST--
Worker Functions
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

$worker_id = bartlby_add_worker($r, $worker_dummy);
if($worker_id > 0) {
	echo "WORKER ADD OK\n";
}

$mod = $worker_dummy;
$mod["name"] = "renamed";

$rtc = bartlby_modify_worker($r,  $worker_id, $mod);
$get = bartlby_get_worker_by_id($r, $worker_id);
if($get["name"] == "renamed") {
	echo "WORKER MODIFIED AND FETCHED\n";
}

$shm = bartlby_get_worker($r, 0);
if($shm) {
	echo "WORKER FROM SHM OK " . $shm["name"] . "\n";
}

$NN=$worker_id+999;
$ret = bartlby_set_worker_id($r, $worker_id, $NN);
$get = bartlby_get_worker_by_id($r, $ret);
if($ret == $NN) {
	echo "WORKER id changed to\n";
}
bartlby_reload($r);
sleep(2);
$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");
$orig = bartlby_get_worker($r, 0);
bartlby_set_worker_state($r, 0, 1);
$get = bartlby_get_worker($r, 0);
if($get["active"] == 1) {
	echo "WORKER activated\n";
}
//Recover orig state
bartlby_set_worker_state($r, 0, $orig["active"]);

$ret = bartlby_delete_worker($r, $NN);
if($ret >= 0) {
	echo "WORKER DEL OK\n";
}




bartlby_close($r);

?>
--EXPECT--
WORKER ADD OK
WORKER MODIFIED AND FETCHED
WORKER FROM SHM OK admin
WORKER id changed to
WORKER activated
WORKER DEL OK

