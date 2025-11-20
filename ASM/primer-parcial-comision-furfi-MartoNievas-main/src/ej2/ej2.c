#include "../ejs.h"

#include <stdlib.h>
#include <unistd.h>

void borrar_publicaciones(feed_t *feed, usuario_t *userABorrar) {

  publicacion_t *prev = NULL;
  publicacion_t *curr = feed->first;

  while (curr != NULL) {
    if (curr->value->id_autor == userABorrar->id) {
      publicacion_t *to_free = curr;
      curr = curr->next;
      // si el prev es nulo entonces estamos borrando el primero
      if (prev == NULL) {
        feed->first = curr;
      } else {
        // Estamos borrando uno del medio
        prev->next = curr;
      }

      free(to_free);
    } else {
      // No estamos borrando nada
      prev = curr;
      curr = curr->next;
    }
  }
}
void bloquearUsuario(usuario_t *usuarioBloqueador,
                     usuario_t *usuarioABloquear) {

  // Ahora tenemos que agregar al usuario a bloquear al final de la lista
  usuario_t **bloqueados = usuarioBloqueador->bloqueados;
  uint32_t cantBloqueados = usuarioBloqueador->cantBloqueados;

  bloqueados[cantBloqueados] = usuarioABloquear;
  usuarioBloqueador->cantBloqueados++;

  borrar_publicaciones(usuarioBloqueador->feed, usuarioABloquear);
  borrar_publicaciones(usuarioABloquear->feed, usuarioBloqueador);
}