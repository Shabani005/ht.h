#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
 
typedef struct{
  char**  value;
  size_t* len;
  size_t  count;
  size_t  capacity;
} ht_Strings;

typedef struct{
  char**    value;
  uint32_t* hash;
  size_t    count;
  size_t    capacity; 
} ht_table;



uint32_t ht_hash(const char* input, size_t size);
size_t   ht_hash_index(ht_table *t, const char *value);
void     ht_hash_append(ht_table *t, const char *value);
void     ht_string_append(ht_Strings* s, char* value);

#define TABLE_SIZE 1024

#ifdef HT_IMPLEMENTATION
uint32_t ht_hash(const char* input, size_t size){
  uint32_t total = 0;
  for (size_t i=0; i<size; ++i){
    total+= (uint32_t)input[i];
  }
  return total;
}

size_t  ht_hash_index(ht_table *t, const char *value) {
    uint32_t hashv = ht_hash(value, strlen(value));
    uint32_t start = hashv; // to detect full table loop

    while (t->value[hashv] != NULL) {
        if (strcmp(t->value[hashv], value) == 0) {
            // Found it
            // printf("Found '%s' at index %u\n", value, hashv);
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
    // printf("Inserting '%s' at index %u\n", value, hashv);
    t->value[hashv] = strdup(value);
    return hashv;
}

void ht_hash_append(ht_table *t, const char *value) {
    if (t->capacity == 0) {
        t->capacity = 10000;
        t->count = 0;
        t->value = calloc(t->capacity, sizeof(char *));
        t->hash = calloc(t->capacity, sizeof(uint32_t));
    }

    if (t->count >= t->capacity * 0.7) { 
        t->capacity *= 2;
        t->value = realloc(t->value, sizeof(char*) * t->capacity);
        t->hash  = realloc(t->hash, sizeof(uint32_t) * t->capacity);
        memset(t->value + t->count, 0,
               sizeof(char*) * (t->capacity - t->count));
        memset(t->hash + t->count, 0,
               sizeof(uint32_t) * (t->capacity - t->count));
    }

    uint32_t hashv =
        ht_hash(value, strlen(value)) % t->capacity; 
    uint32_t start = hashv;                       

    while (t->value[hashv] != NULL) {
        if (strcmp(t->value[hashv], value) == 0) {
            // printf("Value '%s' already exists at index %u\n", value, hashv);
            return;
        }

        hashv = (hashv + 1) % t->capacity;

        if (hashv == start) {
            fprintf(stderr, "Error: hash table full\n");
            exit(EXIT_FAILURE);
        }
    }
    t->value[hashv] = strdup(value);
    t->hash[hashv] = hashv;
    t->count++;
    // printf("Inserted '%s' at index %u\n", value, hashv);
}


void ht_string_append(ht_Strings* s, char* value){
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
#endif
