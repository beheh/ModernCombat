/*--- Der Peacemaker ---*/

#strict 2
#include HZCK

#include L_A2 //Verfügt über CMC Agilität
#include L_ND //Kann Handgranaten einsetzen
#include L_CR //Kann Liegen und Kriechen

public func WeaponCollectionLimit() { return 3; } //Der Clonk kann drei Waffen tragen
public func ObjectCollectionLimit() { return 2; } //Und 2 beliebige Zusatzobjekte


/* Initialisierung */

public func Initialize() 
{
  UpdateAmmoBars();
  return _inherited();
}

public func CanUse(id idObj)
{
  if(idObj == HARM) return;
  return _inherited(idObj,...);
}

private func AbortWalk()
{
  if(GetAction() == "Dive")
  {
    var c = Contents();
    if(c)
    {
      if((c->~IsWeapon() && !c->~GetFMData(FM_Aim)) || c->~IsGrenade())
        SetAction("Jump");
    }
  }
}

protected func ContactBottom()
{
  return;
}

protected func UpdateAmmoBars()
{
  if(FindObject(NOAM))
  {
    RemoveAll(ABAR);
  }
  else
  {
    for(var clonk in FindObjects(Find_OCF(OCF_Alive), Find_OCF(OCF_CrewMember), Find_NoContainer(), Find_Exclude(this)))
    {
      var breaks = false;
      for(var bar in FindObjects(Find_ID(ABAR), Find_ActionTarget(clonk)))
      {
        if(GetOwner(bar) == GetOwner())
        {
          if(!Contents() || GetID(Contents()) != AMPK)
          {
            RemoveObject(bar);
          }
          else
          {
            bar->SetBarCount(GetLength(FindObjects(Find_Func("IsBar"), Find_Func("BarActive"), Find_ActionTarget(clonk), Find_Owner(GetOwner()))));
          }
          breaks = true;
        }
      }
      if(breaks) continue;
      if(!Contents() || GetID(Contents()) != AMPK) continue;
      if(Hostile(GetOwner(clonk),GetOwner())) continue;
      if(!GetPlayerName(GetController(clonk))) continue;
      CreateObject(ABAR,AbsX(GetX(clonk)),AbsY(GetY(clonk)),GetOwner())->Set(clonk,RGB(255,255,80),GetLength(FindObjects(Find_Func("IsBar"), Find_Func("BarActive"), Find_ActionTarget(clonk), Find_Owner(GetOwner())))+1);
    }
  }
  ScheduleCall(0,"UpdateAmmoBars",1);
}