<?
$cfg = "/storage/SF.NET/BARTLBY/GIT/bartlby-core/bartlby.cfg";

var_dump(bartlby_servergroup_map($cfg));

bartlby_delete_servergroup($cfg, 1);
?>
