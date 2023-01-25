#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#define DIMENSION 233

class Element {
private:
    bool arr[DIMENSION]{};

    static bool L[DIMENSION][DIMENSION];

public:
    Element();

    explicit Element(const std::string& str);

    friend std::ostream& operator<<(std::ostream& stream, const Element& element);

    bool operator==(const Element& element) const;

    Element operator+(const Element& element) const;

    static void generateMatrix();

    Element operator*(const Element& element) const;

    Element sqr() const;

    Element operator^(const std::vector<bool>& n) const;
};

std::vector<bool> bitVectorFromStr(const std::string& str);

Element::Element() {
    for (bool& i : arr) {
        i = true;
    }
}

Element::Element(const std::string& str) {
    for (int i = 0; i < DIMENSION - str.length(); ++i) {
        arr[i] = false;
    }
    for (int i = 0; i < str.length(); ++i) {
        arr[DIMENSION - str.length() + i] = (str[i] != '0');
    }
}

std::ostream& operator<<(std::ostream& stream, const Element& element) {
    for (bool i : element.arr) {
        stream << (i ? '1' : '0');
    }
    return stream;
}

Element Element::operator+(const Element& element) const {
    Element res{};
    for (int i = 0; i < DIMENSION; ++i) {
        res.arr[i] = (arr[i] != element.arr[i]);
    }
    return res;
}

bool Element::L[DIMENSION][DIMENSION];

void Element::generateMatrix() {
    const int p = DIMENSION * 2 + 1;
    int rests[DIMENSION];
    int a = 1;
    for (int& rest : rests) {
        rest = a;
        a *= 2;
        a %= p;
    }
    for (int i = 0; i < DIMENSION; ++i) {
        for (int j = 0; j < DIMENSION; ++j) {
            L[i][j] = (((rests[i] + rests[j]) % p + p) % p == 1)
                || (((rests[i] - rests[j]) % p + p) % p == 1)
                || (((-rests[i] + rests[j]) % p + p) % p == 1)
                || (((-rests[i] - rests[j]) % p + p) % p == 1);
        }
    }
}

Element Element::operator*(const Element& element) const {
    Element res{};
    bool u[DIMENSION], v[DIMENSION];
    for (int k = 0; k < DIMENSION; ++k) {
        for (int i = 0; i < DIMENSION - k; ++i) {
            u[i] = arr[i + k];
            v[i] = element.arr[i + k];
        }
        for (int i = 0; i < k; ++i) {
            u[DIMENSION - k + i] = arr[i];
            v[DIMENSION - k + i] = element.arr[i];
        }
        res.arr[k] = false;
        for (int i = 0; i < DIMENSION; ++i) {
            for (int j = 0; j < DIMENSION; ++j) {
                if (u[i] && L[i][j] && v[j])
                    res.arr[k] = !res.arr[k];
            }
        }
    }
    return res;
}

Element Element::sqr() const {
    Element res{};
    res.arr[0] = arr[DIMENSION - 1];
    for (int i = 1; i < DIMENSION; ++i) {
        res.arr[i] = arr[i - 1];
    }
    return res;
}

Element Element::operator^(const std::vector<bool>& n) const {
    Element res{}, a = *this;
    std::vector<bool> rev = n;
    std::reverse(rev.begin(), rev.end());
    for (auto&& i : rev) {
        if (i) {
            res = res * a;
        }
        a = a.sqr();
    }
    return res;
}

bool Element::operator==(const Element& element) const {
    for (int i = 0; i < DIMENSION; ++i) {
        if (arr[i] != element.arr[i])
            return false;
    }
    return true;
}

std::vector<bool> bitVectorFromStr(const std::string& str) {
    std::vector<bool> v(str.length());
    for (int i = 0; i < str.size(); ++i) {
        v[i] = (str[i] != '0');
    }
    return v;
}

int main() {
    Element::generateMatrix();
    Element a("10111000111111100011111111000000010101110011000000100000101111110011101100000101101000110010100001110110111011100011101001010000111000010010010010101111101011100011001011101111000010011101001111011100110111100110111110100111001010010"),
        b("11010111101100001001011111010110000100010110011001100100111000011100010011100011100011001011101011111110010000001111000010100110111011001100000010101110110101110010010000101100000011000101011010011001001001110110011100111101111010010");
    std::vector<bool> n = bitVectorFromStr("01100011000101100100110110111110100111110001001010111101000110101110110110110011100100000011110010111000100001011100101110001101010011100100100101001001000001001100010110000011111100110110001011011101101000100011010110000000110100111");
    Element sum("01101111010011101010100000010110010001100101011001000100010111101111111111100110001011111001001010001000101011101100101011110110000011011110010000000001011110010001011011000011000001011000010101000101111110010000100010011010110000000"),
        mul("10100100010110010101001101011011010111101111000010010001011000100111101010111010000100110101000110110100010110000011100000010100110011111110111100011110001110110110010001111111000111111101001101010100010010011111010011110111101110101"),
        sqr("01011100011111110001111111100000001010111001100000010000010111111001110110000010110100011001010000111011011101110001110100101000011100001001001001010111110101110001100101110111100001001110100111101110011011110011011111010011100101001"),
        pow("00000000000011000100000110100110100010110101001101000110001110100001010001100101110000001101100011101011000000001010111100110000110110010011110010101100000011111111101111100110111011001100111101011000101110000101111100011011010011110");
    std::cout << "Sum: " << ((a + b) == sum ? "ok" : "not ok") << std::endl;
    std::cout << "Mul: " << ((a * b) == mul ? "ok" : "not ok") << std::endl;
    std::cout << "Sqr: " << (a.sqr() == sqr ? "ok" : "not ok") << std::endl;
    std::cout << "Pow: " << ((a ^ n) == pow ? "ok" : "not ok") << std::endl;

    return 0;
}