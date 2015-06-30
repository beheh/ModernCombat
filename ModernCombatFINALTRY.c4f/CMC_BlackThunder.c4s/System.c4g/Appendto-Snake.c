/*-- Schlange --*/

//Schlangen verfügen über erweiterte Angriffe.

#strict 2
#appendto SNKE

public func IsBulletTarget()	{return true;}
public func NoSentryGunTarget()	{return true;}


public func Attack()
{
  //Nur während des Laufens oder Schwimmens kann alle 2 Sekunden einmal gebissen werden
  if(GetEffect("IntAttackDelay", this()) || (GetAction() == "Walk" && GetAction() == "Swim")) return;

  //Beute suchen
  var pPrey = Contained();
  if(!pPrey || !GetAlive(pPrey)) pPrey = FindObject(0, -13 + 26 * GetDir(), 0, 0, 0, OCF_Prey, 0, 0, NoContainer());
  if(!pPrey || !GetAlive(pPrey)) return;

  //Beißen
  DoDmg(6, DMG_Bio, pPrey);
  Sound("Snake*");
  AddEffect("IntAttackDelay", this(), 1, 70);
  return 1;
}