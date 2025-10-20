#include <stdbool.h>
#define NB_IMPLEMENTATION
#include "nb.h"
#include <stdint.h>
#include <assert.h>
 
typedef struct{
  char**  value;
  size_t* len;
  size_t  count;
  size_t  capacity;
} nb_Strings;

typedef struct{
  char**    value;
  uint32_t* hash;
  size_t    count;
  size_t    capacity; 
} nb_ht;


#define TABLE_SIZE 1024

// hashes may collide if sum of "string"[i] of two different
// strings are different
// if two collide we try to give it a value of hash +=1
// if we later index using a string and the initial indexing gives a different one
// we linearly search hash+=1 until we find the actual value
// O(1) best case idk what complexity worst case is
// the same strings will always return the same value
// thus after hashing we can use it to index. 

uint32_t hash(const char* input, size_t size){
  uint32_t total = 0;
  for (size_t i=0; i<size; ++i){
    total+= (uint32_t)input[i];
  }
  return total;
}

// I think this is murmurhash. can be interchangable if I implement linear probing
// uint32_t hash(const char *str, size_t size){
//   unsigned long hash = 5381;
//   int c;
//   int len=0;
//   while (len < size){
//     hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//     len++;
//     }
//   return hash;
// }

// void hash_index(nb_ht* t, char* value){
//   uint32_t hashv = hash(value, strlen(strdup(value)));
//   assert(t->value[hashv]);
//   if (strcmp(t->value[hashv], value) != 0){
//     printf("found different values with same hash\n");
//     printf("%s != %s\n", t->value[hashv], value);
//     if(!t->hash[hashv+1]) {
//       fprintf(stderr, "TODO: no value at hash+1\n");
//       exit(-1);
//     }
//     if(strcmp(t->value[hashv+1], value) == 0){
//       printf("found at hash+1");
//     }
//   } 
//   printf("value: %s\nhash: %d\n", t->value[hashv], hashv);
// }

// void hash_append(nb_ht* t, char* value){
//   if (t->capacity == 0){
//     t->capacity = 10000;
//     t->count = 0;
//     t->value = malloc(sizeof(char*) * t->capacity);
//     t->hash = malloc(sizeof(uint32_t) * t->capacity);
//   }
//   if (t->count >= t->capacity){
//     t->capacity *=2;
//     t->value = realloc(t->value, sizeof(char*) * t->capacity);
//     t->hash =  realloc(t->hash, sizeof(uint32_t) * t->capacity);
//   }

//   bool occupied = true;
//   size_t counter = t->count;

//   // this will not work -> should probably check using hash_index instead
//   if (!t->hash[t->count]){
//     t->hash[t->count] = hash(value, strlen(strdup(value)));
//   } else if (t->hash[t->count]) {
//     while (occupied){
//       if (!t->hash[counter++]) occupied = false;    
//     }
//   }
//   t->value[t->hash[t->count]] = value;
//   t->count++;
// }


int hash_index(nb_ht *t, const char *value) {
    uint32_t hashv = hash(value, strlen(value));
    uint32_t start = hashv; // to detect full table loop

    while (t->value[hashv] != NULL) {
        if (strcmp(t->value[hashv], value) == 0) {
            // Found it
            printf("Found '%s' at index %u\n", value, hashv);
            return hashv;
        }

        // Collision — probe next index
        hashv = (hashv + 1) % TABLE_SIZE;

        if (hashv == start) {
            fprintf(stderr, "Error: hash table is full\n");
            exit(EXIT_FAILURE);
        }
    }

    // Empty slot found
    printf("Inserting '%s' at index %u\n", value, hashv);
    t->value[hashv] = strdup(value);
    return hashv;
}

void hash_append(nb_ht *t, const char *value) {
    if (t->capacity == 0) {
        t->capacity = 10000;
        t->count = 0;
        t->value = calloc(t->capacity, sizeof(char *));
        t->hash = calloc(t->capacity, sizeof(uint32_t));
    }

    if (t->count >= t->capacity * 0.7) { // resize if load factor >= 0.7
        t->capacity *= 2;
        t->value = realloc(t->value, sizeof(char *) * t->capacity);
        t->hash = realloc(t->hash, sizeof(uint32_t) * t->capacity);
        memset(t->value + t->count, 0,
               sizeof(char *) * (t->capacity - t->count));
        memset(t->hash + t->count, 0,
               sizeof(uint32_t) * (t->capacity - t->count));
    }

    uint32_t hashv =
        hash(value, strlen(value)) % t->capacity; // always mod capacity
    uint32_t start = hashv;                       // to detect full table

    // Linear probing
    while (t->value[hashv] != NULL) {
        // Already exists? Do nothing.
        if (strcmp(t->value[hashv], value) == 0) {
            printf("Value '%s' already exists at index %u\n", value, hashv);
            return;
        }

        hashv = (hashv + 1) % t->capacity;

        if (hashv == start) {
            fprintf(stderr, "Error: hash table full\n");
            exit(EXIT_FAILURE);
        }
    }

    // Empty slot found
    t->value[hashv] = strdup(value);
    t->hash[hashv] = hashv; // optional: could store the actual hash
    t->count++;

    printf("Inserted '%s' at index %u\n", value, hashv);
}


void nb_string_append(nb_Strings* s, char* value){
  if (s->capacity == 0) {
    s->capacity = 256;
    s->count = 0;
    s->value = malloc(sizeof(char*)*s->capacity);
    s->len = malloc(sizeof(size_t)*s->capacity);
  }
  if (s->count >= s->capacity){ s->capacity +=2;
    s->value = realloc(s->value, sizeof(char*)*s->capacity);
    s->len = realloc(s->len, sizeof(size_t)*s->capacity); 
  }
  s->value[s->count] = value;
  char* buf = strdup(value); 
  s->len[s->count] = strlen(buf); // this may not work
  s->count++;
}

int main(void){
  nb_ht ht = {0};
  hash_append(&ht, "hello");
  hash_append(&ht, "ifmmk");

  hash_index(&ht, "ifmmk");
  printf("\n");
  hash_index(&ht, "hello");

  return 0;
}
