/*-- Peilsender --*/

#strict 2
#include SHT1

public func TrailColor(int iATime)	{return(Color(iATime));}


/* Schuss */

private func CreateTrail(int iSize, int iTrail)
{
  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail)
  {
    pTrail->Set(iSize+1,iTrail,this());
    pTrail->SetAction("Travel2");
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

/* Treffer */

private func HitObject(object pObject)
{
  if(BulletStrike(pObject))
  {
    LaserLight(30, Color(GetActTime()),0,0,10);
    Remove();
  }
}

public func BulletStrike(object pObj)
{
  if(pObj)
  {
    if(GetAlive(pObj) && Hostile(GetOwner(pObj), GetController())) {
		  if(!GetEffect("TracerDart",pObj))
		  {
		  	Sound("TRDT_Attach.ogg");
		    AddEffect("TracerDart",pObj,20,1, 0, 0, GetPlayerTeam(GetController()));
		    return 1;
		  }
    }
  }
  return 1;
}

private func Color(int iATime)
{
  var iPrg = 100*iATime/iTime;
  return(RGBa(255,127-iPrg,127,iPrg*2));
}

private func GlowColor()
{
  return(RGB(255,90,110));
}

/* Peilsendereffekt */

global func FxTracerDartStart(object pTarget, int iEffectNumber, int iTemp, int iTeam)
{
  //Besitzer des Schusses festlegen (der Schütze)
  //Haftzeit festsetzen (20 Sekunden)
  EffectVar(0, pTarget, iEffectNumber) = iTeam;
  EffectVar(1, pTarget, iEffectNumber) = 20*38;
}

global func FxTracerDartTimer(object pTarget, int iEffectNumber)
{
  //Blinkeffekt (ähnlich C4, mittig im Target; Spielerfarben blinken (Farbe des Schützen, nicht des Ziels))
  //Uhja.
  
  Message("!", pTarget);
  //Haftzeit verringern
  EffectVar(1, pTarget, iEffectNumber)--;
  //Haftzeit zuende? Entfernen.
  if(EffectVar(1, pTarget, iEffectNumber) <= 0) return -1;
  //Target im Wasser? Entfernen.
  if(GBackLiquid(AbsX(GetX(pTarget)), AbsY(GetY(pTarget)))) return -1;
}
