--TEST--
Core Functions
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

bartlby_version();
if(!is_string(bartlby_config("/opt/bartlby/etc/bartlby.cfg", "pidfile_dir"))) {
	echo "PANIC";
}
$lib=bartlby_lib_info($r);



//bartlby_encode
//bartlby_decode
//bartlby_add_service_array
//bartlby_toggle_sirene
//bartlby_shm_destroy
//bartlby_callback_test



bartlby_ack_problem($r, 0);
bartlby_check_force($r, 0);
bartlby_event_tick($r);
bartlby_event_fetch($r, 0);
bartlby_set_passive($r, 0, 0, "unit-test");
bartlby_get_core_extension_info("/opt/bartlby-extensions/lib/nagiosperfdata.so");
bartlby_notification_log_at_index($r, 0);


bartlby_close($r);
echo "OK\n";
?>
--EXPECT--
OK

