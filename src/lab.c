/* $Id$ */
/* ----------------------------------------------------------------------- *
 *
 *   Copyright 2005-2008 Helmut Januschka - All Rights Reserved
 *   Contact: <helmut@januschka.com>, <contact@bartlby.org>
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, Inc., 675 Mass Ave, Cambridge MA 02139,
 *   USA; either version 2 of the License, or (at your option) any later
 *   version; incorporated herein by reference.
 *
 *   visit: www.bartlby.org for support
 * ----------------------------------------------------------------------- */
/*
$Revision$
$HeadURL$
$Date$
$Author$ 
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_bartlby.h"

extern int le_bartlby;

PHP_FUNCTION(bartlby_cleanup_tests) {
	zval * zbartlby_resource;
	bartlby_res * bres;
	char * dlmsg;
	int ret;
	int (*CleanupTestData)(char *);
	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby_resource)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	LOAD_SYMBOL(CleanupTestData,bres->SOHandle, "CleanupTestData");
	ret=CleanupTestData(bres->cfgfile);

	RETURN_LONG(ret);

}
PHP_FUNCTION(bartlby_callback_test) {

	zval * zbartlby_resource;
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby_resource)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);


	BARTLBY_OBJECT_AUDIT(zbartlby_resource, 1, 2222, 1);
	BARTLBY_OBJECT_AUDIT(zbartlby_resource, 2, 3333, 2);
	RETURN_LONG(1);
}



PHP_METHOD(Bartlby, testFunc) /* {{{ */
{
   
    RETURN_STRING("Hello World\n");
}

PHP_FUNCTION(bartlby_in_array_test) {
	zval * arr;
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &arr)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	if(btl_is_array(arr, 47) == 1)	 {
		RETURN_LONG(47);
	}
	RETURN_LONG(-1);

}





