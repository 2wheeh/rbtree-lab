#include "rbtree.h"
#include <stdlib.h>

typedef struct elem{
    node_t *data;
    struct elem *link;
}elem;

typedef struct{
    elem *top;  // stack top을 가리키는 pointer
}stack;

void stk_init(stack *stk)
{
    stk->top = NULL;
}

int stk_is_empty(stack *stk)
{
    return (stk->top == NULL);
}

void stk_push(stack *stk, node_t *item)
{
    elem *tmp = (elem *)malloc(sizeof(elem));   // 동적 elem 생성
    tmp->data = item;
    tmp->link = stk->top;   // 새로운 elem이 기존의 top elem을 가르키도록 함
    stk->top = tmp;
}

node_t *stk_pop(stack *stk)
{
    if(stk_is_empty(stk)){
        return NULL;
    }
    else{
        elem *tmp = stk->top;   // tmp 포인터가 top elem을 가르키도록
        node_t *data = tmp->data;   // 삭제한 값 반환하기 위해 기존의 데이터를 tmp elem에 삽입
        stk->top = stk->top->link;
        free(tmp);
        return data;    // stk에서 삭제되는 node의 주소값 반환
    }
}

rbtree *new_rbtree(void) {
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t)); // nil node 메모리 동적 할당
  nil_node->color = RBTREE_BLACK;                         // nil node 는 black
  t -> nil = nil_node;                                    // t.nil = nil node
  t -> root = nil_node;                                   // t.root = nil node 로 초기화
  return t;                                               // tree t 반환
}

void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;                                    // y를 설정한다
  x->right = y->left;                                      // y의 왼쪽 서브 트리를 x의 오른쪽 서브 트리로 옮긴다
  if(y->left != t->nil){
      y->left->parent = x;
  }
  y->parent = x->parent;                                   // x의 부모를 y로 연결한다
  if(x->parent == t->nil) t->root = y;
  else if(x == x->parent->left) x->parent->left = y;
  else x->parent->right = y;

  y->left = x;                                             // x를 y의 왼쪽자식으로 두자
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left;                                     // y를 설정한다
  x->left = y->right;                                      // y의 왼쪽 서브 트리를 x의 오른쪽 서브 트리로 옮긴다
  if(y->right != t->nil){
      y->right->parent = x;
  }
  y->parent = x->parent;                                   // x의 부모를 y로 연결한다

  if(x->parent == t->nil) t->root = y;
  else if(x == x->parent->right) x->parent->right = y;
  else x->parent->left = y;

  y->right = x;                                            // x를 y의 오른쪽자식으로 두자
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
  node_t *y = t->nil;                        // x가 nil되면 걔 부모를 알고있어야 z가 들어갈 위치를 아는 것임
  node_t *x = t->root;                       // 처음 비교를 root 부터 비교해가며 내려갈 것임
  
  while(x != t->nil){
    y = x;                                   // x가 내력가기전에 y에 저장해주자 왜냐면 x 내려가면 지금 x가 x.p 위치
    if(z->key < x->key) x = x->left;         // 더 작으면 왼쪽 서브트리로 탐색
    else x = x->right;                       // 같거나 크면 오른쪽 서브트리
  }

  z->parent = y;                             // z 부모를 초기화 (전혀 안내려간다면 root 엄마가 nil)
  
  if(y == t->nil) t->root = z;               // z는 root
  else if(z->key < y->key) y->left = z;      // 부모될 애랑 비교해서 왼쪽 자식할지 오른쪽 자식할지
  else y->right = z;

  z->left = t->nil;                          // sentinel 노드 새로 추가된 노드의 자식에 달아주기
  z->right = t->nil;
  z->color = RBTREE_RED;                     // 새로 추가된 z 는 RED 

  insert_fixup(t, z);

  return z;
}

void delete_node(node_t *node, node_t *nil_node) {
  if(node != nil_node){                       // nil 은 하나라서 여러 번 지울수도 없고(double free)할 필요도 없음
    delete_node(node->left, nil_node);    
    delete_node(node->right, nil_node);    
    free(node); node = NULL;                  // 새로 추가된 애들이 섞여있기 때문에 노드단위로 해제 해줌
  }
}

void dfs_delete(node_t *node, node_t *nil_node){
  stack s;
  // node_t *u = node;
  stk_init(&s);
  if(node != nil_node) stk_push(&s, node);
  while(stk_is_empty(&s) == 0){
    node = stk_pop(&s);
    if(node->right != nil_node) stk_push(&s, node->right);
    if(node->left != nil_node) stk_push(&s, node->left);
    free(node);
  }
}

