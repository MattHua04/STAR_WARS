#include <stdio.h>
#include <stdlib.h>
#include "doublyLinkedList.hpp"

LLNode* createLlnode(void* data) {
  // Ensure pointer to data is valid
  if (data == NULL) {
    printf("\tInvalid input parameter. Cannot create node with NULL data.\n");
    return NULL;
  }

  LLNode* newNode = (LLNode*)malloc(sizeof(LLNode));
  // Ensure proper memory allocation for node
  if (newNode == NULL) {
    printf("\tcreateLlnode: Malloc failed. Exiting.\n");
    exit(1);
  }
  // Initialize node attributes
  newNode->data = data;
  newNode->prev = NULL;
  newNode->next = NULL;
  return newNode;
}

DLinkedList* createDlinkedlist(void) {
  DLinkedList* newList = (DLinkedList*)malloc(sizeof(DLinkedList));
  // Ensure proper allocation of memory for list
  if (newList == NULL) {
    printf("\tcreateDlinkedlist: Malloc failed. Exiting.\n");
    exit(1);
  }
  // Initialize list attributes
  newList->head = NULL;
  newList->tail = NULL;
  newList->size = 0;
  return newList;
}

int getSize(DLinkedList* dLinkedList) { return dLinkedList->size; }

LLNode* getHead(DLinkedList* dLinkedList) { return dLinkedList->head; }

LLNode* getTail(DLinkedList* dLinkedList) { return dLinkedList->tail; }

LLNode* getNext(LLNode* node) { return node->next; }

LLNode* getPrev(LLNode* node) { return node->prev; }

void* getData(LLNode* node) { return node->data; }

// Reports whether a node is within a doubly linked list
int nodeExists(DLinkedList* dLinkedList, LLNode* node) {
  LLNode* currentNode = getHead(dLinkedList);
  while (currentNode) {
    // Compare the attributes of each node in the doubly linked list to the provided node
    if (getData(currentNode) == getData(node) &&
        getPrev(currentNode) == getPrev(node) &&
        getNext(currentNode) == getNext(node)) {
      return 1;
    }
    currentNode = getNext(currentNode);
  }
  return 0;
}

void insertAfter(DLinkedList* dLinkedList, LLNode* prevNode, void* data) {
  // Ensure that the doubly linked list is valid
  if (dLinkedList == NULL) {
    printf(
        "\tInvalid input parameters. Cannot insert node into NULL doubly "
        "linked list.\n");
    return;
  }
  // Ensure that the reference node is valid
  if (prevNode == NULL) {
    printf("\tInvalid input parameters. Cannot insert after NULL node.\n");
    return;
  }
  // Ensure that the reference node exists
  if (!nodeExists(dLinkedList, prevNode)) {
    printf("\tInvalid input parameters. PrevNode not in doubly linked list.\n");
    return;
  }
  // Ensure that the data is valid
  if (data == NULL) {
    printf("\tInvalid input parameters. Cannot insert node with NULL data.\n");
    return;
  }
  // Create a node and update surrounding nodes
  LLNode* newNode = createLlnode(data);
  newNode->prev = prevNode;
  newNode->next = getNext(prevNode);
  // Check if new node should be new tail
  if (getNext(newNode) == NULL) {
    dLinkedList->tail = newNode;
  } else {
    getNext(newNode)->prev = newNode;
  }
  prevNode->next = newNode;
  dLinkedList->size++;
}

void insertBefore(DLinkedList* dLinkedList, LLNode* nextNode, void* data) {
  // Ensure that the doubly linked list is valid
  if (dLinkedList == NULL) {
    printf(
        "\tInvalid input parameters. Cannot insert node into NULL doubly "
        "linked list.\n");
    return;
  }
  // Ensure that the reference node list is valid
  if (nextNode == NULL) {
    printf("\tInvalid input parameters. Cannot insert before NULL node.\n");
    return;
  }
  // Ensure that the reference node exists
  if (!nodeExists(dLinkedList, nextNode)) {
    printf("\tInvalid input parameters. PrevNode not in doubly linked list.\n");
    return;
  }
  // Ensure that the data is valid
  if (data == NULL) {
    printf("\tInvalid input parameters. Cannot insert node with NULL data.\n");
    return;
  }
  // Create a node and update surrounding nodes
  LLNode* newNode = createLlnode(data);
  newNode->next = nextNode;
  newNode->prev = getPrev(nextNode);
  // Check if new node should be new head
  if (getPrev(newNode) == NULL) {
    dLinkedList->head = newNode;
  } else {
    getPrev(newNode)->next = newNode;
  }
  nextNode->prev = newNode;
  dLinkedList->size++;
}

