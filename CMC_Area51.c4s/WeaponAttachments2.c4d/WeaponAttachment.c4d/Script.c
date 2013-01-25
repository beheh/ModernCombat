/*-- Waffenaufsatz --*/

#strict 2

local iAttachment;


/* Inhalt festlegen */

func SetAttachment(int iValue)
{
  iAttachment = iValue;
  SetGraphics(0,0,AttachmentIcon(iAttachment),2,GFXOV_MODE_Picture);    
  SetObjDrawTransform(700,0,0,0,700,0, 0, 2);
}

func GetAttachment()
{
  return iAttachment;
}

/* Steuerung */

func Activate(object pClonk)
{
  AttachmentMenu(pClonk);
}

/* Menü */

func AttachmentMenu(object pByObj)
{
  //Falsche Aktion?
  if(!WildcardMatch(GetAction(pByObj), "*Walk*") && !WildcardMatch(GetAction(pByObj), "*Swim*") && !WildcardMatch(GetAction(pByObj), "*Crawl*") && !WildcardMatch(GetAction(pByObj), "*Jump*"))
  {
    PlayerMessage(GetOwner(pByObj), "$CantUpgrade$", pByObj);
    return true;
  }

  //Clonk anhalten
  SetComDir(COMD_Stop, pByObj);

  //Menü erstellen
  CreateMenu(GetID(this), pByObj, this, 0, "$AttachmentMenu$", 0, C4MN_Style_Context);

  var fItemExists = false;

  //Kompatible Waffen auflisten
  for(var pWeapon in FindObjects(Find_Container(Contained()), Find_Func("IsWeapon2")))
  {
    fItemExists = true;
    if(pWeapon->~PermittedAtts() & iAttachment)
      AddMenuItem(Format("$Upgrade$", GetName(0, GetID(pWeapon))), "Attach", GetID(pWeapon), pByObj, 0, pByObj);
  }

  //Menü schließen wenn keine Waffe gefunden wurde
  if(!fItemExists)
  {
    PlayerMessage(GetOwner(pByObj), "$NoWeapon$", pByObj);
    CloseMenu(pByObj);
    Sound("lol.ogg");
  }

}

func DummyFunc()
{
  //Nichts unternehmen; diese Func existiert für die Option "Nichts" im Menü
}

/* Waffe modifizieren */

func Attach(id iItem, object pUser)
{
  //Waffe finden
  var pWeapon = FindContents(iItem, pUser);
  if(!pWeapon) return false;

  //Waffe modifizieren
  iAttachment = pWeapon->SetAttachment(iAttachment);

  //Sound
  Sound("WNAT_AddAttachement.ogg",0,pWeapon);

  //Verschwinden
  if(!iAttachment)
    RemoveObject(this);
}

/* Aufsatz-Icons */

global func AttachmentIcon(int iAtt)
{
  if(iAtt == AT_NoAttachment)		return 0;
  if(iAtt == AT_ExtendedMag)		return SM20;
  if(iAtt == AT_Bayonet)		return SM21;
  if(iAtt == AT_Laserpointer)		return SM22;
  if(iAtt == AT_Silencer)		return SM23;
  if(iAtt == AT_Foregrip)		return SM24;
  if(iAtt == AT_GrenadeLauncher)	return SM25;
  if(iAtt == AT_TracerDart)		return SM26;
}

/* Sonstiges */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg", false, this);
}

protected func Selection()
{
  Sound("FAPK_Charge.ogg", false, this);
}