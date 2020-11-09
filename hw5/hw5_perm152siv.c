/*
Jeremy Tom
CSC 152, #8986
11/6/2019
Authenticated encryption using perm152hash and perm152ctr and implementing a SIV variation
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void perm152hash(unsigned char* m, int mbytes, unsigned char* res);
void perm152ctr(unsigned char* in, unsigned char* out, int nbytes, unsigned char* block, unsigned char* key, int kbytes);

void perm152siv_encrypt(unsigned char* k, int kbytes, // up to 32 bytes
	unsigned char* n, int nbytes, // up to 16 bytes
	unsigned char* p, int pbytes, // up to 2^31 - 1 bytes
	unsigned char* siv,// 16 bytes
	unsigned char* c)// pbytes long

{	
	
	unsigned char* buf = malloc((16+32+pbytes) * sizeof(unsigned char));	//Allocate 16 + 32 + pbytes bytes using malloc
	unsigned char* bufBegin = buf;											//Copy of the buf ptr for beginning of it											
	unsigned char block[64];												//Block = siv || 48 bytes of 0

	//k zero padded to 32 bytes
	for (int i = 0; i < 32; i++) {
		buf[i] = 0x00;
	}
	memcpy(buf, k, kbytes);
	buf = buf + 32;

	//n zero padded to 16 bytes
	for (int i = 0; i < 16; i++) {
		buf[i] = 0x00;
	}

	//Store n into buf
	memcpy(buf, n, nbytes);
	buf = buf + 16;


	//Store p into buf
	memcpy(buf, p, pbytes);

	//Do the hash starting at the beginning of buf (bufBegin) and return it to bufBegin
	perm152hash(bufBegin, 16 + 32 + pbytes, bufBegin);

	//Copy first 16 bytes of buf (aka bufBegin) to siv
	memcpy(siv, bufBegin, 16);

	//Copy siv into block
	memcpy(block, siv, 16);

	//Pad block with 48 bytes of zero
	for (int i = 16; i < 64; i++) {
		block[i] = 0x00;
	}

	//Do perm152ctr and output to c
	perm152ctr(k, c, kbytes, block, p, pbytes);


	//Free buf and bufBegin
	memset(buf, 0, 16+32+pbytes * sizeof(unsigned char));
	memset(bufBegin, 0, 16 + 32 + pbytes * sizeof(unsigned char));
	free(buf);
	free(bufBegin);
}

int perm152siv_decrypt(unsigned char* k, int kbytes, // up to 32 bytes
	unsigned char* n, int nbytes, // up to 16 bytes
	unsigned char* c, int cbytes, // up to 2^31 - 1 bytes
	unsigned char* siv,// 16 bytes
	unsigned char* p) // cbytes long
{
	unsigned char* buf = malloc((16 + 32 + cbytes) * sizeof(unsigned char));	//Allocate 16 + 32 + cbytes bytes using malloc
	unsigned char* bufBegin = buf;												//Copy of the buf ptr for beginning of it			
	unsigned char* tempP = malloc(cbytes * sizeof(unsigned char));
	unsigned char block[64];													//Block = siv || 48 bytes of 0

	//store 64 bytes of 0 to block
	for (int i = 0; i < 64; i++) {
		block[i] = 0x00;
	}
	
	//Memcpy siv to first 16 bytes of block
	memcpy(block, siv, 16);

	//Do perm152ctr and output to tempP
	perm152ctr(k, tempP, kbytes, block, c, cbytes);

	//k zero padded to 32 bytes
	for (int i = 0; i < 32; i++) {
		buf[i] = 0x00;
	}
	memcpy(buf, k, kbytes);
	buf = buf + 32;

	//n zero padded to 16 bytes
	for (int i = 0; i < 16; i++) {
		buf[i] = 0x00;
	}

	//Store n into buf
	memcpy(buf, n, nbytes);
	buf = buf + 16;

	//Store p into buf
	memcpy(buf, p, cbytes);

	//Do the hash starting at the beginning of buf (bufBegin) and return it to bufBegin
	perm152hash(bufBegin, 16 + 32 + cbytes, bufBegin);

	int i = 0;

	//Compare siv and first 16 bytes of bufBegin to see if they match
	while (i < 16) {
		//If there is a mismatch, return -1 and don't output to p
		if (siv[i] != bufBegin[i]) {
			//Free buf and tempP
			memset(buf, 0, 16 + 32 + cbytes * sizeof(unsigned char));
			memset(tempP, 0, cbytes * sizeof(unsigned char));
			free(buf);
			free(tempP);

			return -1;
		}
		i++;
	}

	//Output to p if pass
	memcpy(p, tempP, cbytes);

	//Free buf and tempP
	memset(buf, 0, 16 + 32 + cbytes * sizeof(unsigned char));
	memset(tempP, 0, cbytes * sizeof(unsigned char));
	free(buf);
	free(tempP);

	return 0;

}