/*-- T�uschk�rper --*/

#strict 2

public func IgnoreTracer()	{return true;}
public func IsBulletTarget(id idBullet, object pBullet, object pShooter)
{
  if(Hostile(GetOwner(pShooter),GetOwner()))
    return true;
  else
    return false;
}


/* Initialisierung */

protected func Initialize()
{
  //Licht erstellen
  AddLight(80,RGBa(230,230,255,60));
}

/* Timer */

protected func TimerCall()
{
  //In Wasser verschwinden
  if(InLiquid()) return RemoveObject();

  //Bei Bodenkontakt verschwinden
  if(GetContact(this, -1, CNAT_Bottom))
    return Hit();

  //Langsamer runterfallen
  SetYDir(GetYDir()-GetGravity()/70);

	//Clonks und Objekte anz�nden
	for(var pTarget in FindObjects(Find_Func("IsBulletTarget", GetID(), this), Find_Not(Find_Or(Find_AnyContainer(), Find_Func("IsCMCVehicle"))), Find_Distance(25, AbsX(GetX()), AbsY(GetY())))) {
		if(!OnFire(pTarget)) {
			DoDmg(3, DMG_Fire, pTarget, 0, GetOwner()+1, FLRE);
		}
	}

  //Effekte
  CreateParticle("Flare2",0,0,GetXDir()/3,GetYDir()/3,RandomX(30,40)*5,RGB(255,255,255));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CreateParticle("GroundSmoke",0,0,0,0,RandomX(20,30)*5,RGB(180,180,180));
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return 80;	//Feuer
  if(iType == DMG_Projectile)	return 80;	//Kugeln
  if(iType == DMG_Bio)		return 100;	//S�ure und biologische Schadstoffe
}

public func Damage()
{
  if(GetDamage() > 20)
  {
    //Punkte bei Belohnungssystem (Projektilabwehr)
    DoPlayerPoints(BonusPoints("Protection"), RWDS_TeamPoints, GetOwner(), GetCursor(GetOwner()), IC16);
    Hit();
  }
}

protected func Hit()
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12, 10, 0, 0, 100, 100, RGBa(255,255,255,100), RGBa(255,255,255,130));
  Sparks(30,RGB(255,128));
  Sound("Crackle.ogg");

  RemoveObject();
}
