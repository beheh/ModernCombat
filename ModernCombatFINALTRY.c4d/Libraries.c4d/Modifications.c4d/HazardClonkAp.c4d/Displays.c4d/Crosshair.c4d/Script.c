/*-- Fadenkreuz --*/

#strict 2

local target;
local spread, iLastSpread;
local angle;

static const CH_Distance = 60;
static const CH_Spread_Prec = 10;

public func NoWarp()	{return true;}


/* Initialisierung */

func Initialize()
{
  angle = 0;
  InitGraphics();

  return 1;
}

public func InitGraphics()
{
  //Kontrast links
  SetGraphics(0,0,GetID(),1,GFXOV_MODE_Action,"Base1");
  SetClrModulation(RGB(255,255,255),0,1);

  //Färbung links
  SetGraphics(0,0,GetID(),2,GFXOV_MODE_Action,"Status1",GFX_BLIT_Additive);

  //Kontrast rechts
  SetGraphics(0,0,GetID(),3,GFXOV_MODE_Action,"Base2");
  SetClrModulation(RGB(255,255,255),0,3);

  //Färbung rechts
  SetGraphics(0,0,GetID(),4,GFXOV_MODE_Action,"Status2",GFX_BLIT_Additive);

  UpdateGraphics();
}

protected func GetUser()
{
  return target;
}

/* Timer */

protected func Check()
{
  //Clonk ermitteln
  if(!target)
    return RemoveObject();
  if(!GetAlive(target))
    return RemoveObject();

  //Waffe ermitteln
  var wpn = Contents(0, target);
  //Keine Waffe: Ausblenden
  if(!wpn)
  {
    target->HideCH();
    return;
  }
  if(!wpn->~IsWeapon() && !wpn->~IsGrenade()) return;

  //Clonk mit unpassender Aktion: Ausblenden
  if(GetProcedure(target) == "PUSH" && GetActionTarget(0, target) && GetActionTarget(0, target)->~CanAim())
    target->HideCH();

  //Färbung je nach Waffenstatus
  var rgb = RGB(0,255,0);
  if(wpn->IsReloading() || !wpn->GetCharge())
    rgb = RGB(255,0,0);
  SetClrModulation(rgb,0,2);	//Färbung links
  SetClrModulation(rgb,0,4);	//Färbung rechts

  //Spread bei Bedarf aktualisieren
  if(spread != iLastSpread)
  {
    UpdateGraphics();
    iLastSpread = spread;
  }
}

/* Konfiguration */

public func Init(object pTarget)
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
  if(target)
    if(Contents(0,target))
      if(Contents(0,target)->~IsGrenade())
        if(!target->~IsAiming())
          angle = 65*(GetDir(target)*2-1);
  SetR(angle/*+GetR(target)*/);
  UpdateGraphics();
}

public func SetSpread(int iSpread)
{
  spread = iSpread;
  if(spread != iLastSpread)
  {
    UpdateGraphics();
    iLastSpread = spread;
  }
}

public func GetSpread()
{
  return spread;
}

/* Fadenkreuz zeichnen */

public func UpdateGraphics()
{
  Draw(-GetR()*CH_Spread_Prec-(spread/2),CH_Distance,CH_Spread_Prec,1);
  Draw(-GetR()*CH_Spread_Prec-(spread/2),CH_Distance,CH_Spread_Prec,2);

  Draw(-GetR()*CH_Spread_Prec+(spread/2),CH_Distance,CH_Spread_Prec,3);
  Draw(-GetR()*CH_Spread_Prec+(spread/2),CH_Distance,CH_Spread_Prec,4);
}

private func Draw(int r, int off, int prec, int overlay)
{
  //r = -r;
  var xoff = -Sin(r,off,prec);
  var yoff = -Cos(r,off,prec);

  var fsin = Sin(r, 1000,prec), fcos=Cos(r, 1000,prec);

  SetObjDrawTransform
  (
    +fcos, +fsin, xoff*1000,
    -fsin, +fcos, yoff*1000,
    0, overlay
  );
}