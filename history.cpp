// 

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