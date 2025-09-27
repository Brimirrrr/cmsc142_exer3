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
		srand(7);
		for(i=0; i<n; i++) 
			swap(&a[i], &a[rand()%n]);
	}
}

void merge(int a[], int lower, int mid, int upper) {
	int *temp, i, j, k;

	temp = (int *)malloc((upper-lower+1)*sizeof(int));

	for(i=0, j=lower,k=mid+1; j<=mid || k<=upper; i++)
		// TO DO: implement this
		temp[i]= (k > upper || (j <= mid && a[j] <= a[k]))  ? a[j++]:a[k++];

	for(i=0, j=lower; j<=upper; i++, j++)
		a[j]=temp[i];
	free(temp);
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

//bubble sort
void bsort(int a[], int n) {
	int i, j;

	for(i=n-1; i>0; i--)
		for(j=1; j<=i; j++) 
			if(a[j-1] > a[j])
				swap(&a[j-1], &a[j]);
}

// selection sort
void ssort(int a[], int n) {
	int i, j, max;

	for(i=n-1; i>0; i--){
		max = 0;
		for(j=0; j<=i; j++)
			if(a[j] > a[max]) max = j;
		swap(&a[max], &a[i]);
	}
}

// merge sort
void msort(int a[], int lower, int upper) {
	int mid;

	if(upper-lower > 0) {
		mid = (lower+upper)/2;
		msort(a, lower, mid);
		msort(a, mid+1, upper);
		merge(a, lower, mid, upper);
	}
}

// quick sort
void qsort_own(int a[], int n, int left, int right) {
	// TO DO: implement this
}

// shell sort
void shsort(int a[], int n) {
	int k, i, j;

	// insertion sort
	for(k=n/2; k>0; k/=2)								// interval
		for(i=k; i<n; i=i+k)
			for(j=i; j>0;j=j-k)
				if(a[j-k]>a[j])
					swap(&a[j-1], &a[j]);
				else break;				
}

// used for comparison in built-in qsort
int cmpint(const void *p1, const void *p2) {
	return  *(int *) p1 -  *(int *) p2;
}


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

	t1 = clock();
	// isort(a,n);
	// bsort(a,n);
	ssort(a,n);
	//msort(a,0,n-1);
	// shsort(a,n);
	// qsort_own(a,n,0,n-1);
	// qsort(a,n, sizeof(int),cmpint);
	t2 = clock();

	// output(a,n);
	printf("time elapsed: %0.4f\n", (double)(t2-t1)/(double)CLOCKS_PER_SEC);
	return 0;
}