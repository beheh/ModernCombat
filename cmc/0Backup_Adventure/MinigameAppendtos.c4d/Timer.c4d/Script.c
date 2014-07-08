/*-- Zeitanzeige --*/

#strict 2

local start_time;

public func Initialize() 
{
  SetVisibility(VIS_All);
  SetPosition(50, 100, this);
  return 1;
}

protected func Time(int iTime) 
{
	iTime /= 35;
  Message("@<c %x>%.2d:%.2d", this, RGBa(255, 255, 0), iTime / 60, iTime % 60);
}
