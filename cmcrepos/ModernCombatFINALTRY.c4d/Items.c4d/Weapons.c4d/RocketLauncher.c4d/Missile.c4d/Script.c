/*-- Rakete --*/

#strict 2
#include MISS

local sx, sy, iStartY, iStartX, pLauncher, pLight, iLastAttacker, fGuided, fTracerChasing;

public func MaxTime()		{return 200;}			//Maximale Flugzeit

public func StartSpeed()	{return 5;}			//Startgeschwindigkeit
public func Acceleration()	{return 3+!fGuided*2;}		//Beschleunigung
public func MaxSpeed()		{return 100+!fGuided*50;}	//Maximale Geschwindigkeit

public func SecureTime()	{return 25;}			//Mindestflugzeit
public func MaxDamage()		{return 5;}			//Maximalschaden bis Absturz

public func ExplosionDamage()	{return 25;}			//Explosionsschaden
public func ExplosionRadius()	{return 25;}			//Radius

public func TracerCompatible()	{return true;}			//Peilsendersuchende Rakete
public func TracerRadius()	{return 350;}

public func MaxTurn()		{return 6;}			//max. Drehung
public func MaxTracerTurn()	{return 8;}			//max. Drehung bei Zielverfolgung

public func IgnoreTracer()	{return true;}
public func IsDamaged()		{return GetEffect("Damaged", this);}
public func IsRocket()		{return true;}			//Ist eine Rakete

public func AllowHitboxCheck()	{return true;}
public func RejectC4Attach()	{return true;}


/* Initialisierung */

public func Initialize()
{
  iLastAttacker = NO_OWNER;
  fGuided = true;
}

/* Start */

protected func Construction(object pBy)
{
  if(pBy && pBy->~IsWeapon())
    pLauncher = pBy;
}

public func Launch(int iAngle, object pFollow, bool fUnguided, int iXDir, int iYDir)
{
  //Geschwindigkeit setzen
  iSpeed = StartSpeed();

  //Verfolgung setzen
  fGuided = !fUnguided;

  //Winkel setzen
  SetR(iAngle);
  SetXDir(+Sin(iAngle,iSpeed)+iXDir);
  SetYDir(-Cos(iAngle,iSpeed)+iYDir);
  SetAction("Travel");

  iStartX = iXDir;
  iStartY = iYDir;

  //Sicht setzen
  if(Guideable()) SetPlrViewRange(150);
  SetCategory(C4D_Vehicle);

  sx = GetX();
  sy = GetY();

  //Effekte
  AddEffect("ThrustSound",this,1,11,this);
  pLight = AddLight(70,RGB(255,200,200),this,GLOW);

  //Treffer- und Steuereffekt einsetzen
  AddEffect("HitCheck", this, 1,1, 0, SHT1,shooter);
  AddEffect("Follow", this, 1,1, this, 0, pFollow);

  //Gesichert losfliegen
  if(SecureTime())
    AddEffect("IntSecureTime", this, 1, SecureTime(), this);
}

public func Guideable()
{
  return fGuided;
}

public func Secure()
{
  return GetEffect("IntSecureTime", this);
}

public func StartChasing()
{
  if(GetEffect("Follow", this) && EffectVar(1, this, GetEffect("Follow", this)) && !fTracerChasing)
  {
    Sound("BBTP_Alarm.ogg", 0, this);
    Sound("AT4R_Empty.ogg", 0, pLauncher, 50);
    fTracerChasing = true;
  }
  else
  {
    Sound("MISL_Switch.ogg", 0, this, 0, GetOwner());
    Sound("AT4R_Empty.ogg", 0, pLauncher, 50);
  }
}

/* Soundeffekt */

public func FxThrustSoundTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  Sound("MISL_Thrust.ogg",0,0,0,0,+1);
  return -1;
}

/* Optischer Steuerungseffekt */

