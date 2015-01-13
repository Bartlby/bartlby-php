#!/bin/bash
NO_INTERACTION=1 TEST_PHP_ARGS="-m --show-diff"  make test

#check for untested functions
php get_bartlby_functions.php | while read a; do o=$(grep $a tests/*.phpt); EX=$?; if [ $EX != 0 ]; then echo "$a not tested"; fi;  done


