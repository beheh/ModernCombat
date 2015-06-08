/*-- Techlevel 3 --*/

#strict 2

#include CCUS

public func IsGroupUpgrade()    {return true;}
public func ResearchCost()			{return 350;}	//Entwicklungskosten
public func ResearchDuration()			{return 35*25;}	//Entwicklungsdauer in Frames

public func ResearchBase()			{return [U_T2];}	//Upgrades, die bereits erforscht sein müssen
public func UpgradeConditions(object pByObj)  //Zusätzliche Bedingungen
{
  if(!GetTeamTechLevel(GetPlayerTeam(GetOwner(pByObj)), TECHLEVEL_2))
    return false;
  
  for(var obj in FindObjects(Find_Func("IsCMCBuilding"), Find_Allied(GetOwner(pByObj))))
    if(obj->~ProvideTechLevel() == TECHLEVEL_3 && GetCon(obj) >= 100)
      return true;
  
  return false;
}

public func OnGroupUpgrade(object pByObj)
{
  var found;
  for(var obj in FindObjects(Find_Func("IsCMCBuilding"), Find_Allied(GetOwner(pByObj))))
    if(obj->~ProvideTechLevel() == TECHLEVEL_3 && GetCon(obj) >= 100)
    {
      found = true;
      break;
    }
  
  if(!found)
    return;
  
  SetTeamTechLevel(GetPlayerTeam(GetOwner(pByObj)), TECHLEVEL_3, true);
  return true;
}
