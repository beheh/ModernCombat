/*-- Grundstruktur --*/

#strict 2

local fDestroyed;
local fRepairing;
local iAutorepairDuration;

public func OnDestruction()		{}		//Bei der Zerstörung des Gebäudes, aber folgenden Reparatur
public func OnRepair()			{}		//Nach der Wiederinstandsetzung
public func AutoRepairWait()		{return 36*25;}	//Standardzeit bis zur Autoreparatur
public func BonusPointCondition()	{return true;}	//Zusätzlicher Callback, ob Punkte vergeben werden
public func MaxDamage()			{return 100;}	//Maximalschaden

public func IsRepairing()		{return fRepairing;}
public func IsDestroyed()		{return fDestroyed;}
public func IsCMCStructure()		{return true;}
public func AutoRepairDuration()	{return iAutorepairDuration;}


/* Initialisierung */

public func Initialize()
{
  fDestroyed = false;
  fRepairing = false;
  iAutorepairDuration = 36*20;
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

  return 0;
}

public func FxIntRepairStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  Sound("Repair.ogg",false,this,0,0,-1); 
  if(!iReason)
  {
   pTarget->SetAction("RepairStop");
  }
  return 0;
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
  ScheduleCall(this, "StartRepair", AutoRepairWait());
}

public func SetAutoRepair(int iAuto)
{
  iAutorepairDuration = iAuto;
}

/* Schaden */

public func Damage()
{
  if(GetDamage() > MaxDamage() && !fDestroyed)
  {
   Destroyed();
  }
}

public func OnDmg(int iDmg, int iType)
{
  return(50);	//Default
}

public func OnHit(a, b, object pBy)
{
  SetKiller(GetController(pBy));

  if(fRepairing)
   Sound("BlockOff*.ogg");
}

/* Zerstörung */

public func Destroyed()
{
  //Status setzen
  SetAction("Destroyed");
  fDestroyed = true;

  //Reparatur anordnen
  AutoRepair();

  //Punkte bei Belohnungssystem
  if(BonusPointCondition() && GetKiller(this) != -1)
    if((GetOwner() != -1 && Hostile(GetOwner(), GetKiller())) || GetOwner() == -1 && !GetTeam(this))
		  DoPlayerPoints(BonusPoints("Destruction"), RWDS_BattlePoints, GetKiller(this), GetCursor(GetKiller(this)), IC03);

  //Explosion
  CreateObject(ROCK,0,0)->Explode(20);

  //Callback
  OnDestruction();
}