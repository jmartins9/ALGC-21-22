#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned hash (char *str){
    unsigned hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

//______________________________________________________________________________CHAINING______________________________________________________________________________________


#define Size 11

typedef struct nodo { 
    char *chave;
    int ocorr;
    struct nodo *prox;
} *THash [Size];



void initEmpty (THash t) {
    int i;
    for (i=0;i<Size;i++) {
        t[i]=NULL;
    }
}

void add (char *s, THash t) {
    int n = (hash(s))%Size;
    struct nodo *p,*new;  
    for (p=t[n];p && strcmp(p->chave,s)!=0;p=p->prox);

    if (p!=NULL) p->ocorr++;
    else {
        p = malloc(sizeof(struct nodo));
        p->chave = strdup(s);
        p->ocorr = 1;
        p->prox = t[n];
        t[n] = p;
    }
}

int lookup (char *s, THash t) {
    int i=(hash(s))%Size,r=0;
    struct nodo *p;
    for (p=t[i];p && strcmp(p->chave,s)!=0;p=p->prox);
    if (p!=NULL) r = p->ocorr;
    return r; 
}


int removeElem (char *s, THash t) {
    int i=(hash(s))%Size,r=1;
    struct nodo *p,*ant=t[i];
    for (p=t[i];p && strcmp(p->chave,s)!=0;p=p->prox) ant = p;
    if (p!=NULL) {
        if (ant==p && p->ocorr==1) {
            p = p->prox;
            free(ant->chave);
            free(ant);    
        }
        else if (p->ocorr==1) {
            ant->prox=p->prox;
            free(p->chave);
            free(p);
        }
        else if (p->ocorr>1)  p->ocorr--;
        r=0;
    }
    return r; 
}

//______________________________________________________________________________OPEN ADRESSING______________________________________________________________________________________

#define Free 0
#define Used 1
#define Del 2
typedef struct bucket {
    int status; // Free | Used | Del
    char *chave; 
    int ocorr;
} Thash [Size];


int where (char *s, Thash t) {
    int h = (hash(s))%Size;
    int a = h;
    while (t[a].status==Used || t[a].status==Del) {
        if (t[a].status==Used && strcmp(t[a].chave,s)==0) break;
        a++;
        if (a==Size) h=0;
        if (a==h) return -1;  
    }
    return a;
}

void initempty (Thash t) {
    int i;
    for (i=0;i<Size;i++) {
        t[i].chave=NULL;
        t[i].ocorr=0;
        t[i].status=Free;
    }
}

int auxadd (char *s,Thash t) {
    int h = (hash(s))%Size;
    int a = h;
    while (t[a].status==Used) {
        if (t[a].status==Used && strcmp(t[a].chave,s)==0) break;
        a++;
        if (a==Size) h=0;
        if (a==h) return -1;  
    }
    return a;
}

void Add (char *s,Thash t) {
    int i = auxadd(s,t);
    if (i!=-1) {
        if (t[i].status==Used) {
            t[i].ocorr++;
        }
        else {
            t[i].chave=strdup(s);
            t[i].ocorr=1;
            t[i].status=Used;
        }
    }
}

int Lookup (char *s, Thash t) {
    int i = where(s,t);
    int r=0;
    if (i!=-1) {
        if (t[i].status==Used) r=t[i].ocorr;
    }
    return r;
}

int Remove (char *s, Thash t) {
    int i = where(s,t);
    int r=1;
    if (i!=-1) {
        if (t[i].status==Used) {
            t[i].status=Del;
            t[i].ocorr=0;
            r=0;
        }
    }
    return r;
}

int garbcollection (Thash t) {
    int i,a;
    Thash nova;
    initempty(nova);
    for(i=0; i<Size; i++){
       if(t[i].status==Used){
           a = where(t[i].chave,nova);
           nova[a] = t[i];
       }
    }
    for(i=0; i<Size; i++){
        t[i] = nova[i];
    }
    return 0;
}



int main () {
    printf("Valores da hash:\n");              
    char str[5]="joao";
    printf("%d\n",hash(str)%Size);              
    char str1[6]="maria";
    printf("%d\n",hash(str1)%Size);
    char str2[3]="ze";
    printf("%d\n",hash(str2)%Size);
    char str3[6]="manel";
    printf("%d\n",hash(str3)%Size);
    char str4[5]="tone";
    printf("%d\n",hash(str4)%Size);
    char str5[9]="ambrosio";
    printf("%d\n",hash(str5)%Size);
    char str6[6]="maira";
    printf("%d\n",hash(str6)%Size);
    

    printf("\n------CHAINING-------\n");
    THash h;
    initEmpty(h);


    add(str,h);
    add(str1,h);
    add(str2,h);
    add(str3,h);
    add(str4,h);
    add(str5,h);

    int i;
    struct nodo *p;
    printf("\nHash Table:\n");
    for (i=0;i<Size;i++) {
        printf("%d : ",i);
        if (h[i]!= NULL) {
            for (p=h[i];p;p=p->prox) {   
                printf("%s -> %d ; ",p->chave,p->ocorr);
            }
        }
        printf("\n");
    } 


    printf("\nLookup errado: %d\n",lookup("hello",h));
    printf("Lookup certo: %d\n",lookup(str3,h));

    removeElem(str1,h);
   
    printf("\nHash Table Removed:\n");
    for (i=0;i<Size;i++) {
        printf("%d : ",i);
        if (h[i]!= NULL) {
            for (p=h[i];p;p=p->prox) {   
                printf("%s - %d ; ",p->chave,p->ocorr);
            }
        }
        printf("\n");
    } 


    printf("\n------OPEN ADRESSING-------\n");
  
    Thash t;
    initempty(t);
    Add(str,t);
    Add(str1,t);
    Add(str2,t);
    Add(str3,t);
    Add(str4,t);
    Add(str5,t);
    Add(str6,t);
    
    printf("\nHash Table:\n");
    for (i=0;i<Size;i++) {
        printf("%s - %d - %d\n",t[i].chave,t[i].ocorr,t[i].status);
    }

    printf("\nLookup errado: %d\n",Lookup("hello",t));
    printf("Lookup certo: %d\n",Lookup(str3,t));

    Remove(str1,t);    
    printf("\nHash Table Removed:\n");
    for (i=0;i<Size;i++) {
        printf("%s - %d - %d\n",t[i].chave,t[i].ocorr,t[i].status);
    }

    garbcollection(t);
}