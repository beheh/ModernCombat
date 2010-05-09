/*-- Peilsender --*/

#strict
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
    Sound("TRDT_Attach.ogg");
    if(!GetEffect("TracerDart",pObj))
    {
      AddEffect("TracerDart",pObj);
    }
  }
  return(1);
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

func FxTracerDartStart(object pTarget)
{
  //Besitzer des Schusses festlegen (der Schütze)
  //Haftzeit festsetzen (20 Sekunden)
}

func FxTracerDartTimer(object pTarget)
{
  //Blinkeffekt (ähnlich C4, mittig im Target; Spielerfarben blinken (Farbe des Schützen, nicht des Ziels))
  //Haftzeit verringern
  //Haftzeit zuende? Entfernen.
  //Target im Wasser? Entfernen.
}