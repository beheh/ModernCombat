/*-- Wasserturbine --*/

#strict 2

#include CCUS

public func ResearchCost()			{return 150;}	//Entwicklungskosten
public func ResearchDuration()			{return 35*10;}	//Entwicklungsdauer in Frames

public func ResearchBase()			{return [];}	//Upgrades, die bereits erforscht sein m�ssen
public func UpgradeConditions(object pByObj)	{return true;}	//Zus�tzliche Bedingungen
