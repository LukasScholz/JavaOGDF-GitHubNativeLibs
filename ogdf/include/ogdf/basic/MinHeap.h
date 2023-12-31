/** \file
 * \brief Declares & Implements Binary Heap, and Top10Heap
 *
 * \author Markus Chimani
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 *
 * \par
 * Copyright (C)<br>
 * See README.md in the OGDF root directory for details.
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation;
 * see the file LICENSE.txt included in the packaging of this file
 * for details.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, see
 * http://www.gnu.org/copyleft/gpl.html
 */

#pragma once

#include <ogdf/basic/Array.h>
#include <ogdf/basic/comparer.h>

namespace ogdf {

//! Dynamically growing binary heap tuned for efficiency on a small interface (compared to BinaryHeap).
/**
 * It assumes that the data-elements are themselves comparable, i.e., the compare-function
 * of the items implicitly defines the keys. Hence this datastructure allows no key-changing
 * operations (decreaseKey, etc.).
 *
 * The heap grows (using doubling) dynamically, if there are more elements added. Furthermore,
 * BinaryHeapSimple allows to be directly indexed using traditional array-syntax, e.g., for iterating over
 * all its elements.
 *
 * If your intended datastructure does not offer a (suitable) compare function, but you have
 * certain key-values (scores, etc.), you may want to use the convenience-class
 * Prioritized < Score,X > to bind both together and use within BinaryHeapSimple.
 */
template<class X, class INDEX = int>
class BinaryHeapSimple {
private:
	Array<X, INDEX> data; // array starts at index 1
	INDEX num;

public:
	//! Construtor, giving initial array size
	explicit BinaryHeapSimple(INDEX size) : data(1, size), num(0) { }

	//! Returns true if the heap is empty
	bool empty() const { return num == 0; }

	//! Returns the number of elements in the heap
	INDEX size() const { return num; }

	//! empties the heap [O(1)]
	void clear() { num = 0; }

	//! Returns a reference to the top (i.e., smallest) element of the heap. It does not remove it. [Same as getMin(), O(1)]
	const X& top() const { return data[1]; }

	//! Returns a reference to the top (i.e., smallest) element of the heap. It does not remove it. [Same as top(), O(1)]
	inline const X& getMin() const { return top(); }

	//! Adds an element to the heap [Same as insert(), O(log n)]
	void push(X& x) {
		X y;
		if (num == capacity()) {
			data.grow(capacity(), y); // double the size & init with nulls
		}
		data[++num] = x;
		heapup(num);
	}

	//! Adds an element to the heap [Same as push(), O(log n)]
	inline void insert(X& x) { push(x); }

	//! Returns the top (i.e., smallest) element and removed it from the heap [Same as extractMin(), O(log n)]
	X pop() {
		data.swap(1, num--);
		heapdown();
		return data[num + 1];
	}

	//! Returns the top (i.e., smallest) element and removed it from the heap [Same as  pop(), O(log n)]
	inline X extractMin() { return pop(); }

	//! obtain const references to the element at index \p idx (the smallest array index is 0, as for traditional C-arrays)
	const X& operator[](INDEX idx) const { return data[idx + 1]; }


protected:
	//! Returns the current array-size of the heap, i.e., the number of elements which can be added before the next resize occurs.
	INDEX capacity() const { return data.size(); }

	void heapup(INDEX idx) {
		INDEX papa;
		while ((papa = idx / 2) > 0) {
			if (data[papa] > data[idx]) {
				data.swap(papa, idx);
				idx = papa;
			} else {
				return; //done
			}
		}
	}

	void heapdown() {
		INDEX papa = 1;
		INDEX son;
		while (true) {
			if ((son = 2 * papa) < num && data[son + 1] < data[son]) {
				son++;
			}
			if (son <= num && data[son] < data[papa]) {
				data.swap(papa, son);
				papa = son;
			} else {
				return;
			}
		}
	}
};

//! A variant of BinaryHeapSimple which always holds only the 10 elements with the highest keys.
/**
 * It assumes that the data-elements are themselves comparable, i.e., the compare-function
 * of the items implicitly defines the keys.
 *
 * If your intended datastructure do not directly offer a compare function, but you have
 * certain key-values (scores, etc.), you may want to use the convenience-class
 * Prioritized < Priority,X > to bind both together and use within BinaryHeapSimple.
 */
template<class X, class INDEX = int>
class Top10Heap : protected BinaryHeapSimple<X, INDEX> { // favors the 10 highest values...
public:
	//! The type for results of a Top10Heap::push operation
	enum class PushResult { Accepted, Rejected, Swapped };

	//! Convenience function: Returns true if the PushResults states that the newly pushed element is new in the heap
	static bool successful(PushResult r) { return r != PushResult::Rejected; }

	//! Convenience function: Returns true if the PushResults states that push caused an element to be not/no-longer in the heap
	static bool returnedSomething(PushResult r) { return r != PushResult::Accepted; }

	//! Constructor generating a heap which holds the 10 elements with highest value ever added to the heap
	Top10Heap() : BinaryHeapSimple<X, INDEX>(10) { }

	using BinaryHeapSimple<X, INDEX>::BinaryHeapSimple;

	//! Returns true if the heap is completely filled (i.e. the next push operation will return something)
	bool full() const { return size() == capacity(); }

	using BinaryHeapSimple<X, INDEX>::empty;
	using BinaryHeapSimple<X, INDEX>::size;
	using BinaryHeapSimple<X, INDEX>::capacity;
	using BinaryHeapSimple<X, INDEX>::clear;
	using BinaryHeapSimple<X, INDEX>::top;

