/*
 * Lock Thread Counter.
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

static struct semaphore *tsem = NULL;
static unsigned long COUNTER = 0;
static struct lock *thread_lock;

static
void
init_sem(void)
{
    if(tsem == NULL)
    {
        tsem = sem_create("tsem", 0);
        if(tsem == NULL)
        {
            panic("threadtest: sem_create failed\n");
        }
    }
}

static
void
destroy_sem(void)
{
    if(tsem != NULL)
    {
        sem_destroy(tsem);
        tsem = NULL;
    }
}

static
void
init_lock(void)
{
    if (thread_lock==NULL) {
		thread_lock = lock_create("thread_lock");
		if (thread_lock == NULL) {
			panic("threadtest: lock_create failed\n");
		}
	}
}

static
void 
destroy_lock(void)
{
    if( thread_lock != NULL ){
        lock_destroy(thread_lock);
        thread_lock = NULL;
    }
}

static
void
lockthread(void *junk, unsigned long num )
{
    
    (void)junk;
   
    for( unsigned long i = num; i > 0; i--){
        lock_acquire(thread_lock);
        COUNTER++; 
        lock_release(thread_lock);
    }
   
	V(tsem);
}

static
void
runthreads(int NTHREADS, int INCREMENTS)
{

	char name[16];
	int i, result;

	for (i=0; i<NTHREADS; i++) {
		snprintf(name, sizeof(name), "threadtest%d", i);
		result = thread_fork(name, NULL, lockthread, NULL, INCREMENTS);
        DEBUG(DB_SYNCPROB, "\nthread %d created\n", i);
		if (result) {
			panic("threadtest: thread_fork failed %s)\n", 
			      strerror(result));
		}
	}

	for (i=0; i<NTHREADS; i++) {
		P(tsem);
	}
}


int
lockthreadcounter(int nargs, char **args)
{	
	
	 int NTHREADS = 10;
    int INCREMENTS = 1;

	if( nargs > 1 ){
		NTHREADS = atoi( args[1] );
        if( nargs == 3 ){
            INCREMENTS = atoi( args[2] );
        }
	}

	init_sem();
    init_lock();
	kprintf("\n---Starting Lock Thread---\n\n");
    kprintf("Counter expected value: %d\n", INCREMENTS*NTHREADS);
	runthreads(NTHREADS, INCREMENTS);
    kprintf("Actual counter value: %lu\n", COUNTER);
	kprintf("\n---Lock Thread done---\n\n");
    destroy_lock();
    destroy_sem();

	return 0;
}
