/*-- Versorgungskiste --*/

#strict 2

local idSpawn, iMaxCount, iRespawnTime, iTakeTime;

public func IsSpawnpoint()	{return true;}
public func NoPackMerge()	{return true;}


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
  //Standardfarbe: Grün
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

    //Weiß
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

    //Grün
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

  //Externe Informationen übernehmen
  if(iRespawn) iRespawnTime = iRespawn;
  if(iTake) iTakeTime = iTake;
  if(iMax) iMaxCount = iMax;

  idSpawn = idType;

  RemoveEffect("IntRespawn", this);

  //Spawneffekt erstellen
  if(iRespawnTime && idSpawn)
    AddEffect("IntRespawn", this, 10, iRespawnTime, this);

  //Icon übernehmen  
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
  if(!fGrab)
    RemoveEffect("IntResupply", pClonk);
  if(!CheckGrab())
    return;
  if(idSpawn && pClonk && fGrab && GetAction() == "Open")
    CheckResupply(pClonk,false);
}

protected func GrabLost(object pClonk)
{
  Grabbed(pClonk, false);
}

private func CheckGrab()
{
  //Öffnet sich bzw. offen, wenn (mindestens) ein Clonk es anfasst
  if(FindObject2(Find_OCF(OCF_Alive), Find_Action("Push"), Find_ActionTarget(this)))
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
  if(!CheckGrab() || !idSpawn)
    return;

  //Alle anfassenden Spieler mit Effekt belegen sofern noch nicht vorhanden
  for(var pClonk in FindObjects(Find_OCF(OCF_Alive), Find_Action("Push"), Find_ActionTarget(this)))
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
  //Füllen
  CreateContents(idSpawn, 0, iMaxCount);
}

public func FxIntRespawnTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(ContentsCount() < iMaxCount)
    CreateContents(idSpawn);
}

public func FxIntRespawnInfo(object pTarget, int iEffectNumber)
{
  return Format("$Supplies$", GetName(0, idSpawn));
}

public func FxIntRespawnStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  //Leeren
  if(!fTemp)
    while(Contents())
      RemoveObject(Contents());
}

/* Aufnehmeffekt */

public func FxIntResupplyStart(object pTarget, int iEffectNumber, int iTemp, pCrate, bStartTake)
{
  if(!pCrate)
    return -1;
  EffectVar(0, pTarget, iEffectNumber) = pCrate; 

  EffectCall(pTarget, iEffectNumber, "UpdateMessages");

  if(bStartTake)
  {
    if(EffectCall(pTarget, iEffectNumber, "Timer") == -1)
    {
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
    return;

  //Platz für Munition?
  if(obj->~IsAmmoPacket())
    if(!obj->~MayTransfer(pTarget))
      return;

  //Platz für Handgranaten?
  if(obj->~IsGrenade())
  {
    if(!pTarget->~StoreGrenade(obj))
      return;
  }
  else
  {
    var fAmmo = obj->~IsAmmoPacket();

    //Munition direkt in den Munitionsgürtel
    if(obj && fAmmo)
      obj->~TransferAmmo(pTarget);
    else if(!Collect(obj, pTarget))
      return;

    //Waffen direkt laden
    if(obj && obj->~IsWeapon())
      DoAmmo(obj->GetFMData(FM_AmmoID), obj->GetFMData(FM_AmmoLoad), obj);
  }
  
  Sound("AMCT_Take.ogg", false, pTarget); 

  //Benachrichtigungen aktualisieren
  EffectCall(pTarget, iEffectNumber, "UpdateMessages");
}

public func FxIntResupplyUpdateMessages(object pTarget, int iEffectNumber)
{
  var pCrate = EffectVar(0, pTarget, iEffectNumber);
  var szMessage = Format("@{{%i}}|%d/%d", pCrate->GetSpawnID(), ContentsCount(pCrate->GetSpawnID(), pCrate), pCrate->GetMaxCount());
  //Alle anfassenden Clonks aktualisieren
  for(var pClonk in FindObjects(Find_OCF(OCF_CrewMember), Find_Action("Push"), Find_ActionTarget(pCrate)))
  {
    PlayerMessage(GetController(pClonk), szMessage, pClonk);
  }
}

public func FxIntResupplyStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  //Nachricht entfernen
  PlayerMessage(GetController(pTarget), "", pTarget);
}

/* Aufschlag */ 
  
protected func Hit()
{
  Sound("CrateImpact*.ogg");
  return 1;
}