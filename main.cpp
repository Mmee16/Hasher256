#include "hasher.cpp"
#include <cstdint>
using namespace std;
int main() {
	hasher hash;
	char s[64];
	char p[64];
	for(int i=0;i<64;i++) {
		p[i]='a';
		s[i]='1';
	}
	hash.process(p,64,s);
	
	return 0;
}
