/*-- Votekick --*/

//Das Votekick-System erlaubt bei Runden ohne Host ein Voting zu starten, um einen Spieler oder Clienten, demokratisch enschieden, entfernen zu lassen.

#strict 2

static VOTEKICK_SpamfilterCnt;
static const VOTEKICK_SpamfilterMAX = 3;


/* Daseinsberechtigung für Votekick ermitteln */

global func NoRealHost()
{
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
    if(!GetPlrClientNr(GetPlayerByIndex(i, C4PT_User)))
      return false;

  return true;
}

/* Client-Ermittlung */

global func GetClientCount()
{
  var client_nr = [];
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    var nr = GetPlrClientNr(GetPlayerByIndex(i, C4PT_User));
    if(GetIndexOf(nr, client_nr) > -1)
      continue;

    client_nr[GetLength(client_nr)] = nr;
  }

  return GetLength(client_nr);
}

/* Votekick starten */

global func Votekick(int iPlr, string pars)
{
  //Nur bei Runden ohne Host
  if(!NoRealHost() && !GetPlrClientNr(iPlr))
    return PlayerMessage(iPlr, "$HostInfo$", GetCursor(iPlr));

  //und ohne Liga
  if(GetLeague())
    return PlayerMessage(iPlr, "$LigaInfo$", GetCursor(iPlr));

  //und bei mindestens 3 Clients
  if(GetClientCount() < 3)
    return PlayerMessage(iPlr, "$PlayerCountInfo$", GetCursor(iPlr));

  //Spammende Spieler blockieren
  if(HasVotekickSpamFilter(iPlr))
    return PlayerMessage(iPlr, "$SpamProtectInfo$", GetCursor(iPlr));

  //Votekick starten
  if(!GetLength(pars) && !GetEffect("Votekick"))
  {
    VotekickMenu(iPlr);
  }
  else if(GetEffect("Votekick") && GetLength(pars))
  {
    if(WildcardMatch(pars, "*y*"))
      EffectCall(0, GetEffect("Votekick"), "Add", iPlr, false);
  }
}

global func HasVotekickSpamFilter(int iPlr)
{
  for(var i = 0; i < GetEffectCount("VotekickSpamfilter"); i++)
    if(EffectVar(0, 0, GetEffect("VotekickSpamfilter", 0, i)) == iPlr)
      return true;
  
  return false;
}

global func VotekickMenu(int iPlr, bool fClients)
{
  var mobj = GetCursor(iPlr);
  if(!fClients)
  {
    CreateMenu(CSTD, mobj, 0, 0, "$PlayerChoose$", 0, C4MN_Style_Context);
    for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
    {
      AddMenuItem(GetTaggedPlayerName(GetPlayerByIndex(i, C4PT_User)), Format("StartVotekick(%d, %d)", iPlr, GetPlayerByIndex(i, C4PT_User)), PCMK, mobj);
    }
    AddMenuItem("$ClientChoose$", Format("VotekickMenu(%d, true)", iPlr), MCMC, mobj);
    AddMenuItem("$Cancel$", Format("CloseMenu(Object(%d))", ObjectNumber()), 0, mobj);
  }
  else
  {
    CreateMenu(CSTD, mobj, 0, 0, "$ClientChoose$", 0, C4MN_Style_Context);
    var client_array = [];
    for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
    {
      var client = GetPlrClientNr(GetPlayerByIndex(i, C4PT_User));
      if(GetIndexOf(client, client_array) > -1)
        continue;
      
      client_array[client] = client;
      AddMenuItem(GetPlrClientName(GetPlayerByIndex(i, C4PT_User)), Format("StartVotekick(%d, %d, true)", iPlr, client), PCMK, mobj);
    }
    AddMenuItem("$PlayerChoose$", Format("VotekickMenu(%d, false)", iPlr), MCMC, mobj);
    AddMenuItem("$Cancel$", Format("CloseMenu(Object(%d))", ObjectNumber()), 0, mobj);
  }
}

