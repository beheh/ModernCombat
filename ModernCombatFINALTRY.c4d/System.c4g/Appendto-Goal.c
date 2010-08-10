/*-- Zielsteuerung --*/

#strict 2
#appendto GOAL

protected func FxIntEndSequenceStop()
{
  if (Par(3)) return;
  GameOver();
  return RemoveObject();
}

protected func CheckTime() {
  if(!ActIdle() || GetEffect("IntEndsequence")) return 1;
  if (ObjectCount(RVLR) && CheckRivalry())
     return 1;
  else
    if (CheckCooperative())
      return 1;
  Sound("Trumpet", 1);
  //GameCall EndSequence
  AddEffect("IntEndSequence", this, 1, GameCallEx("EndSequence") + 30, this);
  return 1;
}