/*-- Kampfclonk Appendto --*/

#strict 2
#appendto CIVC



func ResetShowWeapon(object pNew)
{
  var effect = GetEffect("ShowWeapon",this); 
  if(!effect) return false;
  
  if(pNew)
  {
    EffectVar(0, this(), effect) = GetID(pNew);
    EffectVar(1, this(), effect) = pNew;
  }
  else
  {
    EffectVar(0, this(), effect) = 0;
    EffectVar(1, this(), effect) = 0;
  }
  
  return true;
}

/* neues Zielsystem */
static const CH_ShowAlways = 1;

static const CH_MaxSpread = 500;

static const CH_WalkSpread = 80;
static const CH_JumpSpread = 140;
static const CH_ScaleSpread = 200;
static const CH_HangleSpread = 200;

static const CH_CrawlSpreadReduction = 4;
static const CH_AimSpreadReduction = 4;
static const CH_StandSpreadReduction = 3;

local crosshair,ammobag;
local spread, handr;

public func Initialize()
{
  this->~InitCrosshair();
  //if(Contained())
    //HideCrosshair();
  return _inherited();
}

/*func HandR()
{
  return 0);//return handr/CH_Spread_Prec);
}*/

public func GetCrosshair()
{
  return crosshair;
}

private func GetSpreadAOff()
{
  handr = RandomX(-spread/2/CH_Spread_Prec,+spread/2/CH_Spread_Prec);
  return handr;
}

private func GetSpreadAOff2()
{
  handr = RandomX(-spread/2/CH_Spread_Prec,+spread/2/CH_Spread_Prec);
  return handr*(100/CH_Spread_Prec);
}

private func TestSpread()
{
  var proc = GetProcedure();
  
  if(proc == "WALK")
  {
    if(GetComDir())
      if(spread < CH_WalkSpread)
        return CH_WalkSpread;
    return 0;
  }
  
  if(proc == "SWIM")
  {
    if(spread < CH_WalkSpread)
      return CH_WalkSpread;
    return 0;
  }
  
  if(proc == "FLIGHT")
  {
    if(spread < CH_JumpSpread)
      return CH_JumpSpread;
    return 0;
  }

  if((proc == "SCALE")||(GetAction() == "ScaleLadder"))
  {
    if(spread < CH_ScaleSpread)
      return CH_ScaleSpread;
    return 0;
  }
  
  if(proc == "HANGLE")
  {
    if(spread < CH_HangleSpread)
      return CH_HangleSpread;
    return 0;
  }
  
  return 0;
}

public func UpdateCharge()//*ausnutz*
{
  /*var x,y,r;
  if(!this()->~WeaponAt(x,y,r))
  {
     HideCrosshair();
     return _inherited());
  }
    
  crosshair->SetAngle(r);*/
  
  //if(!ControlledCanAim())
  //{
    /*if(!c)
    {
      HideCrosshair();
      return _inherited());
    }*/
    
    if(!this()->~ReadyToFire() || !this()->~IsArmed() || (GetCursor(GetOwner()) != this()))
    {
      this->~HideCrosshair();
      return _inherited();
    }
  //}
  
  var c = Contents();

  this->~ShowCrosshair();
  
  DoSpread(BoundBy(TestSpread()-spread,0,10));
  
  var unspread;
  if(c->~IsWeapon())
    unspread = c->GetFMData(FM_UnSpread);
  else
    unspread = c->~UnSpread();

    
  if(this()->~IsAiming())
  {
    if(this->~IsCrawling())
      DoSpread(-(CH_CrawlSpreadReduction+unspread));
    else
      DoSpread(-(CH_AimSpreadReduction+unspread));
  }
  else
  {
    DoSpread(-(CH_StandSpreadReduction+unspread));
  }

  this->~UpdateAiming();
  
  return _inherited();
}



public func DoSpread(int iChange, int iMax)
{
  var wpn = Contents();
  if(!wpn) return;
  if(!wpn->~IsWeapon() && !wpn->~IsGrenade()) return;
  
  if(iMax) iChange = Max(0,BoundBy(spread+iChange,0,iMax)-spread);
  spread = BoundBy(spread+iChange,0,CH_MaxSpread);
  
  if(crosshair)
    crosshair->SetSpread(spread);
}

public func GetSpread()
{
  return spread;
}