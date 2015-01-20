--TEST--
Server Functions
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

$server_id = bartlby_add_server($r, $server_dummy);
if($server_id > 0) {
	echo "SERVER ADD OK\n";
}

$mod = $server_dummy;
$mod["server_name"] = "renamed";
$mod["server_notify"]=0;
$mod["server_enabled"]=0;
$rtc = bartlby_modify_server($r,  $server_id, $mod);
$get = bartlby_get_server_by_id($r, $server_id);
if($get["server_name"] == "renamed") {
	echo "SERVER MODIFIED AND FETCHED\n";
}
bartlby_reload($r);
sleep(2);
$r = bartlby_new("/opt/bartlby/etc/bartlby.cfg");
$shm = bartlby_get_server($r, 0);
if($shm) {
	echo "SERVER FROM SHM OK\n";
}

$NN=$server_id+999;
$ret = bartlby_set_server_id($r, $server_id, $NN,1);
$get = bartlby_get_server_by_id($r, $ret);
if($ret == $NN) {
	echo "SERVER id changed to\n";
}



bartlby_toggle_server_notify($r, 0, 0);
bartlby_toggle_server_active($r, 0, 0);

$ret = bartlby_delete_server($r, $NN);
if($ret >= 0) {
	echo "SERVER DEL OK\n";
}





bartlby_close($r);

?>
--EXPECT--
SERVER ADD OK
SERVER MODIFIED AND FETCHED
SERVER FROM SHM OK
SERVER id changed to
SERVER DEL OK


