#pragma once
class Polynomial {
private:
	int max_k;
	int min_k;
	int* pol_;
public:
	Polynomial();
	Polynomial(int min, int max, int* pol);
	Polynomial(const Polynomial& other);
	Polynomial& operator=(const Polynomial& other);
	virtual ~Polynomial();
	Polynomial& operator+=(const Polynomial& rhs);
	Polynomial& operator-=(const Polynomial& rhs);
	Polynomial& operator*=(const Polynomial& rhs);
	Polynomial& operator/=(int value);
	int& operator[](int index);
	int operator[](int index) const;
	double get(int value);
	//fixed not check, no return
	void delete_null();
	friend bool operator==(const Polynomial& lhs, const Polynomial& rhs);
	friend bool operator!=(const Polynomial& lhs, const Polynomial& rhs);
	friend Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs);
	friend Polynomial operator-(const Polynomial& lhs, const Polynomial& rhs);
	friend Polynomial operator-(const Polynomial& other);
	friend Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs);
	friend Polynomial operator*(const Polynomial& lhs, int value);
	friend Polynomial operator*(int value, const Polynomial& rhs);
	friend Polynomial operator/(const Polynomial& lhs, int value);
	friend std::ostream& operator<<(std::ostream& stream, const Polynomial& value);
	friend std::istream& operator>>(std::istream& stream, Polynomial& value);
};
