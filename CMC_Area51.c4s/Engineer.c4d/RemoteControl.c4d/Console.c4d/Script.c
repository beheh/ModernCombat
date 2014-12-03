/*-- Konsole --*/

#strict 2

local target;
local pRemoteControl;

public func IsMachine()			{return 1;}
public func GetRealRepairableObject()	{return target;}
public func IsConsole()	{return true;}
public func IsFakeRepairable(int iPlr)
{
  var fAdd = GetDamage(target);
  if(Hostile(iPlr, GetOwner(target)) || HostileTeam(GetPlayerTeam(iPlr), target->~GetTeam()))
    fAdd = true;

  return (target && target->~IsRepairable() && fAdd);
}


/* Timer */

protected func Timer(pClonk)
{
  //Kein Zielobjekt: Selbstzerstörung
  if(!target)
  {
   if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,50,0,0,40,150);
   CastSmoke("Smoke3",5,10,0,0,50,200, RGBa(255,255,255,0));
   Sound("StructureHit*.ogg");
   return RemoveObject();
  }

  //Blinkeffekt
  AddLightFlash(100,0,0,RGB(0,0,255));
  CreateParticle("NoGravSpark", -8, 1, 0, 0, 50, RGBa(0, 0, 255, 50));
  CreateParticle("NoGravSpark", 8, 1, 0, 0, 50, RGBa(0, 0, 255, 50));
}

/* Bedienung */

protected func ControlDig(pClonk)
{
  [$Control$]

  //Kein Menü wenn kein Ziel
  if(!target) return;

  //Sicht auf Zielobjekt zentrieren
  SetPlrView(GetController(pClonk),target);

  //Menü erstellen
  CreateMenu(GetID(target), pClonk, target, 0, Format("$Control$: %s", GetName(target)), 0, 1);
  for(var i = 1, desc ; desc = target->~ConsoleControl(i, pClonk, this) ; i++)
  {
    AddMenuItem(desc, Format("ConsoleControlled(%d, %d, %d)", i, ObjectNumber(pClonk), ObjectNumber(this)), GetID(), pClonk, 0, 0, "$Control$");
    Sound("Acknowledge.ogg", 0, pClonk, 100, GetOwner(pClonk)+1);
  }
  return 1;
}

/* Ziel setzen */

public func Set(pTarget)
{
  target = pTarget;

  //Konsolen, die keine SSA steuern, grün färben
  if(GetID(pTarget) != SEGU)
    SetClrModulation(RGB(0,150,0));
}
