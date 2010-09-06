/*-- Grundfahrzeug --*/

#strict 2

local fDestroyed;
local iLastAttacker;

public func OnDestruction()		{}		//Bei der Zerst�rung des Geb�udes, aber folgenden Reparatur
public func MaxDamage()			{return 100;}	//Maximalschaden

public func GetLastAttacker()		{return iLastAttacker;}
public func IsDestroyed()		{return fDestroyed;}
public func IsCMCVehicle()		{return true;}


/* Initialisierung */

public func Initialize()
{
	fDestroyed = false;
  iLastAttacker = -1;
  return true;
}

/* Schaden */

public func Damage()
{
  if(GetDamage() > MaxDamage() && !IsDestroyed())
  {
   Destroyed();
  }
}

public func OnDmg(int iDmg, int iType)
{
  return 50;	//Default
}

public func OnHit(int iDmg, int iType, object pBy)
{
	if(!IsDestroyed())	
	  iLastAttacker = GetController(pBy);
	return true;
}

/* Zerst�rung */

public func Destroyed()
{
  //Status setzen
  fDestroyed = true;

  //Punkte bei Belohnungssystem
  if(BonusPointCondition() && iLastAttacker != -1)
    if((GetOwner() != -1 && Hostile(GetOwner(), iLastAttacker)) || (GetOwner() == -1 && !GetTeam(this)) || (GetTeam(this) != GetPlayerTeam(iLastAttacker)))
		  DoPlayerPoints(BonusPoints("Destruction"), RWDS_BattlePoints, iLastAttacker, GetCursor(iLastAttacker), IC03);

  //Explosion
  FakeExplode(20, iLastAttacker+1);

  //Sound
  Sound("Blast2", false, this);

  //Letzen Angreifer zur�cksetzen
  iLastAttacker = -1;

  //Callback
  OnDestruction();
}
