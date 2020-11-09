#include <stdio.h>
#include <stdint.h>

static uint64_t reverse_bytes(uint64_t x);
static uint64_t left_rotation(uint64_t x, int num);
static uint64_t addition(uint64_t x, uint64_t y);
static uint64_t round_plus(uint64_t x, uint64_t y);

void hash_round(uint64_t *v) {		// v[0], v[1], v[2], v[3] are each uint64_t
	uint64_t v0 = v[0];
	uint64_t v1 = v[1];
	uint64_t v2 = v[2];
	uint64_t v3 = v[3];
	//printf("Results BEFORE hash_round\n %llx\n %llx\n %llx\n %llx\n", v0, v1, v2, v3);

	v0 = reverse_bytes(v0);
	v1 = reverse_bytes(v1);
	v2 = reverse_bytes(v2);
	v3 = reverse_bytes(v3);
	//printf("\nConvert to Big Endian \n %llx\n %llx\n %llx\n %llx\n", v0, v1, v2, v3);


	v0 = addition(v0, v1);
//	printf("v0 + v1 = %llx\n", v0);

	v1 = left_rotation(v1, 13);
	v1 = round_plus(v1, v0);
//	printf("v1 rotate left by 13 and xor v0 = %llx\n", v1);

	v0 = left_rotation(v0, 32);
//	printf("v0 rotate left by 32 = %llx\n", v[0]);

	v2 = addition(v2, v3);
//	printf("v2 + v3 = %llx\n", v[0]);

	v3 = left_rotation(v3, 16);
	v3 = round_plus(v3, v2);
//	printf("v3 rotate left by 16 and xor v2 = %llx\n", v3);

	v2 = addition(v2, v1);
//	printf("v2 + v1 = %llx\n", v2);

	v1 = left_rotation(v1, 17);
	v1 = round_plus(v1, v2);
//	printf("v1 rotate left by 17 and xor v2 = %llx\n", v1);

	v2 = left_rotation(v2, 32);
//	printf("v2 rotate left by 32 = %llx\n", v2);

	v0 = addition(v0, v3);
//	printf("v0 + v3 = %llx\n", v0);

	v3 = left_rotation(v3, 21);
	v3 = round_plus(v3, v0);
//	printf("v3 rotate left by 21 and xor v0 = %llx\n", v0);


	v[0] = reverse_bytes(v0);		// undo the reverse of the bytes
	v[1] = reverse_bytes(v1);
	v[2] = reverse_bytes(v2);
	v[3] = reverse_bytes(v3);

//	printf("\nResults AFTER hash_round (again)\n %llx\n %llx\n %llx\n %llx\n", v[0], v[1], v[2], v[3]);
}

static uint64_t reverse_bytes(uint64_t x) {
	uint64_t temp;

	uint64_t a = (x & 0x00000000000000FF) << 56;
	uint64_t b = (x & 0x000000000000FF00) << 40;
	uint64_t c = (x & 0x0000000000FF0000) << 24;
	uint64_t d = (x & 0x00000000FF000000) << 8;
	uint64_t e = (x & 0x000000FF00000000) >> 8;
	uint64_t f = (x & 0x0000FF0000000000) >> 24;
	uint64_t g = (x & 0x00FF000000000000) >> 40;
	uint64_t h = (x & 0xFF00000000000000) >> 56;

	temp = a | b | c | d | e | f | g | h;
	return temp;

}

static uint64_t left_rotation(uint64_t x, int num) {
	uint64_t hi, lo, result;
	hi = x << num;
	lo = x >> (64 - num);

	result = hi | lo;

	//printf("Result of left rotation: %llx \n", result);

	return result;

}

static uint64_t addition(uint64_t x, uint64_t y) {  //Add 2 unit64_t where the sum is returned
	uint64_t temp;

	temp = (x + y);

	//printf("v0 + v1 = %llx", temp);

	return temp;
}


static uint64_t round_plus(uint64_t x, uint64_t y) {
	uint64_t temp;

	temp = x ^ y;

	return temp;
}