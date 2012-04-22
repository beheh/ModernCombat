/*-- Neues Script --*/

#strict 2

#appendto CSTD

public func Initialize()
{
	AddMsgBoardCmd("votekick", "Votekick(%player%, \"%s\")");
	return _inherited(...);
}
