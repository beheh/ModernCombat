/*-- Waffenaufsatz --*/

#strict 2

local iAttachment;

public func IsDrawable()	{return true;}	//Wird sichtbar getragen
public func HandSize()		{return 800;}
public func HandX()		{return 6000;}
public func HandY()		{return -1000;}
public func IsAttachmentPack()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  //Keine Waffenaufsätze-Regel vorhanden? Verschwinden
  if(NoAttachments())
    return RemoveObject();

  return true;
}

/* Steuerung */

protected func Activate(object pClonk)
{
  if(NoAttachments())
    return;

  AttachmentMenu(pClonk);
}

/* Inhalt */

protected func SetAttachment(int iValue)
{
  iAttachment = iValue;
  SetGraphics(0,0,AttachmentIcon(iAttachment),2,GFXOV_MODE_Picture);    
  SetObjDrawTransform(700,0,0,0,700,0, 0, 2);
}

protected func GetAttachment()
{
  return iAttachment;
}

/* Menü */

protected func AttachmentMenu(object pByObj)
{
  //Kein Inhalt: Verschwinden
  if(!iAttachment)
    return RemoveObject();

  //Falsche Aktion?
  if(!WildcardMatch(GetAction(pByObj), "*Walk*") && !WildcardMatch(GetAction(pByObj), "*Swim*") && !WildcardMatch(GetAction(pByObj), "*Crawl*") && !WildcardMatch(GetAction(pByObj), "*Jump*"))
  {
    PlayerMessage(GetOwner(pByObj), "$CantUpgrade$", pByObj);
    return;
  }

  //Clonk anhalten
  SetComDir(COMD_Stop, pByObj);

  //Menü öffnen
  CreateMenu(GetID(this), pByObj, this, 0, Format("$Upgrade$", GetName(0, AttachmentIcon(iAttachment))), 0, C4MN_Style_Context);

  var fItemExists = false;

  //Kompatible Waffen auflisten
  for(var pWeapon in FindObjects(Find_Container(Contained()), Find_Func("IsWeapon2")))
  {
    if(pWeapon->~PermittedAtts() & iAttachment && LocalN("iAttachment", pWeapon) != iAttachment)
    {
      fItemExists = true;
      AddMenuItem(GetName(0, GetID(pWeapon)), "Attach", GetID(pWeapon), pByObj, 0, pByObj);
    }
  }

  //Keine Waffe gefunden: Menü schließen
  if(!fItemExists)
  {
    PlayerMessage(GetOwner(pByObj), "$NothingUpgradeable$", pByObj);
    CloseMenu(pByObj);
  }
}

/* Aufrüsten */

protected func Attach(id iItem, object pUser)
{
  //Aufzurüstende Waffe noch vorhanden?
  var pWeapon = FindContents(iItem, pUser);
  if(!pWeapon) return false;

  //Ausgebauter Waffenaufsatz selbst wieder anbringen
  SetAttachment(pWeapon->SetAttachment(iAttachment));

  //Sound
  Sound("WNAT_AddAttachement.ogg",0,pWeapon);

  //Aufgebraucht: Verschwinden
  if(!iAttachment)
    RemoveObject(this);
}

/* Icons */

global func AttachmentIcon(int iAtt)
{
  if(iAtt == AT_NoAttachment)		return 0;
  if(iAtt == AT_ExtendedMag)		return AI01;
  if(iAtt == AT_Bayonet)		return AI02;
  if(iAtt == AT_Laserpointer)		return AI03;
  if(iAtt == AT_Silencer)		return AI04;
  if(iAtt == AT_Flashlight)		return AI05;
}

/* Sonstiges */

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

/* Übergabe an andere */

func ControlThrow(object pByObj)
{
  for(var obj in FindObjects(Find_InRect(-10,-10,20,20),Find_OCF(OCF_CrewMember),Find_Exclude(pByObj),Find_Allied(GetOwner(pByObj)),Find_NoContainer()))
    if(Collect(this, obj)) break;

  return true;
}