#include <algorithm>
#include <iostream>
#include <string.h>
#include <sstream>
#include <istream>
#include <stdio.h>
#include <string>
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
    template <typename Int> 
    LargeIntegar Convert_Int_to_LargeIntegar(Int integar) {
        string Str = to_string(integar);
        return Convert_String_to_LargeIntegar(Str);
    }
    void Redistribute_Memory(int sz) {
        int* NewInt = new int[sz]; EndPos = NewInt + sz;
        std:: fill(NewInt, NewInt + sz, false);
        memcpy(Nums, NewInt + sz - size, size * sizeof(int));
        delete Nums; Nums = NewInt, size = sz; 
    }
    static LargeIntegar Convert_String_to_LargeIntegar(string& Str) {
        int sz = max(int(Str.size() + 5), InitSize);
        LargeIntegar Res = LargeIntegar(sz);
        int *p = Res.EndPos - 1;
        while (Str[0] == '0' and Str.size() > 1) Str.erase(0, 1);
        for (int i = Str.size() - 1; i >= 0; i -= 4) {
            Res.BegPos = p;
            if (i < 3) {
                *p = Str[i] - '0', Res.bit += 1;
                if (i > 0) *p += 10 * (Str[i - 1] - '0'), Res.bit += 1;
                if (i > 1) *p += 100 * (Str[i - 2] - '0'), Res.bit += 1;
                break;
            }
            *p = Str[i] - '0' + 10 * (Str[i - 1] - '0') + 
             	(Str[i - 2] - '0') * 100 + 1000 * (Str[i - 3] - '0');
            p -= 1, Res.bit += 4;
        }
        return Res;
    }
    friend istream& operator >> (istream &in, LargeIntegar &Int) {
        string str; in >> str;
        Int = Convert_String_to_LargeIntegar(str);
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
        LargeIntegar Res = LargeIntegar(max(max(bit, o.bit) + 5, InitSize));
        int *p = EndPos - 1, *q = o.EndPos - 1, *r = Res.EndPos - 1;
        while (p != BegPos - 1) {
            *r += *p + (q == o.BegPos - 1 ? 0 : *q);
            if (*r > MaxOfFBit) {
                *(r - 1) += 1, *r %= MaxOfFBit + 1, 
                Res.BegPos = r - 1, Res.bit += 4;
            }
            else Res.BegPos = r, Res.bit += (p == BegPos ? CalcBit(*r) : 4);
            p -= 1, q -= (q == o.BegPos - 1 ? 0 : 1), r -= 1;
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
        LargeIntegar Res = LargeIntegar(max(max(bit, o.bit) + 5, InitSize));
        int *p = EndPos - 1, *q = o.EndPos - 1, *r = Res.EndPos - 1;
        while (p != BegPos - 1) {
            *r += *p - (q == o.BegPos - 1 ? 0 : *q);
            if (*r < 0) *(r - 1) -= 1, *r += MaxOfFBit + 1, Res.BegPos = r - 1;
            else Res.BegPos = r;
            Res.bit += (p == BegPos ? CalcBit(*r) : 4);
            p -= 1, q -= (q == o.BegPos - 1 ? 0 : 1), r -= 1;
        }
        while (*Res.BegPos == 0 and Res.BegPos < Res.EndPos - 1) 
            Res.BegPos += 1;
        return Res;
    }
};

int Test() {
    LargeIntegar o, b;
    cin >> o >> b;
	cout << o << endl;
	cout << b << endl;
// 	printf("If A < B ? %d\n", o < b);
    o = o + b;
    cout << o << endl;
    return 0;
}

int main () {
    return Test();
}
