#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define Object void*

#define bool int
#define false 0
#define true !0
#define FALSE 0
#define TRUE !0

#define null NULL

#define Comparison int
#define leftEqualToRight 0
#define leftGreaterThanRight 1
#define leftLesserThanRight 2

#define SortType int
#define unsorted 0
#define sortedAscending 1
#define sortedDescending 2

typedef struct List_Struct_Node* List_Node;
typedef struct List_Struct* List;

struct List_Struct_Node{
	Object data;
	List_Node prev;
	List_Node next;
};


struct List_Struct{
	int sort;
	int count;
	List_Node first;
	List_Node last;
};

List List_Create(){
	List list = (List)malloc(sizeof(struct List_Struct));
	list->sort = unsorted;
	list->count = 0;
	list->first = NULL;
	list->last = NULL;
	return list;
}

List_Node List_CreateNode(Object data){
	List_Node n = (List_Node) malloc(sizeof(struct List_Struct_Node));
	n->data = data;
	return n;
}

int List_CountBackwards(List list);

int List_Count(List list){
	if(list == NULL)
		return -1;

	if (list->first == NULL){
		if(list->last != NULL)
			return List_CountBackwards(list);
		list->count = 0;

		return 0;
	}
	int c = 1;
	List_Node n = list->first;
	while(n->next != NULL){
		n=n->next;
		++c;
	}
	list->count = c;
	list->last = n;
	return c;
}

int List_CountBackwards(List list){
	if(list == NULL)
		return -1;

	if (list->last == NULL){
		if (list->first != NULL)
			return List_Count(list);
		list->count = 0;
		return 0;
	}
	int c = 1;
	List_Node n = list->last;
	while(n->prev != NULL){
		n=n->prev;
		++c;
	}
	list->count = c;
	list->first = n;
	return c;
}

int List_Add(List list, Object data){
	if (list == NULL)
		return -1;

	List_Node n = List_CreateNode(data);
	n->next = NULL;
	if (list->first == NULL){
		list->first = n;
		n->prev = NULL;
	}else{
		list->last->next = n;
		n->prev = list->last;
	}
	list->last = n;
	++list->count;
	list->sort = unsorted;
	return list->count - 1;
}



List_Node List_GetNode(List list, int index){
	if(list == NULL || index < 0 || list->first == NULL || index >= list->count)
		return NULL;
	if (index == 0)
		return list->first;
	if (index == list->count -1)
		return list->last;
	List_Node n;
	if (2*index < list->count){
	n = list->first;

	int counter = index+1;
	while(--counter){
		n = n->next;
		if(n == NULL){
			List_Count(list);
			return NULL;
		}
	}
	}else{
		n = list->last;

	int counter = list->count - index;
	while(--counter){
		n = n->prev;
		if(n == NULL){
			List_Count(list);
			return NULL;
		}
	}
	}
	return n;
}

Object List_Get(List list, int index){
	List_Node n = List_GetNode(list, index);
	if (n == NULL)
		return 0;
	return n->data;
}

bool List_RemoveNode(List list, List_Node n, bool freeData){

	if (list == NULL || n == NULL || list->first == NULL)
		return false;

	int c = list->count;
	if (list->first == n){
		list->first = n->next;
		if(c > 1)
			list->first->prev = NULL;
		--list->count;
		if (c == 2)
			list->last = list->first;
		free(n);
		return 1;
	}
	if(list->last == n){
		list->last = n->prev;
		list->last->next = NULL;
		--list->count;
		if (c == 2)
			list->first = list->last;
		free(n);
		return 1;
	}



	n->prev->next = n->next;
	n->next->prev = n->prev;
	--list->count;
	if (freeData)
		free(n->data);
	free(n);
	list->sort = unsorted;
	return 1;
}

bool List_RemoveAt(List list, int index, bool freeData){
	if (list == NULL || index < 0 || list->first == NULL || index >= list->count)
		return false;
	int c = list->count;
	List_Node n;
	if (index == 0){
		n=list->first;
		list->first = n->next;
		if(c > 1)
			list->first->prev = NULL;
		--list->count;
		if (c == 2)
			list->last = list->first;
		free(n);
		return true;
	}
	if(index == c - 1){
		n = list->last;
		list->last = n->prev;
		list->last->next = NULL;
		--list->count;
		if (c == 2)
			list->first = list->last;
		free(n);
		return true;
	}
	n=List_GetNode(list, index);
	if (n == NULL)
		return false;
	n->prev->next = n->next;
	n->next->prev = n->prev;
	--list->count;
	if (freeData)
		free(n->data);
	free(n);
	list->sort = unsorted;
	return true;
}

