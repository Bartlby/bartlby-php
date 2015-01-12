phpize

./configure
make
EX=$?;
if [ $EX != 0 ];
then
	echo "compile failed";
	exit $EX;
fi;
PHP_DIR=$PWD;



git clone https://github.com/Bartlby/bartlby-core.git bcore
cd bcore
travis/compile.sh

cd $PHP_DIR;
NO_INTERACTION=1 make test TESTS="-m"

EX=$?;


exit $EX;







