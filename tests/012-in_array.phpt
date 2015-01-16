--TEST--
Test in array
--SKIPIF--
--POST--
--GET--
--ENV--
--INI--
--FILE--
<?php 
$i = bartlby_in_array_test(array(0,2,47));
var_dump($i);
?>
--EXPECT--
int(47)

