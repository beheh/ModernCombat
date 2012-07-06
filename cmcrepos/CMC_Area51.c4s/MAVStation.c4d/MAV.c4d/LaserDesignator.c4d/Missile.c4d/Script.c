/*-- Rakete für den Laser --*/

#strict 2
#include MISL

//Die Rakete kommt schon von oben, da is nix mehr mit Sicherung.
public func SecureTime()	{return 2;}			//Mindestflugzeit
public func SecureDistance()	{return 2;}			//Mindestabstand

public func FxFollowStart(object pTarget, int iEffectNumber, int iTemp, obj)
{
  if (iTemp)
    return;
  EffectVar(0,pTarget,iEffectNumber) = obj;
  EffectVar(1,pTarget,iEffectNumber) = obj->GetLaser();
}

public func FxFollowTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Nichts unternehmen wenn abgeschossen
  if(GetAction(pTarget) != "Travel")
  {
    //Licht entfernen?
    if (pLight)
      RemoveObject(pLight);
    return;
  }

  var x = GetX(pTarget)-GetX(), y = GetY(pTarget)-GetY();
  
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

  SetR(iAngle+iTurn*((iDiff > 0)*2-1));
}
