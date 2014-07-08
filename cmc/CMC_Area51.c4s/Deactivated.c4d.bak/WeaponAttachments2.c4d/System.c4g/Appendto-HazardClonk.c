/*-- Neues Script --*/

#strict 2

#appendto HZCK

protected func RejectCollect(id idObj, object pObj)
{
  if(FindContents(idObj, this) && pObj->~IsWeapon2() && pObj->~GetAttachment() != AT_NoAttachment && Contained(pObj))
  {
    var att = CreateObject(WNAT, 0, 0, GetOwner());
    if(Collect(att, this))
    {
      att->SetAttachment(pObj->GetAttachment());
      Sound("Merge.ogg");

      //Überschreibt die "Holen nicht möglich"-Nachricht
      Schedule(Format("PlayerMessage(%d, \"$AttachmentCollected$\", this)", GetOwner(), GetName(0, AttachmentIcon(pObj->GetAttachment()))), 1, 0, this);

      pObj->~SetAttachment(AT_NoAttachment);
    }
    else if(att)
      RemoveObject(att);

    return true;
  }

  //Waffenaufsatzobjekte erhalten einen zusätzlichen Inventarslot
  if(pObj->~IsAttachmentPack())
    if(!CustomContentsCount("IsAttachmentPack"))
      return;

  return _inherited(idObj, pObj, ...);
}

/* Ausrüstung ablegen */

protected func ContextUnbuckle(object pCaller)
{
  [$CtxUnbuckleDesc$|Image=HARM|Condition=FunnyBug]
  if(!HasGear() && !HasAttachmentWeapons())
    return;

  CreateMenu(HARM, 0, 0, 0, 0, 0, 1);

  //Alle Ausrüstungen auflisten
  for(var i; i < GetLength(aGear); ++i) 
  {
    var gear = aGear[i];
    if(gear)
      AddMenuItem("$CtxUnbuckleItem$", Format("TakeOffGear(0, %d)", i), GetID(gear));
  }

  //Alle Waffenaufsätze auflisten
  var obj, i = 0;
  while(obj = Contents(i++))
    if(obj->~IsWeapon2() && obj->~GetAttachment() != AT_NoAttachment)
      AddMenuItem(Format("$CtxDetachAttachment$", GetName(0, AttachmentIcon(obj->GetAttachment()))), "DetachAttachment", 0, this, 0, obj, 0, 4, obj);

  return true;
}

public func DetachAttachment(dummy, object pWeapon)
{
  if(!pWeapon->~IsWeapon2() || pWeapon->~GetAttachment() == AT_NoAttachment)
    return;

  var att = pWeapon->~GetAttachment();
  pWeapon->SetAttachment(AT_NoAttachment);

  //Waffenaufsatzobjekt erstellen
  var obj = CreateObject(WNAT, 0, 0, GetOwner());
  obj->SetAttachment(att);
  Collect(obj, this);

  //Waffen-HUD aktualisieren
  UpdateCharge(true);

  Sound("WNAT_RemoveAttachement.ogg");

  return true;
}

public func HasAttachmentWeapons()
{
  var obj, i;
  while(obj = Contents(i++))
    if(obj->~IsWeapon2() && obj->~GetAttachment() != AT_NoAttachment)
      return true;
}

protected func FunnyBug()	{return HasGear() || HasAttachmentWeapons();}

//Wegen der OpenBeta: Damit der Menüeintrag nicht von dem "Ausrüstung ablegen" Eintrag verschiebt.

protected func ContextGrenadeMenu(object pCaller)
{
  [$CtxGrenadeMenu$|Image=GRNS|Condition=HasGrenades]
  Sound("GrenadeCharge.ogg", 1, 0,0, GetOwner()+1);
  GrenadeMenu(pCaller);
}