/*-- Rauchgranate --*/

#strict
#include ESHL


/* Kein Mindestabstand */

protected func SecureDistance(){return(0);}

/* Timer */

func ExplodeDelay() { return(120); }

/* Treffer */

func Trigger()
{
  //Effekte
  CreateParticle("Blast",0,0,0,0,5*10,RGB(255,255,128));
  CastParticles("Smoke3",8,50,0,0,120,150,RGBa(255,255,255,120),RGBa(255,255,255,150));
  Sound("SGRN_Fused.ogg");

  //Rauch erzeugen
  CastObjects(SM4K,3,10);

  //Verschwinden
  RemoveObject();
}