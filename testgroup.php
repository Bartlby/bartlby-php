<?
ini_set("bartlby.force_audit", 1);
//testing 1231
$cfg = "/opt/bartlby/etc/bartlby.cfg";
$r = bartlby_new($cfg);
bartlby_toggle_server_active($r, 1, 1);

echo "Constant Test:" . BARTLBY_AUDIT_TYPE_SERVICE . "\n";



function bartlby_generic_audit($res, $id, $type, $line) {
	echo "GENERIC AUDIT: $id $type $line \n";
	return true;
}

function bartlby_object_audit($res, $type, $id, $action) {
	echo "AUDIT: Type=>" . $type . " ID=>" . $id . " action=>" . $action  . "\n";
	
	return true;
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
