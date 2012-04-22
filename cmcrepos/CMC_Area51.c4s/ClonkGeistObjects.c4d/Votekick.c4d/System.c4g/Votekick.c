/*-- Neues Script --*/

#strict 2

static VOTEKICK_SpamfilterCnt;
static const VOTEKICK_SpamfilterMAX = 3;

// Falls der Host nicht mitspielt oder es sich um einen Dedizierten Server haltet, wird true zurückgegeben.
global func NoRealHost()
{
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
		if(!GetPlrClientNr(GetPlayerByIndex(i, C4PT_User)))
			return false;
	
	return true;
}

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

global func Votekick(int iPlr, string pars)
{
	/*if(!NoRealHost()) // Nur, wenn kein Host da ist.
		return false;*/ // Aus Testgründen deaktiviert
	
	if(GetEffect("VotekickSpamfilter"))
		return PlayerMessage(iPlr, "Es sind nur 3 Umfragen alle 90 Sekunden erlaubt.", GetCursor(iPlr));

	if(!GetLength(pars) && !GetEffect("Votekick")) // Votekick starten.
	{
		VotekickMenu(iPlr);
	}
	else if(GetEffect("Votekick") && GetLength(pars))
	{
		if(WildcardMatch(pars, "*y*"))
			EffectCall(0, GetEffect("Votekick"), "Add", iPlr, false);
	}
}

global func VotekickMenu(int iPlr, bool fClients)
{
	var mobj = GetCursor(iPlr);
	CreateMenu(CSTD, mobj, 0, 0, "", 0, C4MN_Style_Context);
	if(!fClients)
	{
		for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
		{
			AddMenuItem(GetTaggedPlayerName(GetPlayerByIndex(i, C4PT_User)), Format("StartVotekick(%d, %d)", iPlr, GetPlayerByIndex(i, C4PT_User)), PCMK, mobj);
		}
		AddMenuItem("Clientwahl", Format("VotekickMenu(%d, true)", iPlr), FLNT, mobj);
		AddMenuItem("$Cancel$", Format("CloseMenu(Object(%d))", ObjectNumber()), EFLN, mobj);
	}
	else
	{
		var client_array = [];
		for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
		{
			var client = GetPlrClientNr(GetPlayerByIndex(i, C4PT_User));
			if(GetIndexOf(client, client_array) > -1)
				continue;
			
			client_array[client] = client;
			AddMenuItem(GetPlrClientName(GetPlayerByIndex(i, C4PT_User)), Format("StartVotekick(%d, %d, true)", iPlr, client), PCMK, mobj);
		}
		AddMenuItem("Spielerwahl", Format("VotekickMenu(%d, true)", iPlr), FLNT, mobj);
		AddMenuItem("$Cancel$", Format("CloseMenu(Object(%d))", ObjectNumber()), EFLN, mobj);
	}
}

global func StartVotekick(int by_plr, int plr, bool fClient, string szReason, bool fReason)
{
	if(!fReason)
	{
		var tim1 = CreateObject(TIM1);
		LocalN("votekickVars", tim1) = [plr, fClient];
		CallMessageBoard(tim1, false, "Grund:", by_plr);
		return true;
	}
	
	if(GetEffect("Votekick"))
		return false;

	if(!fClient)
	{
		// Ja, ich nutze absichtlich nicht die Eventlog, weil da fällt sowas immernoch weniger auf.
		Log("Spieler %s hat eine Votekick-Umfrage gestartet! Spieler %s soll gekickt werden.", GetTaggedPlayerName(by_plr), GetTaggedPlayerName(plr));
		Log("<c ffff33>Begründung:</c> %s", szReason);
		Log("Mit <c ff0000>/votekick y</c> zustimmen."); 
		Log("Das Voting läuft <c ff0000>30</c> Sekunden lang.");
		AddEffect("Votekick", 0, 101, 36, 0, 0, plr, by_plr);
	}
	else
	{
		// Parameter "plr" steht diesmal für die Client-Nummer.
		var players = GetClientPlayers(plr);
		var str = GetTaggedPlayerName(players[0]);
		for(var i = 1; i < GetLength(players); i++)
		{
			str = Format("%s, %s", str, GetTaggedPlayerName(players[i]));
		}
		Log("Spieler %s hat eine Votekick-Umfrage gestartet! Client %s (%s) soll gekickt werden.", GetTaggedPlayerName(by_plr), GetPlrClientName(players[0]), str);
		Log("<c ffff33>Begründung:</c> %s", szReason);
		Log("Mit <c ff0000>/votekick y</c> zustimmen."); 
		Log("Das Voting läuft <c ff0000>30</c> Sekunden lang.");
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
	if(plr == -1)
		EffectVar(2, target, nr) = players;
	
	EffectVar(0, target, nr) = plr;
	EffectVar(1, target, nr) = 1; // Zustimmungen.
	EffectVar(3, target, nr) = [by_plr]; // Teilnehmer.
}

global func FxVotekickTimer(object target, int nr, int time)
{
	if(!GetPlayerName(EffectVar(0, target, nr)))
		return -1;
	
	if(time >= 36*30)
		return -1;
}

global func FxVotekickAdd(object target, int nr, int by_plr)
{
	if(GetIndexOf(by_plr, EffectVar(3, target, nr)) > -1) // Man darf nur einmal teilnehmen.
		return PlayerMessage(by_plr, "Du hast schon abgestimmt.", GetCursor(by_plr));
	
	EffectVar(1, target, nr)++;
		
	EffectVar(3, target, nr)[GetLength(EffectVar(3, target, nr))] = by_plr;
	
	return true;
}

global func FxVotekickStop(object target, int nr)
{
	VOTEKICK_SpamfilterCnt++;
	
	if(!GetPlayerName(EffectVar(0, target, nr)) && !GetPlayerName(EffectVar(2, target, nr)[0]))
		return Log("Umfrage abgebrochen. (Der Spieler, der gekickt werden sollte, hat die Runde verlassen.)");

	if(VOTEKICK_SpamfilterCnt == VOTEKICK_SpamfilterMAX)
	{
		VOTEKICK_SpamfilterCnt = 0;
		AddEffect("VotekickSpamfilter", 0, 101, 36*90);
	}
	
	var pcnt = GetClientCount();
	var pacnt = pcnt-EffectVar(1, target, nr);
	
	Log("Es haben %d Spieler an der Umfrage teilgenommen. %d Spieler sind für und %d Spieler sind gegen einen Rauswurf von %s.", pcnt, EffectVar(1, target, nr), pacnt, GetTaggedPlayerName(EffectVar(0, target, nr)));
	
	if(pcnt/3*2 <= EffectVar(1, target, nr)) // 2/3 oder mehr Teilnehmer haben für einen Kick gestimmt?
	{
		Log("Es hat eine 2/3 Mehrheit der Teilnehmer für einen Rauswurf gestimmt.");
		SetMaxPlayer(0);
		if(EffectVar(0, target, nr) > -1)
			EliminatePlayer(EffectVar(0, target, nr));
		else
			for(var plr in EffectVar(2, target, nr))
				EliminatePlayer(plr);
	}
	else
	{
		Log("Es haben nicht genügend Teilnehmer für einen Rauswurf gestimmt.");
	}
	
	return true;
}

