#define pb push_back
#define all(c) (c).begin(), (c).end()
typedef long long LL;
vector<LL>witness = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};

LL mul(uint64_t a, uint64_t b, uint64_t m) {
	if(m <= a) a %= m;
	if(m <= b) b %= m;
	long double x = a;
	uint64_t c = x * b / m;
	int64_t r = (int64_t)(a*b - c*m) % (int64_t)m;
	return r < 0 ? r + m : r;
}

LL fpow(LL a, LL b, LL mod) {
	LL res = 1;
	while(b) {
		if(b&1) 
			res = mul(res, a, mod);
		a = mul(a, a, mod);
		b >>= 1;
	}
	return res;
}

LL f(LL x, LL mod, LL c) {
	LL y = mul(x, x, mod) + c;
	if(y >= mod) y -= mod;
	return y;
}

bool test(LL n) {
	if(n == 2) return true;
	if(n < 2 or n % 2 == 0) return false;
	LL d = n - 1, s = 0;
	while(d % 2 == 0) {
		d >>= 1;
		++s;
	}
	for(auto i:witness) {
		if(i % n) {
			LL x = fpow(i, d, n);
			if(x == 1) continue;
			bool composite = true;
			for(int j = 0; j < s; ++j) {
				if(x == n - 1) {
					composite = false;
					break;
				}
				x = mul(x, x, n);
			}
			if(composite) return false;
		}
	}
	return true;
}		

void rho(LL n, vector<LL>& res) {
	if(n<=1) return;
	if(test(n)) {
		res.pb(n);
		return;
	}
	for(LL c = 1; true; ++c) {
		LL x = 2, y = 2, d = 1;
		while(d == 1) {
			x = f(x, n, c);
			y = f(f(y, n, c), n, c);
			d = __gcd(abs(x - y), n);
		}
		if(d < n) {
			rho(d, res);
			rho(n / d, res);
			return;
		}
	}
}

vector<LL> factor(LL n) {
	vector<LL> res;
	for(int i = 2; i <= 100; i++ ) {
		while(n%i == 0) {
			n /= i;
			res.pb(i);
		}
	}
	rho(n, res);
	//sort(all(res));
	return res;
}

int main()
{
	LL x; cin>>x;
	auto c = factor(x);
	for(auto r:c)
		cout<<r<<" ";
	cout<<endl;	
}
