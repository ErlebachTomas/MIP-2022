#include <iostream>
#include "State.h"
using namespace std;

/// <summary>
/// Implementace stavového automatu pomocí návrhového vzoru state 
/// </summary>
class AbstractState {
public:   
    virtual ~AbstractState() {}
    /// <summary>
    /// Vrátí následující stav 
    /// </summary>
    /// <param name="e">Událost øídící pøechod</param>
    /// <returns>Stav v dalším kroce</returns>
    virtual AbstractState* nextState(Event e) {
        return this;
    };
    /// <summary>
    /// Akce stavu
    /// </summary>
    virtual void action() = 0;
};

//TODO definice konkrétních stavù
class CoutingState : public AbstractState {
public: 
    ~CoutingState() {}

    void action() {
        cout << "Èasovaè, tik tak, tik tak..." << endl;
    }
    
    AbstractState* nextState(Event e) {
        switch (e) {
        case Event::explosion:
            return new BumState();
        case Event::set:
            return new SetState();
        default:
            return this;
        }    
    }
    
};

class BumState : public AbstractState {
   
    ~BumState() {}

    void action() {
        cout << "BUM!" << endl;
    }

    AbstractState* nextState(Event e) {
        switch (e) {
        case Event::reset:
            return new CoutingState();
        default:
            return this;
        }

    }
};

class SetState : public AbstractState {
    ~SetState() {}

    void action() {
        cout << "Otevøít nastavení" << endl;
    }
};

/// <summary>
/// Drží kontext s aktuálním stavem, pøechod øízen pomocí událostí 
/// </summary>
class Machine {
public:
    /// <summary>
    /// Konstruktor stavového automatu, vyžaduje zadání poèáteèního stavu 
    /// </summary>
    /// <param name="startState">Poèáteèní stav</param>
    Machine(AbstractState* const startState)  {    
        this->setState(startState);
    }    
    ~Machine() {
        if (state) {
            delete state;
        }
    }

    /// <summary>
    /// Na základì události pøepne stav a spustí akci
    /// </summary>
    void event(Event e) {     
       this->setState(state->nextState(e));
       state->action();
    }
        
    /// <summary>
    /// Spustí akci uvnitø aktuálního stavu
    /// </summary>
    void run() {
        state->action();
    }

private:
    /// <summary>
    /// Drží aktuální stav
    /// </summary>
    AbstractState* state;

    /// <summary>
    /// Zmìní stav
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
    MachineContext->run();

    MachineContext->event(Event::explosion);
    MachineContext->event(Event::reset);
    MachineContext->event(Event::set);
}

