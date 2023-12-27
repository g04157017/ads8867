#ifndef JSCB_HASH
#define JSCB_HASH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if D_HASHSIZE < 101
#define LW_HASHSIZE	101
#else
#define LW_HASHSIZE	D_HASHSIZE
#endif

typedef struct HashTable {
	struct HashTable *next;     /* next entry in chain */
	uint hash_key;              /* intel hash key */
	uint full_key;              /* full key */
    uint index;                 /* value index */
    time_t seconds;
} HashTable;

HashTable **hash_new(void);
int hash_set(HashTable **hash, uint full_key, uint index);
int hash_get(HashTable **hash, uint full_key, uint& index);
int hash_del(HashTable **hash, uint full_key);
int hash_free(HashTable **hash);
void hash_dump(HashTable **hash);
long hash_long(long l);
uint hash_uint(uint i);
uint hash_bj6(uint i);
uint hash_bj7(uint i);
uint hash_tw3(uint i);

#endif

