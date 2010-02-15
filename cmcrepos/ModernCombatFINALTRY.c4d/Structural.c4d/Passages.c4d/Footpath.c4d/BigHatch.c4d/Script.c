/*-- Gro�e Bodenlucke --*/

#strict 2

#include GBDR
#include HA4K


/* Initalisierung */

public func Initialize() 
{
   SetAction("Closed");
   _inherited();
   SetMaxDamage(200);
}

/* Schlie�ung */

public func OnClose()
{
  if(GetAction() == "Closed" || GetAction() == "Close") return ;
  SetAction("Close");
  Sound("Airlock2");
  SetSolidMask(0, 30, 30, 30);
}

/* Zerst�rung */

public func OnDestroyed(iType)
{
  //Explosion
  CreateObject(ROCK)->Explode(30);
  SetAction("Destroyed");
  var phase = Random(3);
  SetPhase(phase);

  //Effekte
  CastParticles("MetalSplinter",4,50,0,0,40,150);
  CastParticles("Smoke3",5,10,0,0,50,200);
  Sound("StructuralDamage*.ogg");
}