int List_AddFirst(List list, Object data){
	if (list == NULL)
		return -1;
	if(list->first == NULL){

		return List_Add(list, data);
	}
	List_Node n = List_CreateNode(data);
	n->next = list->first;
	n->prev = NULL;
	list->first->prev = n;
	list->first = n;
	++list->count;
	list->sort = unsorted;
	return 0;
}

int List_Insert(List list, Object data, int index){
	if (list == NULL)
		return -1;
	if (list->count == 0){
		if (index == 0){
			return List_Add(list, data);
		}else{
			return -1;
		}
	}
	if (index >= list->count){
		return -1;
	}
	if(index < 0){
		return -1;
	}


	if(index == 0){
		return List_AddFirst(list, data);
	}
	List_Node n=List_CreateNode(data);
	List_Node before = List_GetNode(list, index-1);
	n->next = before->next;
	n->prev = before;
	before->next = n;
	n->next->prev=n;
	++list->count;
	list->sort = unsorted;
	return index;
}

int List_InsertAnyway(List list, Object data, int index){
	if (list == NULL)
		return -1;
	if (list->count == 0){
		return List_Add(list, data);
	}
	if (index >= list->count){
		return List_Add(list, data);
	}
	if(index < 0){
		return List_AddFirst(list, data);
	}


	if(index == 0){

		return List_AddFirst(list, data);
	}
	List_Node n=List_CreateNode(data);
	List_Node before = List_GetNode(list, index-1);
	n->next = before->next;
	n->prev = before;
	before->next = n;
	n->next->prev=n;
	++list->count;
	list->sort = unsorted;
	return index;
}


int List_RemoveFirst(List list, bool f(Object, Object), Object param, int count, bool freeData){
	if (list == NULL || list->first == NULL || count < 1)
		return 0;
	int i = 0;
	List_Node n = list->first;
	while(i < count && n != NULL){
		if (f(n->data, param)){
			List_Node m = n;
			n = n->next;
			i+=List_RemoveNode(list, m, freeData);

		}else
			n = n->next;
	}
	list->sort = unsorted;
	return i;
}



int List_RemoveLast(List list, bool f(Object, Object), Object param, int count, bool freeData){
	if (list == NULL || list->first == NULL || count < 1)
		return 0;
	int i = 0;
	List_Node n = list->last;
	while(i < count && n != NULL){
		if (f(n->data, param)){
			List_Node m = n;
			n = n->prev;
			i+=List_RemoveNode(list, m, freeData);

		}else
			n = n->prev;
	}
	list->sort = unsorted;
	return i;
}

int List_RemoveAll(List list, bool f(Object, Object), Object param, bool freeData){
	if (list == NULL || list->first == NULL)
		return 0;

	int i = 0;
	List_Node n = list->first;
	while(n != NULL){
		if (f(n->data, param)){
			List_Node m = n;
			n = n->next;

			i+=List_RemoveNode(list, m, freeData);


		}else{

			n = n->next;
		}
	}
	list->sort = unsorted;
	return i;
}

void Clear(List list, bool freeData){
	if(list == NULL)
		return;
	if (list->first == NULL){
		if (list->last != NULL){
			List_CountBackwards(list);
			Clear(list, freeData);
			return;
		}
	}else if (list->last == NULL){
		List_Count(list);
		Clear(list, freeData);
		return;
	}

	List_Node n = list->first;
	List_Node m;
	while (n != NULL){
		m=n;
		n = n->next;
		if(freeData)
			free(m->data);
		free(m);
	}
	list->sort = unsorted;
	list->count = 0;
	list->first = NULL;
	list->last = NULL;
}

