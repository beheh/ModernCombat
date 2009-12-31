/* ScreenRGB 3 */
#strict 2

local r,g,b,a;
local target,layer;

public func Initialize()
{
  SetObjDrawTransform(1000*1000,0,0,0,1000*1000,0,0,0);

  SetPosition(0,0);

  SetClrModulation(RGBa(0,0,0,255));
  a = 255;

  return 1;
}

global func ScreenRGB3(object pTarget, int dwRGBa, bool bAdditive, int iLayer, object pRGB)
{
  if(!pRGB)
  {
    var plr = NO_OWNER;
    if(pTarget) plr = GetOwner(pTarget);
  
    if(iLayer)
      pRGB = FindObject2(Find_ID(S34K),Find_Owner(plr),Find_Func("SameLayer",iLayer));
    
    if(!pRGB)
      pRGB = CreateObject(S34K,0,0,plr);
  }
    
  pRGB->Set(pTarget,dwRGBa,bAdditive,iLayer);
  return pRGB;
}

public func SameLayer(int iLayer)
{
  if(layer) if(iLayer == layer) return true;
}

public func Set(object pTarget, int dwRGBa, bool bAdditive, int iLayer)
{
  target = pTarget;
  layer = iLayer;

  if(!target)
    SetVisibility(VIS_All);
  else
    SetAction("CursorCheck");

  if(bAdditive)
    SetObjectBlitMode(GFX_BLIT_Additive);
  else
    SetObjectBlitMode(0);

  SetClrModulation(dwRGBa);
}

func CursorCheck()
{
  if(GetCursor(GetOwner()) != target)
    SetVisibility (VIS_None);
  else
    SetVisibility (VIS_Owner);
}

public func NoWarp(){return true;}
public func IsHUD(){return true;}
