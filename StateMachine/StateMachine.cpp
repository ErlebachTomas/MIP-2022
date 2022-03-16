#include <iostream>
#include "StateMachine.h"
using namespace std;


//TODO definice konkrétních stavů
void CoutingState::action() {
    cout << "Casovac, tik tak, tik tak..." << endl;
};

AbstractState* CoutingState::nextState(Event e) {
    switch (e) {
    case Event::explosion:
        return new BumState();
    case Event::set:
        return new SetState();
    default:
        return this;
    }
}

void BumState::action() {
        cout << "BUM!" << endl;
}

AbstractState* BumState::nextState(Event e) {
    switch (e) {
    case Event::reset:
        return new CoutingState();
    default:
        return this;
    }

};


void SetState::action() {
    cout << "Otevrit nastaveni" << endl;
};
AbstractState* SetState::nextState(Event e) {
    return this;
} 

/// <summary>
/// Drží kontext s aktuálním stavem, přechod řízen pomocí událostí 
/// </summary>
class Machine {
public:
    /// <summary>
    /// Konstruktor stavového automatu, vyžaduje zadání počátečního stavu 
    /// </summary>
    /// <param name="startState">Počáteční stav</param>
    Machine(AbstractState* const startState)  {    
        this->setState(startState);
    }    
    ~Machine() {
        if (state) {
            delete state;
        }
    }

    /// <summary>
    /// Na základě události přepne stav a spustí akci
    /// </summary>
    void event(Event e) {     
       this->setState(state->nextState(e));
       state->action();
    }
        
    /// <summary>
    /// Spustí akci uvnitř aktuálního stavu
    /// </summary>
    void start() {
        state->action();
    }

private:
    /// <summary>
    /// Drží aktuální stav
    /// </summary>
    AbstractState* state;

    /// <summary>
    /// Změní stav
    /// </summary>    
    void setState(AbstractState* const s) {
        if (state) {
            delete state;
        }
        state = s;
    }
};

int main() {
    Machine* MachineContext = new Machine(new CoutingState());
    MachineContext->start();
    // zpracovani udalosti
    MachineContext->event(Event::explosion);
    MachineContext->event(Event::reset);
    MachineContext->event(Event::set);
}

