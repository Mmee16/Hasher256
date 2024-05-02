#include "hasher.cpp"
using namespace std;
int main() {
	hasher hash;
	char s[65]={1};
	char p[64];
	hash.process("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno",64,s);
	for(int i=0;i<64;i++) {
		cout<<s[i];
	}
	return 0;
}
