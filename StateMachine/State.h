#pragma once
//using namespace std;
//#include <map>



/* todo odstranit pak (nepouziva se) 
/// <summary>
/// Èíselník stavù
/// </summary>
enum class State {
	COUNTING,
	BUM,
	SET
};

/// <summary>
/// Mapa pøechodù
/// </summary>
const map<State, map<Event, State>> Transitions = {

	{State::BUM, {Event::reset, State::COUNTING}},
	{State::BUM, {Event::explosion, State::BUM}},

	{State::COUNTING, {Event::reset, State::COUNTING}},
	{State::COUNTING, {Event::explosion, State::BUM}}
};
*/