void insertHead(DLinkedList* dLinkedList, void* data) {
  // Ensure that the doubly linked list is valid
  if (dLinkedList == NULL) {
    printf(
        "\tInvalid input parameters. Cannot insert head node into NULL doubly "
        "linked list.\n");
    return;
  }
  // Ensure that the data is valid
  if (data == NULL) {
    printf(
        "\tInvalid input parameters. Cannot insert head node with NULL "
        "data.\n");
    return;
  }
  /** 
   * Ensure that the doubly linked list doesn't have a head before inserting a head,
   * otherwise just insert a new node before the current head
  */
  if (dLinkedList->head == NULL) {
    LLNode* newNode = createLlnode(data);
    dLinkedList->head = newNode;
    dLinkedList->tail = newNode;
    dLinkedList->size++;
  } else {
    insertBefore(dLinkedList, getHead(dLinkedList), data);
  }
}

void insertTail(DLinkedList* dLinkedList, void* data) {
  // Ensure that the doubly linked list is valid
  if (dLinkedList == NULL) {
    printf(
        "\tInvalid input parameters. Cannot insert tail node into NULL doubly "
        "linked list.\n");
    return;
  }
  // Ensure that the data is valid
  if (data == NULL) {
    printf(
        "\tInvalid input parameters. Cannot insert tail node with NULL "
        "data.\n");
    return;
  }
  /** 
   * Ensure that the doubly linked list doesn't have a tail before inserting a tail,
   * otherwise just insert a new node after the current tail
  */
  if (dLinkedList->tail == NULL) {
    LLNode* newNode = createLlnode(data);
    dLinkedList->head = newNode;
    dLinkedList->tail = newNode;
    dLinkedList->size++;
  } else {
    insertAfter(dLinkedList, getTail(dLinkedList), data);
  }
}

void deleteNode(DLinkedList* dLinkedList, LLNode* node) {
  // Ensure that the doubly linked list is valid
  if (dLinkedList == NULL) {
    printf(
        "\tInvalid input parameters. Cannot delete node from NULL doubly "
        "linked list.\n");
    return;
  }
  // Ensure that the reference node list is valid
  if (node == NULL) {
    printf("\tInvalid input parameters. Cannot delete NULL node.\n");
    return;
  }
  // Ensure that the reference node list exists
  if (!nodeExists(dLinkedList, node)) {
    printf("\tInvalid input parameters. Node not in doubly linked list.\n");
    return;
  }
  // Update the head if node to be deleted is currently the head
  if (getPrev(node) == NULL) {
    dLinkedList->head = getNext(node);
  } else {
    getPrev(node)->next = getNext(node);
  }
  // Update the tail if node to be deleted is currently the tail
  if (getNext(node) == NULL) {
    dLinkedList->tail = getPrev(node);
  } else {
    getNext(node)->prev = getPrev(node);
  }
  // Deallocate node memory
  free(node->data);
  free(node);
  dLinkedList->size--;
}

void destroyList(DLinkedList* dLinkedList) {
  // Ensure that the doubly linked list is valid
  if (dLinkedList == NULL) {
    printf(
        "\tInvalid input parameter. Cannot destroy NULL doubly linked list.\n");
    return;
  }
  // Loop through the doubly linked list and delete each node
  LLNode* currentNode = getHead(dLinkedList);
  while (currentNode) {
    LLNode* next_node = getNext(currentNode);
    deleteNode(dLinkedList, currentNode);
    currentNode = next_node;
  }
  // Finally deallocate the doubly linked list itself
  free(dLinkedList);
}

void reverse(DLinkedList* dLinkedList) {
  // Ensure that the doubly linked list is valid
  if (dLinkedList == NULL) {
    printf(
        "\tInvalid input parameter. Cannot reverse NULL doubly linked list.\n");
    return;
  }
  // Loop through the doubly linked list and flip the prev and next of each node
  LLNode* currentNode = getHead(dLinkedList);
  LLNode* temp;
  while (currentNode) {
    temp = getPrev(currentNode);
    currentNode->prev = getNext(currentNode);
    currentNode->next = temp;
    currentNode = currentNode->prev;
  }
  // Finally flip the head and tail of the doubly linked list
  temp = getHead(dLinkedList);
  dLinkedList->head = getTail(dLinkedList);
  dLinkedList->tail = temp;
}
