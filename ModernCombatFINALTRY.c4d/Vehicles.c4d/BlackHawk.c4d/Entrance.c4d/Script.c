/*-- Eingang --*/

#strict 2

local pHelicopter;
local iXOffset, iYOffset;

public func IsHelicopterEntrance() { return true; }


func SetHelicopter(object tempHelicopter)
{
  pHelicopter = tempHelicopter;
  SetEntrance(true);
}

func SetOffset(int iX, int iY)
{
  iXOffset = iX;
  iYOffset = iY;
}

func Collection2(object pUser)
{
  var flag;
  if(pHelicopter->~IsHelicopter())
    if(flag = FindObject2(Find_ID(FLA2), Find_ActionTarget(pUser)))
      flag->~DropFlag();

  if(!Enter(pHelicopter, pUser)) Exit(pUser);
}

/* Timer */

func Timer()
{
  //Kein Host-Helikopter? Verschwinden
  if(!pHelicopter) 
    RemoveObject(this, true);
  else
  {
    //Position anpassen
    SetPosition(GetX(pHelicopter) + iXOffset, GetY(pHelicopter) + iYOffset, this, false);
    SetXDir();
    SetYDir();
  }
}
