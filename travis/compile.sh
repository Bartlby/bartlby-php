if [ "$TRAVIS_BRANCH" = "php7" ];
then
	echo "Installing PHP7";
	mkdir -p $HOME/php
	git clone --depth 1 https://github.com/php/php-src $HOME/php/src

	cd $HOME/php/src
	./buildconf --force
	./configure --prefix=$HOME --disable-all --enable-maintainer-zts
	make -j2 --quiet install

	cd $TRAVIS_BUILD_DIR
	$HOME/bin/phpize
	./configure --with-php-config=$HOME/bin/php-config 
	make -j2 --quiet
	EX=$?;
	if [ $EX != 0 ];
	then
		echo "compile failed";
		exit $EX;
	fi;



	exit $EX;
fi;


phpize
./configure 
make
EX=$?;
if [ $EX != 0 ];
then
	echo "compile failed";
	exit $EX;
fi;



exit $EX;







