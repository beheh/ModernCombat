#strict 2

global func Chance(int chance) { // chance from 0 to 100%
	return Random(100) < chance;
}

global func Normalize(int iAngle, int iStart, int iPrecision) {
  if(!iPrecision) iPrecision = 1;
  var iEnd = iPrecision*360+iStart;
  
  while(iAngle<iStart) iAngle += iPrecision*360;
  while(iAngle>=iEnd) iAngle -= iPrecision*360;

  return iAngle;
}

global func CheckVisibility(object Obj1, object Obj2)
{	
	if(!Obj2)
		Obj2 = this;
    
   //Geblendet?
  if(GetEffect("IntFlashbang",Obj1))
    return false;
		
	var vis = GetVisibility(Obj1);
	if(vis != VIS_All)
  {
		var own1 = GetOwner(Obj1);
		var own2 = GetOwner(Obj2);
	
		// VIS Owner
		if(vis & VIS_Owner)
			if(own1 == own2)
				return true;
		// VIS Enemies
		if(vis & VIS_Enemies)
			if(Hostile(own1,own2))
				return true;
		// VIS Allies
		if(vis & VIS_Allies)
			if(!Hostile(own1,own2))
				return true;
		// VIS Local
		if(vis & VIS_Local) {
			var lo = own2/32;
			var bit = 1 << own2%32;
			// man, kompliziert...
			if(bit & Local(lo,Obj1))
				return true;
		}
		// sonst gibt es keine Modi
		return false;
	}
	return true;
}

global func GetShooter(object weapon)
{
  var shooter;

  if(!weapon) return;
  
  // Waffe?
  if(weapon->~IsWeapon()) {
    shooter = weapon->GetUser();
	// Schütze ist irgendwo drin
	if(Contained(shooter))
	  shooter = Contained(shooter);
	//oder fässt was an
	if(shooter)
	  if(shooter->GetActionTarget())
	    shooter = GetActionTarget();
  }
  
  // noch immer nichst gefunden?
  if(!shooter)
    // dann einfach weapon nehmen.
    shooter = weapon;
	
	return shooter;
}


global func HelpMessage(int iPlr, string szMsg, object pTarget, a,b,c,d,e,f,g)
{
  if(GetPlrExtraData(iPlr, "Hazard_NoHelpMsg")) return 0;
  return PlayerMessage(iPlr, szMsg, pTarget, a,b,c,d,e,f,g);
}
