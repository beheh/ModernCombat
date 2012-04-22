/*-- Neues Script --*/

#strict 2

#appendto TIM1

// Für Votekicks...:
local votekickVars;

public func Initialize()
{
	votekickVars = [];
	return _inherited(...);
}

public func InputCallback(string reason, int plr)
{
	StartVotekick(plr, votekickVars[0], votekickVars[1], reason, true);
	return true;
}
