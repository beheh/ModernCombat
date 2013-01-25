/*-- Waffenattachment --*/

#strict 2

local iAttachment;

func Activate(object pClonk)
{
  AttachmentMenu(pClonk);
}

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

func AttachmentMenu(object pByObj)
{
  CreateMenu(GetID(this), pByObj, this, 0, "$AttachmentMenu$", 0, C4MN_Style_Context);

  var fItemExists = false;

  AddMenuItem(Format("$Attach$", "$Nothing$"), "DummyFunc", SM06, pByObj);
  for(var pWeapon in FindObjects(Find_Container(Contained()), Find_Func("IsWeapon2")))
  {
    fItemExists = true;
    if(pWeapon->~PermittedAtts() & iAttachment) AddMenuItem(Format("$Attach$", GetName(0, GetID(pWeapon))), "Attach", GetID(pWeapon), pByObj, 0, pByObj);
  }

  if(!fItemExists)
    CloseMenu(pByObj);
}

func DummyFunc()
{
  //Nichts unternehmen; diese Func existiert für die Option "Nichts" im Menü
}

func Attach(id iItem, object pUser)
{
  var pWeapon = FindContents(iItem, pUser);
  if(!pWeapon) return false;
  
  iAttachment = pWeapon->SetAttachment(iAttachment);
  
  if(!iAttachment) RemoveObject(this);
}

global func AttachmentIcon(int iAtt)
{
  if(iAtt == AT_NoAttachment) return 0;
  if(iAtt == AT_ExtendedMag) return SM20;
  if(iAtt == AT_Bayonet) return SM21;
  if(iAtt == AT_Laserpointer) return SM22;
  if(iAtt == AT_Silencer) return SM23;
  if(iAtt == AT_Foregrip) return SM24;
  if(iAtt == AT_GrenadeLauncher) return SM25;
  if(iAtt == AT_TracerDart) return SM26;
}