public func FxFollowStart(object pTarget, int iEffectNumber, int iTemp, obj)
{
  if(iTemp)
    return;
  EffectVar(0,pTarget,iEffectNumber) = obj;
  EffectVar(1,pTarget,iEffectNumber) = 0;
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

      Sound("MISL_Click.ogg", 0, this, 50, GetOwner());

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

public func FxFollowStop(pTarget, iEffectNumber)
{
  var pBeam = EffectVar(2, pTarget, iEffectNumber);
  if(pBeam)
    RemoveObject(pBeam);
}

private func Traveling()
{
  //Nichts unternehmen wenn zerstört
  if(GetAction() == "Idle")
    return;

  //Bei zu hoher Flughöhe außerhalb der Landschaft explodieren
  if(GetY() < -500)
    return Hit();

  //Nichts weiter unternehmen wenn deaktiviert
  if(GetAction() == "Fall")
    return;

  //Im Wasser abstürzen
  if(GBackLiquid())
  {
    Sound("MISL_ShotDown.ogg");
    return Fall();
  }

  //Fallen wenn Maximalflugzeit erreicht
  if(GetActTime() >= MaxTime()) return Fall();

  //Geschwindigkeit erhöhen
  Accelerate();

  //Rauchspur
  if(!GBackLiquid()) Smoking();
}

private func Accelerate()
{
  if(iSpeed < MaxSpeed())
  {
    iSpeed += Acceleration();

    var iXDiff = Abs(Sin(GetR(),MaxSpeed())) - Abs(Sin(GetR(),iSpeed));
    var iYDiff = Abs(-Cos(GetR(),MaxSpeed())) - Abs(-Cos(GetR(),iSpeed));

    var iTemp = iStartX - iXDiff;
    if(iTemp > 0)
    {
      iTemp = iStartX/iXDiff;
      iStartX /= iTemp;
      iStartY /= iTemp;
    }

    iTemp = iStartY - iXDiff;
    if(iTemp > 0)
    {
      iTemp = iStartY/iXDiff;
      iStartX /= iTemp;
      iStartY /= iTemp;
    }
  }
  else
  {
    iStartX = 0;
    iStartY = 0;
  }
/*
  var fSX = Sgn(iStartX);
  var fSY = Sgn(iStartY);

  iStartX -= fSX*Acceleration();
  iStartY -= fSY*Acceleration();

  if(fSX != Sgn(iStartX)) iStartX = 0;
  if(fSY != Sgn(iStartY)) iStartY = 0;
*/

  SetXDir(+Sin(GetR(),iSpeed)+iStartX);
  SetYDir(-Cos(GetR(),iSpeed)+iStartY);
}

private func StopThrust()
{
  Sound("MISL_Thrust.ogg",0,0,0,0,-1);
}

/* Rauch */

private func Smoking()
{
  var dist = Distance(0,0,GetXDir(),GetYDir());
  var maxx = +Sin(GetR(),dist/10);
  var maxy = -Cos(GetR(),dist/10);
  var ptrdist = 50;

  for(var i=0; i<dist; i+=ptrdist)
  {
    var x = -maxx*i/dist;
    var y = -maxy*i/dist;

    var rand = RandomX(-10,10);
    var xdir = +Sin(GetR()+rand,RandomX(8,20));
    var ydir = -Cos(GetR()+rand,RandomX(8,20));

    CreateParticle("Smoke3",x,y,xdir,ydir,RandomX(50,80),RGBa(255,255,255,85),0,0);
  }

  CreateParticle("MuzzleFlash2",-maxx,-maxy,+Sin(GetR()+180,500),-Cos(GetR()+180,500),RandomX(80,140),RGBa(255,200),this);
}

/* Schaden */

public func Damage()
{
  //Zerstören wenn bereits deaktiviert
  if(GetAction() == "Idle")
    return Hit();

  //Deaktivieren wenn Schaden erhalten und nicht detonierend
  if(GetDamage() > MaxDamage() && !exploding)
  {
    //Rakete deaktivieren
    Damaged();
  }
}

public func Damaged()
{
  //Schadenseffekt
  AddEffect("Damaged",this,1,1,this);

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",2,80,0,0,45,20,RGB(40,20,20));

  if(!Secure() && Hostile(iLastAttacker, GetController()))
  {
    //Punkte bei Belohnungssystem (Raketenabwehr)
    DoPlayerPoints(BonusPoints("Protection"), RWDS_TeamPoints, iLastAttacker, GetCursor(iLastAttacker), IC16);
  }

  Sound("MISL_ShotDown.ogg");
  SetAction("Idle");
}

public func FxDamagedTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  CreateParticle("Thrust",0,0,0,0,70+Random(30),RGBa(255,255,255,0),0,0);
  SetR(Angle(GetX(), GetY(), GetX()+GetXDir(), GetY()+GetYDir()));
}

