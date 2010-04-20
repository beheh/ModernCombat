/*--- Der Sanitšter ---*/

#strict
#include PCMK

public func WeaponCollectionLimit() { return(2); } // max. Waffen im Inventar
public func ObjectCollectionLimit() { return(3); } // max. Objekte im Inventar

protected func IsMedic() { return(1); } //Diese eine Zeile macht den Clonk so besonders.

public func HazardGearSupported(object pGear)
{
  if(GetID(pGear) == HARM) return(false);
  return(true);
}

func Initialize()
{
  UpdateMediHUD();
  _inherited(...);
}

protected func UpdateMediHUD()
{
  for(var clonk in FindObjects(Find_OCF(OCF_Alive),Find_OCF(OCF_CrewMember),Find_NoContainer(),Find_Exclude()))
  {
    var break = 0;
    for(var bar in FindObjects(Find_ID(HBAR), Find_ActionTarget(clonk)))
      if(GetOwner(bar) == GetOwner())
        break++;
    if(break)
      continue;
    if(!Hostile(GetOwner(clonk),GetOwner()))
      CreateObject(HBAR,0,0,GetOwner())->Set(clonk,RGB(80,190,255));
  }
  ScheduleCall(0,"UpdateMediHUD",1);
}

public func Death()
{
  for(var bar in FindObjects(Find_ID(HBAR),Find_Owner(GetOwner())))
    RemoveObject(bar);
  _inherited(...);
}
