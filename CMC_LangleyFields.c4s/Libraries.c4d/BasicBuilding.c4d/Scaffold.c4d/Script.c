/*-- Gerüst --*/

#strict 2

local xpos, ypos, xlength, ylength;


/* Zeitgebung */

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
  //Anfangs unsichtbar
  SetClrModulation(RGBa(255,255,255,255));

  //Position setzen
  xpos = iX;
  ypos = iY;
  xlength = iXLength;
  ylength = iYLength;

  //Animation (verzögert) starten
  if(ypos)
    Schedule("Construct()",ConstructionTime()* ypos);
  else
    Construct();
  return 1;
}

protected func Construct()
{
  //Sichtbar machen
  SetClrModulation(RGBa(255,255,255,0));
  //Aktion setzen
  SetAction("Construct");
  return 1;
}

/* Entfernung */

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