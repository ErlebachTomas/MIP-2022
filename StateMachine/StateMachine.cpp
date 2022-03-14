#include <iostream>
#include "State.h"
using namespace std;

/// <summary>
/// Implementace stavov�ho automatu pomoc� n�vrhov�ho vzoru state 
/// </summary>
class AbstractState {
public:   
    virtual ~AbstractState() {}
    /// <summary>
    /// Vr�t� n�sleduj�c� stav 
    /// </summary>
    /// <param name="e">Ud�lost ��d�c� p�echod</param>
    /// <returns>Stav v dal��m kroce</returns>
    virtual AbstractState* nextState(Event e) {
        return this;
    };
    /// <summary>
    /// Akce stavu
    /// </summary>
    virtual void action() = 0;
};

//TODO definice konkr�tn�ch stav�
class CoutingState : public AbstractState {
public: 
    ~CoutingState() {}

    void action() {
        cout << "�asova�, tik tak, tik tak..." << endl;
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
        cout << "Otev��t nastaven�" << endl;
    }
};

/// <summary>
/// Dr�� kontext s aktu�ln�m stavem, p�echod ��zen pomoc� ud�lost� 
/// </summary>
class Machine {
public:
    /// <summary>
    /// Konstruktor stavov�ho automatu, vy�aduje zad�n� po��te�n�ho stavu 
    /// </summary>
    /// <param name="startState">Po��te�n� stav</param>
    Machine(AbstractState* const startState)  {    
        this->setState(startState);
    }    
    ~Machine() {
        if (state) {
            delete state;
        }
    }

    /// <summary>
    /// Na z�klad� ud�losti p�epne stav a spust� akci
    /// </summary>
    void event(Event e) {     
       this->setState(state->nextState(e));
       state->action();
    }
        
    /// <summary>
    /// Spust� akci uvnit� aktu�ln�ho stavu
    /// </summary>
    void run() {
        state->action();
    }

private:
    /// <summary>
    /// Dr�� aktu�ln� stav
    /// </summary>
    AbstractState* state;

    /// <summary>
    /// Zm�n� stav
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

