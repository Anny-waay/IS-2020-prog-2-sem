#pragma once

template < class InputIterator, class UnaryPredicate>
bool allOf(InputIterator first, InputIterator last, const UnaryPredicate& func) {
    for (; first != last; ++first)
        if (!func(*first))
            return false;
    return true;
};

template < class InputIterator, class UnaryPredicate>
bool anyOf(InputIterator first, InputIterator last, const UnaryPredicate& func) {
    for (; first != last; ++first)
        if (func(*first))
            return true;
    return false;
};

template < class InputIterator, class UnaryPredicate>
bool noneOf(InputIterator first, InputIterator last, const UnaryPredicate& func) {
    return !anyOf(first, last, func);
};

template < class InputIterator, class UnaryPredicate>
bool oneOf(InputIterator first, InputIterator last, const UnaryPredicate& func) {
    int counter = 0;
    for (; first != last; ++first)
        if (func(*first))
            counter++;
    if (counter == 1)
        return true;
    else
        return false;
};

template < class InputIterator, class BinaryPredicate = std::less<>>
bool isSorted(InputIterator first, InputIterator last, const BinaryPredicate func = BinaryPredicate()) {
    for (; first != last-1; ++first)
        if (!func(*first, *(first+1)))
            return false;
    return true;
};

template < class InputIterator, class UnaryPredicate>
bool isPartitioned(InputIterator first, InputIterator last, const UnaryPredicate& func) {
    int change_st = 0;
    bool curr_st = func(*first);
    ++first;
    for (; first != last; ++first)
        if (func(*first) != curr_st) {
            ++change_st;
            curr_st = func(*first);
        }
    if (change_st == 1)
        return true;
    else
        return false;
};

template < class InputIterator, class T>
InputIterator findNot(InputIterator first, InputIterator last, const T value) {
    for (; first != last; ++first)
        if (*first != value)
            return first;
    return last;
};

template < class InputIterator, class T>
InputIterator findBackward(InputIterator first, InputIterator last, const T value) {
    InputIterator res = last;
    for (; first != last; ++first)
        if (*first == value) 
            res = first;
    return res;
};

template < class BidirectionalIterator, class BinaryPredicate>
bool isPalindrome(BidirectionalIterator first, BidirectionalIterator last, const BinaryPredicate& func) {
    --last;
    while (first != last) {
        if (!func(*first, *last))
            return false;
        ++first;
        if (first != last)
            --last;
    }
    return true;
};