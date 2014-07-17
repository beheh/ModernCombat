/*-- Windanlage --*/

#strict 2

#include CCUS

public func ResearchCost()			{return 100;}	//Entwicklungskosten
public func ResearchDuration()			{return 35*6;}	//Entwicklungsdauer in Frames

public func ResearchBase()			{return [];}	//Upgrades, die bereits erforscht sein müssen
public func UpgradeConditions(object pByObj)	{return true;}	//Zusätzliche Bedingungen
