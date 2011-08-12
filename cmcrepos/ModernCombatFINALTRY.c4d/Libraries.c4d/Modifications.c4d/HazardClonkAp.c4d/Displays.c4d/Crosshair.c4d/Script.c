/*-- Fadenkreuz --*/

#strict 2

local target;
local spread;
local angle;

static const CH_Distance = 60;
static const CH_Spread_Prec = 10;

public func NoWarp()	{return true;}


protected func GetUser()
{
  return target;
}

func Initialize()
{
  angle = 0;
  InitGraphics();
  
  return 1;
}

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

public func Init(object pTarget)
{
  Set(pTarget);
}

public func Set(object pTarget)
{
  SetOwner(GetOwner(pTarget));
  SetAction("Attach",pTarget);
  target = pTarget;
  //Schedule(Format("SetVisibility(%d)",VIS_Owner),1,0,this);
  //SetVisibility(VIS_None);
  SetVisibility(VIS_Owner);
  Check();
}

public func Reset(object pTarget)
{
  SetAction("Attach",pTarget);
  SetVisibility(VIS_Owner);
  Check();
}

public func UnSet()
{
  SetAction("Idle");
  SetActionTargets();
  SetVisibility(VIS_None);
  SetPosition();
}

public func SetAngle(int iAngle)
{
  angle = Normalize(iAngle);
  UpdateAngle();
}

public func GetAngle()
{
  return GetR();
}

public func ChangeDir()
{
  angle = Normalize(360-angle);
  UpdateAngle();
}

public func DoAngle(int iChange)
{
  angle = Normalize(angle+iChange);
  UpdateAngle();
}

public func UpdateAngle()
{
  if(target)  //Haaaaaax
    if(Contents(0,target))
      if(Contents(0,target)->~IsGrenade())
        if(!target->~IsAiming())
          angle = 65*(GetDir(target)*2-1);
  SetR(angle/*+GetR(target)*/);
  UpdateGraphics();
}

public func SetSpread(int iSpread)//Achtung, Präzision beachten!
{
  spread = iSpread;
  UpdateGraphics();
}

public func GetSpread()//Achtung, Präzision beachten!
{
  return spread;
}

public func InitGraphics()
{
  //Oben
  SetGraphics(0,0,GetID(),1,GFXOV_MODE_Action,"Base1"); 
  
  //Status oben
  SetGraphics(0,0,GetID(),2,GFXOV_MODE_Action,"Status1",GFX_BLIT_Additive); 
  
  //Nachtsicht oben
  //SetGraphics(0,0,GetID(),3,GFXOV_MODE_Action,"Add1"); 
  
  //Unten
  SetGraphics(0,0,GetID(),3,GFXOV_MODE_Action,"Base2"); 
  
  //Status unten
  SetGraphics(0,0,GetID(),4,GFXOV_MODE_Action,"Status2",GFX_BLIT_Additive);
  
  //Nachtsicht unten
  //SetGraphics(0,0,GetID(),6,GFXOV_MODE_Action,"Add2"); 
  
  UpdateGraphics();
}

public func UpdateGraphics()
{
  Draw(-GetR()*CH_Spread_Prec-(spread/2),CH_Distance,CH_Spread_Prec,1);
  Draw(-GetR()*CH_Spread_Prec-(spread/2),CH_Distance,CH_Spread_Prec,2,0,0,GFX_BLIT_Additive);
  //Draw(GetR()*CH_Spread_Prec-(spread/2),CH_Distance,CH_Spread_Prec,3,0,0,GFX_BLIT_Additive);
  
  Draw(-GetR()*CH_Spread_Prec+(spread/2),CH_Distance,CH_Spread_Prec,3);
  Draw(-GetR()*CH_Spread_Prec+(spread/2),CH_Distance,CH_Spread_Prec,4,0,0,GFX_BLIT_Additive);
  //Draw(GetR()*CH_Spread_Prec+(spread/2),CH_Distance,CH_Spread_Prec,6,0,0,GFX_BLIT_Additive);
}

private func Draw(int r, int off, int prec, int overlay)
{
  //r = -r;
  var xoff = -Sin(r,off,prec);
  var yoff = -Cos(r,off,prec);

  var fsin = Sin(r, 1000,prec), fcos=Cos(r, 1000,prec);

  SetObjDrawTransform
  (
    +fcos, +fsin,xoff*1000,
    -fsin, +fcos,yoff*1000,
    0, overlay
  );
}