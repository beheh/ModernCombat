/*-- Neues Script --*/

#strict 2

#appendto RWDS

public func Evaluate()
{
	if(FindObject(MCSL))
		Log("%v", LocalN("aOpenBetaStats", FindObject(MCSL)));
		
	return _inherited(...);
}
