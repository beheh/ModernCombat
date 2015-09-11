/*-- Blut --*/

//Das Appendto lässt bestimmte Objekte bei Schaden bluten.

#strict 2

#appendto ANIM	//Tiere
#appendto CLNK	//Clonks
#appendto HZCK	//Hazardclonks


public func OnHit(int iDmg, int iType, object pFrom)
{
  //Lebewesen nicht im Freien: Keine Effekte
  if(Contained(this))
    return;

  //Zu niedrige Schadenswerte ignorieren
  if(iDmg <= 0)
    return;

  //Zu hohe Schadenswerte begrenzen
  if(iDmg > 200)
    iDmg = 200;

  //Effekte
  Splatter(iDmg,iType,pFrom);
  BloodBurst(iDmg,0,0);
  BloodSplatter(iDmg,0,0);
  if(pFrom)
  {
    if((iType == DMG_Explosion)||
       (iType == DMG_Projectile))
    {
      BloodSplatter2(Min(iDmg*2,200),0,0,Angle(GetX(),GetY(),GetX(pFrom),GetY(pFrom)));
    }
  }

  return _inherited(iDmg,iType,pFrom);
}