int List_RemoveRange(List list, int first, int last, bool freeData){
	if (list == NULL || list->first == NULL || last < first || first < 0 || last < 0 || first >= list->count || last >= list->count)
		return 0;
	int delList_Count = last-first+1;
	if(delList_Count > list->count)
		return 0;
	if(delList_Count == 1)
		return List_RemoveAt(list, first, true);
	if (delList_Count == list->count){
		Clear(list, true);
		return true;
	}
	list->sort = unsorted;
	List_Node before;
	List_Node n;
	if (first == 0){
		before = NULL;
		n = List_GetNode(list, first);
	}else{
		before = List_GetNode(list, first-1);
		n=before->next;
	}
	List_Node m = n;

	int i =delList_Count +1;
	while(--i){
		if(n == NULL){
			List_Count(list);
			return 0;
		}
		m = n;
		n = n->next;
		if(freeData)
			free(m->data);
		free(m);
	}
	if(first == 0){
		list->first = n;

	}

	if(n != NULL){
		n->prev = before;
	}
	if(before != NULL){
		before->next = n;

	}
	if(last == list->count -1){
		list->last = before;
	}
	list->count -= delList_Count;
	return true;
}

int List_CountWhere(List list, bool f(Object, Object), Object param){
	if (list == NULL)
		return 0;
	if(list->first == NULL){
		list->count = 0;
		return 0;
	}
	int i = 0;

	List_Node n = list->first;
	if(f(n->data, param))
		++i;
	int c = 1;
	while(n->next != NULL){
		n = n->next;
		if (f(n->data, param))
			++i;
		++c;
	}
	list->count = c;
	list->last = n;
	return i;
}

int List_IndexOfFirst(List list, bool f(Object, Object), Object param){
	if (list == NULL || list->first == NULL)
		return -1;
	List_Node n = list->first;

	int i = 0;
	while(n != NULL){
		if (f(n->data, param))
			return i;
		n = n->next;
		++i;
	}
	return -1;
}

int List_IndexOfLast(List list, bool f(Object, Object), Object param){
	if (list == NULL || list->last == NULL)
		return -1;
	List_Node n = list->last;

	int i = list->count-1;
	while(n != NULL){
		if (f(n->data, param))
			return i;
		n = n->prev;
		--i;
	}
	return -1;
}




void PrintList(List list){
	int c = list->count;
	printf("\n\nList_Count = %d\n", c);
	int i;
	for (i = 0; i < c; i++){
		printf("%d, ", VoidToInt(List_Get(list, i), false));
	}
	/*List_Node n = list->first;
	while(n!=NULL){
		printf("%d, ", n->data);
		n = n->next;
	}*/
	printf("\n\n");
}

Object List_Pop(List list){
	if(list == NULL || list->last == NULL)
		return 0;
	List_Node n = list->last;
	list->last = n->prev;
	if(list->last != NULL)
		list->last->next = NULL;
	--list->count;
	Object ret = n->data;
	free(n);
	return ret;
}

bool List_TryPop(List list, Object* out){
	if (list == NULL || list->count < 1 || list->first == NULL)
		return false;
	*out = List_Pop(list);
	return true;
}

Object List_PopFirst(List list){
	if(list == NULL || list->first == NULL)
		return 0;
	List_Node n = list->first;
	list->first = list->first->next;
	list->first->prev = NULL;
	Object ret = n->data;
	free(n);
	--list->count;
	return ret;
}

bool List_TryPopFirst(List list, Object* out){
	if(list == NULL || list->first == NULL)
		return false;
	*out = List_PopFirst(list);
	return true;

}

int List_SwitchNodes(List list, List_Node a1, List_Node b1){
	if(list == NULL || list->count < 2 || a1 == NULL || b1 == NULL)
		return 0;

	if(b1 == a1->next){
		a1->next = b1->next;
		b1->prev = a1->prev;
		b1->next = a1;
		a1->prev = b1;
		if(a1->next != NULL)
			a1->next->prev = a1;
		if (b1->prev != NULL)
			b1->prev->next = b1;
	}else if (a1 == b1->next){
		b1->next = a1->next;
		a1->prev = b1->prev;
		a1->next = b1;
		b1->prev = a1;
		if(a1->prev != NULL)
			a1->prev->next = a1;
		if (b1->next != NULL)
			b1->next->prev = b1;
	}else{
		List_Node b2 = b1->next;
		b1->next = a1->next;
		List_Node a0 = a1->prev;
		a1->prev = b1->prev;
		b1->prev = a0;
		a1->next = b2;
		if(b2 != NULL)
			b2->prev = a1;
		if(a0 != NULL)
			a0->next = b1;
		if(a1->prev != NULL)
			a1->prev->next = a1;
		if (b1->next != NULL)
			b1->next->prev = b1;
	}
	if (a1->prev == NULL)
		list->first = a1;
	if (a1->next == NULL)
		list->last = a1;
	if(b1->prev == NULL)
		list->first = b1;
	if (b1->next == NULL)
		list->last = b1;
	list->sort = unsorted;
	return true;
}

