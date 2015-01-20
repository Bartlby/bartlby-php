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

PHP_FUNCTION(bartlby_event_tick) {
	zval * zbartlby_resource;

	struct shm_header * shm_hdr;
	
	int idx;

	bartlby_res * bres;
		
	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby_resource)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	idx=shm_hdr->cur_event_index;
	RETURN_LONG(idx);
		
	
}

PHP_FUNCTION(bartlby_event_fetch) {
	zval * zbartlby_resource;
	zval * bartlby_service_id;
	zval * event_index;
	
	
	struct btl_event * evntmap;
	
	bartlby_res * bres;



	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &event_index)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(event_index);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	

	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
		
	
	
	
	evntmap=bartlby_SHM_EventMap(bres->bartlby_address);
	
	if(Z_LVAL_P(event_index) < EVENT_QUEUE_MAX) {
		add_assoc_string(return_value, "message", evntmap[Z_LVAL_P(event_index)].evnt_message, 1);
		add_assoc_long(return_value, "id", evntmap[Z_LVAL_P(event_index)].evnt_id);
		add_assoc_long(return_value, "time", evntmap[Z_LVAL_P(event_index)].evnt_time);
	} else {
		RETURN_FALSE;
	}
		
		
}

