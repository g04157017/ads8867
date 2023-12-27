#include "utils.h"
#include "hash.h"

/* Static functions declations */
static int _undef(HashTable **hashtab, uint full_key);
static HashTable *_lookup(HashTable **hash, uint hash_key);
static HashTable *_install(HashTable **hash, uint full_key, uint index);

/* 
 *   FUNCTION   :
 *         Name : hash_new
 *  Description : This will create new HashTable.
 * 
 */
HashTable **hash_new(void)
{
	HashTable **hash = NULL;

	/* Allocate memory */
	hash = (HashTable**)malloc(LW_HASHSIZE * sizeof(HashTable *));
	if (hash == NULL) {
		return NULL;
	}

	/* Initialize all pointers points to NULL */
	int n;
	for (n = 0; n < LW_HASHSIZE; n++) {
		*(hash+n) = NULL;
	}

	return hash;
}

/* 
 *   FUNCTION   :
 *         Name : hash_get
 *  Description : This will get value from HASH by given KEY.
 *  
 */
int hash_get(HashTable **hash, uint full_key, uint& index)
{
	HashTable *np;
    time_t currents = time((time_t*)NULL);
    time_t durs     = 0;
	if ((np = _lookup(hash, full_key)) != NULL) 
    {
        durs = currents - np->seconds;
        if (durs >= 0 && durs < 300)
        {
            index = np->index;
		    return OK;	/* found */
        }
        else
        {
		    return ERR;
        }
	}
	return ERR;	/* not found */
}

/* 
 *   FUNCTION   :
 *         Name : hash_set
 *  Description : This will set value to HASH for the given KEY.
 *  
 */
int hash_set(HashTable **hash, uint full_key, uint index)
{
	if (_install(hash, full_key, index) != NULL) {
		return OK;
	}
	return ERR;
}

/* 
 *   FUNCTION   :
 *         Name : hash_del
 *  Description : This will remove a key from HASH.
 *  
 */
int hash_del(HashTable **hash, uint full_key)
{
	return _undef(hash, full_key);
}

/* 
 *   FUNCTION   :
 *         Name : hash_free
 *  Description : This will free all HASH objects.
 *  
 */
int hash_free(HashTable **hash)
{
	int i;
	HashTable *np1, *np2;

	if (hash == NULL)
		return ERR;	/* Not a valid 'hash' pointer */

	for (i=0; i < LW_HASHSIZE; i++) 
    {
		for (np1 = np2 = hash[i]; np1 != NULL; np1 = np2) 
        {
			np2 = np1->next;
			/* Free memory */
			free(np1);
		}
		hash[i] = NULL;
	}

	return OK;
}

/* 
 *   FUNCTION   :
 *         Name : hash_dump
 *  Description : This will dump HASH key-value pairs.
 *  
 */
void hash_dump(HashTable **hash)
{
	int i;
	HashTable *np;

	if (hash == NULL)
		return;	/* Not a valid 'hash' pointer */

	printf("Hash Dump : {\n");
	for (i=0; i < LW_HASHSIZE; i++) 
    {
		for (np = hash[i]; np != NULL; np = np->next)	/* loop through each sub-list */
			printf("\t[%u - %u] : [%u],\n", np->hash_key, np->full_key, np->index);
	}
	printf("}\n\n");
}

/* Full-avalanche integer hashing function from Bob Jenkins, suitable for use
 * with a modulo. It has a very smooth distribution.
 * http://burtleburtle.net/bob/hash/integer.html
 * About 0x50 bytes, 6 shifts.
 */
uint hash_bj6(uint i)
{
    uint a = i;
    a = (a+0x7ed55d16) + (a<<12);
    a = (a^0xc761c23c) ^ (a>>19);
    a = (a+0x165667b1) + (a<<5);
    a = (a+0xd3a2646c) ^ (a<<9);
    a = (a+0xfd7046c5) + (a<<3);
    a = (a^0xb55a4f09) ^ (a>>16);
    return a % LW_HASHSIZE;
}

