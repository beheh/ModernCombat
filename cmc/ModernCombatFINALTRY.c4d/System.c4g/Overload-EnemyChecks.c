#strict 2

// Wird für Trefferchecks benutzt
// Checkt ob pObj von pObj2 von der Verfeindung her getroffen werden kann
// berücksichtigt Friendly Fire (nun ab einer bestimmten Distanz)
// tote Feinde gelten hier als keine Feinde
global func CheckEnemy(object pObj, object pObj2, bool findEnemy) 
{
  if(!pObj) 
    return;

  if(!pObj2)
    pObj2 = this;
	
  if(!pObj2) 
    return;
  
  // tote Lebewesen nicht treffen
  if(GetCategory(pObj) & C4D_Living)
	if(!GetAlive(pObj))
		return;
		
  if(GetCategory(pObj2) & C4D_Living)
	if(!GetAlive(pObj2))
		return;

  var own1 = GetOwner(pObj);
  var own2 = GetOwner(pObj2);
  
  // Feinde treffen
  if(Hostile(own1, own2)) 
    return true;
  
  // eines der Objekte  hat keinen Besitzer
  var noown1 = (own1 == NO_OWNER);
  var noown2 = (own2 == NO_OWNER);
  
  var team1 = GetTeam(pObj);
  var team2 = GetTeam(pObj2);
  
  if(noown1 || noown2) 

	// im selben Team: nicht treffen/finden
	// 0 ist kein Team
    if( (team1 == team2) && (team1 || team2)) 
	{ }	
	// beide haben kein Team aber beide haben keinen Besitzer: nicht treffen/finden
	else 
	  if( !(team1 || team2) && noown1 && noown2) 
	  { }	
	  // ansonsten (Sind in unterschiedlichen Teams bzw. bei keinem Team haben sie beide keinen Besitzer)
	  else 
	  {	
	    // bei Trefferchecks, treffen
	    if(!findEnemy)
          return true;
		
	    // bei Feindchecks, anvisieren wenn ein Objekt eine Bedrohung
        if(pObj->~IsThreat()) 
          return true;
	  } 
		
  // bei Trefferchecks treffen wenn FF aktiviert ist und die Distanz groß genug ist
  if(FriendlyFire() && !findEnemy) 
    if(!(pObj->~IgnoreFriendlyFire()) && !(pObj2->~IgnoreFriendlyFire()) && Distance(AbsX(GetX(pObj)),AbsY(GetY(pObj2))) > 30 ) 
		return true;	
	
  return false;
}
