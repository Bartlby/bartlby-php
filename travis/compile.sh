phpize

./configure
make
if [ $? != 0 ]; then
	exit $?;
fi;
make install
ls

exit 0;