public func Hit()
{
  if(GetAction() == "Idle")
  {
    Explode(ExplosionDamage()/8);
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  }
  else
  {
    HitObject();
  }
}

private func OnRocketHit(pObj)
{
  if(!exploding) HitObject(pObj);
}

private func HitObject(pObj)
{
  var pShield = LocalN("pShield", Contained(pLauncher));
  if(pShield && pShield == pObj && Secure()) return;

  if(Secure() || GetAction() == "Idle")
  {
    if(pObj)
    {
      if(pObj == EffectVar(0, this, GetEffect("Follow", this)))
        return;
      DoDmg(Distance(GetXDir(),GetYDir())/5,DMG_Projectile,pObj);
      if(GetEffectData(EFSM_ExplosionEffects) > 0)  CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
      if(GetOCF(pObj) & OCF_Living)
      {
        Sound("SharpnelImpact*.ogg");

        //Kollision berechnen
        Fling(pObj,Min(100, GetXDir())/15, Min(100, GetYDir())/15);
      }
      else
      {
        Sound("BulletHitMetal*.ogg");
        Sparks(30,RGB(255,128));
        if(GetEffectData(EFSM_ExplosionEffects) > 0)  CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
      }
      RemoveObject();
      return;
    }
    else
    {
     Sound("GrenadeHit*.ogg");
     Sparks(30,RGB(255,128));
     if(GetEffectData(EFSM_ExplosionEffects) > 0)  CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
     RemoveObject();
     return;
    }
   }

  if(GetAction() == "Fall")
  {
    if(pObj)
    {
      if(GetOCF(pObj) & OCF_Alive && Hostile(GetOwner(pObj), GetController()))
      {
        //Achievement-Fortschritt (Missile Artillery)
        DoAchievementProgress(1, AC20, GetController());
      }
    }
  }

  if(GetAction() == "Travel")
  {
    if(pObj)
    {
      if((GetID(pObj) == MISL || GetID(pObj) == LRML) && Hostile(GetOwner(pObj), GetController()))
      {
        //Achievement-Fortschritt (Protective Pro)
        DoAchievementProgress(1, AC24, GetController());
      }
    }
  }

  //Explodiert
  exploding = true;

  //Direkttreffer benachrichtigen
  if(pObj)
  {
    pObj->~OnRocketHit(this);
  }

  Sound("GrenadeExplosion*.ogg");

  //Explosion
  Detonate();
}

private func Fall()
{
  //Herunterfallen
  SetAction("Fall");
  var r = GetR()%180;
  if(r > 90) r = 180-r;
  if(GetR() > 180) r *= -1;
  SetRDir(r*3/90);
  return true;
}

private func Detonate()
{
  //Schaden verursachen
  Explode(ExplosionDamage()*3/2);
  DamageObjects(ExplosionRadius(), ExplosionDamage()/2, this);
}

public func Destruction()
{
  StopThrust();
}

/* Schaden */

public func OnHit(a, b, pFrom)
{
  iLastAttacker = GetController(pFrom);
  return;
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return 100;	//Feuer
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}