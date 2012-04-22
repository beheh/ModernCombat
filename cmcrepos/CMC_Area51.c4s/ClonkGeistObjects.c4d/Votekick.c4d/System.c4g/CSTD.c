/*-- Neues Script --*/

#strict 2

#appendto CSTD

public func InitializePlayer()
{
	AddMsgBoardCmd("votekick", "Votekick(%player%, \"%s\")");
	return _inherited(...);
}
