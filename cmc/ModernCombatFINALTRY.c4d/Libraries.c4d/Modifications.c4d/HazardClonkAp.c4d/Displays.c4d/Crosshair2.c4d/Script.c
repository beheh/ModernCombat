/*-- Fahrzeugfadenkreuz --*/

#strict
#include HCRH


protected func Check()
{
  //Nutzer ermitteln
  var user = GetActionTarget();
  if(!user)
    return RemoveObject();
  if(!user->~IsAiming())
    return RemoveObject();	

  //Waffe ermitteln
  var wpn = Contents(0,user);
  if(!wpn) return;
  if(!wpn->~IsWeapon() && !wpn->~IsGrenade()) return;

  //Färbung
  var rgb = RGB(0,255,0);
  if(wpn->IsReloading() || !wpn->GetCharge())
    rgb = RGB(255,0,0);

  SetClrModulation(rgb,0,1);
}