#pragma once
//using namespace std;
//#include <map>


/// <summary>
/// ��seln�k ud�lost�
/// </summary>
enum class Event {
	reset,
	explosion,
	set
};

/* todo odstranit pak (nepouziva se) 
/// <summary>
/// ��seln�k stav�
/// </summary>
enum class State {
	COUNTING,
	BUM,
	SET
};

/// <summary>
/// Mapa p�echod�
/// </summary>
const map<State, map<Event, State>> Transitions = {

	{State::BUM, {Event::reset, State::COUNTING}},
	{State::BUM, {Event::explosion, State::BUM}},

	{State::COUNTING, {Event::reset, State::COUNTING}},
	{State::COUNTING, {Event::explosion, State::BUM}}
};
*/
