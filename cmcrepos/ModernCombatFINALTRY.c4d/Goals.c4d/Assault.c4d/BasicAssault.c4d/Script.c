/*-- Basis-Assault --*/

#strict 2
#include TEAM

local aSpawn;			//Spawnpunkte
local aTargets;			//Ziele

global func IsAssaultTarget()	{return GetEffect("IntAssaultTarget", this);}
public func IsCMCAssaultGoal()	{return GetID() != CASS;}

/* Initialisierung */

protected func Initialize()
{
  aTargets = [];
  aKill = [];
  aDeath = [];
  aSpawn = [];
}

protected func ChooserFinished()
{
  AddEffect("IntGoal", this, 1, 5, this);

  //Keine Klassenwahl? Alle relaunchen
  if (!FindObject(MCSL))
    for (var obj in FindObjects(Find_Func("IsClonk")))
      ScheduleCall(this, "OnClassSelection", 1, 0, obj);
}

/* Globale Funktionen */

global func ReportAssaultTargetDestruction()
{
  var goal = FindObject2(Find_Func("IsCMCAssaultGoal"));
  if (goal)
    return goal->ReportAssaultTargetDestruction(...);
}

global func AddAssaultTarget()
{
  var goal = FindObject2(Find_Func("IsCMCAssaultGoal"));
  if (goal)
    return goal->AddAssaultTarget(...);
}

global func Find_InArray(array a)
{
  var end = [C4FO_Or];
  for (var i; i < GetLength(a); i++)
    if (GetType(a[i]) == C4V_C4Object)
      end[i+1] = [C4FO_Not, [C4FO_Exclude, a[i]]];
  return end;
}

/* Zielobjekt Funktionen */

public func AddAssaultTarget(id idTarget, int iX, int iY, int iMaxDamage, int iTeam, string szName, int iIndex, array aSpawns, bool fNoBar) {
  //Grundobjekt erstellen
  var fake = CreateObject(AHBS, iX, iY+GetDefCoreVal("Offset", 0, idTarget, 1)+2, -1);
  //Und Original imitieren
  SetShape(GetDefCoreVal("Offset", 0, idTarget), GetDefCoreVal("Offset", 0, idTarget, 1), GetDefCoreVal("Width", 0, idTarget), GetDefCoreVal("Height", 0, idTarget), fake);
  SetGraphics(0, fake, idTarget, 1, 1);
  fake->SetImitationID(idTarget);
  //Team setzen
  fake->SetTeam(iTeam);
  //Name
  if (szName)
    SetName(szName, fake);
  else
    SetName(GetName(0, idTarget), fake);
  //Ins Array packen
  if (!aTargets[iTeam])
    aTargets[iTeam] = [];
  aTargets[iTeam][iIndex] = fake;
  //Relaunchpositionen
  if (!aSpawn[iTeam])
    aSpawn[iTeam] = [];
  aSpawn[iTeam][iIndex] = aSpawns;
  //Assault-Effekt
  AddEffect("IntAssaultTarget", fake, 1, 1, this, 0, iMaxDamage, fNoBar, idTarget);
  //Done.
  return true;
}

public func ReportAssaultTargetDestruction(object pTarget, int iTeam)
{
  if (GetIndexOf(pTarget, aTargets[iTeam]) == -1)
    return;

  if (GetID(pTarget) == AHBS)
  {
    var aDmg = pTarget->GetDamager();
    var iPlr = aDmg[1];
	aDmg = aDmg[0];
	//Der letzte Killer bekommt 50 Punkte. -50 wenn er im selben Team war.
	if (GetPlayerTeam(iPlr) == iTeam)
    {
	  DoPlayerPoints(-50, RWDS_MinusPoints, iPlr, GetCrew(iPlr), IC03);
	  DoWealth(iPlr, -50);
	}
	else
	{
	  DoPlayerPoints(50, RWDS_TeamPoints, iPlr, GetCrew(iPlr), IC03);
	  DoWealth(iPlr, 50);
	}

	//Array durchgehen. Jeder gegnerische Helfer bekommt 20 Punkte, jeder Teamabschuss -20
	for (var i; i < GetLength(aDmg); i++)
	  if (GetPlayerName(i) && i != iPlr && aDmg[i]) {
        if (GetPlayerTeam(i) == iTeam) {
		  DoPlayerPoints(-20, RWDS_MinusPoints, i, GetCrew(i), IC03);
		  DoWealth(i, -20);
		}
		else
                {
		  DoPlayerPoints(20, RWDS_TeamPoints, i, GetCrew(i), IC03);
		  DoWealth(i, 20);
		}
	  }
  }

  return true;
}