	//! Tries to push the element \p x onto the heap (and may return a removed element as \p out).
	/**
	 * If the heap is not yet completely filled, the pushed element is accepted and added to the heap.
	 * The function returns PushResult::Accepted, and the \p out parameter is not touched.
	 *
	 * If the heap is filled and the key of the pushed element is too small to be accepted
	 * (i.e. the heap is filled with all larger elements), then the element if rejected: The funtion
	 * returns PushResult::Rejected, and the \p out parameter is set to \p x.
	 *
	 * If the heap is filled and the key of the pushed element is large enough to belong to the top
	 * elements, the element is accepted and the currently smallest element in the heap is removed
	 * from the heap. The function returns PushResult::Swapped and sets the \p out parameter to the element
	 * removed from the heap.
	 *
	 * You may want to use the convenience funtions #successful and #returnedSomething on the
	 * return-value if you are only interested certain aspects of the push.
	 */
	PushResult push(X& x, X& out) {
		// returns element that got kicked out -
		// out is uninitialized if heap wasn't full (i.e. PushResult equals Accepted)
		PushResult ret = PushResult::Accepted;
		if (capacity() == size()) {
			if (BinaryHeapSimple<X, INDEX>::top() >= x) { // reject new item since it's too bad
				out = x;
				return PushResult::Rejected;
			}
			out = BinaryHeapSimple<X, INDEX>::pop(); // remove worst first
			ret = PushResult::Swapped;
		}
		BinaryHeapSimple<X, INDEX>::push(x);
		return ret;
	}

	//! Alternative name for push().
	inline PushResult insert(X& x, X& out) { return push(x, out); }

	//! Simple (and slightly faster) variant of Top10Heap::push.
	/**
	 * The behavior is the identical to Top10Heap::push, but there is nothing reported to the outside
	 */
	void pushBlind(X& x) {
		if (capacity() == size()) {
			if (BinaryHeapSimple<X, INDEX>::top() >= x) { // reject new item since it's too bad
				return;
			}
			BinaryHeapSimple<X, INDEX>::pop(); // remove worst first
		}
		BinaryHeapSimple<X, INDEX>::push(x);
	}

	//! Alternative name for pushBlind().
	inline void insertBlind(X& x) { pushBlind(x); }

	//! obtain const references to the element at index \p idx
	/**
	 * The smallest array index is 0, as for traditional C-arrays.
	 * Useful, e.g., when iterating through the final heap elements.
	 */
	const X& operator[](
			INDEX idx) const { // ATTN: simulate index starting at 0, to be "traditional" to the outside!!!
		return BinaryHeapSimple<X, INDEX>::operator[](idx);
	}
};

//! A variant of Top10Heap which deletes the elements that get rejected from the heap
/**
 * The datastructure of course requires the stored data-elements to be pointers (in order to be deletable when
 * rejected). Hence the template parameter only specifies the data-type, without stating axplicitly that we
 * considere pointers to the structure.
 *
 * The datastructure also allows for non-duplicate insertions, i.e., a new element can be rejected if it is
 * already in the heap. Note that only the compare function has to work
 */
template<class X, class Priority = double, class STATICCOMPARER = StdComparer<X>, class INDEX = int>
class DeletingTop10Heap : public Top10Heap<Prioritized<X*, Priority>, INDEX> {
public:
	//! Construct a DeletingTop10Heap of given maximal capacity
	explicit DeletingTop10Heap(int size) : Top10Heap<Prioritized<X*, Priority>, INDEX>(size) { }

	//! Inserts the element \p x into the heap with priority \p p and deletes the element with smallest priority if the heap is full
	/**
	 * Like the Top10Heap, this function pushes the element \p x onto the heap with priority \p p, and extracts the element with
	 * smallest priority if the heap was already full. In contrast to the Top10Heap, this element which leaves the heap (or \p x
	 * itself if its priority was below all the priorities in the heap) gets deleted, i.e., removed from memory.
	 */
	void pushAndDelete(X* x, Priority p) {
		Prioritized<X*, Priority> vo;
		Prioritized<X*, Priority> nv(x, p);
		if (this->returnedSomething(Top10Heap<Prioritized<X*, Priority>, INDEX>::push(nv, vo))) {
			delete vo.item();
		}
	}

	//! Alternative name for pushAndDelete().
	inline void insertAndDelete(X* x, Priority p) { pushAndDelete(x, p); }

	//! Analogous to pushandDelete(), but furthermore rejects (and deletes) an element if an equal element is already in the heap.
	/**
	 * This function takes linear time in the worst case, and uses the \c compare function of the specified \a COMP template
	 * paremeter class, which can be any function returning \c true if two objects should be considered equal, and \c false otherwise.
	 */
	void pushAndDeleteNoRedundancy(X* x, Priority p) {
		for (INDEX i = Top10Heap<Prioritized<X*, Priority>, INDEX>::size(); i-- > 0;) {
			X* k = Top10Heap<Prioritized<X*, Priority>, INDEX>::operator[](i).item();
#if 0
			OGDF_ASSERT(x);
			OGDF_ASSERT(k);
#endif
			if (TargetComparer<X, STATICCOMPARER>::equal(k, x)) {
				delete x;
				return;
			}
		}
		pushAndDelete(x, p);
	}

	//! Alternative name for pushAndKillNoRedundancy().
	inline void insertAndDeleteNoRedundancy(X* x, Priority p) { pushAndDeleteNoRedundancy(p, x); }
};

}
