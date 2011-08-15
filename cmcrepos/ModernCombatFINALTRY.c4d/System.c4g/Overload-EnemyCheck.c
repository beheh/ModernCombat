/*-- Treffer und Gegnerchecks --*/

//Verbesserte Freund/Feinderkennung.

#strict 2


global func CheckEnemy(object pObj, object pObj2, bool findEnemy)
{
  if(!pObj) return false;

  if(!pObj2)
    pObj2 = this;
  if(!pObj2) return false;

  //Tote Lebewesen nicht treffen
  if(GetCategory(pObj) & C4D_Living)
    if(!GetAlive(pObj))
      return false;

  if(GetCategory(pObj2) & C4D_Living)
    if(!GetAlive(pObj2))
      return false;

  var own1 = GetOwner(pObj);
  var own2 = GetOwner(pObj2);

  //Feinde treffen
  if(Hostile(own1, own2))
    return true;

  if(own1 == NO_OWNER || own2 == NO_OWNER)
    if(GetCategory(pObj) & C4D_Vehicle)
      return false;

  //Eines der Objekte  hat keinen Besitzer
  var noown1 = (own1 == NO_OWNER);
  var noown2 = (own2 == NO_OWNER);

  var team1 = GetTeam(pObj);
  var team2 = GetTeam(pObj2);

  if(noown1 || noown2)
  {
    //Im selben Team: nicht treffen/finden
    //0 ist kein Team
    if( (team1 == team2) && (team1 || team2)) {}

    //Beide haben kein Team aber beide haben keinen Besitzer: nicht treffen/finden
    else if( !(team1 || team2) && noown1 && noown2) {}

    //Ansonsten (Sind sie in unterschiedlichen Teams bzw. bei keinem Team haben sie beide keinen Besitzer)
    else
    {
      //Bei Trefferchecks, treffen
      if(!findEnemy)
        return true;

      //Bei Feindchecks, anvisieren wenn ein Objekt eine Bedrohung
      if(pObj->~IsThreat())
        return true;
    }
  }
		
  //Bei Trefferchecks treffen wenn FF aktiviert ist
  if(FriendlyFire() && !findEnemy)
  {
    if(!(pObj->~IgnoreFriendlyFire()) && !(pObj2->~IgnoreFriendlyFire()))
      return true;
  }

  return false;
}