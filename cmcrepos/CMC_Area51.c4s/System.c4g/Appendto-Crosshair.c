#strict 2
#appendto CRH2

protected func Check()
{
  if(!target)
    return RemoveObject();
  if(!GetAlive(target))
    return RemoveObject();

  var wpn = Contents(0, target);
  if(!wpn)
  {
    target->HideCH();
    return;
  }
  if(!wpn->~IsWeapon() && !wpn->~IsGrenade()) return;
	
	if(GetProcedure(target) == "ATTACH" && GetActionTarget(0, target)->~DisableCH())
	{
  target->HideCH();
  return;
	}
	
  if(GetProcedure(target) == "PUSH" && GetActionTarget(0, target) && GetActionTarget(0, target)->~CanAim())
  {
    target->HideCH();
    /*if(GetActionTarget(0, target)->~IsWeapon() && !(GetActionTarget(0, target)->~IsWeapon2()))
      return false; 
    else
      return true;*/
  }

  var alpha = 0;//Sin(spread*90/CH_MaxSpread,255);
  //var alpha = spread*255/CH_MaxSpread;

  var rgb = RGBa(0,255,0,alpha);
  if(wpn->IsReloading() || !wpn->GetCharge())
    rgb = RGBa(255,0,0,alpha);

  SetClrModulation(RGBa(255,255,255,alpha),0,1);	//Base oben
  SetClrModulation(rgb,0,2);				//Overlay oben
  SetClrModulation(RGBa(255,255,255,alpha),0,3);	//Base unten
  SetClrModulation(rgb,0,4);				//Overlay unten

  UpdateGraphics();
}
