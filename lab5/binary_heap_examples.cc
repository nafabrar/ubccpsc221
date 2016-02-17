#include <iostream>

using namespace std;

/**
 * PRE:  The capacity of the array pointed to by heap is at least size.
 * POST: The first size elements of heap are printed to the screen.
 */
void printList(int* heap, int size) {
	for (int i = 0; i < size; i++)
		cout << heap[i] << " ";
	cout << endl;
}

/**
 *  PRE:  subtrees rooted at leftChild and rightChild of i are heaps.
 *  POST: subtree rooted at i is a heap.
 */
void swapDown(int* heap, int i, int size) {
	int leftChild = 2 * i + 1;
	int rightChild = 2 * i + 2;
	int min = i;
	// find who holds smallest element of i and its two children
	if (leftChild < size && heap[leftChild] < heap[min])
		min = leftChild;
	if (rightChild < size && heap[rightChild] < heap[min])
		min = rightChild;
	// if a child holds smallest element, swap i's element to that child
	// and recurse.
	if (min != i) {
		swap(heap[i], heap[min]);
		swapDown(heap, min, size);
	}
}

/**
 * PRE:  The capacity of the array pointed to by heap is at least size.
 * POST: The first size elements of heap are a heap.
 */
void heapify(int* heap, int size) {
	for (int i = (size - 2) / 2; i >= 0; i--)
		swapDown(heap, i, size);
}

/**
 * PRE:  The capacity of the array pointed to by heap is at least size.
 * POST: The first size elements of heap are sorted in descending order.
 */
void sort(int* heap, int size) {
	heapify(heap, size);  // Heapify algorithm
	for (int i = size - 1; i > 0; i--) {
		swap(heap[0], heap[i]);
		swapDown(heap, 0, i);
	}
}

