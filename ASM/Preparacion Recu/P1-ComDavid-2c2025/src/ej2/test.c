#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define WITH_ABI_MESSAGE
#include "../ejs.h"
#include "../utils.h"

TEST(test_ej2_lista_vacia) {
  catalogo_t catalogo1 = {NULL};
  catalogo_t *result1 = TEST_CALL_S(removerCopias, &catalogo1);
  int count1 = contar_nodos(result1);

  TEST_ASSERT_EQUALS(int, 0, count1);
  TEST_ASSERT(result1 == &catalogo1);
}

TEST(test_ej2_sin_duplicados) {
  catalogo_t catalogo2 = {NULL};

  usuario_t *u1 = crear_usuario(1, 1);
  usuario_t *u2 = crear_usuario(2, 1);

  producto_t *item1 = crear_item(u1, "electro", "celular", 1, 500, 15);
  producto_t *item2 = crear_item(u2, "hogar", "mesa", 1, 200, 11);

  producto_t *item1copy = crear_item(u1, "electro", "celular", 1, 500, 15);
  producto_t *item2copy = crear_item(u2, "hogar", "mesa", 1, 200, 11);

  agregar_nodo(&catalogo2, crear_nodo(item1));
  agregar_nodo(&catalogo2, crear_nodo(item2));

  int count_inicial = contar_nodos(&catalogo2);
  catalogo_t *result2 = TEST_CALL_S(removerCopias, &catalogo2);
  int count_final = contar_nodos(result2);

  TEST_ASSERT_EQUALS(int, count_inicial, count_final);
  TEST_ASSERT(result2 == &catalogo2);
  TEST_ASSERT(!tiene_duplicados(result2));

  TEST_ASSERT(verificarIntegridad(item1, item1copy));
  TEST_ASSERT(verificarIntegridad(item2, item2copy));

  free(item1copy);
  free(item2copy);

  free(item1);
  free(item2);

  free(u1);
  free(u2);

  liberar_lista(&catalogo2);
}

TEST(test_ej2_duplicados) {
  catalogo_t catalogo3 = {NULL};

  usuario_t *u1 = crear_usuario(1, 1);

  producto_t *item1 = crear_item(u1, "electro", "celular", 1, 500, 1);
  producto_t *item2 = crear_item(u1, "electro", "celular", 0, 600,
                                 2); // mismo nombre y usuario ID

  producto_t *item1copy = crear_item(u1, "electro", "celular", 1, 500, 1);

  producto_t *res[1] = {item1};

  agregar_nodo(&catalogo3, crear_nodo(item1));
  agregar_nodo(&catalogo3, crear_nodo(item2));

  catalogo_t *result3 = TEST_CALL_S(removerCopias, &catalogo3);
  int count3 = contar_nodos(result3);

  TEST_ASSERT_EQUALS(int, 1, count3);
  TEST_ASSERT(item_en_lista(res, 1, result3->first->value)); 
  TEST_ASSERT(!tiene_duplicados(result3));                   

  TEST_ASSERT(verificarIntegridad(item1, item1copy));

  free(item1copy);
  free(result3->first->value);

  liberar_lista(&catalogo3);
  free(u1);
}

TEST(test_ej2_diferente_usuario) {

  catalogo_t catalogo4 = {NULL};

  usuario_t *u1 = crear_usuario(1, 1);
  usuario_t *u2 = crear_usuario(2, 1); // diferente ID

  producto_t *item1 = crear_item(u1, "electro", "celular", 1, 500, 123);
  producto_t *item2 = crear_item(u2, "electro", "celular", 1, 600,
                                 321); // mismo nombre, diferente usuario

  producto_t *item1copy = crear_item(u1, "electro", "celular", 1, 500, 123);
  producto_t *item2copy = crear_item(u2, "electro", "celular", 1, 600,
                                     321); // mismo nombre, diferente usuario

  agregar_nodo(&catalogo4, crear_nodo(item1));
  agregar_nodo(&catalogo4, crear_nodo(item2));

  catalogo_t *result4 = TEST_CALL_S(removerCopias, &catalogo4);
  int count4 = contar_nodos(result4);

  TEST_ASSERT_EQUALS(int, 2, count4);

  TEST_ASSERT(verificarIntegridad(item1, item1copy));
  TEST_ASSERT(verificarIntegridad(item2, item2copy));

  free(item1copy);
  free(item2copy);

  free(item1);
  free(item2);

  free(u1);
  free(u2);

  liberar_lista(&catalogo4);
}

