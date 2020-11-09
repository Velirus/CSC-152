// Jeremy Tom
// CSC 152, #8986
// 10/24/2019
//
// Perm152 implementation using SSE registers

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <emmintrin.h>

static __m128i rotl(__m128i reg, int num) {
	__m128i hi = _mm_slli_epi32(reg, num);					//Shift each element to the left by num
	__m128i lo = _mm_srli_epi32(reg, 32 - num);				//Shift each element to the right by 32-num
	return _mm_or_si128(hi, lo);							//Return hi || lo
}

static void update(__m128i* w, __m128i* x, __m128i* y, __m128i* z) {
	*w = _mm_add_epi32(*w, *x);
	*z = _mm_xor_si128(*z, *w);
	*z = rotl(*z, 16);

	*y = _mm_add_epi32(*y, *z);
	*x = _mm_xor_si128(*x, *y);
	*x = rotl(*x, 12);

	*w = _mm_add_epi32(*w, *x);
	*z = _mm_xor_si128(*z, *w);
	*z = rotl(*z, 8);

	*y = _mm_add_epi32(*y, *z);
	*x = _mm_xor_si128(*x, *y);
	*x = rotl(*x, 7);


}
/*
static void psse_int(__m128i x) {     // Print an SSE register as if it holds four ints
	int* p = (int*)& x;
	for (int i = 0; i < 4; i++) {
		printf("%08X ", p[3 - i]);
	}
	printf("\n");
}
*/

void perm152(unsigned char* in, unsigned char* out) {  // each an array of 64 bytes
	__m128i reg[4];

	//Load in to SSE registers
	for (int i = 0; i < 4; i++) {

		reg[i] = _mm_loadu_si128((__m128i*) in);
		//Currently register is (a3, a2, a1, a0) for example

		//Rearrange  to where it's (a0, a1, a2, a3) and so on
		reg[i] = _mm_shuffle_epi32(reg[i], 0b00011011);
		in = in + 16;
//		psse_int(reg[i]);
	}

	for (int i = 0; i < 10; i++) {
		//Call first update
		update(&reg[0], &reg[1], &reg[2], &reg[3]);

		//Shift the vector elements of registers 
		reg[1] = _mm_shuffle_epi32(reg[1], 0b10010011);
		reg[2] = _mm_shuffle_epi32(reg[2], 0b01001110);
		reg[3] = _mm_shuffle_epi32(reg[3], 0b00111001);

		//Call second update
		update(&reg[0], &reg[1], &reg[2], &reg[3]);

		//Shift registers back
		reg[1] = _mm_shuffle_epi32(reg[1], 0b00111001);
		reg[2] = _mm_shuffle_epi32(reg[2], 0b01001110);
		reg[3] = _mm_shuffle_epi32(reg[3], 0b10010011);


	}

/*	psse_int(reg[0]);
	psse_int(reg[1]);
	psse_int(reg[2]);
	psse_int(reg[3]);
*/

	reg[0] = _mm_shuffle_epi32(reg[0], 0b00011011);
	reg[1] = _mm_shuffle_epi32(reg[1], 0b00011011);
	reg[2] = _mm_shuffle_epi32(reg[2], 0b00011011);
	reg[3] = _mm_shuffle_epi32(reg[3], 0b00011011);

/*	psse_int(reg[0]);
	psse_int(reg[1]);
	psse_int(reg[2]);
	psse_int(reg[3]);

*/

	_mm_storeu_si128((__m128i*) out, reg[0]);
	out = out + 16;
	_mm_storeu_si128((__m128i*) out, reg[1]);
	out = out + 16;
	_mm_storeu_si128((__m128i*) out, reg[2]);
	out = out + 16;
	_mm_storeu_si128((__m128i*) out, reg[3]);
	out = out + 16;

}