/*-- Marker --*/

#strict

local timer;

protected func Initialize()
{
  // unsichtbar f�r alle anderen Spieler
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
  // Timer herunterz�hlen
  --timer;
  if (!timer) return(RemoveObject());
}

