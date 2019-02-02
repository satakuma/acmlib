#include<bits/stdc++.h>
using namespace std;

template<typename T>
struct Splay {
	T key;
	Splay *l, *r, *p;
	int roz;
	// user stuff
	bool flip;
	int val;
	T mn;
	Splay(T _key, int _val, Splay *_l=nullptr, Splay *_r=nullptr, Splay *_p=nullptr): 
	key(_key), l(_l), r(_r), p(_p), roz(1), flip(false), val(_val), mn(_key) {}
	
	void touch() {
		if(flip) {
			swap(l, r);
			if(l) l->flip ^= 1;
			if(r) r->flip ^= 1;
			flip = false;
		}
	}
	
	void update() {
		roz = 1;
		//mn = key;
		if(l) roz += l->roz /*, mn = min(mn, l->mn)*/;
		if(r) roz += r->roz /*, mn = min(mn, r->mn)*/;
	}
	
	bool isRoot() {
		return !p /*or (p->l != this and p->r != this)*/;
	}
	
	static void connect(Splay *p, Splay *c, bool left) {
		(left ? p->l : p->r) = c;
		if(c) c->p = p;
	}
	
	void rotate() {
		auto f = p;
		auto g = f->p;
		f->touch();
		touch();
		bool isr = f->isRoot();
		//isr == f->p == g w splayu
		bool left = (f->l == this);
		connect(f, left ? r : l, left);
		connect(this, f, !left);
		if(isr) 
			p = g;
		else
			connect(g, this, f == g->l);
		//if(f->l) f->l->touch();
		//if(f->r) f->r->touch(); // jeśli nasze spychanie jest jakies dziwne to odkomentowac
		f->update();
	}
	
	void splay() {
		touch();
		while(not isRoot()) {
			auto g = p->p;
			if(not p->isRoot())
				(((p->l == this) == (g->l == p)) ? p : this)->rotate();
			rotate();
		}
		touch();
		//if(l) l->touch();
		//if(r) r->touch();
		update();
	}
	
	// raczej sprawdzic czy na pewno jest tyle elementow w spleju
	Splay* find_by_order(int k) { // 0-indexed
		if(roz < k) return nullptr;
		touch();
		if(l) {
			if(l->roz == k) {
				splay();
				return this;
			}
			return (l->roz < k ? r->find_by_order(k - l->roz - 1) : l->find_by_order(k));
		} else if(k == 0) {
			splay();
			return this;
		} else
			return r->find_by_order(k-1);
	}
	Splay* first() {
		return find_by_order(0);
	}
	Splay* last() {
		return find_by_order(roz-1);
	}
	
	Splay* find_by_key(T x) {
		touch();
		auto ptr = (x < key ? l : r);
		if(key == x or not ptr) {
			splay();
			return this;
		}
		return ptr->find_by_key(x);
	}
	
	Splay* get_min() {
		touch();
		if(mn == key) {
			splay();
			return this;
		}
		assert(l or r);
		return (l and l->mn == mn ? l : r)->get_min();
	}	
};

template<typename T>
pair<Splay<T>*, Splay<T>*> split(Splay<T> *s) {
	s->splay(); s->touch();
	auto l = s->l;
	s->l = nullptr;
	s->update();
	if(l) {
		l->p = nullptr;
		//l->touch();
	}
	//if(s->r) s->r->touch();
	return {l, s};
}

template<typename T>
pair<Splay<T>*, Splay<T>*> split_by_key(Splay<T> *s, T key) {
	s = s->find_by_key(key);
	return split(s);
}
template<typename T>
pair<Splay<T>*, Splay<T>*> split_by_order(Splay<T> *s, int k) {
	s = s->find_by_order(k);
	return split(s);
}

template<typename T>
Splay<T>* merge(Splay<T> *a, Splay<T> *b) {
	if(not a and not b) return nullptr;
	if(not a) {
		b->splay();
		return b;
	}
	if(not b) {
		a->splay();
		return a;
	}
	a = a->last();
	b->splay();
	assert(not a->r);
	Splay<T>::connect(a, b, false);
	a->update();
	return a;
}

template<typename T>
void deleteTree(Splay<T> *t) {
	if(t->l)
		deleteTree(t->l);
	if(t->r)
		deleteTree(t->r);
	delete t;
}

using splay = Splay<int>;

int n;
splay *S = nullptr;

void reverse_seg(int l, int r) {
	if(r <= l) return;
	splay *a, *b, *c=nullptr;
	tie(a, b) = split_by_order(S, l);
	if(r < n) // !!!
		tie(b, c) = split_by_order(b, r-l);
	b->flip ^= 1;
	S = merge(a, b);
	S = merge(S, c);
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	
	cin >> n;
	for(int i=0; i<n; i++) {
		int x; cin >> x;
		splay *v = new splay(i, x);
		S = merge(S, v);
	}
	
	int q; cin >> q;
	while(q--) {
		int a, b;
		cin >> a >> b;
		reverse_seg(a, b);
	}
	cout << "\n";
}
