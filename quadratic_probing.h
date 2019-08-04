/**

Jinxiu Liu
Date: 4/4/2019
Professor: Ioannis Stamos
This is a h file called quadratic_probing.h

Part1:  This file is provided by professor 
		Professor Ioannis Stamos, everything stays same excpet, adding additional argument to few functions
		to find number of collisions and adding GetItemSize() and GetTableSize() to get # of elements in table and size of table

**/

#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>
#include <math.h>


namespace {

	// Internal method to test if a positive number is prime.
	bool IsPrime(size_t n) {
		if( n == 2 || n == 3 )
			return true;
  
		if( n == 1 || n % 2 == 0 )
			return false;
  
		for( int i = 3; i * i <= n; i += 2 )
			if( n % i == 0 )
				return false;
  
		return true;
	}


	// Internal method to return a prime number at least as large as n.
	int NextPrime(size_t n) {
		if (n % 2 == 0)
			++n;  
		while (!IsPrime(n)) n += 2;  
		return n;
	}

}  // namespace


// Quadratic probing implementation.
template <typename HashedObj>
class HashTable {
public:
	enum EntryType {ACTIVE, EMPTY, DELETED};

	explicit HashTable(size_t size = 101) : array_(NextPrime(size))
	{ MakeEmpty(); }
  
	// an additional argument was added to keep tract of # of collisions when using FindPos in the function
	bool Contains(const HashedObj & x, int &y) const {
		return IsActive(FindPos(x,y));
	}
  
	void MakeEmpty() {
		current_size_ = 0;
		for (auto &entry : array_)
			entry.info_ = EMPTY;
	}
	// an additional argument was added to keep tract of # of collisions when using FindPos in the function
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

	// an additional argument was added to keep tract of # of collisions when using FindPos in the function
	bool Insert(HashedObj && x, int &countCollisions) {
		// Insert x as active
		size_t current_pos = FindPos(x, countCollisions);
		if (IsActive(current_pos))
			return false;
		array_[current_pos] = std::move(x);
		array_[current_pos].info_ = ACTIVE;

		// Rehash; see Section 5.5
		if (++current_size_ > array_.size() / 2)
			Rehash(countCollisions);

		return true;
	}

	// an additional argument was added to keep tract of # of collisions when using FindPos in the function
	bool Remove(const HashedObj & x,int &countCollisions) {
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
		:element_{e}, info_{i} { }
    
		HashEntry(HashedObj && e, EntryType i = EMPTY)
		:element_{std::move(e)}, info_{ i } {}
	};
    

	std::vector<HashEntry> array_;
	size_t current_size_;
  

	bool IsActive(size_t current_pos) const
	{ return array_[current_pos].info_ == ACTIVE; }

	// an additional argument was added to keep tract of # of collisions 
	size_t FindPos(const HashedObj & x,int & countCollisions) const {
		size_t offset = 1;
		size_t current_pos = InternalHash(x);
      
		while (array_[current_pos].info_ != EMPTY &&
		array_[current_pos].element_ != x) {
			countCollisions++;
			current_pos += offset;//Compute ith probe.
			offset += 2;
			if (current_pos >= array_.size())
				current_pos -= array_.size();
		}
		return current_pos;
	}

	// an additional argument was added to keep tract of # of collisions when calling Insert function
	void Rehash(int&countCollisions) {
		std::vector<HashEntry> old_array = array_;

		// Create new double-sized, empty table.
		array_.resize(NextPrime(2 * old_array.size()));
		for (auto & entry : array_)
			entry.info_ = EMPTY;
    
		// Copy table over.
		current_size_ = 0;
		for (auto & entry :old_array)
			if (entry.info_ == ACTIVE)
				Insert(std::move(entry.element_),countCollisions);
	}
  
	size_t InternalHash(const HashedObj & x) const {
		static std::hash<HashedObj> hf;
		return hf(x) % array_.size( );
	}
};

#endif  // QUADRATIC_PROBING_H
