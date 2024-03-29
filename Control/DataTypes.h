/*-------Abstract Data Types-------
*	Defines abstract types of data such as lists and stacks
*
*
*	Moon Wiz Studios (c) - 05/03/2018
*	by: Ramon Darwich de Menezes
*
*
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 1.0.8
*/

#ifndef _H_DATA_TYPES_
#define _H_DATA_TYPES_

#include "CustomTypes-SA.h"

#ifdef DATA_TYPES_FOREACH											
	//'Foreach' hack for NOT POINTER list
	#define inTheList(forEachObject, ofTheList) for(auto forEachObject = ofTheList.RetrieveFirst(); forEachObject; forEachObject = forEachObject->getNext())
	//'Foreach' hack for POINTER list
	#define inTheListP(forEachObject, ofTheList) for(auto forEachObject = ofTheList->RetrieveFirst(); forEachObject; forEachObject = forEachObject->getNext())
	//Usage Examples:
	//
	//	List<List<int>> Tester;
	//	inTheList(listJ, Tester)
	//	{
	//		inTheListP(integer, listJ->getData())
	//		{
	//			printf("[%3d]", *integer->getData());
	//		}
	//		printf("\n");
	//	}
#endif

enum StackMode
{
	STACK_FIFO,		//First-In-First-Out *(Queue)
	STACK_LIFO		//Last-In-First-Out *(Stack)
};

enum AutomataAdvance
{
	ADVANCE_LEFT = -1,	//Advance the tape to left
	ADVANCE_NO,			//Don't advance the tape at all
	ADVANCE_RIGHT		//Advance the tape to the right
};

template <typename T>
class Node
{
protected:

	T *m_Data;
	Node *m_Next;
	Node *m_Prev;

public:

	Node() : m_Next(nullptr), m_Prev(nullptr), m_Data(nullptr) { };
	Node(T* dt) : m_Next(nullptr), m_Prev(nullptr), m_Data(dt) { };
	~Node()
	{
		m_Next = m_Prev = nullptr;
		if (m_Data)
		{
			delete m_Data;
			m_Data = nullptr;
		}
	}

	bool Match(const T ref)
	{
		return *m_Data == ref;
	}
	bool Match(const T *ref)
	{
		return *m_Data == *ref;
	}

	void setData(T* dt)
	{
		if (m_Data)	//If there's already data, delete it and replace it
		{
			delete m_Data;
		}
		m_Data = dt;
	}
	void setNext(Node *nxt)
	{
		m_Next = nxt;
	}
	void setPrevious(Node *pvs)
	{
		m_Prev = pvs;
	}

	T* getData(void)
	{
		return m_Data;
	}
	Node* getNext(void)
	{
		return m_Next;
	}
	Node* getPrevious(void)
	{
		return m_Prev;
	}

	void deleteData(void)
	{
		if (m_Data)
		{
			delete m_Data;
			m_Data = nullptr;
		}
	}
	T* releaseData(void)
	{
		T* ret = m_Data;
		m_Data = nullptr;
		return ret;
	}
};

//List<Type>
//	*Represents a Double Linked List (Not Circular)
template <typename T>
class List
{
private:

	Node<T> *m_Head;
	Node<T> *m_Tail;
	unsigned int m_Count;

	void Destroy(void)
	{
		if (m_Head)
		{
			Node<T> *Temp = m_Head;
			Node<T> *TempN;
			do {
				TempN = Temp->getNext();
				delete Temp;
				Temp = TempN;
			} while (Temp);
			m_Count = 0;
			m_Head = m_Tail = nullptr;
		}
	}
	void FreeNode(Node<T> *n)
	{
		n->releaseData();
		Delete(n);
	}
	void Delete(Node<T> *n)
	{
		if(m_Count == 1)					//We only have 1 node, that means m_Head and m_Tail are the same. Just delete 'n' and reset it
		{
			m_Head = m_Tail = nullptr;
		}
		else if(n == m_Head)				//Requesting to delete the Head of the list
		{
			m_Head = m_Head->getNext();
			m_Head->setPrevious(nullptr);
		}
		else if(n == m_Tail)				//Requesting to delete the Tail of the list
		{
			m_Tail = m_Tail->getPrevious();
			m_Tail->setNext(nullptr);
		}
		else								//Requesting to delete any other node of the list
		{
			n->getNext()->setPrevious(n->getPrevious());
			n->getPrevious()->setNext(n->getNext());
		}
		delete n;
		m_Count--;
	}

public:

	List() : m_Head(nullptr), m_Tail(nullptr), m_Count(0) { };
	List(T *data)
	{
		m_Head = m_Tail = nullptr;
		m_Count = 0;
		Add(data);
	}
	List(T *data, int size)
	{
		m_Head = m_Tail = nullptr;
		m_Count = 0;
		Add(data, size);
	}
	~List()
	{
		Destroy();
	}

	bool Contains(Node<T> *n)
	{
		if (n)
		{
			Node<T>* Header = m_Head;
			Node<T>* Tailer = m_Tail;
			while (true)
			{
				if (Header == n || Tailer == n)
				{
					return true;
				}
				//We reached the end of our list!
				else if (Header == Tailer || (Header == Tailer->getNext() || Tailer == Header->getPrevious()))
				{
					return false;
				}
				Header = Header->getNext();
				Tailer = Tailer->getPrevious();
			}
		}
		return false;
	}
	bool Contains(const T ref)
	{
		if (!m_Count)
		{
			return false;
		}
		Node<T>* Header = m_Head;
		Node<T>* Tailer = m_Tail;
		while (true)
		{
			if (Header->Match(ref) || Tailer->Match(ref))
			{
				return true;
			}
			//We reached the end of our list!
			else if (Header == Tailer || Header == Tailer->getNext())
			{
				return false;
			}
			Header = Header->getNext();
			Tailer = Tailer->getPrevious();
		}
	}

	unsigned int Count(void)
	{
		return m_Count;
	}

