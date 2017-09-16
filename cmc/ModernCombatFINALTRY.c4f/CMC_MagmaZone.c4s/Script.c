/*-- Magma Zone --*/

#strict 2
#include CSTD

static aFlag,aSelfDefense;

func RecommendedGoals()	{return [GOCC, GBAS];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(0,15,15);
  //Flaggen
  aFlag = [];
  //Selbstschussanlage
  aSelfDefense = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  //Neustart-Button setzen
  SetNextMission("ModernCombat.c4f\\CMC_MagmaZone.c4s", "$RepeatButton$", "$RepeatButtonDesc$");
  return 1;
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Rampen
  DrawMaterialQuad("Wall-Concrete1",500,350,530,330,530,350,500,350,true);
  DrawMaterialQuad("Tunnel-Rough",1430,415,1435,410,1455,430,1455,435,true);
  DrawMaterialQuad("Tunnel-Rough",1680,285,1720,285,1720,291,1680,291,true);
  DrawMaterialQuad("Tunnel-Rough",1760,295,1820,295,1820,301,1760,301,true);
  DrawMaterialQuad("Wall-Concrete1",1740,380,1790,370,1790,391,1740,391,true);
  DrawMaterialQuad("Wall-Concrete1",2140,310,2160,300,2160,310,2140,310,true);
  DrawMaterialQuad("Wall-Concrete1",2180,300,2200,310,2180,310,2180,300,true);

  //Bodenluken
  CreateObject(HA4K, 610, 333, -1);
  CreateObject(HA4K, 1000, 393, -1);
  CreateObject(HA4K, 1540, 343, -1);
  CreateObject(HA4K, 1540, 463, -1);
  CreateObject(HA4K, 1940, 373, -1);
  CreateObject(HA4K, 2320, 243, -1);
  CreateObject(HA4K, 2320, 313, -1);

  //Leitern
  CreateObject(LADR, 446, 340, -1)->Set(4);
  CreateObject(LADR, 610, 442, -1)->Set(13);
  CreateObject(LADR, 695, 545, -1)->Set(26);
  CreateObject(LADR, 1000, 502, -1)->Set(13);
  CreateObject(LADR, 1540, 540, -1)->Set(24);
  CreateObject(LADR, 1940, 458, -1)->Set(10);
  CreateObject(LADR, 2195, 480, -1)->Set(10);
  CreateObject(LADR, 2320, 440, -1)->Set(24);
  CreateObject(LADR, 2320, 544, -1)->Set(7);
  CreateObject(LADR, 2401, 303, -1)->Set(9);

  //Sprungschanzen
  CreateObject(JMPD, 560, 350, -1)->Set(105, 20);
  CreateObject(JMPD, 1575, 300, -1)->Set(85, 10);

  //Stahlbrücken
  CreateObject(_HBR, 455, 362, -1);
  CreateObject(_HBR, 617, 472, -1);
  CreateObject(_HBR, 985, 662, -1)->SwitchMode();
  CreateObject(_HBR, 1845, 382, -1)->SwitchMode();
  CreateObject(_HBR, 1945, 472, -1)->SwitchMode();
  CreateObject(_HBR, 2145, 512, -1)->SwitchMode();
  CreateObject(_HBR, 2265, 322, -1)->SwitchMode();
  CreateObject(_HBR, 2375, 322, -1)->SwitchMode();

  //Benzinfässer
  CreateObject(PBRL, 825, 520, -1)->AutoRespawn();
  CreateObject(PBRL, 1990, 290, -1)->AutoRespawn();

  //Giftfässer
  CreateObject(TBRL, 1580, 340, -1)->AutoRespawn();
  CreateObject(TBRL, 1595, 340, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 2265, 480, -1)->AutoRespawn();
  CreateObject(XBRL, 2300, 560, -1)->AutoRespawn();
  CreateObject(XBRL, 2315, 560, -1)->AutoRespawn();

  //Explosivtank
  CreateObject(XTNK, 2300, 560, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1000, 260, -1)->AutoRespawn();
  CreateObject(GSBL, 1020, 390, -1)->AutoRespawn();
  CreateObject(GSBL, 1765, 375, -1)->AutoRespawn();
  CreateObject(GSBL, 1960, 370, -1)->AutoRespawn();

  //Gerüste
  CreateObject(SFFG, 2145, 450, -1)->Set(4);
  CreateObject(SFFG, 2145, 500, -1)->Set(5);

  //Steine
  CreateObject(STNE, 110, 430, -1)->Set(2);
  CreateObject(STNE, 170, 740, -1)->Set(3);
  CreateObject(STNE, 370, 435, -1);
  CreateObject(STNE, 600, 655, -1)->Set(2);
  CreateObject(STNE, 1030, 165, -1)->Set(0);
  CreateObject(STNE, 1340, 435, -1)->Set(2);
  CreateObject(STNE, 1790, 500, -1)->Set(0);
  CreateObject(STNE, 2470, 375, -1);
  CreateObject(STNE, 2700, 340, -1)->Set(4);
  CreateObject(STNE, 2730, 730, -1)->Set(0);
  CreateObject(STNE, 2800, 200, -1)->Set(0);

  //Kisten
  CreateObject(WCR2, 960, 260, -1)->AutoRespawn();
  CreateObject(WCR2, 970, 242, -1);
  CreateObject(WCR2, 980, 260, -1)->AutoRespawn();
  CreateObject(WCR2, 1040, 390, -1)->AutoRespawn();
  CreateObject(WCR2, 1050, 372, -1);
  CreateObject(WCR2, 1980, 370, -1)->AutoRespawn();
  CreateObject(WCR2, 2350, 310, -1)->AutoRespawn();

  //Verbandskiste
  CreateObject(BECR, 1490, 460, -1)->AutoRespawn();

  //Hinweisschilder
  CreateObject(WLSN, 610, 235, -1)->Set("A");
  var sign = CreateObject(SGNP, 1590, 550, -1);
  sign->SetMode(1);
  sign->Set("Turret");
  CreateObject(WLSN, 2320, 125, -1)->Set("B");

  //Selbstschussanlage und Konsole
  aSelfDefense[0] = CreateObject(SEGU, 1505, 369, -1);
  CreateObject(CONS, 1500, 330, -1)->Set(aSelfDefense[0]);

  //Soundkulisse

  //Lava
  CreateObject(SE4K, 350, 660, -1)->Set("Lava*",1200,300, 75);
  CreateObject(SE4K, 910, 660, -1)->Set("Lava*",1200,300, 75);
  CreateObject(SE4K, 1400, 580, -1)->Set("Lava*",1200,300, 75);
  CreateObject(SE4K, 1930, 500, -1)->Set("Lava*",1200,300, 75);
  CreateObject(SE4K, 2460, 310, -1)->Set("Lava*",1200,300, 75);

  //Erdrutsche
  CreateObject(SE4K, 670, 480, -1)->Set("FallingDirt*.ogg",900,200, 75);
  CreateObject(SE4K, 1400, 200, -1)->Set("FallingDirt*.ogg",900,200, 75);
  CreateObject(SE4K, 2440, 450, -1)->Set("FallingDirt*.ogg",900,200, 75);

  //Lautsprecher
  CreateObject(SE4K, 745, 250, -1)->Set("Announce*.ogg",3000,200, 75);
  CreateObject(SE4K, 2125, 210, -1)->Set("Announce*.ogg",3000,200, 75);

  //Innenbereich
  CreateObject(SE4K, 490, 250, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 1475, 395, -1)->Set("InteriorStress*.ogg",700,100, 75);
  CreateObject(SE4K, 2370, 290, -1)->Set("Interior*.ogg",700,100, 75);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 1490, 80);

  //Raketen
  PlaceSpawnpoint(MBOX, 1560, 330);

  //Automat
  var store = CreateObject(WPVM,1505, 300,-1);
  store->AddWare(C4PA,10);
  store->AddWare(FAPK,10);
  store->AddWare(CDBT,10);
  store->AddWare(BWTH,10);
  store->AddWare(RSHL,10);
  store->AddWare(ATWN,20);
  store->AddWare(FGRN,20);
  store->AddWare(FRAG,20);
  store->AddWare(PGRN,20);
  store->AddWare(SGRN,20);
  store->AddWare(STUN,20);
  store->AddWare(SRBL,20);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Rohre
  CreateObject(PIPL, 610, 365, -1)->Down()->Right()->Down()->Right()->Right()->Right()->Down()->Right()->Down()->Down()->Down();
  CreateObject(PIPL, 750, 675, -1)->Up()->Right()->Right()->Right();
  CreateObject(PIPL, 955, 675, -1)->Up()->Left();
  CreateObject(PIPL, 1000, 425, -1)->Down()->Left()->Down()->Left()->Down()->Down()->Down();
  CreateObject(PIPL, 1590, 455, -1)->Right()->Right()->Right()->Right()->Right()->Right()->Up();
  CreateObject(PIPL, 2010, 323, -1)->Right()->Right()->Up()->Right()->Right()->Right();
  CreateObject(PIPL, 2280, 505, -1)->Up()->Left()->Left()->Left()->Left()->Left()->Left()->Up();
  CreateObject(PIPL, 2370, 540, -1)->Right()->Right()->Up()->Right()->Right()->Right()->Right()->Right()->Right()->Right()->Right()->Right();

  //Alarmlampen
  CreateObject(ALGH, 385, 260, -1)->TurnOn();
  CreateObject(ALGH, 1540, 250, -1)->TurnOn();
  CreateObject(ALGH, 2485, 170, -1)->TurnOn();

  //Radar
  CreateObject(RADR, 2285, 240, -1);

  //Feuerlöscher
  CreateObject(FIE2, 340, 295, -1);
  CreateObject(FIE2, 940, 645, -1);
  CreateObject(FIE2, 1575, 335, -1);
  CreateObject(FIE2, 2185, 495, -1);
  CreateObject(FIE2, 2530, 205, -1);

  //Bildschirme
  CreateObject(SCR3, 400, 290, -1);
  CreateObject(SCR3, 610, 320, -1);
  CreateObject(SCR3, 1540, 281, -1);
  var screen = CreateObject(SCA2, 1540, 420, -1);
  screen->SetClips([12]);
  screen->SetClrModulation(RGB(50,255,50));
  CreateObject(SCR3, 2320, 230, -1);
  CreateObject(SCR3, 2500, 200, -1);

  //Ventillatoren
  CreateObject(MVNT, 610, 302, -1);
  CreateObject(VENT, 965, 300, -1)->SetCon(20);
  CreateObject(VENT, 965, 350, -1)->SetCon(20);
  CreateObject(VENT, 1035, 350, -1)->SetCon(20);
  CreateObject(VENT, 1505, 375, -1)->SetCon(20);
  CreateObject(VENT, 1575, 375, -1)->SetCon(20);
  CreateObject(VEN3, 2320, 450, -1)->SetCon(35);
  CreateObject(VEN3, 2320, 550, -1)->SetCon(35);
  CreateObject(MVNT, 2320, 202, -1);

  //Pumpen
  CreateObject(PMP2, 880, 640, -1);
  CreateObject(PMP2, 1975, 350, -1);

  //Regale
  CreateObject(FRAM, 725, 350, -1);
  CreateObject(FRAM, 745, 350, -1);

  //Büsche
  CreateObject(BSH2, 495, 130, -1);
  CreateObject(BSH2, 2505, 515, -1);

  //Scheinwerfer
  CreateObject(FLGH, 590, 330, -1)->SetRotation(20);
  CreateObject(FLGH, 1975, 290, -1)->SetRotation(-80);
  CreateObject(FLGH, 2360, 240, -1)->SetRotation(-20);

  //Geländer
  CreateObject(RAI1, 343, 300, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 960, 390, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1500, 340, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1500, 460, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1900, 370, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2280, 240, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2280, 560, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2450, 210, -1)->SetRail([1,3,1,3,1,3,1]);

  //Schilder
  CreateObject(MSGN, 495, 350, -1);
  CreateObject(MSGN, 845, 520, -1);
  CreateObject(MSGN, 915, 520, -1);
  CreateObject(MSGN, 1745, 379, -1);
  CreateObject(MSGN, 2465, 310, -1);

  //Poster
  CreateObject(PSTR, 1001, 320, -1)->Set(3,2);
  CreateObject(PSTR, 1001, 360, -1)->Set(3,0);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[3])
    aSelfDefense[0]->SetTeam(iTeam);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Enemy Approaching.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OCC-Spielziel
  if(FindObject(GOCC))
  {
    //Flaggenposten
    aFlag[0] = CreateObject(OFPL,365,300,NO_OWNER);
    aFlag[0] -> AddSpawnPoint(430,490);
    aFlag[0] -> AddSpawnPoint(460,540);
    aFlag[0] -> AddSpawnPoint(500,490);
    aFlag[0] -> AddSpawnPoint(560,450);
    if(aTeams[1] == true)
    {
      aFlag[0]->Set("$Flag1$",80,4);
      aFlag[0]->Capture(1,1);
    }
    else
    {
      aFlag[0]->Set("$Flag1$",80);
    }

    aFlag[1] = CreateObject(OFPL,860,650,NO_OWNER);
    aFlag[1] -> AddSpawnPoint(700,570);
    aFlag[1] -> AddSpawnPoint(760,520);
    aFlag[1] -> AddSpawnPoint(635,610);
    aFlag[1]->Set("$Flag2$");

    aFlag[2] = CreateObject(OFPL,980,390,NO_OWNER);
    aFlag[2] -> AddSpawnPoint(860,140);
    aFlag[2] -> AddSpawnPoint(1010,130);
    aFlag[2] -> AddSpawnPoint(1030,250);
    aFlag[2] -> AddSpawnPoint(1090,110);
    aFlag[2]->Set("$Flag3$");

    aFlag[3] = CreateObject(OFPL,1575,460,NO_OWNER);
    aFlag[3] -> AddSpawnPoint(1405,410);
    aFlag[3] -> AddSpawnPoint(1485,540);
    aFlag[3] -> AddSpawnPoint(1595,540);
    aFlag[3] -> AddSpawnPoint(1710,520);
    aFlag[3]->Set("$Flag4$",95);

    aFlag[4] = CreateObject(OFPL,1905,330,NO_OWNER);
    aFlag[4] -> AddSpawnPoint(1865,150);
    aFlag[4] -> AddSpawnPoint(1980,210);
    aFlag[4] -> AddSpawnPoint(1990,130);
    aFlag[4]->Set("$Flag5$");

    aFlag[5] = CreateObject(OFPL,2355,560,NO_OWNER);
    aFlag[5] -> AddSpawnPoint(2045,440);
    aFlag[5] -> AddSpawnPoint(2100,490);
    aFlag[5] -> AddSpawnPoint(2145,390);
    aFlag[5]->Set("$Flag6$",90);

    aFlag[6] = CreateObject(OFPL,2455,210,NO_OWNER);
    aFlag[6] -> AddSpawnPoint(2210,300);
    aFlag[6] -> AddSpawnPoint(2300,300);
    aFlag[6] -> AddSpawnPoint(2340,300);
    aFlag[6] -> AddSpawnPoint(2440,300);
    if(aTeams[2] == true)
    {
      aFlag[6]->Set("$Flag7$",80,4);
      aFlag[6]->Capture(2,1);
    }
    else
    {
      aFlag[6]->Set("$Flag7$",80);
    }

    //Zusätzliche Munition
    if(!FindObject(NOAM))
    {
      //Versorgungskiste (Kugeln)
      var crate = CreateObject(AMCT, 445, 550, -1);
      crate->Set(ABOX);
      crate = CreateObject(AMCT, 1515, 460, -1);
      crate->Set(ABOX);
      crate = CreateObject(AMCT, 2275, 560, -1);
      crate->Set(ABOX);
    }
  }

  //BAS-Spielziel
  if(FindObject(GBAS))
  {
    //Strukturen
    AddAssaultTarget(CCP2, 390, 300, 350, 1, "$Target1$", 0, [[430, 500], [550, 620], [725, 550]]);
    AddAssaultTarget(GSTA, 980, 390, 350, 1, "$Target2$", 1, [[565, 460], [800, 520], [1030, 650]]);
    AddAssaultTarget(CMSN, 1040, 260, 350, 1, "$Target3$", 2, [[720, 350], [800, 140], [1070, 130]]);
    AddAssaultTarget(CMSN, 1865, 160, 350, 2, "$Target3$", 3, [[2065, 255], [2130, 310], [2150, 140]]);
    AddAssaultTarget(GSTA, 1910, 330, 350, 2, "$Target2$", 4, [[1815, 500], [2030, 450], [2170, 300]]);
    AddAssaultTarget(CCP2, 2345, 240, 350, 2, "$Target1$", 5, [[2075, 130], [2100, 320], [2290, 480]]);

    //Sprengsatz-Spawn
    SetupBombSpawnpoint([[1520, 300],[1520, 340],[1520,460]]);

    //Zusätzliche Munition
    if(!FindObject(NOAM))
    {
      //Versorgungskiste (Kugeln)
      var crate = CreateObject(AMCT, 445, 550, -1);
      crate->Set(ABOX);
      crate = CreateObject(AMCT, 1515, 460, -1);
      crate->Set(ABOX);
      crate = CreateObject(AMCT, 2275, 560, -1);
      crate->Set(ABOX);
    }
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
    //Flaggen
    if(aTeams[1] == true)
    {CreateFlag(1,660,330,GetTeamColor(1));}
    if(aTeams[2] == true)
    {CreateFlag(2,2170,300,GetTeamColor(2));}

    //Sandsackbarrieren
    CreateObject(SBBA, 430, 300, -1)->Right();
    CreateObject(SBBA, 2271, 240, -1);

    //Zusätzliche Munition
    if(!FindObject(NOAM))
    {
      //Versorgungskiste (Kugeln)
      var crate = CreateObject(AMCT, 805, 520, -1);
      crate->Set(ABOX);
      crate = CreateObject(AMCT, 2025, 450, -1);
      crate->Set(ABOX);
    }
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
    //Grenzen setzen
    CreateObject(BRDR, 540, 0, -1)->Set(0);
    CreateObject(BRDR, 2340, 0, -1)->Set(1);

    //Hinweisschilder
    CreateObject(SGNP, 540, 330, -1);
    CreateObject(SGNP, 550, 620, -1);
    CreateObject(SGNP, 560, 460, -1);
    CreateObject(SGNP, 2340, 310, -1);
    CreateObject(SGNP, 2370, 240, -1);
    CreateObject(SGNP, 2370, 560, -1);

    //Sandsackbarrieren
    CreateObject(SBBA, 750, 350, -1)->Right();
    CreateObject(SBBA, 1961, 290, -1);

    //Zusätzliche Munition
    if(!FindObject(NOAM))
    {
      //Versorgungskiste (Kugeln)
      var crate = CreateObject(AMCT, 805, 520, -1);
      crate->Set(ABOX);
      crate = CreateObject(AMCT, 2025, 450, -1);
      crate->Set(ABOX);
    }
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 1745; iY = 380;
    return 1;
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
    if(iTeam == 1)
      return [[385, 290], [430, 490], [465, 540]];
    if(iTeam == 2)
      return [[2100, 490], [2355, 230], [2450, 200]];
    return 1;
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
    if(iTeam == 1)
      return [[835, 280], [835, 370], [885, 390], [930, 250]];
    if(iTeam == 2)
      return [[1980, 210], [2025, 350], [2045, 230], [2100, 310]];
    return 1;
  }
}