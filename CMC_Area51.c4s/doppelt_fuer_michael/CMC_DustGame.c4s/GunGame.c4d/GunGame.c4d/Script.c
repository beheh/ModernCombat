/*-- Gun Game --*/

#strict 2
#include TEAM

local aWeaponList, aWeaponDefs, aMessage, aWeaponProgress;
local fFulfilled, iLevelUpStep;

static const GGNG_MaxWeaponSelect = 10; //Anzahl an Waffen die maximal ausgewaehlt werden duerfen

/***
 * TODO:
 * - Presetliste
 * - Sortierung bei Waffenauswahlliste (Waffen ganz oben, danach Equipment, danach Granaten)
 * - Beim Loeschen der Listeneintraege die Selection beruecksichtigen (nicht GetIndexOf)
 * - Beim Wechseln der Waffe achten, dass Ueberreste auch ordnungsgemaeß geloescht werden (ergo kein uebriges C4, Sprengfallen, Granaten, etc.)
 *   => Aufpassen das es sich dabei um keine Bonusitems handelt!!
 * - Granaten gestacked (sodass man auch nicht zu zielen aufhoeren muss), andererseits hat man dann den Selection-Cooldown, koennte das Spammen etwas verhindern.
 */

public func IsConfigurable()	{return true;}
public func GoalExtraValue()	{return 0;}	//Spielzielinformationen an Scoreboard weitergeben
public func ForceRuleActivation() { return [NOFD, NOHC, NOAT, NOAM]; }
public func ForceRuleDeactivation() { return [MCSL, SUIC]; }

/* Initialisierung */

protected func Initialize()
{
  aMessage = [];
  aWeaponList = [];
  aWeaponProgress = [];
  aWeaponDefs = [];
  iLevelUpStep = 2;

	GenerateRandomList(0, true);
  return true;
}

public func GenerateAvailableDefList() {
	aWeaponDefs = [];

  //Liste von Waffen generieren die zur Auswahl stehen
  var def, i = 0;
  while(def = GetDefinition(i++, C4D_Object))
  	if(ItemIsAllowed(def)) 
  		aWeaponDefs[GetLength(aWeaponDefs)] = def;

	return aWeaponDefs;
}

public func ItemIsAllowed(id def) {
	//Waffen (welche auch keine Errors werfen) sind erlaubt
	if(def->~IsWeapon2() && def->~ClonkCanUseWeapon() != NO_SRSLY_BROKEN) {
		if(def->~ClonkCanUseWeapon() == POCKET_APACHE)
			return GetModifier(GGNG_Mod_FullWeapons);
		else
			return true;
	}
	
	//Ausruestungsgegenstaende die offensiv eingesetzt koennen aktivieren, wenn Modifier aktiviert
	if(def->~IsOffensiveEquipment())
		return GetModifier(GGNG_Mod_OffEquipment);
	
	//Granaten fuer offensive Benutzung (oder man blendet den Gegner bis der in den Abgrund springt, auch eine Moeglichkeit) erlauben, wenn Modifier aktiviert
	if(def->~IsGrenade() && def->~IsOffensiveGrenade())
		return GetModifier(GGNG_Mod_OffGrenades);

	return false;
}

protected func Activate(iPlr)
{
  return MessageWindow(GetDesc(), iPlr);
}

public func ChooserFinished()
{
  var i = AddEffect("IntGoal", this, 1, 150, this);
  //SuddenDeath-Timer berechnen (im Moment nur Anzahl Kills fuer Sieg * 1.25 Minute)
  EffectVar(0, this, AddEffect("SuddenDeathWait", this, 1, 150, this)) = iLevelUpStep*GetLength(aWeaponDefs)*75*36;
  ScheduleCall(this, "UpdateScoreboard", 1);

  return _inherited(...);
}

local iChoosedPlr;

public func ConfigMenu(object pCaller)
{
  iChoosedPlr = GetOwner(pCaller);
  OpenGoalMenu();
  return 1;
}

private func ConfigFinished()
{
  var chos = FindObject(CHOS);
  if(chos)
    chos->OpenMenu();
}

/* Hostmenü */