	//"Get" Family
	//	-> Returns the pointer for the object created residing inside the found Node<T>. Shortcut for Retrieve()->getData()
	//Gets the first item that matches 'ref', be it from the end or the beginning of the list
	T* GetThis(const T ref) const
	{
		Node<T> *temp = RetrieveThis(ref);
		if (temp)
		{
			return temp->getData();
		}
		return nullptr;
	}
	//Gets the nth item that matches 'ref'. If the nth item wasn't found, the CLOSEST will be returned instead
	T* GetThis(const T ref, unsigned int nth, bool fromStart) const
	{
		Node<T> *temp = RetrieveThis(ref, nth, fromStart);
		if (temp)
		{
			return temp->getData();
		}
		return nullptr;
	}
	//Gets the nth item that matches 'ref'. If the nth item wans't found, NOTHING will be returned
	T* GetThisPrecisely(const T ref, unsigned int nth, bool fromStart) const
	{
		Node<T> *temp = RetrieveThisPrecisely(ref, nth, fromStart);
		if (temp)
		{
			return temp->getData();
		}
		return nullptr;
	}
	T* GetAt(unsigned int index) const
	{
		Node<T> *temp = RetrieveAt(index);
		if (temp)
		{
			return temp->getData();
		}
		return nullptr;
	}
	T* First(void) const
	{
		if (m_Head)
		{
			return m_Head->getData();
		}
		return nullptr;
	}
	T* Last(void) const
	{
		if (m_Tail)
		{
			return m_Tail->getData();
		}
		return nullptr;
	}

	//"Index" Family
	//	-> Returns the index of searched item on the list, returning -1 if unable
	int IndexOf(Node<T> *n)
	{
		if (n)
		{
			Node<T>* Header = m_Head;
			Node<T>* Tailer = m_Tail;
			int upDex = 0, dnDex = m_Count - 1;	//Upper Index and Downward Index
			while (true)
			{
				if (Header == n)
				{
					return upDex;
				}
				else if (Tailer == n)
				{
					return dnDex;
				}
				//We reached the end of our list!
				else if (Header == Tailer || (Header == Tailer->getNext() || Tailer == Header->getPrevious()))
				{
					return -1;
				}
				Header = Header->getNext();
				Tailer = Tailer->getPrevious();
				upDex++;
				dnDex--;
			}
		}
		return -1;
	}
	//Finds the index of the first item that matches 'ref', be it from the end or the beginning of the list
	int IndexOf(const T ref)
	{
		if (!m_Count)
		{
			return -1;
		}
		Node<T>* Header = m_Head;
		Node<T>* Tailer = m_Tail;
		int upDex = 0, dnDex = m_Count - 1;
		while (true)
		{
			if (Header->Match(ref))
			{
				return upDex;
			}
			else if (Tailer->Match(ref))
			{
				return dnDex;
			}
			//We reached the end of our list!
			else if (Header == Tailer || Header == Tailer->getNext())
			{
				return -1;
			}
			Header = Header->getNext();
			Tailer = Tailer->getPrevious();
			upDex++;
			dnDex--;
		}
	}
	//Finds the index of the nth item that matches 'ref'. If the nth item wasn't found, the CLOSEST will be retrieved instead
	int IndexOf(const T ref, unsigned int nth, bool fromStart)
	{
		if (!m_Count)
		{
			return -1;
		}
		Node<T> *runner = nullptr;
		int upDex = 0, dnDex = m_Count - 1, index = -1;
		if (fromStart)
		{
			runner = m_Head;
			while (runner && nth)
			{
				if (runner->Match(ref))
				{
					index = upDex;
					nth--;
				}
				runner = runner->getNext();
				upDex++;
			}
		}
		else
		{
			runner = m_Tail;
			while (runner && nth)
			{
				if (runner->Match(ref))
				{
					index = dnDex;
					nth--;
				}
				runner = runner->getPrevious();
				dnDex--;
			}
		}
		return index;
	}
	//Finds the index of the nth item that matches 'ref'. If the nth item wans't found, NOTHING will be retrieved
	int IndexOfPrecisely(const T ref, unsigned int nth, bool fromStart)
	{
		if (!m_Count)
		{
			return -1;
		}
		Node<T> *runner = nullptr;
		int upDex = 0, dnDex = m_Count - 1, index = -1;
		if (fromStart)
		{
			runner = m_Head;
			while (runner && nth)
			{
				if (runner->Match(ref))
				{
					index = upDex;
					nth--;
				}
				runner = runner->getNext();
				upDex++;
			}
		}
		else
		{
			runner = m_Tail;
			while (runner && nth)
			{
				if (runner->Match(ref))
				{
					index = dnDex;
					nth--;
				}
				runner = runner->getPrevious();
				dnDex--;
			}
		}
		return nth ? -1 : index;
	}

	//Add blank elements
	void Allocate(unsigned int amount)
	{
		for (int i = 0; i < amount; i++)
		{
			Add(new T());
		}
	}

