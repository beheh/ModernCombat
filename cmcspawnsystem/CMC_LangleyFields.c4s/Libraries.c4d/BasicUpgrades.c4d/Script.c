/*-- Grundupgrade --*/

#strict 2

public func IsGroupUpgrade()			{return false;}	//Teamglobales Gruppenupgrade
public func ResearchCost()			{return 1;}	//Entwicklungskosten
public func ResearchDuration()			{return 100;}	//Entwicklungsdauer in Frames

public func ResearchBase()			{return [];}	//Upgrades, die bereits erforscht sein müssen
public func UpgradeConditions(object pByObj)	{return true;}	//Zusätzliche Bedingungen


public func CanBeResearched(object pByObj)
{
  var upgradeList;
  if(!IsGroupUpgrade())
    upgradeList = GetTeamUpgradeList(GetPlayerTeam(GetOwner(pByObj)));
  else
    upgradeList = pByObj->~GetUpgradeList();

  for(var base in ResearchBase())
    if(GetIndexOf(base, upgradeList) == -1)
      return false;

  return true;
}

public func Researched(object pBy)
{
  if(!IsGroupUpgrade() && pBy && pBy->~IsCMCBuilding())
  {
    OnUpgrade(pBy);
    return pBy->AddUpgrade(GetID());
  }

  if(IsGroupUpgrade())
  {
    OnGroupUpgrade(pBy);
    return AddTeamUpgrade(GetPlayerTeam(GetOwner(pBy)), GetID());
  }
}

public func OnUpgrade(object pBy)		{}
public func OnGroupUpgrade(object pBy)	{}