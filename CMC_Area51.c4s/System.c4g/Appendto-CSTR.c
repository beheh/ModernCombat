/*-- Neues Script --*/

#strict 2

#appendto CSTR

public func IsRepairable()	{return true;}

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

public func OnRepairing()
{
	if(IsDestroyed() && !IsRepairing())
		StartRepair();

	return true;
}

public func IsFullyRepaired()
{
	RemoveEffect("IntRepair", this);
	return true;
}

public func Damage(int change)
{
  //Struktur zerstören, aber mehr Schaden als den Maximalen nicht zulassen
  if(change > 0 && IsDestroyed())
  {
    if(GetDamage() > MaxDamage())
      DoDamage(-(GetDamage()-MaxDamage()));
  }

/*
  //Bei Reparatur die Selbstreparatur starten, sofern zerstört
  if(change < 0 && IsDestroyed())
  {
    StartRepair();
  }
*/

  //Bei höherem Schaden als dem Maximalen entsprechend zerstören
  if(GetDamage() > MaxDamage() && !IsDestroyed())
  {
    Destroyed();
  }

  //Bei beendetem Reparaturvorgang Sonderfunktionen aufrufen
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
  //Reparatur darf nicht in die Länge gezogen werden
  if(iDmgEngy > 0)
    return 0;

  return iDmgEngy;
}

public func Repair()
{ 
  //Jetzt gepanzert
  if(!GetEffect("IntRepair")) AddEffect("IntRepair",this,50,5,this);
}
