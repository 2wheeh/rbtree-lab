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
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t)); // nil node 생성
  nil_node->color = RBTREE_BLACK;
  t -> nil = nil_node;  // t.nil = nil
  t -> root = nil_node; // t.root = nil 로 초기화
  return t; // tree t 반환
}

void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right; // y를 설정한다
  y->right = y->left; // y의 왼쪽 서브 트리를 x의 오른쪽 서브 트리로 옮긴다
  if(y->left == t->nil){
      y->left->parent = x;
  }
  y->parent = x->parent;  // x의 부모를 y로 연결한다
  if(x->parent == t->nil) t->root = y;
  else if(x == x->parent->left) x->parent->left = y;
  else x->parent->right = y;

  y->left = x;  // x를 y의 왼쪽자식으로 두자
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left; // y를 설정한다
  y->left = y->right; // y의 왼쪽 서브 트리를 x의 오른쪽 서브 트리로 옮긴다
  if(y->right == t->nil){
      y->right->parent = x;
  }
  y->parent = x->parent;  // x의 부모를 y로 연결한다
  if(x->parent == t->nil) t->root = y;
  else if(x == x->parent->right) x->parent->right = y;
  else x->parent->left = y;

  y->right = x;  // x를 y의 왼쪽자식으로 두자
  x->parent = y;
}

void insert_fixup(rbtree *t, node_t *z)
{
  node_t *y;
  while(z->parent->color == RBTREE_RED){ // 특성4. 위반 check
    if(z->parent == z->parent->parent->left)
    {
      y = z->parent->parent->right;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else{
        if(z == z->parent->right){
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else{
      y = z->parent->parent->left;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else{
        if(z == z->parent->left){
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}


node_t *insert(rbtree *t, node_t *z)
{ 
  key_t key = z->key;
  node_t *y = t->nil; // x가 nil되면 걔 부모를 알고있어야 z가 들어갈 위치를 아는 것임
  node_t *x = t->root;  // 처음 비교를 root 부터 비교해가며 내려갈 것임
  
  while(x != t->nil){
    y = x;              // x가 내력가기전에 y에 저장해주자 왜냐면 x 내려가면 지금 x가 x.p 위치잖아
    if(z->key < x->key) // 더 작으면 왼쪽 서브트리로 탐색 하자 ~
      x = x->left; 
    else                // 같거나 크면 오른쪽 서브트리겠지 ~
      x = x->right;
  }
  z->parent = y;    // z 부모를 초기화 ~ (전혀 안내려간다면 root 엄마가 nil ㅋ)
  
  if(y == t->nil)   // 그러면 z는 root 겟네
    t->root = z;

  else if(z->key < y->key)  // 부모될 애랑 비교해서 왼쪽 자식할지 오른쪽 자식할지
    y->left = z;

  else y->right = z;
  z->left = t->nil;   // sentinel 노드 새로 추가된 노드의 자식에 달아주기
  z->right = t->nil;
  z->color = RBTREE_RED;  // 새로 추가된 z 는 RED 

  insert_fixup(t, z);

  return z;
}

void delete_node(node_t *node, node_t *nil_node) {
  if(node != nil_node){ // nil 은 하나니까 계속 지울수도 없고 할필요도 없어
    delete_node(node->left, nil_node);    
    delete_node(node->right, nil_node);    
    free(node); // 혹시 새로 추가된 애들이 섞여있기때문에 걍 노드단위로 해제해줌
    node = NULL;
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t->root, t->nil);
  free(t->nil); // free node_t *nil_node 
  free(t);  // free rbtree *t
  t = NULL; // t 는 정적 할당 된 포인터변수 이기 때문에 직접 없애줌
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *z = (node_t *)calloc(1, sizeof(node_t)); // z node 생성
  z->key = key;
  z = insert(t, z);

  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *z = t->root; // root 로 초기화 하고 찾을 것임

  while(z != t->nil){
    if(z->key == key) return z;
    else if(z->key < key) z = z->right;
    else z = z->left;
  }

  return NULL;

}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *z = t->root;
  node_t *min_node = z;

  while(z != t->nil)
  {
    min_node = z;
    z = z->left;
  }

  return min_node;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *z = t->root;
  node_t *max_node = z;

  while(z != t->nil)
  {
    max_node = z;
    z = z->right;
  }

  return max_node;
}

void transplant(rbtree *t, node_t *u, node_t *v){ // v가 u의 자리로
  if(u->parent == t->nil) t->root = v;
  else if(u == u->parent->left) u->parent->left = v;
  else u->parent->right;

  v->parent = u->parent;
}

void erase_fixup(rbtree *t, node_t *x)
{ 
  node_t *w; // sibling

  while(x != t->root && x->color == RBTREE_BLACK)
  {
    if(x == x->parent->left)
    {
      w = x->parent->right;
      
      if(w->color == RBTREE_RED){ // Case 1: Case2 or (Case3 +) Case4 를 위한 준비
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }

      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){ // Case 2: w이 둘다 blk -> w만 빨강으로 바꿔서 해결 시도
        w->color = RBTREE_RED;
        x = x->parent;  // 다음 while에서 검사할 x cursor 이동
      }
      else{ // (Case3 +) Case4
        if(w->right->color == RBTREE_BLACK){ // Case3
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        // Case4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;  // 다음 반복문 안돌고 탈출 해버리기 위한 설정
        }
    }
    else // 위의 경우와 좌 <-> 우 만 바꿈
    {
      w = x->parent->left;
      
      if(w->color == RBTREE_RED){ // Case 1: Case2 or (Case3 +) Case4 를 위한 준비
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){ // Case 2: w이 둘다 blk -> w만 빨강으로 바꿔서 해결 시도
        w->color = RBTREE_RED;
        x = x->parent;  // 다음 while에서 검사할 x cursor 이동
      }
      else{ // (Case3 +) Case4
        if(w->left->color == RBTREE_BLACK){ // Case3
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        // Case4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;  // 다음 반복문 안돌고 탈출 해버리기 위한 설정
        }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *x, *y = z;
  color_t y_original_color = y->color;
  
  if(z->left == t->nil) // 자식이 없거나 오른쪽에 한명
  {
    x = z->right;
    transplant(t, z, z->right);
  }
  else if(z->right == t->nil) // 자식이 왼쪽에 한명
  {
    x = z->left;
    transplant(t, z, z->left);
  }
  else  // 자식이 2명
  {
    y = rbtree_min(z->right);
    y_original_color = y->color;
    x = y->right;
    if(y->parent == z) x->parent = y; // x가 t.nil일 경우 부모 정보 아무렇게 들어있을 것. 업데이트 필요
    else{
      transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }

    transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if(y_original_color == RBTREE_BLACK) erase_fixup(t, x);

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
