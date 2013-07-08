#ifndef QSORT_H
#define QSORT_H
template<class T>
int partition(T value[], int low, int high)
{
 T tmp = value[low];
 T pivotkey = value[low];
 while (low < high)
 {
  while (low < high && value[high] <= pivotkey) --high;
  value[low] = value[high];
  while (low < high && value[low] >= pivotkey) ++low;
  value[high] = value[low];
 }
 value[low] = tmp;
 return low;
}
template<class T>
void qSort(T value[],int low,int high)
{
 if (low < high)
 {
  int pivotloc = partition(value,low, high);
  qSort(value, low, pivotloc - 1);
  qSort(value, pivotloc + 1, high);
 }
}


template<typename T>void merge(T a[],int i,int j,int k){
int b=i,c=j+1,d=0;
T *temp=new T[k-i+1];
while(b<=j&&c<=k){
if(a[b]>a[c]) temp[d++]=a[b++];
else temp[d++]=a[c++];
}
if(b<=j){
while(b<=j) temp[d++]=a[b++];
}
if(c<=k){
while(c<=k)temp[d++]=a[c++];
}
d=0;
for(b=i;b<=k;b++)
a[b]=temp[d++];
delete[] temp;
}

template<typename T>void msort(T a[],int i,int j){
if(j>i){
int mid=(i+j)/2;
msort(a,i,mid);
msort(a,mid+1,j);
merge(a,i,mid,j);
}
}

template<typename T> void merge_sort(T a[],int n){
msort(a,0,n-1);
}

#endif // QSORT_H
