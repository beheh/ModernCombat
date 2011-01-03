/*-- Dragninautomat --*/

#strict 2

local packet;

public func IsMachine()	{return 1;}


public func RejectCollect(id idObj, object pObj)
{
  //Objekt kein EHP?
  if(idObj != FAPK)
    return PlayerMessage(GetOwner(pObj), "$EnterEHP$", this);
  //Bereits besetzt?
  if(packet)
    return PlayerMessage(GetOwner(pObj), "$AlreadyCharging$", this);
  //Schon voll?
  if(pObj->GetPackPoints() >= 150)
    return PlayerMessage(GetOwner(pObj), "$NoChargeNeeded$", this);
  //Nicht genug Geld?
  if(GetWealth(GetOwner(pObj)) < GetValue(0,FAPK) - GetValue(pObj))
    return PlayerMessage(GetOwner(pObj), "$NotEnoughFunds$", this);
}

/* Aufladung */

public func Collection(pEHP)
{
  packet = pEHP;
  AddEffect("ChargePacket",packet,1,1,this);
  DoWealth(GetOwner(pEHP),-(GetValue(0,FAPK) - GetValue(pEHP)));
  SetAction("Load");
}

public func FxChargePacketTimer(pTarget, iNo, iTime)
{
  if(pTarget->GetPackPoints() >= 150)
  {
    Exit(pTarget,-3,5);
    SetAction("Idle");
    packet = 0;
    return -1;
  }
  pTarget->DoPackPoints(1);
  CreateParticle("FapLight", -2, +2, 0, 0, 30, RGBa(BoundBy(InvertA1(255 * (pTarget->GetPackPoints()) / 150, 255) + 10, 0, 255), 255 * (pTarget->GetPackPoints()) / 150));
}