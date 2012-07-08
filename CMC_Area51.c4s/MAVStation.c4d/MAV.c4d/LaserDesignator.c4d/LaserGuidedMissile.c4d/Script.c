/*-- Luft-Boden-Rakete --*/

#strict 2
#include MISL

public func MaxTime()		{return 300;}	//Maximale Flugzeit
public func MaxSpeed()		{return 80;}	//Maximale Geschwindigkeit

public func SecureTime()	{return 2;}	//Mindestflugzeit
public func SecureDistance()	{return 2;}	//Mindestabstand

public func ExplosionDamage()	{return 32;}	//Explosionsschaden
public func ExplosionRadius()	{return 32;}	//Radius


public func FxFollowStart(object pTarget, int iEffectNumber, int iTemp, obj)
{
  if (iTemp)
    return;
  EffectVar(0,pTarget,iEffectNumber) = obj;
  EffectVar(1,pTarget,iEffectNumber) = obj->GetLaser();
  Sound("Airstrike2.wav");
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

  if(pEnemy && pEnemy->~Active())
    SetR(iAngle+iTurn*((iDiff > 0)*2-1));
  else
    SetR(GetR()+ RandomX(-3,3));
}