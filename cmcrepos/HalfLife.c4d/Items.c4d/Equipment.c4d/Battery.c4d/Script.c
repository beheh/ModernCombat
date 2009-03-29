#strict

public func RejectEntrance(object pObj)
{
  if(pObj->GetAlive())
  {
    var hev = pObj->~HasGear(0,HEVS);
    if(!hev) return(1);
    if(hev->IsCharged()) return(1);
  
    hev->DoCharge(20);
    Sound("H2BT_Pickup.ogg");
    ScreenRGB(pObj,RGBa(0,128,255,192),0,0,35,true);
    RemoveObject();
  }
  return(1);
}