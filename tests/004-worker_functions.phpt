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

$ret = bartlby_set_worker_id($r, $worker_id, 999);
$get = bartlby_get_worker_by_id($r, $ret);
if($ret == 999) {
	echo "WORKER id changed to " . $ret . "\n";
}

$orig = bartlby_get_worker($r, 0);
bartlby_set_worker_state($r, 0, 1);
$get = bartlby_get_worker($r, 0);
if($get["active"] == 1) {
	echo "WORKER activated\n";
}
//Recover orig state
bartlby_set_worker_state($r, 0, $orig["active"]);

$ret = bartlby_delete_worker($r, 999);
if($ret >= 0) {
	echo "WORKER DEL OK\n";
}

bartlby_close($r);

?>
--EXPECT--
WORKER ADD OK
WORKER MODIFIED AND FETCHED
WORKER FROM SHM OK admin
WORKER id changed to 999
WORKER activated
WORKER DEL OK

