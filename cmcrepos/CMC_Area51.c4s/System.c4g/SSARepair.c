/*-- Neues Script --*/

#strict 2

#appendto CSTR

public func IsRepairable() { return true; }

/* Zerstörung */

public func Destroyed()
{
  //Status setzen
  SetAction("Destroyed");
  fDestroyed = true;
  
  // Dmg auf maximalwert setzen.
  if(GetDamage() > MaxDamage())
  	DoDamage(-(GetDamage()-MaxDamage()));

  //Punkte bei Belohnungssystem (StrukturzerstÃ¶rung)
  if(BonusPointCondition() && iLastAttacker != -1)
    if((GetOwner() != -1 && Hostile(GetOwner(), iLastAttacker)) || (GetOwner() == -1 && !GetTeam(this)) || (GetTeam(this) != GetPlayerTeam(iLastAttacker)))
      DoPlayerPoints(BonusPoints("Destruction"), RWDS_BattlePoints, iLastAttacker, GetCursor(iLastAttacker), IC03);

  //Explosion
  FakeExplode(20, iLastAttacker+1);

  //Sound
  Sound("Blast2", false, this);

  //Letzen Angreifer zurÃ¼cksetzen
  iLastAttacker = -1;

  //Callback
  OnDestruction();

	//Reparatur anordnen
  AutoRepair();
}

public func Damage(int change)
{
	if(change > 0 && IsDestroyed())
	{
		if(GetDamage() > MaxDamage()) // Objekt zerstÃ¶rt, aber nicht mehr dmg als max. zugelassen!
  		DoDamage(-(GetDamage()-MaxDamage()));
	}
  if(GetDamage() > MaxDamage() && !IsDestroyed())
  {
    Destroyed();
  }
  if(IsDestroyed() && GetDamage() == 0 && !IsRepairing())
  {
  	fDestroyed = false;
    ObjectSetAction(this, "RepairStop");
    OnRepair();
  }
}

public func FxIntRepairTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(GetDamage(pTarget) <= 0)
  	return -1;

	DoDamage(-1, pTarget);

  if(!Random(2))
  {
    Sparks(2+Random(5), RGB(250,150,0), RandomX(-GetDefWidth()/2,+GetDefWidth()/2), RandomX(-GetDefHeight()/2,+GetDefHeight()/2));
    Sparks(2+Random(5), RGB(100,100,250), RandomX(-GetDefWidth()/2,+GetDefWidth()/2), RandomX(-GetDefHeight()/2,+GetDefHeight()/2));
  }
}

public func FxIntRepairDamage(object pTarget, int iNr, int iDmgEngy)
{
	// Reparatur darf nicht in die Länge gezogen werden können.
	if(iDmgEngy > 0)
		return 0;
	
	return iDmgEngy;
}

public func Repair()
{ 
  //Jetzt gepanzert
  if(!GetEffect("IntRepair")) AddEffect("IntRepair",this,50,5,this);
}
