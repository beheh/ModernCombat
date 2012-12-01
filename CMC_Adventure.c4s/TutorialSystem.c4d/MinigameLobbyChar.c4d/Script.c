/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF, NOAM]; } 
public func SectionName() { return "LobbyChar"; } 
public func HideInMenu() { return true; } 

static g_Chars;

public func Initialize()
{
	g_Chars = [];
  FakeScriptGo(true);
  Music("@CMC_Beach Walk.ogg");
  
  return _inherited(...);
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	SetRespawnPosition(RandomX(340, 380), 170, iPlr);
	
	return _inherited(iPlr, ...);
}

public func CreateInterior()
{
	//Wählbare Charakter plazieren || Wir werden das ganze mit Skins machen. ;)
	var aSkins = ["", "Anti", "Artillerist", "Desert", "F", "Gulli", "PCMK", "Snow", "Soldier", "Vietnam", "VietnamSani", "Wood"];
  var iX = 40, iWdt = 180, iSkinCount = GetLength(aSkins);
  for(var i = 0; i < iSkinCount; i++)
  {
    var pClonk = CreateObject(SKIN, iX + i * (iWdt/iSkinCount), 190, -1);
    SetDir(1, pClonk); //SetCategory(2, pClonk);
    pClonk->SetSkin(aSkins[i]);
    BindTrigger(pClonk, TRIGGER_Collection, [], "PlayerSkin", this);
    //SetCategory(1, pClonk);
    SetColorDw(RGB(130, 130, 130), pClonk);
  }
  
  return true;
}

public func PlayerSkin(object pByObj)
{
	var plr = GetOwner(pByObj);
	g_Chars[plr] = LocalN("skin", Contained(pByObj));
	SetGraphics(g_Chars[plr], pByObj, SKIN);
	return true;
}

public func Script1()
{
	Message("$WelcomeMsg3$", g_Gamemaster);
	
	//Spielern Laufbefehl geben
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    var iPlr = GetPlayerByIndex(i);
    SetCommand(GetCrew(iPlr), "MoveTo", 0, 205 + Random(30), 205);
  }
  return true;
}

public func Script10()
{
	Message("$WelcomeMsg4$", g_Gamemaster);
	
	return true;
}

public func Script20() { Message("$TimeRemaining$", 0, 5); }
public func Script23() { Message("$TimeRemaining$", 0, 4); }
public func Script26() { Message("$TimeRemaining$", 0, 3); }
public func Script29() { Message("$TimeRemaining$", 0, 2); }
public func Script32() { Message("$TimeRemaining$", 0, 1); }
public func Script35() { Message("$TimeRemaining$", 0, 0); }

public func Script38() 
{ 
	Message("");
	Message("$WelcomeMsg5$", g_Gamemaster);
	for(var pClonk in FindObjects(Find_NoContainer(), Find_ID(PCMK), Find_Not(Find_Owner(NO_OWNER))))
    g_Chars[GetOwner(pClonk)] = "";
}

public func Script43()
{
	FakeScriptGo(0);
	LoadMinigame(MLBW);
}

