#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

const short number_length = 13;
const short bits_per_byte = 8;
const short key_size_in_bits = 64;
const short num_buckets = 256;
const short line_length = 2100;

template <typename T>
class MyVector {
public:
    MyVector() : data_(nullptr), size_(0), capacity_(0) {}

    MyVector(size_t size) : data_(new T[size]), size_(size), capacity_(size) {}

    ~MyVector() {
        delete[] data_;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            grow();
        }
        data_[size_++] = value;
    }

    size_t size() const {
        return size_;
    }

    T& operator[](size_t index) {
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    void swap(MyVector& other) {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

private:
    void grow() {
        size_t new_capacity = max(static_cast<size_t>(1), capacity_ * 2);
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

    T* data_;
    size_t size_;
    size_t capacity_;
};

unsigned long long formatPhoneNumber(const string & phone);

class KeyValuePair {
public:

    KeyValuePair() = default;

    KeyValuePair(const string & key, const string & value)
        : key_(formatPhoneNumber(key))
        , value_(value)
    {
        size_t first_non_zero = key.find_first_not_of("0-"
                                            , key.find('+') + 1);
        if (first_non_zero > key.length()) first_non_zero = key.length();
        for (size_t i = key.find('+') + 1; i < first_non_zero; ++i) {
            if (key[i] == '0') ++num_zero_;
        }
    }

    short getDigit(size_t pos) const {

        static const unsigned long long powersOf10[number_length]= {1, 10
                , 100, 1000, 10000, 100000, 1000000, 10000000, 100000000
                , 1000000000, 10000000000, 100000000000, 1000000000000};
        
        return (key_ / powersOf10[pos]) % 10;

    }

    unsigned long long getKey() const { return key_; }
    string getValue() const { return value_; }
    short getNumZero() const { return num_zero_; }

private:
    unsigned long long key_;
    string value_;
    short num_zero_ = 0;
};

void countSort(MyVector<KeyValuePair> & data, int exp) {

    int count[num_buckets] = {0};
    MyVector<KeyValuePair> output(data.size());


    for (size_t i = 0; i < data.size(); ++i) {
        count[(data[i].getKey() >> (bits_per_byte * exp)) & 0xFF]++;
    }
    for (int i = 1; i < num_buckets; i++) {
        count[i] += count[i - 1];
    }
    for (int i = data.size() - 1; i >= 0; --i) {
        output[count[(data[i].getKey() >> (bits_per_byte * exp)) & 0xFF] - 1] = move(data[i]);
        count[(data[i].getKey() >> (bits_per_byte * exp)) & 0xFF]--;
    }

    data.swap(output);

}

void radixSort(MyVector<KeyValuePair> & data) {

    for (int i = 0; i < key_size_in_bits; i += bits_per_byte) {
        countSort(data, i / bits_per_byte);
    }

}

unsigned long long formatPhoneNumber(const string & phone) {

    unsigned long long formatted = 0;

    for (char c : phone) {
        if (isdigit(c)) {
            formatted = formatted * 10 + c - '0';
        }
    }
    
    return formatted;

}


void readInputData(MyVector<KeyValuePair> & data) {
    
    char line[line_length];
    while(scanf("%[^\n]\n", line) != EOF){
        string s(line);
        size_t tabPos = s.find('\t');
        if (tabPos < s.length()) {
            KeyValuePair kv(s.substr(0, tabPos),
                            s.substr(tabPos + 1));
            data.push_back(kv);
        }
    }
    
}

string unformatPhoneNumber(unsigned long long phone, short num_zero) {

    string unformatted;

    while (phone > 0) {
        unformatted += '0' + phone % 10;
        phone /= 10;
    }

    reverse(unformatted.begin(), unformatted.end());

    while (num_zero--) unformatted = "0" + unformatted;

    if (unformatted.length() >= 7) {
        unformatted.insert(unformatted.length() - 7, "-");
    }
    if (unformatted.length() >= 11) {
        unformatted.insert(unformatted.length() - 11, "-");
    }

    unformatted = "+" + unformatted;

    return unformatted;

}


void printVector(MyVector<KeyValuePair> & data) {
    
    for (size_t i = 0; i < data.size(); ++i) {
        printf("%s\t%s\n", unformatPhoneNumber(data[i].getKey()
                , data[i].getNumZero()).c_str(), data[i].getValue().c_str());
    }
    
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    setvbuf(stdin, NULL, _IOFBF, 4096);
    setvbuf(stdout, NULL, _IOFBF, 4096);

    MyVector<KeyValuePair> data;
    
    readInputData(data);

    radixSort(data);

    printVector(data);

    return 0;
    
}
