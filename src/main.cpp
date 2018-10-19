#include <iostream>

#include "event.h"
#include "node.h"
#include "state.h"


using namespace std;


int main()
{
    string name;
    cin >> name;
    auto system = make_unique<ORSS>(name, nullptr);
    int num_of_state, num_of_transitions;
    cin >> num_of_state >> num_of_transitions;
    for(int i = 0; i < num_of_state; ++i){
        cin >> name;
        system->add_simple_state(name,nullptr, nullptr, nullptr);
    }
    for(int i = 0; i < num_of_transitions; ++i){
       string source, event, dest;
       cin >> source >> event >> dest;
       system->add_transition(source, event, dest);
    }

    int num_of_intr_events;
    cin >> num_of_intr_events;
    for(int i = 0; i < num_of_intr_events; ++i){
        string in;
        cin >> in;
        system->make_interesting(in);
    }


    auto result_sys = system->MHELOH();
    result_sys->print_transitions();
    return 0;
}