private func OpenGoalMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChoosedPlr);
  CreateMenu(GetID(),pClonk,0,0,0,0,1);
  
  AddMenuItem("$OpenWeaponListChooser$", "OpenWeaponListChooser", WPN2, pClonk);
  AddMenuItem("$OpenKillStepMenu$", "OpenKillStepMenu", WPN2, pClonk);
  AddMenuItem("$OpenModifierMenu$", "OpenModifierMenu", WPN2, pClonk);

  //Fertig
  AddMenuItem("$Finished$", "ConfigFinished", GOCC, pClonk,0,0,"$Finished$",2,3);

  SelectMenuItem(iSelection, pClonk);
}

protected func FxIntGoalTimer()
{
  UpdateScoreboard();
  if(IsFulfilled())
  {
    Schedule("GameOver()", 150);
    RewardEvaluation();
    Sound("Cheer.ogg", true);
    return -1;
  }
}

/* Waffenwaehler */

private func OpenWeaponListChooser(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChoosedPlr);
  CreateMenu(GetID(),pClonk,0,0,0,0,1);
  
  AddMenuItem("$GenerateRandom$", "GenerateRandomList", WPN2, pClonk);
  AddMenuItem("$SelectPreset$", "OpenPresetSelection", WPN2, pClonk);
  AddMenuItem("$ClearList$", "ClearList", WPN2, pClonk);
  AddMenuItem(" ", 0, 0, pClonk);

	for(var wpn in aWeaponList)
		AddMenuItem(GetName(0, wpn), "RemoveWeaponFromList", wpn, pClonk);

	//Nur so viele Eintraege wie es auch Waffen gibt (oder GGNG_MaxWeaponSelect)
	if(GetLength(aWeaponList) < GetLength(aWeaponDefs) && GetLength(aWeaponList) < GGNG_MaxWeaponSelect)
		AddMenuItem("$AddWeaponToList$", "OpenWeaponSelection", WPN2, pClonk);

  //Zurueck (nur wenn mind. 1 Waffe angegeben ist)
  if(GetLength(aWeaponList))
  	AddMenuItem("$Back$", "OpenGoalMenu", GOCC, pClonk,0,0,"$Back$",2,3);
  else
  	AddMenuItem("<c 777777>$Back$</c>", 0, GOCC, pClonk,0,0,"$Back$",2,3);

  SelectMenuItem(iSelection, pClonk);
}

private func GenerateRandomList(id dummy, bool fNoMenu) {
	//Ggf. Itemliste aktualisieren
	GenerateAvailableDefList();

	var aTempDefs = aWeaponDefs;
	aWeaponList = [];
	
	while(GetLength(aTempDefs) && GetLength(aWeaponList) < GGNG_MaxWeaponSelect/2) {
		var i = Random(GetLength(aTempDefs));
		aWeaponList[GetLength(aWeaponList)] = aTempDefs[i];
		DelArrayItem(aTempDefs, i);
	}
	
	if(fNoMenu)
		return true;

	return OpenWeaponListChooser(0, 0);
}

private func OpenPresetSelection() {
	//TODO. Gibt erstmal wichtigeres.
	
	return OpenWeaponListChooser(0, 1);
}

private func ClearList() {
	aWeaponList = [];

	return OpenWeaponListChooser(0, 2);
}

private func RemoveWeaponFromList(id weapon) {
	var index = GetIndexOf(weapon, aWeaponList);
	if(index == -1)
		return OpenWeaponListChooser(0, 2);
	
	DelArrayItem(aWeaponList, index);
	return OpenWeaponListChooser(0, 4+index);
}

private func OpenWeaponSelection() {
  var pClonk = GetCursor(iChoosedPlr);
  CreateMenu(GetID(),pClonk,0,0,0,0,1);

	//Waffenauswahl anzeigen
	for(var wpn in aWeaponDefs)
		//if(GetIndexOf(wpn, aWeaponList) == -1)
			AddMenuItem(GetName(0, wpn), "AddWeaponToList", wpn, pClonk);
	
	//Fertig
  AddMenuItem("$Back$", "OpenWeaponListChooser", GOCC, pClonk,0,0,"$Back$",2,3);
	return true;
}

private func AddWeaponToList(id weapon) {
	aWeaponList[GetLength(aWeaponList)] = weapon;
	
	return OpenWeaponListChooser(0, 3+GetLength(aWeaponList));
}

