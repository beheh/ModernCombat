/*-- Baum --*/

//Bäume sind unzerstörbar.

#strict 2
#appendto TREE

local damaged;

public func ChopDown()		{}
protected func Incineration()	{Extinguish();}
public func OnDmg()		{if (GetID() != BSH2) return 100;}


/* Initialisierung */

protected func Initialize()
{
  //Zufällige Richtung
  if(Random(2)) SetDir(DIR_Right);

  //Zufällige Färbung
  if(Random(4))
  {
    var color = RandomX(200,240);
    SetClrModulation(RGB(color,color,color));
  }

  //Zufällige Drehung nach Erdoberfläche
  if(Random(10))
  {
    var x_off = 18 * GetCon() / 100;
    var y_off = 15 * GetCon() / 100;
    var slope = GetSolidOffset(-x_off, y_off) - GetSolidOffset(x_off, y_off);
    SetR(slope);
  }
}

private func GetSolidOffset(int x, int y)
{
  var i;
  for (i = -15; GBackSolid(x, y - i) && (i < 15); i++);
  return(i);
}

/* Keine Fortpflanzung */

private func Seed()
{}