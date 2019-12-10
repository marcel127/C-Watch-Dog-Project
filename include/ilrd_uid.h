/*==============================================================================
Unique Identifier
Header
OL66
Version 1
==============================================================================*/
#ifndef ILRD_UID_H
#define ILRD_UID_H

#include <sys/types.h>	/* pid_t   */
#include <stddef.h>		/* size_t  */
#include <sys/time.h>	/* timeval */
#include <unistd.h>	 	/*getpid()*/	

/*
                        WARNING!!!
struct definition is for use of implementor,
any changes to it will result in undefined behaviour.
*/
typedef struct uid_s
{
        struct timeval time;
        pid_t pid;
        size_t counter;
} ilrd_uid_t;

/*
    Returns new unique identifier.

        Complexity O(1)
*/
ilrd_uid_t UIDGet();

/*
    For the two given unique identifiers,
    returns true if uid1 and uid 2 are equal, false otherwise.

        Complexity O(1)
*/
int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2);

/*
    For a given unique identifier, returns true if it is bad, false otherwise.

        Complexity O(1)
*/
int UIDIsBad(ilrd_uid_t uid);

/*
    Returns a bad uid.

        Complexity O(1)
*/
ilrd_uid_t UIDGetBad();

#endif /* ILRD_UID_H */