private func OpenKillStepMenu(id dummy, int iSelection) {
	var pClonk = GetCursor(iChoosedPlr);
  CreateMenu(GetID(),pClonk,0,0,0,0,1);
  
  AddMenuItem(" ", 0, IC20, pClonk, iLevelUpStep);
  AddMenuItem("$IncreaseLvlUpStep$", "ChangeLvlUpStep(+1, 1)", WPN2, pClonk);
  AddMenuItem("$DecreaseLvlUpStep$", "ChangeLvlUpStep(-1, 2)", WPN2, pClonk);
  AddMenuItem("$Back$", "OpenGoalMenu", GOCC, pClonk,0,1,"$Back$",2,3);

  SelectMenuItem(iSelection, pClonk);
}

private func ChangeLvlUpStep(int iChange, int iSelection) {
	iLevelUpStep = BoundBy(iLevelUpStep+iChange, 1, 4);
	return OpenKillStepMenu(0, iSelection);
}

/* Modifier */

local iModifierFlags;

static const GGNG_Mod_FullWeapons = 1;
static const GGNG_Mod_OffEquipment = 2;
static const GGNG_Mod_OffGrenades = 4;
static const GGNG_Mod_RewardOnKill = 8;

private func OpenModifierMenu(id dummy, int iSelection) {
	var pClonk = GetCursor(iChoosedPlr);
  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);

	if(GetModifier(GGNG_Mod_FullWeapons))
		AddMenuItem("$ModifierFullWeapons$", "SwitchModifier(GGNG_Mod_FullWeapons, 0)", WPN2, pClonk);
  else
  	AddMenuItem("<c 777777>$ModifierFullWeapons$</c>", "SwitchModifier(GGNG_Mod_FullWeapons, 0)", WPN2, pClonk);
  
  if(GetModifier(GGNG_Mod_OffEquipment))
		AddMenuItem("$ModifierOffEquipment$", "SwitchModifier(GGNG_Mod_OffEquipment, 1)", WPN2, pClonk);
	else
		AddMenuItem("<c 777777>$ModifierOffEquipment$</c>", "SwitchModifier(GGNG_Mod_OffEquipment, 1)", WPN2, pClonk);
  
  if(GetModifier(GGNG_Mod_OffGrenades))
		AddMenuItem("$ModifierOffGrenades$", "SwitchModifier(GGNG_Mod_OffGrenades, 2)", WPN2, pClonk);
  else
  	AddMenuItem("<c 777777>$ModifierOffGrenades$</c>", "SwitchModifier(GGNG_Mod_OffGrenades, 2)", WPN2, pClonk);

  if(GetModifier(GGNG_Mod_RewardOnKill))
		AddMenuItem("$ModifierRewardOnKill$", "SwitchModifier(GGNG_Mod_RewardOnKill, 3)", WPN2, pClonk);
	else
		AddMenuItem("<c 777777>$ModifierRewardOnKill$</c>", "SwitchModifier(GGNG_Mod_RewardOnKill, 3)", WPN2, pClonk);

  AddMenuItem("$Back$", "OpenGoalMenu", GOCC, pClonk,0,2,"$Back$",2,3);

  SelectMenuItem(iSelection, pClonk);
}

private func SwitchModifier(int iModifier, int iSelection) {
	iModifierFlags ^= iModifier;
	
	GenerateAvailableDefList();
	return OpenModifierMenu(0, iSelection);
}

private func GetModifier(int iModifier) { return iModifierFlags & iModifier; }

/* Relaunch */

protected func InitializePlayer(int iPlr)
{
  //Wird noch eingestellt
  if(FindObject(CHOS))
    return;

  //Scoreboard
  UpdateScoreboard();
}

protected func RemovePlayer(int iPlr)
{
  //Scoreboard
  UpdateScoreboard();
}

/* Scoreboard */

static const GGNG_Icon = 0;
static const GGNG_WeaponInfo = 1;

