--TEST--
Trigger Functions
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

$trigger_id = bartlby_add_trigger($r, $trigger_dummy);
if($trigger_id > 0) {
	echo "TRIGGER ADD OK\n";
}

$mod = $trigger_dummy;
$mod["trigger_name"] = "renamed";

$rtc = bartlby_modify_trigger($r,  $trigger_id, $mod);
$get = bartlby_get_trigger_by_id($r, $trigger_id);
if($get["trigger_name"] == "renamed") {
	echo "TRIGGER MODIFIED AND FETCHED\n";
}
bartlby_reload($r);
sleep(2);
$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");
$shm = bartlby_get_trigger($r, 0);
if($shm) {
	echo "TRIGGER FROM SHM OK\n";
}

$NN=$trigger_id+999;
$ret = bartlby_set_trigger_id($r, $trigger_id, $NN);
$get = bartlby_get_trigger_by_id($r, $ret);
if($ret == $NN) {
	echo "TRIGGER id changed to\n";
}



$ret = bartlby_delete_trigger($r, $NN);
if($ret >= 0) {
	echo "TRIGGER DEL OK\n";
}





bartlby_close($r);

?>
--EXPECT--
TRIGGER ADD OK
TRIGGER MODIFIED AND FETCHED
TRIGGER FROM SHM OK
TRIGGER id changed to
TRIGGER DEL OK


