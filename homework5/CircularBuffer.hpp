#pragma once

#include <exception>
#include <sstream>

template<class T>
class CircularBuffer {
private:
	T* data_;
	int capacity_;
	int size_;
	size_t head_;
	size_t tail_;

	void incTail() {
		if (tail_ == capacity_ - 1)
			tail_ = 0;
		else ++tail_;
	};
	void decTail() {
		if (tail_ == 0)
			tail_ = capacity_ - 1;
		else --tail_;
	};
	void incHead() {
		if (head_ == capacity_ - 1)
			head_ = 0;
		else ++head_;
	};
	void decHead() {
		if (head_ == 0)
			head_ = capacity_ - 1;
		else --head_;
	};

public:
	CircularBuffer(int capacity) {
		data_ = new T[capacity];
		capacity_ = capacity;
		size_ = 0;
		head_ = 0;
		tail_ = 0;
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
		T* _data;
		size_t _tail;
		size_t _index;
		size_t _capacity;
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;

		BufferIterator() : iterator(nullptr) {}
		BufferIterator(T* it, T* buffer, size_t tail, size_t index, size_t capacity) : iterator(it), _data(buffer), _tail(tail), _index(index), _capacity(capacity) {}
		BufferIterator(const BufferIterator& other) : iterator(other.iterator), _data(other._data), _tail(other._tail), _index(other._index), _capacity(other._capacity) {}
		T& operator*() const { return _data[(_tail - _index) % _capacity]; }
		T* operator->() const { return iterator; }
		T& operator[](difference_type i) const { return iterator[i]; }
		BufferIterator& operator ++() {
			++iterator;
			++_index;
			return *this;
		}
		BufferIterator operator++(T) {
			BufferIterator tmp(*this);
			++iterator;
			++_index;
			return tmp;
		}
		BufferIterator& operator--() {
			--iterator;
			--_index;
			return *this;
		}
		BufferIterator operator--(T) {
			BufferIterator tmp(*this);
			--iterator;
			--_index;
			return tmp;
		}
		BufferIterator& operator+=(difference_type it) {
			iterator += it;
			_index += it;
			return *this;
		}
		BufferIterator& operator-=(difference_type it) {
			iterator -= it;
			_index -= it;
			return *this;
		}
		BufferIterator operator+(difference_type it) const { return BufferIterator(iterator + it, _data, _tail, _index + it, _capacity); }
		difference_type operator-(const BufferIterator& it) const { return iterator - it.iterator; }
		BufferIterator operator-(difference_type it) const { return BufferIterator(iterator - it, _data, _tail, _index - it, _capacity); }
		bool operator==(const BufferIterator& other) const { return iterator == other.iterator; }
		bool operator!=(const BufferIterator& other) const { return iterator != other.iterator; }
		bool operator>(const BufferIterator& other) const { return iterator > other.iterator; }
		bool operator<(const BufferIterator& other) const { return iterator < other.iterator; }
		bool operator>=(const BufferIterator& other) const { return iterator >= other.iterator; }
		bool operator<=(const BufferIterator& other) const { return iterator <= other.iterator; }
	};

	BufferIterator begin() const {
		if (tail_ != 0)
			return BufferIterator(&data_[capacity_ - size_], data_, tail_ - 1, 0, capacity_);
		else
			return BufferIterator(&data_[capacity_ - size_], data_, capacity_ - 1, 0, capacity_);
	}

	BufferIterator end() const {
		if (tail_ != 0)
			return BufferIterator(&data_[capacity_], data_, tail_ - 1, 0, capacity_);
		else
			return BufferIterator(&data_[capacity_], data_, capacity_ - 1, 0, capacity_);
	}

	void addLast(T x) {
		if (size_ == capacity_)
			data_[head_] = x;
		else {
			++size_;
			decHead();
			data_[head_] = x;
		}
	}

	void delLast() {
		if (size_ == 0) {
			throw std::out_of_range("No elements in CircularBuffer");
		}
		--size_;
		incHead();
	}

	//fixed O(1)
	void addFirst(T x) {
		data_[tail_] = x;
		if (size_ == capacity_) {
			incTail();
			incHead();
		}
		else {
			++size_;
			incTail();
		}
	}

	void delFirst() {
		if (size_ == 0) {
			throw std::out_of_range("No elements in CircularBuffer");
		}
		--size_;
		decTail();
	}

	const T& first() {
		if (size_ == 0) {
			throw std::out_of_range("No elements in CircularBuffer");
		}
		if (tail_ != 0)
			return data_[tail_ - 1];
		else return data_[capacity_ - 1];
	}

	const T& last() {
		if (size_ == 0) {
			throw std::out_of_range("No elements in CircularBuffer");
		}
		return data_[head_];
	}

	//fixed infomation index and size
	T operator[] (int index) const {
		if (size_ == 0) {
			throw std::out_of_range("No elements in CircularBuffer");
		}
		if (index < 0) {
			throw (std::out_of_range("Incorrect index, index must be >=0"));
		}
		if (index > size_ - 1) {
			std::stringstream ss;
			ss << "Index = " << index << " is out of range, it must be < size(" << size_ << ")";
			std::string s = ss.str();
			const char* error = s.c_str();
			throw (std::out_of_range(error));
		}
		return data_[(tail_ - index - 1) % capacity_];
	}

	T& operator[](int index) {
		if (size_ == 0) {
			throw std::out_of_range("No elements in CircularBuffer");
		}
		if (index < 0) {
			throw (std::out_of_range("Incorrect index, index must be >=0"));
		}
		if (index > size_ - 1) {
			std::stringstream ss;
			ss << "Index = " << index << " is out of range, it must be < size(" << size_ << ")";
			std::string s = ss.str();
			const char* error = s.c_str();
			throw (std::out_of_range(error));
		}
		return data_[(tail_ - index - 1) % capacity_];
	}

	void changeCapacity(int nc) {
		if (nc <= capacity_) {
			throw std::bad_alloc();
		}
		T* dat = new T[nc];
		int temp = 0;
		if (head_ >= tail_) {
			for (int i = head_; i < capacity_; ++i) {
				dat[temp] = data_[i];
				++temp;
			}
			for (int i = 0; i < tail_; ++i) {
				dat[temp] = data_[i];
				++temp;
			}
		}
		else
			for (int i = head_; i < tail_; ++i) {
				dat[temp] = data_[i];
				++temp;
			}
		delete[] data_;
		data_ = dat;
		capacity_ = nc;
		head_ = 0;
		tail_ = size_;
	}
};