public func UpdateScoreboard()
{
  //Wird noch eingestellt
  if(FindObject(CHOS)) return;

  //Titelzeile
  SetScoreboardData(SBRD_Caption, SBRD_Caption, GetName());
  
  //Kopfzeile
  SetScoreboardData(SBRD_Caption, GGNG_Icon, "$WeaponList$", 0);
  
  //Waffenliste anzeigen
  for(var i = 0, str = ""; i < GetLength(aWeaponList); i++) {
  	str = Format("%s{{%i}}", str, aWeaponList[i]);

  	//Ab mehr als 5 Eintraege mit gleichmaessigen Zeilenumbruechen anzeigen
  	if(GetLength(aWeaponList) > 5 && i == GetLength(aWeaponList)/2-1)
  		str = Format("%s|", str);
  }
  SetScoreboardData(SBRD_Caption, GGNG_WeaponInfo, str);
  
  SetScoreboardData(0, GGNG_WeaponInfo, " ");

	var row = 1;
	//Teams auflisten
  if(Teams()) {
    var engine_teams = GetTeamConfig(TEAM_AutoGenerateTeams);
    for(var i = 0; i < GetTeamCount(); i++) {
      var team = GetTeamByIndex(i);
      var name = GetTaggedTeamName(team);
      var clr = GetTeamColor(team);
      var plr;

      //Kein Spieler im Team? Dann nicht anzeigen
      if(GetTeamMemberByIndex(team, 0) == -1)
        continue;

			var j = 0;
			while(plr = GetTeamMemberByIndex(team, j++)+1) {
				plr--;

				SetScoreboardData(row, GGNG_Icon, GetTaggedPlayerName(plr, true));

				var progress = GetPlayerProgress(plr);
				var clr = 0xFF0000;
				if(GetLength(aWeaponList) > progress/iLevelUpStep+1)
					clr = 0xAAAAAA;

				//Naechste Waffe oder Endgrafik anzeigen
				var nextWeapon = aWeaponList[progress/iLevelUpStep+1];
				if(!nextWeapon)
					nextWeapon = ROCK;

				SetScoreboardData(row, GGNG_WeaponInfo, Format("{{%i}} <c %x>(+%d)</c> {{%i}}", aWeaponList[progress/iLevelUpStep], clr, (progress%iLevelUpStep)||iLevelUpStep, nextWeapon));
				row++;
			}
      /*
      //Bei Engine-Teams statt "Team X" und Teamfarbe, Spielername und Spielerfarbe (falls nur ein Spieler im Team)
      if(engine_teams && GetTeamPlayerCount(team) <= 1 && (plr = GetTeamMemberByIndex(team, 0)) > -1) {
        name = GetTaggedPlayerName(plr, true);
        clr = GetPlrColorDw(plr);
      }

      SetScoreboardData(team, GMNR_Name, name);
      SetScoreboardData(team, GMNR_Count, Format("<c %x>%d</c>", clr, aMoney[team]), aMoney[team]);*/
    }
  }
  else
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      var plr = GetPlayerByIndex(i);
      SetScoreboardData(row, GGNG_Icon, GetTaggedPlayerName(plr, true));

			var progress = GetPlayerProgress(plr);
			var clr = 0xFF0000;
			if(GetLength(aWeaponList) > progress/iLevelUpStep+1)
				clr = 0xAAAAAA;

			//Naechste Waffe oder Endgrafik anzeigen
			var nextWeapon = aWeaponList[progress/iLevelUpStep+1];
			if(!nextWeapon)
				nextWeapon = ROCK;

			SetScoreboardData(row, GGNG_WeaponInfo, Format("{{%i}} <c %x>(+%d)</c> {{%i}}", aWeaponList[progress/iLevelUpStep], clr, (progress%iLevelUpStep)||iLevelUpStep, nextWeapon));
			row++;
    }
}

/* Relaunch */

public func OnDeathAnnounce(object pCrew, int iKiller, int iAssist)
{
  var iPlr = GetOwner(pCrew);

  if(!GetPlayerName(iPlr) || (!pCrew && !(pCrew = GetCrew(iPlr))))
    return false;

  var iChange;
  //Ungeklärte Ursache oder Selbstmord
  if(iKiller == NO_OWNER || iPlr == iKiller)
    Message(); //Platzhalter.
  else
    //Teamkill
    if(GetPlayerTeam(iPlr) == GetPlayerTeam(iKiller))
      Message(); //Platzhalter
    //Gegner getötet
    else
      aWeaponProgress[GetPlayerID(iKiller)]++;

	//Keine weitere Waffe in der Liste verfuegbar? Runde abschliessen
	if(!GetPlayerWeapon(iKiller))
		IsFulfilled();
	//Ansonsten ggf. Killer mit neuer Waffe ausstatten
	else if(!FindObject2(Find_ID(GetPlayerWeapon(iKiller)), Find_Container(GetCrew(iKiller)))) {
		//Bei SuddenDeath den Killer gewinnen lassen
		if(SuddenDeathActive()) {
			aWeaponProgress[GetPlayerID(iKiller)] = GetLength(aWeaponList)*iLevelUpStep;
			IsFulfilled();
			return true;
		}
		EquipWeapon(iKiller, GetCrew(iKiller));
	}

  //Credits verteilen
  Money(iPlr, pCrew, iKiller);

  //Scoreboard
  UpdateScoreboard();

  //Effekte
  Sound("GetCash*.ogg", false, pCrew);
  Sound("PaperFly*.ogg", false, pCrew);
  while((iChange++) < 0)
    CreateParticle("Paper2", GetX(pCrew) - GetX() + Random(13) - 6, GetY(pCrew) - GetY() - Random(4), Random(7) - 3, -Random(6), 40 + Random(16), HSLa(Random(256), 192 + Random(64), 96 + Random(64)));
}

