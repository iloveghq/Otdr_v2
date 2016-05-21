// Template based containers classes base.
//
// Coded by Marco Zaratti / marco.zaratti@gmail.it / 2006.04.05
//
//////////////////////////////////////////////////////////////////////

#ifndef _CONTAINERS_H_
#define _CONTAINERS_H_

#include <windows.h>
#include <assert.h>

typedef unsigned long ulong;

// ----- Double Linked List ------------------------------------------------

template <class T>
class CTList
{
public:
	CTList();
	~CTList();	//Calls ClearList() so data stored in the list must be erased
				//manually before deleting CTList object (ex: with FullDelete())
	CTList(const CTList &lst);
	CTList& operator=(const CTList &lst);

	T& GetHead();
	T& GetTail();

	void AddHead(const T &Data); 
	void AddTail(const T &Data);

	//Remove functions doesn't delete data stored in the list (if pointers)!
	T RemoveHead();
	T RemoveTail();

	//Removes nodes with payload==data but without deleting data
	void Remove(const T &Data);
	//Removes nodes with payload==data and deletes data
	//(only to be used if data are pointers returned by 'new' operator)
	void RemoveData(const T &Data);

	//Clears the list but without deleting its data content. If data are pointers
	//and you want to delete them then use the ClearData function.
	void Clear();

	//This function is a helper for quick delete of all data
	//stored in the list and the list itself:
	//(only to be used if data are pointers returned by 'new' operator)
	void ClearData();

	bool IsEmpty();
	ulong GetNumEntries();

	//Iterator functions
	//example:
	//	if(!lst.IsEmpty()) {
	//		data = lst.GetHead();
	//		while(!lst.IsLast())
	//			data = lst.GetNext();
	//	}
	T& GetNext();
	T& GetPrev();
	bool IsLast();
	bool IsFirst();

private:
	struct Node
	{
		T payload;
		Node *prev;
		Node *next;
	};
	Node  *head, *tail, *temp, *pos;
	ulong numEntries;
};

// ----- FIFO Buffer -------------------------------------------------------

template <class T>
class CTFifo : public CTList<T>
{
public:
	
	void PushIn(const T &data){AddTail(data);}
	T PopOut(){return RemoveHead();}
};

// ----- STACK Buffer ------------------------------------------------------

template <class T>
class CTStack : public CTList<T>
{
public:

	void Push(const T &data){AddHead(data);}
	T Pop(){return RemoveHead();}
};

// ----- Dynamic ARRAY -----------------------------------------------------

#define CTARRAY_START_SIZE 64
template <class T>
class CTArray
{
public:
	CTArray();
	CTArray( ulong dim);	//Allocates dim as the array starting size
	CTArray(const CTArray &pA);
	~CTArray();

	ulong Add(const T &data);	//returns position in the array
	void GrowTo(ulong size);	//grows only if size > arraySize
	void Reserve(ulong num);	//reserve the first num positions and sets
								//numEntries = num
	ulong Size() const {return numEntries;}	//arraySize is hidden. For the programmer
										//the array size is = to numEntries
	void Clear();				//deletes the array
	void ClearData();			//deletes the array and its content (use carefully)

	T& operator[](ulong index);
	const T& operator[](ulong index) const;
	CTArray& operator=(const CTArray &pA);

private:
	void CalcNewSize();					//calculates new arraySize
	void ExpandArray(ulong finalSize);	//expands array

	T *array;
	ulong arraySize;
	ulong numEntries;
};

//--------------------------------------------------------------------------
//                       INLINE FUNCTIONS DEFINITION
//--------------------------------------------------------------------------

//= CTArray ================================================================

template <class T>
inline CTArray<T>::CTArray()
{
	array = NULL;
	arraySize = 0;
	numEntries = 0;
}

template <class T>
inline CTArray<T>::CTArray(ulong dim)
{
	array = NULL;
	arraySize = 0;
	numEntries = 0;

	if(dim > arraySize)
	{
		arraySize = dim;
		ExpandArray(dim);
	}
}

