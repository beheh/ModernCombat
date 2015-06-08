/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF, NOAM]; } 
public func SectionName() { return "WalljumpPanic"; } 
public func HideInMenu() { return true; } 
public func IsCMCMinigame() { return true; }

local container;

public func Initialize()
{  
  container = CreateObject(TIM1, 0, 0, NO_OWNER);
  g_Gamemaster->Enter(container);
  BindTrigger(container, TRIGGER_Find, [[Find_ID(PCMK), Find_NoContainer(), Find_InRect(0, 0, 120, 30), Find_Action("Walk")]], "EvalRound", this, 0, true, 1);
  
  return _inherited(...);
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	SetRespawnPosition(55, 380, iPlr);
	
	return _inherited(iPlr, ...);
}

public func CreateInterior()
{
  return true;
}

public func LoadingFinished()
{
	Wait(35*5, -1, true, "StartGame()");
	return true;
}

public func StartGame()
{
	FakeScriptGo(true);
	return true;
}

public func EvalRound(array aObjects)
{
	var players = [];
	for(var obj in aObjects)
		players[GetLength(players)] = GetOwner(obj);
	
	g_aLastRoundWinners = players;
	return LoadMinigame(MLBW);
}

public func Script3()
{
	var rnd = Random(2);
	var x = LandscapeWidth()*rnd+((!rnd)*70)-35;
	var objects = [FGRN, FRAG, PGRN, SRBL, SGRN, STUN, AMPK, BBTP, C4EX, DGNN, FAPK, RSHL, CDBT, ATWN, ASTR, MNGN, PSTL, RVLR, RTLR, MISL, PPGN, SGST, SMGN, PPAR, ABOX, GBOX, MBOX, CUAM];
	var obj = CreateObject(ROCK, x, 5, -1);
	var picID = objects[Random(GetLength(objects))];
	obj->SetShape(GetDefOffset(picID, 0), GetDefOffset(picID, 1), GetDefWidth(picID), GetDefHeight(picID));
	obj->SetPicture(GetDefCoreVal("Picture", "DefCore", picID, 0), GetDefCoreVal("Picture", "DefCore", picID, 1), 
									GetDefCoreVal("Picture", "DefCore", picID, 2), GetDefCoreVal("Picture", "DefCore", picID, 3));
	obj->SetName(GetName(0, picID));
	obj->SetGraphics("", 0, picID, 0, 0, 0, GFX_BLIT_Additive);
	FadeOut4K(3, obj);
	
	return FakeGoto(0);
}

