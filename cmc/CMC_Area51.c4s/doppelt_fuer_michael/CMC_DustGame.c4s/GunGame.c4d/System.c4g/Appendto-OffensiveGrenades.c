/*-- Flamende Granaten --*/

#strict 2

#appendto FGRN
#appendto FRAG
#appendto PGRN

public func IsOffensiveGrenade() { return true; }

/*-- Gehoert zu NADE --*/

func UpdateHUD(object pHUD)
{
  var user = GetUser();
  if(!user) return ;
  if(!user->~MaxGrenades()) return 0;

  pHUD->Ammo(1,user->GrenadeCount(GetID()), GetName(), true);
  if(GetEffect("Cooldown", this))
  	pHUD->~Recharge(GetEffect("Cooldown", this, 0, 6), GetEffect("Cooldown", this, 0, 3)-1);
  else
  	pHUD->Recharge(GetThrowDelayTime(GetUser()), ThrowDelay());
}

public func Fuse()
{
	//Kann nicht Werfen da Cooldown
	if(GetEffect("Cooldown", this))
		return;

  Sound("GrenadeActivate.ogg");
  SetGraphics("Active");
  return AddEffect("IntFuse",this,200,1,this);
}
