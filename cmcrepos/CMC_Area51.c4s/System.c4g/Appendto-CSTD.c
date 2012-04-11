/*-- Neues Script --*/

#strict 2

#appendto CSTD

public func InitializePlayer()
{
	LoadRanks2Cache();
	
	return _inherited(...);
}
