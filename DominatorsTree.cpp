#include<bits/stdc++.h>
using namespace std;

// Użycie: auto D = DominatorsTree(liczba_wierzcholkow, source_vertex (domyslnie 1))
// Dodawanie krawedzi u -> v: D.addEdge(u, v)
// D.build() zwraca vector dominatorow V: V[i] to idom wierzcholka i (V[root] = 0, nieosiagalne wierzcholki mają -1)
// Zlozonosc: O((n+m) log n)

struct DominatorsTree {
	int n, root, lp;
	
	vector<int> no, who, idom, sdom, par, label, fu;
	vector<vector<int>> G, GR, buck;
	
	DominatorsTree(int _n, int _root=1):
	n(_n), root(_root), lp(0), 
	no(_n+1), who(_n+1), idom(_n+1), sdom(_n+1), 
	par(_n+1), label(_n+1), fu(_n+1),
	G(_n+1), GR(_n+1), buck(_n+1) {}
	
	void add_edge(int a, int b) {
		G[a].push_back(b);
	}
	
	int find(int a, int dep=0) {
		if(fu[a] == a) return dep ? -1 : a;
		int v = find(fu[a], dep+1);
		if(v == -1) return a;
		if(sdom[label[fu[a]]] < sdom[label[a]])
			label[a] = label[fu[a]];
		fu[a] = v;
		return dep ? v : label[a];
	}
	void link(int from, int to) {
		fu[from] = to;
	}	
	
	void dfs(int a) {
		no[a] = ++lp;
		who[no[a]] = a;
		for(auto v:G[a]) {
			if(no[v] == 0) {
				dfs(v);
				par[no[v]] = no[a];
			}
			GR[no[v]].push_back(no[a]);
		}
	}
	
	// zwraca vector dominatorow (ret[i] = (dom_i albo 0 dla roota albo -1 dla nieosiagalnego))
	vector<int> build() {
		dfs(root);
		iota(label.begin(), label.end(), 0);
		iota(sdom.begin(), sdom.end(), 0);
		iota(fu.begin(), fu.end(), 0);
		for(int i=lp; i>0; i--) {
			for(auto v:GR[i])
				sdom[i] = min(sdom[i], sdom[find(v)]);
			for(auto a:buck[i]) {
				auto v = find(a);
				idom[a] = (sdom[v] == sdom[a] ? sdom[a] : v); // v bedzie aktualizowane do idom[v]
			}
			if(i > 1) {
				buck[sdom[i]].push_back(i);
				link(i, par[i]);
			}
		}
		
		vector<int> ret(n+1, -1);
		for(int i=2; i<=lp; i++) {
			if(idom[i] != sdom[i]) idom[i] = idom[idom[i]];
			ret[who[i]] = who[idom[i]];
		}
		
		ret[root] = 0;
		return ret;
	}
};
