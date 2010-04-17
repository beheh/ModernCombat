/*-- Ziele --*/

#strict 2
#appendto CGLO
#appendto MVNT
#appendto LBPC
#appendto CCP1

public func Damage()
{
	var maxdamage = EffectVar(0, this, GetEffect("AS_Target",this));
	if(maxdamage)
		if(GetDamage() >= maxdamage) Explode(30);
}

public func OnDmg(int iDamage, int iType)
{
    if(iType == DMG_Projectile) return 65;
    if(iType == DMG_Explosion) return 0;
}

public func IsBulletTarget() { return GetEffect("AS_Target",this); }
public func IgnoreFriendlyFire() { return true; }
