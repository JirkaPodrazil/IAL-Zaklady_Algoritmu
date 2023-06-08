/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  //nastavi vsechny prvky prvky table na NULL
  for (int i = 0; i < HT_SIZE; i++){
    (*table)[i] = NULL;
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  //prvek v table dany klicem
  ht_item_t *item = (*table)[get_hash(key)];

  //kazdy klic prvku v table je zkontrolovan, jestli neopovida hledanemu klici
  while (item){
    if(!strcmp(item->key, key)){
      return item;
    }
    item = item->next;
  }

  //nebyl nalezen schodny klic
  return NULL;
  
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  //vyhleda prvek v table s danym klicem
  ht_item_t *item = ht_search(table, key);
  
  //prvek jiz existoval. Pouze se aktualizuje jeho hodnota
  if(item){
    item->value = value;
    return;
  }

  //prvek neexistoval. Vytvoreni noveho prvku v tabulce
  ht_item_t *insert = (ht_item_t *) malloc(sizeof(ht_item_t));
  //kontrola spravne allokace pameti
  if(!insert){
    return;
  }
  //prideleni dat novemu prvku
  insert->key = key;
  insert->value = value;
  insert->next = NULL;

  //kontrola zda existuje prvek se synonymnik klicem
  int hash = get_hash(key);   //ziska hash z klice
  if ((item = (*table)[hash])){
    insert->next = item;
  }
  //ulozi novy prvek na zacatek seznamu
  (*table)[hash] = insert; 

  }


/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ////vyhleda prvek v table s danym klicem
  ht_item_t *item = ht_search(table, key);
  return item ? (&item->value) : NULL;
  
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  ht_item_t **hash;
  ht_item_t *to_delete;   //pomocny prvek pro predani navaznosti

  hash = &(*table)[get_hash(key)];  //adresa prvku s danym klicem
  
  if (*(hash) == NULL){   //adresa nebyla nalezena
    return;
  }  
  else{   //adresa byla nalezena

    to_delete = *hash;
    while(strcmp((*hash)->key, key)){   //proces mazani se opakuje pro kazky prvek, ktery ma dany klic
      hash = &(*hash)->next;
      to_delete = *hash;
    }
    *hash = (*hash)->next;    //predani navaznosti
    free(to_delete);    //uvolneni prvku
  }

}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {

  ht_item_t *item;
  ht_item_t *to_delete;   //pomocny prvek pro predani navaznosti

  //projde celou table a smaze kazdy prvek v ni
  for (int i = 0; i < HT_SIZE; i++){
    item = (*table)[i];
    while (item){
      //predani navaznosti a uvolneni pameti prvku
      to_delete = item;
      item = item->next;
      free(to_delete);
    }
    //smazani ukazalete na dany prvek
    (*table)[i] = NULL;
  }
}
