/*-- Feinderkennung --*/

//Erweitert die Feinderkennung für CMC.

#strict 2


global func CheckEnemy(object pObj, object pObj2, bool findEnemy, int iX, int iY) 
{
  if(!pObj)
    pObj = this;

  if(!pObj)
    return;

  if(!pObj2)
    pObj2 = this;

  if(!pObj2)
    return;

  if(pObj == this && pObj2 == this)
    return;

  //Tote Lebewesen nicht treffen
  if(GetCategory(pObj) & C4D_Living)
    if(!GetAlive(pObj))
      return;

  if(GetCategory(pObj2) & C4D_Living)
    if(!GetAlive(pObj2))
      return;

  //Besitzer ermitteln
  var own1 = GetOwner(pObj);
  var own2 = GetOwner(pObj2);

  //Feinde treffen
  if(Hostile(own1, own2))
    return true;

  //Eines der Objekte  hat keinen Besitzer
  var noown1 = (own1 == NO_OWNER);
  var noown2 = (own2 == NO_OWNER);

  var team1 = GetTeam(pObj);
  var team2 = GetTeam(pObj2);

  if(noown1 || noown2)

  //Im selben Team: Nicht treffen oder finden (null ist kein Team)
  if((team1 == team2) && (team1 || team2)) {}
  //Beide ohne Team/Besitzer: nicht treffen oder finden
  else 
    if(!(team1 || team2) && noown1 && noown2) {}
    //Ansonsten (Sind in unterschiedlichen Teams bzw. bei keinem Team haben beide keinen Besitzer)
    else
    {
      //Bei Trefferchecks treffen
      if(!findEnemy)
        return true;

      //Bei Feindchecks anvisieren wenn ein Objekt eine Bedrohung
      if(pObj->~IsThreat())
        return true;
    }
  //Bei Trefferchecks treffen wenn Friendly Fire aktiviert und die Distanz groß genug ist
  if(FriendlyFire() && !findEnemy)
    if(!(pObj->~IgnoreFriendlyFire()) && !(pObj2->~IgnoreFriendlyFire()) && (Distance(GetX(pObj),GetY(pObj),iX,iY) > 50))
      return true;

  return false;
}