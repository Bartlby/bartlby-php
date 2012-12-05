<?
//testing 1231
$cfg = "/storage/SF.NET/BARTLBY/GIT/bartlby-core/bartlby.cfg";
$map = bartlby_svc_map($cfg, null, null);

bartlby_modify_servergroup($cfg, "asdf", 1,1,"asd", 3);

var_dump($map);
?>
