/**

Jinxiu Liu
Date: 4/4/2019
Professor: Ioannis Stamos
This is a h file called double_hashing.h


Part2:  Everything is similar to qudratic_probing.h 
       Except: Change FindPos() function to make double_hashing

**/

#ifndef double_hashing_H
#define double_hashing_H

#include <vector>
#include <algorithm>
#include <functional>
#include <math.h>


namespace {

	// Internal method to test if a positive number is prime.
	bool IsPrimeD(size_t n) {
		if (n == 2 || n == 3)
			return true;

		if (n == 1 || n % 2 == 0)
			return false;

		for (int i = 3; i * i <= n; i += 2)
			if (n % i == 0)
				return false;

		return true;
	}


	// Internal method to return a prime number at least as large as n.
	int NextPrimeD(size_t n) {
		if (n % 2 == 0)
			++n;
		while (!IsPrimeD(n)) n += 2;
		return n;
	}

}  // namespace


   // Quadratic probing implementation.
template <typename HashedObj>
class HashTableDouble {
public:
	enum EntryType { ACTIVE, EMPTY, DELETED };

	explicit HashTableDouble(size_t size = 101) : array_(NextPrimeD(size))
	{
		MakeEmpty();
	}

	// an additional argument was added to keep tract of # of collisions when using FindPos
	bool Contains(const HashedObj & x, int &y) const {
		return IsActive(FindPos(x, y));
	}

	void MakeEmpty() {
		current_size_ = 0;
		for (auto &entry : array_)
			entry.info_ = EMPTY;
	}

	// an additional argument was added to keep tract of # of collisions when using FindPos in Insert function
	bool Insert(const HashedObj & x, int &countCollisions) {
		// Insert x as active
		size_t current_pos = FindPos(x, countCollisions);
		if (IsActive(current_pos))
			return false;
		array_[current_pos].element_ = x;
		array_[current_pos].info_ = ACTIVE;

		// Rehash; see Section 5.5
		if (++current_size_ > array_.size() / 2)
			Rehash(countCollisions);
		return true;
	}

	//return the numbers of elements in the table 
	int GetItemSize()
	{
		return current_size_;
	}
	//return the size of the table
	int GetTableSize()
	{
		return array_.size();
	}

	// an additional argument was added to keep tract of # of collisions when using FindPos in Insert Function
	bool Insert(HashedObj && x, int &countCollisions) {
		// Insert x as active
		size_t current_pos = FindPos(x, countCollisions);
		if (IsActive(current_pos))
			return false;
		array_[current_pos] = std::move(x);
		array_[current_pos].info_ = ACTIVE;

		// Rehash; see Section 5.5
		if (++current_size_ > array_.size() / 2)
			Rehash( countCollisions);
		return true;
	}

	// an additional argument was added to keep tract of # of collisions, in a case that FindPos was called 
	bool Remove(const HashedObj & x, int &countCollisions) {
		size_t current_pos = FindPos(x, countCollisions);
		if (!IsActive(current_pos))
			return false;

		array_[current_pos].info_ = DELETED;
		return true;
	}

private:
	struct HashEntry {
		HashedObj element_;
		EntryType info_;

		HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
			:element_{ e }, info_{ i } { }

		HashEntry(HashedObj && e, EntryType i = EMPTY)
			:element_{ std::move(e) }, info_{ i } {}
	};


	std::vector<HashEntry> array_;
	size_t current_size_;

	bool IsActive(size_t current_pos) const
	{
		return array_[current_pos].info_ == ACTIVE;
	}


	//hash2()  function will perform  R-(x%R)
    size_t hash2(const HashedObj & x) const {
        static std::hash<HashedObj> hf;
        return 73 - (hf(x) % 73);
    }

	// an additional argument was added to keep tract of # of collisions 
	//use (hash(x)+i*hash2(x)) %size
	size_t FindPos(const HashedObj & x, int &countCollisions) const {
		
        size_t current_pos=InternalHash(x);
        int count = 1;                                                          // count= i, which will increment each time in the while loop
		while (array_[current_pos].info_ != EMPTY &&
			array_[current_pos].element_ != x) {
			countCollisions++;                                                 // Increment countCollisions by 1
			current_pos = (InternalHash(x) + count*hash2(x))% array_.size();   //calling hash2() function and perform other works
            count++;
			 // Compute ith probe.
			if (current_pos >= array_.size())
				current_pos -= array_.size();
		}
		return current_pos;
	}

	// an additional argument was added to keep tract of # of collisions, in a case that Rehash call insert function
	void Rehash( int&countCollisions) {
		std::vector<HashEntry> old_array = array_;

		// Create new double-sized, empty table.
		array_.resize(NextPrimeD(2 * old_array.size()));
		for (auto & entry : array_)
			entry.info_ = EMPTY;

		// Copy table over.
		current_size_ = 0;
		for (auto & entry : old_array)
			if (entry.info_ == ACTIVE)
				Insert(std::move(entry.element_), countCollisions);
	}

	size_t InternalHash(const HashedObj & x) const {
		static std::hash<HashedObj> hf;
		return hf(x) % array_.size();
	}
};

#endif  // double_hashing_H
