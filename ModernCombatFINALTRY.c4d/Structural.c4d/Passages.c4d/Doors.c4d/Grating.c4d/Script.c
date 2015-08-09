/*-- L¸ftungsgitter --*/

#strict
#include GDDR


/* Schaden */

public func OnDestroyed(iType)
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter", 3, 80, 0,0, 70, 155);
  Sound("CrateDestruct*.ogg");

  SetAction("Destroyed");
}

/* ÷ffnen und Schlieﬂen */

public func OnOpen()
{
  //Nichts tun wenn bereits offen
  if( GetAction() eq "Open" || Status) return(0);

  //Aktion setzen und SolidMask erstellen
  SetAction("Open");
  SetSolidMask(0);
  Status = true;

  Sound("Airlock1");
}

public func OnClose()
{
  //Nichts tun wenn bereits zu
  if( GetAction() eq "Close"|| !Status) return(0);  

  //Aktion setzen und SolidMask entfernen
  SetAction("Close");
  Status = false;
  SetSolidMask(1, 0, 8, 20, 0, 0);

  Sound("Airlock2");
}