public func RelaunchPlayer(int iPlr, object pCrew, int iKiller)
{
  Money(iPlr, pCrew, iKiller);

	return true;
}

public func OnClassSelection(object pCrew) {
	var iPlr = GetOwner(pCrew);
	
	EquipWeapon(iPlr, pCrew);
  
  //Sonstige Ausruestung
  var aGear = GameCall("SpecificEquipment") || [];

  if(GetDarkness() >= 3 && NoAttachments())
    aGear[GetLength(aGear)] = [FLSH, 1];

  //Bei Fallschaden Fallschirme als Zusatzausrüstung
  if(FindObject(FDMG))
    aGear[GetLength(aGear)] = [PPAR, 1];
  
  var aGearTypes = [];

  for(var aEntry in aGear)
  {
    if(!aEntry[0])
      continue;

    if(GetIndexOf(aEntry[0]->~GetGearType(), aGearTypes) > -1)
      continue;

    aGearTypes[GetLength(aGearTypes)] = aEntry[0]->~GetGearType();

    if(GetType(aEntry) == C4V_Array && GetType(aEntry[0]) == C4V_C4ID && aEntry[0]->~IsHazardGear())
      while(aEntry[1]--)
        CreateObject(aEntry[0], 0, 0, iPlr)->~Activate(pCrew);
  }
  
  //Skins
  var aSkins = [[SKIN, "Antiskill", 2], [SKIN, "Artillerist", 2], [SKIN, "Supporter", 2], [SKIN, "Assault", 2], [SKIN, "Medic", 2]];
  if(GetLength(aSkins))
  {
    var skin = aSkins[Random(GetLength(aSkins))];
    SetGraphics(skin[1], pCrew, skin[0]);
    if(skin[2])
      SetPortrait(Format("Ptr%s%d", skin[1], Random(skin[2])+1), pCrew, skin[0]);
  }
  
  return true;
}

public func GetPlayerProgress(int iPlr) { return aWeaponProgress[GetPlayerID(iPlr)]; }
public func GetPlayerWeapon(int iPlr) { return aWeaponList[GetPlayerProgress(iPlr)/iLevelUpStep]; }

//TODO: Throwaway-Animation fuer vorherige Waffe
public func EquipWeapon(int iPlr, object pCrew, bool fNoThrowaway) {
	//Alte Waffen entfernen
	if(!fNoThrowaway) {
		for(var obj in FindObjects(Find_Container(pCrew), Find_Or(Find_Func("IsWeapon2"), Find_Func("IsOffensiveEquipment"), Find_Func("IsOffensiveGrenade")))) {
			if(!GetEffect("IntKeepObject", obj)) {
				RemoveObject(obj);
				continue;
			}

			RemoveEffect("IntKeepObject", obj);
		
			//Duplikat als Throwaway verwenden (da das Objekt sonst sofort geloescht wird)
			var throwaway = CreateObject(GetID(obj), 0, 0, -1);

			//Kategorie wechseln und Interaktion ausschliessen
			SetCategory(C4D_Vehicle, throwaway);
			SetObjectLayer(throwaway, throwaway);
			SetPosition(GetX(obj), GetY(obj), throwaway);

			//Waffe auswerfen
			if(Contained(obj)) {
				var dir = +10, rot = 0;
				if(GetDir(pCrew) == DIR_Right) {
				  dir = -10;
				  rot = 180;
				}

				throwaway->SetR(rot);
				throwaway->SetXDir(dir);
				throwaway->SetYDir(-30);
				throwaway->SetRDir(RandomX(-8, 8));

				Sound("AT4R_ThrowAway.ogg", 0, throwaway);
			}

			throwaway->FadeOut();
			if(obj)
				RemoveObject(obj);
		}
	}
	
	if(!GetPlayerWeapon(iPlr))
		return;

	var wpn = CreateContents(GetPlayerWeapon(iPlr), pCrew);
	AddEffect("IntKeepObject", wpn, 1, 0);

	if(!wpn->~IsWeapon2())
		return true;

	//Waffe auffuellen
	while(wpn->~CycleFM(+1))
  {
    var ammo = wpn->GetFMData(FM_AmmoID);
    var load = wpn->GetFMData(FM_AmmoLoad);
    if(wpn->GetAmmo(ammo) == load) break;
    //erst entladen
    DoAmmo(ammo,-load, wpn);
    //dann neu reinladen
    DoAmmo(ammo, load, wpn);
  }
  //noch ein letztes Mal
  wpn->~CycleFM(+1);

	return true;
}