void List_Sort(List list, Comparison f(Object, Object), bool ascending){
	if(list == NULL || list->count < 2)
		return;
	int i, j, c=list->count;


	i = c;

	while(--i){

		List_Node n = list->first;
		for(j = 0; j < i; j++){
			int flip;
			if(ascending)
				flip = f(n->data, n->next->data) == leftGreaterThanRight;
			else
				flip = f(n->data, n->next->data) == leftLesserThanRight;
			if(flip){
				Object c = n ->data;
				n->data = n->next->data;
				n->next->data = c;
			}
			n = n->next;
		}
	}

	list->sort = ascending ? sortedAscending : sortedDescending;
}

void List_Reverse(List list){
	if (list == NULL || list->count < 2)
		return;

	List_Node a = list->first;
	List_Node z = list->last;
	int c = list->count/2 +1;
	while(--c){
		Object t = a->data;
		a->data = z->data;
		z->data = t;
		a=a->next;
		z=z->prev;
	}
	if(list->sort == 1)
		list->sort = 2;
	else if(list->sort == 2)
		list->sort = 1;

}

void List_ReverseNodes(List list){
	if (list == NULL || list->count < 2)
		return;
	List_Node n = list->first;
	list->first = list->last;
	list->last = n;
	while(n != NULL){
		n->prev = n->next;
		n = n->next;
	}
	n = list->last;
	n->next = NULL;
	while(n->prev != NULL){
		n->prev->next = n;
		n = n->prev;
	}
	if(list->sort == 1)
		list->sort = 2;
	else if(list->sort == 2)
		list->sort = 1;
}

List List_JoinFirst(List List_Add, List to, int freeIt){
	if(List_Add == NULL || to == NULL)
		return NULL;
	if(List_Add->count < 1)
		return to;
	if (to->count < 1){
		to->count = List_Add->count;
		to->first = List_Add->first;
		to->last = List_Add->last;
		to->sort = List_Add->sort;
	}else{
	List_Add->last->next = to->first;
	to->first->prev = List_Add->last;
	to->first = List_Add->first;
	to->count +=List_Add->count;
	to->sort = 0;
	}
	if(freeIt)
		free(List_Add);
	else{
	List_Add->count = 0;
	List_Add->first = NULL;
	List_Add->last = NULL;
	}
	return to;
}

List List_JoinLast(List List_Add, List to, int freeIt){
	if(List_Add == NULL || to == NULL)
		return NULL;
	if(List_Add->count < 1)
		return to;
	if (to->count < 1){
		to->count = List_Add->count;
		to->first = List_Add->first;
		to->last = List_Add->last;
		to->sort = List_Add->sort;
	}else{
		to->last->next = List_Add->first;
		List_Add->first->prev = to->last;
		to->last = List_Add->last;
		to->count +=List_Add->count;
		to->sort = 0;
	}
	if(freeIt)
		free(List_Add);
	else{
		List_Add->count = 0;
		List_Add->first = NULL;
		List_Add->last = NULL;
	}
	return to;
}

List List_JoinAt(List List_Add, List to, int index, int freeIt){
	if(List_Add == NULL || to == NULL || index >= to->count)
		return NULL;
	if(index == 0)
		return List_JoinFirst(List_Add, to, freeIt);
	if (index == to->count)
		return List_JoinLast(List_Add, to, freeIt);
	if(List_Add->count < 1)
		return to;
	if (to->count < 1){
		if(index == 0){
		to->count = List_Add->count;
		to->first = List_Add->first;
		to->last = List_Add->last;
		to->sort = List_Add->sort;
		}
	}else{

		List_Node after = List_GetNode(to, index);
		List_Add->first->prev = after->prev;
		after->prev->next = List_Add->first;
		List_Add->last->next = after;
		after->prev = List_Add->last;

	to->count +=List_Add->count;
	to->sort = 0;
	}
	if(freeIt)
		free(List_Add);
	else{
	List_Add->count = 0;
	List_Add->first = NULL;
	List_Add->last = NULL;
	}
	return to;
}

