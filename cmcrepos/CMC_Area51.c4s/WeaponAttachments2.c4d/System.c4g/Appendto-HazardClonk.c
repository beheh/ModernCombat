/*-- Neues Script --*/

#strict 2

#appendto HZCK

// Ausrüstung ablegen
protected func ContextUnbuckle(object pCaller)
{
  [$CtxUnbuckleDesc$|Image=HARM|Condition=FunnyBug]
  if(!HasGear() && !HasAttachmentWeapons())
  	return;

  CreateMenu(HARM, 0, 0, 0, 0, 0, 1);

  // alle Ausrüstung anzeigen
  for(var i; i < GetLength(aGear); ++i) 
  {
    var gear = aGear[i];
    if(gear)
      AddMenuItem("$CtxUnbuckleItem$", Format("TakeOffGear(0, %d)", i), GetID(gear));
  }
  
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
	CreateContents(WNAT, this)->SetAttachment(att);
	
	return true;
}

public func HasAttachmentWeapons()
{
	var obj, i;
	while(obj = Contents(i++))
		if(obj->~IsWeapon2() && obj->~GetAttachment() != AT_NoAttachment)
			return true;
}
