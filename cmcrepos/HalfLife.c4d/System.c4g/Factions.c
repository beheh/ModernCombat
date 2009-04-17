/*-- Fraktionen --*/

#strict 2

static const FACTION_None = 0;
static const FACTION_Combine = 1;
static const FACTION_Xen = 2;

global func SameFaction(int iFaction)
{
  return this()->~Faction() == iFaction;
}

global func Find_Faction(int iFaction)
{
  return([C4FO_Func, "SameFaction", iFaction]);
}

global func FindTargets(object pObj, int maxDist, int maxAngle, int angle, bool disregardLineOfSight)
{
  if(!pObj)
    pObj = this;

  if(!maxAngle)
    maxAngle = 180;
	
  var pT;
  var targets = CreateArray();
  
  var x = GetX(pObj);
  var y = GetY(pObj);

  var preTargets = FindObjects(
		Find_Distance(maxDist, AbsX(x), AbsY(y)),
		Find_Exclude(pObj),
        Find_NoContainer(),
        Find_Or(
                Find_Func("IsBulletTarget",GetID(pObj),0,pObj),
                Find_OCF(OCF_Alive)),
        Find_Not(Find_Faction(pObj->~Faction())),
        Sort_Distance()
        //Find_Hostile(GetOwner()),  // schon in CheckEnemy drin
        );
  
  for(pT in preTargets)
  {
		var ox = GetX(pT);
		var oy = GetY(pT);
	
		if(!CheckEnemy(pT,pObj, true))
    {
			continue;
		}
	
		// Pfad frei
		if(!disregardLineOfSight)
			if(!PathFree(x,y,ox,oy))
				continue;
		
		// Im Suchkegel
		var diffAngle = Abs(Normalize(Angle(x,y,ox,oy)-angle,-180));
		if(diffAngle > maxAngle)
		    continue;
		
		// unsichtbare Ziele
		if(!CheckVisibility(pT, pObj))
			continue;
		
		// dann rein in Ergebnismenge...
		targets[GetLength(targets)] = pT;
	}
	
	return targets;
}

global func CheckEnemy(object pObj, object pObj2, bool findEnemy)
{
  if(!pObj) return false;

  if(!pObj2)
    pObj2 = this;
  if(!pObj2) return false;
  
  // tote Lebewesen nicht treffen
  if(GetCategory(pObj) & C4D_Living)
	if(!GetAlive(pObj))
		return false;
		
  if(GetCategory(pObj2) & C4D_Living)
	if(!GetAlive(pObj2))
		return false;

  var own1 = GetOwner(pObj);
  var own2 = GetOwner(pObj2);
  
  // Feinde treffen
  if(Hostile(own1, own2)) {
    return true;
  }
  
  // eines der Objekte  hat keinen Besitzer
  var noown1 = (own1 == NO_OWNER);
  var noown2 = (own2 == NO_OWNER);
  
  var team1 = GetTeam(pObj);
  var team2 = GetTeam(pObj2);
  
  if(noown1 || noown2) {

	// im selben Team: nicht treffen/finden
	// 0 ist kein Team
    if( (team1 == team2) && (team1 || team2)) { }
	
	// beide haben kein Team aber beide haben keinen Besitzer: nicht treffen/finden
	else if( !(team1 || team2) && noown1 && noown2 && (pObj->~Faction() == pObj2->Faction())) {}
	
	// ansonsten (Sind in unterschiedlichen Teams bzw. bei keinem Team haben sie beide keinen Besitzer)
	else {
	
	  // bei Trefferchecks, treffen
	  if(!findEnemy)
        return true;
		
	  // bei Feindchecks, anvisieren wenn ein Objekt eine Bedrohung
      if(pObj->~IsThreat()) {
        return true;
      }
	}
 
  }
		
  // bei Trefferchecks treffen wenn FF aktiviert ist
  if(FriendlyFire() && !findEnemy) {
    if(!(pObj->~IgnoreFriendlyFire())
	&& !(pObj2->~IgnoreFriendlyFire()) ) {
		return true;
	}
  }
	
  return false;
}