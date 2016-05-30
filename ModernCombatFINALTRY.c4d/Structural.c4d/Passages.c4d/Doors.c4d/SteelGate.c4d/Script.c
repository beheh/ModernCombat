/*--- Stahltor ---*/

#strict 2
#include CSTR

local direction;

public func IsMachine()		{return true;}
public func MaxDamage()		{return 80;}
public func RepairSpeed()	{return 3;}


/* Initialisierung */

protected func Initialize()
{
  SetAction("GateStill");
  direction = 0;

  return inherited();
}

/* Öffnen und Schließen */

public func Open()
{
  var phase = GetPhase();
  SetAction("GateOpen");
  SetPhase(phase);
  CheckSolidMask();
  direction = 0;
}

protected func Opened()
{
  SetAction("GateStill");
  SetPhase(11);
  StopSound();
  CheckSolidMask();
  direction = 1;
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
  var phase;
  if(GetAction() == "GateClose")
    phase = 11-GetPhase();
  else
    phase = GetPhase();
  SetAction("GateStill");
  SetPhase(phase);
  StopSound();
  CheckSolidMask();
  if(GetPhase() == 0)
    direction = 0;
}

/* Konsolensteuerung */

public func ConsoleControl(int i)
{
  if(IsRepairing()) return;
  if(GetAction() != "Destroyed")
  {
    if(i == 1)
      if(GetAction() == "GateStill")
        if(GetPhase() == 0 || direction == 0)
          return "$Open$";
        else
          return "$Close$";
      else
        return "$Stop$";

    if(i == 2)
      if(direction == 1)
        return "$Open$";
      else
        return "$Close$";
  }
  else
    if(i == 1) return "$Repair$";
}

public func ConsoleControlled(int i)
{
  if(IsRepairing()) return;
  if(GetAction() != "Destroyed")
  {
    if(i == 1)
      if(GetAction() == "GateStill")
        if(GetPhase() == 0 || direction == 0)
          Open();
        else
          if(GetAction() == "GateClose")
            return;
          else
            Close();
      else
        Stop();

    if(i == 2)
      if(direction == 1)
      {
        direction = 0;
        Open();
      }
      else
      {
        direction = 1;
        Close();
      }
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
  if(GetEffectData(EFSM_ExplosionEffects) > 1)
  {
    CastParticles("MetalSplinter",4,110,0,20,40,100);
    CastParticles("MetalSplinter",4,110,0,-20,40,100);
  }
  CastSmoke("Smoke3",5,20,0,20,220,500);
  CastSmoke("Smoke3",5,20,0,-20,220,500);

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