#include <stdio.h>
#include <stdlib.h>


#define NV 9

typedef struct aresta {
  int dest;
  int custo;
  struct aresta *prox;
} *GrafoL [NV];

typedef int GrafoM [NV][NV];

void graphMtoL (GrafoL gl, GrafoM gm, int n) {
    int i, j;
    struct aresta *head, *new;
    
    for (i = 0; i<n; i++) {
        head = NULL;
        for (j = n-1; j>=0; j--) {
            if (gm[i][j] != 0) {
                new = malloc(sizeof(struct aresta));
                new -> dest = j;
                new -> custo = gm[i][j];
                new -> prox = head;
                head = new;
            }
        }
        gl[i] = head;
    }
}

void printGraphM(GrafoM g, int n) {
    int i,j;
    for (i=0; i<n; i++){
        for(j=0; j<n; j++){
            printf("%d ",g[i][j]);
        }
        printf("\n");
    }
}



void printGraphL(GrafoL g, int n) {
    int i;
    struct aresta *v;
    for(i=0 ;i<n; i++){
        v = g[i];
        printf("%d ",i);
        while(v) {
        
        printf("--> (%d,%d) ",v->dest , v->custo);
        
        v = v -> prox;
        }
        printf("\n");
    }
}


int existe (GrafoL h,int o,int d) {
    struct aresta *p;
    for (p=h[o];p;p=p->prox) {
        if (p->dest == d) return 1;  
    }
    return 0;
}
          

int homomorfOK (GrafoL g, GrafoL h, int f[]) {
    int i,r=1;
    struct aresta *p;
    for (i=0;r && i<9;i++) {
        for (p=g[i];r && p;p=p->prox) {
            r = existe(h,f[i],f[p->dest]);
        }
    }
    return r;
}

void initGrafoL(GrafoL g) {
  int i;
  for (i=0; i<NV; i++)
    g[i] = NULL;
}


void inverte (GrafoL in, GrafoL out) {
    int i;
    struct aresta *p,*n;
    initGrafoL(out);
    for (i=NV-1;i>=0;i--) {
        for (p=in[i];p;p=p->prox) {
            n = malloc (sizeof(struct aresta));           
            n->dest = i;
            n->custo = p->custo;
            n->prox = out[p->dest]; 
            out[p->dest]=n;
        }         
    } 
}

int adj (GrafoL g, int cor[], int c, int o) {
    int r=1;
    struct aresta *p;
    for (p=g[o];r && p;p=p->prox) {
        if (c != cor[p->dest]) r=0;
    }
    return r;
}


int colorOK (GrafoL g, int cor[]) {
    int i,r=1;
    struct aresta *p;
    for (i=0;r && i<NV;i++) {
        for (p=g[i];r && p;p=p->prox) {
            r = adj(g,cor,cor[i],p->dest);
        }          
    }
    return r;
}


#define WHITE 0
#define GRAY 1
#define BLACK 2


void df_visit(GrafoL g, int s, int cor[]) { 
    struct aresta *p;

    cor[s]=GRAY;

    for(p=g[s];p;p=p->prox) {
        if (cor[p->dest]==WHITE) {
            df_visit(g,p->dest,cor);      
        }
    }

    cor[s]=BLACK;

}    


void dfs(GrafoL g, int n) {
    int i,cor[n];

    for (i=0;i<n;i++) {
        cor[i]=WHITE; 
    } 

    for (i=0;i<n;i++) {
        if (cor[i]==WHITE) {
            df_visit(g,i,cor);
        }
    }
}

void bf_visit(GrafoL g, int s, int n, int cor[]) {
    int queue[n],frist=0,last=0,u;
    struct aresta *p;

    cor[s]=GRAY;  
    queue[last++]=s;

    while (frist < last) {
        u = queue[frist++];
        for (p=g[u];p;p=p->prox) {
            if (cor[p->dest]==WHITE) {
                cor[p->dest]=GRAY;
                queue[last++]=p->dest;
            }
        } 
        cor[u]=BLACK;      
    }

}


void bfs (GrafoL g, int n) {
    int i,cor[n];

    for (i=0;i<n;i++) {
        cor[i]=WHITE; 
    } 

    for (i=0;i<n;i++) {
        if (cor[i]==WHITE) {
            bf_visit(g,i,n,cor);
        }
    }
}


int shortestPath (GrafoL g, int s, int  d, int p[]) {
    int parent[NV],queue[NV],visit[NV],f=0,l=0,i,u;
    struct aresta *a;
    
    for (i=0;i<NV;i++) {
        parent[i]=-1;
        visit[i]=0;
    }
    queue[l++]=s;
    visit[s]=1;    

    while (f<l) {
        u = queue[f++];
        for (a=g[u];a;a=a->prox) { 
            if (!visit[a->dest]) {
                visit[a->dest]=1;
                parent[a->dest]=u;
                queue[l++]=a->dest;
            }
            if (a->dest==d) break;
        }
    }

    if (parent[d]==-1) return -1;

    int tam=0;
    u = d;

    while (u!=-1) {
        u = parent[u];
        tam++;
    }

    u = d;
    for (i=tam-1;i>=0;i--) {
         p[i]=u;
         u = parent[u];
    }


    return tam;
}


