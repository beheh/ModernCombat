/*-- Munitionskiste --*/

#strict 2

local idSpawn, iMaxCount, iRespawnTime, iTakeTime;

public func IsSpawnpoint()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  SetAction("Closed");
  iRespawnTime = 35 * 30;
  iTakeTime = 35 * 3;
  iMaxCount = 3;
  idSpawn = NONE;
}

/* Respawn bestimmen */

private func PreDef(id idType)
{
  //Default
  iRespawnTime = 35 * 30;
  iTakeTime = 35 * 2;
  iMaxCount = 4;

  if(!idType)
    return;

  if(idType->~IsRifleLaunchedGrenade())
  {
    iRespawnTime = 35 * 20;
    iTakeTime = 35 * 1;
    iMaxCount = 4;
  }
  else if(idType->~IsAmmoPacket())
  {
    iRespawnTime = 35 * 20;
    iTakeTime = 35 * 2;
    iMaxCount = 3;
  }
  else if(idType->~IsGrenade())
  {
    iRespawnTime = 35 * 30;
    iTakeTime = 35 * 2;
    iMaxCount = 3;
  }
  else if(idType == FAPK || idType == CDBT)
  {
    iRespawnTime = 35 * 30;
    iTakeTime = 35 * 3;
    iMaxCount = 2;
  }
}

public func Set(id idType, int iMax, int iTake, int iRespawn)
{
  PreDef(idType);

  if(iRespawn) iRespawnTime = iRespawn;
  if(iTake) iTakeTime = iTake;
  if(iMax) iMaxCount = iMax;

  idSpawn = idType;

  RemoveEffect("IntRespawn", this);

  if(iRespawnTime && idSpawn)
    AddEffect("IntRespawn", this, 10, iRespawnTime, this);

  if(idSpawn->~IsAmmoPacket())
    SetIcon(idSpawn->AmmoID());
  else
    SetIcon(idSpawn);

  return this;
}

public func SetIcon(id idIcon)
{
  if(idIcon)
  {
    SetGraphics(0, 0, idIcon, 1, GFXOV_MODE_IngamePicture);

    var w = 7 * 1000 / GetObjHeight(); 
    var h = 7 * 1000 / GetObjHeight();

    SetObjDrawTransform(w, 0, 4000, 0, h, 0, this, 1);
  }
  else
    SetGraphics(0, 0, 0, 1);
}

/* Öffnen und Schließen */

protected func Open()
{
  if (GetAction() == "Closed")
    return SetAction("Opening");
}

protected func Close()
{
  if (GetAction() == "Open")
    return SetAction("Closing");
}

/* Anfassen und Loslassen */

protected func Grabbed(object pClonk, bool fGrab)
{
  if (!CheckGrab())
    return;
  if (idSpawn && pClonk && fGrab && GetAction() == "Open")
    CheckResupply(pClonk,false);
}

protected func GrabLost(object pClonk)
{
  CheckGrab();
}

private func CheckGrab()
{
  // Die Truhe soll sich selbst öffnen, wenn (mindestens) ein Clonk sie anfasst
  if (FindObject2(Find_OCF(OCF_CrewMember), Find_Action("Push"), Find_ActionTarget(this)))
  {
    Open();
    return true;
  }
  Close();
}

protected func Opening() 
{
  Sound("AMCT_Open.ogg", false, this); 
}

protected func Opened() 
{
  if (!CheckGrab() || !idSpawn)
    return;

  //Allen Clonks die die Kiste anfassen und noch keinen Resupply-Effekt haben, einen geben.
  for (var pClonk in FindObjects(Find_OCF(OCF_CrewMember), Find_Action("Push"), Find_ActionTarget(this)))
    CheckResupply(pClonk, true);
}

protected func CheckResupply(object pClonk, bool bStart)
{
  if(!GetEffect("IntResupply", pClonk))
    AddEffect("IntResupply", pClonk, 10, iTakeTime, 0, GetID(), this, bStart);
}

