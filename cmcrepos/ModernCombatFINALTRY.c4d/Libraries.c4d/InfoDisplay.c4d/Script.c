/*-- Infoanzeige --*/

#strict 2

local iTime;
local szDesc;
local iSize;
local highlight;


/* Initialisierung */

public func Initialize()
{
  iTime = 0;
  iSize = 15;
  szDesc = GetDesc(this);
  SetVisibility(VIS_Owner);
  SetPosition(-GetDefWidth(GetID())/2-120, -GetDefHeight(GetID())/2-42);
  SetClrModulation(RGBa(255,255,255,255));
  SetObjDrawTransform(10000/iSize,0,-GetDefWidth(GetID())*10000/(2*iSize),0,10000/iSize,-GetDefHeight(GetID())*10000/(2*iSize));

  //Highlight-Objekt erstellen
  highlight = CreateObject(HTLT, 0, 0, GetOwner());
  highlight->SetVisibility(VIS_Owner);
  SetPosition(-GetDefWidth(GetID())/2-40, -GetDefHeight(GetID())/2-25,highlight);
  SetClrModulation(RGBa(255,255,255,255), highlight);
  AddEffect("Highlight", highlight, 101, 1, this, 0, this);
  //Parallax
  Local(0) = 0;
  Local(1) = 0;
}

public func IsInfoObject()
{
  return true;
}

public func GetAchievementScore()
{
  return 0;
}

/* Highlight-Effekt */

public func FxHighlightStart(object target, int nr, temp, object ach)
{
  EffectVar(0, target, nr) = 255;	//Alphawert
  EffectVar(1, target, nr) = -5;	//Erhöhung/Verringerung des Alphawertes
  EffectVar(3, target, nr) = ach;	//Achievementobjekt
}

public func FxHighlightTimer(object target, int nr)
{
  //Erst mit dem Achievement starten
  if(!EffectVar(3, target, nr)->IsRunning())
    return true;

  SetClrModulation(RGBa(255, 255, 255, EffectVar(0, target, nr)), target);
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

protected func Fade()
{
  if(!iTime)
    for(var ach in FindObjects(Find_Func("IsInfoObject"), Find_Exclude(), Find_Owner(GetOwner())))
      if(ach->IsRunning())
        return;
  iTime++;
  if(iTime < 1*32)
  {
    SetClrModulation(RGBa(255,255,255,255-(iTime*8)));
    Desc(255-(iTime*8));
    return true;
  }
  if(iTime < 9*32)
  {
    SetClrModulation(RGBa(255,255,255,0));
    Desc(0);
    return true;
  }
  if(iTime < 13*32)
  {
    SetClrModulation(RGBa(255,255,255,(iTime-(9*32))*2));
    EffectVar(0, highlight, GetEffect("Highlight", highlight)) = (iTime-(9*32))*2;
    Desc((iTime-(9*32))*2);
    return true;
  }
  RemoveObject(highlight);
  return RemoveObject(this);
}

protected func Desc(int alpha)
{
  CustomMessage(Format("@<c %x>%s</c>", RGBa(255,255,255,255-alpha),szDesc), this, GetOwner(), (GetDefWidth(GetID())*10/(2*2*iSize)), (GetDefHeight(GetID())*10/(2*iSize))+115, 0, 0, 0, MSG_NoLinebreak);
  return true;
}

protected func IsRunning()
{
  return iTime > 0;
}