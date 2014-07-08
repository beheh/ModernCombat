/*-- Neues Script --*/

#strict 2

#appendto HZCK

protected func RejectCollect(id idObj, object pObj)
{
  if(GetEffect("CannotBeCollected", pObj))
    return true;

  //Für die KI
  var effect;
  if(effect = GetEffect("CollectionException", pObj))
    if(EffectVar(0, pObj, effect) == this)
      return 1;
  //Spawnpunkt-Hack
  if(idObj == SPNP) return;
  
  //Spezielle Spielzielitems immer aufsammeln
  if(pObj->~IsSpecialGoalItem())
  	return;
  
  //Munitionspaket?
  if(pObj ->~ IsAmmoPacket())
    // Davon kann man in jeden Fall _eines_ im Inventar haben
    if(!CustomContentsCount("IsAmmoPacket"))
      return;

  //Waffenaufsatz?
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

  //Waffe?
  if(pObj ->~ IsWeapon())
  {
    //Sonderbehandlung?
    if(!(pObj ->~ OnCollection(this)))
    {
      //nein. Standardprozedur:
      //schon so eine Waffe im Inventar? Oder bereits 3 andere Waffen?
      if(ContentsCount(idObj) || CustomContentsCount("IsWeapon") >= WeaponCollectionLimit())
        return 1;  //Ja, nicht aufnehmen
      else
        return;
    }
    else
      return;
  }

  //Granate?
  if(pObj ->~ IsGrenade())
  {
    if(pObj->~OnCollection(this))
      return;
    else
    {
      if(GrenadeCount() >= MaxGrenades())//Schon genug Granaten im Gürtel/Inventar?
      {
        return 1;
      }
      
      if(CustomContentsCount("IsGrenade"))
        ScheduleCall(pObj,"Activate",1,0,this);//Oha...

      return;
    }
  }

  //Pack?
  if(pObj->~IsPack())
  {
    var other = FindContents(idObj);
    if(other)
    {
      pObj->~JoinPack(other, this);
      return true;
    }
  }

  //Wieviel haben wir denn schon im inventar?
  if(ContentsCount() - (CustomContentsCount("IsWeapon") + CustomContentsCount("IsGrenade") + CustomContentsCount("IsAttachmentPack")) >= ObjectCollectionLimit())
    return 1;

  // nicht angelegte Ausrüstung nochmal aufsammeln
  for(var gear in aGear)
    if(gear)
      if(GetID(gear) == idObj)
        return 1;

  // Ok
  return;
}