template <class T>
inline CTArray<T>::CTArray(const CTArray<T> &pA)
{
	array = NULL;
	if(array != pA.array)
	{
		arraySize = pA.numEntries;
		numEntries = pA.numEntries;
		ExpandArray(arraySize);
		for(ulong i=0; i < numEntries; i++)
			array[i] = pA.array[i];
	}
	else
	{
		arraySize = 0;
		numEntries = 0;
	}
}

template <class T>
inline CTArray<T>& CTArray<T>::operator=(const CTArray<T>& pA)
{
	if(array != pA.array)
	{
		Clear();
		if(pA.array)
		{
			arraySize = pA.numEntries;
			numEntries = pA.numEntries;
			ExpandArray(arraySize);
			for(ulong i=0; i < numEntries; i++)
				array[i] = pA.array[i];
		}
	}
	return *this;
}

template <class T>
inline CTArray<T>::~CTArray()
{
	Clear();
}

template <class T>
inline ulong CTArray<T>::Add(const T &data)
{
	if(numEntries >= arraySize)
	{
		CalcNewSize();
		ExpandArray(arraySize);
	}
	array[numEntries] = data;
	return numEntries++;
}

template <class T>
inline void CTArray<T>::GrowTo(ulong size)
{
	if(size > arraySize)
	{
		arraySize = size;
		ExpandArray(size);
	}
}

template <class T>
inline void CTArray<T>::Reserve(ulong num)
{
	if(num > numEntries)
	{
		if(num > arraySize)
		{
			arraySize = num;
			ExpandArray(num);
		}
		numEntries = num;
	}
}

template <class T>
inline void CTArray<T>::CalcNewSize()
{
	if(arraySize)
		arraySize += arraySize;		//exponential grow
	else
		arraySize = CTARRAY_START_SIZE;
}

template <class T>
inline void CTArray<T>::ExpandArray(ulong finalSize)
{
	assert(finalSize);
	assert(finalSize >= numEntries);
	if(array)
	{
		T *temp = new T[finalSize];
		for(ulong i=0; i < numEntries; i++)
			temp[i] = array[i];
		delete[] array;
		array = temp;
	}
	else
		array = new T[finalSize];
}

template <class T>
inline void CTArray<T>::Clear()
{
	if(array)
	{
		delete[] array;
		array = NULL;
		numEntries = 0;
		arraySize = 0;
	}
}

template <class T>
inline void CTArray<T>::ClearData()
{
	if(array)
	{
		for(ulong i=0; i < numEntries; i++)
			delete array[i];
		delete[] array;
		array = NULL;
		numEntries = 0;
		arraySize = 0;
	}
}

template <class T>
inline T& CTArray<T>::operator[](ulong index)
{
	assert(array);
	assert(index >= 0);
	assert(index < numEntries);
	return array[index];
}

template <class T>
inline const T& CTArray<T>::operator[](ulong index) const
{
	assert(array);
	assert(index >= 0);
	assert(index < numEntries);
	return array[index];
}

//= CTList =================================================================

template <class T>
inline CTList<T>::CTList()
{
	head = NULL;
	tail = NULL;
	temp = NULL;
	pos = NULL;
	numEntries = 0;
}

template <class T>
inline CTList<T>::~CTList()
{
	Clear();
}

template <class T>
inline CTList<T>::CTList(const CTList<T> &lst)
{
	Node* np;
	
	head = NULL;
	tail = NULL;
	temp = NULL;
	pos = NULL;
	numEntries = 0;
	
	if(head != lst.head)
	{
		np = lst.head;
		while (np)
		{
			temp = new Node;
			temp->payload = np->payload;
			temp->prev = tail;
			if(tail)
				tail->next = temp;
			else
				head = temp;
			tail = temp;
			np = np->next;
		}
		tail->next = NULL;
		numEntries = lst.numEntries;
	}
}

