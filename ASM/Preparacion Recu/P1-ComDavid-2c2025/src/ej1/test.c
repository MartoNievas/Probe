#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define WITH_ABI_MESSAGE
#include "../ejs.h"
#include "../utils.h"

TEST(test_ej1_lista_vacia) {
  catalogo_t catalogo1 = {NULL};
  producto_t *result1 =
      TEST_CALL_S(filtrarPublicacionesNuevasDeUsuariosVerificados, &catalogo1);
  int count1 = contar_items_resultado(result1);

  TEST_ASSERT_EQUALS(int, 0, count1);

  if (result1)
    free(result1);
}

TEST(test_ej1_niveles) {
  catalogo_t catalogo2 = {NULL};

  usuario_t *u1 = crear_usuario(1, 0); // nivel 0
  usuario_t *u2 = crear_usuario(2, 1); // nivel 1
  usuario_t *u3 = crear_usuario(3, 2); // nivel 2

  producto_t *item1 =
      crear_item(u1, "electro", "celular", 1, 500, 32); // nivel 0, nuevo -> NO
  producto_t *item2 =
      crear_item(u2, "hogar", "mesa", 1, 200, 2); // nivel 1, nuevo -> SÍ
  producto_t *item3 =
      crear_item(u3, "ropa", "camisa", 1, 100, 1); // nivel 2, nuevo -> SÍ

  producto_t *item1copy =
      crear_item(u1, "electro", "celular", 1, 500, 32); // nivel 0, nuevo -> NO
  producto_t *item2copy =
      crear_item(u2, "hogar", "mesa", 1, 200, 2); // nivel 1, nuevo -> SÍ
  producto_t *item3copy =
      crear_item(u3, "ropa", "camisa", 1, 100, 1); // nivel 2, nuevo -> SÍ

  agregar_nodo(&catalogo2, crear_nodo(item1));
  agregar_nodo(&catalogo2, crear_nodo(item2));
  agregar_nodo(&catalogo2, crear_nodo(item3));

  producto_t *result2 =
      TEST_CALL_S(filtrarPublicacionesNuevasDeUsuariosVerificados, &catalogo2);
  int count2 = contar_items_resultado(result2);

  TEST_ASSERT_EQUALS(int, 2, count2);
  TEST_ASSERT(item_en_resultado(result2, item2));
  TEST_ASSERT(item_en_resultado(result2, item3));

  TEST_ASSERT(verificarIntegridad(item1, item1copy));
  TEST_ASSERT(verificarIntegridad(item2, item2copy));
  TEST_ASSERT(verificarIntegridad(item3, item3copy));

  if (result2)
    free(result2);

  free(item1copy);
  free(item2copy);
  free(item3copy);

  free(item1);
  free(item2);
  free(item3);

  free(u1);
  free(u2);
  free(u3);
  liberar_lista(&catalogo2);
}

TEST(test_ej1_estados) {
  catalogo_t catalogo3 = {NULL};

  usuario_t *u1 = crear_usuario(1, 1); // nivel 1
  usuario_t *u2 = crear_usuario(2, 1); // nivel 1 (usuario diferente)

  producto_t *item1 = crear_item(u1, "electro", "tv", 0, 800, 0); // usado -> NO
  producto_t *item2 =
      crear_item(u2, "electro", "radio", 1, 300, 1); // nuevo -> SÍ

  producto_t *item1copy =
      crear_item(u1, "electro", "tv", 0, 800, 0); // usado -> NO
  producto_t *item2copy =
      crear_item(u2, "electro", "radio", 1, 300, 1); // nuevo -> SÍ

  agregar_nodo(&catalogo3, crear_nodo(item1));
  agregar_nodo(&catalogo3, crear_nodo(item2));

  producto_t *result3 =
      TEST_CALL_S(filtrarPublicacionesNuevasDeUsuariosVerificados, &catalogo3);
  int count3 = contar_items_resultado(result3);

  TEST_ASSERT_EQUALS(int, 1, count3);
  TEST_ASSERT(item_en_resultado(result3, item2));

  TEST_ASSERT(verificarIntegridad(item1, item1copy));
  TEST_ASSERT(verificarIntegridad(item2, item2copy));

  if (result3)
    free(result3);

  free(item1copy);
  free(item2copy);

  free(item1);
  free(item2);

  free(u1);
  free(u2);

  liberar_lista(&catalogo3);
}

