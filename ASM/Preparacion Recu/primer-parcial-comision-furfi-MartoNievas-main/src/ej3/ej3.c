#include "../ejs.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

// funcion auxiliar para determinar cuantos tuits sobresalientes tiene un
// usuario

uint32_t contar_tuits_sobresalientes(usuario_t *user,
                                     uint8_t (*esTuitSobresaliente)(tuit_t *)) {

  feed_t *feed = user->feed;
  publicacion_t *pub = feed->first;
  uint32_t res = 0;

  while (pub != NULL) {
    if ((*esTuitSobresaliente)(pub->value) &&
        pub->value->id_autor == user->id) {
      res++;
    }
    pub = pub->next;
  }
  return res;
}

tuit_t **trendingTopic(usuario_t *user,
                       uint8_t (*esTuitSobresaliente)(tuit_t *)) {
  if (user == NULL)
    return NULL;

  uint32_t tuits_sobresalientes =
      contar_tuits_sobresalientes(user, esTuitSobresaliente);
  if (tuits_sobresalientes == 0)
    return NULL;

  tuit_t **array_tuits = malloc(sizeof(tuit_t *) * (tuits_sobresalientes + 1));
  array_tuits[tuits_sobresalientes] = NULL;

  feed_t *feed = user->feed;
  publicacion_t *pub = feed->first;
  int i = 0;
  while (pub != NULL) {
    if ((*esTuitSobresaliente)(pub->value) &&
        pub->value->id_autor == user->id) {
      array_tuits[i] = pub->value;
      i++;
    }
    pub = pub->next;
  }
  return array_tuits;
}
