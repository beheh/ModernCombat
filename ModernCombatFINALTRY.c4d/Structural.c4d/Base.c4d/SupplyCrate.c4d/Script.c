/*-- Versorgungskiste --*/

#strict 2

local idSpawn, iMaxCount, iRespawnTime, iTakeTime;

public func IsSpawnpoint()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  //Anfangs geschlossen
  SetAction("Closed");

  //Zeit bis zum Objektrespawn
  iRespawnTime = 35 * 30;
  //Zeit zum Herausnehmen
  iTakeTime = 35 * 3;
  //Maximalmenge an Objekten
  iMaxCount = 3;
  //Kein ID
  idSpawn = NONE;
  //Standardfarbe: Gr�n
  SetColorDw(HSL(80, 250, 150, 180));
}

/* Konfiguration */

private func PreDef(id idType)
{
  //Standardwerte
  iRespawnTime = 35 * 30;
  iTakeTime = 35 * 2;
  iMaxCount = 4;

  if(!idType)
    return;

  //Waffen
  if(idType->~IsWeapon2() || idType->~IsWeapon())
  {
    if(idType == ATWN)
      iRespawnTime = 40 * 40;
    else
      iRespawnTime = 40 * 20;
    iTakeTime = 35 * 2;
    iMaxCount = 2;

    //Wei�
    SetColorDw(HSL(100, 250, 250, 0));
  }
  //Munitionspakete
  if(idType->~IsAmmoPacket())
  {
    iRespawnTime = 35 * 20;
    iTakeTime = 35 * 2;
    iMaxCount = 3;

    //Gelb
    SetColorDw(HSL(40, 250, 160, 250));
  }
  //Handgranaten
  else if(idType->~IsGrenade())
  {
    iRespawnTime = 35 * 30;
    iTakeTime = 35 * 2;
    iMaxCount = 3;

    //Gr�n
    SetColorDw(HSL(80, 250, 150, 180));
  }
  //EHPs oder Defibrillatoren
  else if(idType == FAPK || idType == CDBT || idType == DGNN)
  {
    iRespawnTime = 35 * 30;
    iTakeTime = 35 * 3;
    iMaxCount = 2;

    //Rot
    SetColorDw(HSL(250, 250, 160, 150));
  }
}

public func Set(id idType, int iMax, int iTake, int iRespawn)
{
  //Standardwerte anwenden
  PreDef(idType);

  //Externe Informationen �bernehmen
  if(iRespawn) iRespawnTime = iRespawn;
  if(iTake) iTakeTime = iTake;
  if(iMax) iMaxCount = iMax;

  idSpawn = idType;

  RemoveEffect("IntRespawn", this);

  //Spawneffekt erstellen
  if(iRespawnTime && idSpawn)
    AddEffect("IntRespawn", this, 10, iRespawnTime, this);

  //Icon �bernehmen  
  if(idSpawn->~IsAmmoPacket())
    SetIcon(idSpawn->AmmoID());
  else
    SetIcon(idSpawn);

  return this;
}

/* Icondarstellung */

public func SetIcon(id idIcon)
{
  if(idIcon)
  {
    SetGraphics(0, 0, idIcon, 1, GFXOV_MODE_IngamePicture);

    var w = 8 * 1000 / GetObjHeight(); 
    var h = 8 * 1000 / GetObjHeight();

    SetObjDrawTransform(w, 0, 4000, 0, h, 0, this, 1);
  }
  else
    SetGraphics(0, 0, 0, 1);
}

/* �ffnen und Schlie�en */

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
  if(!CheckGrab())
    return;
  if(idSpawn && pClonk && fGrab && GetAction() == "Open")
    CheckResupply(pClonk,false);
}

protected func GrabLost(object pClonk)
{
  CheckGrab();
}

private func CheckGrab()
{
  //�ffnet sich bzw. offen, wenn (mindestens) ein Clonk es anfasst
  if(FindObject2(Find_OCF(OCF_CrewMember), Find_Action("Push"), Find_ActionTarget(this)))
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

  //Alle anfassenden Spieler mit Effekt belegen sofern noch nicht vorhanden
  for (var pClonk in FindObjects(Find_OCF(OCF_CrewMember), Find_Action("Push"), Find_ActionTarget(this)))
    CheckResupply(pClonk, true);
}

protected func CheckResupply(object pClonk, bool bStart)
{
  if(GetEffect("IntResupply", pClonk))
  	RemoveEffect("IntResupply", pClonk);
  
  AddEffect("IntResupply", pClonk, 10, iTakeTime, 0, GetID(), this, bStart);
}

protected func Closing() 
{
  Sound("AMCT_Close.ogg", false, this);
}

/* Aufnahme */

public func RejectCollect(id idObj)
{
  if(GetAction() != "Open")
    return true;

  if(idSpawn)
    return idSpawn != idObj || ContentsCount() >= iMaxCount;
  else
    return ContentsCount() >= 10;
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
  //F�llen
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
    if(EffectCall(pTarget, iEffectNumber, "Timer") == -1)
    {
      EffectCall(pTarget, iEffectNumber, "Stop");
      return -1;
    }
  }
  else
  {
    var obj = Contents(0, pCrate);

    PlayerMessage(GetController(pTarget), "@{{%i}}|%d/%d", pTarget, pCrate->GetSpawnID(), ContentsCount(pCrate->GetSpawnID(), pCrate), pCrate->GetMaxCount());

    if(!obj)
    {
      EffectCall(pTarget, iEffectNumber, "Stop");
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

  //Platz f�r Munition?
  if(obj->~IsAmmoPacket())
    if(!obj->~MayTransfer(pTarget))
      return -1;

  //Platz f�r Handgranaten?
  if(obj->~IsGrenade())
  {
    if(!pTarget->~StoreGrenade(obj))
      return -1;
  }
  else
  {
    if(!Collect(obj, pTarget))
      return -1;

    //Waffen direkt laden
    if(obj->~IsWeapon())
      DoAmmo(obj->GetFMData(FM_AmmoID), obj->GetFMData(FM_AmmoLoad), obj);

    //Munition direkt in den Munitionsg�rtel
    if(obj->~IsAmmoPacket())
      obj->~TransferAmmo(pTarget);
  }

  Sound("AMCT_Take.ogg", false, pTarget); 

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

/* Aufschlag */ 
  
protected func Hit()
{
  Sound("CrateImpact*.ogg");
  return 1;
}
