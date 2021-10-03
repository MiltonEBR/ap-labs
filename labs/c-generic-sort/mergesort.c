void merge(void* lineptr[], int left, int mid, int right, int (*comp)(void *, void *))
{
    int i, j, k;
    int side0 = mid - left + 1;
    int side1 = right - mid;
  
    void* L[side0];
	void* R[side1];

    for (i = 0; i < side0; i++){
		L[i] = lineptr[left + i];
	}
    for (j = 0; j < side1; j++){
        R[j] = lineptr[mid + 1 + j];
	}
  
    i = 0, j = 0, k = left;

    while (i < side0 && j < side1) {
		
        if ((*comp)(L[i], R[j]) < 0) {
            lineptr[k] = L[i];
            i++;
        }
        else {
            lineptr[k] = R[j];
            j++;
        }

        k++;
    }

    while (i < side0) {
        lineptr[k] = L[i];
        i++;
        k++;
    }
    while (j < side1) {
        lineptr[k] = R[j];
        j++;
        k++;
    }
}

void mergesort(void *lineptr[], int left, int right,
	       int (*comp)(void *, void *)) {
	if(left < right){
		int mid = (left + right) / 2;
		mergesort(lineptr, left, mid,comp);
		mergesort(lineptr, mid+1, right,comp);
		merge(lineptr, left, mid, right,comp);
	}

}