global func Teams()
{
  return GetTeamConfig(TEAM_Active) && (!GetTeamConfig(TEAM_AutoGenerateTeams) || GetTeamConfig(TEAM_Dist) != 2);
}

/* Sieg */

public func IsFulfilled()
{
  //Wird noch eingestellt
  if(FindObject(CHOS)) return;

  if(fFulfilled) return true;

  //Siegerermittlung
  for(var i = 0; i < GetPlayerCount(); i++)
    if(!GetPlayerWeapon(GetPlayerByIndex(i)))
    {
      //Verlierer eliminieren
      for(var j = GetPlayerCount(); j >= 0; j--) {
      	if(Teams()) {
      		if(GetPlayerTeam(GetPlayerByIndex(i)) != GetPlayerTeam(GetPlayerByIndex(j)))
      			EliminatePlayer(GetPlayerByIndex(j));
        }
        else if(GetPlayerByIndex(j) != GetPlayerByIndex(i))
          EliminatePlayer(GetPlayerByIndex(j));
      }

      //Spielende planen
      Schedule("GameOver()", 150);

      //Auswertung
      RewardEvaluation();

      //Nachricht über Gewinner
      if(Teams()) {
      	var team = GetPlayerTeam(GetPlayerByIndex(i));
      	Message("$TeamHasWon$", 0, GetTeamColor(team), GetTeamName(team));
      }
      else
      	Message("$PlayerHasWon$", 0, GetPlrColorDw(GetPlayerByIndex(i)), GetPlayerName(GetPlayerByIndex(i)));

      //Sound
      Sound("Cheer.ogg", true);

      return fFulfilled = true;
    }

  //Nur noch eins übrig
  if(GetActiveTeamCount() <= 1)
  {
    var i = GetPlayerTeam(GetPlayerByIndex());

    //Spielende planen
    Schedule("GameOver()", 150);

    //Auswertung
    RewardEvaluation();

    //Nachricht über Gewinner
    Message("$TeamHasWon$", 0, GetTeamColor(i), GetTeamName(i));

    //Sound
    Sound("Cheer.ogg", true);

    return fFulfilled = true;
  }

  return false;
}

/* Sudden Death */

local fSuddenDeath;
public func SuddenDeathActive() { return fSuddenDeath; }

public func FxSuddenDeathWaitTimer(object pTarget, int iNr, int iTime) {
	//Nach 90% der Wartezeit warnen
	if(!EffectVar(1, pTarget, iNr) && iTime > EffectVar(0, pTarget, iNr)*9/10) {
		EffectVar(1, pTarget, iNr) = true;
		EventInfo4K(0, "$SuddenDeathWarning$", GGNG, 0, 0, 0, "Info_Alarm.ogg");
	}
	
	if(iTime > EffectVar(0, pTarget, iNr)) {
		InitializeSuddenDeath();
		return -1;
	}
}

public func InitializeSuddenDeath() {
	fSuddenDeath = true;
	
	EventInfo4K(0, "$SuddenDeathActive$", GGNG, 0, 0, 0, "Info_Alarm.ogg");
	
	//Evtl. Fortschritte zuruecksetzen?
}

/* Ungenutze Funktionen */

public func InitScoreboard()		{}
public func InitMultiplayerTeam()	{}
public func RemoveMultiplayerTeam()	{}
public func InitSingleplayerTeam()	{}
public func RemoveSingleplayerTeam()	{}
public func InitPlayer()		{}
public func RemoveScoreboardPlayer()	{}
public func SortTeamScoreboard()	{}
