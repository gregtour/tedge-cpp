#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

template <class T> class CLinkedList;

template <class T> class CListEntry
{
public:
	CListEntry();
	CListEntry( T* );
	~CListEntry();

	void Insert( T* data );
	void Remove();
	CListEntry<T>* GetNext();
	CListEntry<T>* GetPrev();

	T* data;

	CListEntry<T>* prev;
	CListEntry<T>* next;
	CLinkedList<T>* parent;
};

template <class T> class CLinkedList
{
public:
	CLinkedList();
	~CLinkedList();

	void Add( T* data );
	void Add( T* data, int index );
	void RemoveEntry( CListEntry<T>* l );
	CListEntry<T>* FindEntry( T* data );
	int GetLength();
	CListEntry<T>* GetFirst();
	CListEntry<T>* GetLast();

	int length;
	CListEntry<T>* first;
	CListEntry<T>* last;
};

// Templated Class, Need To Include Implementation Directly
// SHOULD NOT BE COMPILED AS ITS OWN .O! //
// #include "list.h"

#ifndef NULL
	#define NULL 0
#endif

template <class T>
CLinkedList<T>::CLinkedList()
{
	length = 0;
	first = NULL;
	last = NULL;
}

template <class T>
CLinkedList<T>::~CLinkedList()
{
	CListEntry<T>* l = first;
	while ( l )
	{
		RemoveEntry( l );
		l = first;
	}
}

template <class T>
void CLinkedList<T>::Add( T* data )
{
	if ( first && last )
	{
		last->next = new CListEntry<T>( data );
		last->next->prev = last;
		last = last->next;
		length++;
	} else {
		first = new CListEntry<T>( data );
		last = first;
		first->next = NULL;
		first->prev = NULL;
		length++;
	}
	
	last->parent = this;
}

template <class T>
void CLinkedList<T>::Add( T* data, int index )
{
	if ( index == 0 )
	{
		if ( first && last )
		{
			CListEntry<T>* newEntry = new CListEntry<T>( data );
			newEntry->prev = NULL;
			newEntry->next = first;
			newEntry->parent = this;
			first->prev = newEntry;
			first = newEntry;
		} else {
			CListEntry<T>* newEntry = new CListEntry<T>( data );
			newEntry->prev = NULL;
			newEntry->next = NULL;
			newEntry->parent = this;
			first = newEntry;
			last = newEntry;
		}
		length++;
	} else if ( index < length ) {
		CListEntry<T>* insertionPoint = GetFirst();

		for ( index--; index > 0; index-- )
			insertionPoint = insertionPoint->GetNext();

		insertionPoint->Insert( data );
	} else if ( index == length ) {
		Add( data );
	}
}


template <class T>
void CLinkedList<T>::RemoveEntry( CListEntry<T>* l )
{
	if ( l == first )
		first = l->next;
	if ( l == last )
		last = l->prev;
	delete l;
	length--;
}

template <class T>
CListEntry<T>* CLinkedList<T>::FindEntry( T* data )
{
	CListEntry<T>* e = GetFirst();
	
	while ( e && e->data != data )
	{
		e = e->GetNext();
	}

	return e;
}

template <class T>
int CLinkedList<T>::GetLength()
{
	return length;
}

template <class T>
CListEntry<T>* CLinkedList<T>::GetFirst()
{
	return first;
}

template <class T>
CListEntry<T>* CLinkedList<T>::GetLast()
{
	return last;
}

template <class T>
CListEntry<T>::CListEntry()
{
	data = NULL;
	prev = NULL;
	next = NULL;
}

template <class T>
CListEntry<T>::CListEntry( T* data )
{
	this->data = data;
	prev = NULL;
	next = NULL;
}

template <class T>
CListEntry<T>::~CListEntry()
{
	Remove();
}


template <class T>
void CListEntry<T>::Insert( T* data )
{
	CListEntry<T>* newEntry = new CListEntry<T>( data );
	newEntry->next = next;
	if ( next )
		next->prev = newEntry;
	newEntry->parent = parent;
	newEntry->prev = this;
	next = newEntry;
	parent->length++;
}


template <class T>
void CListEntry<T>::Remove()
{
	if ( prev )
		prev->next = next;
	if ( next )
		next->prev = prev;
}

template <class T>
CListEntry<T>* CListEntry<T>::GetNext()
{
	return next;
}

template <class T>
CListEntry<T>* CListEntry<T>::GetPrev()
{
	return prev;
}


#endif
