/*--- Der Peacemaker ---*/

#strict
#include HZCK

#include L_A2 //Verfügt über CMC Agilität
#include L_ND //Kann Handgranaten einsetzen
#include L_CR //Kann Liegen und Kriechen

public func WeaponCollectionLimit() { return(3); } //Der Clonk kann drei Waffen tragen
public func ObjectCollectionLimit() { return(2); } //Und 2 beliebige Zusatzobjekte

public func Initialize() 
{
	UpdateAmmoBars();
	return _inherited();
}

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
  if(Contents()) {
   Contents()->~AbortWalk(this);
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

protected func UpdateAmmoBars()
{
	if(!FindObject(NOAM)) {
		for(var clonk in FindObjects(Find_OCF(OCF_Alive), Find_OCF(OCF_CrewMember), Find_NoContainer(), Find_Exclude(this)))
		{
		  var breaks = false;
		  for(var bar in FindObjects(Find_ID(ABAR), Find_ActionTarget(clonk))) {
		    if(GetOwner(bar) == GetOwner()) {
		    	if(!Contents() || GetID(Contents()) != AMPK) {
		    		bar->RemoveObject();
		    	}
		    	else {
		    		bar->SetBarCount(GetLength(FindObjects(Find_ID(ABAR), Find_ActionTarget(clonk), Find_Owner(GetOwner()))));
		    	}
		      breaks = true;
		    }
		  }
		  if(breaks) continue;
		  if(!Contents() || GetID(Contents()) != AMPK) continue;
		  if(Hostile(GetOwner(clonk),GetOwner())) continue;
		  if(!GetPlayerName(GetController(clonk))) continue;
		  CreateObject(ABAR,AbsX(GetX(clonk)),AbsY(GetY(clonk)),GetOwner())->Set(clonk,RGB(255,255,80));
		}
  }
  else {
  	RemoveAll(ABAR);
  }
  ScheduleCall(0,"UpdateAmmoBars",1);
}
