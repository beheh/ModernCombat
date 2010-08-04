/*-- Konsole --*/

#strict 2

local target;

public func IsMachine()	{return 1;}


/* Timer */

protected func Timer(pClonk)
{
  //Kein zu steuerndes Objekt mehr? Selbstzerstörung.
  if(!target)
  {
   if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,50,0,0,40,150);
   CastParticles("Smoke3",5,10,0,0,50,200);
   Sound("StructuralDamage*.ogg");
   return RemoveObject();
  }

  //Blinkeffekt
  if(Random(2)) 
    CreateParticle("NoGravSpark", 3, 1, 0, 0, 50, RGBa(250, 10, 10, 50));
  else
   CreateParticle("NoGravSpark", 3, 3, 0, 0, 50, RGBa(10, 10, 250, 50));
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