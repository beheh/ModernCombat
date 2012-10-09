/*-- Klassenwahl --*/

#strict 2

local crew;
local lastclass;
local selection;


/* Initialisierung */

protected func Initialize()
{
  SetPosition(10,10);
  crew = [];
  lastclass = [];
  selection = [];
  ScheduleCall(0,"Initialized",1);
}

private func Initialized()
{
  //Verschwinden wenn Waffenwahl oder InstaGib im Spiel sind
  if(FindObject(WPCH) || FindObject(IGIB))
    RemoveObject();
}

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

/* (Re-)Spawn */

func InitializePlayer(int iPlayer)
{
  var pClonk = GetCrew(iPlayer);
  if(!pClonk)
  {
    ScheduleCall(0,"InitializePlayer",1,0,iPlayer);
    return;
  }

  if(FindObject(CHOS))
    return;

  ScheduleCall(0,"InitClassMenu",1,0,pClonk);
}

func ChooserFinished()
{
  ScheduleCall(0,"InitPlayers",1);
}

func InitPlayers()
{
  for(var i = 0; i < GetPlayerCount(); i++)
    InitializePlayer(GetPlayerByIndex(i));
}

public func RelaunchPlayer(int iPlr, object pClonk)
{
  if(!pClonk)
    if(!(pClonk = GetCrew(iPlr)))
      return ScheduleCall(this,"RelaunchPlayer",1,0,iPlr,pClonk);
  if(!GetAlive(pClonk))
    return ScheduleCall(this,"RelaunchPlayer",1,0,iPlr);

  //Men¸ zeitverzˆgert erstellen
  if(!FindObject(CHOS))
    ScheduleCall(0,"InitClassMenu",10,0,pClonk);

  return;
}

/* Spawntimer */

public func FxSpawntimerStart(pTarget, iNo, iTemp, iPlr, pClonk, cont)
{
  if(iTemp)
    return -1;
  if(iPlr < 0)
    return -1;

  //EffectVars
  EffectVar(0, pTarget, iNo) = iPlr;	//Spieler
  EffectVar(1, pTarget, iNo) = 15;	//Zeit
  EffectVar(2, pTarget, iNo) = pClonk;	//Clonk
  EffectVar(3, pTarget, iNo) = cont;	//Container

  PlayerMessage(EffectVar(0, pTarget, iNo), "@$TimeTillRespawn$", 0, EffectVar(1, pTarget, iNo));
}

public func FxSpawntimerTimer(pTarget, iNo, iTime)
{
  EffectVar(1, pTarget, iNo)--;
  PlayerMessage(EffectVar(0, pTarget, iNo), "@$TimeTillRespawn$", 0, EffectVar(1, pTarget, iNo));

  //Verschwinden wenn Clonk/Beh‰lter weg oder Clonk nicht im Beh‰lter
  if (!EffectVar(2, pTarget, iNo) || !EffectVar(3, pTarget, iNo) || Contained(EffectVar(2, pTarget, iNo)) != EffectVar(3, pTarget, iNo)) {
    //Verschwinden wenn Beh‰lter noch vorhanden und TIM1
    if (GetID(EffectVar(3, pTarget, iNo)) == TIM1)
      RemoveObject(EffectVar(3, pTarget, iNo));
    return -1;
  }

  if(EffectVar(1, pTarget, iNo) <= 0)
  {
    var iPlr = EffectVar(0, pTarget, iNo),
    class = CalculatePlayerSelection(iPlr, selection[iPlr]);
    
    PlayerMessage(iPlr, "@");
    if(SetupClass(class, iPlr))
      return -1;
    class = lastclass[iPlr];
    if (class && SetupClass(class, iPlr))
      return -1;
    class = 1;
    SetupClass(class, iPlr);
    return -1;
  }
}

public func FxSpawntimerStop(pTarget, iNo, iReason, fTemp)
{
  if (!fTemp)
    PlayerMessage(EffectVar(0, pTarget, iNo), "@");
}

