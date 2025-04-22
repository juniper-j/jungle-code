#include "rbtree.h"
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////////
//
// rbtree *new_rbtree(void)
//
// 레드-블랙 트리 구조체를 생성하고 초기화하는 함수
// - 트리의 root는 비어 있고 nil 노드만 존재함
// - sentinel 노드(nil)를 생성하여 모든 리프와 초기 포인터로 사용
// rbtree 구조체의 포인터를 반환하는 함수이므로 반환값은 '주소' 또는 'NULL'
//
// 구현 아이디어:
// 1. 트리 구조체를 calloc으로 할당하여 초기화
// 2. sentinel 역할을 하는 nil 노드를 별도로 동적 할당
// 3. nil 노드는 BLACK이며, 모든 포인터는 자기 자신을 가리킴
// 4. root는 아직 노드가 없으므로 nil을 가리키도록 설정
//
//////////////////////////////////////////////////////////////////////////
rbtree *new_rbtree(void) 
{  // 일반적 호출: rbtree *t = new_rbtree()
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));  // 트리 구조체 메모리 할당
  if (p == NULL) return NULL;  // 할당 실패 시 NULL 반환

  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));  // sentinel nil node 메모리 할당
  if (nil_node == NULL) {
    free(p);  // 이미 할당된 트리 구조체의 메모리를 해제함으로 메모리 누수 방지
    return NULL;  // 실패를 호출자에게 알림
  }

  // nil node 속성 설정
  nil_node->color = RBTREE_BLACK;
  nil_node->left = nil_node;
  nil_node->right = nil_node;
  nil_node->parent = nil_node;

  // 트리의 nil과 root 설정
  p->nil = nil_node;
  p->root = p->nil;

  return p;
}


//////////////////////////////////////////////////////////////////////////
// 후위 순회로 노드 하나 씩 free() 후 sentinel 까지 해제해야 함
// 
//////////////////////////////////////////////////////////////////////////
void delete_rbtree(rbtree *t) 
{

  // node_t *cur = t->root;
  // if (cur != t->nil) {

  // }
  // free(t->nil);
  // free(t);
}


//////////////////////////////////////////////////////////////////////////
//
// void left_rotate(rbtree *t, node_t *x)
//
// x를 기준으로 좌회전(left rotation)을 수행하는 함수
// 1. x의 오른쪽 자식 y의 왼쪽 자식을 x의 오른쪽으로 옮기고
// 2. y가 x의 자리를 대신하여 위로 올라가고 
//    - (1) x가 root (2) left child (3) right child
// 3. x는 y의 왼쪽 자식이 됨
// 4. 트리의 연결 관계(parent, left, right)를 모두 업데이트
//
// 전제조건:
//   - x->right != t->nil 이어야 함 (오른쪽 자식이 존재해야 좌회전 가능)
//
//////////////////////////////////////////////////////////////////////////
void left_rotate(rbtree *t, node_t *x) 
{
  node_t *y = x->right;         // y는 x의 오른쪽 자식

  /* 1. x의 오른쪽 자식 y의 왼쪽 자식을 x의 오른쪽으로 옮기고 */
  x->right = y->left;           // y의 왼쪽 서브트리를 x의 오른쪽으로 옮기기 (부모 → 자식)
  if (y->left != t->nil) {      // y의 왼쪽 자식이 있다면, 그 부모를 x로 갱신 (자식 → 부모)
    y->left->parent = x;
  } 
  y->parent = x->parent;        // y의 부모를 x의 부모로 설정

  /* 2. y가 x의 자리를 대신하여 위로 올라가고 */
  if (x->parent == t->nil) {    // (1) x가 루트였으면, y가 새로운 루트
    t->root = y;
  } else if (x == x->parent->left) {  // (2) x가 왼쪽 자식이었다면 y를 왼쪽에 연결
    x->parent->left = y;
  } else {                      // (3) x가 오른쪽 자식이었다면 y를 오른쪽에 연결
    x->parent->right = y;
  }

  /* 3. x는 y의 왼쪽 자식이 됨 */
  y->left = x;    // x를 y의 왼쪽 자식으로 설정
  x->parent = y;  // x의 부모를 y로 설정
}


