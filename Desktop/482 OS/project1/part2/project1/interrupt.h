/*
 * interrupt.h -- interface to control the simulated hardware.
 *
 * This file should be included in the thread library, but NOT in the
 * application program that uses the thread library.
 */
#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include "thread.h"

/*
 * Returns the ID of the CPU this thread is running on, where
 * 0 <= ID < num_cpus
 */
unsigned int cpu_id();

/*
 * guard_test_and_set() and guard_clear() are used to atomically manipulate
 * a guard variable (which is allocated for you by the infrastructure).
 *
 * guard_test_and_set() atomically sets the guard to 1 and returns the prior
 * value.
 *
 * guard_clear() atomically sets the guard to 0.
 *
 * These functions should only be called by the thread library (not by
 * an application).
 *
 * The guard variable is initially false.
 */
bool guard_test_and_set();
void guard_clear();

/*
 * interrupt_disable() disables interrupts on the executing CPU.  Any
 * interrupt that arrives while interrupts are disabled will be saved
 * and delivered when interrupts are re-enabled.
 *
 * interrupt_enable() and interrupt_enable_suspend() enable interrupts
 * on the executing CPU.  They also specify the interrupt handler function,
 * i.e., the function that is called when an interrupt is delivered.
 *
 * interrupt_enable_suspend() atomically enables interrupts and suspends
 * the executing CPU until it receives an interrupt from another CPU.
 * The CPU will ignore timer interrupts while suspended
 *
 * interrupt_send() sends an interrupt to the specified CPU.
 *
 * These functions should only be called by the thread library (not by
 * an application).
 *
 * Each CPU starts with interrupts disabled.
 */
typedef int (*interrupt_handler_t) (void);
extern void interrupt_disable(void);
extern void interrupt_enable(interrupt_handler_t);
extern void interrupt_enable_suspend(interrupt_handler_t);
extern void interrupt_send(unsigned int);

/*
 * assert_interrupts_disabled() and assert_interrupts_enabled() can be used
 * as error checks inside the thread library.  They will assert (i.e. abort
 * the program and dump core) if the condition they test for is not met.
 */
#define assert_interrupts_disabled()					\
		assert_interrupts_private(__FILE__, __LINE__, true)
#define assert_interrupts_enabled()					\
		assert_interrupts_private(__FILE__, __LINE__, false)
/*
 * assert_interrupts_private() is a private function for the interrupt
 * library.  Your thread library should not call it directly.
 */
extern void assert_interrupts_private(const char *, int, bool);

/*
 * thread_cpu_init() is called by boot_cpus() to initialize and run a CPU;
 * After initialization, CPUs should run user threads as they become
 * available.  thread_cpu_init() should not return to the caller.
 * If func is non-NULL, thread_cpu_init() should also create a user thread
 * that executes func(arg).
 *
 * thread_cpu_init() should be provided by the thread library.
 */
extern void thread_cpu_init(thread_startfunc_t func, void *arg);

#endif /* _INTERRUPT_H */
