/*-- Riss --*/

#strict 2

local graphics;

func Initialize()
{
  graphics = 0;
}

func Set(int i)
{
  graphics = i;
  SetGraphics(Format("%d",i));
}

public func Serialize(array& extra)
{
  if(graphics)
	  extra[GetLength(extra)] = Format("Set(%d)", graphics);
}