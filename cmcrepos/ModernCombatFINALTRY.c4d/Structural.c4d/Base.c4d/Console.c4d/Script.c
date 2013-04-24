/*-- Konsole --*/

#strict 2

local target;

public func IsMachine()			{return 1;}
public func GetRealRepairableObject()	{return target;}
public func IsFakeRepairable(int iPlr)		
{
  var fAdd = GetDamage(target);
  if(Hostile(iPlr, GetOwner(target)))
    fAdd = true;

  return (target && target->~IsRepairable() && fAdd);
}


/* Timer */

protected func Timer(pClonk)
{
  //Kein Zielobjekt: Selbstzerst�rung
  if(!target)
  {
   if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,50,0,0,40,150);
   CastSmoke("Smoke3",5,10,0,0,50,200, RGBa(255,255,255,0));
   Sound("StructuralDamage*.ogg");
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

  //Kein Men� wenn kein Ziel
  if(!target) return;

  //Sicht auf Zielobjekt zentrieren
  SetPlrView(GetController(pClonk),target);

  //Men� erstellen
  CreateMenu(GetID(target), pClonk, target, 0, Format("$Control$: %s", GetName(target)), 0, 1);
  for(var i = 1, desc ; desc = target->~ConsoleControl(i) ; i++)
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
}
