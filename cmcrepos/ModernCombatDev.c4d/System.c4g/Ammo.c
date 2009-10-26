/* Ammosystem */

#strict 2

global func DoAmmo(id ammoid, int change, object target, int slot)
{ 
  if(!(ammoid->~IsAmmo())) return 0;
  if(!target) target = this;
	
  var obj = target->~AmmoStoring();
  if(!obj) obj = target;
  if(ObjectCount(NOAM))
    if(obj->~IsAmmoStorage())
      return 0;
	
	var has_slots = obj->~HasAmmoSlots();
	if(!has_slots)
	{
	  if(!slot)
		  slot = obj->~GetSlot();
	}
	else
	{
	  slot = 0;
	}
	
	var ammo, oldammoamount;
	
	if(!has_slots)
	{
		ammo = FindContents(ammoid, obj);
		if(!ammo)
			ammo = CreateContents(ammoid, obj);
			
		oldammoamount = obj->GetAmmo(ammoid);
	}
  else
	{
	  oldammoamount = obj->GetAmmo(ammoid,slot);
	}

	var maxamount = ammo->~MaxAmount();
	var truechange;

	if(maxamount == 0)
		truechange = Max(oldammoamount+change, 0);
	else
		truechange = Max(Min(maxamount, oldammoamount+change), 0);


  // Neuer Wert dem Objekt geben
	if(!has_slots)
	{
	  if(truechange == 0)
		  RemoveObject(ammo);
		else
		  ammo->Local(0) = truechange;
	}
	else
	{
	  if(truechange == 0)
		  ammoid = 0;//Oder NULL ?
		obj->~SetAmmoCount(truechange,slot);
		obj->~SetAmmoID(ammoid,slot);
	}

  return truechange-oldammoamount;
}

global func GetAmmo(id ammoid, object target, int slot)
{
  if(!target) target = this;
	
  var obj = target->~AmmoStoring();
  if(!obj) obj = target;

  if(ObjectCount(NOAM))
    if(obj->~IsAmmoStorage())
      return 0;
	
	var has_slots = obj->~HasAmmoSlots();
	if(!has_slots)
	{
	  if(!slot)
		  slot = obj->~GetSlot();
	}
	else
	{
	  slot = 0;
	}
	
	if(!has_slots && !(ammoid->~IsAmmo())) return 0;

  var ammo;
  if(ammo=FindContents(ammoid, obj))
    return(ammo->Local(0));
  else
    return(0);
		
	if(!has_slots)
	{
		ammo = FindContents(ammoid, obj);
		if(!ammo)
			return 0;
		else
		  return obj->GetAmmo(ammoid);
	}
  else
	{
	  return obj->GetAmmoCount(slot);
	}
}

global func CheckAmmo(id ammoid, int count)
{
  if(!ammoid) return false;

  // gar keine Munition
  if(!(ammoid->~IsAmmo())) return false;

  if(!Par(2)) Par(2) = this;
  var iSum = 0;

  // search
  for(var i=2; i<10 && Par(i); ++i)
	{
    var obj = Par(i)->~AmmoStoring();
    if(!obj) obj = Par(i);

    if(ObjectCount(NOAM))
      if(obj->~IsAmmoStorage())
        return true;

    iSum += GetAmmo(ammoid,obj);
  }


  if(iSum >= count)
    return true;
  return false;
}

global func CheckAmmo2(id ammoid, int count, int slot)
{
  if(!ammoid) return false;

  // gar keine Munition
  if(!(ammoid->~IsAmmo())) return false;

  if(!Par(3)) Par(3) = this;
  var iSum = 0;

  // search
  for(var i = 3; i < 10 && Par(i); ++i)
  {
    var obj = Par(i)->~AmmoStoring();
    if(!obj) obj = Par(i);
		
    if(ObjectCount(NOAM))
      if(obj->~IsAmmoStorage())
        return true;
    iSum += GetAmmo(ammoid,obj,slot);
  }

  if(iSum >= count)
    return true;
  return false;
}