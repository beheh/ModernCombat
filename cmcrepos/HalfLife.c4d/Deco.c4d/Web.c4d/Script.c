#strict 2

public func Initialize()
{
  if(!Random(2))
    SetGraphics("2");
  else
    SetGraphics();
}