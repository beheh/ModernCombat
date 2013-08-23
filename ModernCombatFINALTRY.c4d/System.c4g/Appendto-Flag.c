/*-- CTF-Flagge --*/

//Das CTF Spielziel zeigt Spielevents nun mit dem Eventsystem anstatt im Log.

#strict 2

#appendto FLA2


protected func Collected(pClonk)
{
  //Eventnachricht: Spieler hat Flagge von Spieler
  EventInfo4K(0, Format("$HasTheFlag$", GetTaggedPlayerName(GetOwner(pClonk)), GetTeamColor(team), GetTeamName(team)), FLA2, 0, GetTeamColor(team), 0, "Info_Event.ogg");
  SetAction("Attach", pClonk);

  cteam = GetPlayerTeam(GetOwner(pClonk));

  //game call: FlagCaptured(flagTeam, captureTeam, clonk)
  //flagTeam: The team to which the flag belongs to
  //captureTeam: The team that captured the flag
  //clonk: the clonk who did it
  GameCallEx("FlagCaptured",team, GetPlayerTeam(GetOwner(pClonk)), pClonk);

  RemoveEffect("AutoReturn", this);
  return 1;
}

protected func AttachTargetLost()
{
  DropFlag();
}

public func DropFlag()
{
  //Flagge nach 60 Sekunden automatisch zurückbringen
  AddEffect("AutoReturn", this, 1, 36 * 60, this);

  if(!GetActionTarget())
    return;

  SetAction("Lost");
  SetActionTargets();
  SetDir();

  while(Contained())
    Exit();

  //Falls sie festsitzt, wird sie sofort zurückgebracht
  if(GBackSolid())
  {
    RemoveObject();
    return;
  }
  //Spieler verliert Flagge
  EventInfo4K(0, Format("$FlagLost$", GetTeamColor(team), GetTeamName(team)), FLA2, 0, GetTeamColor(team), 0, "Info_Event.ogg");
  GameCallEx("FlagLost",team);
}

public func FxAutoReturnTimer()
{
  SetR();
  SetRDir();
  SetAction("Fly", base);
  //Flagge zurückgesetzt
  EventInfo4K(0, Format("$FlagReturned$", GetTeamColor(team), GetTeamName(team)), FLA2, 0, GetTeamColor(team), 0, "Info_Event.ogg");

  GameCallEx("FlagReturned", team);

  return -1;
}

protected func Return2Base(pClonk, nolog)
{
  RemoveEffect("AutoReturn", this);

  SetR();
  SetRDir();
  SetAction("Fly", base);
  if(!nolog)
    //Eventnachricht: Spieler setzt Flagge zurück
    EventInfo4K(0, Format("$ReturnedTheFlag$", GetTaggedPlayerName(GetOwner(pClonk))), FLA2, 0, GetTeamColor(team), 0, "Info_Event.ogg");
  GameCallEx("FlagReturned",team, pClonk);
}

protected func CheckFlag(pClonk)
{
  var flag = FindObject(GetID(), 0,0,0,0,0, 0,pClonk);
  if(!flag) return;

  var iPlr = GetOwner(pClonk);
  //Eventnachricht: Spieler klaut Flagge
  EventInfo4K(0, Format("$CapturedTheFlag$", GetTaggedPlayerName(iPlr)), FLA2, 0, GetTeamColor(team), 0, "Info_Objective.ogg");
  //Geldbonus: 30 Clunker
  DoWealth(iPlr, 30);
  DoTeamScore(GetPlayerTeam(iPlr), 1);
  var winscore = GetWinScore(FindObject(GCTF));
  if(FindObject(GCTF) && winscore-1 == FindObject(GCTF)->TeamGetScore(GetPlayerTeam(iPlr)))
    //Spieler gewinnt bald
    EnemyEventInfo(iPlr, Format("$OneFlagLeft$", GetTaggedTeamName(GetPlayerTeam(iPlr))), GCTF, 0, 0, 0, "Info_Alarm.ogg");

  flag->~Return2Base(0,1);
  //game call: FlagScored(flagTeam, scoreTeam, clonk)
  //flagTeam: The team to which the flag belongs to
  //scoreTeam: the team that just scored
  //clonk: the clonk who did it
  GameCallEx("FlagScored",team, GetPlayerTeam(iPlr), pClonk);
}

local fForcedDestruction;

public func Destruction()
{
  //Löschung erzwungen
  if(fForcedDestruction)
    return;

  //Flaggen dürfen nicht einfach weg sein
  var nFlag = CreateObject(FLA2, 0,0, GetOwner());
  nFlag->Activate(base, team, GetColorDw());
  //game call: FlagReturned(flagTeam)
  //flagTeam: The team to which the flag belongs to
  GameCallEx("FlagReturned",team);
  //Flagge zurückgesetzt
  EventInfo4K(0, Format("$FlagReturned$", GetTeamColor(team), GetTeamName(team)), FLA2, 0, GetTeamColor(team), 0, "Info_Event.ogg");
}

public func FxCollectTimer(target, no)
{
  if(WildcardMatch(GetAction(target),"*Attach*")) return;
  if(GetMaterial(AbsX(GetX(target)), AbsY(GetY(target))) == Material("DuroLava"))
  {
    RemoveObject(target);
    return -1;
  }
  return _inherited(target, no);
}