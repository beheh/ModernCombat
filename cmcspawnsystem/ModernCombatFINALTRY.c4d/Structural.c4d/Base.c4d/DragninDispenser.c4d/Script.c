/*-- Dragninautomat --*/

#strict 2

local packet;

public func IsMachine()	{return 1;}


public func RejectCollect(id idObj, object pObj)
{
  //Objekt kein EHP?
  if(idObj != FAPK)
  {
    Sound("BKHK_SwitchFail.ogg", false, pObj, 100, GetOwner(pObj)+1);
    return PlayerMessage(GetOwner(pObj), "$EnterEHP$", this);
  }
  //Bereits besetzt?
  if(packet)
  {
    Sound("BKHK_SwitchFail.ogg", false, pObj, 100, GetOwner(pObj)+1);
    return PlayerMessage(GetOwner(pObj), "$AlreadyCharging$", this);
  }
  //Schon voll?
  if(pObj->~GetPackPoints() >= pObj->~MaxPoints())
  {
    Sound("BKHK_SwitchFail.ogg", false, pObj, 100, GetOwner(pObj)+1);
    return PlayerMessage(GetOwner(pObj), "$NoChargeNeeded$", this);
  }
  //Nicht genug Geld?
  if(GetWealth(GetOwner(pObj)) < GetValue(0, FAPK) - GetValue(pObj))
  {
    Sound("BKHK_SwitchFail.ogg", false, pObj, 100, GetOwner(pObj)+1);
    return PlayerMessage(GetOwner(pObj), Format("$NotEnoughMoney$",GetValue(0, FAPK) - GetValue(pObj)), this);
  }
}

/* Aufladung */

public func Collection(pEHP)
{
  packet = pEHP;
  AddEffect("ChargePacket", packet, 1, 1, this);
  DoWealth(GetOwner(pEHP), GetValue(pEHP) - GetValue(0, FAPK));
  SetAction("Load");
}

public func FxChargePacketTimer(pTarget, iNo, iTime)
{
  if(pTarget->~GetPackPoints() >= pTarget->~MaxPoints())
  {
    Exit(pTarget, -3, 5);
    SetAction("Idle");
    packet = 0;
    return -1;
  }
  pTarget->~DoPackPoints(1);
  CreateParticle("FapLight", -2, +2, 0, 0, 30, pTarget->~Fill2Color());
}