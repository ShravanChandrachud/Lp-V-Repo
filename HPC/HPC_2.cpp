#include <iostream>
#include <omp.h>

using namespace std;

// ======= Bubble Sort Functions =======
void bubble(int array[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (array[j] > array[j + 1])
                swap(array[j], array[j + 1]);
        }
    }
}

void pBubble(int array[], int n)
{
    for (int i = 0; i < n; ++i)
    {
#pragma omp for
        for (int j = 1; j < n; j += 2)
        {
            if (array[j] < array[j - 1])
                swap(array[j], array[j - 1]);
        }

#pragma omp barrier

#pragma omp for
        for (int j = 2; j < n; j += 2)
        {
            if (array[j] < array[j - 1])
                swap(array[j], array[j - 1]);
        }
    }
}

// ======= Merge Sort Functions =======
void merge(int arr[], int low, int mid, int high)
{
    int n1 = mid - low + 1;
    int n2 = high - mid;
    // const int n1 = nn1;
    // const int n2 = nn2;
    int left[n1], right[n2];

    for (int i = 0; i < n1; i++)
        left[i] = arr[low + i];

    for (int j = 0; j < n2; j++)
        right[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = low;

    while (i < n1 && j < n2)
    {
        if (left[i] <= right[j])
            arr[k++] = left[i++];
        else
            arr[k++] = right[j++];
    }

    while (i < n1)
        arr[k++] = left[i++];

    while (j < n2)
        arr[k++] = right[j++];
}

void mergeSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
}

void parallelMergeSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;

#pragma omp parallel sections
        {
#pragma omp section
            parallelMergeSort(arr, low, mid);

#pragma omp section
            parallelMergeSort(arr, mid + 1, high);
        }

        merge(arr, low, mid, high);
    }
}

// ======= Utility Function =======
void printArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << "\n";
}

// ======= Main Program =======
int main()
{
    const int n = 10;
    int arr[n];
    double start_time, end_time;

    // ---- Bubble Sort ----
    cout << "===== Bubble Sort =====\n";

    // Sequential Bubble Sort
    for (int i = 0, j = n; i < n; i++, j--)
        arr[i] = j;

    start_time = omp_get_wtime();
    bubble(arr, n);
    end_time = omp_get_wtime();
    cout << "Sequential Bubble Sort took: " << end_time - start_time << " seconds\n";
    printArray(arr, n);

    // Parallel Bubble Sort
    for (int i = 0, j = n; i < n; i++, j--)
        arr[i] = j;

    start_time = omp_get_wtime();
#pragma omp parallel
    pBubble(arr, n);
    end_time = omp_get_wtime();
    cout << "Parallel Bubble Sort took: " << end_time - start_time << " seconds\n";
    printArray(arr, n);

    // ---- Merge Sort ----
    cout << "\n===== Merge Sort =====\n";

    // Sequential Merge Sort
    for (int i = 0, j = n; i < n; i++, j--)
        arr[i] = j;

    start_time = omp_get_wtime();
    mergeSort(arr, 0, n - 1);
    end_time = omp_get_wtime();
    cout << "Sequential Merge Sort took: " << end_time - start_time << " seconds\n";
    printArray(arr, n);

    // Parallel Merge Sort
    for (int i = 0, j = n; i < n; i++, j--)
        arr[i] = j;

    start_time = omp_get_wtime();
    parallelMergeSort(arr, 0, n - 1);
    end_time = omp_get_wtime();
    cout << "Parallel Merge Sort took: " << end_time - start_time << " seconds\n";
    printArray(arr, n);

    return 0;
}
