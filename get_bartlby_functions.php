<?
$f = get_defined_functions();
$f=$f["internal"];
for($x=0; $x<count($f); $x++) {
	if(preg_match("/bartlby_/i", $f[$x])) {
		echo $f[$x] . "\n";
	}

}

?>
