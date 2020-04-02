#include "threadpool.h"
// #include <iostream>
#include <cstring>
// #include <cstdio>
// #include <ctime>
#include <sys/time.h>
// #include <stdlib.h>
using namespace std;
// int mp[10][10];
#define N 3800
#define M 3800
#define ll long long
//inline int read(){				//读入挂 
//	char ch = getchar();
//	while(!isdigit(ch)){ch = getchar();}
//	return (int)(ch-'0');
//}
int64_t now(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}
struct DancingLink {
	int n,s,ansd;//列数 节点总数
	int S[M],A[M],H[M];//S[]该列节点总数  A[]答案  H[]行首指针
	int L[N],R[N],U[N],D[N]; //L[],R[],U[],D[] 上下左右
	int X[N],C[N];//X[] C[] 行列编号
	void init(int n) { //初始化
		this->n=n;
		for(int i=0; i<=n; i++)
			U[i]=i,D[i]=i,L[i]=i-1,R[i]=i+1;
		R[n]=0,L[0]=n;s=n+1;
		memset(S,0,sizeof(S));
		memset(H,-1,sizeof(H));
	}
	void DelCol(int c) { //删除列
		L[R[c]]=L[c];R[L[c]]=R[c];
		for(int i=D[c]; i!=c; i=D[i])
			for(int j=R[i]; j!=i; j=R[j])
				U[D[j]]=U[j],D[U[j]]=D[j],--S[C[j]];
	}
	void ResCol(int c) { //恢复列
		for(int i=U[c]; i!=c; i=U[i])
			for(int j=L[i]; j!=i; j=L[j])
				++S[C[j]],U[D[j]]=j,D[U[j]]=j;
		L[R[c]]=c,R[L[c]]=c;
	}
	void AddNode(int r,int c) { //添加节点
		++S[c],C[++s]=c,X[s]=r;
		D[s]=D[c],U[D[c]]=s,U[s]=c,D[c]=s;
		if(H[r]<0) H[r]=L[s]=R[s]=s;//行首节点
		else  R[s]=R[H[r]],L[R[H[r]]]=s,L[s]=H[r],R[H[r]]=s;
	}
	bool dfs(int d) { //深度，深搜遍历
		if(!R[0]) {
			ansd=d;return true;
		}
		int c=R[0];
		for(int i=R[0]; i; i=R[i]) if(S[i]<S[c]) c=i;
		DelCol(c);
		for(int i=D[c]; i!=c; i=D[i]) {
			A[d]=X[i];
			for(int j=R[i]; j!=i; j=R[j]) DelCol(C[j]);
			if(dfs(d+1)) return true;
			for(int j=L[i]; j!=i; j=L[j]) ResCol(C[j]);
		}
		ResCol(c);return false;
	}

} dlx;

int ans[10][10];
inline int encode(int a, int b, int c) {
	return a*81+(b-1)*9+c;
}
int decode() {
	int x,y,k;
	for(int i=0; i<dlx.ansd; i++) {
		int r = dlx.A[i];
		k = r%9;
		if(k==0) k = 9;
		int num = (r - k)/9 + 1;
		y = num%9;
		if(y == 0) y = 9;
		x = (num-y)/9 + 1;
		ans[x][y] = k;
	}
}

struct node{
    DancingLink DLK;
    int mp [10][10];
};

void* solve(void* arg){
    node* nd = (node*)arg;
	// dlx.init(9*9*4);
    nd->DLK.init(9*9*4);
		for(int i=1; i<=9; i++)
			for(int j=1; j<=9; j++) 
				for(int k=1; k<=9; k++)
					// if(!mp[i][j] || mp[i][j]==k) {
                    if(!(nd->mp[i][j])||(nd->mp[i][j]==k)){
					int t = encode(0,i,j);
					int r = encode(0,t,k);
					// dlx.AddNode(r,t);
                    nd->DLK.AddNode(r,t);
					// dlx.AddNode(r,encode(1,i,k));
                    nd->DLK.AddNode(r,encode(1,i,k));
					// dlx.AddNode(r,encode(2,j,k));
                    nd->DLK.AddNode(r,encode(2,j,k));
					// dlx.AddNode(r,encode(3,(i-1)/3*3+(j+2)/3,k));
                    nd->DLK.AddNode(r,encode(3,(i-1)/3*3+(j+2)/3,k));
				}

		// dlx.dfs(0);
        nd->DLK.dfs(0);
//		decode();
//		if(dlx.ansd == 0)
//			printf("impossible\n");
//		else {
//			for(int i=1; i<=9; i++){
//				for(int j=1; j<=9; j++)
//					printf("%d",ans[i][j]);
//			}
//			printf("\n");
//		}
}

int main(int argc,char* args[]) {
//	freopen("test1000.txt","r",stdin);
	// freopen("output1.txt","w",stdout);
	FILE* f= fopen(args[1],"r");
	int tot = 0 ;
	char buf[88];
	ll start = now();
    //创建线程池
    struct threadpool *pool = threadpool_init(5, 10);
	while(fgets(buf,sizeof(buf),f)!=NULL){
		++tot;
		int mid=0;
        node* nd = (node*)malloc(sizeof(node));
		for(int i=1;i<=9;i++)
			for(int j=1;j<=9;j++){
				// mp[i][j] = (int)(buf[mid] - '0');
                nd->mp[i][j] = (int)(buf[mid] - '0');
				++mid;
			}
		// init_cache();
		// solve((void*)nd);
        threadpool_add_job(pool, solve, (void*)nd);
	}
	
    threadpool_destroy(pool);
	ll end = now();
    double total = end - start;
	// fclose(stdout);
	// freopen("CON", "w", stdout);
	printf("total time is: %lf ms, each cost %lf us\n", total*1.0/1000,total*1.0/tot);
}

