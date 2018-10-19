#ifndef NODE_H_
#define NODE_H_

#include <iostream>
#include <vector>
#include <memory>
#include <set>
#include <algorithm>
#include "event.h"

using namespace std;
enum class clr{white, gray};
class node{
protected:
    string name;
    node* parent;

public:
    clr color;
    node* ref;
    vector<node*> reps;
public:
    node(): name{""}, parent{nullptr}{};
    node(string name_, node* par): name{move(name_)}, parent{par}{};
    node(string name_, node* par, clr c): name{move(name_)}, parent{par}, color{c}{};
    node(string name_, node* par, clr c, node* ref_): name{move(name_)}, parent{par}, color{c}, ref{ref_}{};
    string get_name(){return name;}
    node* get_parent(){return parent;}
    virtual void print_transitions() = 0;
    virtual bool is_preserved() = 0;
    virtual void add_incoming_transition(node* s, event* e) = 0;
    virtual void add_outgoing_transition(node* d, event* e) = 0;
};

#endif
