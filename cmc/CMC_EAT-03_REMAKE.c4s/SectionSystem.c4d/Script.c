/*-- Sektionssystem --*/

#strict 2

local arRespawn;

public func IsCMCTutorialGoal()		{return true;}			//Nur Spielziele müssen zur Nutzung dieses Systems diese Funktion wiedergeben
public func TutorialIndex()		{return -1;}
public func UsedRules()			{return [];}			//Regeln, die das Tutorial verwendet
public func SectionName()		{return "";}			//Sektionsname
public func Condition(object pByObj)	{return true;}			//Freigabe
public func HideInMenu()		{return (GetID() == TSYS);}	//Anzeige im Menü


/* Initialisierung */

public func Initialize() 
{
  arRespawn = [];
  for(var i = 0; i < GetPlayerCount(); i++)
    if(GetType(arRespawn[GetPlayerByIndex(i)]) != C4V_Array)
    arRespawn[GetPlayerByIndex(i)] = [];
}

public func CreateInterior()		{ }
public func CreateEquipment()		{ }
public func CreateDecoration()		{ }
public func CreateOptionalFeatures()	{ }
public func CreateTriggers()		{ }

public func RejectRelaunch(int iPlr) { return false; } // bool - Relaunch ablehnen?

public func OnClonkRelaunch(object pCrew) { }

public func RelaunchPosition(int &iX, int &iY, object pCrew) { iX = arRespawn[GetOwner(pCrew)][0]; iY = arRespawn[GetOwner(pCrew)][1]; }

public func ExitArrived(object pByObj) // Wird aufgerufen, falls der Ausgang des Tutorials erreicht wurde. (pByObj -> Objekt, das den Ausgang betreten hat.)
{ 
	if(GetID() == TLBY)
		GameCall("TutorialChooser", pByObj);
	else
	{
		for(var i = 0; i < GetPlayerCount(); i++)
		{
			if(Contained(GetCursor(i)) != Contained(pByObj))
			{
				PlayerMessage(GetOwner(pByObj), "$WaitForOtherPlayers$", pByObj);
				return false;
			}
		}
		GameCall("LoadTutorial", TLBY);
	}
	
	return true;
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];

	var iX, iY;
	for(var pCrew in FindObjects(Find_OCF(OCF_CrewMember), Find_Owner(iPlr)))
		RelaunchPlayer(iPlr, pCrew, 0, GetPlayerTeam(iPlr));
	
	return true;
}

protected func RemovePlayer(int iPlr)
{
  var pCursor = GetCursor(iPlr);
  if(!pCursor) return;
  if(pCursor->~GetRealCursor()) pCursor = pCursor->~GetRealCursor();
  if(!pCursor->~IsFakeDeath()) return;
  pCursor->Kill();
}

/** Relaunch **/

public func RelaunchPlayer(int iPlr, object pCrew, killer, int iTeam, bool bFirst)
{
	//Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return false;
	
  //Falscher Cursor?
  if (!pCrew || !pCrew->~IsClonk())
    pCrew = GetCrew(iPlr);

  //Reject?
  if(!bFirst)
    if(RejectRelaunch(iPlr))
      return false;

  //Clonk tot?
  if(!GetAlive(pCrew))
    pCrew = RelaunchClonk(iPlr, pCrew);
	
  //Zufallsposition setzen
  var iX, iY;
  RelaunchPosition(iX, iY, pCrew);

  if(Contained(pCrew))
    SetPosition(iX, iY, Contained(pCrew));
  else
    SetPosition(iX, iY, pCrew);
    
  OnClonkRelaunch(pCrew);
}

public func RelaunchClonk(int iPlr, object pCursor)
{
  //Clonkerstellung
  var pClonk;
  if(pCursor && pCursor->~GetRealCursor()) pCursor = pCursor->~GetRealCursor();
  if(pCursor) 
  {
		pClonk = CreateObject(GetID(pCursor), 10, 10, iPlr);
    GrabObjectInfo(pCursor, pClonk);
  }
  else 
  {
		pClonk = CreateObject(PCMK, 10, 10, iPlr);
    MakeCrewMember(pClonk, iPlr);
  }
  DoEnergy(+150, pClonk);
  SetCursor(iPlr, pClonk);
  SetPlrView(iPlr, pClonk);
	
  //Clonknamen anzeigen
  //PlayerMessage(iPlr, Format("@%s", GetName(pClonk)), tim);

  return pClonk;
}

public func SetRespawnPosition(int iX, int iY, int iPlr) { arRespawn[iPlr] = [iX, iY]; }

/** ScriptCounter **/

static g_fakescriptcounter; // int - Scriptcounter-Wert.

global func FakeScriptGo(bool fStart, object pByObj)
{
	if(!pByObj && !(pByObj = this))
		return false;
	
	if(!GetEffect("FakeScriptCounter") && fStart)
		AddEffect("FakeScriptCounter", 0, 101, 10, 0, 0, pByObj);
	
	if(GetEffect("FakeScriptCounter") && !fStart)
		RemoveEffect("FakeScriptCounter");

	return true;
}

global func FxFakeScriptCounterStart(object target, int nr, temp, object pByObj)
{
	EffectVar(0, target, nr) = pByObj;
	
	return true;
}

global func FxFakeScriptCounterTimer(object target, int nr)
{
	if(!EffectVar(0, target, nr))
		return -1;

	g_fakescriptcounter++;
	EffectVar(0, target, nr)->Call(Format("~Script%d", g_fakescriptcounter));
	return true;
}

global func FakeGoto(int iCounter)
{
  g_fakescriptcounter = Max(iCounter, 0);
}