#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define WITH_ABI_MESSAGE
#include "../ejs.h"
#include "../utils.h"

TEST(test_ej3_sin_ids) {
  uint32_t ids = {};
  usuario_t **result1 = TEST_CALL_S(asignarNivelesParaNuevosUsuarios, &ids, 0,
                                    seleccionDePrivilegio);

  TEST_ASSERT(result1 == NULL);
}

TEST(test_ej3_unico_id) {
  srand((unsigned int)time(NULL));
  uint32_t random_id = (uint32_t)rand();
  uint32_t ids[1] = {random_id};
  uint32_t ids_copy[1] = {random_id}; // Copy for integrity check

  usuario_t u1 = {random_id, seleccionDePrivilegio(random_id)};
  usuario_t **result1 = TEST_CALL_S(asignarNivelesParaNuevosUsuarios, ids, 1,
                                    seleccionDePrivilegio);

  TEST_ASSERT(result1 != NULL);
  TEST_ASSERT(sonUsuariosIguales(result1[0], &u1));
  TEST_ASSERT(ids[0] == ids_copy[0]); // Check integrity

  free(result1[0]);
  free(result1);
}

TEST(test_ej3_multiples_ids) {
  srand((unsigned int)time(NULL));
  uint32_t ids[4];
  uint32_t ids_copy[4];
  for (int i = 0; i < 4; i++) {
    ids[i] = (uint32_t)rand();
    ids_copy[i] = ids[i];
  }

  usuario_t u0 = {ids[0], seleccionDePrivilegio(ids[0])};
  usuario_t u1 = {ids[1], seleccionDePrivilegio(ids[1])};
  usuario_t u2 = {ids[2], seleccionDePrivilegio(ids[2])};
  usuario_t u3 = {ids[3], seleccionDePrivilegio(ids[3])};

  usuario_t **result2 = TEST_CALL_S(asignarNivelesParaNuevosUsuarios, ids, 4,
                                    seleccionDePrivilegio);

  TEST_ASSERT(result2 != NULL);

  TEST_ASSERT(sonUsuariosIguales(result2[0], &u0));
  TEST_ASSERT(sonUsuariosIguales(result2[1], &u1));
  TEST_ASSERT(sonUsuariosIguales(result2[2], &u2));
  TEST_ASSERT(sonUsuariosIguales(result2[3], &u3));

  TEST_ASSERT(ids[0] == ids_copy[0]); // Check integrity
  TEST_ASSERT(ids[1] == ids_copy[1]);
  TEST_ASSERT(ids[2] == ids_copy[2]);
  TEST_ASSERT(ids[3] == ids_copy[3]);

  for (int i = 0; i < 4; i++) {
    free(result2[i]);
  }

  free(result2);
}

int main(int argc, char *argv[]) {
  printf("Corriendo los tests del ejercicio 3...\n");
  test_ej3_sin_ids();
  test_ej3_unico_id();
  test_ej3_multiples_ids();

  tests_end("Ejercicio 3");
}
