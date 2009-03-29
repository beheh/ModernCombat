/*-- Chooser-Append --*/
#strict
#appendto CHOS

protected func OpenGoalMenu(id dummy, int iSelection)
{
  if(!pGoal) return(OpenMenu());
  var pClonk = GetCursor();
  if(pGoal->~ConfigMenu(pClonk))
	return(1);//OpenMenu()
  return(_inherited(dummy, iSelection,...));
}