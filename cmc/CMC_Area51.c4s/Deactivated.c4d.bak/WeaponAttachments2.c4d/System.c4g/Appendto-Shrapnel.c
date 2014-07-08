/*-- Splitter --*/

#strict 2
#appendto SHRP

local iAttachment;

public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int attachment)
{
  //Schaden des Splitters setzen
  if(!iDmg)
    iDamage = 3;
  else
    iDamage = iDmg;
    
  //Attachment setzen
  iAttachment = attachment;

  //Position des Splitters setzen
  SetPosition(GetX(),GetY()+GetDefWidth()/2);

  //Größe und Länge setzen
  if(!iSize)
    iSize = 8;
  if(!iTrail)
    iTrail = 300;

  //Geschwindigkeit setzen
  speed = iSpeed;

  iSize = Min(iSize+2,GetDefWidth());

  DoCon(100*iSize/GetDefWidth()-100);

  lx = GetX();
  ly = GetY();

  iTime = 10*iDist/iSpeed;

  if(!iTime)
    return RemoveObject();

  var self = this;
  SetAction("Travel");

  //Sicherheitscheck, ob der Splitter nicht sofort verschwindet
  if(!self) return ;

  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetR(+iAngle);

  //Trail erzeugen
  size = iSize;
  trail_len = iTrail;
  CreateTrail(iSize, iTrail);

  AddEffect("HitCheck", this, 1,1, 0, GetID(), shooter);
}

public func BulletStrike(object pObj)
{
  if(pObj)
  {
    if(pObj->~IsBullet()) return false;
    if(GetID(pObj) == TRAI) return false;
    if(GetEffect("IntShrapnelHit",pObj))
    {
      return false;
    }

    if(GetID(pObj) != RSLH)
      AddEffect("IntShrapnelHit",pObj,1,10,0,GetID());

    if(GetOCF(pObj) & OCF_Alive)
    {
      //Fling(pObj,GetXDir()/20 + GetXDir(pObj),GetYDir()/20 + GetYDir(pObj));
      ObjectSetAction(pObj, "Tumble");
    }

    if(GetID(pObj) == RSLH)
    {
      DoDmg(1,DMG_Projectile,pObj,0,0,wpnid, iAttachment);
    }
    else
    {
      DoDmg(iDamage,DMG_Projectile,pObj,0,0,wpnid, iAttachment);
    }

    return true;
  }
  return true;
}
