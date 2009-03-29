/*-- Fadenkreuz --*/

#strict

local target;//soll jetzt dauernd benutzt werden ... daher ist eine lokale variable angebracht
local spread;

static const CH_Distance = 60;
static const CH_Spread_Prec = 10;

public func NoWarp() {return(true);}

protected func GetUser()
{
  return(target);
}

func Initialize()
{
  InitGraphics();
  
  return(1);
}

protected func Check()
{
  if(!target)
    return(RemoveObject());
  if(!target->GetAlive())
    return(RemoveObject());

    
  var wpn = target->Contents();
  if(!wpn)
  {
    target->HideCH();//Wegen eine komischen Bug bei UpdateCharge()...
    return();
  }
  if(!wpn->~IsWeapon() && !wpn->~IsGrenade()) return();
    
  var alpha = 0;//Sin(spread*90/CH_MaxSpread,255);
  //var alpha = spread*255/CH_MaxSpread;
  
  var rgb = RGBa(255,180,0,alpha);
  if(wpn->IsReloading() || !wpn->GetCharge())
    rgb = RGBa(180,180,180,alpha);
    
  SetClrModulation(rgb);
  UpdateGraphics();
}

public func Init(object pTarget)//O_o *lol* Henry gefällt der Name nicht. xD
{
  Set(pTarget);
}

public func Set(object pTarget)
{
  SetOwner(GetOwner(pTarget));
  SetAction("Attach",pTarget);
  target = pTarget;
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
  SetR(iAngle);
  UpdateGraphics();
}

public func GetAngle()
{
  return(GetR());
}

public func ChangeDir()
{
  SetR(360-GetR());
}

public func DoAngle(int iChange)
{
  SetR(GetR()+iChange);
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
  //Hier gibts nix zu tun. :)

  UpdateGraphics();
}

public func UpdateGraphics()
{
  var size = Max(Tan2(spread,CH_Distance,CH_Spread_Prec)*1000/32,100);
  Draw(size,GetR(),CH_Distance,CH_Spread_Prec);
}

private func Draw(int s, int r, int off, int prec)
{
  var yoff = -Cos(r,off*1000,prec);

  var fsin = Sin(r, s,prec), fcos = Cos(r, s,prec);

  SetObjDrawTransform
  (
    +fcos, +fsin,0,
    -fsin, +fcos,yoff
  );
}

private func Tan2(int iAngle, int iRadius, int iPrec)
{
  return((iRadius * Sin(iAngle,iRadius*100,iPrec)) / Cos(iAngle,iRadius*100,iPrec));
}