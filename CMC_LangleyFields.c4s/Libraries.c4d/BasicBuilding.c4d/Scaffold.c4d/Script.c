/*-- Gerüst --*/

#strict 2

public func ConstructionTime() {return GetActMapVal("Length", "Construct") * GetActMapVal("Delay", "Construct");}
public func DestructionTime() {return GetActMapVal("Length", "Deconstruct") * GetActMapVal("Delay", "Deconstruct");}

protected func StartConstruction(int iX, int iY)
{
  return 1;
}

protected func StartDestruction(int iX, int iY)
{
  return 1;
}