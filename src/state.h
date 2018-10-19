#ifndef STATE_H
#define STATE_H

#include "node.h"
// Simple State


class SS: public node{
public:
    struct transition{
        node* state;
        event* ev;
    };
public:
    vector<transition> incoming;
    vector<transition> outgoing;
public:
    SS(string name, node* par):
        node(name, par,clr::white)
        {};
    SS(string name, node* par, node* ref_)
        : node(name, par, clr::white, ref_)
        {};
    void add_rep(SS* rep){reps.push_back(rep);}
    void add_incoming_transition(node* source, event* e);
    void add_outgoing_transition(node* dest, event* e);
    void print_transitions();
    bool is_preserved();
};

typedef unique_ptr<node> nPtr;
typedef unique_ptr<SS> ssPtr;

class nCmp{
public:
    bool operator()(const nPtr& n1, const nPtr& n2) const;
};


// OR Superstate
class ORSS: public node{
private:
    set<ePtr, eCmp> event_set;
    vector<nPtr> state_set;
    void procedure1(ORSS* hi_sys, node* source, event* ev, node* curr_state);
    void procedure2(ORSS* hi_sys, ORSS* lo_sys, node* state, node* curr_state);
public:
    ORSS(){};
    ORSS(string name, node* par): node(name, par){};

    void add_simple_state(string name);
    node* add_simple_state(string name, node* ref, node* source, event* ev);
    ORSS* add_superstate(string name);
    void add_incoming_transition(node* n, event* e){};
    void add_outgoing_transition(node* n, event* e){};
    bool is_preserved(){};
    void print_transitions();
    void add_event(ePtr e){event_set.insert(e);}
    void add_transition(string s, string e, string d);
    void add_transition(node* s, event* e, node* d);
    void make_interesting(string e);
    unique_ptr<ORSS> MHELOH();

};



#endif // STATE_H
