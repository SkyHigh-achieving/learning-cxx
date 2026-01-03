#include <cstring>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

std::vector<std::string> RECORDS;

class Resource {
    std::string _records;

public:
    void record(char record) {
        _records.push_back(record);
        std::cout << "Recorded: " << record << " in resource " << this << std::endl;
    }

    ~Resource() {
        RECORDS.push_back(_records);
        std::cout << "Destructed resource " << this << " with records: " << _records << std::endl;
    }
};

using Unique = std::unique_ptr<Resource>;
Unique reset(Unique ptr) {
    std::cout << "Enter reset with ptr: " << (ptr ? "not null" : "null") << std::endl;
    if (ptr) ptr->record('r');
    auto new_ptr = std::make_unique<Resource>();
    std::cout << "Exit reset, returning new ptr: " << new_ptr.get() << std::endl;
    return new_ptr;
}
Unique drop(Unique ptr) {
    std::cout << "Enter drop with ptr: " << (ptr ? "not null" : "null") << std::endl;
    if (ptr) ptr->record('d');
    std::cout << "Exit drop, returning nullptr" << std::endl;
    return nullptr;
}
Unique forward(Unique ptr) {
    std::cout << "Enter forward with ptr: " << (ptr ? "not null" : "null") << std::endl;
    if (ptr) ptr->record('f');
    std::cout << "Exit forward, returning same ptr: " << ptr.get() << std::endl;
    return ptr;
}

int main(int argc, char **argv) {
    std::vector<std::string> problems[3];
    
    std::cout << "=== Test 0 ===" << std::endl;
    drop(forward(reset(nullptr)));
    problems[0] = std::move(RECORDS);
    
    std::cout << "\n=== Test 1 ===" << std::endl;
    forward(drop(reset(forward(forward(reset(nullptr))))));
    problems[1] = std::move(RECORDS);
    
    std::cout << "\n=== Test 2 ===" << std::endl;
    drop(drop(reset(drop(reset(reset(nullptr))))));
    problems[2] = std::move(RECORDS);
    
    std::cout << "\n=== Results ===" << std::endl;
    for (auto i = 0; i < 3; ++i) {
        std::cout << "Test " << i << ": " << problems[i].size() << " records" << std::endl;
        for (const auto& record : problems[i]) {
            std::cout << "  - " << record << std::endl;
        }
    }
    
    return 0;
}