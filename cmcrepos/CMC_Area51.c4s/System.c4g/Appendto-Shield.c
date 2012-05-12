/*-- Schild-Appendto --*/

#strict 2
#appendto RSLH

public func IsBulletTarget(id idBullet, object pBullet, object pShooter, int oldx, int oldy)
{
	if(GetOwner() == NO_OWNER) 
		return false;
	else
		return _inherited(idBullet, pBullet, pShooter, oldx, oldy);
}
