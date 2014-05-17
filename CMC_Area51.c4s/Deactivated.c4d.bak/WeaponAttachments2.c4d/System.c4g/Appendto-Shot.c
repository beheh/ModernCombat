/*-- Schuss --*/

#strict 2
#appendto SHTX

local iAttachment;

public func BulletStrike(object pObj)
{
  if(pObj)
    DoDmg(iDamage, DMG_Projectile, pObj, 0, 0, 0, iAttachment);

  return true;
}

public func CustomLaunch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iRemoveTime, int Attachment)
{
  fNoTrail = (iTrail == -1);
  iAttachment = Attachment;
  return Launch(iAngle,iSpeed,iDist,iSize,iTrail,iDmg,iRemoveTime);
}