#include "utils.h"
#include "ejs.h"
#include <stdint.h>
#include <string.h>

bool sonUsuariosIguales(usuario_t *u1, usuario_t *u2) {
  return u1->id == u2->id && u1->nivel == u2->nivel;
}

uint8_t seleccionDePrivilegio(uint32_t idUsuario) {
  return (idUsuario * idUsuario) % 3;
}

uint8_t comparador(producto_t *item) {
  if (item->precio > 1000 && !strcmp(item->categoria, "electro")) {
    return 0;
  } else if (!strcmp(item->categoria, "electro")) {
    return 1;
  }

  return 2;
}

usuario_t *crear_usuario(uint32_t id, uint8_t nivel) {
  usuario_t *usuario = (usuario_t *)malloc(sizeof(usuario_t));
  if (usuario == NULL)
    return NULL;

  usuario->id = id;
  usuario->nivel = nivel;

  return usuario;
}

bool verificarIntegridad(producto_t *referencia, producto_t *original) {
  return referencia->id == original->id &&
         referencia->estado == original->estado &&
         referencia->precio == original->precio &&
         !strcmp(referencia->nombre, original->nombre) &&
         !strcmp(referencia->categoria, original->categoria) &&
         referencia->usuario->id == original->usuario->id &&
         referencia->usuario->nivel == original->usuario->nivel;
};

producto_t *crear_item(usuario_t *usuario, char *categoria, char *nombre,
                       uint16_t estado, uint32_t precio, uint32_t id_producto) {
  producto_t *item = (producto_t *)malloc(sizeof(producto_t));
  if (item == NULL)
    return NULL;

  item->usuario = usuario;
  item->id = id_producto;
  strcpy(item->categoria, categoria);
  strcpy(item->nombre, nombre);
  item->estado = estado;
  item->precio = precio;

  return item;
}

publicacion_t *crear_nodo(producto_t *item) {
  publicacion_t *nodo = (publicacion_t *)malloc(sizeof(publicacion_t));
  if (nodo == NULL)
    return NULL;

  nodo->value = item;
  nodo->next = NULL;

  return nodo;
}

void agregar_nodo(catalogo_t *catalogo, publicacion_t *nodo) {
  if (catalogo == NULL || nodo == NULL)
    return;

  if (catalogo->first == NULL) {
    catalogo->first = nodo;
  } else {
    publicacion_t *current = catalogo->first;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = nodo;
  }
}

void liberar_usuario(usuario_t *usuario) {
  if (usuario != NULL) {
    free(usuario);
  }
}

void liberar_item(producto_t *item) {
  if (item == NULL)
    return;
  free(item);
}

void liberar_lista(catalogo_t *catalogo) {
  if (catalogo->first == NULL) {
    return;
  }
  publicacion_t *current = catalogo->first;
  while (current != NULL) {
    publicacion_t *next = current->next;
    free(current);
    current = next;
  }

  catalogo->first = NULL;
}

int contar_items_resultado(producto_t *resultado) {
  if (resultado == NULL)
    return 0;

  producto_t **array = (producto_t **)resultado;
  int count = 0;
  while (array[count] != NULL) {
    count++;
  }
  return count;
}

bool item_en_resultado(producto_t *resultado, producto_t *item_buscado) {
  if (resultado == NULL || item_buscado == NULL)
    return false;

  producto_t **array = (producto_t **)resultado;
  int i = 0;
  while (array[i] != NULL) {
    if (array[i] == item_buscado) {
      return true;
    }
    i++;
  }
  return false;
}

int contar_nodos(catalogo_t *catalogo) {
  if (catalogo == NULL || catalogo->first == NULL)
    return 0;

  int count = 0;
  publicacion_t *current = catalogo->first;
  while (current != NULL) {
    count++;
    current = current->next;
  }
  return count;
}

bool item_en_lista(producto_t **res, uint32_t n, producto_t *item_buscado) {
  for (uint32_t i = 0; i < n; i++) {
    if (res[i] == item_buscado) {
      return true;
    }
  }
  return false;
}

bool tiene_duplicados(catalogo_t *catalogo) {
  if (catalogo == NULL || catalogo->first == NULL)
    return false;

  publicacion_t *current1 = catalogo->first;
  while (current1 != NULL) {
    publicacion_t *current2 = current1->next;
    while (current2 != NULL) {
      if (current1->value != NULL && current2->value != NULL &&
          current1->value->usuario != NULL &&
          current1->value->usuario->id == current2->value->usuario->id &&
          strcmp(current1->value->nombre, current2->value->nombre) == 0) {
        return true;
      }
      current2 = current2->next;
    }
    current1 = current1->next;
  }
  return false;
}

int contar_items_array(producto_t **array) {
  if (array == NULL)
    return 0;
  int count = 0;
  while (array[count] != NULL) {
    count++;
  }
  return count;
}

bool item_en_array(producto_t **array, producto_t *item_buscado) {
  if (array == NULL || item_buscado == NULL)
    return false;
  int i = 0;
  while (array[i] != NULL) {
    if (array[i] == item_buscado) {
      return true;
    }
    i++;
  }
  return false;
}
void liberar_catalogo(catalogo_t *catalogo) {
  publicacion_t *f = catalogo->first;
  publicacion_t *g = f;

  while (f != NULL) {
    f = f->next;
    free(g);
    g = f;
  }
}
