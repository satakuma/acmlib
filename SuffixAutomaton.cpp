#define st first
#define nd second
#define siz(c) (int)(c).size()

struct SuffixAutomaton {
	struct Node
	{
		int len,link,cnt=1;
		map<char,int>nxt;
		Node(int len=0,int link=0,int cnt=1): 
		len(len),link(link),cnt(cnt){}
		int& operator[](char c){
			return nxt[c];
		}
	};
	vector<Node>s;
	int lst;
	void addChar(const char c){
		s.emplace_back(s[lst].len+1);
		int cur=siz(s)-1,p=lst;
		while(p>-1 and !s[p][c]){
			s[p][c]=cur;
			p=s[p].link;
		}
		if(p>-1){
			int q=s[p][c];
			if(s[q].len==s[p].len+1)
				s[cur].link=q;
			else {
				int cl=siz(s);
				s.emplace_back(s[q]);
				s[cl].len=s[p].len+1;
				s[q].link=s[cur].link=cl;
				while(p>-1 and s[p][c]==q) {
					s[p][c]=cl;
					p=s[p].link;
				}
			}
		}
		lst=cur;
	}
	void init(){
		s.emplace_back(0,-1,0);
		lst=0;
	}
	SuffixAutomaton(){
		init();
	}
	SuffixAutomaton(const string& S){
		init();
		for(auto& c:S)
			addChar(c);
	}
	void dfs(int a,vector<bool>& vis){
		vis[a]=true;
		for(auto& it:s[a].nxt){
			if(!vis[it.nd]) dfs(it.nd,vis);
			s[a].cnt+=s[it.nd].cnt;
		}
	}
	void calcDP(){
		vector<bool>vis(siz(s));
		dfs(0,vis);
	}
	string getKth(long long k){
		int cur=0;
		string ans="";
		while(k){
			for(auto& it:s[cur].nxt){
				if(s[it.nd].cnt<k)
					k-=s[it.nd].cnt;
				else {
					ans.push_back(it.st);
					cur=it.nd;
					k--;
					break;
				}
			}
		}
		return ans;
	}
};

int main()
{
	string S; cin>>S;
	SuffixAutomaton sa(S);
	sa.calcDP();
	int q; cin>>q;
	while(q--){
		long long k; cin>>k;
		cout<<sa.getKth(k)<<"\n";
	}
}
