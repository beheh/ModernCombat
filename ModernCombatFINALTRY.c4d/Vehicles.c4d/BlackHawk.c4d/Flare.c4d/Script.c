/*-- Täuschkörper --*/

#strict 2

public func IsBulletTarget()	{return true;}
public func IgnoreTracer()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  //Licht erstellen
  AddLight(80,RGBa(230,230,255,60));
}

/* Timer */

protected func TimerCall()
{
  //Bei Bodenkontakt verschwinden
  if(GetContact(this, -1, CNAT_Bottom))
    return Hit();

  //Langsamer runterfallen
  SetYDir(GetYDir()-GetGravity()/70);

  //Effekte
  CreateParticle("Flare2",0,0,GetXDir()/3,GetYDir()/3,RandomX(30,40)*5,RGB(255,255,255));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CreateParticle("GroundSmoke",0,0,0,0,RandomX(20,30)*5,RGB(180,180,180));
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return 80;	//Feuer
  if(iType == DMG_Projectile)	return 80;	//Kugeln
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}

protected func Damage()
{
  if(GetDamage() > 20)
  {
    //Punkte bei Belohnungssystem
    DoPlayerPoints(BonusPoints("Protection"), RWDS_TeamPoints, GetOwner(), GetCursor(GetOwner()), IC16);
    Hit();
  }
}

protected func Hit()
{
  RemoveObject();
}
