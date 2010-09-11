/*-- On Screen Coloration --*/

#strict 2

local R,G,B,a;
local Trans;
local iADir;

public func IsHUD()	{return true;}


/* Initialisierung */

public func Initialize()
{
  SetObjDrawTransform(1000*1000,0,0,0,1000*1000,0,0,0);
  SetPosition(0,0);
  SetClrModulation (RGBa(0,0,0,255));
  return true;
}

public func Set(int iPlayer, int dwRGBa, bool bAdditive,int iTrans)
{
  SetOwner(iPlayer);
  if(iPlayer)
    SetVisibility(VIS_Owner);
  else
    SetVisibility(VIS_All);

  if(bAdditive)// Additiv zeichnen?
    SetObjectBlitMode(GFX_BLIT_Additive);

  SplitRGBaValue(dwRGBa,R,G,B,a);
  a=255-a;
  if(iTrans)
    Trans=1000-iTrans*10;
}

public func SetClr(int dwRGBa)
{
  SplitRGBaValue(dwRGBa,R,G,B,a);
  a=255-a;
}

// In Promille
public func SetTrans(alpha)
{
  Trans=alpha;
}

public func GetTrans()
{
  return Trans;
}

// In Promille pro Frame
public func SetFading(dir)
{
  iADir=dir;
}

public func Timer()
{
  Trans+=iADir;
  if(Trans>1000)
  {
    iADir=0;
    Trans=1000;
  }
  if(Trans<1)
  {
    iADir=0;
    Trans=1;
  }
  SetClrModulation(RGBa(R,G,B,255-(a*Trans/1000)));
}