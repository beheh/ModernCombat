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
  if(!ActIdle() || GetEffect("IntEndSequence")) return 1;
  if (ObjectCount(RVLR) && CheckRivalry())
     return 1;
  else
    if (CheckCooperative())
      return 1;
  Sound("Trumpet", 1);
  //GameCall EndSequence
  var time = GameCallEx("EndSequence");
  //Der GameCall erledigt den Rest
  if (time == -1) return;
  AddEffect("IntEndSequence", this, 1, time + 30, this);
  return 1;
}