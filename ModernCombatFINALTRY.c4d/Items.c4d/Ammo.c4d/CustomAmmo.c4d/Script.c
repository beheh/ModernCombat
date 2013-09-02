/*-- Gepackte Munitionsbox --*/

#strict 2

local ammoid,count;

public func IsDrawable()	{return true;}		//Wird sichtbar getragen
public func HandSize()		{return 800;}
public func HandX()		{return 6000;}
public func HandY()		{return -1000;}

public func NoWeaponChoice()	{return true;}
public func IsAmmoPacket()	{return true;}		//Ist Munition
public func AmmoID()		{return ammoid;}	//ID der Munition
public func AmmoCount()		{return count;}		//Munitionmenge

public func LimitationCount()	{return 3;}


/* Initialisierung */

protected func Initialize()
{
  CheckLimitation();
}

/* Icon-Info für QuickInventory */

public func SpeedMenuIcon()
{
  if(ammoid == STAM)	return ABOX;
  if(ammoid == GRAM)	return GBOX;
  if(ammoid == MIAM)	return MBOX;
}

public func SetAmmoID(id idType)
{
  if(idType->~IsAmmo())
  {
    ammoid = idType;

    if(idType == STAM)
      SetGraphics(0,0, ABOX);
    else
    if(idType == GRAM)
      SetGraphics(0,0, GBOX);
    else
    if(idType == MIAM)
      SetGraphics(0,0, MBOX);
    else
    {
      SetGraphics(0,0,idType,2,GFXOV_MODE_Picture);
      SetObjDrawTransform(500, 0, 0, 0, 500, 0, this, 2);
    }
  }
  return false;
}

public func GetMax()
{
  var max = ammoid->~MaxAmmo()/10;
  if(!max) max = 50;
  return max;
}

public func SetAmmoCount(int iCount, bool fForceCount)
{
  if(!iCount)
  {
    RemoveObject();
    return;
  }
  if (!fForceCount) count = BoundBy(iCount,0,GetMax());
  //z.B. MTP
  else count = iCount;
  return count;
}

public func DoAmmoCount(int iCount)
{
  var new = BoundBy(iCount,0,GetMax());
  var dif = new-AmmoCount();
  count   = new;
  return dif;
}

private func OnTransfer()	{}

protected func Activate(object pObj)
{
  return TransferAmmo(pObj);
}

public func MayTransfer(object pObj)
{
  if(!pObj) return false;
  var MaxAmmo = AmmoID()->~MaxAmmo();
  if(MaxAmmo)
    if(GetAmmo(AmmoID(),pObj) + AmmoCount() > MaxAmmo)
      return false;
  return true;
}

public func TransferAmmo(object pObj)
{
  if(!pObj) return false;
  if(NoAmmo()) return false;

  //Nicht wenn das Ziel schon zu viel hat
  if(!MayTransfer(pObj))
  {
    PlayerMessage(GetOwner(pObj),"$NotMoreAmmo$",pObj,AmmoID());
    return;
  }

  //Nachricht ausgeben
  PlayerMessage(GetOwner(pObj),"$Collected$",pObj,AmmoCount(), AmmoID());

  //Packer erhält Munitionspunkte, wenn er im gleichen Team ist
  if (GetPlayerName(GetOwner()) && GetOwner() != GetOwner(pObj) && !Hostile(GetOwner(), GetOwner(pObj)))
  {
    var factor = AmmoID()->~GetPointFactor();
    if(!factor)
      factor = 2;

    //Punkte bei Belohnungssystem (Munitionierung)
    DoPlayerPoints(BonusPoints("Restocking", count*factor), RWDS_TeamPoints, GetOwner(), GetCursor(GetOwner()), IC14);
    //Achievement-Fortschritt (Ammo Distributor)
    DoAchievementProgress(AmmoID()->MaxAmmo()/10*factor, AC03, GetOwner());
  }

  var old = count;
  SetAmmoCount(old-DoAmmo(AmmoID(),AmmoCount(),pObj));

  pObj->~AmmoTransferred();
  Sound("Resupply.ogg");
  if(!OnTransfer()) RemoveObject();

  return true;
}

public func ControlThrow(object caller)
{
  //Verbündeten suchen
  for (var obj in FindObjects(Find_InRect(-10,-10,20,20),Find_OCF(OCF_CrewMember),Find_Exclude(caller),Find_Allied(GetOwner(caller)),Find_NoContainer()))
    //Kann noch Munition aufnehmen?
    if (obj->~IsClonk() && MayTransfer(obj))
    {
      //Munition geben und abbrechen
      TransferAmmo(obj);
      break;
    }
    else
    {
      PlayerMessage(GetOwner(caller), "$EnoughAmmo$",caller,AmmoID());
      return 1;
    }
  return true;
}

protected func RejectCollect(object pInto)
{
  return ContentsCount(GetID(), pInto);
}

/* Allgemein */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg");
  return 1;
}

protected func Selection()
{
  Sound("FAPK_Charge.ogg");
  return 1;
}