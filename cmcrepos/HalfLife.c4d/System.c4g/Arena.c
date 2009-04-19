#strict
#appendto NODR

protected func Timer()
{
  //...
}

public func FxRemoveInvOnDeathStop(target, no, reason)
{
  if(reason == 4)
  {
    for(var i = 0 ; Contents(i, target) ; i++)
      if(Contents(i, target)->~IsWeapon() || Contents(i, target)->~IsEquipment())
        Schedule("RemoveObject();", 1, 0, Contents(i, target));
    if(LocalN("aGear", target))
      for(var pGear in LocalN("aGear", target))
        if(pGear)
        {
          pGear->GearUnbuckle(target);
          pGear->RemoveObject();
        }
  }
}