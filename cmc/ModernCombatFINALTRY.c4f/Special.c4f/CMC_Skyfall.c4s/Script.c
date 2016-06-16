/*-- Skyfall --*/

#strict 2
#include CSTD

static aTowerInterior,aDoorWay;

func RecommendedGoals()	{return [GLMS];}	//Spielzielempfehlung

static const FKDT_SuicideTime = 5;		//Verkürzte Wartezeit


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Bildschirmfärbung
  SetGamma(RGB(15,15,15),RGB(118,118,118),RGB(215,215,215));
  //Materialregen starten
  LaunchRain(0, Material("Rain"), LandscapeWidth(), 100)->SetCategory(1);
  //Globaler Regensound
  Sound("Rain.ogg",true,0,50,0,+1);
  //Turmobjekte
  aTowerInterior = [];
  //Türverbindungen
  aDoorWay = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  return 1; 
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Sonne umplazieren
  FindObject(LENS)->SetPosition(1300,530);

  //Grenze setzen
  CreateObject(BRDR, 0, 1380, -1)->Set(3,0,1);

  //Sendemast
  var tower = CreateObject(AATR, 1300, 1300, -1);
  tower->AddNode(815, 590, 0, CreateObject(REHR, 815, 600, -1), 0, 1);
  tower->AddNode(1785, 590, 0, CreateObject(REHR, 1785, 600, -1), 0, 1);
  tower->AddNode(922, 944, 1, CreateObject(REHR, 925, 970, -1));
  tower->AddNode(1672, 944, 1, CreateObject(REHR, 1675, 970, -1));

  //Verbundene Räume
  aDoorWay[00] = CreateObject(GAT1, 1300, 880, -1);
  aDoorWay[01] = CreateObject(ROOM, 1300, 1300, -1);
  aDoorWay[01]->Connect(aDoorWay[00]);
  aDoorWay[02] = CreateObject(GAT1, 1300, 680, -1);
  aDoorWay[03] = CreateObject(ROOM, 1300, 924, -1);
  aDoorWay[03]->Connect(aDoorWay[02]);

  //Nebel
  for(var i=0;i<180;++i)
    CreateParticle("Fog",Random(LandscapeWidth()),Random(LandscapeHeight()),0,0,RandomX(900,1700));

  //Soundkulisse

  //Erdrutsche
  CreateObject(SE4K, 90, 600, -1)->Set("FallingDirt*.ogg",800,300);
  CreateObject(SE4K, 90, 1200, -1)->Set("FallingDirt*.ogg",800,300);
  CreateObject(SE4K, 2510, 600, -1)->Set("FallingDirt*.ogg",800,300);
  CreateObject(SE4K, 2510, 1200, -1)->Set("FallingDirt*.ogg",800,300);

  //Wind
  CreateObject(SE4K, 500, 600, -1)->Set("WindSound*.ogg",300,200);
  CreateObject(SE4K, 500, 120, -1)->Set("WindSound*.ogg",300,200);
  CreateObject(SE4K, 2100, 600, -1)->Set("WindSound*.ogg",300,200);
  CreateObject(SE4K, 2100, 1200, -1)->Set("WindSound*.ogg",300,200);

  //Innenbereich
  CreateObject(SE4K, 1300, 1220, -1)->Set("InteriorStress*.ogg",700,100, 75);
  CreateObject(SE4K, 1300, 1220, -1)->Set("Interior*.ogg",700,100, 50,75);
  CreateObject(SE4K, 1300, 1220, -1)->Set("InteriorMetal*.ogg",700,100, 25,50);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Geschützstellungen
  aTowerInterior[0] = CreateObject(GNET, 1267, 742, -1);
  aTowerInterior[0] -> Set(0,-90);
  aTowerInterior[1] = CreateObject(GNET, 1267, 923, -1);
  aTowerInterior[1] -> Set(SATW,-90);
  CreateObject(GNET, 1240, 1300, -1)->Set(0,-90);
  aTowerInterior[2] = CreateObject(GNET, 1333, 742, -1);
  aTowerInterior[2] -> Set(0,90);
  aTowerInterior[3] = CreateObject(GNET, 1333, 923, -1);
  aTowerInterior[3] -> Set(SATW,90);
  CreateObject(GNET, 1360, 1300, -1)->Set(0,90);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Verbrannte Windmühlen
  CreateObject(WMLB, 68, 850, -1)->SetR(90);
  CreateObject(WMLB, 80, 550, -1);
  CreateObject(WMLB, 98, 392, -1)->SetR(35);
  CreateObject(WMLB, 520, 995, -1)->SetR(45);
  CreateObject(WMLB, 2070, 1031, -1);
  CreateObject(WMLB, 2100, 1030, -1)->SetR(-45);
  CreateObject(WMLB, 2510, 410, -1)->SetR(-45);
  CreateObject(WMLB, 2522, 890, -1)->SetR(-35);
  CreateObject(WMLB, 2530, 540, -1)->SetR(-45);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD03,500,1000,-1);
  CreateObject(BD03,1000,500,-1);
  CreateObject(BD03,2000,900,-1);
  CreateObject(BD05,1000,1750,-1);
  CreateObject(BD01,500,1750,-1);
}

