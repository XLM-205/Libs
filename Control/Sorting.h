/*-------Sorting Collection-------
*	WIP
*	
*
*
*	Moon Wiz Studios (c) - 16/08/2015
*	by: Ramon Darwich de Menezes
*
*
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 0.0.0
*/

#ifndef _H_MWSORT_
#define _H_MWSORT_

typedef struct queue
{
	int *data;
	int pointer;
	int max;
	int front;
};

void initQueue(queue *q, int size)
{
	q->data = new int[size];
	q->max = size;
	q->pointer = q->front = 0;
}

void destroyQueue(queue *q)
{
	if (q->data)
	{
		delete[] q->data;
	}
}

int empty(queue *q)
{
	return !q->pointer;
}

int remove(queue *q)
{
	if (!empty(q))
	{
		int temp = q->data[q->front++];
		q->pointer--;
		if (empty(q))
		{
			q->front = 0;	//If the queue became empty on this remove, reset the front pointer to prevent future leakage
		}
		return temp;
	}
}

void insert(queue *q, int val)
{
	if (q->pointer < q->max)
	{
		q->data[q->pointer++] = val;
		if (q->front > 0)
		{
			q->front--;
		}
	}
}

#ifdef _H_CSTTYPE_	//Custom Types Library. If undefined, recreate XORSwap and uint32 typedef
using namespace BitOperations;
#else
typedef unsigned int uint32;

template <typename T, typename T2>
inline void XORSwap(T &A, T2 &B)
{
	if (&A != &B)
	{
		A ^= B;
		B ^= A;
		A ^= B;
	}
}

#endif

template<typename T>
void MergeArray(T *v, uint32 left, uint32 middle, uint32 right)
{
	uint32 i = 0, j = 0, k = left;	//Initial indexes for the first array, the second array and the merged one
	uint32 end1 = middle - left + 1;
	uint32 end2 = right - middle;
	
	T *tempL = new T[end1];
	T *tempR = new T[end2];

	while (i < end1 && j < end2)
	{

	}

	delete[] tempL;
	delete[] tempR;
	tempL = tempR = nullptr;
}

template<typename T>
void heapify(T *v, uint32 index, uint32 size)
{
	int high = index;			//Index of the largest value of a subtree. Init as the root
	int left = (2 * index) + 1;	//Left node of this index [(2 * i) + 1]
	int right = left + 1;		//Right node of this index [(2 * i) + 1] OR [left + 1]
	//Checking both sides to see who is greater. If not the root, we will change it
	if (left < size && v[left] > v[high])
	{
		high = left;
	}
	if (right < size && v[right] > v[high])
	{
		high = right;
	}
	if (high - index)				//If the largest value isn't the root...
	{
		XORSwap(v[index], v[high]);	//...then swap them!
		heapify(v, high, size);		//...and keep doing it until the root becomes the largest value!
	}
}

namespace Sorting
{
	//Sorting algorhtims sorted by complexity from the slowest to faster
	template<typename T>
	void Bubble(T *v, uint32 size)
	{
		uint32 stop = size - 1, swaps;
		for (uint32 i = 0; i < size; ++i, --stop)
		{
			swaps = 0;
			for (uint32 j = 0; j < stop; ++j)
			{
				if (v[j] > v[j + 1])
				{
					XORSwap(v[j], v[j + 1]);
					swaps++;
				}
			}
			if (!swaps)		//If no swaps have been done, them the array is sorted
			{
				return;
			}
		}
	}

	template<typename T>
	void Selection(T *v, uint32 size)
	{
		T *low = nullptr;
		for (uint32 i = 0; i < size; ++i)
		{
			low = &v[i];
			for (uint32 j = i + 1; j < size; ++j)
			{
				if (*low > v[j])
				{
					low = &v[j];
				}
			}
			XORSwap(v[i], *low);
		}
	}

