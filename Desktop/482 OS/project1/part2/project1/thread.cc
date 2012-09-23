#include <ucontext.h>
#include <vector>
#include "thread.h"
#include "interrupt.h"
using namespace std;

struct thread_struct
{
	int id;
	int cpuid;
	bool finished;
	ucontext* uc;
};

static int last_thread_id = 1; //thread_id is always positive
static vector<thread_struct> vector_threads;
static ucontext* vector_monitor_threads[MAX_CPUS];
static int running[MAX_CPUS];
int thread_schedule();

int thread_switch_to(int thread_id)
{
	int cpuid = cpu_id();
	int local_running = running[cpuid];
	running[cpuid] = thread_id;
	swapcontext(vector_threads[local_running].uc, vector_threads[thread_id].uc);
	running[cpuid] = local_running;
	return 0;
}

int thread_schedule()
{
	int cpuid = cpu_id();
	vector<thread_struct>::iterator it;
	for (it = vector_threads.begin() + 1; it < vector_threads.end(); it++)
	{
		if (it->finished == false && it->cpuid == cpuid)
		{
			thread_switch_to(it->id);
		}
	}
	return 0;
}

void thread_cpu_init(thread_startfunc_t func, void* arg)
{
	int cpuid = cpu_id();
	ucontext* ucontext_ptr = new ucontext;
	getcontext(ucontext_ptr);
	vector_monitor_threads[cpuid] = ucontext_ptr;
	running[cpuid] = 0; //0 means monitor is running
	thread_struct tcb;
	tcb.id = 0;
	tcb.cpuid = 0;
	tcb.finished = false;
	tcb.uc = ucontext_ptr;
	vector_threads.push_back(tcb);
	if (func)
	{
		thread_create(func, arg);
	}
	while (true)
		thread_yield();
}

int thread_create(thread_startfunc_t func, void* arg)
{
	int cpuid = cpu_id();
	ucontext* ucontext_ptr = new ucontext;
	getcontext(ucontext_ptr);
	char* stack = new char[STACK_SIZE];
	ucontext_ptr->uc_stack.ss_sp = stack;
	ucontext_ptr->uc_stack.ss_size = STACK_SIZE;
	ucontext_ptr->uc_stack.ss_flags = 0;
	ucontext_ptr->uc_link = vector_monitor_threads[cpuid]; //if thread returns, go back to monitor's context
	makecontext(ucontext_ptr, (void (*)())func, 1, arg);
	
	thread_struct tcb;
	tcb.id = last_thread_id;
	tcb.cpuid = cpuid;
	tcb.finished = false;
	tcb.uc = ucontext_ptr;

	vector_threads.push_back(tcb);
	return last_thread_id++;
}

int thread_yield()
{
	thread_schedule();
	return 0;
}

int thread_join(int thread_id)
{
	return 0;
}

int thread_lock(unsigned int lock)
{
	return 0;
}

int thread_unlock(unsigned int lock)
{
	return 0;
}

int thread_wait(unsigned int lock, unsigned int cond)
{
	return 0;
}

int thread_signal(unsigned int lock, unsigned int cond)
{
	return 0;
}

int thread_broadcast(unsigned int lock, unsigned int cond)
{
	return 0;
}

