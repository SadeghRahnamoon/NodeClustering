#include "state.h"


void SS::add_incoming_transition(node* source, event* e){
    incoming.push_back(transition{source, e});
}

void SS::add_outgoing_transition(node* dest, event* e){
    outgoing.push_back(transition{dest, e});
}

void SS::print_transitions(){
    for(auto& t: outgoing){
        if(t.state->get_parent() == this->parent)
            cout << this->name << ':' << this->get_parent()->get_name() << ' ' << t.ev->name << ' ' << t.state->get_name() << ':' << t.state->get_parent()->get_name() << endl;
        else
            cout << this->name << ':' << this->get_parent()->get_name() << ' ' << t.ev->name << ' ' << t.state->get_name() << ':' << t.state->get_parent()->get_name() << endl;
//             cout << this->name << ' ' << t.ev->name << ' ' << t.state->get_parent()->get_name() << endl;
    }
}
bool SS::is_preserved(){

    auto it = find_if(outgoing.begin(), outgoing.end(), [](auto t){
        return t.ev->interesting == false;
    });
    if (it != outgoing.end() )
        return false;
    else return true;
}

bool nCmp::operator()(const nPtr& n1, const nPtr& n2) const{
    return n1->get_name() < n2->get_name();
}


void ORSS::print_transitions(){
    cout << name << endl;
    for(auto& n:state_set){
        n->print_transitions();
    }
}

node* ORSS::add_simple_state(string name, node* ref, node* source, event* ev){
    auto new_state = make_unique<SS>(name, this, ref);
    if(ref != nullptr){
        ref->reps.push_back(new_state.get());
    }
    auto ret_val = new_state.get();
    state_set.push_back(move(new_state));
    if(source != nullptr)
        add_transition(source, ev, ret_val);
    return ret_val;
}

ORSS* ORSS::add_superstate(string name){
    auto new_super_state = make_unique<ORSS>("H_"+name+"^lo", this);
    auto ret_val = new_super_state.get();
    state_set.push_back(move(new_super_state));
    return ret_val;
}

void ORSS::add_transition(string s, string e, string d){
    auto it1 = find_if(state_set.begin(), state_set.end(), [&](auto& n){
        return n->get_name() == s;
    });
    auto it2 = find_if(state_set.begin(), state_set.end(), [&](auto& n){
        return n->get_name() == d;
    });
    if(it1 == state_set.end() || it2 == state_set.end()){
        cout << "invalid transition!" << endl;
        return;
    }
    auto new_event = make_shared<event>(event{e});
    auto result = event_set.insert(move(new_event));
    auto it = result.first;

    (*it1)->add_outgoing_transition(it2->get(), it->get());
    (*it2)->add_incoming_transition(it1->get(), it->get());

}
void ORSS::add_transition(node* s, event* e, node* d){
    if(s!= nullptr){
        (s)->add_outgoing_transition(d,e);
        (d)->add_incoming_transition(s,e);
     }
}

void ORSS::make_interesting(string ev){
    auto it = find_if(event_set.begin(), event_set.end(), [&](auto& e){
        return e->name == ev;
    });
    if(it != event_set.end())
        (*it)->interesting = true;
    else cout << "not found" << endl;

}

//MHELOH procedure


void ORSS::procedure2(ORSS* hi_sys, ORSS* lo_sys, node* state, node* curr_state){
    node* lo_sys1 = static_cast<node*>(lo_sys);
    if(SS*  Curr_state = dynamic_cast<SS*>(curr_state)){
        for(auto& t:Curr_state->outgoing){
            if(t.ev->interesting){
                procedure1(hi_sys, state, t.ev, t.state);
            }
            else{
                auto it = find_if(t.state->reps.begin(), t.state->reps.end(), [&](auto& n1){
                    return n1->get_parent() == lo_sys1;
                });
                if ( curr_state->color == clr::gray && it != t.state->reps.end()){
                    lo_sys->add_transition( state, t.ev, *it);
                    return;
                }
                else{
                    t.state->color = clr::gray;
                    node* new_node =  lo_sys->add_simple_state(t.state->get_name() + to_string(t.state->reps.size() + 1), t.state, state, t.ev);
                    procedure2(hi_sys, lo_sys, new_node, t.state);
                }
            }
        }
    }
};

void ORSS::procedure1(ORSS* hi_sys, node* source, event* ev, node* curr_state){

    if(SS*  Curr_state = dynamic_cast<SS*>(curr_state)){
        if(Curr_state->color == clr::gray){
            hi_sys->add_transition(source, ev, Curr_state->reps[0]);
            return;
        }
        else{
            Curr_state->color = clr::gray;
            if( Curr_state->is_preserved() ){
                auto* temp_node = hi_sys->add_simple_state(curr_state->get_name() + "1",curr_state, source, ev);
                //hi_sys->add_transition(source, ev, temp_node);
                for(auto& t: Curr_state->outgoing)
                    procedure1(hi_sys, temp_node, t.ev, t.state);
            }
            else{
                auto lo_sys = hi_sys->add_superstate(Curr_state->get_name());
                auto new_state = lo_sys->add_simple_state(Curr_state->get_name() + to_string(Curr_state->reps.size() + 1), Curr_state, source, ev);
                procedure2(hi_sys, lo_sys, new_state, curr_state);
            }
        }
    }
}

unique_ptr<ORSS> ORSS::MHELOH(){
    auto sys = make_unique<ORSS>(this->get_name()+"res", nullptr);
    for(auto& e:event_set)
        sys->add_event(e);
    for(auto& s: state_set)
        if(s->color == clr::white)
            procedure1(sys.get(),nullptr,nullptr, s.get());
    return move(sys);
}