//////////////////////////////////////////////////////////////////////////
//
// void right_rotate(rbtree *t, node_t *x)
//
// x를 기준으로 우회전(right rotation)을 수행하는 함수
// 1. x의 왼쪽 자식 y의 오른쪽 자식을 x의 왼쪽으로 옮기고
// 2. y가 x의 자리를 대신하여 위로 올라가고 
//    - (1) x가 root (2) left child (3) right child
// 3. x는 y의 오른쪽 자식이 됨
// 4. 트리의 연결 관계(parent, left, right)를 모두 업데이트
//
// 전제조건:
//   - x->left != t->nil 이어야 함 (왼쪽 자식이 존재해야 우회전 가능)
//
//////////////////////////////////////////////////////////////////////////
void right_rotate(rbtree *t, node_t *x) 
{
  node_t *y = x->left;          // y는 x의 왼쪽 자식

  /* 1. x의 왼쪽 자식 y의 오른쪽 자식을 x의 왼쪽으로 옮기고 */
  x->left = y->right;           // y의 오른쪽 서브트리를 x의 왼쪽으로 옮기기 (부모 → 자식)
  if (y->right != t->nil) {     // y의 오른쪽 자식이 있다면, 그 부모를 x로 갱신 (자식 → 부모)
    y->right->parent = x;
  }
  y->parent = x->parent;        // y의 부모를 x의 부모로 설정

  /* 2. y가 x의 자리를 대신하여 위로 올라가고 */
  if (x->parent == t->nil) {    // (1) x가 루트였으면, y가 새로운 루트
    t->root = y;
  } else if (x == x->parent->left) {   // (2) x가 왼쪽 자식이었다면 y를 왼쪽에 연결
    x->parent->left = y;
  } else {                      // (3) x가 오른쪽 자식이었다면 y를 오른쪽에 연결
    x->parent->right = y;
  }

  /* 3. x는 y의 오른쪽 자식이 됨 */
  y->right = x;   // x를 y의 오른쪽 자식으로 설정
  x->parent = y;  // x의 부모를 y로 설정
}


//////////////////////////////////////////////////////////////////////////
//
// void invert_color(rbtree *t, node_t *gp)
//
// insert_fixup CASE 1 처리 함수: 부모와 삼촌이 모두 RED인 경우
// - 조부모는 RED로, 부모와 삼촌은 BLACK으로 바꿈
//
//////////////////////////////////////////////////////////////////////////
void invert_color(rbtree *t, node_t *y)
{ // parent, uncle이 모두 red이면 black으로, grand parent를 red로 바꾼다.
  if (y == t->nil || y->left == t->nil || y->right == t->nil) return;

  if (y->color == RBTREE_BLACK) {
    y->color = RBTREE_RED;
    y->left->color = RBTREE_BLACK;
    y->right->color = RBTREE_BLACK;
  } else {
    y->color = RBTREE_BLACK;
    y->left->color = RBTREE_RED;
    y->right->color = RBTREE_RED;
  }
}


//////////////////////////////////////////////////////////////////////////
//
// void swap_color(node_t *par, node_t *gp)
//
// insert_fixup CASE 3 처리 함수: 삼촌이 BLACK이고 z가 일직선 구조일 때
// - 부모는 BLACK, 조부모는 RED로 바꾸어 회전에 대비
//
//////////////////////////////////////////////////////////////////////////
void swap_color(rbtree *t, node_t *y) 
{ // parent는 red, uncle은 black일 경우, parent를 black, grand parent를 red 바꾼다.
  if (y == t->nil || y->parent == t->nil) return;

  int tmp = y->color;
  y->color = y->parent->color;
  y->parent->color = tmp;
}