	//"Add" Family
	//	-> Add items to the list and returns a pointer to the created node. By default, all items are append to the tail. To add to the head (or front) use 'AddFront'
	//Add an item at the specified index. If index is out of bounds, 'data' will be added to the tail instead
	Node<T>* AddAt(T *Data, unsigned int index)
	{
		if(Data)
		{
			Node<T> *temp = RetrieveAt(index);
			if(temp)
			{
				Node<T> *New = new Node<T>(Data);
				New->setPrevious(temp->getPrevious());
				New->setNext(temp);
				if(temp == m_Head)
				{
					m_Head = New;
				}
				else if(temp->getPrevious())
				{
					temp->getPrevious()->setNext(New);
				}
				temp->setPrevious(New);
				m_Count++;
				return New;
			}
			return Add(Data);
		}
		return nullptr;
	}
	//Add 'size' items at the specified index. If index is out of bounds, 'data' will be added to the tail instead.
	//Assuming 'Data' is an array. This method copies and allocated data instead of just saving the pointer
	Node<T>* AddCopyAt(T *Data, unsigned int size)
	{
		for(int i = 0; i < size; i++)
		{
			Add(new T(Data[i]));
		}
		return m_Tail;
	}
	//Add an item to the tail of the list
	Node<T>* Add(T *Data)
	{
		if(Data)
		{
			if (m_Head)	//Adding a new entry at the end (Tail) of the List
			{
				m_Tail->setNext(new Node<T>(Data));
				m_Tail->getNext()->setPrevious(m_Tail);
				m_Tail = m_Tail->getNext();
			}
			else		//The List is empty!
			{
				m_Tail = m_Head = new Node<T>(Data);
			}
			m_Count++;
		}
		return m_Tail;
	}
	//Add 'size' items to the tail of the  list, assuming 'Data' is an array. Those items are added on the order of 'Data'
	Node<T>* Add(T *Data, unsigned int size)
	{
		for (int i = 0; i < size; i++)
		{
			Add(&Data[i]);
		}
		return m_Tail;
	}
	//Add 'size' items to the tail of the  list, assuming 'Data' is an array. This method copies and allocated data instead of just saving the pointer
	Node<T>* AddCopy(T *Data, unsigned int size)
	{
		for(int i = 0; i < size; i++)
		{
			Add(new T(Data[i]));
		}
		return m_Tail;
	}
	//Add an item to the head of the list
	Node<T>* AddFront(T *Data)
	{
		if(Data)
		{
			if(m_Head)	//Adding a new entry at the start (Head) of the List
			{
				m_Head->setPrevious(new Node<T>(Data));
				m_Head->getPrevious()->setNext(m_Head);
				m_Head = m_Head->getPrevious();

			}
			else		//The List is empty!
			{
				m_Tail = m_Head = new Node<T>(Data);
			}
			m_Count++;
		}
		return m_Head;
	}
	//Add 'size' items to the head of the list, assuming 'Data' is an array. Those items are added on the order of 'Data'
	Node<T>* AddFront(T *Data, unsigned int size)
	{
		for(int i = 0; i < size; i++)
		{
			AddFront(&Data[i]);
		}
		return m_Head;
	}
	//Add 'size' items to the head of the  list, assuming 'Data' is an array. This method copies and allocated data instead of just saving the pointer
	Node<T>* AddCopyFront(T *Data, unsigned int size)
	{
		for(int i = 0; i < size; i++)
		{
			AddFront(new T(Data[i]));
		}
		return m_Tail;
	}

	//"Retrieve" Family
	//	-> Returns the whole Node that contains the data. Useful for fast list iterators. All 'Get' family use these
	//Retrieves the first item that matches 'ref', be it from the end or the beginning of the list
	Node<T>* RetrieveThis(const T ref) const
	{
		if (!m_Count)
		{
			return nullptr;
		}
		Node<T>* Header = m_Head;
		Node<T>* Tailer = m_Tail;
		while (true)
		{
			if (Header->Match(ref))
			{
				return Header;
			}
			else if (Tailer->Match(ref))
			{
				return Tailer;
			}
			//We reached the end of our list!
			else if (Header == Tailer || Header == Tailer->getNext())
			{
				return nullptr;
			}
			Header = Header->getNext();
			Tailer = Tailer->getPrevious();
		}
	}
	//Retrieves the nth item from the list. If the nth item wans't found, the CLOSEST will be retrieved instead
	Node<T>* RetrieveThis(const T ref, unsigned int nth, bool fromStart) const
	{
		if (!m_Count)
		{
			return nullptr;
		}
		Node<T> *runner = nullptr, *ret = nullptr;
		if (fromStart)
		{
			runner = m_Head;
			while (runner && nth)
			{
				if (runner->Match(ref))
				{
					ret = runner;
					nth--;
				}
				runner = runner->getNext();
			}
		}
		else
		{
			runner = m_Tail;
			while (runner && nth)
			{
				if (runner->Match(ref))
				{
					ret = runner;
					nth--;
				}
				runner = runner->getPrevious();
			}
		}
		return ret;
	}
	//Retrieves the nth item from the list. If the nth item wans't found, NOTHING will be retrieved
	Node<T>* RetrieveThisPrecisely(const T ref, unsigned int nth, bool fromStart) const
	{
		if (!m_Count)
		{
			return nullptr;
		}
		Node<T> *runner = nullptr, *ret = nullptr;
		if (fromStart)
		{
			runner = m_Head;
			while (runner && nth)
			{
				if (runner->Match(ref))
				{
					ret = runner;
					nth--;
				}
				runner = runner->getNext();
			}
		}
		else
		{
			runner = m_Tail;
			while (runner && nth)
			{
				if (runner->Match(ref))
				{
					ret = runner;
					nth--;
				}
				runner = runner->getPrevious();
			}
		}
		return nth ? nullptr : runner;	//"If we still have 'nth' then we didn't founf the nth element!"
	}
	Node<T>* RetrieveAt(unsigned int index) const
	{
		if (!m_Count)						//The list is empty, what are you searching for ?
		{
			return nullptr;
		}
		else if (index >= m_Count)			//Will not find this node since it's bigger than the actual size of the List<T>
		{
			return nullptr;
		}
		else if (!index)					//If 'index' is 0, then it's the head
		{
			return RetrieveFirst();
		}
		else if (!(index - m_Count - 1))	//If 'index' is equal to 'm_Count' - 1, or the last index, then it's the tail
		{
			return RetrieveLast();
		}
		Node<T> *Temp = nullptr;
		if (index > (m_Count >> 1))			//If the 'index' is bigger than half of the amount of elements, then we should start from the tail!
		{
			Temp = m_Tail;
			index = m_Count - index - 1;	//But since we are traversing the List backwards, let's change the 'index' acordingly
			while (index)
			{
				Temp = Temp->getPrevious();
				if (Temp == nullptr)		//Nothing at this index;
				{
					return nullptr;
				}
				index--;
			}
		}
		else
		{
			Temp = m_Head;
			while (index)
			{
				Temp = Temp->getNext();
				if (Temp == nullptr)			//Nothing at this index;
				{
					return nullptr;
				}
				index--;
			}
		}
		return Temp;
	}
	Node<T>* RetrieveFirst(void) const
	{
		if (m_Head)
		{
			return m_Head;
		}
		return nullptr;
	}
	Node<T>* RetrieveLast(void) const
	{
		if (m_Tail)
		{
			return m_Tail;
		}
		return nullptr;
	}

