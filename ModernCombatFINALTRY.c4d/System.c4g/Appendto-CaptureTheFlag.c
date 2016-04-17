/*-- Capture the Flag --*/

//Capture the Flag erstellt bei Spielstart Spielzielhinweise.

#strict 2
#appendto GCTF


public func GoalExtraValue()	{return iWinScore;}	//Spielzielinformationen an Scoreboard weitergeben

public func ChooserFinished()
{
  //Spielzielhinweise erstellen
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    DoScoreboardShow(1, GetPlayerByIndex(i) + 1);
    CreateObject(TK06, 0, 0, GetPlayerByIndex(i));
    Sound("Info_Round.ogg", true, 0, 100, GetPlayerByIndex(i) + 1);
  }
  return _inherited(...);
}

private func SetFlagMarker(int iPlr, int iCTeam, bool fRemove)
{
  var iCol = TEAM_PlayerColumn, iData;
  //var iPlr = GetOwner(pPlr);
  if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) == 1)
  {
    iCol = TEAM_TeamColumn;
    iData = GetPlayerTeam(iPlr);
  }

  if(!fRemove)
    SetScoreboardData(iPlr, iCol, Format("<c %x>{{FLBS}}</c> %s", GetTeamColor(iCTeam), GetTaggedPlayerName(iPlr, true)), iData);
  else if(iPlr > -1)
    SetScoreboardData(iPlr, iCol, GetTaggedPlayerName(iPlr, true), iData);
}

/* gespeicherte CTF-Flaggen wiederherstellen/löschen */

public func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  if(GetType(aFlagData) == C4V_Array && aFlagData[iTeam] && !aFlagData[iTeam][3])
    CreateFlag(iTeam, aFlagData[iTeam][0], aFlagData[iTeam][1], aFlagData[iTeam][2]);

  return _inherited(iPlr, iX, iY, pBase, iTeam, ...);
}

public func RemovePlayer(int iPlr)
{
  var iTeam = GetPlayerTeam(iPlr);

  if(GetTeamPlayerCount(iTeam) == 1 && GetType(aFlagData) == C4V_Array && aFlagData[iTeam])
  {
    if(aFlagData[iTeam][3])
      RemoveObject(aFlagData[iTeam][3]);

    var flag = aFlagData[iTeam][4];
    if(flag)
    {
      var actTarget = GetActionTarget(0, flag);
      if(actTarget)
        SetFlagMarker(GetOwner(actTarget), 0, true);

      LocalN("fForcedDestruction", flag) = true;
      RemoveObject(flag);
    }
  }

  return _inherited(iPlr, ...);
}

local aFlagData;

/* Globale Funktionen */

global func CreateFlag(int iTeam, int iX, int iY, int iColor) // Platziert eine Flagge für Team iTeam, Position und Farbe angeben
{
  var ctf = FindObject2(Find_ID(GCTF));
  if(!ctf)
    return;

  var flagdata = LocalN("aFlagData", ctf);
  if(!flagdata)
    flagdata = [];

  //Daten speichern
  flagdata[iTeam] = [iX, iY, iColor];
  LocalN("aFlagData", ctf) = flagdata;

  //Existiert das Team überhaupt?
  if(!GetTeamPlayerCount(iTeam))
    return;

  var pBase = CreateObject(FLBS, AbsX(iX), AbsY(iY), -1);
  var pFlag = CreateObject(FLA2, AbsX(iX), AbsY(iY), -1);
  pFlag->Activate(pBase, iTeam, iColor);
  flagdata[iTeam][3] = pBase;
  flagdata[iTeam][4] = pFlag;

  LocalN("aFlagData", ctf) = flagdata;

  ctf->InitTeam(iTeam);
}

public func OnPlayerRankUp(int iPlr)
{
  //Scoreboard-Spielerränge aktualisieren
  if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) > 1)
    SetScoreboardData(iPlr, TEAM_PlayerColumn, GetTaggedPlayerName(iPlr, true));
  else
    SetScoreboardData(iPlr, TEAM_TeamColumn, GetTaggedPlayerName(iPlr, true), GetPlayerTeam(iPlr));

  var team = GetIndexOf(iPlr, aFlagCarrier);
  if(team < 1)
    return true;

  SetFlagMarker(iPlr, team);

  return true;
}

public func WinScoreChange(int iNewScore)
{
  //Einstellbarer Punktestand bei Ligarunden begrenzen
  if(GetLeague())
    iNewScore = BoundBy(iNewScore, 3, 6);

  return _inherited(iNewScore, ...);
}