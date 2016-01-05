/*-- Piranha --*/

//Piranha verfügen über erweiterte Angriffe.

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
  if(!Random(3)) SetComDir(COMD_Stop,pPrey);
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