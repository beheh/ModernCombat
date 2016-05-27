/*-- Elektrische Kugel --*/

#strict 2
#include SHT1

public func TrailColor(int iATime)	{return Color(iATime);}
public func AllowHitboxCheck()		{return true;}

local iShotAngle, iShotSpeed;


/* Schuss */

private func CreateTrail(int iSize, int iTrail)
{
  if(pTrail)
  {
    pTrail->Set(iSize+1,iTrail,this);
    ObjectSetAction(pTrail, "Travel2");
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

public func Launch()
{
  iShotAngle = Par();
  iShotSpeed = Par(1);
  var i = _inherited(...);
  SetPosition(GetX(), GetY() - GetDefCoreVal("Offset", "DefCore", GetID(), 1) / 2);
  if(!GetEffect("TracerTrail", this))
    AddEffect("TracerTrail", this, 1, 1, this);
  return i;
}

protected func FxTracerTrailTimer()
{
  var iX = -Sin(iShotAngle, iShotSpeed/10),
  iY = Cos(iShotAngle, iShotSpeed/10),
  iDist = -1,
  iClr = TrailColor(GetActTime());
  while(++iDist < iShotSpeed/10)
    CreateParticle("Flare", iX + Sin(iShotAngle, iDist) + Random(5) - 2, iY - Cos(iShotAngle, iDist) + Random(5) - 2, Sin(Random(360), Random(10)), Sin(Random(360), Random(10)), 60 + Random(51), iClr);
}

/* Timer */

private func Traveling()
{
  //In Wasser verschwinden
  if(InLiquid()) return Remove();

  var iATime = GetActTime();

  //Ausfaden
  SetClrModulation(Color(iATime));

  //Löschen
  if(iATime >= iTime) return Remove();

  //Bei Verlassen der Map entfernen
  if(GetY()<0) return Remove();
}

/* Treffer */

func Hit(int iXDir, int iYDir)
{
  //Erst nochmal zeichnen
  SetXDir(iXDir, this, 100);
  SetYDir(iYDir, this, 100);
  EffectCall(this, GetEffect("TracerTrail", this), "Timer");
  LaserLight(30, SetRGBaValue(Color(GetActTime())), 0, 0, 10);

  //Entfernen
  Remove();
}

private func HitObject(object pObject)
{
  if(BulletStrike(pObject))
    LaserLight(30, SetRGBaValue(Color(GetActTime())), 0, 0, 10);
}

public func BulletStrike(object pObj)
{
  //Schaden für getroffenes Objekt errechnen
  if(pObj)
  {
    DoDmg(iDamage, DMG_Energy, pObj);

    //Blendeffekt am Clonk erstellen
    AddEffect("FlashlightBlindness", pObj, 100, 1, 0, LHC3);

    //Verschwinden
    Remove();
  }

  return false;
}

/* Farbeffekt */

private func Color(int iATime)
{
  var iAlpha = Interpolate2(0, 128, iATime, iTime) + Random(32),
  iColor = LightenColor(GetPlrColorDw(GetController()), 92);
  return SetRGBaValue(iColor, iAlpha);
}

private func GlowColor()	//!INFO
{
  return Color(...);
}