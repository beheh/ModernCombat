/*-- Capture the Flag - EventInfo4K --*/

#strict 2
#appendto FLA2

protected func Collected(pClonk)
{
  EventInfo4K(0, Format("$HasTheFlag$", GetTaggedPlayerName(GetOwner(pClonk)), GetTeamColor(team), GetTeamName(team)), FLA2, 0, GetTeamColor(team));
  SetAction("Attach", pClonk);
  
  cteam = GetPlayerTeam(GetOwner(pClonk));
  
  // game call: FlagCaptured(flagTeam, captureTeam, clonk)
  // flagTeam: The team to which the flag belongs to
  // captureTeam: The team that captured the flag
  // clonk: the clonk who did it
  GameCallEx("FlagCaptured",team, GetPlayerTeam(GetOwner(pClonk)), pClonk);
  return 1;
}

protected func AttachTargetLost()
{
  SetAction("Lost");
  SetActionTargets();
  SetDir();
  // falls sie festsitzt, wird sie sofort zurückgebracht
  if(GBackSolid()) {
    RemoveObject();
    return;
  }
  EventInfo4K(0, Format("$FlagLost$", GetTeamColor(team), GetTeamName(team)), FLA2, 0, GetTeamColor(team));
  GameCallEx("FlagLost",team);
}

protected func Return2Base(pClonk, nolog)
{
  SetR();
  SetRDir();
  SetAction("Fly", base);
  if(!nolog)
    EventInfo4K(0, Format("$ReturnedTheFlag$", GetTaggedPlayerName(GetOwner(pClonk))), FLA2, 0, GetTeamColor(team));
  // game call: FlagReturned(flagTeam, clonk)
  // flagTeam: The team to which the flag belongs to
  // clonk: the clonk who did it
  GameCallEx("FlagReturned",team, GetPlayerTeam(GetOwner(pClonk)));
}

protected func CheckFlag(pClonk)
{
  var flag = FindObject(GetID(), 0,0,0,0,0, 0,pClonk);
  if(!flag) return;

  EventInfo4K(0, Format("$CapturedTheFlag$", GetTaggedPlayerName(GetOwner(pClonk))), FLA2, 0, GetTeamColor(team));
  DoWealth(GetOwner(pClonk), 50); // Geld!
  DoTeamScore(GetPlayerTeam(GetOwner(pClonk)), 1);
  flag->~Return2Base(0,1);
  // game call: FlagScored(flagTeam, scoreTeam, clonk)
  // flagTeam: The team to which the flag belongs to
  // scoreTeam: the team that just scored
  // clonk: the clonk who did it
  GameCallEx("FlagScored",team, GetPlayerTeam(GetOwner(pClonk)), pClonk);
}


public func Destruction()
{
  // Flaggen dürfen nicht einfach weg sein
  var nFlag = CreateObject(FLA2, 0,0, GetOwner());
  nFlag->Activate(base, team, GetColorDw());
  // game call: FlagReturned(flagTeam)
  // flagTeam: The team to which the flag belongs to
  GameCallEx("FlagReturned",team);
  EventInfo4K(0, Format("$FlagReturned$", GetTeamColor(team), GetTeamName(team)), FLA2, 0, GetTeamColor(team));
}