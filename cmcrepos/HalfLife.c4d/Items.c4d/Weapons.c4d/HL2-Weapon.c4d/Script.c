/*-- HL2-Waffe --*/

#strict
#include WPN2

static const FM_AmmoWarning = 30;

private func Shoot(object caller)
{
  if(!_inherited(caller)) return(false);
  if(GetFMData(FM_AmmoWarning))
    if(GetAmmo(GetFMData(FM_AmmoID)) == GetFMData(FM_AmmoWarning))
      Sound("H2WP_Warning.ogg",false,0,0,GetController(GetUser())+1); 
}

public func Hit2()
{
  Sound("H2WP_Hit*.ogg");
}