/*
Jeremy Tom
CSC 152, #8986
10/21/2019
Sponge-based cryptographic hash that uses perm152
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

void perm152(unsigned char* in, unsigned char* out);  // each an array of 64 bytes

static void xor (unsigned char* dst, unsigned char* src, int num_bytes) { 
	for (int i = 0; i < num_bytes; i++) {
		dst[i] ^= src[i];
	}
}

void perm152hash(unsigned char* m, int mbytes, unsigned char* res) {
	//Capacity c, rate r, and output length b are all 32 bytes
	//Thus, output 'b' length of 'r' to res would be an output of 64 bytes

	int r = 32;						//Rate
	int c = 32;						//Capacity
	int b = 32;						//Desired output length

	unsigned char block[64];				//Create a 'block' that holds r+c (64 in this case) bytes
	unsigned char m_padded[32];			//uint_8 that takes holds 32 bytes

	//Block = 0x01 || 0x02 || ... || (unsigned char)(r+c)
	for (int i = 0; i < (r + c); i++) {
		block[i] = (unsigned char)i + (unsigned char)1;
//		printf("%02x\n", block[i]);
	}


	//Process 32 bytes each
	while (mbytes >= 32) {
		//Copy 32 bytes from 'm' into m_padded
		memcpy(m_padded, m, 32);
/*
		for (int i = 0; i < 32; i++) {
			printf("%02x", m_padded[i]);
		}
		printf("\n");
*/

		//Xor m_padded into 'block'
		xor (block, m_padded, 32);

		//Call perm for block
		perm152(block, block);

		//Increase/decrease the respective pointers by 32
		mbytes -= 32;
		m += 32;
	}

	//Check to see if there's still 'mbytes' left over
	if (mbytes != 0) {

		//Copy whatever leftover bytes from 'm' into m_padded
		memcpy(m_padded, m, (unsigned int)mbytes);

		//Set the first tail byte to 1 and then padded with 0s
		m_padded[mbytes] = 0b10000000;

		//Set the rest of the tail with leading 0s
		for (int i = mbytes + 1; i < 32; i++) {
			m_padded[i] = 0b00000000;
		}

/*

		for (int i = 0; i < 32; i++) {
			printf("%02x", m_padded[i]);
		}
*/

		//Xor m_padded into 'block'
		xor (block, m_padded, 32);

		//Call perm for block
		perm152(block, block);
	}

	//Copy first min(r,b) lf block to output
	int min = b;
	if (r < b) {
		min = r;
	}
	memcpy(res, block, (unsigned int)min);

	//Assignment shouldn't reach this if statement since 'b' is not greater than 'r', but included as per assignment pseudocode
	if (b > r) {
		perm152(block, block);
		memcpy(res, block, (unsigned int)(b - r));
	}


}