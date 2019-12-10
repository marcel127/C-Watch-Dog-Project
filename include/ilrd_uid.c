#include <time.h>    /* timeval */

#include "ilrd_uid.h"

ilrd_uid_t UIDGet()
{
	static size_t count = 0;
	
	ilrd_uid_t uid = {0};

	uid.counter = count;
	uid.pid = getpid();
	gettimeofday(&uid.time,NULL);

	++count;	

	return uid;
}

int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{	
	return !((uid1.counter != uid2.counter) ||
			(uid1.pid != uid2.pid) ||
			(uid1.time.tv_sec != uid2.time.tv_sec) ||
			(uid1.time.tv_usec != uid2.time.tv_usec));	
}

int UIDIsBad(ilrd_uid_t uid)
{
	ilrd_uid_t is_bad_uid = UIDGetBad();
	
	return UIDIsSame(uid,is_bad_uid);
}

ilrd_uid_t UIDGetBad()
{
	ilrd_uid_t bad_uid = {0};

	return bad_uid;
}


