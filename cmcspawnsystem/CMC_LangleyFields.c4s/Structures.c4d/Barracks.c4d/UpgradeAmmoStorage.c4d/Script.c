/*-- Munitionsspeicher --*/

#strict 2

#include CCUS

public func ResearchCost()			{return 50;}	//Entwicklungskosten
public func ResearchDuration()			{return 35*10;}	//Entwicklungsdauer in Frames

public func ResearchBase()			{return [];}	//Upgrades, die bereits erforscht sein müssen
public func UpgradeConditions(object pByObj)	{return GetTeamTechLevel(GetPlayerTeam(GetOwner(pByObj)), TECHLEVEL_2);}	//Zusätzliche Bedingungen