func InitClassMenu(object pClonk)
{
  //Kein Clonk?
  if(!pClonk)
    return;

  var iPlayer = GetOwner(pClonk);

  crew[iPlayer] = pClonk;

  //Clonk in Spawnpoint verschieben
  if(GetID(Contained(pClonk)) != TIM1)
  {
    var tmp = CreateObject(TIM1, AbsX(GetX(pClonk)), AbsY(GetY(pClonk)), iPlayer);
    SetCategory(GetCategory(tmp) | C4D_Foreground, tmp);
    SetGraphics(0, tmp, GetID(pClonk), 1, GFXOV_MODE_Object, 0, 1, pClonk);
    if(FindObject(GOCC))
      SetVisibility(VIS_None, tmp);
    else
      SetVisibility(VIS_Owner, tmp);
    Enter(tmp,pClonk);
  }

  //Zeitbegrenzung bei LMS, DM und AS
  if(FindObject(GLMS) || FindObject(GTDM) || FindObject(GASS))
    AddEffect("Spawntimer", this, 100, 35, this, GetID(), iPlayer, pClonk, tmp);

  //Bereits ein Men¸ offen?
  if(GetMenu(pClonk))
    CloseMenu(pClonk); //Men¸ schlieﬂen

  AddEffect("ClassMenu", pClonk, 1, 10, this);
}

func Finish(object pClonk, int iClass)
{
  if(!pClonk || !Contained(pClonk)) return;
  var iPlayer = GetOwner(pClonk);

  //Men¸ schlieﬂen
  CloseMenu(GetCursor(iPlayer));

  //Alle Waffen auff¸llen
  for(var wpn in FindObjects(Find_Container(pClonk), Find_Func("IsWeapon")))
  {
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
  }

  pClonk->~UpdateCharge();

  //Aus Spawnpoint entlassen
  if(GetID(Contained(pClonk)) == TIM1) RemoveObject(Contained(pClonk),true);

  //Sound
  Sound("RSHL_Deploy.ogg", 0, pClonk, 100, GetOwner(pClonk)+1);

  //Effekt entfernen
  for(var i = 0; i < GetEffectCount("Spawntimer", this); i++)
    if(EffectVar(0, this, GetEffect("Spawntimer", this, i)) == iPlayer)
      RemoveEffect("Spawntimer", this, i);
  PlayerMessage(iPlayer, "@");

  //Broadcasten
  GameCallEx("OnClassSelection", crew[iPlayer], iClass);
}

/* Men¸ */

private func InfoMenuItems()
{
  return 6 + !FindObject(NOAM);
}

public func FxClassMenuTimer(object pTarget)
{
  if(!GetMenu(pTarget))
    OpenMenu(pTarget, selection[GetOwner(pTarget)]);

  return true;
}

local bNoMenuUpdate;

