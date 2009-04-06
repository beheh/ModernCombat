/*-- Schlauchboot (inaktiv) --*/

#strict

protected func Initialize()
{
  SetDir(Random(2));
  SetAction("OnLand");
}

private func Floating()
{
  // Aufrichten
  SetRDir(BoundBy(GetR(),-20,+20)/-5);
  // Nicht mehr im Wasser
  if (!(GetOCF()&OCF_InLiquid())) 
    return(SetAction("OnLand"));
  // Segeln, wenn aufgerichtet
  if (Inside(GetR(),-5,+5))
  {
    ChangeDef(INFS);
    SetAction("LowerSail");
    SetComDir(COMD_None);
    this()->~SetUp();
  }
}

public func ControlDigDouble(object pCaller)
{
  var pack = CreateObject(INFP,0,0,GetOwner(pCaller));
  if(pCaller)
    Collect(pack,pCaller); 
    
  RemoveObject();
}
