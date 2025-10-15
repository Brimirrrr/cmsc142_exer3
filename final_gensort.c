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

/* ---------- comparators ---------- */
int cmpint_asc(const void *p1, const void *p2) {
    int a = *(int *)p1, b = *(int *)p2;
    return (a > b) - (a < b);
}
int cmpint_desc(const void *p1, const void *p2) {
    int a = *(int *)p1, b = *(int *)p2;
    return (a < b) - (a > b);
}

/* ---------- sorting algorithms ---------- */
void isort(int a[], int n, int descending) {
    for (int i = 1; i < n; i++)
        for (int j = i; j > 0; j--) {
            if ((!descending && a[j - 1] > a[j]) || (descending && a[j - 1] < a[j]))
                swap(&a[j - 1], &a[j]);
            else break;
        }
}

void bsort(int a[], int n, int descending) {
    for (int i = n - 1; i > 0; i--)
        for (int j = 1; j <= i; j++)
            if ((!descending && a[j - 1] > a[j]) || (descending && a[j - 1] < a[j]))
                swap(&a[j - 1], &a[j]);
}

void ssort(int a[], int n, int descending) {
    for (int i = n - 1; i > 0; i--) {
        int extreme = 0;
        for (int j = 1; j <= i; j++)
            if ((!descending && a[j] > a[extreme]) || (descending && a[j] < a[extreme]))
                extreme = j;
        swap(&a[extreme], &a[i]);
    }
}

void merge(int a[], int lower, int mid, int upper, int descending) {
    int *temp = malloc((upper - lower + 1) * sizeof(int));
    int i = lower, j = mid + 1, k = 0;
    while (i <= mid && j <= upper)
        temp[k++] = (!descending && a[i] <= a[j]) || (descending && a[i] >= a[j]) ? a[i++] : a[j++];
    while (i <= mid) temp[k++] = a[i++];
    while (j <= upper) temp[k++] = a[j++];
    for (i = lower, k = 0; i <= upper; i++, k++) a[i] = temp[k];
    free(temp);
}

void msort(int a[], int lower, int upper, int descending) {
    if (lower >= upper) return;
    int mid = (lower + upper) / 2;
    msort(a, lower, mid, descending);
    msort(a, mid + 1, upper, descending);
    merge(a, lower, mid, upper, descending);
}

void shsort(int a[], int n, int descending) {
    for (int gap = n / 2; gap > 0; gap /= 2)
        for (int i = gap; i < n; i++)
            for (int j = i - gap; j >= 0 &&
                ((!descending && a[j] > a[j + gap]) || (descending && a[j] < a[j + gap])); j -= gap)
                swap(&a[j], &a[j + gap]);
}

int partition(int a[], int left, int right, int descending) {
    int pivot = a[right], i = left - 1;
    for (int j = left; j < right; j++)
        if ((!descending && a[j] < pivot) || (descending && a[j] > pivot))
            swap(&a[++i], &a[j]);
    swap(&a[i + 1], &a[right]);
    return i + 1;
}

void qsort_own(int a[], int left, int right, int descending) {
    if (left < right) {
        int p = partition(a, left, right, descending);
        qsort_own(a, left, p - 1, descending);
        qsort_own(a, p + 1, right, descending);
    }
}

/* ---------- general sort wrapper ---------- */
void gensort(int *a, int n, int algo, int descending) {
    switch (algo) {
        case ISORT: isort(a, n, descending); break;
        case BSORT: bsort(a, n, descending); break;
        case SSORT: ssort(a, n, descending); break;
        case MSORT: msort(a, 0, n - 1, descending); break;
        case SHSORT: shsort(a, n, descending); break;
        case QSORT: qsort_own(a, 0, n - 1, descending); break;
        case QSORT_STD:
            qsort(a, n, sizeof(int),
                  descending ? cmpint_desc : cmpint_asc);
            break;
        default:
            printf("Unknown algorithm ID.\n");
    }
}

/* ---------- helpers ---------- */
void init(int a[], int n, int order) {
    for (int i = 0; i < n; i++)
        a[i] = i + 1;

    if (order == 2) {  // descending
        for (int i = 0; i < n / 2; i++)
            swap(&a[i], &a[n - 1 - i]);
    } 
    else if (order == 3) {  // random
        srand(time(NULL));  // randomize seed per run
        for (int i = 0; i < n; i++)
            swap(&a[i], &a[rand() % n]);
    }
}

/* ---------- main ---------- */
int main(int argc, char **argv) {
    if (argc < 5) {
        printf("Usage:\n");
        printf("  ./sort <algo_id> <size_multiplier> <order> <asc|desc>\n");
        printf("  order: 1=ascending, 2=descending, 3=random\n");
        printf("Example: ./sort 3 2 3 asc\n");
        return 0;
    }

    int algo = atoi(argv[1]);
    int multiplier = atoi(argv[2]);
    int order = atoi(argv[3]);
    int descending = (strcmp(argv[4], "desc") == 0);

    int n = 10000 * multiplier;
    int *a = malloc(n * sizeof(int));
    init(a, n, order);

    /* ---------- Write unsorted input if random ---------- */
    if (order == 3) {
        FILE *unsorted = fopen("unsorted_numbers.txt", "w");
        if (!unsorted) {
            perror("unsorted_numbers.txt");
            free(a);
            return 1;
        }
        for (int i = 0; i < n; i++)
            fprintf(unsorted, "%d\n", a[i]);
        fclose(unsorted);
        printf("Random input saved to unsorted_numbers.txt\n");
    }

    clock_t t1 = clock();
    gensort(a, n, algo, descending);
    clock_t t2 = clock();

    printf("Sorted %d ints (input order: %s, output: %s). Algorithm %d, Time: %.4f sec\n",
           n,
           (order == 1 ? "ascending" : (order == 2 ? "descending" : "random")),
           descending ? "descending" : "ascending",
           algo, (double)(t2 - t1) / CLOCKS_PER_SEC);

    /* ---------- Always overwrite output file ---------- */
    FILE *out = fopen("sorted_numbers.txt", "w");
    if (!out) {
        perror("sorted_numbers.txt");
        free(a);
        return 1;
    }

    for (int i = 0; i < n; i++)
        fprintf(out, "%d\n", a[i]);
    fclose(out);

    printf("Sorted output written to sorted_numbers.txt\n");

    free(a);
    return 0;
}
