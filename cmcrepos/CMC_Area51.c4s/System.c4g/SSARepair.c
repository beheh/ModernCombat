/*-- Neues Script --*/

#strict 2

#appendto CSTR

public func IsRepairable() { return true; }

/* ZerstÃ¶rung */

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