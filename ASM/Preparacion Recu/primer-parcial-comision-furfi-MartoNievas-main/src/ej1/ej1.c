#include "../ejs.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// funcion auxiliar
void nueva_publicacion(tuit_t *tuit, feed_t *feed) {
  if (tuit == NULL || feed == NULL)
    return;

  publicacion_t *pub = malloc(sizeof(publicacion_t));
  if (pub == NULL)
    return;

  pub->value = tuit;
  pub->next = feed->first; // El nuevo nodo apunta al antiguo primero
  feed->first = pub;       // El feed ahora empieza con el nuevo nodo
}

// Función principal: publicar un tuit
tuit_t *publicar(char *mensaje, usuario_t *user) {
  if (mensaje == NULL || user == NULL)
    return NULL;
  tuit_t *new_tuit = malloc(sizeof(tuit_t));

  new_tuit->id_autor = user->id;
  new_tuit->retuits = 0;
  new_tuit->favoritos = 0;
  strncpy(new_tuit->mensaje, mensaje, 139);

  // Una vez que tenemos el nuevo tuit actualizamos el feed del usuario
  nueva_publicacion(new_tuit, user->feed);

  for (uint32_t i = 0; i < user->cantSeguidores; i++) {
    usuario_t *seguidor = user->seguidores[i];

    nueva_publicacion(new_tuit, seguidor->feed);
  }
  return new_tuit;
}
