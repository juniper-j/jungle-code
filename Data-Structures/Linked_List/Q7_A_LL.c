//////////////////////////////////////////////////////////////////////////////////

/* CE1007/CZ1007 Data Structures
Lab Test: Section A - Linked List Questions
Purpose: Implementing the required functions for Question 7 */

//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////

typedef struct _listnode
{
	int item;
	struct _listnode *next;
} ListNode;			// You should not change the definition of ListNode

typedef struct _linkedlist
{
	int size;
	ListNode *head;
} LinkedList;			// You should not change the definition of LinkedList


//////////////////////// function prototypes /////////////////////////////////////

// You should not change the prototype of this function
void recursiveReverse(ListNode **ptrHead);

void printList(LinkedList *ll);
void removeAllItems(LinkedList *ll);
ListNode * findNode(LinkedList *ll, int index);
int insertNode(LinkedList *ll, int index, int value);
int removeNode(LinkedList *ll, int index);


//////////////////////////// main() //////////////////////////////////////////////

int main()
{
	LinkedList ll;
	int c, i, j;
	c = 1;
	//Initialize the linked list 1 as an empty linked list
	ll.head = NULL;
	ll.size = 0;


	printf("1: Insert an integer to the linked list:\n");
	printf("2: Reversed the linked list:\n");
	printf("0: Quit:\n");

	while (c != 0)
	{
		printf("Please input your choice(1/2/0): ");
		scanf("%d", &c);

		switch (c)
		{
		case 1:
			printf("Input an integer that you want to add to the linked list: ");
			scanf("%d", &i);
			j = insertNode(&ll, ll.size, i);
			printf("The resulting linked list is: ");
			printList(&ll);
			break;
		case 2:
			recursiveReverse(&(ll.head)); // You need to code this function
			printf("The resulting linked list after reversed the given linked list is: ");
			printList(&ll);
			removeAllItems(&ll);
			break;
		case 0:
			removeAllItems(&ll);
			break;
		default:
			printf("Choice unknown;\n");
			break;
		}
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////////////
//
// *** recursiveReverse(ListNode **ptrHead) ***
//
// - 연결 리스트를 반복문으로 뒤집는다 (실제로는 재귀가 아닌 반복이지만, 함수명 유지)
// - 각 노드의 next 포인터 방향을 역방향으로 설정하여 리스트를 뒤집음
// - 최종적으로 *ptrHead를 새로운 head (가장 마지막 노드)로 갱신
//
// [개념 정리]
// - ptrHead (ListNode **)       : head 포인터의 주소
// - *ptrHead (ListNode *)       : 연결 리스트의 시작점 (head 노드 자체)
// - **ptrHead (ListNode 구조체) : 실제 노드의 내용물
//
// [로직 개요]
// 1. pre는 이미 뒤집힌 노드를 가리킴 (초기엔 NULL)
// 2. cur는 아직 뒤집히지 않은 노드를 따라가며 하나씩 연결 방향을 바꿈
// 3. 반복이 끝나면 pre가 새로운 head가 되며 *ptrHead를 갱신함
//
//////////////////////////////////////////////////////////////////////////////////
void recursiveReverse(ListNode **ptrHead)
{
    // 예외 처리: NULL 포인터 또는 빈 리스트일 경우 아무 작업도 하지 않음
    if (ptrHead == NULL || *ptrHead == NULL) return;

    // pre: 현재까지 뒤집은 리스트의 head (초기에는 NULL)
    ListNode *pre = NULL;

    // cur: 아직 뒤집지 않은 나머지 리스트의 현재 노드
    ListNode *cur = *ptrHead;

    // 리스트 전체를 순회하며 방향을 반전시킴
    while (cur != NULL)
    {
        // temp: 다음 노드를 백업해둠 (next 포인터를 바꾸기 전에 저장해둬야 함)
        ListNode *temp = cur->next;

        // 현재 노드의 next를 이전 노드를 가리키게 설정 (방향 반전)
        cur->next = pre;
        // pre를 한 칸 앞으로 이동 (현재 노드가 이제 뒤집힌 리스트의 head가 됨)
        pre = cur;
        // cur도 한 칸 앞으로 이동 (다음 노드를 처리하기 위해)
        cur = temp;
    }

    // 리스트의 head를 마지막 노드(pre)로 갱신
    *ptrHead = pre;
}
//////////////////////////////////////////////////////////////////////////////////


void printList(LinkedList *ll){

	ListNode *cur;
	if (ll == NULL)
		return;
	cur = ll->head;

	if (cur == NULL)
		printf("Empty");
	while (cur != NULL)
	{
		printf("%d ", cur->item);
		cur = cur->next;
	}
	printf("\n");
}

ListNode * findNode(LinkedList *ll, int index){

	ListNode *temp;

	if (ll == NULL || index < 0 || index >= ll->size)
		return NULL;

	temp = ll->head;

	if (temp == NULL || index < 0)
		return NULL;

	while (index > 0){
		temp = temp->next;
		if (temp == NULL)
			return NULL;
		index--;
	}

	return temp;
}

int insertNode(LinkedList *ll, int index, int value){

	ListNode *pre, *cur;

	if (ll == NULL || index < 0 || index > ll->size + 1)
		return -1;

	// If empty list or inserting first node, need to update head pointer
	if (ll->head == NULL || index == 0){
		cur = ll->head;
		ll->head = malloc(sizeof(ListNode));
		ll->head->item = value;
		ll->head->next = cur;
		ll->size++;
		return 0;
	}


	// Find the nodes before and at the target position
	// Create a new node and reconnect the links
	if ((pre = findNode(ll, index - 1)) != NULL){
		cur = pre->next;
		pre->next = malloc(sizeof(ListNode));
		pre->next->item = value;
		pre->next->next = cur;
		ll->size++;
		return 0;
	}

	return -1;
}


int removeNode(LinkedList *ll, int index){

	ListNode *pre, *cur;

	// Highest index we can remove is size-1
	if (ll == NULL || index < 0 || index >= ll->size)
		return -1;

	// If removing first node, need to update head pointer
	if (index == 0){
		cur = ll->head->next;
		free(ll->head);
		ll->head = cur;
		ll->size--;

		return 0;
	}

	// Find the nodes before and after the target position
	// Free the target node and reconnect the links
	if ((pre = findNode(ll, index - 1)) != NULL){

		if (pre->next == NULL)
			return -1;

		cur = pre->next;
		pre->next = cur->next;
		free(cur);
		ll->size--;
		return 0;
	}

	return -1;
}

void removeAllItems(LinkedList *ll)
{
	ListNode *cur = ll->head;
	ListNode *tmp;

	while (cur != NULL){
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
	ll->head = NULL;
	ll->size = 0;
}
