#ifndef Artemis_Bag_h__
#define Artemis_Bag_h__

#include <cstring>
#include <iostream>

namespace artemis
{
/**
 * Collection type a bit like ArrayList but does not preserve the order of its
 * entities, speedwise it is very good, especially suited for games.
 *
 * @port   Vladimir Ivanov (ArCorvus)
 */

template<typename E>
class Bag
{
public:
    virtual void dummy() = 0;
};

template<typename E>
class Bag<E*> /*final*/
{
private:
    E**mData;
    size_t mSize;
    size_t mCapacity;

    void init(size_t capacity)
    {
        mCapacity = capacity;
        mSize = 0;
        mData = new E*[capacity];
        clear();
    }

public:
    /**
     * Constructs an empty Bag with an initial capacity of 64.
     *
     */
    Bag()
    {
        init(64);
    }

    /**
     * Constructs an empty Bag with the specified initial capacity.
     *
     * @param capacity
     *            the initial capacity of Bag
     */
    Bag(int capacity)
    {
        init(capacity);
    }

    ~Bag()
    {
        delete[] mData;
    }

    /**
     * Removes the element at the specified position in this Bag. does this by
     * overwriting it was last element then removing last element
     *
     * @param index
     *            the index of element to be removed
     * @return element that was removed from the Bag
     */
    E* remove(int index)
    {
    	E* e = mData[index]; // make copy of element* to remove so it can be returned
    	mData[index] = mData[--size]; // overwrite item to remove with last element
    	mData[size] = nullptr; // null last element*
    	return e;
    }


    /**
     * Remove and return the last object in the bag.
     *
     * @return the last object in the bag, null if empty.
     */
    E* removeLast()
    {
        if (mSize > 0)
        {
            E* e = mData[--mSize];
            mData[mSize] = nullptr;
            return e;
        }

        return nullptr;
    }

    /**
     * Removes the first occurrence of the specified element from this Bag, if
     * it is present. If the Bag does not contain the element, it is unchanged.
     * does this by overwriting it was last element then removing last element
     *
     * @param e
     *            element to be removed from this list, if present
     * @return <tt>true</tt> if this list contained the specified element
     */
    bool remove(const E *e)
    {
        for (size_t i = 0; i < mSize; ++i)
        {
            if (e == mData[i])
            {
                mData[i] = mData[--mSize]; // overwrite item to remove with last element
                mData[mSize] = nullptr; // null last element
                return true;
            }
        }

        return false;
    }

    /**
     * Check if bag contains this element.
     *
     * @param e
     * @return
     */
    bool contains(const E *e) const
    {
        for (size_t i = 0; i < mSize; ++i)
        {
            if (e == mData[i])
            {
                return true;
            }
        }
        return false;
    }


    /**
     * Removes from this Bag all of its elements that are contained in the
     * specified Bag.
     *
     * @param bag
     *            Bag containing elements to be removed from this Bag
     * @return {@code true} if this Bag changed as a result of the call
     */
    bool removeAll(const Bag<E*> &bag)
    {
        bool modified = false;

        for (int i = 0; i < bag.size(); ++i)
        {
            const E* &e1 = bag.get(i);

            for (int j = size-1; j >= 0; --j)
            {
                if (e1 == mData[j])
                {
                    remove(j);
                    modified = true;
                    break;
                }
            }
        }

        return modified;
    }



    /**
     * Returns the element at the specified position in Bag.
     *
     * @param index
     *            index of the element to return
     * @return the element at the specified position in bag
     */
    E* get(int index) const
    {
        return mData[index];
    }

    /**
     * Returns the number of elements in this bag.
     *
     * @return the number of elements in this bag
     */
    size_t size() const
    {
        return mSize;
    }

    /**
     * Returns the number of elements the bag can hold without growing.
     *
     * @return the number of elements the bag can hold without growing.
     */
    int capacity() const
    {
        return mCapacity;
    }

    /**
     * Checks if the internal storage supports this index.
     *
     * @param index
     * @return
     */
    bool isIndexWithinBounds(int index) const
    {
        return index < capacity();
    }

    /**
     * Returns true if this list contains no elements.
     *
     * @return true if this list contains no elements
     */
    bool isEmpty() const
    {
        return mSize == 0;
    }

    /**
     * Adds the specified element to the end of this bag. if needed also
     * increases the capacity of the bag.
     *
     * @param e
     *            element to be added to this list
     */
    void add(E* e)
    {
        // is size greater than capacity increase capacity
        if (mSize == mCapacity)
        {
            grow();
        }

        mData[mSize++] = e;
    }

    /**
     * Set element at specified index in the bag.
     *
     * @param index position of element
     * @param e the element
     */
    void set(size_t index, E* e)
    {
        if (index >= mCapacity)
        {
            grow(index*2);
        }

        if (mSize < index+1)
            mSize = index+1;

        mData[index] = e;
    }

    void grow()
    {
        int newCapacity = (mCapacity * 3) / 2 + 1;
        grow(newCapacity);
    }

    void grow(int newCapacity)
    {
        E** oldData = mData;
        mData = new E*[newCapacity];
        std::memset(mData, 0, sizeof(E*)*newCapacity);
        std::memcpy(mData, oldData, mCapacity);
        delete[] oldData;
        mCapacity = newCapacity;
    }

    void ensureCapacity(size_t index)
    {
        if (index >= mCapacity)
        {
            grow(index*2);
        }
    }

    /**
     * Removes all of the elements from this bag. The bag will be empty after
     * this call returns.
     */
    void clear()
    {
        // null all elements so gc can clean up
        std::memset(mData, 0, sizeof(E*)*mCapacity);
// 		for (size_t i = 0; i < size; i++) {
// 			data[i] = nullptr;
// 		}

        mSize = 0;
    }

    /**
     * Add all items into this bag.
     * @param added
     */
    void addAll(const Bag<E*> &items)
    {
    	for (int i = 0; items.size() > i; ++i) {
    		add(items.get(i));
    	}
    }
    //*/
};
}
#endif // Artemis_Bag_h__
