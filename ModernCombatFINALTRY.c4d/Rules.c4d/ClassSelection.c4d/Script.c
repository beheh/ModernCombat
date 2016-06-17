/*-- Klassenwahl --*/

#strict 2

local crew;
local lastclass;
local selection;
local submenu;

public func AvoidDefaultSpawnObject() { return true; }


/* Initialisierung */

protected func Initialize()
{
  SetPosition(10,10);
  crew = [];
  lastclass = [];
  selection = [];
  submenu = [];

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

  if(!GameCall("RespawnDelayRejected") || GetEffect("WaitingObject", Contained(pClonk)))
    if(GameCall("GetPlayerRespawnTime", iPlr))
      return ScheduleCall(this,"RelaunchPlayer",3,0,iPlr,pClonk);

  //Menü zeitverzögert erstellen
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

  CustomMessage(Format("@$TimeTillRespawn$", EffectVar(1, pTarget, iNo)), FindObject2(Find_ID(1HUD), Find_Owner(iPlr)), iPlr, 0, 80, 0, 0, 0, 1);
}

public func FxSpawntimerTimer(pTarget, iNo, iTime)
{
  EffectVar(1, pTarget, iNo)--;
  var iPlr = EffectVar(0, pTarget, iNo);
  CustomMessage(Format("@$TimeTillRespawn$", EffectVar(1, pTarget, iNo)), FindObject2(Find_ID(1HUD), Find_Owner(iPlr)), iPlr, 0, 80, 0, 0, 0, 1);

  //Verschwinden wenn Clonk/Behälter weg oder Clonk nicht im Behälter
  if (!EffectVar(2, pTarget, iNo) || !EffectVar(3, pTarget, iNo) || Contained(EffectVar(2, pTarget, iNo)) != EffectVar(3, pTarget, iNo))
  {
    //Verschwinden wenn Behälter noch vorhanden und TIM1
    if(GetID(EffectVar(3, pTarget, iNo)) == TIM1)
      RemoveObject(EffectVar(3, pTarget, iNo));
    return -1;
  }

  if(EffectVar(1, pTarget, iNo) <= 0)
  {
    var class = CalculatePlayerSelection(iPlr, selection[iPlr]);

    CustomMessage("", FindObject2(Find_ID(1HUD), Find_Owner(iPlr)), iPlr);
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
  if(!fTemp)
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

  //Zeitbegrenzung bei AS, LMS und DM
  if(FindObject(GASS) || FindObject(GLMS) || FindObject(GTDM))
    AddEffect("Spawntimer", this, 100, 35, this, GetID(), iPlayer, pClonk, tmp);

  //Bereits ein Menü vorhanden: Schließen
  if(GetMenu(pClonk))
    CloseMenu(pClonk);

  AddEffect("ClassMenu", pClonk, 1, 10, this);
}

func Finish(object pClonk, int iClass)
{
  if(!pClonk || !Contained(pClonk)) return;
  var iPlayer = GetOwner(pClonk);

  //Menü schließen
  CloseMenu(GetCursor(iPlayer));

  //Zuschauerobjekte zurücksetzen
  if(FindObject2(Find_ID(SPEC), Find_Owner(GetOwner(pClonk))))
    FindObject2(Find_ID(SPEC), Find_Owner(GetOwner(pClonk)))->Set(pClonk);

  //Alle Waffen auffüllen
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
  Sound("WPN2_Modify*.ogg", 1, pClonk, 100, GetOwner(pClonk)+1);

  //Effekt entfernen
  for(var i = 0; i < GetEffectCount("Spawntimer", this); i++)
    if(EffectVar(0, this, GetEffect("Spawntimer", this, i)) == iPlayer)
      RemoveEffect("Spawntimer", this, i);
  PlayerMessage(iPlayer, "@");

  //Broadcasten
  GameCallEx("OnClassSelection", crew[iPlayer], iClass);
}

/* Menü */

private func InfoMenuItems()
{
  return 7;
}

public func FxClassMenuTimer(object pTarget, int nr)
{
  if(!GetMenu(pTarget) || (GetMenu(pTarget) != GetID() && GetMenu(pTarget) != SPEC && ++EffectVar(0, pTarget, nr) <= 1))
  {
    CloseMenu(pTarget);
    OpenMenu(pTarget, selection[GetOwner(pTarget)]);
  }

  return true;
}

local bNoMenuUpdate;

private func OpenMenu(object pClonk, int iSelection)
{
  var iOwner = GetOwner(pClonk);

  //Auswahl aktualisieren
  var iClass = 1;
  if(!iSelection && lastclass[iOwner])
    iClass = lastclass[iOwner];

  if(!submenu[GetOwner(pClonk)] && GetMenu(pClonk))
    iClass = CalculatePlayerSelection(iOwner, GetMenuSelection(pClonk));
  else
    if(iSelection)
      iClass = CalculatePlayerSelection(iOwner, iSelection);

  submenu[GetOwner(pClonk)] = 0;

  //Menü öffnen
  CloseMenu(pClonk);
  CreateMenu(GetID(), pClonk, this, 0, 0, 0, C4MN_Style_Dialog, true);

  var headitems = 0;
  if(GetCData(iClass, CData_Name))
  {
    //Icon
    AddMenuItem(" | ", 0, GetCData(iClass, CData_Icon), pClonk, 0, 0, " ", 2, GetCData(iClass, CData_Facet));

    //Name und Beschreibung
    AddMenuItem(Format("<c ffff33>%s</c>|%s", GetCData(iClass, CData_Name), GetCData(iClass, CData_Desc)), 0, NONE, pClonk, 0, 0, " ");

    //Leerzeile
    AddMenuItem(" ", 0, NONE, pClonk, 0, 0, " ");

    //Clonktyp
    AddMenuItem(Format("{{%i}} %s", GetCData(iClass, CData_Clonk), GetName(0, GetCData(iClass, CData_Clonk)), 0, NONE, pClonk, 0, 0, " "));

    //Munition
    if(!FindObject(NOAM))
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
    else
      AddMenuItem(" ", 0, NONE, pClonk, 0, 0, " ");

    //Gegenstände
    var szItems = "", aItems = GetCData(iClass, CData_Items), nextline = false, first = true;
    for (var aEntry in aItems)
    {
      if (GetType(aEntry) != C4V_Array || GetType(aEntry[0]) != C4V_C4ID)
        continue;
      szItems = Format("%s%2dx {{%i}}     ", szItems, aEntry[1], aEntry[0]);
      //Nach jedem zweiten Item umbrechen, außer beim letzten
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

    //Ausrüstung
    var szGear = "", aGear = GetCData(iClass, CData_Gear);
    var aAdditionalGear = GameCall("SpecificEquipment");

    if(GetType(aAdditionalGear) == C4V_Array)
      AddArray4K(aAdditionalGear, aGear);

    if(GetDarkness() >= 3 && NoAttachments())
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
    headitems = InfoMenuItems();
  }
  else
  {
    //Icon
    AddMenuItem(" | ", 0, SPEC, pClonk, 0, 0, " ", 2);

    //Name und Beschreibung
    AddMenuItem("<c ffff33>$Spectate$</c>|$SpectateDesc$", 0, NONE, pClonk, 0, 0, " ");

    //Leerzeilen
    AddMenuItem(" ", 0, NONE, pClonk, 0, 0, " ");
    AddMenuItem(" ", 0, NONE, pClonk, 0, 0, " ");
    AddMenuItem(" ", 0, NONE, pClonk, 0, 0, " ");
    AddMenuItem(" ", 0, NONE, pClonk, 0, 0, " ");
    AddMenuItem(" ", 0, NONE, pClonk, 0, 0, " ");
    AddMenuItem(" ", 0, NONE, pClonk, 0, 0, " ");

    headitems = 7;
    iSelection = -1;
  }

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

    AddMenuItem(szName, Format("OpenMenuAttachment(%d, %d, Object(%d))", 0, i, ObjectNumber(pClonk)), GetCData(i, CData_Icon), pClonk, 0, 0, 0, 2, GetCData(i, CData_Facet));
    displaying++;
    if(i == iClass) iSelection = headitems + displaying;
  }
  var spectate = (GetTeamPlayerCount(GetPlayerTeam(GetOwner(pClonk))) > 1);
  if(spectate)
    AddMenuItem("$Spectate$", "OpenSpectateMenu", SPEC, pClonk, 0, pClonk);

  if(!bNoMenuUpdate)
  {
    bNoMenuUpdate = true;
    if(iSelection >= 0)
      SelectMenuItem(Min(iSelection, headitems+displaying+spectate), pClonk);
    else
      SelectMenuItem(headitems+displaying+spectate+1+iSelection, pClonk);
  }

  return true;
}

protected func OpenSpectateMenu(dummy, object pTarget)
{
  SPEC->OpenSpectateMenu(pTarget, this);
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

public func MenuQueryCancel(int iSelection, object pMenuObject)
{
  if(GetMenu(pMenuObject) == SPEC)
  {
    if(FindObject2(Find_ID(SPEC), Find_Owner(GetOwner(pMenuObject))))
      FindObject2(Find_ID(SPEC), Find_Owner(GetOwner(pMenuObject)))->Set(pMenuObject);
    ScheduleCall(this, "OpenMenu", 1, 0, pMenuObject, -1);
    return false;
  }

  return true;
}

protected func OnMenuSelection(int iIndex, object pClonk)
{
  if(GetMenu(pClonk) == SPEC)
    return SPEC->SpectateMenuSelection(iIndex, pClonk);

  if(submenu[GetOwner(pClonk)]) return;
  selection[GetOwner(pClonk)] = iIndex;
  if(bNoMenuUpdate)
    bNoMenuUpdate = false;
  else
    OpenMenu(pClonk,iIndex);
}

public func GetLastClass(int iPlr)
{
  return lastclass[iPlr];
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
static const CData_Skins		= 12;

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

  //Auswählen
  SelectCrew(iPlr, pCrew, 1);
  SetPlrView(iPlr, pCrew);
  SetCursor(iPlr, pCrew, true, false); 

  //Ausrüsten...
  //Munition
  if (!FindObject(NOAM))
  {
    var aAmmo = GetCData(iClass, CData_Ammo);
    for (var aEntry in aAmmo)
      if (GetType(aEntry) == C4V_Array && GetType(aEntry[0]) == C4V_C4ID && aEntry[0]->~IsAmmo())
        DoAmmo(aEntry[0], aEntry[1], pCrew);
  }

  //Gegenstände
  var aItems = GetCData(iClass, CData_Items);
  for (var aEntry in aItems)
    if (GetType(aEntry) == C4V_Array && GetType(aEntry[0]) == C4V_C4ID)
    {
      var tempItem = CreateContents(aEntry[0], pCrew, aEntry[1]);
      var idWeap, iAtt;
      idWeap = GetPlrExtraData(iPlr, Format("CMC_Weap%d", iClass));
      iAtt = GetPlrExtraData(iPlr, Format("CMC_Att%d", iClass));
      if(idWeap && iAtt && idWeap == aEntry[0] && !NoAttachments())
      { 
        tempItem->SetAttachment(iAtt);
      }
    }

  //Granaten
  var aGrenades = GetCData(iClass, CData_Grenades);
  for (var aEntry in aGrenades)
    if (GetType(aEntry) == C4V_Array && GetType(aEntry[0]) == C4V_C4ID && aEntry[0]->~IsGrenade())
      while (aEntry[1]--)
        CreateObject(aEntry[0], 0, 0, iPlr)->~Activate(pCrew);

  //Ausrüstung
  var aGear = GetCData(iClass, CData_Gear);
  var aAdditionalGear = GameCall("SpecificEquipment");

  if(GetType(aAdditionalGear) == C4V_Array)
    AddArray4K(aAdditionalGear, aGear);

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
  var aSkins = GetCData(iClass, CData_Skins);
  if(GetLength(aSkins))
  {
    var skin = aSkins[Random(GetLength(aSkins))];
    SetGraphics(skin[1], pCrew, skin[0]);
    if(skin[2])
      SetPortrait(Format("Ptr%s%d", skin[1], Random(skin[2])+1), pCrew, skin[0]);
  }

  var szAction = Format("%d", GetCData(iClass, CData_Facet));
  if (!GetActMapVal("Name", szAction, GetCData(iClass, CData_Icon)))
    szAction = 0;
  if(!FindObject2(Find_Func("RejectChoosedClassInfo", iPlr, iClass)))
    for(var i = 0; i < GetPlayerCount(); i++)
      if(GetPlayerTeam(GetPlayerByIndex(i)) == GetPlayerTeam(iPlr))
        //Eventnachricht: Spieler steigt ein
        EventInfo4K(GetPlayerByIndex(i)+1, Format("$PlayerChoosedClass$", GetTaggedPlayerName(iPlr), GetCData(iClass, CData_Name)), GetCData(iClass, CData_Icon), RGB(220, 220, 220), 0, 0, 0, szAction);

  //Speichern
  lastclass[iPlr] = iClass;

  Finish(pCrew, iClass);

  return true;
}

public func SpawnEventInfo(string szAdditional, int iPlr, int iClass, object pByObj, string szInfo)
{
  //Falls mehrere Spielziele existieren sollten, brauchen wir keine mehrfachen Nachrichten
  if(!pByObj || pByObj != FindObject2(Find_Func("RejectChoosedClassInfo", iPlr, iClass)))
    return;

  var str = Format("$PlayerChoosedClass2$", GetTaggedPlayerName(iPlr), GetCData(iClass, CData_Name), szAdditional);
  if(szInfo)
    str = szInfo;

  var szAction = Format("%d", GetCData(iClass, CData_Facet));
  if (!GetActMapVal("Name", szAction, GetCData(iClass, CData_Icon)))
    szAction = 0;
  for(var i = 0; i < GetPlayerCount(); i++)
    if(GetPlayerTeam(i) == GetPlayerTeam(iPlr))
      //Eventnachricht: Spieler steigt ein
      EventInfo4K(i+1, str, GetCData(iClass, CData_Icon), RGB(220, 220, 220), 0, 0, 0, szAction);

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
  if(iData == CData_Skins)		return [];
  return true;
}

public func GetClassAmount()
{
  var i = 1;
  while(GetCData(i)) i++;
  return i;
}

public func OpenMenuAttachment(id idParamWeapon, int iClass, object pClonk, int iSelection)
{
  var iOwner = GetOwner(pClonk);

  //Menü öffnen
  CloseMenu(pClonk);
  submenu[iOwner] = 2;
  var i = 0;
  var j = 0;
  var count = 5;
  var fOne = true;

  //Gegenstände
  var aItems = GetCData(iClass, CData_Items);
  var idWeap, idFirstWeap, iAtt, idActualWeap, fNextWeap;
  idWeap = GetPlrExtraData(iOwner, Format("CMC_Weap%d", iClass));
  iAtt = GetPlrExtraData(iOwner, Format("CMC_Att%d", iClass));

  for(var aEntry in aItems)
    if(GetType(aEntry) == C4V_Array && GetType(aEntry[0]) == C4V_C4ID && aEntry[0]->~PermittedAtts())
    {
      if(fNextWeap)
      {
        idActualWeap = aEntry[0];
        fNextWeap = false;
        break;
      }
      if(idParamWeapon == aEntry[0])
        fNextWeap = true;
      if(!idFirstWeap)
        idFirstWeap = aEntry[0];

      if(!idParamWeapon && !idWeap)
      {
        idActualWeap = aEntry[0];
        break;
      }
      if(!idParamWeapon && idWeap == aEntry[0])
      {
        idActualWeap = aEntry[0];
        break;
      }
    }

  //Zweiter Fall kommt vor, wenn die gespeicherte Waffe/das Attachment für die Klasse nicht existieren, z.B. weil die Klasse verändert wurde
  if(fNextWeap || idActualWeap == 0)
    idActualWeap = idFirstWeap;

  if(idActualWeap != idWeap || !(idWeap->~PermittedAtts() & iAtt))
    iAtt = 0;

  CreateMenu(GetID(), pClonk, this, 0, 0, 0, C4MN_Style_Dialog, true);

  //Icon
  var facet = 11;
  if(NoAttachments())
    facet = GetCData(iClass, CData_Facet);
  AddMenuItem(" | ", 0, GetCData(iClass, CData_Icon), pClonk, 0, 0, " ", 2, facet);

  //Name und Beschreibung
  var iterations = 0;
  var iTempAtt = iAtt;
  var DescAtts = $DescAttachments$;

  while(iTempAtt > 0)
  {
    iterations++;
    iTempAtt/=2;
  }

  var name = GetName(0, AttachmentIcon(iAtt));
  if(!iAtt)
    name = "$NoAttachment$";

  if(!NoAttachments() && idActualWeap)
  {
    AddMenuItem(Format("<c ffff33>%s</c>|%s", name, DescAtts[iterations]), 0, NONE, pClonk, 0, 0, " ");

    //Leerzeilen
    AddMenuItem(" |||| ", 0, NONE, pClonk, 0, 0, " ");
  }
  else
  {
    //Bei aktivierter Keine Waffenaufsätze-Regel Klasseninformationen anzeigen

    //Name und Beschreibung
    AddMenuItem(Format("<c ffff33>%s</c>|%s", GetCData(iClass, CData_Name), GetCData(iClass, CData_Desc)), 0, NONE, pClonk, 0, 0, " ");

    //Leerzeile
    AddMenuItem(" ", 0, NONE, pClonk, 0, 0, " ");

    //Clonktyp
    AddMenuItem(Format("{{%i}} %s", GetCData(iClass, CData_Clonk), GetName(0, GetCData(iClass, CData_Clonk)), 0, NONE, pClonk, 0, 0, " "));

    //Munition
    if(!FindObject(NOAM))
    {
      var szAmmo = "", aAmmo = GetCData(iClass, CData_Ammo);
      for (var aEntry in aAmmo)
      {
        if(GetType(aEntry) != C4V_Array || GetType(aEntry[0]) != C4V_C4ID || !aEntry[0]->~IsAmmo())
          continue;
        szAmmo = Format("%s%2dx {{%i}}", szAmmo, aEntry[1], aEntry[0]);
      }
      AddMenuItem(szAmmo, 0, NONE, pClonk, 0, 0, " ");
    }
    else
      AddMenuItem(" ", 0, NONE, pClonk, 0, 0, " ");

    //Gegenstände
    var szItems = "", aItems = GetCData(iClass, CData_Items), nextline = false, first = true;
    for (var aEntry in aItems)
    {
      if(GetType(aEntry) != C4V_Array || GetType(aEntry[0]) != C4V_C4ID)
        continue;
      szItems = Format("%s%2dx {{%i}}     ", szItems, aEntry[1], aEntry[0]);
      //Nach jedem zweiten Item umbrechen, außer beim letzten
      if(!first && (nextline = !nextline) && GetIndexOf(aEntry, aItems) < GetLength(aItems) - 1)
        szItems = Format("%s|", szItems);
      first = false;
    }
    AddMenuItem(szItems, 0, NONE, pClonk, 0, 0, " ");

    //Granaten
    var szGrenades = "", aGrenades = GetCData(iClass, CData_Grenades);
    for (var aEntry in aGrenades)
    {
      if(GetType(aEntry) != C4V_Array || GetType(aEntry[0]) != C4V_C4ID || !aEntry[0]->~IsGrenade())
        continue;
      szGrenades = Format("%s%2dx {{%i}}     ", szGrenades, aEntry[1], aEntry[0]);
    }
    AddMenuItem(szGrenades, 0, NONE, pClonk, 0, 0, " ");

    //Ausrüstung
    var szGear = "", aGear = GetCData(iClass, CData_Gear);
    var aAdditionalGear = GameCall("SpecificEquipment");

    if(GetType(aAdditionalGear) == C4V_Array)
      AddArray4K(aAdditionalGear, aGear);

    if(GetDarkness() >= 3 && NoAttachments())
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
  }

  //Spawnen
  AddMenuItem("$Spawn$", Format("SetupClass(%d, %d)", iClass, iOwner), CHOS, pClonk, 0, pClonk, 0, 2, 3);

  //Zurück
  AddMenuItem("$Back$", Format("OpenMenu(Object(%d), %d)", ObjectNumber(pClonk), InfoMenuItems()+iClass), 0, pClonk, 0, 0, "$Back$");

  if(!NoAttachments() && idActualWeap)
  {
    //Waffen-Wechsler
    AddMenuItem(Format("<c ff3333>%s</c>", GetName(0, idActualWeap)), Format("ChangeWeapon(%d, %i, Object(%d), %d)", iClass, idActualWeap, ObjectNumber(pClonk), count), idActualWeap, pClonk, 0, pClonk, 0, 2, GetCData(i, CData_Facet));

    //Waffenaufsätze
    for(j = 0; j < 1000000000; j*=2)
    {
      if(fOne && j == 2) {j = 1; fOne = false;}

      var select = false;
      var szName = Format("%s",GetName(0, AttachmentIcon(j)));
      if(idWeap && iAtt && iAtt == j && idWeap == idActualWeap) 
      {
        select = true;
        szName = Format("<c ffff33>%s</c>", szName);
      }
      else
        szName = Format("<c 777777>%s</c>", szName);

      if(idActualWeap->~PermittedAtts() & j)
      {
        count++;
        AddMenuItem(szName, Format("ChooseAttachment(%d, %i, %d, Object(%d), %d, %d)", iClass, idActualWeap, j, ObjectNumber(pClonk), count, select), AttachmentIcon(j), pClonk, 0, pClonk, 0, 2, GetCData(i, CData_Facet));
        //if(!idParamWeapon && select) SelectMenuItem(count + 1, pClonk);
      }

      //j soll bei 0 anfangen, sich ab 1 dann aber verdoppeln.
      if(!j) j++;
    }

    var clr = 0x777777, select = false;
    if(idWeap == idActualWeap && !iAtt)
    {
      select = true;
      clr = 0xFFFF33;
    }

    if(idActualWeap)
      AddMenuItem(Format("<c %x>$NoAttachment$</c>", clr), Format("ChooseAttachment(%d, %i, 0, Object(%d), %d, %d)", iClass, idActualWeap, ObjectNumber(pClonk), count+1, select), SM06, pClonk, 0, pClonk, 0, 2, GetCData(i, CData_Facet));
  }

  if(iSelection) SelectMenuItem(iSelection, pClonk);
}

public func ChooseAttachment(int iClass, id idWeapon, int iAttachment, object pClonk, int iSelection, bool fSelected)
{
  if(fSelected) return SetupClass(iClass, GetOwner(pClonk));

  //Wahl speichern
  SetPlrExtraData(GetOwner(pClonk), Format("CMC_Weap%d", iClass), idWeapon);
  SetPlrExtraData(GetOwner(pClonk), Format("CMC_Att%d", iClass), iAttachment);

  //Menü aktualisieren
  OpenMenuAttachment(0, iClass, pClonk, iSelection);

  //Sound
  Sound("WPN2_Modify*.ogg", 1, pClonk, 100, GetOwner(pClonk)+1);
}

public func ChangeWeapon(int iClass, id idWeapon, object pClonk, int iSelection)
{
  OpenMenuAttachment(idWeapon, iClass, pClonk, iSelection);

  //Sound
  Sound("WPN2_Handle*.ogg", 1, pClonk, 100, GetOwner(pClonk)+1);
}
