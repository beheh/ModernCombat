/*-- Grundstruktur --*/

#strict 2

local fDestroyed;
local fRepairing;
local iAutorepairWait;
local iLastAttacker;

public func OnDestruction()		{}				//Bei der Zerstörung des Gebäudes, aber folgenden Reparatur
public func OnRepair()			{}				//Nach der Wiederinstandsetzung
public func AutoRepairDuration()	{return 36*20;}			//Dauer der Reparatur
public func BonusPointCondition()	{return true;}			//Zusätzlicher Callback, ob Punkte vergeben werden
public func MaxDamage()			{return 100;}			//Maximalschaden
public func AutoRepairWait()		{return iAutorepairWait;}	//Zeit bis zu Autoreparatur

public func IsRepairing()		{return fRepairing;}		//Reparierend
public func IsDestroyed()		{return fDestroyed;}		//Zerstört
public func IsCMCStructure()		{return true;}			//Ist eine CMC Struktur

public func IsRepairable()		{return true;}			//Reparierbar
public func RepairSpeed()		{return 0;}			//Standard-Reparaturgeschwindigkeit


/* Initialisierung */

public func Initialize()
{
  fDestroyed = false;
  fRepairing = false;
  iAutorepairWait = 80*20;
  iLastAttacker = -1;
  SetAction("Idle");
  return true;
}

/* Reparatur */

local iRejectRepairCounter;

public func RejectRepair()
{
  if(--iRejectRepairCounter < 0)
    iRejectRepairCounter = RepairSpeed();

  return iRejectRepairCounter;
}

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
  if(!GetEffect("IntRepair")) AddEffect("IntRepair",this,50,6,this);
}

/* Reparatureffekt */

public func FxIntRepairStart(object pTarget, int iEffectNumber, int iTemp)
{
  Sound("Repair.ogg",false,this,50,0,+1); 
  return 1;
}

public func FxIntRepairTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(GetDamage(pTarget) <= 0)
    return -1;

  DoDamage(-1, pTarget);

  if(!Random(2))
  {
    Sparks(2+Random(5), RGB(250,150,0), RandomX(-GetDefWidth()/2,+GetDefWidth()/2), RandomX(-GetDefHeight()/2,+GetDefHeight()/2));
    Sparks(2+Random(5), RGB(100,100,250), RandomX(-GetDefWidth()/2,+GetDefWidth()/2), RandomX(-GetDefHeight()/2,+GetDefHeight()/2));
  }
}

public func FxIntRepairDamage(object pTarget, int iNr, int iDmgEngy)
{
  //Reparatur darf nicht in die Länge gezogen werden
  if(iDmgEngy > 0)
    return 0;

  return iDmgEngy;
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

public func Damage(int change)
{
  //Struktur zerstören, aber mehr Schaden als den Maximalen nicht zulassen
  if(change > 0 && IsDestroyed())
  {
    if(GetDamage() > MaxDamage())
      DoDamage(-(GetDamage()-MaxDamage()));
  }

  //Bei höherem Schaden als dem Maximalen entsprechend zerstören
  if(GetDamage() > MaxDamage() && !IsDestroyed())
  {
    Destroyed();
  }

  //Bei beendetem Reparaturvorgang Sonderfunktionen aufrufen
  if(IsDestroyed() && GetDamage() == 0 && !IsRepairing())
  {
    fDestroyed = false;
    ObjectSetAction(this, "RepairStop");
    OnRepair();
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

  //Schaden auf Maximalwert setzen
  if(GetDamage() > MaxDamage())
    DoDamage(-(GetDamage()-MaxDamage()));

  //Punkte bei Belohnungssystem (Strukturzerstörung)
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

public func OnRepairing()
{
  if(IsDestroyed() && !IsRepairing())
    StartRepair();

  return true;
}

public func IsFullyRepaired()
{
  RemoveEffect("IntRepair", this);
  return true;
}