/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF, NOAM]; } 
public func SectionName() { return "Lobby"; } 
public func HideInMenu() { return true; } 

static g_Gamemaster, g_iWinScore;
static g_aMinigames;

public func Initialize()
{
	Music("@CMC_Beach Walk.ogg");
  FakeScriptGo(true);
  g_iWinScore = BoundBy(11 - GetPlayerCount(), 5, 10);
  g_aMinigames = [];
  var def, i;
  while(def = GetDefinition(i++, C4D_Goal))
	{
		if(def->~IsCMCMinigame())
		{
			g_aMinigames[GetLength(g_aMinigames)] = def;
		}
	}
  
  return _inherited(...);
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	SetRespawnPosition(RandomX(10, LandscapeWidth()-10), 30, iPlr);
	
	return _inherited(iPlr, ...);
}

public func CreateInterior()
{
	g_Gamemaster = CreateObject(PCMK, 310, 200, -1);
  
  return true;
}

public func Script1()
{
	Message("$WelcomeMsg1$", g_Gamemaster);
	Settings();
  
  return true;
}

public func Settings()
{
	var pClonk = GetCursor();
	CreateMenu(CSTR, pClonk, this, C4MN_Extra_None, 0, 0, C4MN_Style_Dialog);
    
  var szPoints = Format("$Points$", g_iWinScore);
  if(g_iWinScore == BoundBy(11 - GetPlayerCount(), 5, 10))
    szPoints = Format("<c 00ff00>%s ($Recommended$!)</c>", szPoints);
  AddMenuItem("$PointsMore$", "ChangeWinScore(1, 0)", WBRL, pClonk, 0, 0, 0, 2, 0, 0);
  AddMenuItem("$PointsLess$", "ChangeWinScore(-1, 1)", BARL, pClonk, 0, 0, 0, 2, 0, 0);
  AddMenuItem("$Minigames$", "MinigameMenu", CSTD, pClonk);
  AddMenuItem(szPoints, "", FLAG, pClonk, 0, 0, 0, 2, 0, 0);
  //AddMenuItem(Format("<c %x%x00>Lautstärke", 55+2*S_iMusicVolume, 55+2*S_iMusicVolume), "SetGlobalOption(1, 5)", SBMS, pClonk, S_iMusicVolume, 0, 0, 2, 2, 0);
  //AddMenuItem("$MusicDone$", "SetGlobalOption(1337)", ROCK, pClonk, 0, 0, 0, 0, 0, 0);
  AddMenuItem("$Ready$", "Ready", CSTD, pClonk);
}

public func MinigameMenu()
{
	var pByObj = GetCursor();
	CreateMenu(CSTR, pByObj, 0, C4MN_Extra_None, "Minigamewahl", 0, C4MN_Style_Context);
	var i, def, j = 0;
	while(def = GetDefinition(i++, C4D_Goal))
	{
		if(def->~IsCMCMinigame())
		{
			var clr = 0x777777;
			if(GetIndexOf(def, g_aMinigames) > -1)
				clr = 0xFFFFFF;
			
			AddMenuItem(Format("<c %x>%s</c>", clr, GetName(0, def)), "SwitchMinigame", def, pByObj, 0, j, GetDesc(0, def));
			++j;
		}
	}
	AddMenuItem("$Cancel$", "Settings", CHOS, pByObj);
	
	return true;
}

public func SwitchMinigame(id idMinigameID, int iSel)
{
	if(GetIndexOf(idMinigameID, g_aMinigames) > -1)
		DelArrayItem4K(g_aMinigames, GetIndexOf(idMinigameID, g_aMinigames));
	else
		g_aMinigames[GetLength(g_aMinigames)] = idMinigameID;
	
	MinigameMenu();
	return SelectMenuItem(iSel, GetCursor());
}

public func ChangeWinScore(int iChange, int iSel)
{
	g_iWinScore = BoundBy(g_iWinScore+iChange, 0, 30);
	
	Settings();
	return SelectMenuItem(iSel, GetCursor());
}

public func Ready() 
{ 
	//Minigames zufällig anordnen
	var temp = g_aMinigames;
	g_aMinigames = [];
	while(GetLength(temp))
	{
		var pos = Random(GetLength(temp));
		g_aMinigames[GetLength(g_aMinigames)] = temp[pos];
		DelArrayItem4K(temp, pos); 
	}
	
	CloseMenu(GetCrew());
	
	//Charakterwahl!
	LoadMinigame(MLBC); 
}

public func Script10()
{
	Message("$WelcomeMsg2$", g_Gamemaster);
	
	for(var i = 0; i < GetPlayerCount(); i++)
  {
    var info = CreateObject(TKBT, 0, 0, GetPlayerByIndex(i)); // 'cause it rules!11... x)
    info->SetHighlightColor(RGB(0,250,0));
    Sound("RankUp.ogg", true, 0, 100, GetPlayerByIndex(i) + 1);
  	// Das Zeug macht angeblich die Sachen tausend mal toller!111. trolol, betatestgirl.
  }
  
	FakeScriptGo(0);
	
	return true;
}

global func LoadMinigame(id idMinigame) { return GameCall("LoadTutorial", idMinigame, 0, [g_Gamemaster]); }