private func OpenMenu(object pClonk, int iSelection)
{
  var iOwner = GetOwner(pClonk);
  
  //Auswahl updaten
  var iClass = 1;
  if(!iSelection && lastclass[iOwner])
    iClass = lastclass[iOwner];

  if(GetMenu(pClonk))
    iClass = CalculatePlayerSelection(iOwner, GetMenuSelection(pClonk));
  else
    if(iSelection)
      iClass = CalculatePlayerSelection(iOwner, iSelection);

  //Men¸ ˆffnen
  CloseMenu(pClonk);
  CreateMenu(GetID(), pClonk, this, 0, 0, 0, C4MN_Style_Dialog, true);

  //Icon
  AddMenuItem(" | ", 0, GetCData(iClass, CData_Icon), pClonk, 0, 0, " ", 2, GetCData(iClass, CData_Facet));

  //Name
  AddMenuItem(Format("<c ffff33>%s</c>|%s", GetCData(iClass, CData_Name), GetCData(iClass, CData_Desc)), 0, NONE, pClonk, 0, 0, " ");

  //Leerzeile
  AddMenuItem(" ", 0, NONE, pClonk, 0, 0, " ");

  //Clonktyp
  AddMenuItem(Format("{{%i}} %s", GetCData(iClass, CData_Clonk), GetName(0, GetCData(iClass, CData_Clonk)), 0, NONE, pClonk, 0, 0, " "));

  //Munition
  if (!FindObject(NOAM))
  {
    var szAmmo = "", aAmmo = GetCData(iClass, CData_Ammo);
    for (var aEntry in aAmmo)
    {
      if (GetType(aEntry) != C4V_Array || GetType(aEntry[0]) != C4V_C4ID || !aEntry[0]->~IsAmmo())
        continue;
      szAmmo = Format("%s%2dx {{%i}}", szAmmo, aEntry[1], aEntry[0]);
    }
    AddMenuItem(szAmmo, 0, NONE, pClonk, 0, 0, " ");
  }

  //Gegenst‰nde
  var szItems = "", aItems = GetCData(iClass, CData_Items), nextline = false, first = true;
  for (var aEntry in aItems)
  {
    if (GetType(aEntry) != C4V_Array || GetType(aEntry[0]) != C4V_C4ID)
      continue;
    szItems = Format("%s%2dx {{%i}}     ", szItems, aEntry[1], aEntry[0]);
    //Nach jedem zweiten Item umbrechen, auﬂer beim letzten
    if (!first && (nextline = !nextline) && GetIndexOf(aEntry, aItems) < GetLength(aItems) - 1)
      szItems = Format("%s|", szItems);
    first = false;
  }
  AddMenuItem(szItems, 0, NONE, pClonk, 0, 0, " ");

  //Granaten
  var szGrenades = "", aGrenades = GetCData(iClass, CData_Grenades);
  for (var aEntry in aGrenades)
  {
    if (GetType(aEntry) != C4V_Array || GetType(aEntry[0]) != C4V_C4ID || !aEntry[0]->~IsGrenade())
      continue;
    szGrenades = Format("%s%2dx {{%i}}     ", szGrenades, aEntry[1], aEntry[0]);
  }
  AddMenuItem(szGrenades, 0, NONE, pClonk, 0, 0, " ");

  //Ausr¸stung
  var szGear = "", aGear = GetCData(iClass, CData_Gear);
  var aAdditionalGear = GameCall("SpecificEquipment");

  if(GetType(aAdditionalGear) == C4V_Array)
    AddArray4K(aAdditionalGear, aGear);

  if(GetDarkness() >= 3)
    aGear[GetLength(aGear)] = [FLSH, 1];

  if(FindObject(FDMG))
    aGear[GetLength(aGear)] = [PPAR, 1];

  var aGearTypes = [];
  
  for(var aEntry in aGear)
  {
    if(GetType(aEntry) != C4V_Array || GetType(aEntry[0]) != C4V_C4ID || !aEntry[0]->~IsHazardGear())
      continue;

    if(GetIndexOf(aEntry[0]->~GetGearType(), aGearTypes) > -1)
      continue;

    aGearTypes[GetLength(aGearTypes)] = aEntry[0]->~GetGearType();
    szGear = Format("%s%2dx {{%i}}     ", szGear, aEntry[1], aEntry[0]);
  }
  AddMenuItem(szGear, 0, NONE, pClonk, 0, 0, " ");

  //Leerzeile
  AddMenuItem(" ", 0, NONE, pClonk, 0, 0, " ");

  //Die Klassen
  var i = 0;
  var displaying = 0;
  while(GetCData(++i, CData_Name))
  {
    if(!GetCData(i, CData_DisplayCondition, iOwner))
      continue;
    var szName = GetCData(i, CData_Name);
    if(!GetCData(i, CData_Condition, iOwner))
      szName = Format("<c 777777>%s</c>", szName);
    else
      szName = Format("<c ffff33>%s</c>", szName);

    AddMenuItem(szName, Format("SetupClass(%d, %d)", i, iOwner), GetCData(i, CData_Icon), pClonk, 0, 0, 0, 2, GetCData(i, CData_Facet));
    displaying++;
    if(i == iClass) iSelection = InfoMenuItems() + displaying;
  }

  if(!bNoMenuUpdate && iSelection >= 0)
  {
    bNoMenuUpdate = true;
    SelectMenuItem(iSelection, pClonk);
  }

  return true;
}

private func CalculatePlayerSelection(int iOwner, int iSelection)
{
  var iClass = iSelection - InfoMenuItems();
  for(var i = 1; i <= iClass && i <= GetClassAmount(); i++)
  {
    if(!GetCData(i, CData_DisplayCondition, iOwner))
    {
      iClass++;
    }
  }
  return iClass;
}

public func MenuQueryCancel()	{return 1;}

protected func OnMenuSelection(int iIndex, object pClonk)
{
  selection[GetOwner(pClonk)] = iIndex;
  if(bNoMenuUpdate)
    bNoMenuUpdate = false;
  else
    OpenMenu(pClonk,iIndex);
}

/* Klassen */

static const CData_Name			= 1;
static const CData_Desc			= 2;
static const CData_Clonk		= 3;
static const CData_Ammo			= 4;
static const CData_Items		= 5;
static const CData_Grenades		= 6;
static const CData_Gear			= 7;
static const CData_Icon			= 8;
static const CData_Condition		= 9;
static const CData_DisplayCondition	= 10;
static const CData_Facet		= 11;

