/*-- Respawner --*/

#strict

local items,target;

func Initialize()
{
  items = [];
  target = 0;
  SetOwner(NO_OWNER);
  //SetVisibility(VIS_God);
  return(1);
}

func Set(int iFrames, array aItems)
{
  if(!iFrames) iFrames = 35*30;
  SetVisibility(VIS_God);
  items = aItems;//:o *copy*
  RemoveEffect("IntRespawn",this);
  AddEffect("IntRespawn",this(),25,iFrames,this());
  Spawn();
}

func Spawn()
{
  var crate = CreateObject(H2IC,0,0,-1);
  for(var item in items)
    crate->CreateContents(item);
  
  crate->FadeIn4K(10);
  target = crate;
}

public func FxIntRespawnTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!target || (ObjectDistance(target,this) > 50))
    Spawn();
}