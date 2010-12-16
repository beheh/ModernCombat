/*-- ScreenRGB --*/

#strict 2

local r,g,b,a;
local target,layer,fade;

static const SR4K_LayerSmoke		= 1;
static const SR4K_LayerLight		= 2;
static const SR4K_LayerDamage		= 3;
static const SR4K_LayerFakeDeath	= 4;
static const SR4K_LayerBorder		= 5;

public func GetTargetCursor()		{}
public func IsOverlayScreen()		{return true;}
public func NoWarp()			{return true;}
public func IsHUD()			{return true;}
public func GetAlpha()			{return a;}


/* Initialisierung */

public func Initialize()
{
  SetObjDrawTransform(2000 * LandscapeWidth() / GetDefWidth(GetID()), 0, 0, 0, 2000 * LandscapeHeight() / GetDefHeight(GetID()));
  SetPosition();
  SetClrModulation (RGBa(0, 0, 0, a = 255));
}

global func ScreenRGB(object pTarget, int dwRGBa, int iAlphaAdd, int iFadeRate, bool bAdditive, int iLayer)
{
  var obj;
  if(iLayer)
    obj = GetScreenRGB(GetOwner(pTarget), iLayer);

  if(!obj)
    obj = CreateObject(S24K,0,0,GetOwner(pTarget));

  obj->Set(pTarget,dwRGBa,iAlphaAdd,iFadeRate,bAdditive,iLayer);
  return obj;
}

global func GetScreenRGB(int iPlr, int iLayer)
{
  return FindObject2(Find_ID(S24K), Find_Func("SameLayer", iLayer), Find_Owner(iPlr));
}

public func SameLayer(int iLayer)
{
  return layer && layer == iLayer;
}

public func Set(object pTarget, int dwRGBa, int iAlphaAdd, int iFadeRate, bool bAdditive, int iLayer)
{
  target = pTarget;
  layer = iLayer;

  if(!target)
    SetVisibility(VIS_All);
  else
    SetAction("CursorCheck");

  //Additiv zeichnen?
  if(bAdditive)
    SetObjectBlitMode(GFX_BLIT_Additive);

  fade = iFadeRate;

  var a_save = a;
  SplitRGBaValue(dwRGBa,r,g,b,a);
  a = BoundBy(a_save-iAlphaAdd,0,255);

  if(!fade)
  {
    RemoveEffect("IntRGBFade",this);
    SetClrModulation(RGBa(r, g, b, a));
  }
  else
    if(!GetEffect("IntRGBFade",this))
      AddEffect("IntRGBFade",this,1,3,this);
}

/* Fade-Effekt */

public func FxIntRGBFadeStart(object pTarget, int iEffectNumber, int iTemp)
{
  return 1;
}

public func FxIntRGBFadeTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  a = BoundBy(a+fade,0,255);
  SetClrModulation(RGBa(r,g,b,a), pTarget);

  if(a <= 0)
    return -1;

  if (!GetPlayerName(GetOwner(pTarget)))
    return -1;
}

public func FxIntRGBFadeStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  RemoveObject();
}

public func SetAlpha(int iValue)
{
  a = BoundBy(iValue,0,255);
  SetClrModulation(RGBa(r,g,b,a));

  if(a >= 255) RemoveObject();
}

public func DoAlpha(int iValue, int iMin, int iMax)
{
  if(!iMax) iMax = 255;
  a = BoundBy(a-iValue,Max(iMin,0),Min(iMax,255));
  SetClrModulation(RGBa(r,g,b,a));

  if(a >= 255) RemoveObject();
}

func CursorCheck()
{
  var cursor = false;
  if(GetCursor(GetOwner()))
  {
    if(GetCursor(GetOwner()) == target)
      cursor = true;
    if(GetCursor(GetOwner())->~GetRealCursor() == target)
      cursor = true;
  }
  else
  {
    cursor = true;
  }
  if(cursor)
    SetVisibility(VIS_Owner);
  else
    SetVisibility(VIS_None);
}