/*-- Säule --*/

#strict 2
#include CHBX

local iStatus,iName,iDelay;

public func IsBulletTarget(id idBullet, object pBullet, object pShooter)
{
  if(iStatus == 3) return false;
  if(pBullet && (pBullet->~AllowHitboxCheck()))
    if(!IsInHitbox(AbsX(GetX(pBullet)), AbsY(GetY(pBullet))))
      return false;

  return true;
}


/* Hitbox */

public func HitboxXOffset()	{return 0;}	//X-Abstand vom Offset zum Hitboxmittelpunkt
public func HitboxYOffset()	{return -27;}	//Y-Abstand vom Offset zum Hitboxmittelpunkt
public func HitboxWidth()	{return 24;}	//Breite der Hitbox
public func HitboxHeight()	{return 12;}	//Höhe der Hitbox
public func HitboxRotation()	{return 1;}
public func UseOwnHitbox()	{return (iStatus != 3);}


/* Initialisierung */

protected func Initialize()
{
  //Hitbox erstellen
  InitializeHitbox();

  SetAction("Standing");
}

/* Einstellung */

public func Set(string szName, int iTime)
{
  //Aufruf festlegen
  if(szName)
    iName = szName;
  if(iTime)
    iDelay = iTime;
}

/* Zerstörung */

public func Damage(int iChange, int iPlr)
{
  if(GetAction() == "Destroyed") return;

  if(GetDamage() > 100)
  {
    //Zusammensturz melden
    if(iName && iDelay)
      Schedule("GameCallEx(iName)", iDelay);
    else
      GameCallEx(iName, iPlr);

    //Rauch erzeugen
    CastObjects(SM4K,2,20);

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",8,110,-20,0,60,100);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",8,110,20,0,60,100);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",4,100,0,0,40,15,RGB(40,20,20));
    CastSmoke("Smoke3",10,20,0,-20,220,500);
    CastSmoke("Smoke3",10,20,0,20,220,500);
    Sound("StructureIntegrity*.ogg");
    Sound("StructureHit*.ogg");
    Sound("StructureDebris*.ogg");

    //Aussehen verändern
    SetAction("Destroyed");
    SetPhase(RandomX(0,2));

    iStatus = 3;
    return 1;
  }
  else
  if(GetDamage() > 65 && iStatus < 2)
  {
    //Aussehen verändern
    SetPhase(2);

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",8,110,0,0,60,100);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,100,0,0,40,15,RGB(40,20,20));
    CastSmoke("Smoke3",15,20,0,0,220,500);
    Sound("StructureHit*.ogg");
    Sound("StructureDebris*.ogg");

    iStatus = 2;
    return 1;
  }
  else
  if(GetDamage() > 30 && iStatus < 1)
  {
    //Aussehen verändern
    SetPhase(1);

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",8,110,0,0,60,100);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,100,0,0,40,15,RGB(40,20,20));
    CastSmoke("Smoke3",15,20,0,0,220,500);
    Sound("StructureHit*.ogg");
    Sound("StructureDebris*.ogg");

    iStatus = 1;
    return 1;
  }
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Energy)	return 0;	//Energie
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
  if(iType == DMG_Melee)	return 80;	//Nahkampf
  if(iType == DMG_Fire)		return 80;	//Feuer
  if(iType == DMG_Explosion)	return 0;	//Explosionen
  if(iType == DMG_Projectile)	return 80;	//Projektile

  return 50;
}