/*-- Baum --*/

//B�ume sind unzerst�rbar.

#strict 2
#appendto TREE

public func ChopDown()		{}
protected func Incineration()	{Extinguish();}
public func OnDmg()		{if (GetID() != BSH2) return 100;}