	//Deletes the whole list of all elements
	void Clear(void)
	{
		Destroy();
	}
	//Deletes all node elements that don't have nothing in them
	void ClearEmpty(void)
	{
		for(Node<T> *runner = m_Head, *back = nullptr; runner != nullptr; runner = runner->getNext())
		{
			if(runner->getData() == nullptr)
			{
				back = runner->getPrevious();
				Delete(runner);
				runner = back;
			}
		}
	}
	bool Empty(void)
	{
		return !m_Count;
	}

	//"Remove" Family
	//	-> Deletes elements from the list
	void Remove(Node<T> *n)					//Final checks before we call the function that deletes nodes
	{
		if (n == nullptr || !m_Count)		//Nothing to do here....
		{
			return;
		}
		else if (!Contains(n))				//This node don't even exists in the list. What are you doing programmer ?!
		{
			return;
		}
		Delete(n);							//This node exists whitin this list, we can safely delete it
	}
	//Removes the first item that matches 'ref', be it from the end of the beginning of the list
	void RemoveThis(const T ref)
	{
		Node<T> *temp = RetrieveThis(ref);
		if (temp)
		{
			Delete(temp);
		}
	}
	//Removes the nth item from the list. If the nth item wans't found, the closest will be deleted instead
	void RemoveThis(const T ref, unsigned int nth, bool fromStart)
	{
		Node<T> *temp = RetrieveThis(ref, nth, fromStart);
		if (temp)
		{
			Delete(temp);
		}
	}
	//Removes the nth item from the list. If the nth item wans't found, NOTHING will be deleted
	void RemoveThisPrecisely(const T ref, unsigned int nth, bool fromStart)
	{
		Node<T> *temp = RetrieveThisPrecisely(ref, nth, fromStart);
		if (temp)
		{
			Delete(temp);
		}
	}
	void RemoveAt(unsigned int index)
	{
		Node<T> *Temp = RetrieveAt(index);
		if (Temp)
		{
			Delete(Temp);
		}
	}
	void RemoveFirst(void)
	{
		if (m_Head)
		{
			Delete(m_Head);
		}
	}
	void RemoveLast(void)
	{
		if (m_Tail)
		{
			Delete(m_Tail);
		}
	}

	//"Release" Family
	//	-> Release elements from the list, without deleting them. Take caution to NOT LEAK THIS MEMORY afterwards!!
	//	Note: To prevent memory problems, no 'ReleaseThis' method is included
	void Release(Node<T> *n)
	{
		if(n == nullptr || !m_Count)
		{
			return;
		}
		else if(!Contains(n))
		{
			return;
		}
		FreeNode(n);
	}
	void ReleaseAt(unsigned int index)
	{
		Node<T> *Temp = RetrieveAt(index);
		if(Temp)
		{
			Release(Temp);
		}
	}
	void ReleaseFirst(void)
	{
		if(m_Head)
		{
			Release(m_Head);
		}
	}
	void ReleaseLast(void)
	{
		if(m_Tail)
		{
			Release(m_Tail);
		}
	}

	//Copy all contents to an array T, but keep then in the list as well
	T* ToArray(void)
	{
		if(Empty())
		{
			return nullptr;
		}
		T *arr = new T[m_Count];
		Node<T> *reader = m_Head;
		for(int i = 0; reader; i++)
		{
			arr[i] = *reader->getData();
			reader = reader->getNext();
		}
		return arr;
	}
	//Copy all contents to an array T and then RELEASE all of the list contents
	T* ToArrayTransfer(void)
	{
		if(Empty())
		{
			return nullptr;
		}
		T *arr = new T[m_Count]();
		for(int i = 0; m_Count > 0; i++)
		{
			arr[i] = *m_Head->getData();
			ReleaseFirst();
		}
		return arr;
	}
	//Enabling the [] operator to be used in this List configuration
	/*T& operator[](unsigned int index)
	{
		return *GetAt(index);
	}
	T& operator[](unsigned int index) const
	{
		return *GetAt(index);
	}*/

#ifdef _INC_STDIO
	void print(const char *format, int spacing)
	{
		Node<T> *runner = m_Head;
		for(int i = 0; runner; i++)
		{
			printf(format, *runner->getData());
			if (!((i + 1) % spacing))
			{
				printf("\n");
			}
			runner = runner->getNext();
		}
		printf("\n");
	}

	void print(const char *format)
	{
		Node<T> *runner = m_Head;
		while (runner)
		{
			printf(format, *runner->getData());
			runner = runner->getNext();
		}
		printf("\n");
	}
#endif
};

#ifdef _INC_STDIO	//List<> Templated Specializations
//String specializations
template <>
inline void List<char>::print(const char *format)
{
	Node<char> *runner = m_Head;
	while (runner)
	{
		printf(format, runner->getData());
		runner = runner->getNext();
	}
}

	#ifdef _H_CSTTYPE_
template <>
inline void List<BaseString>::print(const char *format)
{
	Node<BaseString> *runner = m_Head;
	while (runner)
	{
		printf(format, runner->getData()->getString());
		runner = runner->getNext();
	}
}
	#endif

#endif

//Stack<Type>
//	*Represents a Stack or a Queue depending of the mode supplied
//		-STACK_FIFO (First In First Out)	-> Queue Mode
//		-STACK_LIFO (Last In First Out)		-> Stack Mode
//	*Uses a List<Type> as it's main stack element
template <typename T>
class Stack
{
private:

