#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void output(int a[], int n) {
	int i;
	for(i=0; i<n; i++)
		printf("%i\n", a[i]);
	printf("\n");
}

void init(int a[], int n, int mode) {
	int i;

	//ascending
	if(mode == 0) 
		for(i=0; i<n; i++)
			a[i] = i+1;

	//descending
	if(mode == 1)
		for(i=0; i<n; i++)
			a[i] = n-i;

	// random
	if(mode == 2) {
		for(i=0; i<n; i++) 
			a[i] = i+1;
		// set seed
		srand(37);
		for(i=0; i<n; i++) 
			swap(&a[i], &a[rand()%n]);
	}
}

// insertion sort
void isort(int a[], int n) {
	int i, j;

	for(i=1; i<n; i++)
		for(j=i; j>0;j--)
			if(a[j-1]>a[j])
				swap(&a[j-1], &a[j]);
			else break;
}

// parameters expected: input multiplier, mode(1-asc, 2-desc, 3-random)
int main(int argc, char **argv) {
	int n=10000;							// set depending on the machine
	int *a, i, multiplier, mode;
	clock_t t1, t2;

	if(argc < 2) {
		printf("File usage: ./a.out <multiplier> <mode>");
		return 0;
	}
	multiplier = atoi(argv[1]);
	n = multiplier*n;
	mode = atoi(argv[2]);
	a = (int*)malloc(sizeof(int)*(n*multiplier));

	init(a, n, mode);
	// output(a, n);

	t1 = clock();
	isort(a,n);
	t2 = clock();

	output(a,n);
	printf("time elapsed: %0.4f\n", (double)(t2-t1)/(double)CLOCKS_PER_SEC);
	return 0;
}