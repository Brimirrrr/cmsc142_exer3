/***
Testing sorts using permutations of 1..N (Sir Cariño template)
gcc permu.c && ./a.out 4
*/

#include<stdio.h>
#include<stdlib.h>

/* Prototypes */
void LFSamplesort(int A[], int first, int last);
void Leapfrog(int A[], int s1, int ss, int u);
void MergeSort(int A[], int first, int last);
void merge(int a[], int lower, int mid, int upper);
void isort(int a[], int n);
void bsort(int a[], int n);
void ssort(int a[], int n);
void shsort(int a[], int n);
void qsort_own(int a[], int n, int left, int right);
static inline void swap(int *x, int *y);

/* ----------------- LFS (given) ----------------- */
void LFSamplesort(int A[], int first, int last)
{
  int s;
  if (last > first) {
    s = 1;
    while (s <= (last-first-s)) {
      Leapfrog(A, first, first+s-1, first+s+s);
      s += (s+1);
    }
    Leapfrog(A, first, first+s-1, last);
  }
}

void Leapfrog(int A[], int s1, int ss, int u)
{
  int i,j,k, sm, v,t;
  if (s1 > ss) LFSamplesort(A, ss+1, u);
  else if (u > ss) {
    sm = (s1+ss) / 2;
    /* Partition */
    v = A[sm];
    j = ss;
    for(i=ss+1; i <= u; i++) {
      if (A[i] < v) {
        j++;
        t = A[j];
        A[j] = A[i];
        A[i] = t;
      }
    }
    /* Move Sample */
    if (j > ss) {
      for (k=j, i=ss; i >= sm; k--, i--) {
        t = A[i];
        A[i] = A[k];
        A[k] = t;
      }
    }
    Leapfrog(A, s1, sm-1,sm+j-ss-1);
    Leapfrog(A, sm+j-ss+1, j, u);
  }
}

/* ----------------- Merge Sort (given) ----------------- */
void MergeSort(int A[], int first, int last) {
  if (first < last) {
    int mid = first + (last - first) / 2;
    MergeSort(A, first, mid);
    MergeSort(A, mid + 1, last);
    merge(A, first, mid, last);
  }
}

void merge(int a[], int lower, int mid, int upper) {
  int *temp, i, j, k;
  temp = (int *)malloc((upper - lower + 1) * sizeof(int));
  if (!temp) {
    fprintf(stderr, "malloc failed in merge()\n");
    exit(1);
  }
  for (i = 0, j = lower, k = mid + 1; j <= mid || k <= upper; i++)
    temp[i] = (k > upper || (j <= mid && a[j] <= a[k])) ? a[j++] : a[k++];
  for (i = 0, j = lower; j <= upper; i++, j++)
    a[j] = temp[i];
  free(temp);
}

/* ----------------- Simple sorts (given) ----------------- */
void isort(int a[], int n) {
  int i, j;
  for (i = 1; i < n; i++)
    for (j = i; j > 0; j--)
      if (a[j - 1] > a[j])
        swap(&a[j - 1], &a[j]);
      else
        break;
}

void bsort(int a[], int n) {
  int i, j;
  for (i = n - 1; i > 0; i--)
    for (j = 1; j <= i; j++)
      if (a[j - 1] > a[j])
        swap(&a[j - 1], &a[j]);
}

void ssort(int a[], int n) {
  int i, j, max;
  for (i = n - 1; i > 0; i--) {
    max = 0;
    for (j = 0; j <= i; j++)
      if (a[j] > a[max]) max = j;
    swap(&a[max], &a[i]);
  }
}

void shsort(int a[], int n) {
  int k, i, j;
  for (k = n / 2; k > 0; k /= 2) {
    for (i = k; i < n; i += k) {
      for (j = i; j >= k; j -= k) {
        if (a[j - k] > a[j]) swap(&a[j - k], &a[j]);
        else break;
      }
    }
  }
}

/* ----------------- Swap ----------------- */
static inline void swap(int *x, int *y) { int t = *x; *x = *y; *y = t; }

/* ----------------- Optimized QuickSort ----------------- */
#define CUTOFF 16  /* insertion-sort cutoff */

