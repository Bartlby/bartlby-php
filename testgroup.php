<?
//testing 1231
$cfg = "/opt/bartlby/etc/bartlby.cfg";
$r = bartlby_new($cfg);
$v = bartlby_notification_log_at_index($r, 511);
$v = bartlby_callback_test($r);




function bartlby_audit($res, $type, $id, $action) {
	echo "AUDIT: Type=>" . $type . " ID=>" . $id . " action=>" . $action . "\n";
	var_dump($res);
}

function callback_btl($p1, $p2, $p3) {
	echo "CALLBACK: p1: " . $p1 . "\n";
	echo "CALLBACK: p1: " . $p2 . "\n";
	echo "CALLBACK: p3:" . $p3 . "\n";
}

exit;
$cfg = "/storage/SF.NET/BARTLBY/GIT/bartlby-core/bartlby.cfg";
$map = bartlby_svc_map($cfg, null, null);
$info = bartlby_get_info($cfg);
//var_dump($info);
//var_dump($map);


echo "mem: " . memory_get_usage(true) . "\n";
//var_dump($map);

//var_dump(bartlby_get_service($cfg, 4));


var_dump(bartlby_get_service_by_id($cfg,7));
?>
