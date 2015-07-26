#ifndef List_
#define List_
#include "ListUtils.h"
#include<stdio.h>
#include<stdlib.h>



struct List{
	void *data;
	ListRef next;

};

typedef struct List List;


ListRef newList(void* headData){
	ListRef result = (ListRef)malloc(sizeof(List));  //allocation of the new list (pointer to the list)
	if (result == NULL)  //if malloc fails, return NULL
		return NULL;
	result->data = headData;   //update the new list's head to be with the new headData. if headData is NULL we get an empty list!
	result->next = NULL;
	return result;
}

void* headData(ListRef list){
	if (list == NULL){
		return NULL;
	}
	return (list->data);
}


ListRef tail(ListRef list){
	if (list == NULL || list->next == NULL)
		return NULL;
	else{
		return (list->next);
	}
}

void destroyNext(ListRef list){
	list->next = NULL;
}


ListRef append(ListRef list, void* data){
	if (data == NULL)
		return NULL;
	if (isEmpty(list)){     //if the list is empty, update the head to be with the data given.
		list->data = data;
		return list;
	}
	ListRef newNode = newList(data);
	if (newNode == NULL)    //if malloc fails, return NULL.
		return NULL;
	ListRef cur = list;
	if (cur == NULL)    //if the list is NULL, return NULL.
		return NULL;
	while (cur->next != NULL)     //we want cur to be the last node of the list
		cur = cur->next;
	cur->next = newNode;  //put the new node after cur (the last node)
	return newNode;
}

ListRef concat(ListRef list1, ListRef list2){
	if (list1 == NULL)
		return list2;
	if (list2 == NULL)
		return list1;
	ListRef cpy;
	if (isEmpty(list1)){
		list1->data = list2->data;
		list2 = tail(list2);
	}
	cpy = list1;
	while (tail(cpy) != NULL)
		cpy = tail(cpy);
	cpy->next = list2;
	return list1;
}

int isEmpty(ListRef list){
	return (list->data == NULL && list->next == NULL);
}

int size(ListRef list){
	int s;
	ListRef cpy;

	s = 0;
	cpy = list;
	while (cpy != NULL&&!isEmpty(cpy)){
		cpy = tail(cpy);
		s++;
	}
	return s;
}


#endif
/////