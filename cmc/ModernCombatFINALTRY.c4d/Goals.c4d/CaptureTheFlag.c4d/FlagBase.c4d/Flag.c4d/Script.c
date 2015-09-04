/*--- Flagge! ---*/

#strict

local base, team, cteam;

/* TimerCall */

private func Wee()
{
  if(WildcardMatch(GetAction(), "*Fly*")) return(Wind2Fly());
  if(WildcardMatch(GetAction(), "*Attach*")) return(Clonk2Rotate());
}

private func Wind2Fly()
{
  if(GetWind() > 0)
    SetDir(DIR_Right);
  else
    SetDir(DIR_Left);
}

private func Clonk2Rotate()
{
  if(!GetAlive(GetActionTarget())) return(AttachTargetLost());

  SetDir(GetActionTarget()->GetDir());

  if(GetDir(GetActionTarget()) == DIR_Left())
    {
    SetR(15-GetXDir(GetActionTarget()));
    }
  if(GetDir(GetActionTarget()) == DIR_Right())
    {
    SetR(-15-GetXDir(GetActionTarget()));
    }
}

protected func Activate(pBase, iTeam, rgb)
{
  base = pBase;
  SetAction("Fly", base);
  Wind2Fly();

  team = iTeam;
  SetColorDw(rgb);

  // bei Dunkelheit besser zu sehen
  var tmp = AddLightAmbience(30,this());
  var r,g,b,a;
  SplitRGBaValue(rgb,r,g,b,a);
  tmp->ChangeColor(RGBa(r,g,b,a+30));
  SetVisibility(VIS_All,tmp);

  AddEffect("Collect", this(), 101, 5, this());
}

/* Einsamml0rn!!11 */

protected func Collected(pClonk)
{
  Log("$HasTheFlag$", GetPlayerName(GetOwner(pClonk)), GetTeamName(team));
  SetAction("Attach", pClonk);
  
  cteam = GetPlayerTeam(GetOwner(pClonk));
  
  // game call: FlagCaptured(flagTeam, captureTeam, clonk)
  // flagTeam: The team to which the flag belongs to
  // captureTeam: The team that captured the flag
  // clonk: the clonk who did it
  GameCallEx("FlagCaptured",team, GetPlayerTeam(GetOwner(pClonk)), pClonk);
  return(1);
}

protected func CheckCollect(pClonk)
{
  if(WildcardMatch(GetAction(), "*Attach*")) return();
  if(GetPlayerTeam(GetOwner(pClonk)) == team)
    {
    if(GetAction() ne "Lost") CheckFlag(pClonk);
    else Return2Base(pClonk);
    return();
    }
  if(FindObject(GetID(), 0,0,0,0,0, 0, pClonk)) return();  // Mehr als 2 Teams ftw!

  return(Collected(pClonk));
}

protected func AttachTargetLost()
{
  SetAction("Lost");
  SetActionTargets();
  SetDir();
  // falls sie festsitzt, wird sie sofort zurückgebracht
  if(GBackSolid()) {
    RemoveObject();
	return();
  }
  Log("$FlagLost$", GetTeamName(team));
  GameCallEx("FlagLost",team);
}

/* Zurückfliegen und Punkte vergeben */

public func IsAtHome() {
	if(GetActionTarget() == base)
		return(true);
}

public func GetCarrier() {
	if(WildcardMatch(GetAction(), "*Attach*")) {
      return(GetActionTarget()); 
	}
}

protected func Return2Base(pClonk, nolog)
{
  SetR();
  SetRDir();
  SetAction("Fly", base);
  if(!nolog)
    Log("$ReturnedTheFlag$", GetPlayerName(GetOwner(pClonk)));
  // game call: FlagReturned(flagTeam, clonk)
  // flagTeam: The team to which the flag belongs to
  // clonk: the clonk who did it
  GameCallEx("FlagReturned",team, GetPlayerTeam(GetOwner(pClonk)));
}

protected func CheckFlag(pClonk)
{
  var flag = FindObject(GetID(), 0,0,0,0,0, 0,pClonk);
  if(!flag) return();

  Log("$CapturedTheFlag$", GetPlayerName(GetOwner(pClonk)));
  DoWealth(GetOwner(pClonk), 50); // Geld!
  DoTeamScore(GetPlayerTeam(GetOwner(pClonk)), 1);
  flag->~Return2Base(0,1);
  // game call: FlagScored(flagTeam, scoreTeam, clonk)
  // flagTeam: The team to which the flag belongs to
  // scoreTeam: the team that just scored
  // clonk: the clonk who did it
  GameCallEx("FlagScored",team, GetPlayerTeam(GetOwner(pClonk)), pClonk);
}

// verbessertes Einsammeln
public func FxCollectTimer(target, no)
{
  if(WildcardMatch(GetAction(target),"*Attach*")) return();
  var clonk;
  while(clonk = FindObject(0, -20, -20, 40, 40, OCF_CrewMember(), 0,0, NoContainer(), clonk))
    if(clonk->GetOCF() & OCF_Alive())
      if(CheckCollect(clonk))
        return();
}

/* Kaputt! */

public func Destruction()
{
  // Flaggen dürfen nicht einfach weg sein
  var nFlag = CreateObject(FLA2, 0,0, GetOwner());
  nFlag->Activate(base, team, GetColorDw());
  // game call: FlagReturned(flagTeam)
  // flagTeam: The team to which the flag belongs to
  GameCallEx("FlagReturned",team);
  Log("$FlagReturned$", GetTeamName(team));
}

public func NoWarp() { return(1); }

public func GetBase() { return(base); }

public func GetTeam() { return (team); }
