/*--- Spawnantenne ---*/

#strict 2
#include CSTR
#include FDSE
#include CRSP

public func BuildCost() {return 50;}
public func AutoRepairDuration()	{return 0;}
public func MaxDamage()			{return 30;}
public func IsRepairable()	{return !fDestroyed;}
public func IsRespawnPoint(){return !fDestroyed;}

public func GetRespawnPoints()
{
	return [GetX(), GetY()];
}

func Construction(object kit)
{
	SetObjectTeam(GetPlayerTeam(GetOwner()));
	return _inherited(kit);
}
public func Destroyed()
{
	FadeOut(this);
}

public func Replaced()
{
	FadeOut(this);
}