TEST(test_ej1_ninguno) {
  catalogo_t catalogo4 = {NULL};

  usuario_t *u1 = crear_usuario(1, 0); // nivel 0
  usuario_t *u2 = crear_usuario(2, 1); // nivel 1

  producto_t *item1 =
      crear_item(u1, "hogar", "silla", 1, 150, 5); // nivel 0, nuevo -> NO
  producto_t *item2 =
      crear_item(u2, "ropa", "pantalon", 0, 80, 10); // nivel 1, usado -> NO

  producto_t *item1copy =
      crear_item(u1, "hogar", "silla", 1, 150, 5); // nivel 0, nuevo -> NO
  producto_t *item2copy =
      crear_item(u2, "ropa", "pantalon", 0, 80, 10); // nivel 1, usado -> NO

  agregar_nodo(&catalogo4, crear_nodo(item1));
  agregar_nodo(&catalogo4, crear_nodo(item2));

  producto_t *result4 =
      TEST_CALL_S(filtrarPublicacionesNuevasDeUsuariosVerificados, &catalogo4);
  int count4 = contar_items_resultado(result4);

  TEST_ASSERT_EQUALS(int, 0, count4);

  TEST_ASSERT(verificarIntegridad(item1, item1copy));
  TEST_ASSERT(verificarIntegridad(item2, item2copy));

  if (result4)
    free(result4);

  free(item1copy);
  free(item2copy);

  free(item1);
  free(item2);

  free(u1);
  free(u2);
  liberar_lista(&catalogo4);
}

TEST(test_ej1_todos) {
  catalogo_t catalogo5 = {NULL};

  usuario_t *u1 = crear_usuario(1, 1); // nivel 1
  usuario_t *u2 = crear_usuario(2, 2); // nivel 2

  producto_t *item1 =
      crear_item(u1, "electro", "laptop", 1, 1500, 100); // nivel 1, nuevo -> SÍ
  producto_t *item2 = crear_item(u2, "hogar", "heladera", 1, 2000,
                                 1000); // nivel 2, nuevo -> SÍ

  producto_t *item1copy =
      crear_item(u1, "electro", "laptop", 1, 1500, 100); // nivel 1, nuevo -> SÍ
  producto_t *item2copy = crear_item(u2, "hogar", "heladera", 1, 2000,
                                     1000); // nivel 2, nuevo -> SÍ
  //
  agregar_nodo(&catalogo5, crear_nodo(item1));
  agregar_nodo(&catalogo5, crear_nodo(item2));

  producto_t *result5 =
      TEST_CALL_S(filtrarPublicacionesNuevasDeUsuariosVerificados, &catalogo5);
  int count5 = contar_items_resultado(result5);

  TEST_ASSERT_EQUALS(int, 2, count5);
  TEST_ASSERT(item_en_resultado(result5, item1));
  TEST_ASSERT(item_en_resultado(result5, item2));

  TEST_ASSERT(verificarIntegridad(item1, item1copy));
  TEST_ASSERT(verificarIntegridad(item1, item1copy));

  if (result5)
    free(result5);

  free(item1);
  free(item2);

  free(u1);
  free(u2);

  free(item1copy);
  free(item2copy);
  liberar_lista(&catalogo5);
}

TEST(test_ej1_usuario_verificado_multiples_productos_nuevos) {
  catalogo_t catalogo = {NULL};
  usuario_t *u1 = crear_usuario(1, 1); // nivel 1 - verificado

  producto_t *item1 = crear_item(u1, "hogar", "silla", 1, 150, 5); // nuevo
  producto_t *item2 = crear_item(u1, "hogar", "mesa", 1, 300, 3);  // nuevo
  producto_t *item3 =
      crear_item(u1, "electronica", "laptop", 1, 800, 1); // nuevo

  producto_t *item1copy = crear_item(u1, "hogar", "silla", 1, 150, 5);
  producto_t *item2copy = crear_item(u1, "hogar", "mesa", 1, 300, 3);
  producto_t *item3copy = crear_item(u1, "electronica", "laptop", 1, 800, 1);

  agregar_nodo(&catalogo, crear_nodo(item1));
  agregar_nodo(&catalogo, crear_nodo(item2));
  agregar_nodo(&catalogo, crear_nodo(item3));

  producto_t *result =
      TEST_CALL_S(filtrarPublicacionesNuevasDeUsuariosVerificados, &catalogo);
  int count = contar_items_resultado(result);

  TEST_ASSERT_EQUALS(int, 3, count); // Todos los productos deben aparecer
  TEST_ASSERT(verificarIntegridad(item1, item1copy));
  TEST_ASSERT(verificarIntegridad(item2, item2copy));
  TEST_ASSERT(verificarIntegridad(item3, item3copy));

  if (result)
    free(result);

  free(item1copy);
  free(item2copy);
  free(item3copy);
  free(item1);
  free(item2);
  free(item3);
  free(u1);
  liberar_lista(&catalogo);
}