//////////////////////////////////////////////////////////////////////////
//
// node_t *rbtree_insert(rbtree *t, const key_t key)
//
// 주어진 키 값을 레드-블랙 트리에 삽입하는 함수
// - 트리는 이진 탐색 트리의 특성을 유지하며 노드를 삽입하고,
// - 삽입 후에는 색상과 회전을 통해 레드-블랙 트리 속성을 복구함
//
// 반환:
// - 삽입된 노드의 포인터 (성공 시)
// - 기존 동일 키 노드의 포인터 (중복 키인 경우)
// - NULL (할당 실패 또는 유효하지 않은 트리)
//
// 구현 아이디어:
// 1. 새 노드를 RED로 생성하고, nil 노드를 초기 자식으로 설정
// 2. 이진 탐색 트리 방식으로 삽입 위치 탐색
// 3. 부모 포인터와 부모의 자식 포인터를 통해 위치에 노드 연결
// 4. 삽입된 노드로 인해 위반된 레드-블랙 트리 속성은 fixup 함수로 복구
//
//////////////////////////////////////////////////////////////////////////
node_t *rbtree_insert(rbtree *t, const key_t key) 
{
  if (t == NULL || t->nil == NULL) return NULL; // 트리 포인터, nil 노드 유효성 검사

  /* Step 1. 새 노드 생성 및 기본 설정 */
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  if (new_node == NULL) return NULL;  // 할당 실패 시 NULL 반환
  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->parent = t->nil;  
  new_node->left = t->nil;
  new_node->right = t->nil;

  /* Step 2. 새 노드 삽입 위치 찾기 */
  node_t *par = t->nil;   // 부모 후보
  node_t *cur = t->root;  // 탐색은 루트부터 시작

  while (cur != t->nil) // 현재 노드가 nil이 될 때까지 탐색
  {                         
    par = cur;          // 현재 위치를 부모 후보로 저장 (삽입 지점에 도달하기 직전의 노드를 저장)

    // (1) 중복 키는 허용하지 않으며, 기존 노드를 반환함
    if (key == cur->key) {
      free(new_node);
      return cur;
    }

    if (key < cur->key) { // (2) 삽입할 키값이 현재보다 작으면 왼쪽으로 이동
      cur = cur->left;
    }
    else {                // (3) 삽입할 키값이 현재보다 크면 오른쪽으로 이동
      cur = cur->right;
    }
  }

  /* Step 3. 새 노드 부모 설정 (자식 → 부모) */
  new_node->parent = par;

  /* Step 3: 새 노드 부모 설정 (부모 → 자식) */
  if (par == t->nil) {        // (1) 부모가 NIL이면 트리가 비어있는 것 → 루트 설정
    t->root = new_node;
    new_node->color = RBTREE_BLACK;
  } 
  else if (key < par->key) {  // (2) 부모의 왼쪽 자식인지 확인
    par->left = new_node;
  } 
  else {                      // (3) 아니면 오른쪽 자식
    par->right = new_node;
  }

  /* Step 4: RB-트리 속성 복구 */
  rbtree_insert_fixup(t, new_node);
  return new_node;
}