void dists (GrafoL g, int s, int dist[]) {
    int q[NV],f=0,l=0,visit[NV],i,u;
    struct aresta *p;

    for (i=0;i<NV;i++) {
        visit[i]=0;
        dist[i]=-1;
    }
    
    visit[i]=1;
    dist[i]=0;
    q[l++]=s;

    while (f<l) {
        u = q[f++];
        for (p=g[u];p;p=p->prox) {
            if (visit[p->dest]==0) {
                visit[p->dest]=1;
                q[l++]=p->dest;
                dist[p->dest]=dist[u]+1;
            }
        }
        
    }
}


int maisLonga (GrafoL g, int s, int path[]) {
    int queue[NV],visit[NV],dist[NV],parent[NV];
    int f=0,l=0,tam=0,i,u,v;
    struct aresta *p;

    for (i=0;i<NV;i++) {
        visit[i]=0;
        dist[i]=-1;
        parent[i]=-1;
    }
    
    visit[s]=1;
    dist[s]=0;
    queue[l++]=s;

    while (f<l) {
        u = queue[f++];
        for (p=g[u];p;p=p->prox) {
            if (!visit[p->dest]) {
                visit[p->dest]=1;
                dist[p->dest]=dist[u]+1;
                parent[p->dest]=u;
                queue[l++]=p->dest;
            }
        }
    }
    
    tam = dist[u]+1;
    for (i=tam-1;i>=0;i--) {
        path[i]=u;
        u = parent[u];
    }


    return tam;
}

void dfcomp_visit (GrafoL g, int comp[], int s,int c) {
    struct aresta *p;
    int i;

    comp[s]=c;

    for (p=g[s];p;p=p->prox) {
        if (comp[p->dest]==0) {
            dfcomp_visit(g,comp,p->dest,c);
        }
    } 
}



void componentes(GrafoL g, int n, int comp[]) {
    int i,c=1;

    for (i=0;i<n;i++) comp[i]=0;
     
    for (i=0;i<n;i++) {
        if (comp[i]==0) {
            dfcomp_visit(g,comp,i,c++);
        }
    }


}

int t;

int DFS (GrafoL g,int i, int tsort[], int visit[]) {
    int u,c=0;
    struct aresta *p; 

    visit[i]=GRAY;

    for (p=g[i];p && !c;p=p->prox) {
        if (visit[p->dest]==WHITE) {
            c = DFS(g,i,tsort,visit);
        }
        else if (visit[p->dest]==GRAY) c=1;
    }

    visit[i]=BLACK;
    
    tsort[t--] = i;
    return c;
}


int topSort_Tarjan (GrafoL g, int n, int tsort[]) {
    int i,c=0;
    int visit[n];
    t=n-1;

    for (i=0;i<n;i++) {
        visit[i]=WHITE;
    }
    
    for (i=0;i<n && !c;i++) {
        if (visit[i]==WHITE) {
            c = DFS(g,i,tsort,visit);
        }
    }
    return c;
}

void inDegrees (GrafoL g, int n, int inD[]) {
    struct aresta *p;
    int i;

    for (i=0;i<n;i++) {
        inD[i]=0;
    }
    
    for (i=0;i<n;i++){
        for (p=g[i];p;p=p->prox) {
            inD[p->dest]++;
        } 
    }
}


int topSort_Kahn (GrafoL g, int n, int tsort[]) {
    int inD[n],f=0,l=0,i,u;
    struct aresta *p;

    inDegrees(g,n,inD);


    for (i=0;i<n;i++) {
       if (inD[i]==0) tsort[l++]=i;

    }

    while (f!=l) {
        u = tsort[f++];
        for (p=g[u];p;p=p->prox) {
            inD[p->dest]--;
            if (inD[p->dest]==0) tsort[l++]=p->dest;
            }
    } 

    return (f!=n);
}







int main () {
    
    GrafoM gm4 = {
    {0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 1, 0, 1, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 1},
    {0, 0, 0, 0, 1, 0, 1, 0, 0},
    }; 
      
    GrafoM gm3 = {
    {0, 0, 1, 0, 0, 0, 1, 0, 0},    
    {1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 1, 0, 1, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 1, 0, 0, 0, 1},
    {0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 1},
    {0, 0, 0, 0, 1, 0, 1, 0, 0},
    };

    int cor[9] = {0,0,0,0,1,0,0,0,0}; 

    printf("\n");
    printGraphM(gm3,9);
    printf("\n");


    GrafoL gl3;
    
    graphMtoL(gl3, gm3, 9);
    printGraphL(gl3,9);   
    printf("\n");
    
    int path[NV];
    int tam = shortestPath(gl3,0,7,path);

    int i;
    for (i=0;i<tam;i++) {
        printf("%d ",path[i]);
    }
    printf("\n\n");
    
    dists(gl3,0,path);
    for (i=0;i<NV;i++) {
        printf("%d ",path[i]);
    }
    printf("\n\n");
    

}