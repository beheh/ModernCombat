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
    if(LocalN("iPermittedAtts", pWeapon) & iAttachment) AddMenuItem(Format("$Attach$", GetName(0, GetID(pWeapon))), "Attach", GetID(pWeapon), pByObj, 0, pByObj);
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
