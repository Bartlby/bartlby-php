--TEST--
Trap Functions
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

$trap_id = bartlby_add_trap($r, $trap_dummy);
if($trap_id > 0) {
	echo "TRAP ADD OK\n";
}

$mod = $trap_dummy;
$mod["trap_name"] = "renamed";

$rtc = bartlby_modify_trap($r,  $trap_id, $mod);
$get = bartlby_get_trap_by_id($r, $trap_id);
if($get["trap_name"] == "renamed") {
	echo "TRAP MODIFIED AND FETCHED\n";
}
bartlby_reload($r);
sleep(2);
$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");
$shm = bartlby_get_trap($r, 0);
if($shm) {
	echo "TRAP FROM SHM OK\n";
}

$NN=$trap_id+999;
$ret = bartlby_set_trap_id($r, $trap_id, $NN);
$get = bartlby_get_trap_by_id($r, $ret);
if($ret == $NN) {
	echo "TRAP id changed to\n";
}



$ret = bartlby_delete_trap($r, $NN);
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


