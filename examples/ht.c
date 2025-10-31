#define HT_IMPLEMENTATION
#include "../ht.h"
 

int main(void){
  ht_table ht = {0};
  ht_hash_append(&ht, "hello");
  ht_hash_append(&ht, "ifmmk");

  printf("string: %s, hash: %zu\n", ht.value[ht_hash_index(&ht, "ifmmk")], ht_hash_index(&ht, "ifmmk"));
  printf("string: %s, hash %zu\n", ht.value[ht_hash_index(&ht, "hello")], ht_hash_index(&ht, "hello"));
  return 0;
}
