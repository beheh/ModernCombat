/*-- Flieﬂbandteil --*/

#strict 2

local father, x, y, speed, fCheckBottom;

public func IgnoreProductionLine()	{return true;}
public func Speed()			{return speed;}
public func SetSpeed(int iSpeed)	{return speed = iSpeed;}


/* Initialisierung */

func Initialize()
{
  //Voreinstellungen
  SetAction("Stop");
  x = GetX() - GetX(father);
  y = GetY() - GetY(father);
  speed = 3;
  return 1;
}

/* Position aktuell halten */

func CheckPosition()
{
  SetPosition(GetX(father)+x, GetY(father)+y);

  return;
}

func Push()
{
  var dir = (GetAction() == "Right") * 2 - 1;

  var objects = FindObjects(Find_OnLine(-20, -12, 20, -12), Find_Category(C4D_Living|C4D_Object|C4D_Vehicle), Find_Not(Find_Func("IgnoreProductionLine")));
  for(obj in objects)
    if(Inside(GetX(obj)-GetX(), -20, 20))
      SetPosition(GetX(obj)+Speed()*dir, GetY(obj), obj);
  
  if(fCheckBottom)
  {
    var objects = FindObjects(Find_OnLine(-20, 12, 20, 12), Find_Category(C4D_Living|C4D_Object|C4D_Vehicle), Find_Not(Find_Func("IgnoreProductionLine")));
    for(obj in objects)
      if(Inside(GetX(obj)-GetX(), -20, 20))
        SetPosition(GetX(obj)+Speed()*(-dir), GetY(obj), obj);
  }

  CheckEnds(dir);
  return;
}

func CheckEnds(dir)
{
  return;
}