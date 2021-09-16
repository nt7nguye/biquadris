#include "subject.h"
#include "observer.h"
#include <vector>
#include <algorithm>

using namespace std;

void Subject::attach(Observer* o) {
    observers.emplace_back(o);
}

void Subject::detach(Observer *o) {
    auto it = find(observers.begin(), observers.end(), o);
    if (it != observers.end()) {
        observers.erase(it);
    }
}

void Subject::notifyObservers() {
    for (auto & observer : observers) {
        observer->notify();
    }
}
