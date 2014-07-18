/*-- Geruest --*/

#strict 2

local xpos, ypos, xlength, ylength;

public func ConstructionTime() 
{
  return (GetActMapVal("Length", "Construct") * GetActMapVal("Delay", "Construct"));
}

public func DestructionTime() 
{
  return (GetActMapVal("Length", "Deconstruct") * GetActMapVal("Delay", "Deconstruct"));
}

protected func StartConstruction(int iX, int iY, int iXLength, int iYLength)
{
  //erstmal verstecken 
  SetClrModulation(RGBa(255,255,255,255));
  xpos = iX;
  ypos = iY;
  xlength = iXLength;
  ylength = iYLength;
  if(ypos)
    Schedule("Construct()",ConstructionTime()* ypos);
  else
    Construct();
  return 1;
}

protected func Construct()
{
  //wieder sichtbar machen
  SetClrModulation(RGBa(255,255,255,0));
  //Aufbauaction setzen
  SetAction("Construct");
  return 1;
}

protected func StartDestruction()
{
  if(ylength-ypos)
    Schedule("SetAction(\"Deconstruct\")",DestructionTime()*(ylength-ypos));
  else
    SetAction("Deconstruct");
  return 1;
}

protected func Destruct()
{
  RemoveObject();
  return 1;
}
