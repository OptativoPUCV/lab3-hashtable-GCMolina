#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"
#include <stdbool.h>


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    long posicion= hash(key , map->capacity);
    Pair* aux0= map->buckets[posicion];
    while(true){
    if((aux0==NULL) || (aux0 -> key==NULL)){
      Pair* data = createPair(key, value);
      map->buckets[posicion]=data;
      map->current = posicion;
      map->size++;
      return;
      
    }
      posicion=(posicion+1)%map->capacity;
      aux0 = map->buckets[posicion];
  }
  
  

  map->current = posicion;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair** old_buckets = map->buckets;
    map->capacity = map->capacity * 2;
    map->buckets=(Pair**)malloc(sizeof(Pair*)*map->capacity);
    map->size=0;
    for(long i=0; i<map->capacity/2;i++){
      if(old_buckets[i]!=NULL&&old_buckets[i]->key!=NULL){
        insertMap(map,old_buckets[i]->key,old_buckets[i]->value);
        
      }
    }

}


HashMap * createMap(long capacity) {
  HashMap * map = (HashMap*)malloc(sizeof(HashMap));
  map->buckets=(Pair**)malloc(sizeof(Pair*)*capacity);
  map->size=0;
  map->capacity=capacity;
  map->current=-1;
  return map;
}

void eraseMap(HashMap * map,  char * key) {
  if(map==NULL || key == NULL){
    return;
  }
  long posicion = hash(key , map->capacity);
  while(map->buckets[posicion]!= NULL){
    Pair* aux = map->buckets[posicion];
    if(is_equal(aux->key,key)){
      aux->key=NULL;
      map->size--;
      return;
    }
    posicion=(posicion+1)%map->capacity;
    if(posicion==map->current){
      break;
    }
  }
  
}

Pair * searchMap(HashMap * map,  char * key) {   
  if(map == NULL || key==NULL){
    return NULL;
  }
  long posicion = hash(key,map->capacity);

  while(map->buckets[posicion]!=NULL){
    Pair* aux= map->buckets[posicion];
    if(is_equal(aux->key,key)){
      map->current=posicion;
      return aux;
    }
    posicion=(posicion+1)%map->capacity;
    if(posicion==map->current){
      break;
    }
    
  }

  return NULL;
}

Pair * firstMap(HashMap * map) {
  for(long i=0;i<map->capacity;i++){
    if(map->buckets[i]!=NULL && map->buckets[i]->key!=NULL){
      map->current=i;
      return map->buckets[i];
    }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) {
  for(long i=map->current+1;i<map->capacity;i++){
    if(map->buckets[i]!=NULL&&map->buckets[i]->key!=NULL){
      map->current=i;
      return map->buckets[i];
    }
  }
    return NULL;
}
