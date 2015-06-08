/*-- Fundament --*/

#strict 2

local basement;

private func BasementID()	{return(BT01);}
private func BasementWidth()	{return(116);}

/*-- Fundament-Objekte --*/

/* Fertigstellung */

protected func Initialize()
{
  if(GetID() != BasementID() ) 
    return _inherited(...);
  SetAction("Consolidate");
  MoveOutClonks();
  return _inherited(...);
}

private func Consolidated()
{
  if(GetID() != BasementID() ) 
    return _inherited(...);
  
  // Das Fundament ist fertig und abgesunken: in den Hintergrund (fixieren)
  SetCategory(1);
  return _inherited(...);
}

public func Consolidate()
{
  return(SetCategory(1));
}

/* Schaden / Zerstörung */

protected func Damage(iChange, iByPlayer)
{
  //todo

  return _inherited(...);
}
  
/* Feststeckende Clonks und andere Objekte rauswerfen */

private func MoveOutClonks() 
{
  var pObj;
  if(GetID() != BasementID()) 
    return _inherited(...);
  while(pObj=FindObject(0, -BasementWidth()/2-10, -15, BasementWidth() + 20,25, OCF_Collectible | OCF_Grab | OCF_Alive,0,0, NoContainer(), pObj))
    if(!DefinitionCall(GetID(pObj),"IsBridge"))
      if (~GetCategory(pObj) & C4D_Structure)
        MoveOutClonk(pObj);
}
  
private func MoveOutClonk(object pObj) 
{
  if(GetID() != BasementID()) 
    return _inherited(...);
  while(Stuck(pObj) && Inside(GetY(pObj)-GetY(),-15,+5))
    SetPosition(GetX(pObj),GetY(pObj)-1,pObj);  
}
  
/*-- Gebäude-Objekte --*/

/* Fundament */

protected func Construction()
{
  if(GetID() == BasementID()) 
    return _inherited(...);
  // Nur wenn noch kein Fundament vorhanden: Damit bei Überladung von
  // Fundamentobjekten nicht Fundamente mehrfach erstellt werden.
  if(!basement) 
    basement = CreateObject(BasementID(), 0, 8, GetOwner());
  return _inherited(...);
}
  
protected func Destruction()
{
  if(GetID() == BasementID()) 
    return _inherited(...);
  if(basement) 
    RemoveObject(basement);
  return _inherited(...);
}

