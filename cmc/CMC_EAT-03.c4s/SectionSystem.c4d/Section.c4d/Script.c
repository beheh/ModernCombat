/*-- Sektionskontrolle --*/

#strict 2

static iScript;
static iRespawnX;
static iRespawnY;

public func IsSection()	{return true;} //Sektionscallback


/* Initialisierung */

public func Initialize()
{
  iScript = 0;
  iRespawnX = 0;
  iRespawnY = 0;
  this->~SetupSection();
  AddEffect("SectionCount", this, 25, 10, this);
}

global func FxSectionCountTimer(pTarget)
{
  var iScript = pTarget->GetScript();
  if(iScript < 0) return;
  pTarget->SectionGoto(iScript+1);
  ObjectCall(pTarget, Format("Script%d", iScript));
  return FX_OK;
}

public func SectionGoto(int iTo)
{
  iScript = iTo;
  return true;
}

public func GetScript()
{
  return iScript;
}

public func SectionStop()
{
  iScript = -1;
  return true;
}

public func SectionUnload()
{
  RemoveObject();
}

public func Relaunch(object pClonk)
{
  pClonk->SetPosition(AbsX(iRespawnX), AbsY(iRespawnY));
  this->~OnRespawn(pClonk);	
}

public func SetRespawnPosition(int iX, int iY)
{
  iRespawnX = iX;
  iRespawnY = iY;
  return true;
}