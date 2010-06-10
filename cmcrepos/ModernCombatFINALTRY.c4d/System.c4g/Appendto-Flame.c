/*-- KillIcons für Flammen --*/

//Tod durch Flammen werden nun mit einem passenden Icon markiert.

#strict 2
#appendto FFLM


local creator;

func Construction(object byObj)
{
  if(byObj) creator = GetID(byObj);
  return _inherited(byObj);
}

func GetKillIcon()
{
  if(creator) return creator;
  return;
}