public func GetCData(int iClass, int iData, int iPlr)
{
  return PrivateCall(this, Format("Class%dInfo", iClass), iData, iPlr);
}

public func SetupClass(int iClass, int iPlr)
{
  //Geht nicht
  if(!GetCData(iClass, CData_Condition, iPlr))
    return;

  var oldCrew = crew[iPlr];

  //Neuer Clonk
  var pCrew = crew[iPlr] = CreateObject(GetCData(iClass, CData_Clonk, iPlr), 0, 0, iPlr);

  if(Contained(oldCrew))
  {
    var tmp = Contained(oldCrew);
    SetGraphics(0, tmp, GetID(pCrew), 1, GFXOV_MODE_Object, 0, 1, pCrew);
    Enter(tmp, pCrew);
  }

  //Infosektion holen
  if(GetID(oldCrew) == GetID(pCrew))
    GrabObjectInfo(oldCrew, pCrew);
  else
    MakeCrewMember(pCrew, iPlr);
  SilentKill4K(oldCrew);

  //Ausw‰hlen
  SelectCrew(iPlr, pCrew, 1);
  SetPlrView(iPlr, pCrew);
  SetCursor(iPlr, pCrew, true, false); 

  //Ausr¸sten...
  //Munition
  if (!FindObject(NOAM))
  {
    var aAmmo = GetCData(iClass, CData_Ammo);
    for (var aEntry in aAmmo)
      if (GetType(aEntry) == C4V_Array && GetType(aEntry[0]) == C4V_C4ID && aEntry[0]->~IsAmmo())
        DoAmmo(aEntry[0], aEntry[1], pCrew);
  }

  //Gegenst‰nde
  var aItems = GetCData(iClass, CData_Items);
  for (var aEntry in aItems)
    if (GetType(aEntry) == C4V_Array && GetType(aEntry[0]) == C4V_C4ID)
      CreateContents(aEntry[0], pCrew, aEntry[1]);

  //Granaten
  var aGrenades = GetCData(iClass, CData_Grenades);
  for (var aEntry in aGrenades)
    if (GetType(aEntry) == C4V_Array && GetType(aEntry[0]) == C4V_C4ID && aEntry[0]->~IsGrenade())
      while (aEntry[1]--)
        CreateObject(aEntry[0], 0, 0, iPlr)->~Activate(pCrew);

  //Ausr¸stung
  var aGear = GetCData(iClass, CData_Gear);
  var aAdditionalGear = GameCall("SpecificEquipment");

  if(GetType(aAdditionalGear) == C4V_Array)
    AddArray4K(aAdditionalGear, aGear);

  if(GetDarkness() >= 3)
    aGear[GetLength(aGear)] = [FLSH, 1];

  //Bei Fallschaden Fallschirme als Zusatzausr¸stung
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

  //Nachricht
  var szAction = Format("%d", GetCData(iClass, CData_Facet));
  if (!GetActMapVal("Name", szAction, GetCData(iClass, CData_Icon)))
    szAction = 0;
  for(var i = 0; i < GetPlayerCount(); i++)
    if(GetPlayerTeam(i) == GetPlayerTeam(iPlr))
      EventInfo4K(i+1, Format("$PlayerChoosedClass$", GetTaggedPlayerName(iPlr), GetCData(iClass, CData_Name)), GetCData(iClass, CData_Icon), RGB(220, 220, 220), 0, 0, 0, szAction);

  //Speichern
  lastclass[iPlr] = iClass;

  Finish(pCrew, iClass);

  return true;
}

private func Default(int iData)
{
  if(iData == CData_Name)		return "<Classname>";
  if(iData == CData_Desc)		return "<Description>";
  if(iData == CData_Clonk)		return CIVC;
  if(iData == CData_Ammo)		return [[STAM, 30]];
  if(iData == CData_Items)		return [[PSTL, 1]];
  if(iData == CData_Grenades)		return [[FGRN, 1]];
  if(iData == CData_Gear)		return [[]];
  if(iData == CData_Icon)		return GetID();
  if(iData == CData_Condition)		return true;
  if(iData == CData_DisplayCondition)	return true;
  if(iData == CData_Facet)		return;
  return true;
}

public func GetClassAmount()
{
  var i = 1;
  while(GetCData(i)) i++;
  return i;
}