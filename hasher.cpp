#include <cstdint>
#include <iostream> 
#include <sys/types.h>
using namespace std; 
class hasher{
	private:
	void convert8_32(const uint8_t *buff,uint32_t *ptr,int size) {
		for(int i=0;i<size;i++) {
			ptr[i] = (buff[i+0] <<24) | (buff[i+1] <<16) | (buff[i+2] << 8) | (buff[i]);
		}
	}
	uint32_t hash[8] = {
		0x6a09e667,
		0xbb67ae85,
		0x3c6ef372,
		0xa54ff53a,
		0x510e527f,
		0x9b05688c,
		0x1f83d9ab,
		0x5be0cd19
	};
	uint32_t constants[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
	uint32_t right_rotate(uint32_t data,int d) {
		return (data>>d)|(data<<(32-d));
	}
	void copybuff(uint32_t *buff,const uint32_t *w,int start) {
		for(int i=0;i<16;i++) {
			buff[i]=w[start+i];
		}
	}
	void process_buff(uint32_t *buff,int start) {
		for(int i=16;i<64;i++) {
			uint32_t s0 =(right_rotate(buff[start+i-15],7)) ^ (right_rotate(buff[start+i-15], 18)) ^ (buff[start+i-15]>>3);
			uint32_t s1 =(right_rotate(buff[start+i-2],17)) ^ (right_rotate(buff[start+i-2], 19)) ^ (buff[start+i-2]>>10);
			buff[start+i] = buff[start+i-16] + s0 + buff[i-7] + s1;
		}
	}
	void initilize_working_variables(uint32_t *a) {
		for(int i=0;i<8;i++) {
			a[i] = hash[i];
		}
	}
	void compress(uint32_t *buff,uint32_t *a) {
		for(int i=0;i<64;i++) {
			uint32_t s1 = (right_rotate(a[4], 6)) ^ (right_rotate(a[4], 6)) ^ (right_rotate(a[4], 25));
			uint32_t ch = (a[4] & a[5]) ^ ((!a[4]) & a[6]);
			uint32_t temp1 = a[7] + s1 +ch + constants[i] + buff[i];
			uint32_t s0 = right_rotate(a[0], 2) ^ right_rotate(a[0], 13) ^ right_rotate(a[0], 22);
			uint32_t maj = (a[0] & a[1]) ^ (a[0] & a[2]) ^ (a[1] & a[2]);
			uint32_t temp2 = s0 + maj;
			a[7] = a[6];
			a[6] = a[5];
			a[5] = a[4];
			a[4] = a[3] + temp1;
			a[3] = a[2];
			a[2] = a[1];
			a[1] = a[0];
			a[0] = temp1 + temp2;
		}
		for(int i=0;i<8;i++)
			hash[i]+=a[i];
	}
	void finalize_hash(uint8_t *output) {
		for(int i=0;i<8;i++) {
			output[i+0] = (hash[i] >> 24) & 0xFF;
			output[i+1] = (hash[i] >> 16) & 0xFF;
			output[i+2] = (hash[i] >> 8) & 0xFF;
			output[i+3] = (hash[i]) & 0xFF;
		}
	}
	public:
	void process(const void *data,int size,uint8_t *output) {
		const uint8_t *buff = (const uint8_t *)data;
		uint32_t ptr[size];
		convert8_32(buff,ptr,size);
		int hash_size = 0;
		uint32_t w[64];
		while(size>=64) {
			copybuff(w,ptr,hash_size);
			process_buff(w,hash_size);
			uint32_t a[8];
			initilize_working_variables(a);
			compress(w,a);
			size-=64;
			hash_size+=64;
		}
		finalize_hash(output);
	}
};
