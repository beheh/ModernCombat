/*-- Assault Respawnpunkt --*/

#strict 2

#include CRSP

local aTargets;

public func Initialize()
{
	aTargets = [];
	
	return _inherited(...);
}

public func AddTarget(int iTarget) 
{ 
  aTargets[GetLength(aTargets)] = iTarget; 
}

public func AddTargets(array iNumbers) 
{
  for(var i = 0; i < GetLength(iNumbers); i++)	
    AddTarget(iNumbers[i]);
}

public func IsValid(int iTarget)
{
  for(var i = 0; i < GetLength(aTargets); i++)
    if(aTargets[i] == iTarget)
      return true;		

  return false;
}

public func IsRespawnplace(object pClonk) 
{
  return(_inherited(pClonk) && FindObject(GASS) && IsValid(FindObject(GASS)->GetNextTarget()));    	  
}
