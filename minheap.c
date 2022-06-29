#include <stdio.h>
#define Max 100


typedef struct pQueue {
    int valores [Max];
    int tamanho;
} PriorityQueue;

void initHeap (PriorityQueue *h) {
    h->tamanho=0;
}

void swap(int h[], int xp, int yp){
    int temp = h[xp];
    h[xp] = h[yp];
    h[yp] = temp;
}


int esq(int n){
    return 2*n+1;
}

int dir(int n){
    return 2*n+2;
}

int pai(int n) {
    return ((n-1)/2);
}


void bubbleup (int i, int h[]) {
 
    while (i>0 && h[pai(i)]>h[i]) {
        swap(h,i,pai(i));
        i = pai(i);
    }
}


void bubbledown (int i,int h[],int n) {
    
    while(i<n/2 && (h[i]>h[esq(i)] || (dir(i)<n && h[i]>h[dir(i)]))) {

        if (dir(i)<n && h[esq(i)]>h[dir(i)]) {
            swap(h,i,dir(i));
            i=dir(i);
        }  
        else {
            swap(h,i,esq(i));
            i=esq(i);
        }
    }
}

void insertHeap (PriorityQueue *h,int x) {
    h->valores[h->tamanho++] = x;
    bubbleup(h->tamanho-1,h->valores);
} 


void removeheap (PriorityQueue *q, int *x) {
    if (q->tamanho == 0) return;
    else {
        *x = q->valores[0];
        q->valores[0] = q->valores[--q->tamanho];
        bubbledown(0,q->valores,q->tamanho);
    }
}



/*
void heapify (int v[],int N) {
    int i;
    for (i=1;i<N;i++) {
        bubbleup(i,v);
    }
}
*/
void heapify (int h[],int N) {
    int i;
    for (i=N/2;i>=0;i--) {
        bubbledown(i,h,N);
    }
}


void ordenaheap(int h[], int N){
    int i;
    int v[N];
    for (i=N-1;i>=0;i--) {
        v[i]=h[0];
        h[0]=h[i];   
        bubbledown(0,h,i);
    }
    for (i=0;i<N;i++) {
        h[i]=v[i];
    }
}




void sequencia (int v[],int N,int k) {
    int i;

    heapify(v,k);
    for(i=k;i<N;i++) {
        if (v[0]<v[i]) {
            v[0]=v[i];
            bubbledown(0,v,k);
        }
    }
}


int main ()  {

    PriorityQueue h;
    int i,x;

    initHeap(&h);
    
    insertHeap(&h, 10);
    insertHeap(&h, 30);
    insertHeap(&h, 40);
    insertHeap(&h, 60);
    insertHeap(&h, 100);
    insertHeap(&h, 20);
    insertHeap(&h, 90);
    insertHeap(&h, 50);
    insertHeap(&h, 80);
    insertHeap(&h, 110);
    insertHeap(&h, 220);
    insertHeap(&h, 95);
    insertHeap(&h, 55);
    insertHeap(&h, 85);


    printf("Heap construída (capacidade %d):\n", h.tamanho);
    for (i = 0; i < h.tamanho; i++)
        printf("%d\n", h.valores[i]);
   
    ordenaheap(h.valores,h.tamanho);
    printf("\nHeap ordenada (capacidade %d):\n", h.tamanho);
    for (i = 0; i < h.tamanho; i++)
        printf("%d\n", h.valores[i]);

}
