/*-- Neues Script --*/

#strict 2

#appendto ATWN

private func Check()
{
  //Positionsänderungen ermitteln
  lastX = lastXTemp;
  lastY = lastYTemp;
  lastXTemp = GetX();
  lastYTemp = GetY();

  //Wegwurf wenn Rakete abgefeuert und/oder explodiert
  if(fired)
    if(!pRocket || !guided)
      if(Contained() && Contained()->~IsClonk())
      {
      	ThrowAway();
      }

  if(!Contained() || Contents(0, Contained()) != this || !Contained()->~IsClonk()) return;

  //Sicht auf existierende Rakete setzen
  if(Contained()->~IsAiming())
    if(pRocket && pRocket->Guideable())
      SetPlrView(GetOwner(Contained()), pRocket);
}
