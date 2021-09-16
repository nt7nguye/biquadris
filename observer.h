#ifndef OBSERVER_H
#define OBSERVER_H

// Simple observer (as described in lecture)
class Observer {
    public:
    virtual void notify() = 0;
    virtual ~Observer() = default;
};

#endif
