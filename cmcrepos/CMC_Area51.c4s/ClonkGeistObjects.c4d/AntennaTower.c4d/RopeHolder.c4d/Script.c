/*-- Seilhalterung --*/

#strict 2

local pAntenna, fDestroyed;

public func IsBulletTarget() { return !fDestroyed; }

public func IsDestroyed() { return fDestroyed; }

public func SetAntenna(object pObject) { return pAntenna = pObject; }

public func Damage()
{
	if(!IsDestroyed() && GetDamage() > 100)
	{
		SetCategory(C4D_Object);
		pAntenna->RopeHolderDestroyed(this);
		fDestroyed = true;

		var angle = Angle(GetX(), GetY(), GetX(pAntenna), GetY(pAntenna));
		SetXDir(+Sin(angle, 100)/2);
		SetYDir(-Cos(angle, 100)/2);
	}

	return true;
}
