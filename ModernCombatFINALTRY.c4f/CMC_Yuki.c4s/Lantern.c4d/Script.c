/*-- Glühbirne - Grundobjekt --*/

#strict

local bulb;


public func Initialize()
{
  bulb = CreateObject(LAT2);
  bulb->Init(this(),50); 
  return(1);
}

public func Set(int Length)
{
  bulb->Set(Length);
}

public func Destruction()
{
  if(bulb)
    RemoveObject(bulb);
}