/*-- Flare --*/

#strict 2

public func IsBulletTarget() { return true; }

protected func Timer()
{
  //Langsamer runterfliegen
  SetYDir(GetYDir()-GetGravity()/70);
  //Effekte
  CreateParticle("Flare",0,0,GetXDir()/3,GetYDir()/3,RandomX(15,25)*5,RGB(255,255,255));
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
    CreateParticle("Smoke2",0,0,0,0,RandomX(20,35)*5,RGB(200,200,200));
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		    return 80;	//Feuer
  if(iType == DMG_Projectile) return 80;  //Kugeln
  if(iType == DMG_Bio)		    return 100;	//Säure und biologische Schadstoffe
}

protected func Damage()
{
  if(GetDamage() > 20)
    Hit();
}

protected func Hit()
{
  RemoveObject();
}
