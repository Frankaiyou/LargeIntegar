#include <algorithm>
#include <iostream>
#include <string.h>
#include <sstream>
#include <istream>
#include <stdio.h>
#include <stack>
#include <deque>
using namespace std;

#define InitSize 30
#define MaxOfFBit 9999
#define CalcBit(X) (X > 999 ? 4 : (X > 99 ? 3 : (X > 9 ? 2 : (X > 0 ? 1 : 0))))

class LargeIntegar {
	int *Nums, *BegPos, *EndPos, size, bit; bool Sign;
public:
	LargeIntegar(int sz = InitSize) {
		Sign = false, BegPos = NULL, bit = 0;
		Nums = new int[sz], size = sz, EndPos = Nums + sz;
		std:: fill(Nums, Nums + sz, false);	
	}
	void Redistribute_Memory(int sz) {
		int* NewInt = new int[sz]; EndPos = NewInt + sz;
		std:: fill(NewInt, NewInt + sz, false);
		memcpy(Nums, NewInt + sz - size, size * sizeof(int));
		delete Nums; Nums = NewInt, size = sz; 
	}
	friend istream& operator >> (istream &in, LargeIntegar &lInt) {
		char ch; in >> ch;
		while (not isdigit(ch)) {
			if (ch == '-') lInt.Sign = true;
			ch = getchar();
		}
		deque<char> Q;
		while (isdigit(ch)) {
			Q.push_back(ch - '0'); 
			ch = getchar();
		}
		while (Q.size() > 2 and Q.front() == 0) Q.pop_front();
		if (Q.size() / 4 + 1 > lInt.size) 
			lInt.Redistribute_Memory(max(int(Q.size() / 4 + 1), InitSize));
		int* now = lInt.EndPos - 1;
		while (not Q.empty()) {
			int tmp = 0;
			if (not Q.empty()) { tmp += Q.back(); Q.pop_back(); lInt.bit += 1; }
			if (not Q.empty()) { tmp += 10 * Q.back(); Q.pop_back(); lInt.bit += 1; }
			if (not Q.empty()) { tmp += 100 * Q.back(); Q.pop_back(); lInt.bit += 1; }
			if (not Q.empty()) { tmp += 1000 * Q.back(); Q.pop_back(); lInt.bit += 1; }
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
		out << *num; num += 1;
		int tmp;
		while (num != lInt.EndPos) {	
			if (*num == 0) out << "000";
			else if ((tmp = CalcBit(*num)) < 4) {
				tmp = 4 - tmp;
				while (tmp --) putchar('0');
			}
			out << *num;
			num += 1;
		}
		return out;
	}
	bool operator < (const LargeIntegar& o) const {
		if (not Sign and o.Sign) return false;
		if (Sign and not o.Sign) return true;
		if (Sign and o.Sign) return not((-(*this)) < (-o));
		if (bit != o.bit) return bit < o.bit;
		int *p = this->BegPos, *q = o.BegPos;
		while (p != this->EndPos and q != o.EndPos) {
			if (*p != *q) return *p < *q;
			p = p + 1, q = q + 1;
		}
		return false;
	}
	LargeIntegar operator + (const LargeIntegar& o) const {
		bool flag = false;
		if (o.Sign and Sign) flag = true;
		else if (o.Sign) return *this - (-o);
		else if (Sign) return o - (-(*this));
		if (*this < o) return o + *this;
		LargeIntegar Res = LargeIntegar(max(size, o.size) + 10);
		int *p = EndPos - 1, *q = o.EndPos - 1, *r = Res.EndPos - 1;
		while (p != BegPos - 1 and q != o.BegPos - 1) {
			if (p == Nums) Res.Redistribute_Memory(Res.size + 10);
			*r += *p + *q;
			if (*r > MaxOfFBit) {
				*(r - 1) += 1, *r %= MaxOfFBit + 1, 
				Res.BegPos = r - 1, Res.bit += 4;
			}
			else Res.BegPos = r, Res.bit += (p == BegPos ? CalcBit(*r) : 4);
			p -= 1, q -= 1, r -= 1;
		}
		while (p != BegPos - 1) {
			if (p == Nums) Res.Redistribute_Memory(Res.size + 10);
			*r += *p;
			if (*r > MaxOfFBit) {
				*(r - 1) += 1, *r %= MaxOfFBit + 1;
				Res.BegPos = r - 1, Res.bit += 4;
			}
			else Res.BegPos = r, Res.bit += (p == BegPos ? CalcBit(*r) : 4);
			p -= 1, r -= 1;
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
		if (*this < o) return -(o - *this);
		LargeIntegar Res = LargeIntegar(max(size, o.size));
		int *p = EndPos - 1, *q = o.EndPos - 1, *r = Res.EndPos - 1;
		while (p != BegPos - 1 and q != o.BegPos - 1) {
			if (p == Nums) Res.Redistribute_Memory(Res.size + 10);
			*r += *p - *q;
			if (*r < 0) *(r - 1) -= 1, *r += MaxOfFBit + 1, Res.BegPos = r - 1;
			else Res.BegPos = r;
			Res.bit += (p == BegPos ? CalcBit(*r) : 4);
			p -= 1, q -= 1, r -= 1;
		}
		while (p != BegPos - 1) {
			if (p == Nums) Res.Redistribute_Memory(Res.size + 10);
			*r += *p;
			Res.BegPos = r;
			Res.bit += CalcBit(*r);
			p -= 1, r -= 1;
		}
		return Res;
	}
};

int Test() {
	LargeIntegar o, b;
	cin >> o >> b;
	// cout << o << endl;
	// cout << b << endl;
	// printf("If A < B ? %d\n", o < b);
	o = o + b;
	cout << o << endl;
	return 0;
}

int main () {
	return Test();
}