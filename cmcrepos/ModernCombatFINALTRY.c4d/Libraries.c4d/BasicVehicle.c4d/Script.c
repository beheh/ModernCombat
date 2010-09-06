/*-- Grundfahrzeug --*/

#strict 2

local fDestroyed;
local iLastAttacker;
local aDealers;

public func OnDamage()		{}		//Beim Erhalten von Schaden
public func OnDestruction()		{}		//Bei der Zerstörung des Fahzeugs
public func MaxDamage()			{return 100;}		//Maximalschaden
public func BonusPointCondition()			{return Hostile(GetLastAttacker(),GetController());}			//Ob bei der Zerstörung Punkte vergeben werden

public func GetLastAttacker()		{return iLastAttacker;}
public func IsDestroyed()		{return fDestroyed;}
public func IsCMCVehicle()		{return true;}


/* Initialisierung */

public func Initialize()
{
	fDestroyed = false;
	iLastAttacker = -1;
	aDealers = CreateArray();
	return true;
}

/* Schaden */

public func Damage()
{
	if(GetDamage() > MaxDamage() && !IsDestroyed())
	{
		Destroyed();
	}
	OnDamage();
}

public func OnDmg(int iDmg, int iType)
{
  return 50;	//Default
}

public func OnHit(int iDmg, int iType, object pBy)
{
	var iPlr = GetController(pBy);
	if(!IsDestroyed())
	  iLastAttacker = iPlr;
	if(!aDealers)
		aDealers = CreateArray();
	if(Hostile(iPlr, GetController())) {
		if(!aDealers[iPlr]) aDealers[iPlr] = 0;
		aDealers[iPlr] += iDmg;
		while(aDealers[iPlr] >= 50) {
			DoPlayerPoints(BonusPoints("VehicleAssist"), RWDS_BattlePoints, iPlr, pBy, IC18);
			aDealers[iPlr] -= 50;
		}
	}
	return true;
}

/* Zerstörung */

public func Destroyed()
{
  //Status setzen
  fDestroyed = true;

  //Punkte bei Belohnungssystem
  if(BonusPointCondition() && iLastAttacker != -1)
    if((GetOwner() != -1 && Hostile(GetOwner(), iLastAttacker)) || (GetOwner() == -1 && !GetTeam(this)) || (GetTeam(this) != GetPlayerTeam(iLastAttacker)))
		  DoPlayerPoints(BonusPoints("Destruction"), RWDS_BattlePoints, iLastAttacker, GetCursor(iLastAttacker), IC03);

  //Sound
  Sound("Blast2", false, this);

  //Letzen Angreifer zurücksetzen
  iLastAttacker = -1;

  //Callback
  OnDestruction();
}