global func StartVotekick(int by_plr, int plr, bool fClient, string szReason, bool fReason)
{
  if(!fReason)
  {
    var tim1 = CreateObject(TIM1);
    LocalN("votekickVars", tim1) = [plr, fClient];
    CallMessageBoard(tim1, false, "$MsgBoardReason$", by_plr);
    return true;
  }

  if(GetEffect("Votekick"))
    return false;

  if(!fClient)
  {
    Log("$PlayerVotekickStart$", GetTaggedPlayerName(by_plr), GetTaggedPlayerName(plr));
    Log("$Reason$", szReason);
    Schedule("Log(\"$VotekickInfo$\")", 40); 
    Schedule("Log(\"$VotekickDurationInfo$\")", 80);
    AddEffect("Votekick", 0, 101, 36, 0, 0, plr, by_plr);
  }
  else
  {
    //Parameter "plr" steht diesmal für die Client-Nummer.
    var players = GetClientPlayers(plr);
    var str = GetTaggedPlayerName(players[0]);
    for(var i = 1; i < GetLength(players); i++)
    {
      str = Format("%s, %s", str, GetTaggedPlayerName(players[i]));
    }
    Log("$ClientVotekickStart$", GetTaggedPlayerName(by_plr), GetPlrClientName(players[0]), str);
    Log("$Reason$", szReason);
    Schedule("Log(\"$VotekickInfo$\")", 40); 
    Schedule("Log(\"$VotekickDurationInfo$\")", 80);
    AddEffect("Votekick", 0, 101, 36, 0, 0, -1, by_plr, players);
  }
}

global func GetClientPlayers(int client)
{
  var players = [];
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    if(GetPlrClientNr(GetPlayerByIndex(i, C4PT_User)) == client)
      players[GetLength(players)] = GetPlayerByIndex(i, C4PT_User);
  }

  return players;
}

global func FxVotekickStart(object target, int nr, temp, int plr, int by_plr, array players)
{
  EffectVar(2, target, nr) = [-1];

  if(plr == -1)
    EffectVar(2, target, nr) = players;

  EffectVar(0, target, nr) = plr;
  EffectVar(1, target, nr) = 1;		//Zustimmungen
  EffectVar(3, target, nr) = [by_plr];	//Teilnehmer

  var effect = AddEffect("VotekickSpamfilter", 0, 101, 36*120);
  EffectVar(0, 0, effect) = by_plr;
}

global func FxVotekickTimer(object target, int nr, int time)
{
  if(!GetPlayerName(EffectVar(0, target, nr)) && !GetPlayerName(EffectVar(2, target, nr)[0]))
    return -1;

  if(GetClientCount()/3*2+(!!(GetClientCount() % 3)) <= EffectVar(1, target, nr))
    return -1;

  if(time >= 36*30)
    return -1;
}

/* Ja-Stimme hinzufügen */

global func FxVotekickAdd(object target, int nr, int by_plr)
{
  //Mehrfachteilnahme verhindern
  if(GetIndexOf(by_plr, EffectVar(3, target, nr)) > -1)
    return PlayerMessage(by_plr, "$HaveAlreadyVoted$", GetCursor(by_plr));

  EffectVar(1, target, nr)++;

  EffectVar(3, target, nr)[GetLength(EffectVar(3, target, nr))] = by_plr;

  return true;
}

/* Ergebnis ermitteln und ausgeben */

global func FxVotekickStop(object target, int nr)
{
  if(!GetPlayerName(EffectVar(0, target, nr)) && !GetPlayerName(EffectVar(2, target, nr)[0]))
    return Log("$VotingCancelled$");

  var pcnt = GetClientCount();
  var pacnt = pcnt-EffectVar(1, target, nr);
  var players = EffectVar(2, target, nr);

  if(players[0] > -1)
  {
    var playernames = GetTaggedPlayerName(players[0]);
    for(var i = 1; i < GetLength(players); i++)
      playernames = Format("%s, %s", playernames, GetTaggedPlayerName(players[i]));

    Log("$VotingStatistics$", pcnt, EffectVar(1, target, nr), pacnt, playernames);
  }
  else
    Log("$VotingStatistics$", pcnt, EffectVar(1, target, nr), pacnt, GetTaggedPlayerName(EffectVar(0, target, nr)));

  if(pcnt/3*2+(!!(pcnt % 3)) <= EffectVar(1, target, nr)) // 2/3 oder mehr Teilnehmer haben für einen Kick gestimmt?
  {
    Log("$VotekickSuccessful$");
    SetMaxPlayer(0);
    if(EffectVar(0, target, nr) > -1)
      EliminatePlayer(EffectVar(0, target, nr));
    else
      for(var plr in EffectVar(2, target, nr))
        EliminatePlayer(plr);
  }
  else
  {
    Log("$VotekickNotEnough$");
  }

  return true;
}