/*-- Use Grenades --*/

#strict

local aGear;

public func MaxGrenades(){return(4);}	//Maximale Granaten im Inventar + G�rtel


/* Allgemeines */

local pGrenadeStoring;

protected func ContextGrenadeMenu(object pCaller)
{
  [$CtxGrenadeMenu$|Image=GRNS|Condition=HasGrenades]
  Sound("GrenadeCharge.ogg", 1, 0,0, GetOwner()+1);
  GrenadeMenu(pCaller);
}

protected func HasGrenades()
{
  if(!pGrenadeStoring) return(false);
  if(pGrenadeStoring->Contents()) return(true);
  return(false);
}

public func GrenadeMenu(object pCaller)
{
  CreateMenu(GRNS,pCaller,this(),0,"$CtxGrenadeMenu$",0,C4MN_Style_Context);
  
  var id_list = CreateArray();
  for(var nade in FindObjects(Find_Container(pGrenadeStoring)))
  {
    if(FindInArray4K(id_list,GetID(nade)) != -1) continue;
    id_list[GetLength(id_list)] = GetID(nade);
    
    AddMenuItem(Format("<c %x>%.1s</c>",nade->Color(),GetName(nade)), "GrabGrenade", GetID(nade), pCaller,
                ObjectCount2(Find_ID(GetID(nade)), Find_Container(pGrenadeStoring)),
                0, GetDesc(nade));
  }
}

public func GetGrenade(id type)
{
  return(FindContents(type,pGrenadeStoring));
}

public func GrabGrenade(id type)
{
  var grenade = FindContents(type,pGrenadeStoring);
  if(!grenade) return(0);
  grenade->Enter(this());
  ShiftContents (this(),false,type,true);
  return(grenade);
}

public func GrabGrenades(object pInto) {
	for(var pGrenade in FindObjects(Find_Container(pGrenadeStoring))) {
		pGrenade->Enter(pInto);
	}
	return(true);
}

public func StoreGrenade(object pGrenade)
{
  if(!pGrenade) return();

  if(pGrenade->Contained()  == this())
  {
    if(ObjectCount2(Find_Container(pGrenadeStoring)) >= MaxGrenades())
      return(false);
  }
  else
  {
    if(GrenadeCount() >= MaxGrenades())
      return(false);
  }
  Sound("GrenadeCharge.ogg", 1, 0,0, GetOwner()+1);
  pGrenade->Enter(pGrenadeStoring);
  return(true);
}

public func GrenadeCount(id type)
{
  if(!type) return(ObjectCount2(Find_Container(pGrenadeStoring)) + ObjectCount2(Find_Func("IsGrenade"),Find_Container(this())));
  return(ObjectCount2(Find_ID(type), Find_Container(pGrenadeStoring)) + ObjectCount2(Find_ID(type), Find_Container(this())));
}

public func GetGrenadeStoring()
{
  if(!pGrenadeStoring)
    pGrenadeStoring = CreateObject(GRNS,0,0,GetOwner());
  return(pGrenadeStoring);
}

/* Overloads */

protected func Initialize()
{
  if(!pGrenadeStoring)
    pGrenadeStoring = CreateObject(GRNS,0,0,GetOwner());
    
  pGrenadeStoring->SetUser(this());
  
  return(_inherited(...));
}

protected func Destruction()
{
  if(pGrenadeStoring)
    RemoveObject(pGrenadeStoring);
  return(_inherited(...));
}

public func IsArmed()
{
  if(!Contents()) return(); 
  if(Contents()->~IsWeapon())
    return(true);
  if(Contents()->~IsGrenade())
    return(true);
  return(false);
}

protected func Collection2(object pObj)// Einsammeln
{
  // das neue Item nach hinten verschieben (au�er es ist Ammo oder eine Granate)
  if(!(pObj->~IsAmmoPacket()) || !(pObj->~IsGrenade()) || NoAmmo())
    if (1 == ContentsCount(GetID(pObj)))
      if(!(pObj->GetOCF() & OCF_Living)) {
      ShiftContents(0,0,0,0);
      pObj ->~ OnDeselect();
    }
  this()->~UpdateCharge();
}

