#pragma once

#include <vector>
#include <functional>
#include <algorithm>

// lightweight simple delegate
template<typename T, typename...Args>
class delegate final {
public:
    void add(std::function<T(Args...)> inFunc) {
        this->funcs.push_back(inFunc);
    }

    bool remove(std::function<T(Args...)> inFunc) {
        this->funcs.erase(std::remove(this->funcs.begin(), this->funcs.end(), inFunc), this->funcs.end());
    }

    void broadcast(Args... args) {
        for (auto it = this->funcs.begin(); it != this->funcs.end();) {
            if (!(*it)) {
                it = this->funcs.erase(it);
            }
            else {
                (*it)(args...);
                it++;
            }
        }
    }

private:
	std::vector<std::function<T(Args...)>> funcs;
};