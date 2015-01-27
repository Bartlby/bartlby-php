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

PHP_FUNCTION(bartlby_get_thread_count) {
	zval * zbartlby_resource, * idx;
	struct shm_header * shm_hdr;

	bartlby_res * bres;
	
	
	ZEND_PARSE_PARAMETERS_START(1,1)
		Z_PARAM_RESOURCE(zbartlby_resource)
	ZEND_PARSE_PARAMETERS_END(); 

	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);

	RETURN_LONG(shm_hdr->sched_workers_count);

}
PHP_FUNCTION(bartlby_get_thread_info) {
	zval * zbartlby_resource, * idx;
	struct shm_header * shm_hdr;

	bartlby_res * bres;
	
	
	ZEND_PARSE_PARAMETERS_START(2,2)
		Z_PARAM_RESOURCE(zbartlby_resource)
		Z_PARAM_ZVAL(idx)
	ZEND_PARSE_PARAMETERS_END(); 	
	

	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);

/*
		int pid;
	struct service * svc;
	int start_time;
	int  idle;
	int shutdown;
	struct tms timing;
	int idx;

*/
	convert_to_long(idx);
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	if( shm_hdr->sched_workers_count  == -1 ) RETURN_LONG(-1);
	if(Z_LVAL_P(idx) > shm_hdr->sched_workers_count) RETURN_LONG(-1);

	add_assoc_long(return_value, "pid", shm_hdr->worker_threads[Z_LVAL_P(idx)].pid);
	add_assoc_long(return_value, "start_time", shm_hdr->worker_threads[Z_LVAL_P(idx)].start_time);
	add_assoc_long(return_value, "idle", shm_hdr->worker_threads[Z_LVAL_P(idx)].idle);
	add_assoc_long(return_value, "shutdown", shm_hdr->worker_threads[Z_LVAL_P(idx)].shutdown);
	add_assoc_long(return_value, "service_id", shm_hdr->worker_threads[Z_LVAL_P(idx)].svc_id);
	
	add_assoc_long(return_value, "time_used", shm_hdr->worker_threads[Z_LVAL_P(idx)].timing.tms_utime);
	add_assoc_long(return_value, "memory_used", shm_hdr->worker_threads[Z_LVAL_P(idx)].memory_used);
	

	

}
