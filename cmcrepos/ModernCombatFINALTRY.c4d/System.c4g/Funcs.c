#strict 2

global func GetShooter(object weapon)
{
  var shooter;

  if(!weapon) return;
  
  // Waffe?
  if(weapon->~IsWeapon())
  {
    shooter = weapon->GetUser();
	// Sch�tze ist irgendwo drin
	if(Contained(shooter))
	  shooter = Contained(shooter);
	//oder f�sst was an
	if(shooter)
	  if(shooter->GetActionTarget() && (shooter->GetProcedure() == "PUSH"))//!!!
	    shooter = GetActionTarget();
  }
  
  // noch immer nichst gefunden?
  if(!shooter)
    // dann einfach weapon nehmen.
    shooter = weapon;
	
	return shooter;
}
