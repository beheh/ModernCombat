/*-- Skyfall --*/

#strict 2
#include CSTD

static aFlag,aSelfDefense,aTowerInterior,aDoorWay;

func RecommendedGoals()	{return [GLMS];}	//Spielzielempfehlung

static const FKDT_SuicideTime = 5;		//Verkürzte Wartezeit


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Bildschirmfärbung
  SetGamma(RGB(15,15,15),RGB(118,118,118),RGB(215,215,215));
  //Himmelparallaxität
  SetSkyParallax(1,50,15);
  //Materialregen starten
  LaunchRain(0, Material("Rain"), LandscapeWidth(), 100)->SetCategory(1);
  //Globaler Regensound
  Sound("Rain.ogg",true,0,50,0,+1);
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
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
  //Neustart-Button setzen
  SetNextMission("ModernCombat.c4f\\Special.c4f\\CMC_Skyfall.c4s", "$RepeatButton$", "$RepeatButtonDesc$");
  return 1; 
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Sonne umplazieren
  FindObject(LENS)->SetPosition(2300,530);

  //Grenzen setzen
  CreateObject(BRDR, 100, 0, -1)->Set();
  CreateObject(BRDR, 4500, 0, -1)->Set(1);
  CreateObject(BRDR, 0, 15, -1)->Set(2);
  CreateObject(BRDR, 0, 1380, -1)->Set(3,0,1);

  //Sendemast
  var tower = CreateObject(AATR, 2300, 1300, -1);
  tower->AddNode(1823, 591, 0, CreateObject(REHR, 1815, 600, -1), 30, 1);
  tower->AddNode(2774, 593, 0, CreateObject(REHR, 2775, 603, -1), -30, 1);
  tower->AddNode(1932, 944, 1, CreateObject(REHR, 1925, 970, -1));
  tower->AddNode(2672, 944, 1, CreateObject(REHR, 2675, 970, -1));

  //Verbundene Räume
  aDoorWay[00] = CreateObject(GAT1, 2300, 880, -1);
  aDoorWay[01] = CreateObject(ROOM, 2300, 1300, -1);
  aDoorWay[01]->Connect(aDoorWay[00]);
  aDoorWay[02] = CreateObject(GAT1, 2300, 680, -1);
  aDoorWay[03] = CreateObject(ROOM, 2300, 924, -1);
  aDoorWay[03]->Connect(aDoorWay[02]);

  //Nebel
  for(var i=0;i<180;++i)
    CreateParticle("Fog",Random(LandscapeWidth()),Random(LandscapeHeight()),0,0,RandomX(1760,2840));

  //Soundkulisse

  //Erdrutsche
  CreateObject(SE4K, 90, 600, -1)->Set("FallingDirt*.ogg",800,300);
  CreateObject(SE4K, 90, 1200, -1)->Set("FallingDirt*.ogg",800,300);
  CreateObject(SE4K, 4510, 600, -1)->Set("FallingDirt*.ogg",800,300);
  CreateObject(SE4K, 4510, 1200, -1)->Set("FallingDirt*.ogg",800,300);

  //Wind
  CreateObject(SE4K, 500, 600, -1)->Set("WindSound*.ogg",300,200);
  CreateObject(SE4K, 500, 120, -1)->Set("WindSound*.ogg",300,200);
  CreateObject(SE4K, 4100, 600, -1)->Set("WindSound*.ogg",300,200);
  CreateObject(SE4K, 4100, 1200, -1)->Set("WindSound*.ogg",300,200);

  //Innenbereich
  CreateObject(SE4K, 2300, 1020, -1)->Set("InteriorStress*.ogg",700,100, 75);
  CreateObject(SE4K, 2300, 1120, -1)->Set("Interior*.ogg",700,100, 50,75);
  CreateObject(SE4K, 2300, 1220, -1)->Set("InteriorMetal*.ogg",700,100, 25,50);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Geschützstellungen
  aTowerInterior[0] = CreateObject(GNET, 2267, 742, -1);
  aTowerInterior[0] -> Set(0,-90);
  aTowerInterior[1] = CreateObject(GNET, 2267, 923, -1);
  aTowerInterior[1] -> Set(SATW,-90);
  CreateObject(GNET, 2240, 1300, -1)->Set(0,-90);
  aTowerInterior[2] = CreateObject(GNET, 2333, 742, -1);
  aTowerInterior[2] -> Set(0,90);
  aTowerInterior[3] = CreateObject(GNET, 2333, 923, -1);
  aTowerInterior[3] -> Set(SATW,90);
  CreateObject(GNET, 2360, 1300, -1)->Set(0,90);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Verbrannte Windmühlen
  CreateObject(WMLB, 68, 850, -1)->SetR(90);
  CreateObject(WMLB, 105, 480, -1);
  CreateObject(WMLB, 520, 995, -1)->SetR(45);
  CreateObject(WMLB, 3030, 1031, -1);
  CreateObject(WMLB, 3045, 1010, -1)->SetR(-45);
  CreateObject(WMLB, 4510, 410, -1)->SetR(-45);
  CreateObject(WMLB, 4522, 890, -1)->SetR(-35);
  CreateObject(WMLB, 4530, 540, -1)->SetR(-45);

  //Truhen
  CreateObject(CHST, 290, 260, -1);
  CreateObject(CHST, 4225, 230, -1);

  //Gerüste
  CreateObject(SFFG, 370, 1020, -1)->Set(5);
  CreateObject(SFFG, 1400, 670, -1)->Set(5);
  CreateObject(SFFG, 1480, 670, -1)->Set(5);
  CreateObject(SFFG, 3120, 670, -1)->Set(5);
  CreateObject(SFFG, 3200, 670, -1)->Set(5);
  CreateObject(SFFG, 4240, 1020, -1)->Set(5);

  //Container
  CreateObject(CON1, 490, 950, -1)->SetPerspective();
  CreateObject(CON1, 495, 922, -1)->SetPerspective(2);
  CreateObject(CON1, 4085, 922, -1)->SetPerspective(2);
  CreateObject(CON1, 4090, 950, -1)->SetPerspective();

  //Radare
  CreateObject(RADR, 310, 1020, -1);
  CreateObject(RADR, 4290, 1020, -1);

  //Geländer
  var rail = CreateObject(RAI1, 280, 1020, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1]);
  for(var i = 0; i <= 6; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  rail = CreateObject(RAI1, 528, 1130, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1]);
  for(var i = 0; i <= 6; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  rail = CreateObject(RAI1, 458, 950, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 16; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  rail = CreateObject(RAI1, 1358, 670, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 16; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  rail = CreateObject(RAI1, 3078, 670, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 16; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  rail = CreateObject(RAI1, 3978, 950, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 16; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  rail = CreateObject(RAI1, 4008, 1130, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1]);
  for(var i = 0; i <= 6; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  rail = CreateObject(RAI1, 4258, 1020, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1]);
  for(var i = 0; i <= 6; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);

  //Pumpen
  CreateObject(PMP2, 560, 1138, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(PMP2, 4040, 1138, -1)->SetClrModulation(RGB(125,125,125));

  //Objekte in den Hintergrund verschieben
  for(var obj in FindObjects(Find_Or(Find_ID(CON1), Find_ID(RADR), Find_ID(SFFG))))
  {
    obj->SetCategory(C4D_StaticBack);
    obj->SetCategory(C4D_Background);
    obj->SetClrModulation(RGB(125,125,125));
    obj->SetSolidMask();
  }
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD03,1500,1000,-1);
  CreateObject(BD03,2000,500,-1);
  CreateObject(BD03,3000,900,-1);
  CreateObject(BD05,2000,1750,-1);
  CreateObject(BD01,1500,1750,-1);
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

  //OCC-Spielziel
  if(FindObject(GOCC))
    aFlag[2]->SetClrModulation(RGB(150,150,150));
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

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[0] && aSelfDefense[0])
    aSelfDefense[0]->SetTeam(iTeam);

  if(pPoint == aFlag[4] && aSelfDefense[1])
    aSelfDefense[1]->SetTeam(iTeam);
}

/* Bei Relaunch */

public func OnClassSelection(object pClonk, int iClass)
{
  if(!FindObject(GOCC))
  {
    //Flugbereiten Apache erstellen
    var apache = CreateObject(APCE,GetX(pClonk),GetY(pClonk), GetOwner(pClonk));
    apache->SetAction("Fly");
    apache->EngineStarted();
    LocalN("throttle", apache) = 100;

    //Clonk hineinsetzen
    Enter(apache, pClonk);

    return _inherited(pClonk, ...);
  }
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Script starten
  ScriptGo(1);

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Back in the Earth.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Hinweisschilder
  CreateObject(SGNP, 40, 790, -1);
  CreateObject(SGNP, 45, 1340, -1);
  CreateObject(SGNP, 1010, 250, -1);
  CreateObject(SGNP, 3580, 230, -1);
  CreateObject(SGNP, 4550, 850, -1);
  CreateObject(SGNP, 4555, 1340, -1);

  //OCC-Spielziel
  if(FindObject(GOCC))
  {
    //Flaggenposten
    aFlag[0] = CreateObject(OFPL,600,950,NO_OWNER);
    aFlag[0] -> SetClrModulation(RGB(150,150,150));
    aFlag[0] -> AddSpawnPoint(300, 750);
    aFlag[0] -> AddSpawnPoint(400, 1200);
    aFlag[0] -> AddSpawnPoint(500, 600);
    if(aTeams[1] == true)
    {
      aFlag[0]->Set("$Flag1$",250,4);
      aFlag[0]->Capture(1,1);
    }
    else
    {
      aFlag[0]->Set("$Flag1$",250);
    }

    aFlag[1] = CreateObject(OFPL,1440,670,NO_OWNER);
    aFlag[1] -> SetClrModulation(RGB(150,150,150));
    aFlag[1] -> AddSpawnPoint(1240, 1200);
    aFlag[1] -> AddSpawnPoint(1440, 1200);
    aFlag[1] -> AddSpawnPoint(1640, 1200);
    if(aTeams[3] == true)
    {
      aFlag[1]->Set("$Flag2$",250,4);
      aFlag[1]->Capture(3,1);
    }
    else
    {
      aFlag[1]->Set("$Flag2$",250);
    }

    aFlag[2] = CreateObject(OFPL,2300,742,NO_OWNER);
    aFlag[2] -> AddSpawnPoint(2100,300);
    aFlag[2] -> AddSpawnPoint(2100,1100);
    aFlag[2] -> AddSpawnPoint(2500,300);
    aFlag[2] -> AddSpawnPoint(2500,1100);
    aFlag[2]->Set("$Flag3$",250);

    aFlag[3] = CreateObject(OFPL,3160,670,NO_OWNER);
    aFlag[3] -> SetClrModulation(RGB(150,150,150));
    aFlag[3] -> AddSpawnPoint(2960, 1200);
    aFlag[3] -> AddSpawnPoint(3160, 1200);
    aFlag[3] -> AddSpawnPoint(3360, 1200);
    if(aTeams[4] == true)
    {
      aFlag[3]->Set("$Flag4$",250,4);
      aFlag[3]->Capture(4,1);
    }
    else
    {
      aFlag[3]->Set("$Flag4$",250);
    }

    aFlag[4] = CreateObject(OFPL,4000,950,NO_OWNER);
    aFlag[4] -> SetClrModulation(RGB(150,150,150));
    aFlag[4] -> AddSpawnPoint(4300,750);
    aFlag[4] -> AddSpawnPoint(4200,1200);
    aFlag[4] -> AddSpawnPoint(4100,600);
    if(aTeams[2] == true)
    {
      aFlag[4]->Set("$Flag5$",250,4);
      aFlag[4]->Capture(2,1);
    }
    else
    {
      aFlag[4]->Set("$Flag5$",250);
    }

    //Selbstschussanlagen
    if(aTeams[1] == true)
    {
      aSelfDefense[0] = CreateObject(SEGU, 475, 950, -1);
      aSelfDefense[0]->SetCategory(C4D_StaticBack);
      aSelfDefense[0]->Set(0,1,1,180,0,970);
      aSelfDefense[0]->SetTeam(1);
    }
    if(aTeams[2] == true)
    {
      aSelfDefense[1] = CreateObject(SEGU, 4120, 950, -1);
      aSelfDefense[1]->SetCategory(C4D_StaticBack);
      aSelfDefense[1]->Set(0,1,1,180,3630,4600);
      aSelfDefense[1]->SetTeam(2);
    }
  }

  //LMS-Spielziel
  if(FindObject(GLMS))
  {
    //Artilleriebatterie
    aTowerInterior[4] = CreateObject(ATBY,2300,742,-1);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 2300; iY = 602;
   return 1;
  }

  //LMS-Spielziel
  if(FindObject(GLMS))
  {
   return [[400, 600], [535, 950], [735, 1200], [1020, 1200], [1320, 300], [3280, 300], [3580, 1200], [3860, 1200], [4060, 950], [4200, 600]];
   return 1;
  }
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

  var obj = CreateObject(RndObjectID(), RandomX(1220,1570), -100, -1);
  obj->AddEffect("IntPara", obj, 1, 1);
  obj = CreateObject(RndObjectID(), RandomX(1220,1570), -150, -1);
  obj->AddEffect("IntPara", obj, 1, 1);
  obj = CreateObject(RndObjectID(), RandomX(1220,1570), -200, -1);
  obj->AddEffect("IntPara", obj, 1, 1);

  obj = CreateObject(RndObjectID(), RandomX(3030,3380), -100, -1);
  obj->AddEffect("IntPara", obj, 1, 1);
  obj = CreateObject(RndObjectID(), RandomX(3030,3380), -150, -1);
  obj->AddEffect("IntPara", obj, 1, 1);
  obj = CreateObject(RndObjectID(), RandomX(3030,3380), -200, -1);
  obj->AddEffect("IntPara", obj, 1, 1);

  CreateObject(SMWN,1395,150, -1)->FadeOut(0,3);
  CreateObject(SMWN,3205,150, -1)->FadeOut(0,3);

  Sound("JetFlyBy*.ogg",1,0,30);
}

protected func RndObjectID()
{
 var r = Random(10);
 if (!r)   return(PLLT);
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