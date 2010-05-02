/* ScreenRGB */
#strict 2

local r,g,b,a;
local target,layer,fade;

static const SR4K_LayerSmoke = 1;
static const SR4K_LayerLight = 2;

public func Initialize()
{
  SetObjDrawTransform(1000*1000,0,0,0,1000*1000,0,0,0);

  SetPosition(0,0);

  SetClrModulation (RGBa(0,0,0,255));
  a = 255;

  return 1;
}

global func ScreenRGB(object pTarget, int dwRGBa, int iAlphaAdd, int iFadeRate, bool bAdditive, int iLayer)
{
  var obj;
  if(iLayer)
    obj = FindObject2(Find_ID(S24K),Find_Owner(GetOwner(pTarget)),Find_Func("SameLayer",iLayer));
    
  if(!obj)
    obj = CreateObject(S24K,0,0,GetOwner(pTarget));
    
  obj->Set(pTarget,dwRGBa,iAlphaAdd,iFadeRate,bAdditive,iLayer);
  return obj;
}

public func SameLayer(int iLayer)
{
  if(layer) if(iLayer == layer) return true;
}

public func Set(object pTarget, int dwRGBa, int iAlphaAdd, int iFadeRate, bool bAdditive, int iLayer)
{
  target = pTarget;
  layer = iLayer;

  if(!target)
    SetVisibility(VIS_All);
  else
    SetAction("CursorCheck");

  if(bAdditive)//Additiv zeichnen?
    SetObjectBlitMode(GFX_BLIT_Additive);
    
  fade = Max(fade,iFadeRate);
  
  var a_save = a;
  SplitRGBaValue(dwRGBa,r,g,b,a);
  a = BoundBy(a_save-iAlphaAdd,0,255);
    
  if(!fade)
    RemoveEffect("IntRGBFade",this);
  else
    if(!GetEffect("IntRGBFade",this))
      AddEffect("IntRGBFade",this,25,3,this);
}


///Fade-Effekt

public func FxIntRGBFadeStart(object pTarget, int iEffectNumber, int iTemp)
{
  return 1;
}

public func FxIntRGBFadeTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  a = BoundBy(a+fade,0,255);
  pTarget->SetClrModulation(RGBa(r,g,b,a));
  
  if(a <= 0)
    return -1;

  return 0;
}

public func FxIntRGBFadeStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  RemoveObject();
  return 0;
}

public func GetAlpha(){return a;}

public func SetAlpha(int iValue)
{
  a = BoundBy(iValue,0,255);
  SetClrModulation(RGBa(r,g,b,a));
  
  if(a >= 255) RemoveObject();// !!!
}

public func DoAlpha(int iValue, int iMin, int iMax)
{
  if(!iMax) iMax = 255;
  a = BoundBy(a-iValue,Max(iMin,0),Min(iMax,255));
  SetClrModulation(RGBa(r,g,b,a));
  
  if(a >= 255) RemoveObject();// !!!
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