TEST(test_ej2_diferente_nombre) {
  catalogo_t catalogo5 = {NULL};

  usuario_t *u1 = crear_usuario(1, 1);

  producto_t *item1 = crear_item(u1, "electro", "celular", 1, 500, 3);
  producto_t *item2 = crear_item(u1, "electro", "tablet", 1, 600,
                                 5); // diferente nombre, mismo usuario

  producto_t *item1copy = crear_item(u1, "electro", "celular", 1, 500, 3);
  producto_t *item2copy = crear_item(u1, "electro", "tablet", 1, 600,
                                     5); // diferente nombre, mismo usuario

  agregar_nodo(&catalogo5, crear_nodo(item1));
  agregar_nodo(&catalogo5, crear_nodo(item2));

  catalogo_t *result5 = TEST_CALL_S(removerCopias, &catalogo5);
  int count5 = contar_nodos(result5);

  TEST_ASSERT_EQUALS(int, 2, count5);

  TEST_ASSERT(verificarIntegridad(item1, item1copy));
  TEST_ASSERT(verificarIntegridad(item2, item2copy));

  free(item1copy);
  free(item2copy);

  free(item1);
  free(item2);

  free(u1);
  liberar_lista(&catalogo5);
}

TEST(test_ej2_multiples) {
  catalogo_t catalogo6 = {NULL};

  usuario_t *u1 = crear_usuario(1, 1);
  usuario_t *u2 = crear_usuario(2, 1);

  producto_t *item1 = crear_item(u1, "electro", "celular", 1, 500, 1);
  producto_t *item2 =
      crear_item(u1, "electro", "celular", 0, 600, 2); // duplicado
  producto_t *item3 =
      crear_item(u1, "electro", "celular", 1, 700, 3);            // duplicado
  producto_t *item4 = crear_item(u2, "hogar", "mesa", 1, 200, 4); // diferente

  producto_t *res[3] = {item1, item3, item4};

  producto_t *item1copy = crear_item(u1, "electro", "celular", 1, 500, 1);
  producto_t *item4copy =
      crear_item(u2, "hogar", "mesa", 1, 200, 4); // diferente

  agregar_nodo(&catalogo6, crear_nodo(item1));
  agregar_nodo(&catalogo6, crear_nodo(item2));
  agregar_nodo(&catalogo6, crear_nodo(item3));
  agregar_nodo(&catalogo6, crear_nodo(item4));

  catalogo_t *result6 = TEST_CALL_S(removerCopias, &catalogo6);
  int count6 = contar_nodos(result6);

  TEST_ASSERT_EQUALS(int, 2, count6);
  TEST_ASSERT(item_en_lista(res, 3, result6->first->value));
  TEST_ASSERT(item_en_lista(res, 3, result6->first->next->value));
  TEST_ASSERT(!tiene_duplicados(result6));

  TEST_ASSERT(verificarIntegridad(result6->first->value, item1copy));
  TEST_ASSERT(verificarIntegridad(result6->first->next->value, item4copy));

  free(item1copy);
  free(item4copy);

  free(result6->first->value);
  free(result6->first->next->value);

  free(u1);
  free(u2);
  liberar_lista(&catalogo6);
}

TEST(test_ej2_usuario_multiples_items_sin_duplicados) {
  catalogo_t catalogo = {NULL};

  usuario_t *u1 = crear_usuario(1, 1);
  usuario_t *u2 = crear_usuario(2, 0);

  producto_t *item1 = crear_item(u1, "electro", "celular", 1, 500, 10);
  producto_t *item2 = crear_item(u1, "electro", "tablet", 1, 300, 5);
  producto_t *item3 = crear_item(u1, "hogar", "mesa", 0, 150, 3);
  producto_t *item4 = crear_item(u2, "ropa", "camisa", 1, 50, 20);
  producto_t *item5 = crear_item(u2, "ropa", "pantalon", 0, 80, 15);

  producto_t *item1copy = crear_item(u1, "electro", "celular", 1, 500, 10);
  producto_t *item2copy = crear_item(u1, "electro", "tablet", 1, 300, 5);
  producto_t *item3copy = crear_item(u1, "hogar", "mesa", 0, 150, 3);
  producto_t *item4copy = crear_item(u2, "ropa", "camisa", 1, 50, 20);
  producto_t *item5copy = crear_item(u2, "ropa", "pantalon", 0, 80, 15);

  agregar_nodo(&catalogo, crear_nodo(item1));
  agregar_nodo(&catalogo, crear_nodo(item2));
  agregar_nodo(&catalogo, crear_nodo(item3));
  agregar_nodo(&catalogo, crear_nodo(item4));
  agregar_nodo(&catalogo, crear_nodo(item5));

  catalogo_t *result = TEST_CALL_S(removerCopias, &catalogo);
  int count = contar_nodos(result);

  TEST_ASSERT_EQUALS(int, 5, count); // Todos los items deben permanecer
  TEST_ASSERT(!tiene_duplicados(result));

  TEST_ASSERT(verificarIntegridad(item1, item1copy));
  TEST_ASSERT(verificarIntegridad(item2, item2copy));
  TEST_ASSERT(verificarIntegridad(item3, item3copy));
  TEST_ASSERT(verificarIntegridad(item4, item4copy));
  TEST_ASSERT(verificarIntegridad(item5, item5copy));

  free(item1copy);
  free(item2copy);
  free(item3copy);
  free(item4copy);
  free(item5copy);

  free(item1);
  free(item2);
  free(item3);
  free(item4);
  free(item5);

  free(u1);
  free(u2);
  liberar_lista(&catalogo);
}

