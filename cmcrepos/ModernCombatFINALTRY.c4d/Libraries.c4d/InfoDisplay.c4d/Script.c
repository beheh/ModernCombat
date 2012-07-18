/*-- Infoanzeige --*/

#strict 2

local szDesc;
local iSize;
local highlight;

public func IsInfoDisplay() { return true; }
public func GetHighlight() { return highlight; }

/* Initialisierung */

public func Initialize()
{
  iSize = 15;
  szDesc = GetDesc(this);

  SetVisibility(VIS_Owner);
  SetCategory(C4D_Parallax|C4D_Foreground|C4D_MouseIgnore|C4D_IgnoreFoW);
  SetPosition(-GetDefWidth(GetID())/2-150, -GetDefHeight(GetID())/2-42);
  SetClrModulation(RGBa(255,255,255,255));
  SetObjDrawTransform(10000/iSize,0,-GetDefWidth(GetID())*10000/(2*iSize),0,10000/iSize,-GetDefHeight(GetID())*10000/(2*iSize));

  //Highlight-Objekt erstellen
  highlight = CreateObject(IHLT, 0, 0, GetOwner());
  highlight->SetVisibility(VIS_Owner);
  SetObjectOrder(highlight);
  SetPosition(-GetDefWidth(GetID())/2-70, -GetDefHeight(GetID())/2-25,highlight);
  SetClrModulation(RGBa(255,255,255,255), highlight);
  AddEffect("IntHighlight", highlight, 101, 1, this, 0, this);
  AddEffect("IntFade", this, 101, 1, this, 0, this);

  //Parallax
  Local(0) = 0;
  Local(1) = 0;
}

public func SetCustomDesc(string szNewDesc)
{
  szDesc = szNewDesc;
}

public func SetHighlightColor(int dwColor)
{
  var nr = GetEffect("IntHighlight", highlight);
  if(!nr) return false;
  EffectVar(4, highlight, nr) = dwColor;
  return true;
}

/* Highlight-Effekt */

public func FxIntHighlightStart(object target, int nr, temp, object ach)
{
  EffectVar(0, target, nr) = 255;		//Alphawert
  EffectVar(1, target, nr) = -5;		//Erh�hung/Verringerung des Alphawertes
  EffectVar(3, target, nr) = ach;		//Achievementobjekt
  EffectVar(4, target, nr) = RGB(255,204,0);	//Standardfarbwert f�r Highlight
}

public func FxIntHighlightTimer(object target, int nr)
{
  //Erst mit dem Achievement starten
  var color = EffectVar(4, target, nr);
  if(!EffectVar(3, target, nr)->IsRunning())
    return true;
  SetClrModulation(RGBa(GetRGBaValue(color, 1), GetRGBaValue(color, 2), GetRGBaValue(color, 3), EffectVar(0, target, nr)), target);
  if(EffectVar(0, target, nr) <= 0)
    EffectVar(1, target, nr) = +5;
  else
  if(EffectVar(0, target, nr) >= 150)
    EffectVar(1, target, nr) = -5;

  //Rotation
  SetR(EffectVar(2, target, nr)++, target);

  EffectVar(0, target, nr) += EffectVar(1,target, nr);
  return true;
}

/* Ausblenden */

public func FxIntFadeStart(object target, int nr, temp)
{
  EffectVar(0, target, nr) = 0;
}

public func FxIntFadeTimer(object target, int nr)
{
  var iTime = EffectVar(0, target, nr);
  if(!iTime)
    for(var display in FindObjects(Find_Func("IsInfoDisplay"), Find_Exclude(target), Find_Owner(GetOwner(target))))
      if(display->IsRunning())
        return;

  EffectVar(0, target, nr)++;
  iTime = EffectVar(0, target, nr);

  if(iTime < 1*32)
  {
    SetClrModulation(RGBa(255,255,255,255-(iTime*8)), target);
    target->DrawDescription(255-(iTime*8));
    return true;
  }
  if(iTime < 9*32)
  {
    SetClrModulation(RGBa(255,255,255,0), target);
    target->DrawDescription(0);
    return true;
  }
  if(iTime < 13*32)
  {
    SetClrModulation(RGBa(255,255,255,(iTime-(9*32))*2), target);
    EffectVar(0, target->GetHighlight(), GetEffect("IntHighlight", target->GetHighlight())) = (iTime-(9*32))*2;
    target->DrawDescription((iTime-(9*32))*2);
    return true;
  }

  return -1;
}

public func FxIntFadeStop(object target, int nr) {
  RemoveObject(target);
}

protected func Destruction()
{
  if(highlight) RemoveObject(highlight);
}

public func DrawDescription(int alpha)
{
  CustomMessage(Format("@<c %x>%s</c>", RGBa(255,255,255,255-alpha),szDesc), this, GetOwner(), (GetDefWidth(GetID())*10/(2*2*iSize)), (GetDefHeight(GetID())*10/(2*iSize))+115, 0, 0, 0, MSG_NoLinebreak);
  return true;
}

protected func IsRunning()
{
  var nr = GetEffect("IntFade", this);
  if(!nr) return false;
  return EffectVar(0, this, nr) > 0;
}
