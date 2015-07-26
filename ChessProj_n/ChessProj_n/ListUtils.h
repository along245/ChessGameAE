#ifndef LISTUTILS_H_
#define LISTUTILS_H_

/**
* The List declaration - actual structure is left up to the implementer.
*/
struct List;
typedef struct List* ListRef;

/**
* Signature of a resource deallocation function.
*/
typedef void(*FreeFunc)(void* data);

/**
* Allocate a new list with the given data pointer at the head.
* The list does not support NULL data - in this case if the data is NULL then an empty list will be created.
*
* May return NULL if malloc fails.
*/
ListRef newList(void* headData);

/**
* Return the data pointer held by the head of the list or NULL if the list is empty.
*/
void* headData(ListRef list);

/**
* Return the tail of the given list or NULL if the list has less than 2 elements.
* E.g. if the list holds the addresses [1,2,3] then tail(list) will return the sub-list [2,3].
*/
ListRef tail(ListRef list);

/**
* Append the given data to the end of the list and return a pointer to the sub-list containing only this data.
* If the given data is NULL no change will be made to the list and NULL will be returned.
* E.g. if the list holds the addresses [1,2,3] then after calling append(list, (void*)4) the list will hold [1,2,3,4]
* May return NULL if malloc fails.
*/
ListRef append(ListRef list, void* data);


/** taking two lists and adding each element from the second list to the first list*/
ListRef concat(ListRef list1, ListRef list2);
int size(ListRef list);//calculates number of elements.
/**
* Return zero if the list has any data or non-zero if it is empty.
**/
int isEmpty(ListRef list);

//kills the list's next

void destroyNext(ListRef list);

#endif /* LISTUTILS_H_ */





