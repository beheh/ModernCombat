/*-- Munition --*/

#strict 2

public func IsAmmoPacket()	{return GetID() != MCAM;}	//Ist Munition
public func AmmoID()		{return STAM;}			//ID der Munition
public func AmmoCount()		{return 100;}			//Munitionmenge
public func NoArenaRemove()	{return true;}
public func IsDrawable()	{return true;}
public func HandSize()		{return 800;}
public func HandX()		{return 6000;}
public func HandY()		{return -1000;}


/* Initialisierung */

protected func Initialize()
{
  //Keine Munition Regel: Kein Existenzgrund
  if(FindObject(NOAM)) Schedule("RemoveObject()", 1);

  return 1;
}

/* Aktivierung */

protected func Activate(object pObj)
{
  return TransferAmmo(pObj);
}

private func OnTransfer()	{}

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

  //Transfer abbrechen wenn das Ziel genug besitzt
  if(!MayTransfer(pObj))
  {
    PlayerMessage(GetOwner(pObj),"$NotMoreAmmo$",pObj,AmmoID());
    return;
  }

  //Punkte, wenn jemandem anders gegeben
  var clonk = Contained(),
  factor = AmmoID()->~GetPointFactor();
  if (clonk && clonk->~IsClonk() && factor && GetOwner(clonk) != GetOwner(pObj))
  {
    DoPlayerPoints(BonusPoints("Restocking", AmmoCount()*factor), RWDS_TeamPoints, GetOwner(clonk), GetCursor(GetOwner(clonk)), IC14);
    DoAchievementProgress(AmmoID()->~MaxAmmo()/10*factor, AC03, GetOwner(clonk));
  }

  //Nachricht ausgeben
  HelpMessage(GetOwner(pObj), "$Collected$", pObj, AmmoCount(), AmmoID());
  DoAmmo(AmmoID(), AmmoCount(), pObj);
  pObj->~AmmoTransferred();
  Sound("Resupply.ogg");
  if(!OnTransfer()) RemoveObject();

  return true;
}

public func ControlThrow(object caller)
{
  //Verb�ndeten suchen
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

/* Aufschlag */

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