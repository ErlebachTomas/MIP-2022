#pragma once

/// <summary>
/// Číselník událostí
/// </summary>
enum class Event {
	reset,
	explosion,
	set
};

/// <summary>
/// Implementace stavového automatu pomocí návrhového vzoru state 
/// </summary>
class AbstractState {
public:
    virtual ~AbstractState() {}
    /// <summary>
    /// Vrátí následující stav 
    /// </summary>
    /// <param name="e">Událost řídící přechod</param>
    /// <returns>Stav v dalším kroce</returns>
    virtual AbstractState* nextState(Event e) {
        return this;
    }
    /// <summary>
    /// Akce stavu
    /// </summary>
    virtual void action() = 0;
};
class CoutingState : public AbstractState {
public:   
    void action();
    AbstractState* nextState(Event e);
};

class BumState : public AbstractState {
public:    
    void action();
    AbstractState* nextState(Event e);
};

class SetState : public AbstractState {
public:    
    void action();
    AbstractState* nextState(Event e);
};
