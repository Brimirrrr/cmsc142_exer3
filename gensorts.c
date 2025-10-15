#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ISORT 0
#define BSORT 1
#define SSORT 2
#define MSORT 3
#define SHSORT 4
#define QSORT  5
#define QSORT_STD 6

/* ---------- utility ---------- */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* ---------- comparators and generic swap ---------- */
int cmpint(const void *p1, const void *p2) {
    int a = *(int *)p1;
    int b = *(int *)p2;
    return (a > b) - (a < b);  // returns 1, 0, or -1
}

void genswap(void *p1, void *p2, size_t size) {
    unsigned char *a = p1, *b = p2, tmp;
    for (size_t i = 0; i < size; i++) {
        tmp = a[i];
        a[i] = b[i];
        b[i] = tmp;
    }
}

/* ---------- base sorting algorithms ---------- */
void isort(int a[], int n) {
    for (int i=1; i<n; i++)
        for (int j=i; j>0; j--)
            if (a[j-1] > a[j])
                swap(&a[j-1], &a[j]);
            else break;
}

void bsort(int a[], int n) {
    for (int i=n-1; i>0; i--)
        for (int j=1; j<=i; j++)
            if (a[j-1] > a[j])
                swap(&a[j-1], &a[j]);
}

void ssort(int a[], int n) {
    for (int i=n-1; i>0; i--) {
        int max = 0;
        for (int j=1; j<=i; j++)
            if (a[j] > a[max]) max = j;
        swap(&a[max], &a[i]);
    }
}

void merge(int a[], int lower, int mid, int upper) {
    int *temp = malloc((upper - lower + 1) * sizeof(int));
    int i=lower, j=mid+1, k=0;
    while (i<=mid && j<=upper)
        temp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i<=mid) temp[k++] = a[i++];
    while (j<=upper) temp[k++] = a[j++];
    for (i=lower, k=0; i<=upper; i++, k++) a[i] = temp[k];
    free(temp);
}

void msort(int a[], int lower, int upper) {
    if (upper - lower <= 0) return;
    int mid = (lower + upper)/2;
    msort(a, lower, mid);
    msort(a, mid+1, upper);
    merge(a, lower, mid, upper);
}

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

int partition(int a[], int left, int right) {
    int pivot = a[right], i = left - 1;
    for (int j=left; j<right; j++)
        if (a[j] < pivot) swap(&a[++i], &a[j]);
    swap(&a[i+1], &a[right]);
    return i+1;
}

void qsort_own(int a[], int left, int right) {
    if (left < right) {
        int p = partition(a, left, right);
        qsort_own(a, left, p-1);
        qsort_own(a, p+1, right);
    }
}

/* ---------- general sort wrapper ---------- */
void gensort(void *base, int n, size_t size, int algo, int (*cmp)(const void*, const void*)) {
    int *a = (int *)base;

    switch (algo) {
        case ISORT:     isort(a, n); break;
        case BSORT:     bsort(a, n); break;
        case SSORT:     ssort(a, n); break;
        case MSORT:     msort(a, 0, n-1); break;
        case SHSORT:    shsort(a, n); break;
        case QSORT:     qsort_own(a, 0, n-1); break;
        case QSORT_STD: qsort(a, n, size, cmp); break;
        default:
            printf("Unknown sorting algorithm.\n");
    }
}

/* ---------- helpers ---------- */
void init(int a[], int n, int mode) {
    for (int i=0; i<n; i++)
        a[i] = i+1;
    if (mode == 1)  // descending
        for (int i=0; i<n/2; i++)
            swap(&a[i], &a[n-1-i]);
    else if (mode == 2) {  // random
        srand(7);
        for (int i=0; i<n; i++)
            swap(&a[i], &a[rand()%n]);
    }
}

/* ---------- main ---------- */
int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Usage: ./a.out <algo_id> <multiplier> <mode>\n");
        printf("Algo IDs: 0=isort 1=bsort 2=ssort 3=msort 4=shsort 5=qsort_own 6=qsort_std\n");
        return 0;
    }

    int algo = atoi(argv[1]);
    int mult = atoi(argv[2]);
    int mode = atoi(argv[3]);
    int n = 10000 * mult;

    int *a = malloc(n * sizeof(int));
    init(a, n, mode);

    clock_t t1 = clock();
    gensort(a, n, sizeof(int), algo, cmpint);
    clock_t t2 = clock();

    printf("Algorithm %d, Time elapsed: %.4f sec\n", algo, (double)(t2 - t1) / CLOCKS_PER_SEC);
    free(a);
    return 0;
}
