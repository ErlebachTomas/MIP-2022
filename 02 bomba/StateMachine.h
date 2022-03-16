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
    void reset();
};

class SetState : public AbstractState {
public:    
    void action();
    AbstractState* nextState(MyEvent e);
};

/// <summary>
/// Drží kontext s aktuálním stavem, přechod řízen pomocí událostí 
/// </summary>
class Machine {
public:
    /// <summary>
    /// Konstruktor stavového automatu, vyžaduje zadání počátečního stavu 
    /// </summary>
    /// <param name="startState">Počáteční stav</param>
    Machine(AbstractState* const startState);
    ~Machine();

    /// <summary>
    /// Na základě události přepne stav a spustí akci
    /// </summary>
    void event(MyEvent e);

    /// <summary>
    /// Spustí akci uvnitř aktuálního stavu
    /// </summary>
    void start();

private:
    /// <summary>
    /// Drží aktuální stav
    /// </summary>
    AbstractState* state;

    /// <summary>
    /// Změní stav
    /// </summary>    
    void setState(AbstractState* const s);
};