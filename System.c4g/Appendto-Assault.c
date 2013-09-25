/*-- Neues Script --*/

#strict 2

#appendto GASS

public func IsAttackable(object pTarget, int iNr)
{
	//Ziel an der Reihe?
  var iTarget = GetIndexOf(pTarget, aTargets[iDefender]),
  iNext = GetNextTarget();

  var fConnected = GetType(Connected[iNext]) == C4V_Array;

  var bar = EffectVar(0, pTarget, iNr);

  if(iTarget != iNext && (fConnected && GetIndexOf(iTarget, Connected[iNext]) == -1 || !fConnected))
  {
    bar->Update(0, true);
    return true;
  }

  return false;
}

public func OnPlantingComplete(array aAttackers, object pTarget)
{
  //Ticketabzug-Timer zur�cksetzen
  var effect = GetEffect("TicketSubtraction", this);
  if(effect)
    EffectCall(this, effect, "Reset");

  //Eventnachricht: Ladung plaziert, verteidigen
  TeamEventInfo(iAttacker, Format("$TargetArmedAttacker$", GetName(pTarget)), SM16, 0, 0, 0, "Info_Event.ogg");
  //Eventnachricht: Ladung plaziert, entsch�rfen
  TeamEventInfo(iDefender, Format("$TargetArmedDefender$", GetName(pTarget)), SM17, 0, 0, 0, "Info_Event.ogg");
  
  return true;
}

public func OnDefusingComplete(array aDefenders, object pTarget)
{
  //Eventnachricht: Ladung entsch�rft, neue setzen
  TeamEventInfo(iAttacker, Format("$TargetDefusedAttacker$", GetName(pTarget)), SM17, 0, 0, 0, "Info_Event.ogg");
  //Eventnachricht: Ladung entsch�rft
  TeamEventInfo(iDefender, Format("$TargetDefusedDefender$", GetName(pTarget)), SM16, 0, 0, 0, "Info_Event.ogg");

	return true;
}

public func OnBombTimer(object pTarget)
{
	//Ticketabzug-Timer zur�cksetzen
  var effect = GetEffect("TicketSubtraction", this);
  if(effect)
    EffectCall(this, effect, "Reset");

	return true;
}