template <class T>
inline CTList<T>& CTList<T>::operator=(const CTList<T> &lst)
{
	Node *np;
	
	if(head != lst.head)
	{
		Clear();
		np = lst.head;
		while (np)
		{
			temp = new Node;
			temp->payload = np->payload;
			temp->prev = tail;
			if(tail)
				tail->next = temp;
			else
				head = temp;
			tail = temp;
			np = np->next;
		}
		if(tail)
		{
			tail->next = NULL;
			numEntries = lst.numEntries;
		}
	}
	return *this;
}

template <class T>
inline T& CTList<T>::GetHead()
{
	assert(head);
	pos = head;
	return head->payload;
}

template <class T>
inline T& CTList<T>::GetTail()
{
	assert(tail);
	pos = tail;
	return tail->payload;
}

template <class T>
inline T CTList<T>::RemoveHead()
{
	assert(head);
	temp = head;
	head = head->next;
	if(!head)
		tail = NULL;
	else
		head->prev = NULL;
	T tp = temp->payload;
	delete temp;
	numEntries--;
	return tp;
}

template <class T>
inline T CTList<T>::RemoveTail()
{
	assert(tail);
	temp = tail;
	tail = tail->prev;
	if(!tail)
		head = NULL;
	else
		tail->next = NULL;
	T tp = temp->payload;
	delete temp;
	numEntries--;
	return tp;
}

template <class T>
inline void CTList<T>::Remove(const T &Data)
{
	temp = head;
	Node *prev, *next;

	//Removes all data nodes from the list
	while(temp)
	{
		if(temp->payload == Data)
		{
			prev = temp->prev;
			next = temp->next;
			if(prev)
				prev->next = next;
			else
				head = next;
			if(next)
				next->prev = prev;
			else
				tail = prev;
			delete temp;
			numEntries--;
			temp = next;
		}
		else
			temp = temp->next;
	}
}

template <class T>
inline void CTList<T>::RemoveData(const T &Data)
{
	temp = head;
	Node *prev, *next;

	//Removes all data nodes from the list including data itself
	while(temp)
	{
		if(temp->payload == Data)
		{
			prev = temp->prev;
			next = temp->next;
			if(prev)
				prev->next = next;
			else
				head = next;
			if(next)
				next->prev = prev;
			else
				tail = prev;
			delete temp->payload;
			delete temp;
			numEntries--;
			temp = next;
		}
		else
			temp = temp->next;
	}
}

template <class T>
inline void CTList<T>::AddHead(const T &data)
{
	temp = new Node;
	temp->next = head;
	temp->prev = NULL;
	temp->payload = data;
	if(head)
		head->prev = temp;
	else
		tail = temp;
	head = temp;
	numEntries++;
}

template <class T>
inline void CTList<T>::AddTail(const T &data)
{
	temp = new Node;
	temp->next = NULL;
	temp->prev = tail;
	temp->payload = data;
	if(tail)
		tail->next = temp;
	else
		head = temp;
	tail = temp;
	numEntries++;
}

template <class T>
inline void CTList<T>::Clear()
{
	while(head)
	{
		temp = head;
		head = head->next;
		delete temp;
	}
	tail = NULL;
	numEntries = 0;
}

template <class T>
inline void CTList<T>::ClearData()
{
	while(head)
	{
		temp = head;
		head = head->next;
		delete temp->payload;
		delete temp;
	}
	tail = NULL;
	numEntries = 0;
}

template <class T>
inline bool CTList<T>::IsEmpty()
{
	return numEntries == 0;
}

template <class T>
inline ulong CTList<T>::GetNumEntries()
{
	return numEntries;
}

template <class T>
inline T& CTList<T>::GetNext()
{
	assert(pos);
	pos = pos->next;
	assert(pos);
	return pos->payload;
}

template <class T>
inline T& CTList<T>::GetPrev()
{
	assert(pos);
	pos = pos->prev;
	assert(pos);
	return pos->payload;
}

template <class T>
inline bool CTList<T>::IsLast()
{
	assert(pos);
	return !pos->next;
}

template <class T>
inline bool CTList<T>::IsFirst()
{
	assert(pos);
	return !pos->prev;
}

#endif	//_CONTAINERS_H_
