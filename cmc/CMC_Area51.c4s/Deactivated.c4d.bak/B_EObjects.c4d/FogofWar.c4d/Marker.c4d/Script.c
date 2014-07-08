/*-- Marker --*/

#strict

local timer;

protected func Initialize()
{
  // unsichtbar für alle anderen Spieler
  SetVisibility(VIS_Owner());
  SetAction("Blink");
}

public func SetTimer(value)
{
  // setzt den Timer
  timer = value;
}

private func Step()
{
  // Timer herunterzählen
  --timer;
  if (!timer) return(RemoveObject());
}

