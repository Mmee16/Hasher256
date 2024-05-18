#include "hasher.cpp"
using namespace std;
int main() {
	hasher hash;
	char s[65]={1};
	char p[64];
	cout<<"Main";
	hash.process("",0,s);
	for(int i=0;i<64;i++) {
		cout<<s[i];
	}
	return 0;
}
