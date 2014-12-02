/*-- Piranha --*/

//Piranha meiden Schlauchboote und verfügen über erweiterte Angriffe.

#strict 2
#appendto PIRA

public func IsBulletTarget()	{return true;}
public func NoSentryGunTarget()	{return true;}


private func Attack(object pPrey)
{
  SetAction("Knabber");
  Sound("Munch*");

  //Schaden
  DoDmg(8,DMG_Melee,pPrey,0,0,GetID());
  return(1);
}

protected func Death() 
{
  SetDir(0);
  ChangeDef(DPIR);
  SetAction("Dead", this);
  this->FadeOut(this);
}

protected func Activity() 
{
  _inherited();

  //Booten aus dem Weg gehen
  var boot;
  if(boot = FindObject2(Find_ID(SPBT),Find_Distance(150)))
    if(GetY(boot) - GetY() < 40)
      SetComDir(BoundBy(GetComDir(), COMD_DownRight, COMD_DownLeft));
}

private func CheckAttack()
{
  //Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if(GetEffect("PossessionSpell", this())) return;

  var pPrey;
  if(GetAction() != "Swim") return;
  if(!Contained())
    if(pPrey = FindObject(0,+1,+1,0,0,OCF_Prey,0,0,NoContainer()))
      Attack(pPrey);
    else if(pPrey = FindObject(0,-8 + 16 * GetDir(),+1,0,0,OCF_Prey,0,0,NoContainer()))
      Attack(pPrey);
}