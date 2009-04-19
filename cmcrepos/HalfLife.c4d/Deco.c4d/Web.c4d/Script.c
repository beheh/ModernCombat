#strict 2

public func Initialize()
{
  SetAction("Be");

  if(!Random(2))
    SetGraphics("2");
  else
    SetGraphics();
}