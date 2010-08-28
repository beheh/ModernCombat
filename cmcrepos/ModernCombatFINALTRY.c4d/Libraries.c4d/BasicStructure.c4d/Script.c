/*-- Grundstruktur --*/

#strict 2

local fDestroyed;
local fRepairing;
local iAutorepairWait;
local iLastAttacker;

public func OnDestruction()		{}		//Bei der Zerstörung des Gebäudes, aber folgenden Reparatur
public func OnRepair()			{}		//Nach der Wiederinstandsetzung
public func AutoRepairDuration()	{return 36*20;}	//Dauer der Reparatur
public func BonusPointCondition()	{return true;}	//Zusätzlicher Callback, ob Punkte vergeben werden
public func MaxDamage()			{return 100;}	//Maximalschaden

public func IsRepairing()		{return fRepairing;}
public func IsDestroyed()		{return fDestroyed;}
public func IsCMCStructure()		{return true;}
public func AutoRepairWait()		{return iAutorepairWait;}


/* Initialisierung */

public func Initialize()
{
  fDestroyed = false;
  fRepairing = false;
  iAutorepairWait = 36*20;
  iLastAttacker = -1;
  SetAction("Idle");
  return true;
}

/* Reparatur */

public func StartRepair()
{
  ClearScheduleCall(this, "StartRepair");
  if(!fRepairing)
  {
   fRepairing = true;
   SetAction("RepairStart");
  }
}

public func Repair()
{ 
  //Jetzt gepanzert
  DoDamage(-GetDamage());
  if(!GetEffect("IntRepair")) AddEffect("IntRepair",this,50,5,this);
}

/* Reparatureffekt */

public func FxIntRepairStart(object pTarget, int iEffectNumber, int iTemp)
{
  Sound("Repair.ogg",false,this,50,0,+1); 
  return 1;
}

public func FxIntRepairTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(iEffectTime >= AutoRepairDuration())
   return -1;

  if(!Random(2))
   Sparks(2+Random(5), RGB(187, 214, 224), RandomX(-GetDefWidth()/2,+GetDefWidth()/2), RandomX(-GetDefHeight()/2,+GetDefHeight()/2));
}

public func FxIntRepairStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  Sound("Repair.ogg",false,this,0,0,-1); 
  if(!iReason)
    ObjectSetAction(pTarget, "RepairStop");
}

public func StopRepair()
{
  fRepairing = false;
  fDestroyed = false;
  DoDamage(-GetDamage());
  OnRepair();
}

public func AutoRepair()
{
  if(AutoRepairWait() > 0) ScheduleCall(this, "StartRepair", AutoRepairWait());
}

public func SetAutoRepair(int iAuto)
{
  iAutorepairWait = iAuto;
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

  if(fRepairing && iType == DMG_Projectile)
   Sound("BlockOff*.ogg");
}

/* Zerstörung */

public func Destroyed()
{
  //Status setzen
  SetAction("Destroyed");
  fDestroyed = true;

  //Punkte bei Belohnungssystem
  if(BonusPointCondition() && iLastAttacker != -1)
    if((GetOwner() != -1 && Hostile(GetOwner(), iLastAttacker)) || (GetOwner() == -1 && !GetTeam(this)) || (GetTeam(this) != GetPlayerTeam(iLastAttacker)))
		  DoPlayerPoints(BonusPoints("Destruction"), RWDS_BattlePoints, iLastAttacker, GetCursor(iLastAttacker), IC03);

  //Explosion
  FakeExplode(20, iLastAttacker+1);

  //Sound
  Sound("Blast2", false, this);

  //Letzen Angreifer zurücksetzen
  iLastAttacker = -1;

  //Callback
  OnDestruction();

  //Reparatur anordnen
  AutoRepair();
}