protected func RejectCollect(id idObj, object pObj)
{
  // F�r die KI
  var effect;
  if(effect = GetEffect("CollectionException", pObj))
    if(EffectVar(0, pObj, effect) == this())
      return(1);
  // Spawnpunkt-Hack
  if(idObj == SPNP) return();
  // Munitionspaket?
  if(pObj ->~ IsAmmoPacket())
    // Davon kann man in jeden Fall _eines_ im Inventar haben
    if(!this()->~CustomContentsCount("IsAmmoPacket"))
      return(0);
  
  //Waffe?
  if(pObj ->~ IsWeapon())
  {
    //Sonderbehandlung?
    if(!(pObj ->~ OnCollection(this())))
    {
      //nein. Standardprozedur:
      //schon so eine Waffe im Inventar? Oder bereits 3 andere Waffen?
      if(ContentsCount(idObj) || this()->~CustomContentsCount("IsWeapon") >= this()->~WeaponCollectionLimit())
        return(1);  //Ja, nicht aufnehmen
      else
        return(0);
    }
    else
      return(0);
  }
  
  //Granate?
  if(pObj ->~ IsGrenade())
  {
    if(pObj->~OnCollection(this()))
      return(0);
    else
    {
      if(GrenadeCount() >= MaxGrenades())//Schon genug Granaten im G�rtel/Inventar?
      {
        return(1);
      }
      
      if(this()->~CustomContentsCount("IsGrenade"))
        ScheduleCall(pObj,"Activate",1,0,this());//Oha...

      return(0);
    }
  }
  
  // Einsammellimit f�r Ausr�stung
  /*if(pObj ->~ IsEquipment() && ContentsCount(idObj) > 0)
    return(1);*/

  //Wieviel haben wir denn schon im inventar?
  if(ContentsCount() - (this()->~CustomContentsCount("IsWeapon") + this()->~CustomContentsCount("IsGrenade")) >= this()->~ObjectCollectionLimit())
    return(1);
  
  // nicht angelegte Ausr�stung nochmal aufsammeln
  for(var gear in aGear)
    if(gear)
      if(GetID(gear) == idObj)
        return(1);
  
  // Ok
  return(0);
}

private func ChangeWeapon(object pTarget)
{
	if(pTarget->~IsWeapon() || pTarget->~IsGrenade())
  {
		var phase = GetPhase();
    if(this()->~IsCrawling())
    {
      SetAction("AimCrawl");
    }
    else if(pTarget->~GetFMData(FM_Aim) > 0)
    {
      if(WildcardMatch(GetAction(), "*Squat*"))
        SetAction("AimSquatLow");
      else
			  SetAction("AimLow");
    }
		else
    {
      if(WildcardMatch(GetAction(), "*Squat*"))
        SetAction("AimSquat");
      else
			  SetAction("Aim");
    }
	  SetPhase(phase);
	}
	else
  {
		this()->~StopAiming();
	}
}

/*protected func ControlContents(idTarget)
{
  // Aktuelles Objekt verhindert Shift?
  if(Contents())
    if(Contents()->~RejectShift(idTarget))
      return(1);
      
  // Zielobjekt will gar nicht, dass wir was machen
  if(idTarget->~NoControlContents()) return();
  
  // Wer ist idTarget?
  var pTarget = FindContents(idTarget);
  
  // Zielaktion abbrechen (Spezial: au�er wenn mit anw�hlbarem 
  // Objekt auch gezielt werden kann...)
  if(this()->~IsAiming())
  {
     var phase = GetPhase();
     // Zielaktion anpassen
     if(pTarget->~IsWeapon())
     {
       if(pTarget->~GetFMData(FM_Aim) == 2)
         SetAction("AimSquatLow");
       else
         SetAction("AimSquat");
         
       SetPhase(phase);
     }
     else if(pTarget->~IsGrenade())
     {
       SetAction("AimSquatLow");
       SetPhase(phase);
     }
     else
     {
       this()->~StopAiming();
     }
  }
  // Hast du noch einen letzten Wunsch, Contents(0)??!
  if(Contents(0)) Contents(0)->~Deselection(this());
  // Rotieren
  if (!ShiftContents(0, 0, idTarget)) return(1);
  // Waffe ziehen/wegstecken
  this()->~CheckArmed();
  // Munitionsanzeige updaten
  UpdateCharge();
  // Objekt benachrichtigen
  if(Contents(0)) Contents(0)->~Selection(this());
}*/
