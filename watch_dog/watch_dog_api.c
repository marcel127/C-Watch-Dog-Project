#define _POSIX_C_SOURCE (0xfffffffff)

#include <pthread.h>        /*thread            */
#include <signal.h>         /*signals           */
#include <stdlib.h>         /*envp              */
#include <stdio.h>          /*print             */
#include <string.h>         /*strcmp            */
#include <semaphore.h>      /*semaphore         */
#include <fcntl.h>          /*named semaphore   */

#define RESET   	"\033[0m"       
#define BOLDBLUE	"\033[01;34m"      
#define BOLDGREEN	"\033[01;32m"    
#define BOLDYELLOW	"\033[01;33m"   

#include "watch_dog.h"
#include "scheduler.h"

#define WD (1)
#define APP (0)
#define SUCCESS (0)
#define FAILURE (1)
#define ATTEMPT_NUM (4)
#define UNUSED(x) ((void)x)
#define UP_WD ("./wd.out")
#define RETRY (4)

static sch_t *g_sch = NULL;
static pid_t g_who_to_kill = {0};
static pthread_t g_thread = {0};
static sem_t *g_shared_sem = NULL;
static pthread_mutex_t g_lock;
static size_t g_counter = 0;
static int g_who_am_i = APP;
static char *g_wd_arg[2] = {0};  
static volatile int g_application_running = 1;

/*handlers*/
static int InitResuorces(void);
static void *APPThread(void *arg);
static void DestroyAll(void);
/*signal handlers*/
static void USR1Handler(int sig);
static void USR2Handler(int sig);
/*schduler*/
static int SendUSR1(void *arg);
static int CheckCounter(void *arg);
/*tasks*/
static void InitScheduler(void);
static void WDTask(void);
static int APPTask(pid_t pid);

int WDKeepAlive(const char *abs_app_path, int argc,char const *argv[])
{
    pid_t wd_process = {0};
    
    UNUSED(abs_app_path);
    UNUSED(argc);

    if (SUCCESS != InitResuorces())
    {
        return FAILURE;
    }

    if (0 == strcmp(argv[0], UP_WD))
    {   
        g_who_am_i = WD;
        g_wd_arg[0] = (char *)argv[1];
        
        WDTask();
    }

    else
    {
        wd_process = fork();
        g_wd_arg[1] = (char *)argv[0];
        g_wd_arg[0] = UP_WD;
        
        if (-1 == wd_process)
        {
            printf("fork failed\n");
            
            return FAILURE;
        }
        
        return APPTask(wd_process);    
    }
    
    return SUCCESS;
}

static void WDTask(void)
{
    sem_post(g_shared_sem);
    g_who_to_kill = getppid();
    
    printf(BOLDBLUE"i am wd: %u\n",getpid());
    
    InitScheduler();
    SchRun(g_sch);

    DestroyAll();
}

static void *APPThread(void *arg)
{
    UNUSED(arg);
    
    sem_wait(g_shared_sem);
    
    InitScheduler();
    SchRun(g_sch);
    
    return NULL;
}

static int APPTask(pid_t pid)
{
    if (0 == pid)
    {   
        execv(UP_WD, g_wd_arg);
    }
    
    else 
    {
        g_who_to_kill = pid;
        
        printf(BOLDGREEN"i am app: %u\n" ,getpid());
        
        if (0 != pthread_create(&g_thread, NULL, APPThread, NULL))
        {
            printf("can't create wd thread \n");
            
            return FAILURE;
        }
    }
    
    return SUCCESS;
}

void WDFree(void)
{
    size_t i = 0;

    for (i = 0 ;i < RETRY ; ++i)
    {
        kill(g_who_to_kill, SIGUSR2);

        if (0 == g_application_running)
        {
            break;
        }

    }
    
    g_application_running = 0;

    pthread_join(g_thread,NULL);
    DestroyAll();
}

static int InitResuorces(void)
{
    struct sigaction signal_USR1 = {0};
    struct sigaction signal_USR2 = {0};
    
    signal_USR1.sa_handler = USR1Handler;
    signal_USR2.sa_handler = USR2Handler;
    
    if (0 != sigaction(SIGUSR1, &signal_USR1, NULL))
    {
        printf("sigaction error :( \n");

        return FAILURE;
    }

    if (0 != sigaction(SIGUSR2, &signal_USR2, NULL))
    {
        printf("sigaction error :( \n");

        return FAILURE;
    }

    g_sch = SchCreate();

    if (NULL == g_sch)
    {
        return FAILURE;
    } 

    g_shared_sem = sem_open("shared semaphore", O_CREAT, S_IRUSR | S_IWUSR, 0);

    if (pthread_mutex_init(&g_lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        DestroyAll();
        
        return FAILURE;
    }
    
    return SUCCESS;
}

static void USR1Handler(int sig)
{
    UNUSED(sig);
        
    pthread_mutex_lock(&g_lock); 

    g_counter = 0;

    pthread_mutex_unlock(&g_lock);
}

static void USR2Handler(int sig)
{
    UNUSED(sig);

    if (WD == g_who_am_i)
    {
        kill(g_who_to_kill, SIGUSR2);
    }

    g_application_running = 0;
    
}

static int SendUSR1(void *arg)
{
    UNUSED(arg);

    kill(g_who_to_kill,SIGUSR1);

    if (g_application_running == 0)
    {
        SchStop(g_sch);
          
        return 1;
    }

    pthread_mutex_lock(&g_lock);
    ++g_counter; 
    pthread_mutex_unlock(&g_lock);
            
    return 0;
}

static int CheckCounter(void *arg)
{
    UNUSED(arg);

    pthread_mutex_lock(&g_lock);

    if (g_counter >= ATTEMPT_NUM)
    {   
        if (WD == g_who_am_i) 
        {   /* application stop */
            printf(BOLDYELLOW"\napp die new app entering\n");
            execv(g_wd_arg[0], g_wd_arg);
        }

        else 
        {   /*watch dog stop*/   
            printf(BOLDYELLOW"\nwd die new wd entering\n");
            g_who_to_kill = fork();

            if (0 == g_who_to_kill)
            {
                execv(UP_WD, g_wd_arg);
            }

            else
            {
                g_counter = 0;
                sem_wait(g_shared_sem);
            }
        }
    }
    
    pthread_mutex_unlock(&g_lock);
    
    return 0;
}

static void InitScheduler(void)
{
    SchAdd(g_sch, 1, SendUSR1, NULL);
    SchAdd(g_sch, 1, CheckCounter, NULL);
}

static void DestroyAll(void)
{
    SchDestroy(g_sch);
    pthread_mutex_destroy(&g_lock);
    sem_destroy(g_shared_sem);
}