void delete_rbtree(rbtree *t) {
  // delete_node(t->root, t->nil);
  dfs_delete(t->root, t->nil);
  free(t->nil);                               // free node_t *nil_node 
  free(t); t = NULL;                          // free rbtree *t, t 는 정적 할당 된 포인터변수 이기 때문에 직접 없애줌
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *z = (node_t *)calloc(1, sizeof(node_t)); // z node 생성 -> 나중에 delete에서 dfs 하며 free 가능
  z->key = key;
  z = insert(t, z);

  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *z = t->root;                          // root 로 초기화 하고 찾을 것임

  while(z != t->nil){
    if(z->key == key) return z;
    else if(z->key < key) z = z->right;
    else z = z->left;
  }

  return NULL;                                  // 못 찾으면 NULL 반환
}

node_t *rbtree_min(const rbtree *t) {
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
  node_t *z = t->root;
  node_t *max_node = z;

  while(z != t->nil)
  {
    max_node = z;
    z = z->right;
  }

  return max_node;
}

void transplant(rbtree *t, node_t *u, node_t *v){  // v가 u의 자리로
  if(u->parent == t->nil) t->root = v;
  else if(u == u->parent->left) u->parent->left = v;
  else u->parent->right = v;

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
        x = x->parent;                        // 다음 while에서 검사할 x cursor 이동
      }
      else{                                   // (Case3 +) Case4
        if(w->right->color == RBTREE_BLACK){  // Case3
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

      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){ // Case 2: w이 둘다 blk -> w만 빨강으로 바꿔서 해결 시도
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

node_t *get_successor(rbtree *t, node_t *z){  // 그냥 z->right 받아서 z->right 반복해도 되지만 ... 그냥 이렇게 해봄
  node_t *y;
  rbtree *sub = new_rbtree();
                     
  free(sub->nil);                             // new_rbtree하면서 새로운 nil_node가 동적 할당 됨 -> 걔를 없애주고
  sub->nil = t->nil;                          // 기존 rbtree t의 서브트리니까 원래 t.nil을 연결해줌
  sub->root = z->right;
  y = rbtree_min(sub);
  free(sub);

  return y;
}

int rbtree_erase(rbtree *t, node_t *z) {
  node_t *x, *y = z;
  color_t y_original_color = y->color;
  
  if(z->left == t->nil)                 // 자식이 없거나 오른쪽에 한명
  {
    x = z->right;
    transplant(t, z, z->right);
  }
  else if(z->right == t->nil)           // 자식이 왼쪽에 한명
  {
    x = z->left;
    transplant(t, z, z->left);
  }
  else                                  // 자식이 2명
  { 
                                        // z->right을 root로 하는 subtree의 min node가 successor
    y = get_successor(t, z);
    y_original_color = y->color;

    x = y->right;
    if(y->parent == z) x->parent = y;  // x가 t.nil일 경우 부모 정보가 아무렇게 들어있을 것. 이런 경우를 위해서 업데이트 해주는 것
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

  free(z); z = NULL; // 삭제된 node z 는 tree t와 연결이 끊어졌기 때문에 나중에 delete안에서 dfs돌때 방문이 불가능함

  if(y_original_color == RBTREE_BLACK) erase_fixup(t, x);

  return 0;
}

void inorder(node_t *z, size_t *idx, key_t *arr, node_t *nil_node){
  if(z == nil_node) return;

  inorder(z->left, idx, arr, nil_node);
  *(arr + (*idx)++) = z->key;
  inorder(z->right, idx, arr, nil_node);
}

void dfs_stack(node_t *z, size_t *idx, key_t *arr, node_t *nil_node){
  stack s;
  node_t *u = z;
  // node_t *v;
  stk_init(&s);
  // stk_push(&s, u);

  while(1){
    if(u != nil_node){
      stk_push(&s, u);
      u = u->left;
      continue;
    }
    if(stk_is_empty(&s) == 0){
      u = stk_pop(&s);
      *(arr + (*idx)++) = u->key;
      u = u->right;
      continue;
    }
    break;
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  size_t *idx = (size_t *)calloc(1, sizeof(size_t));

  *idx = 0;
  // inorder(t->root, idx, arr, t->nil);
  dfs_stack(t->root, idx, arr, t->nil);
  free(idx); idx = NULL;
  
  return 0;
}
