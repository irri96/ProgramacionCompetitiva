#include <bits/stdc++.h>
using namespace std;
 
typedef long long intt;
typedef pair<int, int> par;
typedef vector<vector<int> > graph;
typedef vector<vector<par> > wgraph;
#define pb push_back
#define ppb pop_back
vector <int> origen,destino,capacidad,costos,dia,orden,inicial;
class Dinic {
	struct edge {
		int to, rev;
		intt f, cap;
	};
	vector<vector<edge>> g;
	vector<intt> dist;
	vector<int> q, work;
	int n, sink;
	bool bfs(int start, int finish) {
		dist.assign(n, -1);
		dist[start] = 0;
		int head = 0, tail = 0;
		q[tail++] = start;
		while (head < tail) {
			int u = q[head++];
			for (const edge &e : g[u]) {
				int v = e.to;
				if (dist[v] == -1 and e.f < e.cap) {
					dist[v] = dist[u] + 1;
					q[tail++] = v;
				}
			}
		}
		return dist[finish] != -1;
	}
	intt dfs(int u, intt f) {
		if (u == sink) return f;
		for (int &i = work[u]; i < (int)g[u].size(); ++i) {
			edge &e = g[u][i];
			int v = e.to;
			if (e.cap <= e.f or dist[v] != dist[u] + 1)
				continue;
			intt df = dfs(v, min(f, e.cap - e.f));
			if (df > 0) {
				e.f += df;
				g[v][e.rev].f -= df;
				return df;
			}
		}
		return 0;
	}
public:
	Dinic(int n) {
		this->n = n;
		g.resize(n);
		dist.resize(n);
		q.resize(n);
	}
	Dinic(){
 
	}
	// aristas bidireccionales si cap de edge b = cap, si es 0 no son bidireccionales!!
	void add_edge(int u, int v, intt cap) {
    edge a = {v, (int)g[v].size(), 0, cap};
    edge b = {u, (int)g[u].size(), 0, 0};
    g[u].pb(a);
    g[v].pb(b);
	}
	intt max_flow(int source, int dest) {
		sink = dest;
		intt ans = 0;
		while (bfs(source, dest)) {
			work.assign(n, 0);
			while (intt delta = dfs(source, 1000)) ans += delta;
		}
		return ans;
	}
};
 
//contruyo el dinic

Dinic construir(int maximo,int tam,int g,int d){
	Dinic D(tam+1);	
	for(int i = 0;i < inicial.size();i++){
		D.add_edge(0,(i)*(d+1) +1,inicial[i]);
	}
	for(int i = 0; i < inicial.size();i++){
		for(int j = 0; j < d; j++){
			D.add_edge(i*(d+1) + 1 + j,i*(d+1)+2+j,g);
		}
	}
	for(int i = 0;i <origen.size();i++)if(costos[i] <= maximo){
	 	D.add_edge(origen[i],destino[i],capacidad[i]);
	 }
	return D;
}

// buscqueda binaria cuando quiero minimizar un valor X asociado al flujo 

int BS(int lo,int hi, int tam,int tope,int d){
	int mi;
	while(lo<hi){
		mi = (lo+hi)/2;
	Dinic D = construir(orden[mi],tam,tope,d);
		int flujo = D.max_flow(0,tam);
		if(flujo==tope) hi = mi;
		else lo = mi+1;
		}
	return lo;
}

void conectar(pair<int,int> &a,pair<int,int> &b,Dinic &d){
	d.add_edge(a.first,b.first,1);
	if(a.second!=-1){
		d.add_edge(a.second,b.first,1);
	}
	if(b.second!=-1){
		d.add_edge(a.first,b.second,1);
	}
	if(a.second!=-1 && b.second !=-1){
		d.add_edge(a.second,b.second,1);
	}
}

int main(){
	int r,c;
	while(scanf("%d %d",&r,&c)!=EOF){
		vector<string> grid(r,string());
		pair<int,int> numbers[r][c];
		for(int i=0;i<r;i++){
			cin>>grid[i];
		}

		int idx = 1;
		for(int i=0;i<r;i++){
			for(int j=0;j<c;j++){
				numbers[i][j].first=idx++;
				numbers[i][j].second=(grid[i][j]=='-')?idx++:-1;
			}
		}
		//puts("asd");
		//cout<<idx<<endl;
		Dinic d(idx+100);
		int wea1=0,wea2=0;
		for(int i=0;i<r;i++){
			for(int j=0;j<c;j++){
				if((i+j)%2==0){		//casillas negras
					//conecto al origen
					wea2++;
					d.add_edge(0,numbers[i][j].first,1);
					//conecto a las vecinas
					if(numbers[i][j].second!=-1){
						wea2++;
						d.add_edge(0,numbers[i][j].second,1);
					}
					if(i-1>=0){
						conectar(numbers[i][j],numbers[i-1][j],d);
					}
					if(i+1<r){
						conectar(numbers[i][j],numbers[i+1][j],d);
					}
					if(j-1>=0){
						conectar(numbers[i][j],numbers[i][j-1],d);
					}
					if(j+1<c){
						conectar(numbers[i][j],numbers[i][j+1],d);
					}
				}else{				//casillas blancas
					//conecto al destino
					d.add_edge(numbers[i][j].first,idx,1);
					wea1++;
					if(numbers[i][j].second!=-1){
						wea1++;
						d.add_edge(numbers[i][j].second,idx,1);
					}
				}
				
			}
		}
		intt valor = d.max_flow(0,idx);
		if(wea1==wea2 && wea2==valor){
			puts("Y");
		}else{
			puts("N");
		}
	}
	return 0;
}
/*
int main(){
	int t,aux;
	scanf("%d",&t);
	int caso = 1;
	while(t--){
		int n,d,m,u,v,c,p,e;
		scanf("%d %d %d",&n,&d,&m);
		origen.clear();destino.clear();capacidad.clear();
		costos.clear(),orden.clear(),inicial.clear();
		set <int> ordenNR;
		for(int i = 0; i < m; i++){
			scanf("%d %d %d %d %d",&u,&v,&c,&p,&e);
			origen.push_back( (u-1)*(d+1) + e +1);
			destino.push_back( (v-1)*(d+1) + e+2);
			capacidad.push_back(c);
			costos.push_back(p);
			ordenNR.insert(p);
		}
		int total = 0;
		for(int i = 0; i < n; i++){
			scanf("%d",&aux);
			inicial.pb(aux);
			total += aux;
		}
		for(auto it = ordenNR.begin();it != ordenNR.end();it++){
			orden.push_back(*it);
		}
		int tama = n*(d+1);
		sort(orden.begin(),orden.end());
		int sol = BS(0,orden.size()-1,tama,total,d);
		printf("Case #%d:",caso);
		Dinic D = construir(orden[sol],tama,total,d);
		int flujo = D.max_flow(0,tama);
		if(flujo>=total)printf(" %d\n",orden[sol]);
		else puts(" Impossible");
		caso++;
	}
	return 0;
}
*/