// =============================================================
//  big_integer.cpp -- BigInteger class implementation
//
//  TASK: Implement all methods declared in big_integer.h
//  This stub file exists only so the project structure is clear.
//  Replace its contents with your implementation.
// =============================================================

#include "big_integer.h"

// TODO: your code here
#include "big_integer.h"
#include <algorithm>
#include <stdexcept>


BigInteger::BigInteger() {
    digits_.push_back(0);
    negative_ = false;
}

BigInteger::BigInteger(int value) {
    if (value == 0) {
        digits_.push_back(0);
        negative_ = false;
        return;
    }
    
    if (value < 0) {
        negative_ = true;
        value = -value;
    } else {
        negative_ = false;
    }
    
    while (value > 0) {
        digits_.push_back(value % 10);
        value = value / 10;
    }
}

BigInteger::BigInteger(long long value) {
    if (value == 0) {
        digits_.push_back(0);
        negative_ = false;
        return;
    }
    
    if (value < 0) {
        negative_ = true;
        value = -value;
    } else {
        negative_ = false;
    }
    
    while (value > 0) {
        digits_.push_back(value % 10);
        value = value / 10;
    }
}

BigInteger::BigInteger(const std::string& str) {
    digits_.clear();
    
    if (str.empty()) {
        digits_.push_back(0);
        negative_ = false;
        return;
    }
    
    int start = 0;
    if (str[0] == '-') {
        negative_ = true;
        start = 1;
    } else {
        negative_ = false;
        if (str[0] == '+') {
            start = 1;
        }
    }

    while (start < (int)str.size() && str[start] == '0') {
        start++;
    }
    
    // Если все нули
    if (start == (int)str.size()) {
        digits_.push_back(0);
        negative_ = false;
        return;
    }
    
    // Записываем цифры в обратном порядке
    for (int i = (int)str.size() - 1; i >= start; i--) {
        if (str[i] < '0' || str[i] > '9') {
            digits_.push_back(0);
            negative_ = false;
            return;
        }
        digits_.push_back(str[i] - '0');
    }
}



static void removeLeadingZeros(std::vector<int>& digits, bool& negative) {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
    
    if (digits.size() == 1 && digits[0] == 0) {
        negative = false;
    }
}

static int compareAbsolute(const std::vector<int>& a, const std::vector<int>& b) {
    if (a.size() != b.size()) {
        if (a.size() < b.size()) return -1;
        else return 1;
    }
    
    for (int i = (int)a.size() - 1; i >= 0; i--) {
        if (a[i] != b[i]) {
            if (a[i] < b[i]) return -1;
            else return 1;
        }
    }
    
    return 0;
}


BigInteger BigInteger::operator+(const BigInteger& rhs) const {
    BigInteger result;
    
    if (negative_ == rhs.negative_) {
        result.negative_ = negative_;
        result.digits_.clear();
        
        int carry = 0;
        size_t maxSize = digits_.size();
        if (rhs.digits_.size() > maxSize) {
            maxSize = rhs.digits_.size();
        }
        
        for (size_t i = 0; i < maxSize; i++) {
            int sum = carry;
            
            if (i < digits_.size()) {
                sum += digits_[i];
            }
            if (i < rhs.digits_.size()) {
                sum += rhs.digits_[i];
            }
            
            result.digits_.push_back(sum % 10);
            carry = sum / 10;
        }
        
        if (carry > 0) {
            result.digits_.push_back(carry);
        }
        
        removeLeadingZeros(result.digits_, result.negative_);
        return result;
    }
    
    if (negative_) {
        // -a + b = b - a
        BigInteger a = *this;
        a.negative_ = false;
        result = rhs - a;
    } else {
        // a + (-b) = a - b
        BigInteger b = rhs;
        b.negative_ = false;
        result = *this - b;
    }
    
    return result;
}

BigInteger BigInteger::operator-(const BigInteger& rhs) const {
    BigInteger result;
    
    if (negative_ != rhs.negative_) {
        BigInteger b = rhs;
        b.negative_ = !rhs.negative_;
        result = *this + b;
        return result;
    }
    
    int cmp = compareAbsolute(digits_, rhs.digits_);
    
    const std::vector<int>* biggerDigits = &digits_;
    const std::vector<int>* smallerDigits = &rhs.digits_;
    bool resultNegative = negative_;
    
    if (cmp < 0) {
        biggerDigits = &rhs.digits_;
        smallerDigits = &digits_;
        resultNegative = !negative_;
    }
    
    result.digits_.clear();
    result.negative_ = resultNegative;
    
    int borrow = 0;
    
    for (size_t i = 0; i < biggerDigits->size(); i++) {
        int diff = (*biggerDigits)[i] - borrow;
        
        if (i < smallerDigits->size()) {
            diff -= (*smallerDigits)[i];
        }
        
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result.digits_.push_back(diff);
    }
    
    removeLeadingZeros(result.digits_, result.negative_);
    return result;
}
