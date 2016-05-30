/*-- Konsole --*/

#strict 2

local target;

public func IsConsole()			{return true;}
public func IsMachine()			{return 1;}
public func GetRealRepairableObject()	{return target;}
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

protected func ControlThrow(pClonk)
{
  //Keine Aktion wenn kein Ziel
  if(!target) return;

  //Sicht auf Zielobjekt zentrieren und benennen
  SetPlrView(GetController(pClonk),target);
  target->PlayerMessage(GetController(pClonk),GetName(target),target);

  //Ersten Menüpunkt ausführen
  ControlTarget(1, ObjectNumber(pClonk), ObjectNumber(this));

  return 1;
}

protected func ControlDig(pClonk)
{
  //Keine Aktion wenn kein Ziel
  if(!target) return;

  //Sicht auf Zielobjekt zentrieren und benennen
  SetPlrView(GetController(pClonk),target);
  target->PlayerMessage(GetController(pClonk),GetName(target),target);

  //Menü erstellen
  CreateMenu(GetID(), pClonk, this, 0, Format("%s", GetName(target)), 0, 1);
  for(var i = 1, desc ; desc = target->~ConsoleControl(i, pClonk, this) ; i++)
  {
    AddMenuItem(desc, Format("ControlTarget(%d, %d, %d)", i, ObjectNumber(pClonk), ObjectNumber(this)), GetID(target), pClonk);
    Sound("Acknowledge.ogg", 0, pClonk, 100, GetOwner(pClonk)+1);
  }
  return 1;
}

protected func ControlTarget(int selection, int objn, int objn2)
{
  if(!target)
    return;

  target->ConsoleControlled(selection, objn, objn2);

  Sound("Switch.ogg");
}

/* Ziel setzen */

public func Set(pTarget)
{
  target = pTarget;

  //Konsolen, die keine SSA steuern, grün färben
  if(GetID(pTarget) != SEGU)
    SetClrModulation(RGB(0,150,0));
}