	const bool m_isLIFO;
	T* m_PopOutput;
	List<T> m_Stack;

	//Following this implementation of List<T>, the newer T objects are stored at the Tail of it
	T* PopFIFO(void)	// "First-Out"	-> Remove items at the order they come in (Queue)
	{
		if (m_Stack.Count() > 0)
		{
			m_PopOutput = new T(*m_Stack.First());
			m_Stack.RemoveFirst();
		}
		return m_PopOutput;
	}
	T* PopLIFO(void)	// "Last-In"	-> Remove the most recent item (Stack)
	{
		if (m_Stack.Count() > 0)
		{
			m_PopOutput = new T(*m_Stack.Last());
			m_Stack.RemoveLast();
		}
		return m_PopOutput;
	}

	T *ReleaseFIFO(void)
	{
		if(m_Stack.Count() > 0)
		{
			T* ret = m_Stack.RetrieveFirst()->releaseData();
			m_Stack.RemoveFirst();
			return ret;
		}
		return nullptr;
	}
	T *ReleaseLIFO(void)
	{
		if(m_Stack.Count() > 0)
		{
			T* ret = m_Stack.RetrieveLast()->releaseData();
			m_Stack.RemoveLast();
			return ret;
		}
		return nullptr;
	}

	T* PeekFIFO(void)
	{
		if (m_Stack.Count() > 0)
		{
			return m_Stack.First();
		}
		return nullptr;
	}
	T* PeekLIFO(void)
	{
		if (m_Stack.Count() > 0)
		{
			return m_Stack.Last();
		}
		return nullptr;
	}

	//Function Pointer to prevent StackMode checking for each Pop() operation
	T* (Stack<T>::*PopFN)(void);
	T* (Stack<T>::*PopRFN)(void);
	T* (Stack<T>::*PeekFN)(void);

public:

	Stack(StackMode Mode) : m_isLIFO(Mode == STACK_LIFO ? true : false), m_PopOutput(nullptr)
	{
		if (m_isLIFO)
		{
			PopFN = &Stack<T>::PopLIFO;
			PopRFN = &Stack<T>::ReleaseLIFO;
			PeekFN = &Stack<T>::PeekLIFO;
		}
		else
		{
			PopFN = &Stack<T>::PopFIFO;
			PopRFN = &Stack<T>::ReleaseFIFO;
			PeekFN = &Stack<T>::PeekFIFO;
		}
	}
	~Stack()
	{
		m_Stack.Clear();
	}

	void AddAt(T *in, unsigned int index)
	{
		m_Stack.AddAt(in, index);
	}
	void Push(T *in)
	{
		m_Stack.Add(in);
	}
	void Push(T *in, unsigned int size)
	{
		m_Stack.Add(in, size);
	}
	T* Peek(void)
	{
		return (this->*PeekFN)();
	}
	T* Pop(void)
	{
		if (m_PopOutput)
		{
			delete m_PopOutput;
			m_PopOutput = nullptr;
		}
		return (this->*PopFN)();
	}
	T* Release(void)
	{
		return (this->*PopRFN)();
	}
	Node<T> *RetrieveFirst(void)
	{
		return m_Stack.RetrieveFirst();
	}
	Node<T> *RetrieveLast(void)
	{
		return m_Stack.RetrieveLast();
	}
	int Count(void)
	{
		return m_Stack.Count();
	}
	void Clear(void)
	{
		m_Stack.Clear();
	}
	bool Empty(void)
	{
		return m_Stack.Empty();
	}

	StackMode getMode(void)
	{
		return m_isLIFO;
	}
};

//Tree<Type> Class
//	*Represents a Binary Tree
//	*'T' MUST be direct comparable by either <, > or ==
template <class T>
class Tree
{
private:

	Node<T> *m_Root;			//Using 'next' as RIGHT and 'prev' as LEFT
	Node<T> *m_LeftMost;		//The Left Most value of this tree (The Lowest value)
	Node<T> *m_RightMost;		//The Right Most value of this tree (The Highest value)
	unsigned int m_Count;
	unsigned int m_Height;

	void Delete(Node<T> *n)
	{
		if (n->getNext())
		{
			Delete(n->getNext());
			n->setNext(nullptr);
		}
		if (n->getPrevious())
		{
			Delete(n->getPrevious());
			n->setPrevious(nullptr);
		}
		delete n;
	}
	void CheckHeight(int h)
	{
		if (h > m_Height)
		{
			m_Height = h;
		}
	}
	//Searches the tree for value 'Data'. If found, return it's node
	Node<T>* Traverse(Node<T> *n, const T Data)
	{
		if (n)
		{
			if (Data < *n->getData())
			{
				return Traverse(n->getPrevious(), Data);
			}
			else if (Data > *n->getData())
			{
				return Traverse(n->getNext(), Data);
			}
		}
		return n;
	}
	bool HaveChild(Node<T> *n)
	{
		return (n->getNext() || n->getPrevious());
	}

#ifdef _IOSTREAM_
	//Print Order: Left -> Root -> Right
	void InOrder(Node<T> *n)
	{
		if (n)
		{
			InOrder(n->getPrevious());
			std::cout << *n->getData() << " ";
			InOrder(n->getNext());
		}
		return;
	}
	//Print Order: Root -> Left -> Right
	void PreOrder(Node<T> *n)
	{
		if (n)
		{
			std::cout << *n->getData() << " ";
			PreOrder(n->getPrevious());
			PreOrder(n->getNext());
		}
		return;
	}
	//Print Order: Left -> Right -> Root
	void PostOrder(Node<T> *n)
	{
		if (n)
		{
			PostOrder(n->getPrevious());
			PostOrder(n->getNext());
			std::cout << *n->getData() << " ";
		}
		return;
	}
#endif

public:

