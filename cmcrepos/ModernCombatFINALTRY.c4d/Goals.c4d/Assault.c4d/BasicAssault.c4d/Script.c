/*-- Basis-Assault --*/

#strict 2
#include TEAM

local aSpawn;			//Spawnpunkte
local aTargets;			//Ziele

global func IsAssaultTarget()	{return GetEffect("IntAssaultTarget", this);}
public func IsCMCAssaultGoal()	{return GetID() != CASS;}
public func CustomSpawnSystem()	{return true;}


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
  if(!FindObject(MCSL))
    for(var obj in FindObjects(Find_Func("IsClonk")))
      ScheduleCall(this, "OnClassSelection", 1, 0, obj);
}

/* Globale Funktionen */

global func ReportAssaultTargetDestruction()
{
  var goal = FindObject2(Find_Func("IsCMCAssaultGoal"));
  if(goal)
    return goal->ReportAssaultTargetDestruction(...);
}

global func AddAssaultTarget()
{
  var goal = FindObject2(Find_Func("IsCMCAssaultGoal"));
  if(goal)
    return goal->AddAssaultTarget(...);
}

global func GetAssaultTarget()
{
  var goal = FindObject2(Find_Func("IsCMCAssaultGoal"));
  if(goal)
    return goal->GetAssaultTarget(...);
}

global func Find_InArray(array a)
{
  var end = [C4FO_Or];
  for(var i; i < GetLength(a); i++)
    if(GetType(a[i]) == C4V_C4Object)
      end[i+1] = [C4FO_Not, [C4FO_Exclude, a[i]]];
  return end;
}

/* Zielobjekt Funktionen */

public func AddAssaultTarget(id idTarget, int iX, int iY, int iMaxDamage, int iTeam, string szName, int iIndex, array aSpawns, bool fNoBar)
{
  //Grundobjekt erstellen
  var fake = CreateObject(AHBS, iX, iY+GetDefCoreVal("Offset", 0, idTarget, 1)+2, -1);
  //Und Original imitieren
  SetShape(GetDefCoreVal("Offset", 0, idTarget), GetDefCoreVal("Offset", 0, idTarget, 1), GetDefCoreVal("Width", 0, idTarget), GetDefCoreVal("Height", 0, idTarget), fake);
  SetGraphics(0, fake, idTarget, 1, 1);
  fake->SetImitationID(idTarget);
  //Team setzen
  fake->SetTeam(iTeam);
  //Name
  if(szName)
    SetName(szName, fake);
  else
    SetName(GetName(0, idTarget), fake);
  //Ins Array packen
  if(!aTargets[iTeam])
    aTargets[iTeam] = [];
  aTargets[iTeam][iIndex] = fake;
  //Relaunchpositionen
  if(!aSpawn[iTeam])
    aSpawn[iTeam] = [];
  aSpawn[iTeam][iIndex] = aSpawns;
  //Assault-Effekt
  AddEffect("IntAssaultTarget", fake, 1, 1, this, 0, iMaxDamage, fNoBar, idTarget);
  //Done.
  return true;
}

public func ReportAssaultTargetDestruction(object pTarget, int iTeam)
{
  if(GetIndexOf(pTarget, aTargets[iTeam]) == -1)
    return;

  if(GetID(pTarget) == AHBS)
  {
    var aDmg = pTarget->GetDamager();
    var iPlr = aDmg[1];
        aDmg = aDmg[0];

    //Punkte für den letzten Killer
    if(GetPlayerTeam(iPlr) == iTeam)
    {
      //Punkte bei Belohnungssystem (Team-Zielobjektzerstörung)
      DoPlayerPoints(BonusPoints("ASTeamDestruction"), RWDS_MinusPoints, iPlr, GetCrew(iPlr), IC03);
      //Geldbonus: -20 Clunker
      DoWealth(iPlr, -20);
    }
    else
    {
      //Punkte bei Belohnungssystem (Zielobjektzerstörung)
      DoPlayerPoints(BonusPoints("ASDestruction"), RWDS_TeamPoints, iPlr, GetCrew(iPlr), IC03);
      //Geldbonus: 30 Clunker
      DoWealth(iPlr, 30);
    }

    //Punkte für alle Helfer
    for (var i; i < GetLength(aDmg); i++)
    if(GetPlayerName(i) && i != iPlr && aDmg[i])
    {
      if(GetPlayerTeam(i) == iTeam)
      {
        //Punkte bei Belohnungssystem (Hilfe bei Team-Zielobjektzerstörung)
        DoPlayerPoints(BonusPoints("ASTeamDestructionAssist"), RWDS_MinusPoints, i, GetCrew(i), IC03);
        //Geldbonus: -15 Clunker
        DoWealth(i, -15);
      }
      else
      {
        //Punkte bei Belohnungssystem (Hilfe bei Zielobjektzerstörung)
        DoPlayerPoints(BonusPoints("ASDestructionAssist"), RWDS_TeamPoints, i, GetCrew(i), IC03);
        //Geldbonus: 25 Clunker
        DoWealth(i, 25);
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

//Leisten-Offset in Pixeln, damit bei sich leerender Leiste die Grafik nicht verrutscht
static const CASS_BarOffset = 4;

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
  if(EffectVar(1, pTarget, iEffect))
    return;

  var team = pTarget->~GetTeam();

  //Erstmal die Leiste prüfen
  if(!EffectVar(2, pTarget, iEffect))
  {
    var bar = EffectVar(2, pTarget, iEffect) = CreateObject(SBAR, 0, 0, -1);
    bar->Set(pTarget, GetTeamColor(team), BAR_AssaultBar, 200);
    bar->ChangeDefHeight(GetObjHeight(pTarget));
    bar->ChangeDefOffset(-GetObjHeight(pTarget)/2+5);
    bar->PositionToVertex(0, true); 
  }
  var bar = EffectVar(2, pTarget, iEffect);

  //Schaden
  if(EffectVar(3, pTarget, iEffect) < GetDamage(pTarget))
    EffectVar(3, pTarget, iEffect) = GetDamage(pTarget);
  var dmg = EffectVar(3, pTarget, iEffect), maxdmg = EffectVar(0, pTarget, iEffect);

  //Bei zu viel Schaden zerstören
  if(GetDamage(pTarget) > maxdmg)
    return ReportAssaultTargetDestruction(pTarget, team);

  //Und die Leiste füllen...
  bar->Update(100 * (maxdmg-dmg) / maxdmg);
}

protected func FxIntAssaultTargetStop(object pTarget, int iEffect, int iCause, bool fTemp)
{
  if(!fTemp && EffectVar(2, pTarget, iEffect))
    RemoveObject(EffectVar(2, pTarget, iEffect));
}

/* Relaunch */

public func OnClassSelection()				{}
public func GetRespawnPoint(int &iX, int &iY, int iTeam){}

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