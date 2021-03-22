#include <iostream>
#include "polynomial.h"
#include <sstream>
#include <assert.h>
#include <cmath>

Polynomial::Polynomial(int min, int max, int* pol) {
	max_k = max;
	min_k = min;
	pol_ = new int[max_k - min_k + 1];
	for (int i = 0; i < max_k - min_k + 1; i++)
		pol_[i] = pol[i];
}
Polynomial::Polynomial() {
	max_k = 0;
	min_k = 0;
	pol_ = new int[1];
	pol_[0] = 0;
}
Polynomial::Polynomial(const Polynomial& other) {
	max_k = other.max_k;
	min_k = other.min_k;
	pol_ = new int[max_k - min_k + 1];
	for (int i = 0; i < max_k - min_k + 1; i++)
		pol_[i] = other.pol_[i];
} 
Polynomial& Polynomial::operator=(const Polynomial& other) {
	if (&other == this)
		return *this;
	delete[]pol_;
	max_k = other.max_k;
	min_k = other.min_k;
	pol_ = new int[max_k - min_k + 1];
	for (int i = 0; i < max_k - min_k + 1; i++)
		pol_[i] = other.pol_[i];
	return *this;
}
Polynomial::~Polynomial() {
	delete[]pol_;
}
Polynomial& Polynomial::operator+=(const Polynomial& rhs) {
	if ((this->max_k == rhs.max_k) && (this->min_k == rhs.min_k)) {
		for (int i = 0; i < max_k - min_k + 1; i++)
			this->pol_[i] += rhs.pol_[i];
		this->check_null();
	}
	else
		*this = operator+(*this,rhs);
	return* this;
}
Polynomial& Polynomial::operator-=(const Polynomial& rhs) {
	*this += -rhs;
	return *this;
}
Polynomial& Polynomial::operator*=(const Polynomial& rhs) {
	*this = operator*(*this, rhs);
	return *this;
}
Polynomial& Polynomial::operator/=(int value) {
	for (int i = 0; i < max_k - min_k + 1; i++)
		pol_[i] /= value;
	this->check_null();
	return *this;
}
int& Polynomial::operator[](int index) const {
	int temp = 0;
	if ((index > max_k) || (index < min_k))
		return temp;
	else
		return pol_[index - min_k];
}
int& Polynomial::operator[](int index) {
	if (index > max_k) {
		int* temp = new int[max_k-min_k+1];
		for (int i = 0; i < max_k - min_k + 1; i++)
			temp[i] = pol_[i];
		delete[] pol_;
		pol_ = new int[index - min_k + 1];
		for (int i = 0; i < index - min_k + 1; i++)
			pol_[i] = 0;
		for (int i = 0; i < max_k - min_k + 1; i++)
			pol_[i] = temp[i];
		delete[]temp;
		max_k = index;
		return pol_[max_k - min_k];
	}
	if (index < min_k) {
		int* temp = new int[max_k - min_k + 1];
		for (int i = 0; i < max_k - min_k + 1; i++)
			temp[i] = pol_[i];
		delete[] pol_;
		pol_ = new int[max_k - index + 1];
		for (int i = 0; i < max_k-index + 1; i++)
			pol_[i] = 0;
		for (int i = min_k-index, j =0; i < max_k - index + 1; i++, j++)
			pol_[i] = temp[j];
		delete[]temp;
		min_k = index;
		return pol_[0];
	}
	return pol_[index - min_k];
}
double Polynomial::get(int value) {
	double res = 0;
	for (int i = 0; i < max_k - min_k + 1;i++) {
		res += pol_[i] * pow(value, i + min_k);
	}
	return res;
}
Polynomial Polynomial::check_null() {
	Polynomial res(min_k, max_k, pol_);
	int i = res.max_k-res.min_k;
	while (res.pol_[i] == 0 && i > -1) {
		i--;
	}
	if (i == -1) {
		res.min_k = 0;
		res.max_k = 0;
		delete[]res.pol_;
		res.pol_ = new int[res.max_k-res.min_k+1];
		res.pol_[0] = 0;
	}
	else {
		res.max_k = i + res.min_k;
		int i = 0;
		while (res.pol_[i] == 0) {
			i++;
		}
		res.min_k += i;
		int* help = new int[res.max_k - res.min_k+1];
		for (int j = 0; j < res.max_k - res.min_k + 1; j++) {
			help[j] = res.pol_[j + i];
		}
		delete[]res.pol_;
		res.pol_ = new int[res.max_k - res.min_k + 1];
		for (i = 0; i < res.max_k - res.min_k + 1; i++) {
			res.pol_[i] = help[i];
		}
		delete[]help;
	}
	*this = res;
	return *this;
}
bool operator==(const Polynomial& lhs, const Polynomial& rhs) {
	Polynomial lhs1(lhs.min_k, lhs.max_k, lhs.pol_);
	lhs1.check_null();
	Polynomial rhs1(rhs.min_k, rhs.max_k, rhs.pol_);
	rhs1.check_null();
	if ((lhs1.max_k != rhs1.max_k) || (lhs1.min_k != rhs1.min_k))
		return false;
	else {
		for (int i = 0; i < lhs1.max_k-lhs1.min_k+1;i++)
			if (lhs1.pol_[i] != rhs1.pol_[i])
				return false;
		return true;
	}
	
}
bool operator!=(const Polynomial& lhs, const Polynomial& rhs) {
	return !operator==(lhs, rhs);
}
Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs) {
	if (lhs == Polynomial())
		return rhs;
	if (rhs == Polynomial())
		return lhs;
	Polynomial res = Polynomial();
	if (lhs.max_k > rhs.max_k)
		res.max_k = lhs.max_k;
	else
		res.max_k = rhs.max_k;
	if (lhs.min_k < rhs.min_k)
		res.min_k = lhs.min_k;
	else
		res.min_k = rhs.min_k;
	res.pol_ = new int[res.max_k - res.min_k + 1];
	for (int i = 0; i < res.max_k - res.min_k + 1; i++)
		res.pol_[i] = 0;
	for (int i = 0; i < res.max_k - res.min_k + 1; i++)
		for (int j = 0; j < lhs.max_k - lhs.min_k + 1; j++)
			if (i + res.min_k == j + lhs.min_k)
				res.pol_[i] += lhs.pol_[j];
	for (int i = 0; i < res.max_k - res.min_k + 1; i++)
		for (int j = 0; j < rhs.max_k - rhs.min_k + 1; j++)
			if (i + res.min_k == j + rhs.min_k)
				res.pol_[i] += rhs.pol_[j];
	return res.check_null();
}
Polynomial operator-(const Polynomial& other) {
	Polynomial res = Polynomial(other);
	for (int i = 0; i < other.max_k - other.min_k + 1;i++)
		res.pol_[i] *= -1;
	return res;
}
Polynomial operator-(const Polynomial& lhs, const Polynomial& rhs) {
	return operator+(lhs, -rhs);
}
Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs) {
	if (lhs == Polynomial())
		return lhs;
	if (rhs == Polynomial())
		return rhs;
	Polynomial res = Polynomial();
	res.max_k = lhs.max_k + rhs.max_k;
	res.min_k = lhs.min_k + rhs.min_k;
	delete[] res.pol_;
	res.pol_ = new int[res.max_k - res.min_k + 1];
	for (int i = 0; i < res.max_k - res.min_k + 1; i++)
		res.pol_[i] = 0;
	for (int i = 0; i < lhs.max_k - lhs.min_k + 1; i++) {
		for (int j = 0; j < rhs.max_k - rhs.min_k + 1; j++) {
			if (lhs.pol_[i] == 0)
				break;
			res.pol_[i+lhs.min_k + j+rhs.min_k-res.min_k] += lhs.pol_[i] * rhs.pol_[j];
		}
	}
	return res;
}
Polynomial operator*(const Polynomial& lhs, int value) {
	Polynomial res = lhs;
	for (int i = 0; i < lhs.max_k - lhs.min_k + 1;i++)
		res.pol_[i] *= value;
	return res;
}
Polynomial operator*(int value, const Polynomial& rhs) {
	return operator*(rhs, value);
}
Polynomial operator/(const Polynomial& lhs, int value) {
	Polynomial res = lhs;
	res /= value;
	return res;
}
std::ostream& operator<<(std::ostream& stream, const Polynomial& value) {
	if ((value.max_k == value.min_k) && (value.min_k == 0))
		stream << value.pol_[0];
	else {
		for (int i = value.max_k - value.min_k; i >= 0;i--) {
			if (value.pol_[i] != 0) {
				if (i == value.max_k - value.min_k) {
					if (value.pol_[i] == -1) {
						if (i + value.min_k == 0)
							stream << "-1";
						else
							stream << "-";
					}
					else {
						if ((value.pol_[i] == 1) && (i + value.min_k == 0))
							stream << '1';
						else
							if (value.pol_[i] != 1)
								stream << value.pol_[i];
					}
					if (i + value.min_k != 0) {
						if (i + value.min_k == 1)
							stream << "x";
						else
							stream << "x^" << i + value.min_k;
					}
				}
				else {
					if (value.pol_[i] == -1) {
						if (i + value.min_k == 0)
							stream << "-1";
						else
							stream << "-";
					}
					else {
						if (value.pol_[i] == 1) {
							if (i + value.min_k == 0)
								stream << "+1";
							else
								stream << "+";
						}
						else {
							if(value.pol_[i]>0)
								stream << '+' << value.pol_[i];
							else 
								stream << value.pol_[i];
						}
					}
					if (i + value.min_k != 0) {
						if (i + value.min_k == 1)
							stream << "x";
						else
							stream << "x^" << i + value.min_k;
					}
				}
			}
		}
	}
	return stream;
}
std::istream& operator>>(std::istream& stream, Polynomial& value) {
	stream >> value.min_k;
	stream >> value.max_k;
	for (int i = 0; i < value.max_k - value.min_k + 1;i++) {
		stream >> value.pol_[i];
	}
	return stream;
}