	Tree() :  m_Root(nullptr), m_LeftMost(nullptr), m_RightMost(nullptr), m_Count(0), m_Height(0) { };
	~Tree()
	{
		if (m_Root)
		{
			Delete(m_Root);
		}
	}

	//For basic types or types that support direct comparison between themselves (int / float / double / etc..)
	//Returns true if a new value was inserted
	bool Insert(T *Data)
	{
		// Insert concept: Lower values should be put at the left, and Higher at the right (if facing the tree from the front)
		//
		//			(5)		-> Root (Count = 6 | Height = 3)
		//			/ \
		//		  (3) (8)	-> Branches
		//		  /	\	\
		//		(1)	(4)	(9)	-> Leaves
		if (m_Root)
		{
			int Height = 1;
			Node<T> *Monkey = m_Root;
			while (true)
			{
				Height++;
				if (*Data < *Monkey->getData())					//Lower! Insert Left
				{
					if (Monkey->getPrevious() == nullptr)		//We don't have a left here. Insert it
					{
						Monkey->setPrevious(new Node<T>(Data));
						m_Count++;
						CheckHeight(Height);
						if (*m_LeftMost->getData() > *Monkey->getPrevious()->getData())
						{
							m_LeftMost = Monkey->getPrevious();
						}
						return true;
					}
					Monkey = Monkey->getPrevious();				//Go Left to find a vacant spot
				}
				else if (*Data > *Monkey->getData())			//Higher! Insert Right
				{
					if (Monkey->getNext() == nullptr)
					{
						Monkey->setNext(new Node<T>(Data));
						m_Count++;
						CheckHeight(Height);
						if (*m_RightMost->getData() > *Monkey->getNext()->getData())
						{
							m_RightMost = Monkey->getNext();
						}
						return true;
					}
					//Go Right to find a vacant spot
					Monkey = Monkey->getNext();
				}
				else
				{
					return false;
				}
			}
		}
		//This Tree have no Root! Let's plant one
		if(Data)	//...But only if 'Data' is not nullptr
		{
			m_LeftMost = m_RightMost = m_Root = new Node<T>(Data);
			m_Count = 1;
			m_Height = 1;
			return true;
		}
		return false;
	}
	bool Insert(T *Data, unsigned int size)
	{
		unsigned int out = 0;
		for (int i = 0; i < size; i++)
		{
			out += Insert(&Data[i]);
		}
		return out;
	}

	int Count(void)
	{
		return m_Count;
	}
	int Height(void)
	{
		return m_Height;
	}
	// TODO
	void Balance(void)
	{

	}
	void Clear(void)
	{
		if (m_Root)
		{
			Delete(m_Root);
			m_Count = m_Height = 0;
			m_Root = m_LeftMost = m_RightMost = nullptr;
		}
	}
	bool Exists(const T Data)
	{
		return Traverse(m_Root, Data);
	}
	T* Find(const T Data)
	{
		Node<T> *Temp = Traverse(m_Root, Data);
		if(Temp)
		{
			return Temp->getData();
		}
		return nullptr;
	}
	bool Empty(void)
	{
		return !m_Count;
	}

	void COutInOrder(void)
	{
		InOrder(m_Root);
	}
	void COutPreOrder(void)
	{
		PreOrder(m_Root);
	}
	void COutPostOrder(void)
	{
		PostOrder(m_Root);
	}
};

//Trie<Type> Class
//	*Represents a special kind of Tree used for string search and traversal
template <class T>
class Trie
{
protected:
	T *m_data;					//Holds generalized data for this node
	List<Trie> *m_keyNext;		//List of possible letters that can fit after this node's Key, or possible transitions
	unsigned int m_longest;		//Longest string length from this node till the deepest node
	const unsigned char m_key;	//This node's Key

	int recInsert(const char *key, int i, T *data)
	{
		if (key[i])
		{
			if (m_keyNext == nullptr)	//Because it's better to always test this than to build a new List<Trie> everytime we want to Retrieve
			{
				m_keyNext = new List<Trie>();
			}
			Node<Trie> *runner = m_keyNext->RetrieveThis(key[i]);
			if (runner)													//If temp exists, then we found a letter. Test the next one on this node
			{
				return runner->getData()->recInsert(key, i + 1, data) + 1;
			}		
			m_keyNext->Add(new Trie(key[i]));							//If temp is null, this letter doesn't exist in this node. Add this key...
			return m_keyNext->Last()->recInsert(key, i + 1, data) + 1;	//...and then the rest of it
		}
		else if (i)	//If we reached the end of our key ('\0') and 'i' is not 0, then we ran across all of it! Insert data now!
		{
			setData(data);
		}
		return 0;
	}

	bool recContains(const char *str, int i)
	{
		if (str[i])									//If we managed to reach '\0' then we ran across the entire 'str'! Return true
		{
			if (m_keyNext && m_keyNext->Count())	//If we have keys, run trough them
			{
				Node<Trie> *runner = m_keyNext->RetrieveThis(str[i]);
				if (runner)	//If we found it, keep going!
				{
					return runner->getData()->recContains(str, i + 1);
				}
			}
			return false;							//But if we don't have or if we didn't found 'str[i]' this iteration, 'str' isn't here
		}
		return true;
	}

	Trie* recRetrieve(const char *str, int i)
	{
		if (m_keyNext && m_keyNext->Count())	//If we have keys, run trough them
		{
			Node<Trie> *runner = m_keyNext->RetrieveThis(str[i]);
			if (runner)							//If we found this letter, keep going!
			{
				if (str[i + 1])					//If the next letter isn't the end of the string, we still have work to do!
				{
					return runner->getData()->recRetrieve(str, i + 1);
				}
				return runner->getData();		//But if it is, then we are done here!
			}
		}
		return nullptr;							//But if we don't have or if we didn't find 'str[i]' this iteration, 'str' isn't here
	}
	