//////////////////////////////////////////////////////////////////////////
//
// void rbtree_insert_fixup(rbtree *t, node_t *node)
//
// 레드-블랙 트리에 노드를 삽입한 후, 트리의 균형과 색상 속성을 복구하는 함수
// - 삽입된 노드는 항상 RED이므로, 부모가 RED인 경우 RB 트리의 성질이 위반됨
// - 부모, 삼촌, 조부모의 색과 방향 관계에 따라 3가지 Case로 나누어 처리
//   (1) 부모와 삼촌이 모두 RED → 색상 반전 (Case 1)
//   (2) 삼촌은 BLACK, z가 부모의 반대쪽 자식 → 회전 준비 (Case 2)
//   (3) 삼촌은 BLACK, z가 부모와 일직선 → 회전 및 색상 교환 (Case 3)
// - 삽입 이후에도 트리 전체의 불변성을 유지하기 위한 핵심 함수
//
// 반환: 없음 (트리 내부 구조 직접 수정)
//
// 구현 아이디어:
// 1. z의 부모가 RED인 동안 반복하며 위로 올라감
// 2. 부모가 조부모의 왼쪽 자식인지, 오른쪽 자식인지에 따라 대칭 처리
// 3. 각 Case에 따라 색상 변경 및 left/right 회전 수행
// 4. 최종적으로 루트 노드는 항상 BLACK이어야 하므로 보정
//
//////////////////////////////////////////////////////////////////////////
void rbtree_insert_fixup(rbtree *t, node_t *node) 
{  
  while (node->parent->color == RBTREE_RED) // 자식-부모 Double Red로 속성 위반
  {
    node_t *gpar = node->parent->parent;
    node_t *par = node->parent;
    node_t *unc;

    if (par == gpar->left) {  // node의 부모가 조부모의 왼쪽 자식인지 검사
      unc = gpar->right;      // node의 삼촌은 오른쪽 자식
      if (unc->color == RBTREE_RED) 
      { // CASE 1: 부모와 삼촌이 모두 Red → 색상 반전
        par->color = RBTREE_BLACK;
        unc->color = RBTREE_BLACK;
        gpar->color = RBTREE_RED;
        node = gpar;
      }
      else { // 부모는 Red, 삼촌은 Black 
        if (node == par->right) 
        { // CASE 2: node가 "삼각형" 구조 → 왼쪽 회전 후 Case 3 구조로 전환
          node = par;                 // node를 부모로 이동 후 (이후 루프에서 par, gpar는 node 기준으로 다시 계산됨)
          left_rotate(t, node);       // 왼쪽 회전으로 CASE 3 구조로 변경
        }
        else 
        { // CASE 3: node가 "일자형" 구조 → 색상 교환 후 오른쪽 회전
          par->color = RBTREE_BLACK;  // 부모는 Red로
          gpar->color = RBTREE_RED;   // 조부모는 Black으로 변경 후
          right_rotate(t, gpar);      // 조부모 기준 오른쪽 회전
        }
      }
    }
    else {  // 대칭 처리: 부모가 조부모의 오른쪽 자식일 때
      unc = gpar->left;
      if (unc->color == RBTREE_RED) {
        par->color = RBTREE_BLACK;
        unc->color = RBTREE_BLACK;
        gpar->color = RBTREE_RED;
        node = gpar;
      }
      else {
        if (node == par->left) {
          node = par;
          right_rotate(t, node);
        }
        else {
          par->color = RBTREE_BLACK;
          gpar->color = RBTREE_RED;
          left_rotate(t, gpar);
        }
      }
    }
  }

  t->root->color = RBTREE_BLACK;  // 루트는 항상 Black 이어야 함
}


//////////////////////////////////////////////////////////////////////////
//
// node_t *rbtree_find(const rbtree *t, const key_t key)
//
// 레드-블랙 트리에서 key를 갖는 노드를 탐색하는 함수
// - key와 일치하는 노드가 있으면 해당 노드 포인터 반환
// - 없으면 sentinel 노드(t->nil)를 반환함
//
// 일반적 호출 예시:
//   node_t *p = rbtree_find(t, 12);
//   if (p != t->nil) { printf("%d found!\n", p->key); }
//
//////////////////////////////////////////////////////////////////////////
node_t *rbtree_find(const rbtree *t, const key_t key) 
{
  node_t *cur = t->root;  // 탐색은 루트부터 시작

  while (cur != t->nil) { // 현재 노드가 nil이 될 때까지 반복
    if (key == cur->key) {  // 현재 노드의 키와 찾는 키가 같으면 해당 노드를 반환
      return cur;
    } else if (key > cur->key) {  // 찾는 키가 현재 노드보다 크면 오른쪽 서브트리로 이동
      cur = cur->right;
    } else {  // 찾는 키가 더 작으면 왼쪽 서브트리로 이동
      cur = cur->left;
    }
  }

  return t->nil;  // 루프를 빠져나오면 찾는 키가 없는 것을 의미 -> sentinel 노드 반환
}


