#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>

class Observer;

// A Subject for the observer pattern (as discussed in lecture)
class Subject {
    std::vector<Observer*> observers;
    public:
    void attach(Observer* o);
    void detach(Observer* o);
    void notifyObservers();
    virtual ~Subject() = default;
};

#endif
