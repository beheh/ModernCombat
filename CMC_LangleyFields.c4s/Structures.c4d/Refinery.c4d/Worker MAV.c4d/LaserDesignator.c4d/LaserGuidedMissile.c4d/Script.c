/*-- Luft-Boden-Rakete --*/

#strict 2
#include MISL

public func MaxTime()		{return 400;}	//Maximale Flugzeit
public func MaxSpeed()		{return 90;}	//Maximale Geschwindigkeit

public func SecureTime()	{return 2;}	//Mindestflugzeit
public func MaxDamage()		{return 40;}	//Maximalschaden bis Absturz

public func ExplosionDamage()	{return 32;}	//Explosionsschaden
public func ExplosionRadius()	{return 32;}	//Radius


public func FxFollowStart(object pTarget, int iEffectNumber, int iTemp, object pObj)
{
  if (iTemp)
    return;
  EffectVar(0,pTarget,iEffectNumber) = pObj;
  EffectVar(1,pTarget,iEffectNumber) = pObj->GetLaser();
}

public func FxFollowTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Nichts unternehmen wenn abgeschossen
  if(GetAction(pTarget) != "Travel")
  {
    //Licht entfernen?
    if(pLight)
      RemoveObject(pLight);
    return;
  }

  //Soll-Winkel
  var iDAngle;
  var iMaxTurn;
  //Sonst anvisieren
  if(EffectVar(1,pTarget,iEffectNumber))
  {
    var pEnemy = EffectVar(1,pTarget,iEffectNumber);
    iDAngle = Angle(GetX(), GetY(), GetX(pEnemy), GetY(pEnemy));
    iMaxTurn = MaxTracerTurn();
  }

  var iAngle = GetR();
  var iDiff = Normalize(iDAngle - iAngle,-180);
  var iTurn = Min(Abs(iDiff),iMaxTurn);

  if(pEnemy && pEnemy->~Active())
    SetR(iAngle+iTurn*((iDiff > 0)*2-1));
  else
    SetR(GetR()+ RandomX(-6,6));
}