	template<typename T>
	void Insertion(T *v, uint32 size)
	{
		uint32 lastSorted = 0;
		T *extracted = nullptr;
		while (lastSorted - (size - 1)) //While the last sorted item is not the last possible index... (lastSorted < (size - 1))
		{
			extracted = &v[lastSorted + 1];				//Extract the next unsorted element
			for (int i = lastSorted++; i >= 0; --i)		//Go from the last sorted element to 0... When we finish this loop, increment the last sorted element by 1
			{
				if (v[i] > *extracted)					//If it's lower, swap it
				{
					XORSwap(v[i], *extracted--);		//Swap it with the extracted and them  move the extracted back one space (so we don't lose the extracted value)
					//Note: this method works because at (i0), v[i] == v[i + 1] or 'extracted'
				}
				else								//If not, break the loop. From here to back it's sorted
				{
					break;
				}
			}
		}
	}

	//Explanation:	https://www.youtube.com/watch?v=SHcPqUe2GZM
	//				https://www.geeksforgeeks.org/shellsort/
	template<typename T>
	void Shell(T *v, uint32 size)
	{
		for (int gap = size >> 1; gap > 0; gap >>= 1)			//Create a 'gap' and progressively reduce it by 2 each time
		{
			for (int i = gap; i < size; i++)
			{
				int temp = v[i];				
				int j = i;
				for (; j >= gap && v[j - gap] > temp; j -= gap)	//For each value to the right of 'gap' that is bigger than it, swap it with the oposite of it (j - gap)
				{
					v[j] = v[j - gap];
				}
				v[j] = temp;
			}
		}
	}

	void Radix(int *v, uint32 size)
	{
		queue Buckets[10];
		int *copy = new int[size];
		int vals = 0, digitCount = 1;
		for (int i = 0; i < 10; i++)
		{
			initQueue(&Buckets[i], size);
		}
		for (int i = 0; i < size; i++)
		{
			copy[i] = v[i];
		}
		//Now we will divide each of the 'copy' by 10 to get the digit. Since they are integers, the least value they can reach is 0. If all of them is 0, we stop the sorting
		do {
			vals = 0;
			for (int i = 0; i < size; i++)
			{
				insert(&Buckets[copy[i] % 10], v[i]);
			}
			for (int i = 0, j = 0; i < 10; i++)
			{
				while (!empty(&Buckets[i]))
				{
					v[j] = remove(&Buckets[i]);
					copy[j] = v[j];
					copy[j] /= (10 * digitCount);
					vals += copy[j];
					j++;
				}
			}
			digitCount *= 10;
		} while (vals);
		for (int i = 0; i < 10; i++)
		{
			destroyQueue(&Buckets[i]);
		}
	}

	template<typename T>
	void Heap(T *v, uint32 size)
	{
		//Heap sort basically creates a binary tree
		//First step is to prepare the array into a tree structure
		//			[x]	
		//			(A)	
		//		   /   \
		//		 (B)   (C) 	
		//	  [2*x+1] [2*x+2]	 	
		//	-Where A > B && A > C and 'x' is the index of this node in the tree-array
		for (int i = (size >> 1) - 1; i >= 0; --i)
		{
			heapify(v, i, size);
		}
		//Second step is to extract each element from the heap...
		for (int i = size - 1; i >= 0; --i)
		{
			XORSwap(v[0], v[i]);	//...by moving the current root to the end of the array...
			heapify(v, 0, i);		//...and "max heaping" the heap
		}
	}

	template<typename T>
	void Merge(T *v, uint32 size)
	{

	}

	template<typename T>
	void Merge(T *v, uint32 left, uint32 right)		//This algorithm simply splits an array in subparts, feeding the 'MergeArray' that actually does the sorting
	{
		if (left < right)
		{
			uint32 middle = (left - right) >> 1;	//Equivalent of dividing by 2
			Merge(v, left, middle);
			Merge(v, middle + 1, right);
			MergeArray(v, left, middle, right);
		}
	}

	void Counting(int *v, uint32 size, uint32 highest)
	{
		int *count;
		int stop = highest;
		if (size > highest)
		{
			count = new int[size]();
			stop = size;
		}
		else
		{
			count = new int[++stop]();
		}
		for (int i = 0; i < size; ++i)
		{
			count[v[i]]++;
		}
		for (int i = 0, j = 0; i < stop; ++i)
		{
			while (count[i])
			{
				v[j++] = i;
				--count[i];
			}
		}
		delete[] count;
	}

	void Counting(int *v, uint32 size)
	{
		int high = 0;
		for (int i = 0; i < size; i++)
		{
			if (v[i] > high)
			{
				high = v[i];
			}
		}
		Counting(v, size, high);
	}
}

#endif