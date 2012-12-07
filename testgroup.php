<?
//testing 1231
$cfg = "/storage/SF.NET/BARTLBY/GIT/bartlby-core/bartlby.cfg";
$map = bartlby_svc_map($cfg, null, null);
$info = bartlby_get_info($cfg);
//var_dump($info);
//var_dump($map);


echo "mem: " . memory_get_usage(true) . "\n";
//var_dump($map);

//var_dump(bartlby_get_service($cfg, 4));


var_dump(bartlby_get_server_by_id($cfg, 11));
?>
