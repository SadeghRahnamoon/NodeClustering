#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <memory>

using namespace std;

struct event{
    string name;
    bool controllable = true;
    bool observable = true;
    bool interesting = false;
};

typedef shared_ptr<event> ePtr;

class eCmp{
public:
    bool operator()(const ePtr& e1, const ePtr& e2) const;
};

#endif // EVENT_H
