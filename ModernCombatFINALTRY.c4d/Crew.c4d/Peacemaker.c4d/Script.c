/*--- Der Peacemaker ---*/

#strict
#include HZCK

#include L_A2 //Verfügt über CMC Agilität
#include L_ND //Kann Handgranaten einsetzen
#include L_CR //Kann Liegen und Kriechen

public func WeaponCollectionLimit() { return(3); } //Der Clonk kann drei Waffen tragen
public func ObjectCollectionLimit() { return(2); } //Und 2 beliebige Zusatzobjekte


public func CanUse(id idObj)
{
  if(idObj == HARM) return(0);
  return(_inherited(idObj,...));
}

private func AbortWalk()
{
  if(GetAction() eq "Dive")
  {
    var c = Contents();
    if(c)
    {
      if((c->~IsWeapon() && !c->~GetFMData(FM_Aim)) || c->~IsGrenade())
        SetAction("Jump");
    }
  }
}

private func CheckArmed()
{
  if(GetAction() eq "Swim") return();
  //if (GetAction() eq "Dive") if (IsArmed2()) return(SetAction("JumpArmed"));
  
  return(_inherited());
}

protected func ContactBottom()
{
  return(0);
}