	//TODO: TEST THIS v
	Trie* intRetrieve(const char *str)
	{
		Trie *curNode = this;
		for(int i = 0; str[i]; i++)
		{
			if(curNode->m_keyNext && curNode->m_keyNext->Count())
			{
				Node<Trie> *test = curNode->m_keyNext->RetrieveThis(str[i]);
				if(test)
				{
					curNode = test->getData();
					if(str[i+1] == '\0')
					{
						return curNode;
					}
				}
			}
		}
		return nullptr;
	}

	void recLister(List<T> *master)
	{
		if (m_keyNext && m_keyNext->Count())	//If we have keys to print, print them recursively
		{
			Node<Trie> *runner = m_keyNext->RetrieveFirst();
			while (runner)
			{				
				runner->getData()->recLister(master);
				runner = runner->getNext();
			}
		}
		if (m_data)	//If this Trie have internal data, say it have stuff in it
		{
			master->Add(m_data);
		}
	}

#ifdef _INC_STDIO
	//
	//	    c-a-s-a-(\0)
	//	   /   \ \ \
	// 	  /	    \ \ c-o-(\0)
	//	 /	     \ e-b-r-e-(\0)
	//(\0)	      r-a-c-a-(\0)
	//   \
	//    b-a-r-a-c-a-(\0)
	//
	// Paths from (\0) 'Root': 'c' OR 'b'
	// Paths from 'c': 'a'
	// Paths from 'a': 's' OR 'r'...
	void recPrint(char *buf, int i)
	{
		if (m_keyNext && m_keyNext->Count())	//If we have keys to print, print them recursively
		{
			Node<Trie> *runner = m_keyNext->RetrieveFirst();
			while (runner)
			{
				buf[i] = runner->getData()->Key();
				if (m_data)	//If this Trie have internal data, say it have stuff in it
				{
					printf("{!}");
				}
				runner->getData()->recPrint(buf, i + 1);
				runner = runner->getNext();
			}
		}
		else 	//If we came here, we have run trough all possible letters of this chain
		{
			buf[i] = '\0';	//To prevent previous text from apearing again
			printf("%s", buf);
			printf("\n");

		}
	}
#endif

	void Destroy(void)
	{
		if (m_keyNext)
		{
			delete m_keyNext;
			m_keyNext = nullptr;
		}
		if (m_data)
		{
			delete m_data;
			m_data = nullptr;
		}
	}

public:
	Trie( ) : m_key('\0'), m_longest(0), m_keyNext(nullptr), m_data(nullptr) { }
	Trie(const char mainKey) : m_key(mainKey), m_longest(0), m_keyNext(nullptr), m_data(nullptr) { }
	Trie(const char mainKey, T* data) : m_key(mainKey), m_longest(0), m_keyNext(nullptr), m_data(data) { }
	Trie(const char *fullKey) : m_key(fullKey[0])
	{
		Insert(fullKey, nullptr);
	}
	Trie(const char *fullKey, T *data) : m_key(fullKey[0])
	{
		Insert(fullKey, data);
	}

	~Trie()
	{
		Destroy();
	}

	void Clear(void)
	{
		Destroy();
	}

	//Insert a word in the Trie, returning the length of the longest word inside of it
	void Insert(const char *key, T *data)
	{
		unsigned int temp = recInsert(key, 0, data);
		if (temp > m_longest)
		{
			m_longest = temp;
		}
	}
	void Insert(const char *key)
	{
		unsigned int temp = recInsert(key, 0, nullptr);
		if (temp > m_longest)
		{
			m_longest = temp;
		}
	}

	//Return how many possible 'paths' we can go from this Trie
	unsigned int Derivations(void)
	{
		if (m_keyNext)
		{
			return m_keyNext->Count();
		}
		return 0;
	}
	unsigned int Derivations(const char *key)
	{
		Trie *temp = recRetrieve(key, 0);
		if (temp)
		{
			return temp->Derivations();
		}
		return 0;
	}

	unsigned int Longest(void)
	{
		return m_longest;
	}

	unsigned char Key(void)
	{
		return m_key;
	}

	void Fill(const char **keys, T **data, int amount)
	{
		for(int i = 0; i < amount; i++)
		{
			Insert(keys[i], data[i]);
		}
	}

	//Returns true if the Trie have this path
	bool Contains(const char *key)
	{
		if (key && key[0])	//If 'str' exists and it's not an empty string, search for it
		{
			return recContains(key, 0);
		}
		return false;
	}

	//Get the data at the end a path defined by 'key'. If no data exists or the path doesn't lead anywhere, returns null
	T* Get(const char *key)
	{
		Trie *temp = Retrieve(key);
		if (temp)
		{
			return temp->getData();
		}
		return nullptr;
	}
	//Same as Get, but return the whole Trie instead
	Trie* Retrieve(const char *key)
	{
		if (key && key[0])
		{
			return recRetrieve(key, 0);
		}
		return nullptr;
	}

	Trie* RetrieveIterative(const char *key)
	{
		if (key && key[0])
		{
			return intRetrieve(key);
		}
		return nullptr;
	}

	//List all paths that have data in them, and return the list of it
	List<T>* ListAll(void)
	{
		List<T> *master = new List<T>();
		recLister(master);
		return master;
	}

	//Return true if the Trie is empty
	bool Empty(void)
	{
		return !m_keyNext;	//If our list is nullptr, then this key is called 'Empty'
	}

	//Insert data in this Trie node
	void setData(T *data)
	{
		if (m_data)
		{
			delete m_data;
		}
		m_data = data;
	}
	T* getData(void)
	{
		return m_data;
	}

#ifdef _INC_STDIO
	void print(void)
	{
		char *buf = new char[m_longest + 1]();
		printf("* Note: Nodes that contains data have '{!}' before it's key\n");
		recPrint(buf, 0);
		delete[] buf;
	}
#endif

	// Operators -------------------------------------------------------------------------------------
	bool operator==(Trie &in)
	{
		return m_key == in.Key();
	}
	bool operator==(const Trie &in) const
	{
		return m_key == in.m_key;
	}
};