TEST(test_ej2_usuario_multiples_items_con_duplicados) {
  catalogo_t catalogo = {NULL};

  usuario_t *u1 = crear_usuario(1, 1);

  producto_t *item1 = crear_item(u1, "electro", "celular", 1, 500, 10);
  producto_t *item2 = crear_item(u1, "electro", "tablet", 1, 300, 5);
  producto_t *item3 =
      crear_item(u1, "electro", "celular", 0, 450, 8); // duplicado con item1
  producto_t *item4 = crear_item(u1, "hogar", "mesa", 1, 150, 3);
  producto_t *item5 =
      crear_item(u1, "electro", "tablet", 1, 280, 7); // duplicado con item2

  producto_t *item1copy = crear_item(u1, "electro", "celular", 1, 500, 10);
  producto_t *item2copy = crear_item(u1, "electro", "tablet", 1, 300, 5);
  producto_t *item4copy = crear_item(u1, "hogar", "mesa", 1, 150, 3);

  producto_t *res[5] = {item1, item2, item3, item4, item5};

  agregar_nodo(&catalogo, crear_nodo(item1));
  agregar_nodo(&catalogo, crear_nodo(item2));
  agregar_nodo(&catalogo, crear_nodo(item3));
  agregar_nodo(&catalogo, crear_nodo(item4));
  agregar_nodo(&catalogo, crear_nodo(item5));

  catalogo_t *result = TEST_CALL_S(removerCopias, &catalogo);
  int count = contar_nodos(result);

  TEST_ASSERT_EQUALS(int, 3, count); // item1, item2, item4 deben permanecer
  TEST_ASSERT(item_en_lista(res, 5, result->first->value));
  TEST_ASSERT(item_en_lista(res, 5, result->first->next->value));
  TEST_ASSERT(item_en_lista(res, 5, result->first->next->next->value));
  TEST_ASSERT(!tiene_duplicados(result));

  TEST_ASSERT(verificarIntegridad(result->first->value, item1copy));
  TEST_ASSERT(verificarIntegridad(result->first->next->value, item2copy));
  TEST_ASSERT(verificarIntegridad(result->first->next->next->value, item4copy));

  free(item1copy);
  free(item2copy);
  free(item4copy);

  free(item1);
  free(item2);
  free(item4);

  liberar_lista(&catalogo);
  free(u1);
}

TEST(test_ej2_todos_duplicados) {
  catalogo_t catalogo = {NULL};

  usuario_t *u1 = crear_usuario(1, 1);

  producto_t *item1 = crear_item(u1, "electro", "celular", 1, 500, 10);
  producto_t *item2 = crear_item(u1, "electro", "celular", 0, 450, 8);
  producto_t *item3 = crear_item(u1, "electro", "celular", 1, 520, 12);
  producto_t *item4 = crear_item(u1, "electro", "celular", 0, 480, 6);

  producto_t *item1copy = crear_item(u1, "electro", "celular", 1, 500, 10);

  agregar_nodo(&catalogo, crear_nodo(item1));
  agregar_nodo(&catalogo, crear_nodo(item2));
  agregar_nodo(&catalogo, crear_nodo(item3));
  agregar_nodo(&catalogo, crear_nodo(item4));

  catalogo_t *result = TEST_CALL_S(removerCopias, &catalogo);
  int count = contar_nodos(result);

  producto_t *res[4] = {item1, item2, item3, item4};

  TEST_ASSERT_EQUALS(int, 1, count);
  TEST_ASSERT(item_en_lista(res, 4, result->first->value));
  TEST_ASSERT(!tiene_duplicados(result));

  TEST_ASSERT(verificarIntegridad(item1, item1copy));

  free(item1copy);
  free(item1);
  free(u1);

  liberar_lista(&catalogo);
}

int main(int argc, char *argv[]) {
  printf("Corriendo los tests del ejercicio 2...\n");
  test_ej2_lista_vacia();
  test_ej2_sin_duplicados();
  test_ej2_duplicados();
  test_ej2_diferente_usuario();
  test_ej2_diferente_nombre();
  test_ej2_multiples();
  test_ej2_usuario_multiples_items_sin_duplicados();
  test_ej2_usuario_multiples_items_con_duplicados();
  test_ej2_todos_duplicados();

  tests_end("Ejercicio 2");
}
