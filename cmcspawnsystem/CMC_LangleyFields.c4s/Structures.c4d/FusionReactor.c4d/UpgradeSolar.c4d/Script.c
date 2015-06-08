/*-- Solarpanele --*/

#strict 2

#include CCUS

public func ResearchCost()			{return 80;}	//Entwicklungskosten
public func ResearchDuration()			{return 35*4;}	//Entwicklungsdauer in Frames

public func ResearchBase()			{return [];}	//Upgrades, die bereits erforscht sein müssen
public func UpgradeConditions(object pByObj)	{return true;}	//Zusätzliche Bedingungen
