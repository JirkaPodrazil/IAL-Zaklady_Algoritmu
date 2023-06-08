/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  //kontrola klice. Prvek ve stromu neni
  if (tree == NULL){
    return false;
  }

  //klic hledaneho uzlu se schoduje s korenem (pod)stromu
  if (key == tree->key){
    *value = tree->value;
    return true;
  }
  //klic hledaneho uzlu je mensi nez korene (pod)stromu
  //dal se hleda v levem podstromu
  else if (key < tree->key){
    return bst_search(tree->left, key, value);
  }
  //klic hledaneho uzlu je vetsi nez korene (pod)stromu
  //dal se hleda v pravem podstromu
  else {
    return bst_search(tree->right, key, value);
  }
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

  //strom je prazdny
  //prvek se vlozi jako koren (pod)stromu
  if (((*tree) == NULL)) 
  {    
    if (!((*tree) = (bst_node_t*) malloc(sizeof(bst_node_t)))){
      return;   //kontrola allokace
    }
    //vlozeni dat do noveho prvku
    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
  }

  //klic ve strome uz existuje
  //pouze se zmeni hodnota prvku
  if ((*tree)->key == key){
    (*tree)->value = value;
    return;
  }

  //novy prvek ma mensi koren, nez aktualni (pod)strom
  //prvek se vlozi do leveho podstromu 
  if ((*tree)->key > key){
    bst_insert(&((*tree)->left), key, value);
      return;
  }  

  //novy prvek ma vetsi koren, nez aktualni (pod)strom
  //prvek se vlozi do praveho podstomu 
  if ((*tree)->key < key){
    bst_insert(&((*tree)->right), key, value);
      return;
  }

  return;

}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

  int rightest_value;
  char rightest_key;

  
  if (target != NULL && *tree != NULL){   //kontrola validniho vstupu tree a target
    if ((*tree)->right != NULL){
      bst_replace_by_rightmost(target, &(*tree)->right);    //pristup k nejpravejsimu uzlu
    }
    else{
      //pomocne ulozeni dat a klice pro predani navaznosti
      rightest_key = (*tree)->key;
      rightest_value = (*tree)->value;

      //smazani nejpravejsi uzlu
      bst_delete(&(*tree), rightest_key);

      //predani hodnot z nejpravejsiho uzlu
      target->key = rightest_key;
      target->value = rightest_value;
    }
  }
  return;
    
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {


  //pomocny prvek pro predani dedicnosti
  bst_node_t *son_ptr;

  //strom je prazdny. Nic se nedeje
  if (*tree == NULL){
    return;
  }
  
  //strom neni prazdny
  //najde uzel s danym klicem

  //klic je mensi nez koren (pod)stromu
  //dal se hlada v levem podstromu
  if (key < (*tree)->key){
    bst_delete(&(*tree)->left, key);
  }
  //klic je vetsi nez koren
  //dal se hleda v pravem podstromu
  else if (key > (*tree)->key){
    bst_delete(&(*tree)->right, key);
  }
  //v jinem pripade se koreny rovnaji
  //koren je nalezen
  //mazani prvku
  else{

    //nalezeny uzel nema zadny podstrom
    //uzel je smazan a nic se nepredava
    if ((*tree)->right == NULL && (*tree)->left == NULL){
      free(*tree);
      *tree = NULL;
    }

    //nalezeny uzel ma levy podstrom a nema pravy
    //otec zdedi levy podstrom nalezeneho uzlu
    else if ((*tree)->left != NULL && (*tree)->right == NULL){
      son_ptr = (*tree)->left;
      free(*tree);
      *tree = son_ptr;
    }

    //nalezeny uzel ma pravy podstrom a nema levy
    //otec zdedi pravy podstrom nalezeneho uzlu
    else if ((*tree)->right != NULL && (*tree)->left == NULL){
      son_ptr = (*tree)->right;
      free(*tree);
      *tree = son_ptr;
    }

    //nalezeny uzel ma dva podstromy
    //otec zdedi nejpravejsi uzel leveho podstromu
    else{
      //levy podstrom nema pravy podstrom
      //hleda se nejpravejsi uzel LEVEHO podstromu
      if ((*tree)->left->right == NULL){
        bst_replace_by_rightmost(*tree, &(*tree)->left);
      }
      //levy podstrom ma pravy podstrom
      //hlede se v nem
      else{
        bst_replace_by_rightmost(*tree, &(*tree)->left->right);
      }
    }

  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {

  if (*tree == NULL){
    return;
  }
  bst_dispose(&((*tree)->left));
  bst_dispose(&((*tree)->right));
  free(*tree);
  *tree = NULL; 

}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {

  //stromem se prochazi Root, Left, Right
  if (tree != NULL){
    bst_print_node(tree);
    bst_preorder(tree->left);
    bst_preorder(tree->right);
    return;
  }
  
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {

  //stromem se prochazi  Left, Root, Right
  if (tree != NULL){
    bst_inorder(tree->left);
    bst_print_node(tree);
    bst_inorder(tree->right);
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {

//stromem se prochazi Left, Right, Root
if (tree != NULL){
    bst_inorder(tree->left);
    bst_inorder(tree->right);
    bst_print_node(tree);    
  }
}
