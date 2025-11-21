#ifndef TEST_H
#define TEST_H

#include "../test_utils/test-utils.h"
#include "ejs.h"

usuario_t *crear_usuario(uint32_t id, uint8_t nivel);
producto_t *crear_item(usuario_t *usuario, char *categoria, char *nombre,
                       uint16_t estado, uint32_t precio, uint32_t id_producto);
publicacion_t *crear_nodo(producto_t *item);
void agregar_nodo(catalogo_t *catalogo, publicacion_t *nodo);
void liberar_lista(catalogo_t *catalogo);
void liberar_item(producto_t *item);
void liberar_usuario(usuario_t *usuario);
int contar_items_resultado(producto_t *resultado);
bool item_en_resultado(producto_t *resultado, producto_t *item_buscado);
int contar_nodos(catalogo_t *catalogo);
bool item_en_lista(producto_t **res, uint32_t n, producto_t *item_buscado);
bool tiene_duplicados(catalogo_t *catalogo);
int contar_items_array(producto_t **array);
bool item_en_array(producto_t **array, producto_t *item_buscado);
uint8_t comparador(producto_t *item);
void liberar_catalogo(catalogo_t *catalogo);
bool verificarIntegridad(producto_t *referencia, producto_t *original);
uint8_t seleccionDePrivilegio(uint32_t idUsuario);
bool sonUsuariosIguales(usuario_t *u1, usuario_t *u2);

#endif
