#pragma once

/// <summary>
/// Číselník událostí
/// </summary>
enum class MyEvent {
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
    virtual AbstractState* nextState(MyEvent e) {
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
    AbstractState* nextState(MyEvent e);
};

class BoomState : public AbstractState {
public:    
    void action();
    AbstractState* nextState(MyEvent e);
private:
    void boomAnimation(int count);
};

class SetState : public AbstractState {
public:    
    void action();
    AbstractState* nextState(MyEvent e);
};
