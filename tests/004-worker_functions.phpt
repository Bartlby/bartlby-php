--TEST--
Worker Functions
	bartlby_get_worker
	bartlby_add_worker - done
	bartlby_delete_worker
	bartlby_modify_worker
	bartlby_get_worker_by_id
	bartlby_set_worker_id
	bartlby_set_worker_state
--SKIPIF--
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

?>
--EXPECT--
WORKER ADD OK