//Hash<Type>
//	*Represents a Hash Table
template <class T>
class Hash
{	//TODO!! More study required
private:
	List<T> *m_labels;	//Array of lists. Every index is a 'label' of this table
	const int m_count;	//Amount of labels in this table
	int m_entries;		//Amount of entries in this table

	int hasher(const char *key)
	{
		int index = 0;
		for (int i = 0; key[i]; i++)
		{
			index += key[i];
		}
		return index % m_count;
	}

public:
	Hash(int labels) : m_count(labels > 0 ? labels : 1), m_entries(0)  
	{
		m_labels = new List<T>[m_count]();
	}

	~Hash()
	{
		m_labels->Clear();
		delete[] m_labels;
		m_labels = nullptr;
	}

	void Insert(const char *key, T *data)
	{
		m_labels[hasher(key)].Add(data);
		m_entries++;
	}

	List<T>* Retrieve(const char *key)
	{
		return &m_labels[hasher(key)];
	}

	int Labels(void)
	{
		return m_count;
	}
	int Entries(void)
	{
		return m_entries;
	}

	// Operators ----------------------------------------------------------------------------------
	List<T>* operator[](const char *key)
	{
		return Retrieve(key);
	}

#ifdef _INC_STDIO
	void print(const char *format)
	{
		for (int i = 0; i < m_count; i++)
		{
			printf("[%3d] - %d entries\n", i, m_labels[i].Count());
			m_labels[i].print(format);
			printf("\n");
		}
	}
#endif
};

class Automata
{
private:
	//Automata Transition Class
	//	*Defines a transition between two Automata nodes
	struct AutomataTransition
	{
		const char m_read;			//What character this transition should read
		Automata *m_next;			//Next Automata node this transition will jump to

		AutomataTransition(Automata *next, char read) : m_next(next), m_read(read) { }

		~AutomataTransition()
		{
			//Don't delete anything here, as each automata linked should be deleted by itself
		}

		bool haveTransition(const char key)
		{
			return m_read == key;
		}

		// Operators ------------------------------------------------------------------------------------
		bool operator==(const AutomataTransition &at)
		{
			return m_read == at.m_read && m_next == at.m_next;
		} 

#ifdef _INC_STDIO
		void print(void)
		{
			printf("\\__'%c' ->", m_read);
			if (m_next->EndNode())
			{
				printf("((%s))\n", m_next->Name());
			}
			else
			{
				printf("  %s\n", m_next->Name());
			}
		}
#endif
	};

	const bool m_isEnd;					//If true, this Automata marks the end of the chain
	BaseString *m_name;					//Name of this node (optional)
	List<AutomataTransition> m_paths;	//Possible paths this Automata can take

	//If we can transit to another Automata Node using this key, return the pointer to it. If we can't return NULL
	Automata* canTransit(const char key)
	{
		Node<AutomataTransition> *runner = m_paths.RetrieveFirst();
		while(runner)
		{
			if (runner->getData()->haveTransition(key))
			{
				return runner->getData()->m_next;
			}
			runner = runner->getNext();
		}
		return nullptr;
	}

public:
	Automata(bool endNode) : m_isEnd(endNode), m_name(nullptr) { };
	Automata(bool endNode, const char *name) : m_isEnd(endNode), m_name(new BaseString(name)) { };

	~Automata()
	{
		if (m_name)
		{
			delete m_name;
			m_name = nullptr;
		}
		m_paths.Clear();
	}

	void AddTransition(Automata *jump, char read)
	{
		m_paths.Add(new AutomataTransition(jump, read));
	}
	void AddTransition(char read)
	{
		m_paths.Add(new AutomataTransition(this, read));
	}
	void Remove(AutomataTransition &transition)
	{
		Node<AutomataTransition> *temp = m_paths.RetrieveThis(transition);
		if (temp)
		{
			delete temp;
		}
	}

	int Transitions(void)
	{
		return m_paths.Count();
	}
	const char* Name(void)
	{
		return m_name->getString();
	}
	bool EndNode(void)
	{
		return m_isEnd;
	}

	bool Accepts(const char *input)
	{
		Automata *runner = canTransit(input[0]);		//Transit the first time
		for (int i = 1; input[i] && runner; i++)		//Suppose 'input' is valid...
		{
			runner = runner->canTransit(input[i]);		//...get the next node we can transit trough at this step
		}
		//If we reached here, we consumed all 'input', which means we always had a transtion to make...
		if (runner == nullptr || !runner->EndNode())	//...but if the node we stopped isn't an end node, then 'input' is invalid
		{
			return false;
		}
		return true;
	}

#ifdef _INC_STDIO
	bool d_Accepts(const char *input)
	{
		Automata *runner = this;
		for(int i = 0; input[i]; i++)				//Suppose 'input' is valid...
		{
			runner = runner->canTransit(input[i]);	//...get the next node we can transit trough at this step
			if(runner == nullptr)					//If there isn't a valid node, 'input' is NOT valid
			{
				printf("[%4s](R: '%c') -> -NO TRANS.- \n", m_name->getString(), input[i]);
				return false;
			}
			printf("[%4s](R: '%c') -> [%4s]\n", m_name->getString(), input[i], runner->m_name->getString());
		}
		//If we reached here, we run trough 'input', which means we always had a transtion to make...
		if(runner->EndNode())						//...and after that, if we reached an 'end' node, then 'input' is valid
		{
			printf("\t\t\t\t[ACCEPTED]\n");
			return true;
		}
		printf("\t\t\t\t[-FAILED-]\n");
		return false;
	}

	void print(void)
	{
		Node<AutomataTransition> *runner = m_paths.RetrieveFirst();
		printf("%s\n", m_name->getString());
		while (runner)
		{
			printf(" ");
			runner->getData()->print();
			runner = runner->getNext();
		}
	}
#endif
};
#endif
