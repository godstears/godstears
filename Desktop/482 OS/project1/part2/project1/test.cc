#include <iostream>
#include <cstdlib>
#include "thread.h"
#include <assert.h>

using namespace std;

int g=0;

void loop(void *a)
{
	char *id;
	int i;

	id = (char *) a;
	cout <<"loop called with id " << (char *) id << endl;

	thread_lock(0);
	for (i=0; i<5; i++, g++) {
		cout << id << ":\t" << i << "\t" << g << endl;
		thread_unlock(0);
		thread_yield();
		thread_lock(0);
	}
	cout << id << ":\t" << i << "\t" << g << endl;
	thread_unlock(0);
}

void parent(void *a)
{
	long arg;
	arg = (long) a;

	cout << "parent called with arg " << arg << endl;
	thread_create((thread_startfunc_t) loop, (void *) "child thread");

	loop( (void *) "parent thread");
}

int main()
{
	boot_cpus(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}

