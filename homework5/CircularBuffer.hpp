#pragma once

#include <exception>

template<class T>
class CircularBuffer {
private:
	T* data_;
	int capacity_;
	int size_;
	int tail_;
public:
	CircularBuffer(int capacity) {
		data_ = new T[capacity];
		capacity_ = capacity;
		size_ = 0;
		tail_ = 0;
		for (int i = 0; i < capacity_; ++i) {
			data_[i] = 0;
		}
	}
	~CircularBuffer() {
		delete[] data_;
		capacity_ = 0;
		size_ = 0;
		tail_ = 0;
	}
	class BufferIterator {
	private:
		T* iterator;
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;

		BufferIterator() : iterator(nullptr) {}
		BufferIterator(T* it) : iterator(it) {}
		BufferIterator(const BufferIterator& other) : iterator(other.iterator) {}
		T& operator*() const { return *iterator; }
		T* operator->() const { return iterator; }
		T& operator[](difference_type i) const { return iterator[i]; }
		BufferIterator& operator ++() {
			++iterator;
			return *this;
		}
		BufferIterator operator++(T) {
			BufferIterator temp(*this);
			++iterator;
			return temp;
		}
		BufferIterator& operator--() {
			--iterator;
			return *this;
		}
		BufferIterator operator--(T) {
			BufferIterator tmp(*this);
			--iterator;
			return tmp;
		}
		BufferIterator& operator+=(difference_type it) {
			iterator += it;
			return *this;
		}
		BufferIterator& operator-=(difference_type it) {
			iterator -= it;
			return *this;
		}
		BufferIterator operator+(difference_type it) const { return BufferIterator(iterator + it); }
		difference_type operator-(const BufferIterator& it) const { return iterator - it.iterator; }
		BufferIterator operator-(difference_type it) const { return BufferIterator(iterator - it); }
		bool operator==(const BufferIterator& other) const { return iterator == other.iterator; }
		bool operator!=(const BufferIterator& other) const { return iterator != other.iterator; }
		bool operator>(const BufferIterator& other) const { return iterator > other.iterator; }
		bool operator<(const BufferIterator& other) const { return iterator < other.iterator; }
		bool operator>=(const BufferIterator& other) const { return iterator >= other.iterator; }
		bool operator<=(const BufferIterator& other) const { return iterator <= other.iterator; }
	};

	BufferIterator begin() const {
		return BufferIterator(data_);
	}

	BufferIterator end() const {
		return BufferIterator(data_ + size_);
	}

	//fixed O(1)
	void addLast(T x) {
		if (tail_ >= capacity_) {
			tail_ = 0;
		}
		if (size_ == capacity_) {
			data_[tail_] = x;
		}
		else {
			data_[size_] = x;
			++size_;
		}
		++tail_;
	}

	void delLast() {
		if (size_ == 0) {
			throw std::out_of_range("No elements in CircularBuffer");
		}
		--size_;
		data_[size_] = 0;
		--tail_;
	}

	void addFirst(T x) {
		if (size_ == capacity_) {
			T* dat = new T[size_];
			dat[0] = x;
			for (int i = 1; i < size_; ++i) {
				dat[i] = data_[i - 1];
			}
			delete[] data_;
			data_ = dat;
		}
		else {
			T* dat = new T[capacity_];
			dat[0] = x;
			for (int i = 1; i <= size_; ++i) {
				dat[i] = data_[i - 1];
			}
			delete[] data_;
			data_ = dat;
			++size_;
		}
	}

	void delFirst() {
		if (size_ == 0) {
			throw std::out_of_range("No elements in CircularBuffer");
		}
		T* dat = new T[capacity_];
		for (int i = 0; i < size_ - 1; ++i) {
			dat[i] = data_[i + 1];
		}
		delete[] data_;
		data_ = dat;
		--size_;
		--tail_;
	}

	const T& first() {
		if (size_ == 0) {
			throw std::out_of_range("No elements in CircularBuffer");
		}
		return data_[0];
	}

	const T& last() {
		if (size_ == 0) {
			throw std::out_of_range("No elements in CircularBuffer");
		}
		return data_[size_ - 1];
	}

	//fixed exception more information
	T operator[] (int index) const {
		if (size_ == 0) {
			throw std::out_of_range("No elements in CircularBuffer");
		}
		if (index < 0) {
			throw (std::out_of_range("Incorrect index, index must be >=0"));
		}
		if (index > size_ - 1) {
			throw (std::out_of_range("Index is out of range"));
		}
		return data_[index];
	}

	T& operator[](int index) {
		if (size_ == 0) {
			throw std::out_of_range("No elements in CircularBuffer");
		}
		if (index < 0) {
			throw (std::out_of_range("Incorrect index, index must be >=0"));
		}
		if (index > size_ - 1) {
			throw (std::out_of_range("Index is out of range"));
		}
		return data_[index];
	}

	void changeCapacity(int nc) {
		if (nc <= capacity_) {
			throw std::bad_alloc();
		}
		T* dat = new T[nc];
		for (int i = 0; i < size_; ++i) {
			dat[i] = data_[i];
		}
		delete[] data_;
		data_ = dat;
		capacity_ = nc;
		tail_ = size_;
	}
};
