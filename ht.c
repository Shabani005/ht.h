#include <stdbool.h>
#define NB_IMPLEMENTATION
#include "nb.h"
#include <stdint.h>
#include <assert.h>
 
#define NUM 4

// maybe add kv's to this struct
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


void hash_index(nb_ht* t, char* value){
  uint32_t hashv = hash(value, strlen(strdup(value)));
  assert(t->value[hashv]);
  if (strcmp(t->value[hashv], value) != 0){
    printf("found two values with same hash\n");
    printf("%s != %s\n", t->value[hashv], value);
  } 
  printf("value: %s\nhash: %d\n", t->value[hashv], hashv);
}

void hash_append(nb_ht* t, char* value){
  if (t->capacity == 0){
    t->capacity = 10000;
    t->count = 0;
    t->value = malloc(sizeof(char*) * t->capacity);
    t->hash = malloc(sizeof(uint32_t) * t->capacity);
  }
  if (t->count >= t->capacity){
    t->capacity *=2;
    t->value = realloc(t->value, sizeof(char*) * t->capacity);
    t->hash =  realloc(t->hash, sizeof(uint32_t) * t->capacity);
  }

  bool occupied = true;
  size_t counter = t->count;

  // this will not work -> should probably check using hash_index instead
  if (!t->hash[t->count]){
    t->hash[t->count] = hash(value, strlen(strdup(value)));
  } else if (t->hash[t->count]) {
    while (occupied){
      if (!t->hash[counter]++) occupied = false;    
    }
  }
  t->value[t->hash[t->count]] = value;
  t->count++;
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






// int main2(void){
//   nb_Strings strings = {0};
//   char* words[NUM] = {"hello", "world", "foo", "bar"};
//   for (size_t i=0; i<NUM; ++i){
//    nb_string_append(&strings, words[i]);
//    printf("hash: %d\n", hash(strings.value[i], strings.len[i]));
//   }  
// }
