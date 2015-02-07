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


struct shm_header * bartlby_SHM_GetHDR(void * shm_addr) {
	return (struct shm_header *)(void *)shm_addr;
}

struct trigger * bartlby_SHM_TriggerMap(void * shm_addr) {
	//Is beyond the servergroups
	struct shm_header * hdr;
	struct trap * trapmap;
	
	hdr=bartlby_SHM_GetHDR(shm_addr);
	trapmap=bartlby_SHM_TrapMap(shm_addr);
	
	
	return (struct trigger *)(void *)&trapmap[hdr->trapcount+1];
}


struct trap * bartlby_SHM_TrapMap(void * shm_addr) {
	//Is beyond the servergroups
	struct shm_header * hdr;
	struct servicegroup * svcgrpmap;
	
	hdr=bartlby_SHM_GetHDR(shm_addr);
	svcgrpmap=bartlby_SHM_ServiceGroupMap(shm_addr);
	
	
	return (struct trap *)(void *)&svcgrpmap[hdr->svcgroupcount+1];
}


struct servicegroup * bartlby_SHM_ServiceGroupMap(void * shm_addr) {
	//Is beyond the servergroups
	struct shm_header * hdr;
	struct servergroup * srvgrpmap;
	
	hdr=bartlby_SHM_GetHDR(shm_addr);
	srvgrpmap=bartlby_SHM_ServerGroupMap(shm_addr);
	
	
	return (struct servicegroup *)(void *)&srvgrpmap[hdr->srvgroupcount+1];
}

struct servergroup * bartlby_SHM_ServerGroupMap(void * shm_addr) {
	//Is beyond the events
	struct shm_header * hdr;
	struct btl_event * evmap;
	
	hdr=bartlby_SHM_GetHDR(shm_addr);
	evmap=bartlby_SHM_EventMap(shm_addr);
	
	
	return (struct servergroup *)(void *)&evmap[EVENT_QUEUE_MAX+1];
}

struct btl_event * bartlby_SHM_EventMap(void * shm_addr) {
	//Is beyond the 3 integers :-)
	struct shm_header * hdr;
	struct server * srvmap;
	
	hdr=bartlby_SHM_GetHDR(shm_addr);
	srvmap=bartlby_SHM_ServerMap(shm_addr);
	
	
	return (struct btl_event *)(void *)&srvmap[hdr->srvcount+1];
}

struct server * bartlby_SHM_ServerMap(void * shm_addr) {
	//Is beyond the 3 integers :-)
	struct shm_header * hdr;
	struct service * svcmap;
	struct downtime * dtmap;
	
	hdr=bartlby_SHM_GetHDR(shm_addr);
	
	svcmap=bartlby_SHM_ServiceMap(shm_addr);
	//wrkmap=(struct worker *)(void*)&svcmap[hdr->svccount];
	dtmap=bartlby_SHM_DowntimeMap(shm_addr);
	
	return (struct server *)(void *)&dtmap[hdr->dtcount+1];
}

struct downtime * bartlby_SHM_DowntimeMap(void * shm_addr) {
	//Is beyond the 3 integers :-)
	struct shm_header * hdr;
	struct service * svcmap;
	struct worker * wrkmap;
	
	hdr=bartlby_SHM_GetHDR(shm_addr);
	
	svcmap=bartlby_SHM_ServiceMap(shm_addr);
	//wrkmap=(struct worker *)(void*)&svcmap[hdr->svccount];
	wrkmap=bartlby_SHM_WorkerMap(shm_addr);
	
	return (struct downtime *)(void *)&wrkmap[hdr->wrkcount+1];
}

struct worker * bartlby_SHM_WorkerMap(void * shm_addr) {
	//Is beyond the 3 integers :-)
	struct shm_header * hdr;
	struct service * svcmap;
	hdr=bartlby_SHM_GetHDR(shm_addr);
	
	svcmap=bartlby_SHM_ServiceMap(shm_addr);
	
	return (struct worker *)(void*)&svcmap[hdr->svccount+1];
}

struct service * bartlby_SHM_ServiceMap(void * shm_addr) {
	//Is beyond the 3 integers :-)
	return (struct service *)(void *)(shm_addr+sizeof(struct shm_header));
}