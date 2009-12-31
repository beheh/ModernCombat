/* ScreenRGB */
#strict 2

static const SR4K_LayerRedHurt = 1;
static const SR4K_LayerSmoke = 2;

local r,g,b,a;
local fadein,fadeout,duration,target,layer;

public func Initialize()
{
  SetObjDrawTransform(1000*1000,0,0,0,1000*1000,0,0,0);

  SetPosition(0,0);

  SetClrModulation (RGBa(0,0,0,255));

  return 1;
}

global func ScreenRGB(object pTarget, int dwRGBa, int iFadeIn, int iDuration, int iFadeOut, bool bAdditive, int iLayer)
{
  var obj;
  if(iLayer)
    obj = FindObject2(Find_ID(SR4K),Find_Owner(GetOwner(pTarget)),Find_Func("SameLayer",iLayer));
    
  if(!obj)
    obj = CreateObject(SR4K,0,0,GetOwner(pTarget));
    
  obj->Set(pTarget,dwRGBa,iFadeIn,iDuration,iFadeOut,bAdditive,iLayer);
  return obj;
}

public func SameLayer(int iLayer)
{
  if(!layer) if(iLayer == layer) return true;
}

public func Set(object pTarget, int dwRGBa, int iFadeIn, int iDuration, int iFadeOut, bool bAdditive, int iLayer)
{
  target = pTarget;
  layer = iLayer;

  if(!target)
    SetVisibility(VIS_All);
  else
    SetAction("CursorCheck");

  if(bAdditive)//Additiv zeichnen?
    SetObjectBlitMode(GFX_BLIT_Additive);

  if(!GetRGBaValue(dwRGBa))//Kein Alphakanal? :o
    dwRGBa = SetRGBaValue (dwRGBa,1);
    
  fadein = Max(fadein,iFadeIn);
  fadeout = Max(fadeout,iFadeOut);
  duration += iDuration;

  var a_save = a;
  SplitRGBaValue(dwRGBa,r,g,b,a);
  if(a_save)
    a = Min(a,a_save);
  
  SetClrModulation (RGBa(r,g,b,255));

  if(!fadein)
    fadein = 1;

  if(!fadeout)
    fadeout = 1;
    
  var effect = GetEffect("IntRGBFade",this);
  if(effect)
    EffectVar(0,this,effect) = 0;//:S
  else
    AddEffect("IntRGBFade",this,25,1,this);
}

///Fade-Effekt

/* -Effektvariablen-

0: Animationsstatus
   +1: Einfaden
    0: Da sein.
   -1: Ausfaden.
   
1: Alpha (Der aktuelle Alpha-Wert.)

2: Effektzeit zu der das Einfaden abgeschlossen wurde. (Für duration wichtig.)

*/

public func FxIntRGBFadeStart(object pTarget, int iEffectNumber, int iTemp)
{
  var state = +1;
  var alpha = 255;
  
  if(fadein < 0)//Kein Einfaden?
    state = 0;
  if(duration < 0)//Sofort Ausfaden?
    state = -1;
    
  if(state < +1)//Nicht Einfaden?
    alpha = a;
    
  EffectVar(0,pTarget,iEffectNumber) = state;
  EffectVar(1,pTarget,iEffectNumber) = alpha;
  
  return 1;
}

public func FxIntRGBFadeTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var state = EffectVar(0,pTarget,iEffectNumber);
  
  if(state == +1)//Einfaden.
  {
    EffectVar(1,pTarget,iEffectNumber) = BoundBy( 255 - (iEffectTime*(255-a)/fadein) ,0,255);
    pTarget->SetClrModulation(RGBa(r,g,b,EffectVar(1,pTarget,iEffectNumber)));
    
    if(EffectVar(1,pTarget,iEffectNumber) <= a)//Fertig mit Einfaden?
    {
      if(duration < 0)
        EffectVar(0,pTarget,iEffectNumber) = -1;
      else
      {
        EffectVar(0,pTarget,iEffectNumber) =  0;
        EffectVar(2,pTarget,iEffectNumber) = iEffectTime;//Aktuelle Zeit speichern.
      }
    }
  }
  else if(state == 0)//Da sein.
  {
    if(iEffectTime - EffectVar(3,pTarget,iEffectNumber) >= duration)
    {
      EffectVar(0,pTarget,iEffectNumber) = -1;
      EffectVar(2,pTarget,iEffectNumber) = iEffectTime;
    }
    return 0;
  }
  else if(state == -1)//Ausfaden.
  {
    EffectVar(1,pTarget,iEffectNumber) = BoundBy( a + ((iEffectTime-EffectVar(2,pTarget,iEffectNumber))*(255-a)/fadeout) ,0,255);
    pTarget->SetClrModulation(RGBa(r,g,b,EffectVar(1,pTarget,iEffectNumber)));
    
    if(EffectVar(1,pTarget,iEffectNumber) >= 255)//Fertig mit Ausfaden?
      return -1;//Weg damit!
  }

  return 0;
}

public func FxIntRGBFadeStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  RemoveObject();
  return 0;
}


public func Fade(iTime)
{
  if(!iTime) iTime = fadeout;
  fadeout = iTime;
  
  var effect = GetEffect("IntRGBFade",this);
  if(effect) EffectVar(0,this,effect) = -1;
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