/* Bei Turmzusammenfall */

func OnTowerCollapse()
{
  EventInfo4K(0,Format("$MsgCollapse$"),AATR, 0, 0, 0, "Info_Event.ogg");

  //Geschützstellungen entfernen
  if(aTowerInterior[0]) aTowerInterior[0]->DecoExplode(30);
  if(aTowerInterior[1]) aTowerInterior[1]->DecoExplode(30);
  if(aTowerInterior[2]) aTowerInterior[2]->DecoExplode(30);
  if(aTowerInterior[3]) aTowerInterior[3]->DecoExplode(30);
  if(aTowerInterior[4]) aTowerInterior[4]->DecoExplode(30);

  //Türverbindung entfernen
  aDoorWay[00]->SealEntrance(1);
  aDoorWay[01]->SealEntrance();
  aDoorWay[02]->SealEntrance(1);
  aDoorWay[03]->SealEntrance(1);
}

/* Apache bei Ausrüstung */

public func OnClassSelection(object pClonk)
{
  //Flugbereiten Apache erstellen
  var apache = CreateObject(APCE,GetX(pClonk),GetY(pClonk), GetOwner(pClonk));
  apache->SetAction("Fly");
  apache->EngineStarted();
  LocalN("throttle", apache) = 100;

  //Effekt
  AddSpawnEffect(apache, pClonk->GetColorDw());

  //Clonk hineinsetzen
  Enter(apache, pClonk);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Script starten
  ScriptGo(1);

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Back in the Earth.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Hinweisschilder
  CreateObject(SGNP, 60, 210, -1);
  CreateObject(SGNP, 45, 1340, -1);
  CreateObject(SGNP, 2540, 210, -1);
  CreateObject(SGNP, 2535, 1090, -1);

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Flaggenposten
    var flag = CreateObject(OFPL, 1300,742, -1);
    flag->~Set("$Flag1$", 250);

    //Grenzen setzen
    CreateObject(BRDR, 100, 0, -1)->Set(0);
    CreateObject(BRDR, 2500, 0, -1)->Set(1);
  }

  //LMS-Spielziel
  if(FindObject(GLMS))
  {
    //Grenzen setzen
    CreateObject(BRDR, 100, 0, -1)->Set(0);
    CreateObject(BRDR, 2500, 0, -1)->Set(1);

    //Artilleriebatterie
    aTowerInterior[4] = CreateObject(ATBY,1300,742,-1);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 1300; iY = 732;
    return 1;
  }

  //HTF/LMS-Spielziel
  if(FindObject(GHTF) || FindObject(GLMS))
    return [[600, 400], [600, 600], [650, 800], [650, 1000], [700, 1200], [1900, 400], [1950, 600], [1950, 800], [2000, 1000], [2000, 1200]];
}

/* Abwurfsteuerung */

protected func Script250()
{
  SupplyDrop();

  goto(0);
}

protected func SupplyDrop()
{
  EventInfo4K(0,Format("$MsgSupplyDrop$"),PARA, 0, 0, 0, "PriorityInfo.ogg");

  var obj = CreateObject(RndObjectID(), RandomX(300,650), -200, -1);
  obj->AddEffect("IntPara", obj, 1, 1);
  obj = CreateObject(RndObjectID(), RandomX(300,650), -250, -1);
  obj->AddEffect("IntPara", obj, 1, 1);
  obj = CreateObject(RndObjectID(), RandomX(300,650), -300, -1);
  obj->AddEffect("IntPara", obj, 1, 1);

  obj = CreateObject(RndObjectID(), RandomX(1950,2300), -200, -1);
  obj->AddEffect("IntPara", obj, 1, 1);
  obj = CreateObject(RndObjectID(), RandomX(1950,2300), -250, -1);
  obj->AddEffect("IntPara", obj, 1, 1);
  obj = CreateObject(RndObjectID(), RandomX(1950,2300), -300, -1);
  obj->AddEffect("IntPara", obj, 1, 1);

  CreateObject(SMWN,475,150, -1)->FadeOut(0,3);
  CreateObject(SMWN,2125,150, -1)->FadeOut(0,3);

  Sound("JetFlyBy*.ogg",1,0,30);
}

protected func RndObjectID()
{
 var r = Random(10);
 if (!r)   return(GSBL);
 if (!--r) return(PLLT);
 if (!--r) return(BECR);
 if (!--r) return(WCR2);
 if (!--r) return(XWCR);
 return(MWCR);
}

global func FxIntParaTimer(object pTarget)
{
  CreateObject(PARA,0,0,GetOwner(pTarget))->Set(pTarget);
  return -1;
}