/* Similar function with one more shift and no magic number. It is slightly
 * slower but provides the overall smoothest distribution.
 * About 0x40 bytes, 7 shifts.
 */

uint hash_uint(uint i)
{
    uint a = i;
    a -= (a<<6);
    a ^= (a>>17);
    a -= (a<<9);
    a ^= (a<<4);
    a -= (a<<3);
    a ^= (a<<10);
    a ^= (a>>15);
    return a % LW_HASHSIZE;
}

/* Thomas Wang's multiplicative hash function. About 0x30 bytes, and it is
 * extremely fast on recent processors with a fast multiply. However, it
 * must not be used on low bits only, as multiples of 0x00100010 only return
 * even values !
 */
uint hash_tw3(uint i)
{
    uint a = i;
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

// Thomas Wang's 32bit hash
uint hash_bj7(uint i) 
{ 
    uint key = i;
    key = ~key + (key << 15);   // key = (key << 15) - key - 1;
    key = key ^ (key >> 12);
    key = key + (key << 2);
    key = key ^ (key >> 4);
    key = key * 2057;           // key = (key + (key << 3)) + (key << 11);
    key = key ^ (key >> 16);
    return key % LW_HASHSIZE;
}

// Thomas Wang's 64bit hash
long hash_long(long l)
{ 
    unsigned long key = l;
    key = (~key) + (key << 21);  // key = (key << 21) - key - 1;
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8);  // key * 265
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4);  // key * 21
    key = key ^ (key >> 28);
    key = key + (key << 31);
    return key % LW_HASHSIZE;
}

/* 
 *   FUNCTION   :
 *         Name : _hashval
 *  Description : internal func: form hash value for string "s"
 *  
 */
unsigned int hash_val(const char *s)
{
	unsigned int hashval;
	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % LW_HASHSIZE;
}

/* 
 *   FUNCTION   :
 *         Name : _lookup
 *  Description : internal func: look for s in HASH
 *  
 */
static HashTable *_lookup(HashTable **hash, uint full_key)
{
	HashTable *np;
    unsigned hash_key = 0;
    hash_key = hash_uint(full_key);
	for (np = hash[hash_key]; np != NULL; np = np->next) 
    {
		if ((hash_key == np->hash_key) && (full_key == np->full_key))
		{
		    return np;	/* found */
        }
	}
	return NULL;	/* not found */
}

/* 
 *   FUNCTION   :
 *         Name : _install
 *  Description : internal func: put (name, defn) in hashtab
 *  
 */
static HashTable *_install(HashTable **hash, uint full_key, uint index)
{
	HashTable *np    = NULL;
	unsigned hash_key = 0;
    hash_key = hash_uint(full_key);
	if ((np = _lookup(hash, full_key)) == NULL) 
    {   /* not found */
		np = (HashTable *) malloc(sizeof(HashTable));
		CHK_NULL(np, NULL);

		np->next  = hash[hash_key];
		hash[hash_key] = np;
	} 
    else
    {
        np->index = index;
    }

    np->hash_key = hash_key;
    np->full_key = full_key;    
    np->index    = index;
    np->seconds  = time((time_t*)NULL);

	return np;
}

/* 
 *   FUNCTION   :
 *         Name : _undef
 *  Description : internal func: remove (name) in hashtab
 *  
 */
static int _undef(HashTable **hash, uint full_key)
{
	HashTable *np1, *np2;
	unsigned hash_key = 0;
    hash_key = hash_uint(full_key);
    
	if ((np1 = _lookup(hash, full_key)) == NULL)
		return ERR;	/* name not found */

	for ( np1 = np2 = hash[hash_key]; np1 != NULL; np2 = np1, np1 = np1->next )
    {
		if ((hash_key == np1->hash_key) && (full_key == np1->full_key))
        { 
			/* Remove node from list */
			if ( np1 == np2 )
				hash[hash_key]= np1->next;
			else
				np2->next = np1->next;

			/* Free memory */
			free(np1);
			return OK;	/* successfully removed from 'hash' */
		}
	}

	return ERR;  /* name not found */
}

