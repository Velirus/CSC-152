// Jeremy Tom
// 8986
// 9/20/2019
//
// Inverse function of hw2_perm152 where it reverts the permutation

#include <stdio.h>
#include <stdint.h>
#include <string.h>

static uint32_t rotr(uint32_t arr, int num);


static void update(uint32_t* w, uint32_t* x, uint32_t* y, uint32_t* z) {
	uint32_t wTmp = *w;
	uint32_t xTmp = *x;
	uint32_t yTmp = *y;
	uint32_t zTmp = *z;

//	printf("Start values:\n w: %lx, x: %lx, y: %lx, z: %lx\n\n", wTmp, xTmp, yTmp, zTmp);

	xTmp = rotr(xTmp, 7);
//	printf("x = rotl(x, 7)\n w: %lx, x: %lx, y: %lx, z: %lx\n\n", wTmp, xTmp, yTmp, zTmp);

	xTmp = xTmp ^ yTmp;
//	printf("x = x ^ y\n w: %lx, x: %lx, y: %lx, z: %lx\n\n", wTmp, xTmp, yTmp, zTmp);

	yTmp = yTmp - zTmp;
//	printf("y = y - z\n w: %lx, x: %lx, y: %lx, z: %lx\n\n", wTmp, xTmp, yTmp, zTmp);

	zTmp = rotr(zTmp, 8);
//	printf("z = rotr(z,8)\n w: %lx, x: %lx, y: %lx, z: %lx\n\n", wTmp, xTmp, yTmp, zTmp);

	zTmp = zTmp ^ wTmp;
//	printf("z = z ^ w\n w: %lx, x: %lx, y: %lx, z: %lx\n\n", wTmp, xTmp, yTmp, zTmp);

	wTmp = wTmp - xTmp;
//	printf("w = w - x\n w: %lx, x: %lx, y: %lx, z: %lx\n\n", wTmp, xTmp, yTmp, zTmp);

	xTmp = rotr(xTmp, 12);
//	printf("x = rotr(x,12)\n w: %lx, x: %lx, y: %lx, z: %lx\n\n", wTmp, xTmp, yTmp, zTmp);

	xTmp = xTmp ^ yTmp;
//	printf("x = x ^ y\n w: %lx, x: %lx, y: %lx, z: %lx\n\n", wTmp, xTmp, yTmp, zTmp);

	yTmp = yTmp - zTmp;
//	printf("y = y - z\n w: %lx, x: %lx, y: %lx, z: %lx\n\n", wTmp, xTmp, yTmp, zTmp);

	zTmp = rotr(zTmp, 16);
//	printf("*z = rotr(z, 16)\n w: %lx, x: %lx, y: %lx, z: %lx\n\n", wTmp, xTmp, yTmp, zTmp);

	zTmp = zTmp ^ wTmp;
//	printf("*z = *z ^ *w\n w: %lx, x: %lx, y: %lx, z: %lx\n\n", wTmp, xTmp, yTmp, zTmp);

	wTmp = wTmp - xTmp;
//	printf("w = w - x\n w: %lx, x: %lx, y: %lx, z: %lx\n\n", wTmp, xTmp, yTmp, zTmp);

	*w = wTmp;
	*x = xTmp;
	*y = yTmp;
	*z = zTmp;
//	printf("Result IN update: \n a[0]: %08lx, a[4]: %08lx, a[8]: %08lx, a[12]: %08lx\n", *w, *x, *y, *z);


}

static uint32_t rotr(uint32_t arr, int num) {
	uint32_t hi, lo;
	hi = arr >> num;
	lo = arr << (32 - num);
	hi = hi | lo;
	return hi;
}

void perm152inverse(unsigned char* in, unsigned char* out) { // each an array of 64 bytes
	uint32_t a[16];											// Array of 16 uint32_t variables
	int inArrayNum = 0;										// Array number in 'in'. Initialize to 0

	for (int i = 0; i < 16; i++) {
		unsigned char buffer[] = { in[inArrayNum],				//Create a temporary buffer that stores an array of 4 bytes 
								   in[inArrayNum + 1],			//Ex: buffer[] = {in[3], in[2], in[1], in[0]}
								   in[inArrayNum + 2],			//Memcpy the 4 bytes in 'buffer' into a[i]					
								   in[inArrayNum + 3] };		//Ex: a[1] contains buffer[] = {in[3], in[2], in[1], in[0]}
		memcpy(&a[i], buffer, 4);								//    a[2] contains buffer[] = {in[7], in[6], in[5], in[4]}

//		printf("a[%d]: %08lx\n", i, a[i]);
		inArrayNum += 4;											//Increase inArrayNum by 4 for the next 4 bytes of arrays in 'in'

	}

	for (int j = 0; j < 10; j++) {
		update(&a[3], &a[4], &a[9], &a[14]);
		update(&a[2], &a[7], &a[8], &a[13]);
		update(&a[1], &a[6], &a[11], &a[12]);
		update(&a[0], &a[5], &a[10], &a[15]);
		update(&a[3], &a[7], &a[11], &a[15]);
		update(&a[2], &a[6], &a[10], &a[14]);
		update(&a[1], &a[5], &a[9], &a[13]);
		update(&a[0], &a[4], &a[8], &a[12]);
	}

	/* Store the resulting bytes into 'out'*/
	inArrayNum = 0;											//set inArrayNum back to 0
	for (int i = 0; i < 16; i++) {							//Assign 1 bit from uint32_t a to out[i]
		out[inArrayNum] = a[i] & 0xFF;						//Assign first 8 bits
		out[inArrayNum + 1] = (a[i] >> 8) & 0xFF;			//Assign second 8 bits
		out[inArrayNum + 2] = (a[i] >> 16) & 0xFF;			//Assign third 8 bits
		out[inArrayNum + 3] = (a[i] >> 24) & 0xFF;			//Assign fourth 8 bits
//		printf("out[%d]: %02X ", inArrayNum, out[inArrayNum]);
//		printf("out[%d]: %02X ", inArrayNum+1, out[inArrayNum+1]);
//		printf("out[%d]: %02X ", inArrayNum+2, out[inArrayNum+2]);
//		printf("out[%d]: %02X\n", inArrayNum+3, out[inArrayNum+3]);
//		printf("out[%d...%d]: %02X%02X%02X%02X\n", inArrayNum, inArrayNum + 3,		//a[0...15] representation
//													out[inArrayNum],
//													out[inArrayNum + 1],
//													out[inArrayNum + 2],
//													out[inArrayNum + 3]);
		inArrayNum += 4;
	}
}