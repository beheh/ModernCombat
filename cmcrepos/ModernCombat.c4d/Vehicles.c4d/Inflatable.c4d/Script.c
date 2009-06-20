/*-- Schlauchboot (inaktiv) --*/

#strict

protected func Initialize()
{
  SetDir(Random(2));
  SetAction("OnLand");
}

func Incineration()
{
 Sound("OutOfAir.ogg");
 CastParticles("XSpark", 10, 40, RandomX(-38,38), RandomX(-2,3), 50, 0, RGB(190,85,20));
 FadeOut();
 ChangeDef(INFB);
 
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
    this()->~SetUp();
  }
}

public func ControlDigDouble(object pCaller)
{
  ChangeDef(INFP);
  this->~Initialize();
  SetCategory(GetCategory(0,INFP));
  if(pCaller)
    Collect(this,pCaller); 
}
