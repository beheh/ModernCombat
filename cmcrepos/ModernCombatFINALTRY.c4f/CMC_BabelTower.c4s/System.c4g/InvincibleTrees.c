/*-- Baum --*/

//B�ume sind unzerst�rbar.

#strict 2
#appendto TREE

local damaged;

public func ChopDown()		{}
protected func Incineration()	{Extinguish();}
public func OnDmg()		{if (GetID() != BSH2) return 100;}


/* Initialisierung */

protected func Initialize()
{
  //Zuf�llige Richtung
  if(Random(2)) SetDir(DIR_Right);

  //Zuf�llige F�rbung
  if(Random(4))
  {
    var color = RandomX(200,240);
    SetClrModulation(RGB(color,color,color));
  }

  //Zuf�llige Drehung nach Erdoberfl�che
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