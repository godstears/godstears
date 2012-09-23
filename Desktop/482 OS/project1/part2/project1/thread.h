/*
 * thread.h -- public interface to thread library
 *
 * This file should be included in both the thread library and application
 * programs that use the thread library.
 */
#ifndef _THREAD_H
#define _THREAD_H

#define STACK_SIZE 262144	/* size of each thread's stack */
#define MAX_CPUS 16             /* maximum number of CPUs */

typedef void (*thread_startfunc_t) (void *);

/*
 * boot_cpus() is provided by the infrastructure (not by the thread library).
 *
 * boot_cpus() starts all CPUs in the system (the number of CPUs is
 * specified by num_cpus).  One CPU will call thread_cpu_init(func, arg);
 * the other CPUs will call thread_cpu_init(NULL, NULL).
 * boot_cpus() does not return to the caller.
 *
 * async, sync, random_seed configure each CPU's generation of timer
 * interrupts (which are only delivered if interrupts are enabled).
 * Timer interrupts in turn lead to preemptions, i.e., calls to thread_yield().
 *
 * The sync and async parameters can generate several patterns of interrupts:
 *
 *     1. async = true: generate interrupts asynchronously every 1 ms using
 *        SIGALRM.  These are non-deterministic.  
 *
 *     2. sync = true: generate synchronous, pseudo-random interrupts on each
 *        CPU.  You can generate different (but repeatable) interrupt
 *        patterns by changing random_seed.
 *
 * An application will be deterministic if and only if num_cpus=1 && async=false.
 */
extern void boot_cpus(unsigned int num_cpus, thread_startfunc_t func, void *arg,
                      bool async, bool sync, int random_seed);

/*
 * The thread_* functions are provided by the thread library
 * and are called by the application (except for thread_cpu_init()).
 */
extern int thread_create(thread_startfunc_t func, void *arg);
extern int thread_yield(void);
extern int thread_join(int thread_id);
extern int thread_lock(unsigned int lock);
extern int thread_unlock(unsigned int lock);
extern int thread_wait(unsigned int lock, unsigned int cond);
extern int thread_signal(unsigned int lock, unsigned int cond);
extern int thread_broadcast(unsigned int lock, unsigned int cond);

/*
 * The thread library will also provide thread_cpu_init():
 *     void thread_cpu_init(thread_startfunc_t func, void *arg);
 * thread_cpu_init() is declared in interrupt.h because it's called only
 * by boot_cpus() (not by the application).
 */
#endif /* _THREAD_H */