//////////////////////////////////////////////////////////////////////////
//
// node_t *rbtree_min(const rbtree *t)
//
// 레드-블랙 트리에서 가장 작은 값을 가진 노드를 반환하는 함수
// - 트리의 왼쪽 자식을 따라 내려가면 항상 값이 작아지므로, 가장 왼쪽 리프 노드가 최소값을 갖는다.
// - sentinel(nil) 노드가 리프 역할을 하므로, left 포인터가 nil일 때까지 탐색을 반복한다.
// - 트리가 비어 있는 경우(t->root == t->nil)는 그대로 nil을 반환한다.
//
// 일반적 사용 예시:
//   node_t *min = rbtree_min(t);
//   if (min != t->nil) printf("최소값은 %d입니다\n", min->key);
//
//////////////////////////////////////////////////////////////////////////
node_t *rbtree_min(const rbtree *t) 
{
  node_t *cur = t->root;  // 루트부터 탐색 시작
  if (cur == t->nil) return t->nil; // 트리가 비어 있으면 nil 반환

  while (cur->left != t->nil) { // 가장 왼쪽 자식이 nil일 때까지 왼쪽으로 이동
    cur = cur->left;
  }

  return cur; // 최소값 노드 반환
}


//////////////////////////////////////////////////////////////////////////
//
// node_t *rbtree_max(const rbtree *t)
//
// 레드-블랙 트리에서 가장 큰 값을 가진 노드를 반환하는 함수
// - 트리의 오른쪽 자식을 따라 내려가면 항상 값이 커지므로, 가장 왼쪽 리프 노드가 최댓값을 갖는다.
// - sentinel(nil) 노드가 리프 역할을 하므로, right 포인터가 nil일 때까지 탐색을 반복한다.
// - 트리가 비어 있는 경우(t->root == t->nil)는 그대로 nil을 반환한다.
//
// 일반적 사용 예시:
//   node_t *max = rbtree_max(t);
//   if (max != t->nil) printf("최댓값은 %d입니다\n", max->key);
//
//////////////////////////////////////////////////////////////////////////
node_t *rbtree_max(const rbtree *t) 
{
  node_t *cur = t->root;  // 루트부터 탐색 시작
  if (cur == t->nil) return t->nil; // 트리가 비어 있으면 nil 반환

  while (cur->right != t->nil) {  // 가장 오른쪽 자식이 nil일 때까지 오른쪽으로 이동
    cur = cur->right;
  }

  return cur; // 최댓값 노드 반환
}


int rbtree_erase(rbtree *t, node_t *p) 
{
  // TODO: implement erase
  return 0;
}


//////////////////////////////////////////////////////////////////////////
//
// int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
//
// 레드-블랙 트리를 중위 순회하며 키 값을 정렬된 배열로 저장하는 함수
// - 중위 순회 결과는 오름차순 정렬을 보장함
// - 배열의 크기가 트리 노드 수보다 작을 경우, 결과가 잘릴 수 있음
// - 성공 시 실제로 저장된 노드 개수 반환, 입력 오류 또는 배열 부족 시 -1 반환
//
// 구현 전략:
// 1. 스택을 직접 만들어 중위 순회를 통해 트리를 오름차순으로 탐색 (left → root → right)
// 2. 순회하면서 배열에 key 값을 저장
// 3. 배열이 꽉 차면 조기 종료
//
//////////////////////////////////////////////////////////////////////////
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) 
{
  if (t == NULL || arr == NULL || t->nil == NULL || t->root == NULL) return -1;

  node_t *stack[100];     // 레드블랙 트리는 항상 균형 이진 탐색 트리이기 때문에 최대 깊이는 O(log₂ n) 정도
  int top = -1;           // 스택 포인터
  node_t *cur = t->root;  // 루트부터 탐색 시작
  size_t idx = 0;         // 결과 배열의 인덱스

  while (top != -1 || cur != t->nil)
  {
    while (cur != t->nil) { // 왼쪽 서브트리를 따라 내려가며 스택에 push
      if (top + 1 >= 100) return -1;  // 스택 오버플로우 방지
      stack[++top] = cur;
      cur = cur->left;
    }
    
    cur = stack[top--];   // 왼쪽 끝에 도달하면 스택에서 pop

    if(idx >= n) break;   // 배열 크기 초과 방지
    arr[idx++] = cur->key;

    cur = cur->right;     // 오른쪽 서브트리로 이동
  }

  return (int)idx;
}