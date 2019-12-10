#ifndef OL66_WATCH_DOG
#define OL66_WATCH_DOG

/*        Creates a Watchdog process to keep calling process alive.
        arguments:
                num_args - number of strings in args_vector.
                args_vector - array of strings, arguments for calling process execution
                environment - NULL terminated array of strings

        returns:
                on success - 0
                on failure - != 0
*/

int WDKeepAlive(const char *abs_app_path,
                                int num_args,
                                char const *args_vector[]);

/*        Frees all resources allocated by WDKeepAlive
        arguments:
                resources - pointer to resources, WDKeepAlive
*/
void WDFree(void);

#endif /* OL66_WATCH_DOG */
