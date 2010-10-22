/*-- Klassenwahl --*/

#strict 2

local crew;
local lastclass;
local selection;


/* Initalisierung */

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

  //Men� zeitverz�gert erstellen
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

  //Clonk/Beh�lter weg oder Clonk nicht im Beh�lter? Weg mit dem Effekt
  if (!EffectVar(2, pTarget, iNo) || !EffectVar(3, pTarget, iNo) || Contained(EffectVar(2, pTarget, iNo)) != EffectVar(3, pTarget, iNo)) {
    //Beh�lter noch da und TIM1? Weg damit.
    if (GetID(EffectVar(3, pTarget, iNo)) == TIM1)
      RemoveObject(EffectVar(3, pTarget, iNo));
    return -1;
  }

  if(EffectVar(1, pTarget, iNo) <= 0)
  {
    var iPlr = EffectVar(0, pTarget, iNo),
    class = selection[iPlr]-InfoMenuItems();
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

  //Bereits ein Men� offen?
  if(GetMenu(pClonk))
    CloseMenu(pClonk); //Men� schlie�en

  OpenMenu(pClonk);
}

func Finish(object pClonk)
{
  if(!pClonk || !Contained(pClonk)) return;
  var iPlayer = GetOwner(pClonk);

  //Men� schlie�en
  CloseMenu(GetCursor(iPlayer));

  //Alle Waffen auff�llen
  for(var wpn in FindObjects(Find_Container(pClonk), Find_Func("IsWeapon")))
  {
    while(wpn->CycleFM(+1))
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
    wpn->CycleFM(+1);
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
  GameCallEx("OnClassSelection",crew[iPlayer]);
}

/* Men� */

private func InfoMenuItems()
{
  if(!FindObject(NOAM))
  {return 6;}
  else
  {return 5;}
}

local bNoMenuUpdate;

private func OpenMenu(object pClonk, int iSelection)
{
  if(!iSelection)
    if(lastclass[GetOwner(pClonk)] > 0)
      iSelection = lastclass[GetOwner(pClonk)]+InfoMenuItems();
  else
    iSelection = InfoMenuItems()+1;

  if(GetMenu(pClonk))
    iSelection = GetMenuSelection(pClonk);

  CloseMenu(pClonk);
  if(!CreateMenu(GetID(),pClonk,this,0,0,0,C4MN_Style_Dialog,1)) return false;

  var class = iSelection-InfoMenuItems();

  //Klassenicon
  AddMenuItem(" | ", "", GetCData(class,CData_Icon), pClonk, 0, 0, "", 514, GetCData(class,CData_Facet), 0);

  //Klassenname und -beschreibung
  //34 Zeichen pro Zeile! (Ohne Icon!)
  AddMenuItem(Format("<c ffff33>%s</c>|%s", GetCData(class,CData_Name), GetCData(class,CData_Desc)),
              "", NONE, pClonk, 0, 0, "", 512, 0, 0);

  //Leerzeile
  AddMenuItem(" ","", NONE,pClonk, 0, 0, "", 512, 0, 0);

  //Clonkicon und -name
  AddMenuItem(Format("{{%i}}%s",GetCData(class,CData_Clonk),GetName(0,GetCData(class,CData_Clonk))),
              "", NONE, pClonk, 0, 0, "", 512, 0, 0);

  //Klassenmunition
  if(!FindObject(NOAM))
  {
    AddMenuItem(Format("%s",GetCData(class,CData_Ammo)),
               "", NONE, pClonk, 0, 0, "", 512, 0, 0);}

    //Klassenausr�stung
    AddMenuItem(Format("%s",GetCData(class,CData_Items)),
                "", NONE, pClonk, 0, 0, "", 512, 0, 0);

    //Leerzeile
    AddMenuItem(" ","", NONE,pClonk, 0, 0, "", 512, 0, 0);

    //Auswahlliste der Klassen
    var i = 1;
    while(GetCData(i))
    {
      var szName = GetCData(i,CData_Name);
      if(!GetCData(i, CData_Condition, GetOwner(pClonk)))
      {
        szName = Format("<c 777777>%s</c>", szName);
      }
      else
      {
        szName = Format("<c ffff33>%s</c>", szName);
      }
      AddMenuItem (szName,Format("SetupClass(%d,%d)",i,GetOwner(pClonk)),GetCData(i,CData_Icon),pClonk,0,pClonk,0,2,GetCData(i,CData_Facet));
      i++;
    }

  if(!bNoMenuUpdate && iSelection >= 0)
  {
    bNoMenuUpdate = true;
    SelectMenuItem(iSelection,pClonk);
  }

  return true;
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

static const CData_Name             = 1;
static const CData_Desc             = 2;
static const CData_Clonk            = 3;
static const CData_Ammo             = 4;
static const CData_Items            = 5;
static const CData_Icon             = 6;
static const CData_Condition        = 7;
static const CData_DisplayCondition = 8;
static const CData_Facet            = 9;

public func GetCData(int iClass,int iData,int iPlayer)
{
  return PrivateCall(this,Format("Class%dInfo",iClass),iData,iPlayer);
}

public func SetupClass(int iClass, int iPlayer)
{
  if(!GetCData(iClass, CData_Condition, iPlayer)) return;

  var oldCrew = crew[iPlayer];
  crew[iPlayer] = PrivateCall(this,Format("Class%dSetup",iClass),iPlayer);
  
  if(Contained(oldCrew))
  {
    var tmp = Contained(oldCrew);
    SetGraphics(0,tmp,GetID(crew[iPlayer]),1,5,0,1,crew[iPlayer]);
    Enter(tmp,crew[iPlayer]);
  }

  if(GetID(oldCrew) == GetID(crew[iPlayer]))
    GrabObjectInfo(oldCrew, crew[iPlayer]);
  else
    MakeCrewMember(crew[iPlayer], iPlayer);
  SilentKill4K(oldCrew);
  SelectCrew(iPlayer,crew[iPlayer],1);
  SetPlrView(iPlayer,crew[iPlayer]);
  SetCursor(iPlayer,crew[iPlayer],true,false); 

  //Nachricht
  for(var i = 0; i < GetPlayerCount(); i++)
    if(GetPlayerTeam(i) == GetPlayerTeam(iPlayer))
      EventInfo4K(i+1, 
        Format("$PlayerChoosedClass$", GetTaggedPlayerName(iPlayer), GetCData(iClass,CData_Name)),
        GetCData(iClass,CData_Icon),
        RGB(220,220,220));

  //Speichern
  lastclass[iPlayer] = iClass;

  Finish(crew[iPlayer]);

  return true;
}

private func Default(int iData)
{
  if(iData == CData_Name)  return "<Classname>";
  if(iData == CData_Desc)  return "<Description>";
  if(iData == CData_Clonk) return CLNK;
  if(iData == CData_Ammo)  return "<Ammo>";
  if(iData == CData_Items) return "<Items>";
  if(iData == CData_Icon)  return GetID();
  if(iData == CData_Condition)  return true;
  if(iData == CData_DisplayCondition)  return true;
  if(iData == CData_Facet) return;
  return true;
}

public func GetClassAmount()
{
  var i = 1;
  while(GetCData(i))  i++;
  return i;
}