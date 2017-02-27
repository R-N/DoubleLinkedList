#include<stdio.h>
#include<stdlib.h>

#define bool int
#define false 0
#define true !0
#define FALSE 0
#define TRUE !0
#define null NULL

struct node{
	int data;
	struct node* prev;
	struct node* next;
};
struct list_t{
	int sort;
	int count;
	struct node* first;
	struct node* last;
};

struct list_t* CreateList(){
	struct list_t* list = (struct list_t*)malloc(sizeof(struct list_t));
	list->sort = 0;
	list->count = 0;
	list->first = NULL;
	list->last = NULL;
	return list;
}

struct node* CreateNode(int data){
	struct node* n = (struct node*) malloc(sizeof(struct node));
	n->data = data;
	return n;
}

int CountBackwards(struct list_t* list);

int Count(struct list_t* list){
	if(list == NULL)
		return -1;

	if (list->first == NULL){
		if(list->last != NULL)
			return CountBackwards(list);
		list->count = 0;

		return 0;
	}
	int c = 1;
	struct node* n = list->first;
	while(n->next != NULL){
		n=n->next;
		++c;
	}
	list->count = c;
	list->last = n;
	return c;
}

int CountBackwards(struct list_t* list){
	if(list == NULL)
		return -1;

	if (list->last == NULL){
		if (list->first != NULL)
			return Count(list);
		list->count = 0;
		return 0;
	}
	int c = 1;
	struct node* n = list->last;
	while(n->prev != NULL){
		n=n->prev;
		++c;
	}
	list->count = c;
	list->first = n;
	return c;
}

int Add(struct list_t* list, int data){
	if (list == NULL)
		return -1;

	struct node* n = CreateNode(data);
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
	list->sort = 0;
	return list->count - 1;
}


int AddBulk(struct list_t* list, int dataCount, int* datas){
	if (list == NULL || dataCount < 1 || datas == NULL)
		return -1;

	if (dataCount == 1)
		return Add(list, datas[0]);

	struct node* f = CreateNode(datas[0]);

	struct node* m;
	struct node* n = f;

	int i;
	for (i = 1; i < dataCount; i++){
		m = n;
		n = CreateNode(datas[i]);
		m->next = n;
		n->prev = m;
	}

	n->next = NULL;
	if (list->first == NULL){
		list->first = f;
		f->prev = NULL;
	}else{
		list->last->next = f;
		f->prev = list->last;
	}
	list->last = n;
	int c = list->count;
	list->count = c + dataCount;
	list->sort = 0;
	return c;
}

struct list_t* ListFromArray(int dataCount, int* datas){
	struct list_t* list = (struct list_t*)malloc(sizeof(struct list_t));
	list->sort = 0;
	list->count = 0;
	list->first = NULL;
	list->last = NULL;
	AddBulk(list, dataCount, datas);
	return list;
}

struct node* GetNode(struct list_t* list, int index){
	if(list == NULL || index < 0 || list->first == NULL || index >= list->count)
		return NULL;
	if (index == 0)
		return list->first;
	if (index == list->count -1)
		return list->last;
	struct node* n;
	if (2*index < list->count){
	n = list->first;

	int counter = index+1;
	while(--counter){
		n = n->next;
		if(n == NULL){
			Count(list);
			return NULL;
		}
	}
	}else{
		n = list->last;

	int counter = list->count - index;
	while(--counter){
		n = n->prev;
		if(n == NULL){
			Count(list);
			return NULL;
		}
	}
	}
	return n;
}

int Get(struct list_t* list, int index){
	struct node* n = GetNode(list, index);
	if (n == NULL)
		return 0;
	return n->data;
}

bool RemoveNode(struct list_t* list, struct node* n){

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
	free(n);
	list->sort = 0;
	return 1;
}

bool RemoveAt(struct list_t* list, int index){
	if (list == NULL || index < 0 || list->first == NULL || index >= list->count)
		return false;
	int c = list->count;
	struct node* n;
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
	n=GetNode(list, index);
	if (n == NULL)
		return false;
	n->prev->next = n->next;
	n->next->prev = n->prev;
	--list->count;
	free(n);
	list->sort = 0;
	return true;
}

