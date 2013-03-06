/*-- Hazardclonk --*/

#strict 2 
#appendto HZCK

public func UpdateCH() 
{ 
  if((!ReadyToFire() && !ReadyToGrenade()) || !IsArmed()) 
    return HideCH(); 
 
  var c = Contents(); 
 
  DoSpread(BoundBy(TestSpread()-spread,0,10)); 
 
  var unspread, minspread; 
  if(c->~IsWeapon()) 
  { 
    unspread = c->GetFMData(FM_UnSpread); 
    minspread = c->GetFMData(FM_MinSpread); 
  } 
  else 
  { 
    unspread = c->~UnSpread(); 
    minspread = c->~MinSpread(); 
  } 
 
 
  if(IsAiming()) 
    if(IsCrawling()) 
      DoSpread(-(CH_CrawlSpreadReduction+unspread), 0, minspread); 
    else 
      DoSpread(-(CH_AimSpreadReduction+unspread), 0, minspread); 
  else 
    DoSpread(-(CH_StandSpreadReduction+unspread), 0, minspread); 
 
  if(GetCursor(GetOwner()) != this) 
  { 
    HideCH(); 
    return; 
  } 
 
  ShowCH(); 
 
  UpdateAiming(); 
  return true; 
}
