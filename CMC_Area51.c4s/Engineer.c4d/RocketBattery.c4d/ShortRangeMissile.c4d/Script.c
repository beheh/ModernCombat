/*-- Kurzstreckenrakete --*/

#strict 2
#include MISL


public func MaxTime()		{return 100;}			//Maximale Flugzeit

public func StartSpeed()	{return 20;}			//Startgeschwindigkeit
public func Acceleration()	{return 4;}			//Beschleunigung
public func MaxSpeed()		{return 150;}			//Maximale Geschwindigkeit

public func SecureTime()	{return 10;}			//Mindestflugzeit

public func ExplosionDamage()	{return 20;}			//Explosionsschaden

public func MaxTurn()		{return 7;}			//max. Drehung
public func MaxTracerTurn()	{return 9;}			//max. Drehung bei Zielverfolgung


private func Detonate()
{
  Explode(ExplosionDamage());
  DamageObjects(ExplosionRadius(), ExplosionDamage()/2, this);
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

  //Rakete unterstützt Peilsender?
  if(pTarget->~TracerCompatible() && Guideable())
  {
    var x = GetX(pTarget)-GetX(), y = GetY(pTarget)-GetY();
    //Gültigkeit des Ziels prüfen
    if(EffectVar(1,pTarget,iEffectNumber))
    {
      var pEnemy = EffectVar(1,pTarget,iEffectNumber);
      var del;
      if(!GetEffect("TracerDart", pEnemy)) del = true;
      if(!PathFree(GetX(pTarget), GetY(pTarget), GetX(pEnemy), GetY(pEnemy))) del = true;
      if(!fTracerChasing && Distance(GetX(pTarget), GetY(pTarget), GetX(pEnemy), GetY(pEnemy)) > TracerRadius()) del = true;
      if(del) EffectVar(1,pTarget,iEffectNumber) = 0;
    }
    //Haben wir noch ein markiertes Ziel?
    if(!EffectVar(1,pTarget,iEffectNumber))
    {
      fTracerChasing = false;
      EffectVar(3, pTarget, iEffectNumber) = 0;
      if(EffectVar(2, pTarget, iEffectNumber))
        RemoveObject(EffectVar(2, pTarget, iEffectNumber));
      for(var pEnemy in FindObjects(Find_Distance(TracerRadius(), x, y), Sort_Distance(x, y)))
      {
        var iEffectTracer = GetEffect("TracerDart", pEnemy);
        if(!iEffectTracer) continue;
        var iTeam = EffectVar(2, pEnemy, iEffectTracer);
        if(iTeam != GetPlayerTeam(GetController())) continue;
        if(!PathFree(GetX(), GetY(), GetX(pEnemy), GetY(pEnemy))) continue;
        EffectVar(1, pTarget, iEffectNumber) = pEnemy;
        fTracerChasing = true;
        break;
      }
    }
  }
  //Soll-Winkel
  var iDAngle;
  var iMaxTurn;

  //Sonst anvisieren
  if(EffectVar(1,pTarget,iEffectNumber))
  {
    var pEnemy = EffectVar(1,pTarget,iEffectNumber);
    var pBeam = EffectVar(2, pTarget, iEffectNumber);
    if(!fTracerChasing)
    {
      var xPos = GetX(pTarget);
      var yPos = GetY(pTarget);
      var x = GetX(pEnemy);
      var y = GetY(pEnemy);
      //Laser zeichnen
      if(!pBeam)
        pBeam = CreateObject(LRBM, 0, 0, GetOwner(pTarget));
      pBeam->SetPosition(x, y);

      //Sichtbarkeit nur für Besitzer
      pBeam->SetVisibility(VIS_Owner);

      //Laser passend strecken
      pBeam->SetObjDrawTransform(100 * Distance(xPos, yPos, x, y), 0, -453 * Distance(xPos, yPos, x, y), 0, 1000, 0);
      pBeam->SetR(Angle(x, y, xPos, yPos)+90);

      EffectVar(2, pTarget, iEffectNumber) = pBeam;

      Sound("MISL_Click.ogg", 0, this, 50, GetOwner()+1);

      //Da noch keine automatische Steuerung erfolgt, normale Steuerung fortsetzen
      //Kann nicht gesteuert werden
      if(!Guideable())
        return;
      var obj = EffectVar(0,pTarget,iEffectNumber);
      if(!obj)
        return;
      //Schütze nicht mehr am Zielen?
      if(!obj->~IsAiming())
        return;
      //Schütze kann mit der Waffe nicht zielen
      if(pLauncher && Contents(0, obj) != pLauncher)
        return;

      iDAngle = obj->AimAngle();
      iMaxTurn = MaxTurn();
    }
    else
    {
      if(pBeam)
      {
        var xPos = GetX(pTarget);
        var yPos = GetY(pTarget);
        var x = GetX(pEnemy);
        var y = GetY(pEnemy);

        pBeam->SetPosition(x, y);

        //Laser passend strecken
        pBeam->SetObjDrawTransform(100 * Distance(xPos, yPos, x, y), 0, -453 * Distance(xPos, yPos, x, y), 0, 1000, 0);

        pBeam->SetR(Angle(x, y, xPos, yPos)+90);

        var iAlpha = EffectVar(3, pTarget, iEffectNumber);
        iAlpha += 3;
        if(iAlpha >= 255)
          RemoveObject(pBeam);
        else
          SetClrModulation(RGBa(255, 255, 255, iAlpha), pBeam);
        EffectVar(3, pTarget, iEffectNumber) = iAlpha;
      }
      iDAngle = Angle(GetX(), GetY(), GetX(pEnemy), GetY(pEnemy));
      iMaxTurn = MaxTracerTurn();
    }
  }
  else
  {
    //Kann nicht gesteuert werden
    if(!Guideable())
      return;
    var obj = EffectVar(0,pTarget,iEffectNumber);
    if(!obj)
      return;
    //Schütze nicht mehr am Zielen?
    if(!obj->~IsAiming())
      return;
    //Schütze kann mit der Waffe nicht zielen
    if(pLauncher && Contents(0, obj) != pLauncher)
      return;

    iDAngle = obj->AimAngle();
    iMaxTurn = MaxTurn();
  }
  var iAngle = GetR();

  var iDiff = Normalize(iDAngle - iAngle,-180);
  var iTurn = Min(Abs(iDiff),iMaxTurn);

  SetR(iAngle+iTurn*((iDiff > 0)*2-1));
}