int AddFirst(struct list_t* list, int data){
	if (list == NULL)
		return -1;
	if(list->first == NULL){

		return Add(list, data);
	}
	struct node* n = CreateNode(data);
	n->next = list->first;
	n->prev = NULL;
	list->first->prev = n;
	list->first = n;
	++list->count;
	list->sort = 0;
	return 0;
}

int Insert(struct list_t* list, int data, int index){
	if (list == NULL)
		return -1;
	if (list->count == 0){
		if (index == 0){
			return Add(list, data);
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
		return AddFirst(list, data);
	}
	struct node* n=CreateNode(data);
	struct node* before = GetNode(list, index-1);
	n->next = before->next;
	n->prev = before;
	before->next = n;
	n->next->prev=n;
	++list->count;
	list->sort = 0;
	return index;
}

int InsertAnyway(struct list_t* list, int data, int index){
	if (list == NULL)
		return -1;
	if (list->count == 0){
		return Add(list, data);
	}
	if (index >= list->count){
		return Add(list, data);
	}
	if(index < 0){
		return AddFirst(list, data);
	}


	if(index == 0){

		return AddFirst(list, data);
	}
	struct node* n=CreateNode(data);
	struct node* before = GetNode(list, index-1);
	n->next = before->next;
	n->prev = before;
	before->next = n;
	n->next->prev=n;
	++list->count;
	list->sort = 0;
	return index;
}

int AddBulkFirst(struct list_t* list, int dataCount, int* datas){
	if (list == NULL || dataCount < 1 || datas == NULL)
		return -1;
	if(list->first == NULL){
		AddBulk(list, dataCount, datas);
		return -1;
	}
	if(dataCount == 1){
		AddFirst(list, datas[0]);
		return -1;
	}

	struct node* f = CreateNode(datas[0]);
	f->prev = NULL;
	struct node* n = f;
	struct node* m;

	int i;
	for(i = 1; i < dataCount; ++i){
		m = n;
		n = CreateNode(datas[i]);
		n->prev = m;
		m->next = n;
	}

	f->prev = NULL;
	if(list->last == NULL){
		list->last = n;
		n->next = NULL;
	}else{
		n->next = list->first;
		list->first->prev = n;
	}
	list->first = f;
	list->count += dataCount;
	list->sort = 0;
	return 0;
}

int InsertBulk(struct list_t* list, int dataCount, int* datas, int index){
	if (list == NULL || dataCount < 1 || datas == NULL)
		return -1;
	if(dataCount == 1)
		return Insert(list, datas[0], index);
	if (list->count == 0){
		if (index == 0){
			return AddBulk(list, dataCount, datas);
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
		return AddBulkFirst(list, dataCount, datas);
	}
	list->sort = 0;
	struct node* after = GetNode(list, index);

	struct node* m = after->prev;
	struct node* n;
	list->count += dataCount;
	int i;
	for(i = 0; i < dataCount; ++i){
	   n=CreateNode(datas[i]);
	   m->next = n;
	   n->prev = m;
	   m = n;
	}
	n->next = after;


	n->next->prev=n;

	list->sort = 0;
	return index;
}
int InsertBulkAnyway(struct list_t* list, int dataCount, int* datas, int index){
	if (list == NULL || dataCount < 1 || datas == NULL)
		return -1;
	if(dataCount == 1)
		return InsertAnyway(list, datas[0], index);
	if (list->count == 0){
			return AddBulk(list, dataCount, datas);

	}
	if (index >= list->count){
		return AddBulk(list, dataCount, datas);
	}
	if(index < 0){
		return AddBulkFirst(list, dataCount, datas);
	}


	if(index == 0){
		return AddBulkFirst(list, dataCount, datas);
	}
	list->sort = 0;
	struct node* after = GetNode(list, index);

	struct node* m = after->prev;
	struct node* n;
	list->count += dataCount;
	int i;
	for(i = 0; i < dataCount; ++i){
	   n=CreateNode(datas[i]);
	   m->next = n;
	   n->prev = m;
	   m = n;
	}
	n->next = after;


	n->next->prev=n;

	list->sort = 0;
	return index;
}


int RemoveFirst(struct list_t* list, int data, int count){
	if (list == NULL || list->first == NULL || count < 1)
		return 0;
	int i = 0;
	struct node* n = list->first;
	while(i < count && n != NULL){
		if (n->data == data){
			struct node* m = n;
			n = n->next;
			i+=RemoveNode(list, m);

		}else
			n = n->next;
	}
	list->sort = 0;
	return i;
}



int RemoveLast(struct list_t* list, int data, int count){
	if (list == NULL || list->first == NULL || count < 1)
		return 0;
	int i = 0;
	struct node* n = list->last;
	while(i < count && n != NULL){
		if (n->data == data){
			struct node* m = n;
			n = n->prev;
			i+=RemoveNode(list, m);

		}else
			n = n->prev;
	}
	list->sort = 0;
	return i;
}

int RemoveAll(struct list_t* list, int data){
	if (list == NULL || list->first == NULL)
		return 0;

	int i = 0;
	struct node* n = list->first;
	while(n != NULL){
		if (n->data == data){
			struct node* m = n;
			n = n->next;

			i+=RemoveNode(list, m);


		}else{

			n = n->next;
		}
	}
	list->sort = 0;
	return i;
}

void Clear(struct list_t* list){
	if(list == NULL)
		return;
	if (list->first == NULL){
		if (list->last != NULL){
			CountBackwards(list);
			Clear(list);
			return;
		}
	}else if (list->last == NULL){
		Count(list);
		Clear(list);
		return;
	}

	struct node* n = list->first;
	struct node* m;
	while (n != NULL){
		m=n;
		n = n->next;
		free(m);
	}
	list->sort = 0;
	list->count = 0;
	list->first = NULL;
	list->last = NULL;
}

int RemoveRange(struct list_t* list, int first, int last){
	if (list == NULL || list->first == NULL || last < first || first < 0 || last < 0 || first >= list->count || last >= list->count)
		return 0;
	int delCount = last-first+1;
	if(delCount > list->count)
		return 0;
	if(delCount == 1)
		return RemoveAt(list, first);
	if (delCount == list->count){
		Clear(list);
		return !0;
	}
	list->sort = 0;
	struct node* before;
	struct node* n;
	if (first == 0){
		before = NULL;
		n = GetNode(list, first);
	}else{
		before = GetNode(list, first-1);
		n=before->next;
	}
	struct node* m = n;

	int i =delCount +1;
	while(--i){
		if(n == NULL){
			Count(list);
			return 0;
		}
		m = n;
		n = n->next;
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
	list->count -= delCount;
	return !0;
}

int CountWhere(struct list_t* list, int data){
	if (list == NULL)
		return 0;
	if(list->first == NULL){
		list->count = 0;
		return 0;
	}
	int i = 0;

	struct node* n = list->first;
	if(n->data == data)
		++i;
	int c = 1;
	while(n->next != NULL){
		n = n->next;
		if (n->data == data)
			++i;
		++c;
	}
	list->count = c;
	list->last = n;
	return i;
}

int FirstIndexOf(struct list_t* list, int data){
	if (list == NULL || list->first == NULL)
		return -1;
	struct node* n = list->first;

	int i = 0;
	while(n != NULL){
		if (n->data == data)
			return i;
		n = n->next;
		++i;
	}
	return -1;
}

int LastIndexOf(struct list_t* list, int data){
	if (list == NULL || list->last == NULL)
		return -1;
	struct node* n = list->last;

	int i = list->count-1;
	while(n != NULL){
		if (n->data == data)
			return i;
		n = n->prev;
		--i;
	}
	return -1;
}




void PrintList(struct list_t* list){
	int c = list->count;
	printf("\n\nCount = %d\n", c);
	int i;
	for (i = 0; i < c; i++){
		printf("%d, ", Get(list, i));
	}
	/*struct node* n = list->first;
	while(n!=NULL){
		printf("%d, ", n->data);
		n = n->next;
	}*/
	printf("\n\n");
}

int Pop(struct list_t* list){
	if(list == NULL || list->last == NULL)
		return 0;
	struct node* n = list->last;
	list->last = n->prev;
	if(list->last != NULL)
		list->last->next = NULL;
	--list->count;
	int ret = n->data;
	free(n);
	return ret;
}

int TryPop(struct list_t* list, int* out){
	if (list == NULL || list->count < 1 || list->first == NULL)
		return 0;
	*out = Pop(list);
	return !0;
}

int PopFirst(struct list_t* list){
	if(list == NULL || list->first == NULL)
		return 0;
	struct node* n = list->first;
	list->first = list->first->next;
	list->first->prev = NULL;
	int ret = n->data;
	free(n);
	--list->count;
	return ret;
}

int TryPopFirst(struct list_t* list, int* out){
	if(list == NULL || list->first == NULL)
		return 0;
	*out = PopFirst(list);
	return !0;

}

int SwitchNodes(struct list_t* list, struct node* a1, struct node* b1){
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
		struct node* b2 = b1->next;
		b1->next = a1->next;
		struct node* a0 = a1->prev;
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
	list->sort = 0;
	return !0;
}

void Sort(struct list_t* list, bool ascending){
	if(list == NULL || list->count < 2)
		return;
	int i, j, c=list->count;


	i = c;

	while(--i){

		struct node* n = list->first;
		for(j = 0; j < i; j++){

			int flip;
			if(ascending)
				flip = n->data > n->next->data;
			else
				flip = n->data < n->next->data;
			if(flip){

				int c = n ->data;
				n->data = n->next->data;
				n->next->data = c;
			}
			n = n->next;
		}
	}

	list->sort = ascending ? 1 : 2;
}

void Reverse(struct list_t* list){
	if (list == NULL || list->count < 2)
		return;

	struct node* a = list->first;
	struct node* z = list->last;
	int c = list->count/2 +1;
	while(--c){
		int t = a->data;
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

void ReverseNodes(struct list_t* list){
	if (list == NULL || list->count < 2)
		return;
	struct node* n = list->first;
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

struct list_t* JoinFirst(struct list_t* add, struct list_t* to, int freeIt){
	if(add == NULL || to == NULL)
		return NULL;
	if(add->count < 1)
		return to;
	if (to->count < 1){
		to->count = add->count;
		to->first = add->first;
		to->last = add->last;
		to->sort = add->sort;
	}else{
	add->last->next = to->first;
	to->first->prev = add->last;
	to->first = add->first;
	to->count +=add->count;
	to->sort = 0;
	}
	if(freeIt)
		free(add);
	else{
	add->count = 0;
	add->first = NULL;
	add->last = NULL;
	}
	return to;
}

struct list_t* JoinLast(struct list_t* add, struct list_t* to, int freeIt){
	if(add == NULL || to == NULL)
		return NULL;
	if(add->count < 1)
		return to;
	if (to->count < 1){
		to->count = add->count;
		to->first = add->first;
		to->last = add->last;
		to->sort = add->sort;
	}else{
		to->last->next = add->first;
		add->first->prev = to->last;
		to->last = add->last;
		to->count +=add->count;
		to->sort = 0;
	}
	if(freeIt)
		free(add);
	else{
		add->count = 0;
		add->first = NULL;
		add->last = NULL;
	}
	return to;
}

struct list_t* JoinAt(struct list_t* add, struct list_t* to, int index, int freeIt){
	if(add == NULL || to == NULL || index >= to->count)
		return NULL;
	if(index == 0)
		return JoinFirst(add, to, freeIt);
	if (index == to->count)
		return JoinLast(add, to, freeIt);
	if(add->count < 1)
		return to;
	if (to->count < 1){
		if(index == 0){
		to->count = add->count;
		to->first = add->first;
		to->last = add->last;
		to->sort = add->sort;
		}
	}else{

		struct node* after = GetNode(to, index);
		add->first->prev = after->prev;
		after->prev->next = add->first;
		add->last->next = after;
		after->prev = add->last;

	to->count +=add->count;
	to->sort = 0;
	}
	if(freeIt)
		free(add);
	else{
	add->count = 0;
	add->first = NULL;
	add->last = NULL;
	}
	return to;
}

struct list_t* JoinAtAnyway(struct list_t* add, struct list_t* to, int index, int freeIt){
	if(add == NULL || to == NULL)
		return NULL;
	if(index == 0)
		return JoinFirst(add, to, freeIt);
	if (index >= to->count)
		return JoinLast(add, to, freeIt);
	if(add->count < 1)
		return to;

	if (to->count < 1){
		to->count = add->count;
		to->first = add->first;
		to->last = add->last;
		to->sort = add->sort;

	}else{

		struct node* after = GetNode(to, index);
		add->first->prev = after->prev;
		after->prev->next = add->first;
		add->last->next = after;
		after->prev = add->last;


	to->count +=add->count;
	to->sort = 0;
	}
	if(freeIt)
		free(add);
	else{
	add->count = 0;
	add->first = NULL;
	add->last = NULL;
	}
	return to;
}

void Destroy(struct list_t* list){
	Clear(list);
	free(list);
}

int main(){
	int arr1[] = {3,1,2,5,6,9,0};
	struct list_t* list = ListFromArray(7, arr1);
	printf("Start list");
	PrintList(list);

	printf("Adding '-1' at the end of the list");
	Add(list,-1);
	PrintList(list);

	printf("Removing item at index = 3 %s", RemoveAt(list, 3) ? "succeeded" : "failed");

	PrintList(list);
	printf("Removing items from index = 3 to 4 %s", RemoveRange(list, 3, 4) ? "succeeded" : "failed");

	PrintList(list);

	printf("Popping last item : %d", Pop(list));
	PrintList(list);

	printf("Popping first item : %d", PopFirst(list));
	PrintList(list);

	printf("Inserting '4' at index = 0 %s", AddFirst(list, 4) > -1 ?"succeeded":"failed");
	PrintList(list);

	printf("Inserting '13' at index = 3 %s", Insert(list, 13, 3) > -1 ? "succeeded" : "failed");
	PrintList(list);


	printf("Clearing list");
	Clear(list);
	PrintList(list);

	printf("Bulk adding '5,8,2,7,8'");
	int arr2[] = {5,8,2,7,8};
	AddBulk(list, 5, arr2);
	PrintList(list);

	printf("Bulk inserting '4,1,2' at front");
	int arr3[] = {4,1,2};
	AddBulkFirst(list, 3, arr3);
	PrintList(list);

	int arr4[] = {7,-4,-1};
	printf("Bulk inserting '7,-4,-1' starting at index = 3 %s", InsertBulkAnyway(list, 3, arr4, 3)?"succeeded":"failed");
	PrintList(list);

	printf("First index of '2' is %d\n\n", FirstIndexOf(list, 2));
	printf("Last index of '2' is %d\n\n",LastIndexOf(list, 2));

	printf("Removing all '2'. Removed : %d", RemoveAll(list, 2));
	PrintList(list);

	printf("Switching node at index = 1 with node at index = 3");
	SwitchNodes(list, GetNode(list, 1), GetNode(list, 3));
	PrintList(list);

	printf("Reversing list");
	Reverse(list);
	PrintList(list);

	printf("Reversing list");
	Reverse(list);
	PrintList(list);

	printf("Sorting list ascending");
	Sort(list, true);
	PrintList(list);

	printf("Sorting list descending");
	Sort(list, false);
	PrintList(list);

	printf("Removing list from third item to last item");
	RemoveRange(list, 3, list->count-1);
	PrintList(list);

	printf("Creating three new list\n\nList 2");
	int arr5[4] = {5,2,4,6};
	struct list_t* list2 = ListFromArray(4, arr5);
	PrintList(list2);
	printf("List 3");
	int arr6[3] = {-4,-1,6};
	struct list_t* list3 = ListFromArray(3, arr6);
	PrintList(list3);
	printf("List 4");
	int arr7[2] = {15, 0};
	struct list_t* list4 = ListFromArray(2, arr7);
	PrintList(list4);

	printf("Joining List 2 to main list, at index = 0");
	JoinFirst(list2, list, true);
	PrintList(list);


	printf("Joining List 3 to main list, at the end of list");
	JoinLast(list3, list, true);
	PrintList(list);


	printf("Joining List 4 to main list, at index = 4");
	JoinAtAnyway(list4, list, 4, false);
	PrintList(list);

	return 0;
}
