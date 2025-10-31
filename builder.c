#define NB_IMPLEMENTATION
#include "nb.h"

int main(int argc, char** argv){
  nb_rebuild(argc, argv);

  nb_arr cmd = {0};

  nb_append_da(&cmd, "cc");
  nb_append_da(&cmd, "-o ht");
  nb_append_da(&cmd, "examples/ht.c");
  nb_cmd(&cmd);

  nb_append_da(&cmd, "./ht");
  nb_cmd(&cmd);
  return 0;
}
