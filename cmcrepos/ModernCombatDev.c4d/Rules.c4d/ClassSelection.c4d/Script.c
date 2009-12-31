/*-- Klassenwahl --*/

#strict 2

local spawntimer;
local spawnclonk;
local crew;

/* Allgemeines */

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

protected func Initialize()
{
  SetPosition(10,10);
  crew = CreateArray(0);
  ScheduleCall(0,"Initialized",1);
}

private func Initialized()
{
  if(FindObject(WPCH) || FindObject(IGIB))
    RemoveObject();
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return 1; }


/* (Re-)Spawn */

func InitializePlayer(int iPlayer)
{
  var pClonk = GetCursor(iPlayer);
  if(!pClonk)
  {
    ScheduleCall(0,"InitializePlayer",1,0,iPlayer);
    return ;
  }
  
  if(FindObject(CHOS))
    return ;

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

func RelaunchPlayer(int iPlr, object pClonk)
{
  if(!pClonk)
    if(!(pClonk = GetCursor(iPlr)))
      return ScheduleCall(this,"RelaunchPlayer",1,0,iPlr,pClonk);
  if(!GetAlive(pClonk))
    return ScheduleCall(this,"RelaunchPlayer",1,0,iPlr);

  if(FindObject(GLMS))
  {
  	spawntimer = 10;
  	spawnclonk = pClonk;
  }
	
  InitClassMenu(pClonk);
  return ;
}

public func Spawntimer()
{
  if(spawntimer)
  {
  	spawntimer--;
  	PlayerMessage(GetOwner(spawnclonk),"@$TimeTillRespawn$",0,spawntimer);
  	if(!spawntimer)
    {
  		Finish(0,spawnclonk);
  	}
  }
}

func InitClassMenu(object pClonk)
{
  //Parameter vergessen?
  if(!pClonk)
    return 0;

  var iPlayer = GetOwner(pClonk);
    
  //Stuff damit man nicht Angreifbar ist in der Zwischenzeit
  if(GetID(Contained(pClonk)) != TIM1)//Vielleicht einfach nur auf Contained() prüfen?
  {
    var tmp = CreateObject(TIM1,AbsX(GetX(pClonk)),AbsY(GetY(pClonk)),iPlayer);
    SetCategory(GetCategory(tmp) | C4D_Foreground,tmp);
    SetGraphics(0,tmp,GetID(pClonk),1,5,0,1,pClonk);
    SetVisibility(VIS_Owner,tmp);
    Enter(tmp,pClonk);
  }
  
  crew[iPlayer] = pClonk;

  //Schon ein Menü offen?
  if(GetMenu(pClonk))
  {
    CloseMenu(pClonk); //zumachen.
  }
  
  if(IsAIControlled(pClonk))
  {
    return AIThink(iPlayer,pClonk);
  }
 
  OpenMenu(pClonk);
}

func Finish(object pClonk)
{
  PlayerMessage(GetOwner(spawnclonk),"");
  spawnclonk = 0;
  spawntimer = 0;
  
  var iPlayer = GetOwner(pClonk);
  
  //Menü schließen
  CloseMenu(GetCursor(iPlayer));
  
  //Alle Waffen auffüllen
  for(var wpn in FindObjects(Find_Container(pClonk), Find_Func("IsWeapon")))
  {
    //:O Feuermodi berücksichtigen bei WPN2!1
  
    var ammo = wpn->GetFMData(FM_AmmoID);
    var load = wpn->GetFMData(FM_AmmoLoad);
    //erst entladen
    DoAmmo(ammo,-load, wpn);
    //dann neu reinladen
    DoAmmo(ammo, load, wpn);
  }
  
  pClonk->~UpdateCharge();
  
  //Wieder angreifbar machen
  RemoveObject(Contained(pClonk),1);
  
  //Broadcasten
  GameCallEx("OnClassSelection",crew[iPlayer]);
}

/* Menüzeug */

private func InfoMenuItems(){return 3;}

local bNoMenuUpdate;

private func OpenMenu(object pClonk, int iSelection)
{
  if(!iSelection)
    iSelection = InfoMenuItems()+1;
  
  if(GetMenu(pClonk))
    iSelection = GetMenuSelection(pClonk);



  CloseMenu(pClonk);
  if(!CreateMenu(GetID(),pClonk,this,0,0,0,C4MN_Style_Dialog,1)) return false;
  
  var class = iSelection-InfoMenuItems();
  
  AddMenuItem(" | ", "", GetCData(class,CData_Icon), pClonk, 0, 0, "", 514, GetCData(class,CData_Facet), 0);//*hack*
  
  AddMenuItem(Format("%s|%s", GetCData(class,CData_Name), GetCData(class,CData_Desc)),
              "", NONE, pClonk, 0, 0, "", 512, 0, 0);
  
  AddMenuItem(Format("{{%i}}%s|%s",GetCData(class,CData_Clonk),GetName(0,GetCData(class,CData_Clonk)), GetCData(class,CData_Items)),
              "", NONE, pClonk, 0, 0, "", 512, 0, 0);
       
  AddMenuItem(" ","", NONE,pClonk, 0, 0, "", 512, 0, 0);//leer
  
  //34 Zeichen pro Zeile! (Ohne Icon!)
  
  var i = 1;
  while(GetCData(i))
  {
    AddMenuItem (GetCData(i,CData_Name),Format("SetupClass(%d,%d)",i,GetOwner(pClonk)),GetCData(i,CData_Icon),pClonk,0,pClonk,0,2,GetCData(i,CData_Facet));
    i++;
  }
  
  if(!bNoMenuUpdate && iSelection >= 0)
  {
    bNoMenuUpdate = true;
    SelectMenuItem(iSelection,pClonk);
  }
  
  return true;
} 

public func MenuQueryCancel() { return 1; }

protected func OnMenuSelection(int iIndex, object pClonk)
{
  if(bNoMenuUpdate)
    bNoMenuUpdate = false;
  else
    OpenMenu(pClonk,iIndex);
}

/* Klassen */

static const CData_Name  = 1;
static const CData_Desc  = 2;
static const CData_Clonk = 3;
static const CData_Items = 4;
static const CData_Icon  = 5;
static const CData_Facet = 6;

public func GetCData(int iClass,int iData)
{
  return PrivateCall(this,Format("Class%dInfo",iClass),iData);
}

public func SetupClass(int iClass, int iPlayer)
{
  var oldCrew = crew[iPlayer];
  crew[iPlayer] = PrivateCall(this,Format("Class%dSetup",iClass),iPlayer);
  
  if(Contained(oldCrew))
  {
    var tmp = Contained(oldCrew);
    SetGraphics(0,tmp,GetID(crew[iPlayer]),1,5,0,1,crew[iPlayer]);
    Enter(tmp,crew[iPlayer]);
  }
  
  MakeCrewMember(crew[iPlayer],iPlayer);
  var ai = IsAIControlled(oldCrew);
  if(ai) ai->~SetClonk(crew[iPlayer]);
  SilentKill4K(oldCrew);
  SelectCrew(iPlayer,crew[iPlayer],1);
  SetPlrView(iPlayer,crew[iPlayer]);
  SetCursor(iPlayer,crew[iPlayer],true,false); 

  Finish(crew[iPlayer]);
}

private func Default(int iData)
{
  if(iData == CData_Name)  return "<Classname>";
  if(iData == CData_Desc)  return "<Description>";
  if(iData == CData_Clonk) return CLNK;
  if(iData == CData_Items) return "<Items>";
  if(iData == CData_Icon)  return GetID();
  if(iData == CData_Facet) return 0;
  return true;
}


/* KI-Support */

public func AIThink(int iPlr, object pClonk)
{
  var i = 1;
  while(GetCData(i))
    i++;
    
  i = Random(i-1)+1;
  SetupClass(i,iPlr);
}
