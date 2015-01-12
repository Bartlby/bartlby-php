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



CORE_DIR=$PWD;
git clone https://github.com/Bartlby/bartlby-core.git bcore
cd bcore
travis/compile.sh
cd $CORE_DIR;
cp bcore/bartlby.cfg $PHP_DIR/


cd $PHP_DIR;
NO_INTERACTION=1 make test TESTS="-m"

EX=$?;


exit $EX;







