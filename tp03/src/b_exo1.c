#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#define SIZE (int) 1e8 // 100 000 000
#define random_max (int) 1e9 // 1 000 000 000

typedef struct args_struct {
	int start;
	int end;
} args_struct;

typedef struct result_struct {
	int min;
	int max;
	pthread_mutex_t mutex;
	pthread_cond_t condition;
} result_struct;

static result_struct results = {
	.mutex = PTHREAD_MUTEX_INITIALIZER
};

int myArray[SIZE];
struct timeval begin, end;

// Search the minimum value of the array myArray
void * search_min(void* args) {
	args_struct *arguments = (struct args_struct *) args;
	pthread_mutex_lock(&results.mutex);
	for(int i=arguments->start;i<arguments->end;i++) {
		if(results.min > myArray[i]) {
			results.min = myArray[i];
		}
	}
	pthread_mutex_unlock(&results.mutex);
	return NULL;
}

// Search the maximum value of the array myArray
void * search_max(void* args) {
	args_struct *arguments = (struct args_struct *) args;
	pthread_mutex_lock(&results.mutex);
	for(int i=arguments->start;i<arguments->end;i++) {
		if(results.max < myArray[i]) {
			results.max = myArray[i];
		}
	}
	pthread_mutex_unlock(&results.mutex);
	return NULL;
}

// Creates the threads & assign equals portion of array to each
int createThreads(int nbThreads, void *(fct) (void *)) {
	pthread_t threads[nbThreads];
	args_struct args[nbThreads];
	int sizeThread = SIZE / nbThreads;
	int rest = SIZE % nbThreads;

	for(int i = 0; i < nbThreads; i++) {
		args[i].start = sizeThread*i;
		args[i].end = args[i].start + sizeThread;
		if(i == nbThreads - 1 && rest > 0) {
			args[i].end += 1;
		}
	}

	gettimeofday (&begin, NULL);
	for(int i = 0; i < nbThreads; i++) {
		if (pthread_create(&threads[i], NULL, fct, (void *) &args[i])) {
			return EXIT_FAILURE;
		}
	}
	for(int i = 0; i < nbThreads; i++) {
		if(pthread_join(threads[i], NULL)) {
			return EXIT_FAILURE;
		}
	}
	gettimeofday (&end, NULL);
	printf("search process time (%d threads): %fs\n", nbThreads, (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0));
	return 0;
}

void initThreads(int nbThreads, void *(fct) (void *)) {

	results.min = random_max;
	results.max = 0;

	if(nbThreads > 0) {
		createThreads(nbThreads, fct);
	} else {
		// Search with 0 threads
		args_struct args;
		args.start = 0;
		args.end = SIZE;

		gettimeofday (&begin, NULL);
		search_min((void *) &args);
		gettimeofday (&end, NULL);
		printf("search process time (%d threads): %fs\n", nbThreads, (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0));

		gettimeofday (&begin, NULL);
		search_max((void *) &args);
		gettimeofday (&end, NULL);
		printf("search process time (%d threads): %fs\n", nbThreads, (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0));
	}
}

int main(){

	// Array initialisation
	srand(time(NULL));
	for(int i=0;i < SIZE; i++) {
		myArray[i] = rand() % random_max;
	}

	// Search with 2 / 4 / 8 threads
	initThreads(0, NULL);
	printf("--------------------\n");
	initThreads(2, search_min);
	initThreads(2, search_max);
	printf("--------------------\n");
	initThreads(4, search_min);
	initThreads(4, search_max);
	printf("--------------------\n");
	initThreads(8, search_min);
	initThreads(8, search_max);

	return EXIT_SUCCESS;
}
