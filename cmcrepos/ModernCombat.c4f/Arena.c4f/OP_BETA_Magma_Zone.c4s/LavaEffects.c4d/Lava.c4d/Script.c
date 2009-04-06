/*--- Lava ---*/

#strict

local mat;

protected func Initialize()
{
  SetAction("Evaporate");
  Set(RandomX(-10,+50));
  return(1);
}

public func Set(int iSize)
{
  iSize = Max(iSize,-20);
  SetCon(100+iSize);
}
  
protected func HitLiquid()
{
  RemoveObject();
}
  
protected func Hit(int iXDir, int iYDir)
{
  var size = (GetCon()-80)/2;
  for(var obj in FindObjects(Find_AtPoint(GetX(),GetY()), Find_OCF(OCF_Alive)))
  {
    obj->Fling(GetXDir(obj)+iXDir/2,GetYDir(obj)+iYDir/2);
    Incinerate(obj);
    if(obj)
      DoDmg(size,DMG_Fire,obj);
  }
  CastParticles ("LavaGlow3", 8,size*2/3, 0, -4, size*2, size*4);
  Sound("Pshshsh",0,0,50);
  RemoveObject();
  return(1);
}

private func SmokeTrail()
{
  var speed = Distance(GetXDir(),GetYDir());
  if(speed > 15)
    SetR(Angle(0,0,GetXDir(),GetYDir()));
  
  CastParticles ("LavaGlow2", 1 ,0, 0, 0, speed*3/4, speed*3/4+10);
  
  //if (GetActTime() > 500) Hit();
  return(1);
}