static inline int median3_index(int a[], int i, int j, int k) {
  int x=a[i], y=a[j], z=a[k];
  if (x < y) { if (y < z) return j; else if (x < z) return k; else return i; }
  else       { if (x < z) return i; else if (y < z) return k; else return j; }
}

static inline int choose_pivot_index(int a[], int low, int high) {
  int len = high - low + 1, mid = low + len/2;
  if (len > 40) {
    int step = len/8;
    int m1 = median3_index(a, low,           low+step,     low+2*step);
    int m2 = median3_index(a, mid-step,      mid,          mid+step);
    int m3 = median3_index(a, high-2*step,   high-step,    high);
    return median3_index(a, m1, m2, m3);     /* Tukey ninther */
  } else {
    return median3_index(a, low, mid, high); /* median-of-3 */
  }
}

static inline void insertion_sort_range(int a[], int lo, int hi) {
  for (int i = lo + 1; i <= hi; i++) {
    int x = a[i], j = i;
    while (j > lo && a[j - 1] > x) { a[j] = a[j - 1]; j--; }
    a[j] = x;
  }
}

/* 3-way DNF partition; returns [*lt..*gt] equal-to-pivot block */
static inline void partition3(int a[], int low, int high, int *lt, int *gt) {
  int pidx = choose_pivot_index(a, low, high);
  swap(&a[low], &a[pidx]);
  int pivot = a[low];
  int i = low + 1, lt_idx = low, gt_idx = high;
  while (i <= gt_idx) {
    if (a[i] < pivot)        swap(&a[i++], &a[lt_idx++]);
    else if (a[i] > pivot)   swap(&a[i],   &a[gt_idx--]);
    else                     i++;
  }
  *lt = lt_idx; *gt = gt_idx;
}

/* Public quicksort: sorts a[left..right] in place */
void qsort_own(int a[], int n, int left, int right) {
  (void)n; /* signature compatibility with your harness */
  while (left < right) {
    int size = right - left + 1;
    if (size <= CUTOFF) { insertion_sort_range(a, left, right); return; }

    int lt, gt;
    partition3(a, left, right, &lt, &gt);

    /* Recurse on smaller side first; tail-eliminate the other */
    if (lt - left < right - gt) {
      if (left < lt - 1) qsort_own(a, n, left, lt - 1);
      left = gt + 1;
    } else {
      if (gt + 1 < right) qsort_own(a, n, gt + 1, right);
      right = lt - 1;
    }
  }
}

/* ----------------- Print ----------------- */
void printA(int a[], int n){
  int i;
  for(i=0;i<n;i++) printf("%2i ",a[i]);
  printf("\n");
}

/* ----------------- Driver (permutation tester) ----------------- */
int main(int argc, char *argv[]){
  int n=atoi(argv[1]);
  int a[n];

  /* Sir Cariño's Template */
  int option[n+2][n+2];
  int nopts[n+2];
  int move, start;
  int i,c;

  move=start=0;
  nopts[start]=1;
  option[start][nopts[start]]=0;

  while(nopts[start]>0){
    if(nopts[move]>0) {
      nopts[++move]=0;
      if(move==n+1){
        for(i=0;i<n;i++)
          a[i]=option[i+1][nopts[i+1]];
        printA(a,n);

        /* Pick the sorter you want to test: */
        // LFSamplesort(a,0,n-1);
        // MergeSort(a, 0, n - 1);
        // isort(a, n);
        // bsort(a, n);
        // ssort(a, n);
        // shsort(a, n);
        qsort_own(a, n, 0, n - 1);   /* <-- optimized QuickSort */

        // printA(a,n);
        for(i=0;i<n-1;i++)
          if(a[i]>a[i+1]) break;
        if(i<n-1){
          printf("sorting error: ");
          printA(a,n);
          exit(1);
        }
      }
      else {
        for(c=n;c>=1;c--){
          for(i=move-1;i>=1;i--)
            if(option[i][nopts[i]]==c) break;
          if(!(i>=1)) option[move][++nopts[move]]=c;
        }
      }
    }
    else nopts[--move]--; /* backtrack */
  }
}