protected func Closing() 
{
  Sound("AMCT_Close.ogg", false, this);
}

public func RejectCollect(id idObj)
{
  if(GetAction() != "Open")
    return true;

  if(idSpawn)
    return idSpawn != idObj || ContentsCount() >= iMaxCount;
  else
    return ContentsCount() >= 10;

  return false;
}

protected func ControlDig(object pClonk)
{
  if(RejectContents())
    return true;

  SetCommand(pClonk, "Get", this, 0, 0, 0, 1);
}

protected func RejectContents()
{
  return GetAction() != "Open" || idSpawn;
}

public func GetMaxCount()
{
  return iMaxCount;
}

public func GetSpawnID()
{
  return idSpawn;
}

/* Respawneffekt */

public func FxIntRespawnStart(object pTarget, int iEffectNumber, int iTemp)
{
  if(!idSpawn || iTemp)
    return -1;
  //Leeren
  while(Contents())
    RemoveObject(Contents());
  //Füllen
  CreateContents(idSpawn, 0, iMaxCount);
}

public func FxIntRespawnTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(ContentsCount() < iMaxCount)
    CreateContents(idSpawn);
}

public func FxIntRespawnStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  //Leeren
  if (!fTemp)
    while(Contents())
      RemoveObject(Contents());
}

/* Aufnehmeffekt */

public func FxIntResupplyStart(object pTarget, int iEffectNumber, int iTemp, pCrate, bStartTake)
{
  if(!pCrate)
    return -1;
  EffectVar(0, pTarget, iEffectNumber) = pCrate; 

  if(bStartTake)
  {
    if(EffectCall(pTarget, iEffectNumber, "Timer", pTarget, iEffectNumber) == -1)
    {
      EffectCall(pTarget, iEffectNumber, "Stop", pTarget, iEffectNumber);
      return -1;
    }
  }
  else
  {
    var obj = Contents(0, pCrate);

    PlayerMessage(GetController(pTarget), "@{{%i}}|%d/%d", pTarget, pCrate->GetSpawnID(), ContentsCount(pCrate->GetSpawnID(), pCrate), pCrate->GetMaxCount());

    if(!obj)
    {
      EffectCall(pTarget, iEffectNumber, "Stop", pTarget, iEffectNumber);
      return -1;
    }
  }
}

public func FxIntResupplyTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var crate = EffectVar(0, pTarget, iEffectNumber);
  if(!crate || GetProcedure(pTarget) != "PUSH" || GetActionTarget(0, pTarget) != crate)
    return -1;

  var obj = Contents(0, crate), objid = GetID(obj);
  if(!obj)
    return -1;

  if(obj->~IsAmmoPacket())
    if(!obj->~MayTransfer(pTarget))
      return -1;

  //Granate? In den Gürtel packen
  if(obj->~IsGrenade())
  {
    if(!pTarget->~StoreGrenade(obj))
      return -1;
  }
  else
  {
    if(!Collect(obj, pTarget))
      return -1;

    //Waffe? Laden
    if(obj->~IsWeapon())
      DoAmmo(obj->GetFMData(FM_AmmoID), obj->GetFMData(FM_AmmoLoad), obj);

    //Munition? Aufnehmen
    if(obj->~IsAmmoPacket())
      obj->~TransferAmmo(pTarget);
  }

  if(!Contents(0, crate))
  {
    EffectVar(1, pTarget, iEffectNumber) = true;
    PlayerMessage(GetController(pTarget), "{{%i}}|%d/%d", pTarget, objid, ContentsCount(objid, crate), crate->GetMaxCount());
    return -1;
  }
  else
    PlayerMessage(GetController(pTarget), "@{{%i}}|%d/%d", pTarget, objid, ContentsCount(objid,crate), crate->GetMaxCount());
}

public func FxIntResupplyStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(!EffectVar(1, pTarget, iEffectNumber))
    PlayerMessage(GetController(pTarget), "", pTarget);
}