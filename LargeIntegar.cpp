#include <algorithm>
#include <iostream>
#include <string.h>
#include <sstream>
#include <istream>
#include <stdio.h>
#include <stack>
using namespace std;
/* ToDoList
[x]1. Input
[x]2. Output
[x]3. Manage the memory
[x]4. Integar Addition
[x]5. Compare size
[ ]5. Integar substruction
[ ]6. Multiplicaion
[ ]7. division
*/

#define InitSize 10
#define MaxOfFBit 9999
class LargeIntegar {
	int *Nums, *BegPos, *EndPos, size; bool Sign;
public:
	LargeIntegar(int sz = InitSize) {
		Sign = false; BegPos = NULL;
		Nums = new int[sz]; size = sz; EndPos = Nums + sz;
		std:: fill(Nums, Nums + sz, false);	
	}
	void Redistribute_Memory(int sz) {
		int* NewInt = new int[sz]; EndPos = NewInt + sz;
		std:: fill(NewInt, NewInt + sz, false);
		memcpy(Nums, NewInt + sz - size, size * sizeof(int));
		delete Nums; Nums = NewInt; size = sz; 
	}
	friend istream& operator >> (istream &in, LargeIntegar &lInt) {
		char ch; in >> ch;
		while (not isdigit(ch)) {
			if (ch == '-') lInt.Sign = true;
			in >> ch;
		}
		stack<char> Q;
		while (isdigit(ch)) {
			Q.push(ch - '0'); ch = getchar();
		}
		if (Q.size() / 4 + 1 > lInt.size) 
			lInt.Redistribute_Memory(max(int(Q.size() / 4 + 1), InitSize));
		int* now = lInt.EndPos - 1;
		while (not Q.empty()) {
			int tmp = 0;
			if (not Q.empty()) { tmp += Q.top(); Q.pop(); }
			if (not Q.empty()) { tmp += 10 * Q.top(); Q.pop(); }
			if (not Q.empty()) { tmp += 100 * Q.top(); Q.pop(); }
			if (not Q.empty()) { tmp += 1000 * Q.top(); Q.pop(); }
			*now = tmp, lInt.BegPos = now;
			now = now - 1;
		}
		return in;
	}
	friend ostream& operator << (ostream &out, LargeIntegar &lInt) {
		if (not lInt.BegPos) {
			out << 0; return out;
		}
		int* num = lInt.BegPos;
		if (lInt.Sign) cout << '-';
		while (num != lInt.EndPos) 
			out << *num << ' ', num += 1;
	}
	bool operator < (const LargeIntegar& o) const {
		if (not Sign and o.Sign) return false;
		if (Sign and not o.Sign) return true;
		if (Sign and o.Sign) return not((-(*this)) < (-o));
		if ()
	}
	LargeIntegar operator + (const LargeIntegar& o) const {
		bool flag = false;
		if (o.Sign and Sign) flag = true;
		else if (o.Sign) return *this - o;
		else if (Sign) return o - *this;
		LargeIntegar Res = LargeIntegar(max(size, o.size));
		int *p = EndPos - 1, *q = o.EndPos - 1, *r = Res.EndPos - 1;
		while (p != BegPos - 1 and q != o.BegPos - 1) {
			if (r == Res.BegPos) Res.Redistribute_Memory(Res.size + 10);
			*r += *p + *q;
			if (*r > MaxOfFBit) *(r - 1) += 1, *r %= MaxOfFBit + 1, Res.BegPos = r - 1;
			else Res.BegPos = r;
			p -= 1, q -= 1, r -= 1;
		}
		if (flag) Res.Sign = true;
		return Res;
	}
	LargeIntegar operator - () const {
		LargeIntegar Res = *this;
		Res.Sign ^= 1;
		return Res;
	}
	LargeIntegar operator - (const LargeIntegar& o) const {
		if (not Sign and o.Sign) return *this + (-o);
		if (Sign and not o.Sign) return -((-(*this) + o));
		if (Sign and Sign) return ((-o) - (-(*this)));
		LargeIntegar Res = LargeIntegar(max(size, o.size));
		int *p = EndPos - 1, *q = o.EndPos - 1, *r = Res.EndPos - 1;
		while (p != BegPos - 1 and q != o.BegPos - 1) {
			if (r == Res.BegPos) Res.Redistribute_Memory(Res.size + 10);
			*r += *p - *q;
		}
	}
};

int Test() {
	LargeIntegar o, b;
	cin >> o >> b;
	cout << o << endl;
	cout << b << endl;
	o = o - b;
	cout << o << endl;
	return 0;
}

int main () {
	return Test();
}