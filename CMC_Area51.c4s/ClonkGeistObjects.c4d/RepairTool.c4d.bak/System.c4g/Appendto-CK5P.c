/*-- Neues Script --*/

#strict 2

#appendto CK5P

local dwcolor;

public func Initialize()
{
	dwcolor = RGBa(100, 50, 0, 50);
	return _inherited();
}

public func SetRopeColor(int dwClr) { return dwcolor = dwClr; }

private func Timer()
{
  var i, iAmount;
  // Partikel-Effekt
  while(i < GetPointNum() - 1)
  {
    var iAngle1 = Angle(GetPoint(i), GetPoint(i, 1), GetPoint(i + 1), GetPoint(i + 1, 1));
    var iAngle2 = iAngle1 - 180;
    var iX1 = GetPoint(i) - GetX() - Sin(iAngle1, 2);
    var iY1 = GetPoint(i, 1) - GetY() + Cos(iAngle1, 2);
    var iX2 = GetPoint(i + 1) - GetX() - Sin(iAngle2, 2);
    var iY2 = GetPoint(i + 1, 1) - GetY() + Cos(iAngle2, 2);
    iAmount += DrawParticleLine2("Rope", iX1, iY1, iX2, iY2, 5, 40, dwcolor);
    i++;
  }
}
