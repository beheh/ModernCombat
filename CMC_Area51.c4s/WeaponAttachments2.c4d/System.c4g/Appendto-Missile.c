/*-- Rakete --*/

#strict 2
#appendto MISL

local sx, sy, pLauncher, pLight, iLastAttacker, fGuided, fLaserGuided;

public func MaxTime()		{return 200;}			//Maximale Flugzeit

public func StartSpeed()	{return 5;}			//Startgeschwindigkeit
public func Acceleration()	{return 3;}			//Beschleunigung
public func MaxSpeed()		{return 100+!fGuided*50;}	//Maximale Geschwindigkeit

public func SecureTime()	{return 25;}			//Mindestflugzeit
public func MaxDamage()		{return 5;}			//Maximalschaden bis Absturz

public func ExplosionDamage()	{return 25;}			//Explosionsschaden
public func ExplosionRadius()	{return 25;}			//Radius

public func TracerCompatible()	{return true;}			//Peilsendersuchende Rakete
public func TracerRadius()	{return 300;}

public func MaxTurn()		{return 6;}			//max. Drehung
public func MaxTracerTurn()	{return 8;}			//max. Drehung bei Zielverfolgung

public func IgnoreTracer()	{return true;}
public func IsDamaged()		{return GetEffect("Damaged", this);}
public func IsRocket()		{return true;}			//Ist eine Rakete

public func AllowHitboxCheck() {return true;}


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

public func Launch(int iAngle, object pFollow, bool fUnguided, bool laserguided)
{
  //Geschwindigkeit setzen
  iSpeed = StartSpeed();

  //Verfolgung setzen
  fGuided = !fUnguided;
  fLaserGuided = laserguided;

  //Winkel setzen
  SetR(iAngle);
  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetAction("Travel");

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
  if(!fLaserGuided)
    EffectVar(1,pTarget,iEffectNumber) = 0;
  else
    EffectVar(1,pTarget,iEffectNumber) = obj->~GetLaser();
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

  //Lasergesteuert?
  if(fLaserGuided)
    EffectVar(1,pTarget,iEffectNumber) = pLauncher->~GetLaser();

  //Rakete unterst�tzt Peilsender?
  if(pTarget->~TracerCompatible() && Guideable() && !fLaserGuided)
  {
    var x = GetX(pTarget)-GetX(), y = GetY(pTarget)-GetY();
    //G�ltigkeit des Ziels pr�fen
    if(EffectVar(1,pTarget,iEffectNumber))
    {
      var pEnemy = EffectVar(1,pTarget,iEffectNumber);
      var del;
      if(!GetEffect("TracerDart", pEnemy)) del = true;
      if(!PathFree(GetX(pTarget), GetY(pTarget), GetX(pEnemy), GetY(pEnemy))) del = true;
      if(del) EffectVar(1,pTarget,iEffectNumber) = 0;
    }
    //Haben wir noch ein markiertes Ziel?
    if(!EffectVar(1,pTarget,iEffectNumber))
      for(var pEnemy in FindObjects(Find_Distance(TracerRadius(), x, y), Sort_Distance(x, y)))
      {
        var iEffectTracer = GetEffect("TracerDart", pEnemy);
        if(!iEffectTracer) continue;
        var iTeam = EffectVar(2, pEnemy, iEffectTracer);
        if(iTeam != GetPlayerTeam(GetController())) continue;
        if(!PathFree(GetX(), GetY(), GetX(pEnemy), GetY(pEnemy))) continue;
        EffectVar(1, pTarget, iEffectNumber) = pEnemy;
        Sound("BBTP_Alarm.ogg", 0, pTarget);
        break;
      }
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
  else
  {
    //Kann nicht gesteuert werden
    if(!Guideable())
      return;
    var obj = EffectVar(0,pTarget,iEffectNumber);
    if(!obj)
      return;
    //Sch�tze nicht mehr am Zielen?
    if(!obj->~IsAiming())
      return;
    //Sch�tze kann mit der Waffe nicht zielen
    if(pLauncher && Contents(0, obj) != pLauncher)
      return;

    iDAngle = obj->AimAngle();
    iMaxTurn = MaxTurn();
  }
  var iAngle = GetR();

  var iDiff = Normalize(iDAngle - iAngle,-180);
  var iTurn = Min(Abs(iDiff),iMaxTurn);

  if(!fLaserGuided || (pEnemy && pEnemy->~Active()))
    SetR(iAngle+iTurn*((iDiff > 0)*2-1));
  else
    SetR(GetR()+ RandomX(-6,6));
}

private func Traveling()
{
  //Nichts unternehmen wenn zerst�rt
  if(GetAction() == "Idle")
    return;

  //Bei zu hoher Flugh�he au�erhalb der Landschaft explodieren
  if(GetY() < -500)
    return Hit();

  //Nichts weiter unternehmen wenn deaktiviert
  if(GetAction() == "Fall")
    return;

  //Im Wasser abst�rzen
  if(GBackLiquid())
  {
    Sound("MISL_ShotDown.ogg");
    return Fall();
  }

  //Fallen wenn Maximalflugzeit erreicht
  if(GetActTime() >= MaxTime()) return Fall();

  //Geschwindigkeit erh�hen
  Accelerate();

  //Rauchspur
  if(!GBackLiquid()) Smoking();
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
  //Zerst�ren wenn bereits deaktiviert
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
}

public func Hit()
{
  if(GetAction() == "Idle")
  {
    Explode(ExplosionDamage()/8); if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
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
        Fling(pObj,GetXDir()/15,GetYDir()/15-1);
      }
      else
      {
        Sound("MetalHit*.ogg");
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
  DamageObjects(ExplosionRadius(), ExplosionDamage()/2, this);
  Explode(ExplosionDamage()*3/2);
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
  if(iType == DMG_Bio)		return 100;	//S�ure und biologische Schadstoffe
}
