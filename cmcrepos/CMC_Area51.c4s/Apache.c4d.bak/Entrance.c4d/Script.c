/*-- Apache-Eingang --*/

#strict 2

local pApache;
local iXOffset, iYOffset;


func SetApache(object tempApache)
{
  pApache = tempApache;
  SetEntrance(true);
}

func SetOffset(int iX, int iY)
{
  iXOffset = iX;
  iYOffset = iY;
}

func Collection2(object pUser)
{
  if(!Enter(pApache, pUser)) Exit(pUser);
}

/* Timer */

func Timer()
{
  //Kein Host-Apache? Verschwinden
  if(!pApache) 
    RemoveObject(this, true);
  else
  {
    //Position anpassen
    SetPosition(GetX(pApache) + iXOffset, GetY(pApache) + iYOffset, this, false);
    SetXDir();
    SetYDir();
  }
}