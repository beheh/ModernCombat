/*--- Stahltor ---*/

#strict 2
#include CSTR

public func IsMachine()		{return true;}
public func MaxDamage()		{return 80;}
public func RepairSpeed()	{return 3;}


/* Initialisierung */

protected func Initialize()
{
  SetAction("GateStill");

  return inherited();
}

/* Öffnen und Schließen */

public func Open()
{
  var phase = GetPhase();
  SetAction("GateOpen");
  SetPhase(phase);
  CheckSolidMask();
}

protected func Opened()
{
  SetAction("GateStill");
  SetPhase(11);
  StopSound();
  CheckSolidMask();
}

public func Close()
{
  var phase = 11-GetPhase();
  SetAction("GateClose");
  SetPhase(phase);
  CheckSolidMask();
}

public func Stop()
{
  var phase = GetPhase();
  SetAction("GateStill");
  SetPhase(phase);
  StopSound();
  CheckSolidMask();
}

/* Konsolensteuerung */

public func ConsoleControl(int i)
{
  if(IsRepairing()) return;
  if(GetAction() != "Destroyed")
  {
    if(i == 1) return("$Open$");
    if(i == 2) return("$Close$");
    if(i == 3) return("$Stop$");
  }
  else
    if(i == 1) return("$Repair$");
}

public func ConsoleControlled(int i)
{
  if(GetAction() != "Destroyed")
  {
    if(i == 1 && GetAction() != "GateOpen") Open();
    if(i == 2 && GetAction() != "GateClose") Close();
    if(i == 3 && GetAction() != "GateStill") Stop();
  }
  else
    if(i == 1)
      StartRepair();
}

private func StopSound()	{Sound("Discharge");}

private func CheckSolidMask()
{
  SetSolidMask(10*GetPhase(), 0, 10, 60, 0, 0);
}

/* Bonus-Punkte */

public func BonusPointCondition()
{
  return false;
}

/* Reperatur */

public func OnRepair()
{
  SetAction("GateOpen");
  Opened();
}

/* Schaden */

public func OnDestruction()
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
  {
    CastSmoke("Smoke3",5,20,0,20,220,500);
    CastSmoke("Smoke3",5,20,0,-20,220,500);
  }
  if(GetEffectData(EFSM_ExplosionEffects) > 1)
  {
    CastParticles("MetalSplinter",4,110,0,20,40,100);
    CastParticles("MetalSplinter",4,110,0,-20,40,100);
  }

  //Tor sofort offen
  Opened();
  SetAction("Destroyed");
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)			return 80;	//Feuer
  if(iType == DMG_Explosion)		return;		//Explosionen und Druckwellen
  if(iType == DMG_Bio)			return 100;	//Säure und biologische Schadstoffe
  return 80;
}