protected func Activate(int iPlr)
{
  return MessageWindow(GetDesc(), iPlr);
}

/* Assault-Effekt */

//Effekt-Vars
//0: Maximalschaden
//1: Leiste oder nicht
//2: Leiste
//3: Aktuell angezeigter Schaden (für "flüssige" Leiste)
//4: ID. Wichtig für FakeTarget

protected func FxIntAssaultTargetStart(object pTarget, int iEffect, int iTemp, int iDmg, bool fNoBar, id idTarget)
{
  EffectVar(0, pTarget, iEffect) = iDmg;
  EffectVar(1, pTarget, iEffect) = fNoBar;
  EffectVar(4, pTarget, iEffect) = idTarget;
}

protected func FxIntAssaultTargetTimer(object pTarget, int iEffect)
{
  //Ziel will keine Leiste
  if (EffectVar(1, pTarget, iEffect))
    return;

  //Erstmal die Leiste prüfen
  if (!EffectVar(2, pTarget, iEffect))
    EffectVar(2, pTarget, iEffect) = CreateObject(EBR2, GetX(pTarget), GetY(pTarget)+GetDefHeight(EffectVar(4, pTarget, iEffect))/2+10, -1);
  var bar = EffectVar(2, pTarget, iEffect);

  //Schaden
  if (EffectVar(3, pTarget, iEffect) < GetDamage(pTarget))
    EffectVar(3, pTarget, iEffect)++;
  var dmg = EffectVar(3, pTarget, iEffect), maxdmg = EffectVar(0, pTarget, iEffect), team = pTarget->~GetTeam();

  //Bei zu viel Schaden zerstören
  if (GetDamage(pTarget) > maxdmg)
    return ReportAssaultTargetDestruction(pTarget, team);

  //Und die Leiste füllen...
  SetPosition(GetX(pTarget), GetY(pTarget)+GetDefHeight(EffectVar(4, pTarget, iEffect))/2+10, bar);
  SetGraphics("Row", bar, GetID(bar), 1, 1, 0, 4);
  var permill = 1000*(maxdmg-dmg)/maxdmg;
  SetObjDrawTransform(permill, 0, (permill-1000)*GetDefWidth(GetID(bar))/2, 0, 1000, 0, bar, 1);
  SetClrModulation(GetTeamColor(team), bar, 1);
}

protected func FxIntAssaultTargetStop(object pTarget, int iEffect, int iCause, bool fTemp)
{
  if (!fTemp && EffectVar(2, pTarget, iEffect))
    RemoveObject(EffectVar(2, pTarget, iEffect));
}

/* Relaunch */

public func OnClassSelection() {}
public func GetRespawnPoint(int &iX, int &iY, int iTeam) {}

/* Ungenutzte Funktionen */

private func InitScoreboard()			{}
private func InitMultiplayerTeam(int iTeam)	{}
private func RemoveMultiplayerTeam(int iTeam)	{}
private func InitSingleplayerTeam(int iPlr)	{}
private func RemoveSingleplayerTeam(int iPlr)	{}
private func InitPlayer(int iPlr)		{}
private func RemoveScoreboardPlayer(int iPlr)	{}
public func WinScoreChange(int iNewScore)	{}
private func SortTeamScoreboard()		{}
private func TeamGetScore()			{}