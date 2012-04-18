/*-- Konsole --*/

#strict 2

#appendto DRSU

public func Activate(object byObj)
{
  //Checken, ob der Clonk nicht schon eine hat. :Ooo
  var pDrone;
  
  // Kann Ding nicht benutzen...
  if(!(byObj->~CanUse(GetID()))) return(1);
  
 
  //Kaputte Drohne?
  if(EMPShocked())
  {
    PlayerMessage(byObj->GetOwner(), "$Damaged$", byObj);
    Sound("Error");
    return(1);
  }
  
  // Menü erzeugen
  CreateMenu(DRSU, byObj, this(),0,"$NoWeap$");
  
  //Inventar des Clonks durchgehen
  for(var i = 0; i < byObj->ContentsCount(); ++i)
  {
    var pInvObj = byObj->Contents(i);
    //Wenn Waffe, dem Menü hinzufügen
    if(pInvObj->~GetFMData(FM_Aim)>0) continue; // Aber keine Waffen, die zielen müssen
    if(pInvObj->~IsWeapon())
      AddMenuItem("$ArmWith$", "GetArmed", pInvObj->GetID(), byObj, 0, pInvObj);
  } 
  
  //Schützling zuweisen
  pGuard = byObj;
  SetOwner(GetOwner(pGuard));
  return(1);
}