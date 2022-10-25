#include "rbtree.h"

#include <stdlib.h>


rbtree *new_rbtree(void) {
  /*
  1. tree t 메모리 할당
  2. nil node 메모리 할당
  3. nil.color = black
  4. t.nil = nil node
  5. t.root = nil node 
  */
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t)); // nil node 생성
  nil_node->color = RBTREE_BLACK;
  t -> nil = nil_node;  // t.nil = nil
  t -> root = nil_node; // t.root = nil 로 초기화
  return t; // tree t 반환
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