List List_JoinAtAnyway(List List_Add, List to, int index, int freeIt){
	if(List_Add == NULL || to == NULL)
		return NULL;
	if(index == 0)
		return List_JoinFirst(List_Add, to, freeIt);
	if (index >= to->count)
		return List_JoinLast(List_Add, to, freeIt);
	if(List_Add->count < 1)
		return to;

	if (to->count < 1){
		to->count = List_Add->count;
		to->first = List_Add->first;
		to->last = List_Add->last;
		to->sort = List_Add->sort;

	}else{

		List_Node after = List_GetNode(to, index);
		List_Add->first->prev = after->prev;
		after->prev->next = List_Add->first;
		List_Add->last->next = after;
		after->prev = List_Add->last;


	to->count +=List_Add->count;
	to->sort = 0;
	}
	if(freeIt)
		free(List_Add);
	else{
	List_Add->count = 0;
	List_Add->first = NULL;
	List_Add->last = NULL;
	}
	return to;
}

void List_Destroy(List list, bool freeData){
	Clear(list, freeData);
	free(list);
}

Object IntToVoid(int x){
	Object y = malloc(sizeof(x));

	return memcpy(y, &x, sizeof(x));
}

int VoidToInt(Object x, bool freeIt){
    int ret = *(int *)x;
    if (freeIt)
		free(x);
    return ret;
}

bool IntEquality(Object x, Object y){
	return VoidToInt(x, false) == VoidToInt(y, false);
}

Comparison CompareInt(Object x, Object y){
    int a = VoidToInt(x, false);
    int b = VoidToInt(y, false);

    if (a == b)
		return leftEqualToRight;
	else if (a < b)
		return leftLesserThanRight;
	else
		return leftGreaterThanRight;
}

int main(){

	int a1 = 3;
	int a2 = 1;
	int a3 = 2;
	int a4 = 5;
	int a5 = 6;
	int a6 = 9;
	int a7 = 0;
	List list = List_Create();
	List_Add(list, IntToVoid(3));
	List_Add(list, &a2);
	List_Add(list, &a3);
	List_Add(list, &a4);
	List_Add(list, &a5);
	List_Add(list, &a6);
	List_Add(list, &a7);
	printf("Start list");
	PrintList(list);

	int b = -1;
	printf("List_Adding '-1' at the end of the list");
	List_Add(list,&b);
	PrintList(list);

	printf("Removing item at index = 3 %s", List_RemoveAt(list, 3, true) ? "succeeded" : "failed");

	PrintList(list);
	printf("Removing items from index = 3 to 4 %s", List_RemoveRange(list, 3, 4, true) ? "succeeded" : "failed");

	PrintList(list);

	printf("Popping last item : %d", VoidToInt(List_Pop(list), true));
	PrintList(list);

	printf("Popping first item : %d", VoidToInt(List_PopFirst(list), true));
	PrintList(list);

	int c = 4;
	printf("List_Inserting '4' at index = 0 %s", List_AddFirst(list, &c) > -1 ?"succeeded":"failed");
	PrintList(list);

	int d = 13;
	printf("List_Inserting '13' at index = 3 %s", List_Insert(list, &d, 3) > -1 ? "succeeded" : "failed");
	PrintList(list);

	printf("First index of '2' is %d\n\n", List_IndexOfFirst(list, IntEquality, IntToVoid(2)));
	printf("Last index of '2' is %d\n\n",List_IndexOfLast(list, IntEquality, IntToVoid(2)));

	printf("Removing all '2'. List_Removed : %d", List_RemoveAll(list, IntEquality, IntToVoid(2), true));
	PrintList(list);

	printf("Switching node at index = 1 with node at index = 3");
	List_SwitchNodes(list, List_GetNode(list, 1), List_GetNode(list, 3));
	PrintList(list);

	printf("Reversing list");
	List_Reverse(list);
	PrintList(list);

	printf("Reversing list");
	List_Reverse(list);
	PrintList(list);

	printf("Sorting list ascending");
	List_Sort(list, CompareInt, true);
	PrintList(list);

	printf("Sorting list descending");
	List_Sort(list, CompareInt, false);
	PrintList(list);

	return 0;
}
