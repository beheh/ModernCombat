/*-- ScreenRGB --*/

#strict 2

local r,g,b,a;
local target,targetset,layer,fade;

static const SR4K_LayerSmoke		= 1;
static const SR4K_LayerLight		= 2;
static const SR4K_LayerDamage		= 3;
static const SR4K_LayerFakeDeath	= 4;
static const SR4K_LayerBorder		= 5;
static const SR4K_LayerMedicament	= 6;
static const SR4K_LayerLensflare	= 7;

public func GetTargetCursor()		{}
public func IsOverlayScreen()		{return true;}
public func NoWarp()			{return true;}
public func IsHUD()			{return true;}
public func GetAlpha()			{return a;}


/* Initialisierung */

public func Initialize()
{
  SetObjDrawTransform(2000 * Max(LandscapeWidth(), 4000) / GetDefWidth(GetID()), 0, 0, 0, 2000 * Max(LandscapeHeight(), 2000) / GetDefHeight(GetID()));
  SetPosition();
  SetClrModulation (RGBa(0, 0, 0, a = 255));
}

global func GetPlayerViewAlpha(int iPlr)
{
  var a = 255, b;
  var screens = FindObjects(Find_ID(S24K), Find_Owner(iPlr));
  for(var screen in screens)
  {
    b = screen->GetAlpha();
    if(b < a) a = b;

  }
  return a;
}

global func ScreenRGB(object pTarget, int dwRGBa, int iAlphaAdd, int iFadeRate, bool bAdditive, int iLayer, int iMin)
{
  var obj;
  if(iLayer)
    obj = GetScreenRGB(GetOwner(pTarget), iLayer, pTarget);

  if(!obj && pTarget && GetCategory(pTarget) & C4D_Living && !(GetOCF(pTarget) & OCF_Alive))
    return false;

  if(!obj)
    obj = CreateObject(S24K,0,0,GetOwner(pTarget));

  obj->Set(pTarget,dwRGBa,iAlphaAdd,iFadeRate,bAdditive,iLayer,iMin);
  return obj;
}

global func GetScreenRGB(int iPlr, int iLayer, pTarget)
{
  return FindObject2(Find_ID(S24K), Find_Func("SameLayer", iLayer, pTarget), Find_Owner(iPlr));
}

public func SameLayer(int iLayer, object pTarget)
{
  var targetcheck = true;
  if(pTarget)
    targetcheck = (target == pTarget);

  return (layer && layer == iLayer) && targetcheck;
}

public func Set(object pTarget, int dwRGBa, int iAlphaAdd, int iFadeRate, bool bAdditive, int iLayer, int iMin)
{
  target = pTarget;
  layer = iLayer;

  if(target)
    targetset = true;

  if(target && GetCategory(target) & C4D_Living && !(GetOCF(target) & OCF_Alive))
    return RemoveObject();

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
  if(a_save < 255 || !a)
    a = BoundBy(a_save-iAlphaAdd,iMin,255);

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

  if(target && GetCategory(target) & C4D_Living && !(GetOCF(target) & OCF_Alive))
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

public func GetAlpha()
{
  return GetRGBaValue(GetClrModulation(), 0);
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
  if(targetset && !target)
    return RemoveObject();

  if(GetOwner(target) != GetOwner())
    SetOwner(GetOwner(target));

  var cursor = false;
  var targetcursor = target;
  if(target && target->~GetRealCursor())
    targetcursor = target->~GetRealCursor();

  if(GetCursor(GetOwner()))
  {
    if(GetCursor(GetOwner()) == targetcursor)
      cursor = true;
    if(GetCursor(GetOwner())->~GetRealCursor() == targetcursor)
      cursor = true;
  }
  if(cursor)
    SetVisibility(VIS_Owner);
  else
    SetVisibility(VIS_None);
}