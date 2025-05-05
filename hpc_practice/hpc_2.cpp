#include <iostream>
#include <omp.h>

using namespace std;

void bubble(int a[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (a[j] > a[j + 1])
            {
                swap(a[j], a[j + 1]);
            }
        }
    }
}

void pBubble(int a[], int n)
{
    for (int i = 0; i < n; i++)
    {
#pragma omp for
        for (int j = 1; j < n; j += 2)
        {
            if (a[j] < a[j - 1])
                swap(a[j], a[j - 1]);
        }
#pragma omp barrier

#pragma omp for
        for (int j = 2; j < n; j += 2)
        {
            if (a[j] < a[j - 1])
                swap(a[j], a[j - 1]);
        }
    }
}

void merge(int arr[], int low, int mid, int high)
{
    int n1 = mid - low + 1;
    int n2 = high - mid;

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

void printArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main()
{
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    int arr[n], temp[n];
    double start_time, end_time;

    cout << "Enter elements: ";
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    cout << "Bubble Sort\n";

    copy(arr, arr + n, temp);
    start_time = omp_get_wtime();
    bubble(temp, n);
    end_time = omp_get_wtime();
    cout << "Sequential Bubble Sort took: " << end_time - start_time << " seconds\n";
    printArray(temp, n);

    copy(arr, arr + n, temp);
    start_time = omp_get_wtime();
#pragma omp parallel
    pBubble(temp, n);
    end_time = omp_get_wtime();
    cout << "Parallel Bubble Sort took: " << end_time - start_time << " seconds\n";
    printArray(temp, n);

    cout << "Merge Sort\n";

    copy(arr, arr + n, temp);
    start_time = omp_get_wtime();
    mergeSort(temp, 0, n - 1);
    end_time = omp_get_wtime();
    cout << "Sequential Merge Sort took: " << end_time - start_time << " seconds\n";
    printArray(temp, n);

    copy(arr, arr + n, temp);
    start_time = omp_get_wtime();
    parallelMergeSort(temp, 0, n - 1);
    end_time = omp_get_wtime();
    cout << "Parallel Merge Sort took: " << end_time - start_time << " seconds\n";
    printArray(temp, n);

    return 0;
}