TEST(test_ej1_multiples_usuarios_multiples_productos) {
  catalogo_t catalogo = {NULL};
  usuario_t *u1 = crear_usuario(1, 1); // verificado
  usuario_t *u2 = crear_usuario(2, 0); // no verificado
  usuario_t *u3 = crear_usuario(3, 1); // verificado

  producto_t *item1 =
      crear_item(u1, "hogar", "silla", 1, 150, 5);                // nuevo -> SÍ
  producto_t *item2 = crear_item(u1, "hogar", "mesa", 0, 200, 3); // usado -> NO

  producto_t *item3 = crear_item(u2, "ropa", "camisa", 1, 50,
                                 10); // nuevo pero usuario no verificado -> NO
  producto_t *item4 = crear_item(u2, "ropa", "pantalon", 1, 80,
                                 5); // nuevo pero usuario no verificado -> NO

  producto_t *item5 =
      crear_item(u3, "electronica", "mouse", 1, 25, 15); // nuevo -> SÍ
  producto_t *item6 =
      crear_item(u3, "electronica", "teclado", 1, 40, 8); // nuevo -> SÍ

  producto_t *item1copy = crear_item(u1, "hogar", "silla", 1, 150, 5);
  producto_t *item2copy = crear_item(u1, "hogar", "mesa", 0, 200, 3);
  producto_t *item3copy = crear_item(u2, "ropa", "camisa", 1, 50, 10);
  producto_t *item4copy = crear_item(u2, "ropa", "pantalon", 1, 80, 5);
  producto_t *item5copy = crear_item(u3, "electronica", "mouse", 1, 25, 15);
  producto_t *item6copy = crear_item(u3, "electronica", "teclado", 1, 40, 8);

  agregar_nodo(&catalogo, crear_nodo(item1));
  agregar_nodo(&catalogo, crear_nodo(item2));
  agregar_nodo(&catalogo, crear_nodo(item3));
  agregar_nodo(&catalogo, crear_nodo(item4));
  agregar_nodo(&catalogo, crear_nodo(item5));
  agregar_nodo(&catalogo, crear_nodo(item6));

  producto_t *result =
      TEST_CALL_S(filtrarPublicacionesNuevasDeUsuariosVerificados, &catalogo);
  int count = contar_items_resultado(result);

  TEST_ASSERT_EQUALS(int, 3, count); // item1, item5, item6
  TEST_ASSERT(verificarIntegridad(item1, item1copy));
  TEST_ASSERT(verificarIntegridad(item2, item2copy));
  TEST_ASSERT(verificarIntegridad(item3, item3copy));
  TEST_ASSERT(verificarIntegridad(item4, item4copy));
  TEST_ASSERT(verificarIntegridad(item5, item5copy));
  TEST_ASSERT(verificarIntegridad(item6, item6copy));

  if (result)
    free(result);

  free(item1copy);
  free(item2copy);
  free(item3copy);
  free(item4copy);
  free(item5copy);
  free(item6copy);

  free(item1);
  free(item2);
  free(item3);
  free(item4);
  free(item5);
  free(item6);

  free(u1);
  free(u2);
  free(u3);
  liberar_lista(&catalogo);
}

int main(int argc, char *argv[]) {
  printf("Corriendo los tests del ejercicio 1...\n");
  test_ej1_lista_vacia();
  test_ej1_niveles();
  test_ej1_estados();
  test_ej1_ninguno();
  test_ej1_todos();
  test_ej1_multiples_usuarios_multiples_productos();
  test_ej1_usuario_verificado_multiples_productos_nuevos();

  tests_end("Ejercicio 1");
}
