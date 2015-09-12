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
  return 1;
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Bodenluken
  CreateObject(HA4K, 610, 333, -1);
  CreateObject(HA4K, 1000, 393, -1);
  CreateObject(HA4K, 1540, 343, -1);
  CreateObject(HA4K, 1540, 463, -1);
  CreateObject(HA4K, 1940, 373, -1);
  CreateObject(HA4K, 2320, 243, -1);

  //Leitern
  CreateObject(LADR, 446, 340, -1)->Set(4);
  CreateObject(LADR, 610, 442, -1)->Set(13);
  CreateObject(LADR, 1000, 502, -1)->Set(13);
  CreateObject(LADR, 1540, 540, -1)->Set(24);
  CreateObject(LADR, 1940, 466, -1)->Set(11);
  CreateObject(LADR, 2320, 528, -1)->Set(35);

  //Sprungschanzen
  CreateObject(JMPD, 565, 330, -1)->Set(100, 20);
  CreateObject(JMPD, 1575, 300, -1)->Set(85, 10);

  //Stahlbrücken
  CreateObject(_HBR, 985, 660, -1);
  CreateObject(_HBR, 1940, 475, -1);
  CreateObject(_HBR, 2125, 510, -1);

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
  CreateObject(GSBL, 1960, 370, -1)->AutoRespawn();

  //Steine
  CreateObject(STNE, 110, 430, -1)->Set(2);
  CreateObject(STNE, 170, 740, -1)->Set(3);
  CreateObject(STNE, 370, 435, -1);
  CreateObject(STNE, 600, 655, -1)->Set(2);
  CreateObject(STNE, 1030, 165, -1)->Set(0);
  CreateObject(STNE, 1340, 435, -1)->Set(2);
  CreateObject(STNE, 1845, 495, -1)->Set(0);
  CreateObject(STNE, 2470, 375, -1);
  CreateObject(STNE, 2700, 340, -1)->Set(4);
  CreateObject(STNE, 2730, 730, -1)->Set(0);


  //Kisten
  CreateObject(WCR2, 960, 260, -1)->AutoRespawn();
  CreateObject(WCR2, 970, 242, -1);
  CreateObject(WCR2, 980, 260, -1)->AutoRespawn();
  CreateObject(WCR2, 1040, 390, -1)->AutoRespawn();
  CreateObject(WCR2, 1044, 370, -1);
  CreateObject(WCR2, 1980, 370, -1)->AutoRespawn();

  //Verbandskiste
  CreateObject(BECR, 1490, 460, -1)->AutoRespawn();

  //Hinweisschild
  var sign = CreateObject(SGNP, 1590, 550, -1);
  sign->SetMode(1);
  sign->Set("Turret");

  //Selbstschussanlage und Konsole
  aSelfDefense[0] = CreateObject(SEGU, 1505, 369, -1);
  CreateObject(CONS, 1500, 330, -1)->Set(aSelfDefense[0]);

  //Sounds

  //Lava
  CreateObject(SE4K, 350, 660, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 910, 660, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 1400, 580, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 1930, 500, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 2460, 310, -1)->Set("Lava*",1200,350);

  //Erdrutsche
  CreateObject(SE4K, 670, 480, -1)->Set("FallingDirt*.ogg",800,250);
  CreateObject(SE4K, 1400, 200, -1)->Set("FallingDirt*.ogg",800,250);
  CreateObject(SE4K, 2440, 450, -1)->Set("FallingDirt*.ogg",800,250);

  //Hallen
  CreateObject(SE4K, 490, 250, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1475, 395, -1)->Set("Interior*.ogg",765,105);
  CreateObject(SE4K, 2370, 290, -1)->Set("Interior*.ogg",665,105);

  //Lautsprecher
  CreateObject(SE4K, 745, 250, -1)->Set("Announce*.ogg",3000,200);
  CreateObject(SE4K, 2125, 210, -1)->Set("Announce*.ogg",3000,200);
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

  //Alarmlampen
  CreateObject(ALGH, 385, 260, -1)->TurnOn();
  CreateObject(ALGH, 1540, 250, -1)->TurnOn();
  CreateObject(ALGH, 2500, 170, -1)->TurnOn();

  //Radar
  CreateObject(RADR, 2285, 240, -1);

  //Feuerlöscher
  CreateObject(FIE2, 415, 295, -1);
  CreateObject(FIE2, 1575, 335, -1);
  CreateObject(FIE2, 2520, 205, -1);

  //Bildschirme
  CreateObject(SCR3, 610, 271, -1);
  CreateObject(SCR3, 1540, 281, -1);
  CreateObject(SCR3, 2320, 151, -1);

  //Ventillatoren
  CreateObject(VENT, 565, 240, -1)->SetCon(20);
  CreateObject(VENT, 565, 290, -1)->SetCon(20);
  CreateObject(VENT, 565, 330, -1)->SetCon(20);
  CreateObject(VENT, 965, 300, -1)->SetCon(20);
  CreateObject(VENT, 965, 350, -1)->SetCon(20);
  CreateObject(VENT, 965, 390, -1)->SetCon(20);
  CreateObject(VENT, 1505, 375, -1)->SetCon(20);
  CreateObject(VENT, 1575, 375, -1)->SetCon(20);
  CreateObject(VENT, 1975, 330, -1)->SetCon(20);
  CreateObject(VENT, 2285, 520, -1)->SetCon(20);
  CreateObject(VENT, 2355, 155, -1)->SetCon(20);
  CreateObject(VENT, 2355, 200, -1)->SetCon(20);
  CreateObject(VENT, 2355, 240, -1)->SetCon(20);

  //Regale
  CreateObject(FRAM, 630, 330, -1);
  CreateObject(FRAM, 650, 330, -1);
  CreateObject(FRAM, 670, 330, -1);

  //Büsche
  CreateObject(BSH2, 495, 130, -1);
  CreateObject(BSH2, 2505, 515, -1);

  //Scheinwerfer
  CreateObject(FLGH, 645, 330, -1)->SetRotation(20);
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
  CreateObject(MSGN, 340, 300, -1);
  CreateObject(MSGN, 430, 300, -1);
  CreateObject(MSGN, 2440, 210, -1);
  CreateObject(MSGN, 2530, 210, -1);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[2])
    aSelfDefense[0]->SetTeam(iTeam);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Grenade.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Toward the Flag.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Enemy Approaching.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Sprungschanzen
  if(FindObject(FDMG))
  {
    CreateObject(JMPD, 1585, 460, -1)->Set(40, -20);
    CreateObject(JMPD, 1890, 370, -1)->Set(75, -80);
  }
  else
  {
    CreateObject(JMPD, 1890, 370, -1)->Set(85, -80);
  }

  //OCC-Spielziel
  if(FindObject(GOCC))
  {
    //Flaggenposten
    aFlag[0] = CreateObject(OFPL,365,300,NO_OWNER);
    aFlag[0] -> AddSpawnPoint(500,490);
    aFlag[0] -> AddSpawnPoint(555,450);
    aFlag[0] -> AddSpawnPoint(745,340);
    if(aTeams[1] == true)
    {
      aFlag[0]->Set("$Flag1$",0,4);
      aFlag[0]->Capture(1,1);
    }
    else
    {
      aFlag[0]->Set("$Flag1$");
    }

    aFlag[1] = CreateObject(OFPL,980,390,NO_OWNER);
    aFlag[1] -> AddSpawnPoint(930,510);
    aFlag[1] -> AddSpawnPoint(1025,570);
    aFlag[1] -> AddSpawnPoint(1085,480);
    aFlag[1]->Set("$Flag2$");

    aFlag[2] = CreateObject(OFPL,1575,460,NO_OWNER);
    aFlag[2] -> AddSpawnPoint(1485,540);
    aFlag[2] -> AddSpawnPoint(1595,540);
    aFlag[2] -> AddSpawnPoint(1405,410);
    aFlag[2]->Set("$Flag3$");

    aFlag[3] = CreateObject(OFPL,1905,330,NO_OWNER);
    aFlag[3] -> AddSpawnPoint(1865,150);
    aFlag[3] -> AddSpawnPoint(1990,130);
    aFlag[3]->Set("$Flag4$");

    aFlag[4] = CreateObject(OFPL,2355,560,NO_OWNER);
    aFlag[3] -> AddSpawnPoint(2055,440);
    aFlag[4] -> AddSpawnPoint(2180,510);
    aFlag[4] -> AddSpawnPoint(2290,470);
    aFlag[4]->Set("$Flag5$");

    aFlag[5] = CreateObject(OFPL,2455,210,NO_OWNER);
    aFlag[5] -> AddSpawnPoint(2200,300);
    aFlag[5] -> AddSpawnPoint(2300,300);
    aFlag[5] -> AddSpawnPoint(2340,300);
    if(aTeams[2] == true)
    {
      aFlag[5]->Set("$Flag6$",0,4);
      aFlag[5]->Capture(2,1);
    }
    else
    {
      aFlag[5]->Set("$Flag6$");
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
    AddAssaultTarget(CCP2, 390, 300, 350, 1, "$Target1$", 0, [[475, 570], [550, 460], [635, 620]]);
    AddAssaultTarget(GSTA, 980, 390, 350, 1, "$Target2$", 1, [[720, 350], [805, 520], [1030, 650]]);
    AddAssaultTarget(CMSN, 1040, 260, 350, 1, "$Target3$", 2, [[810, 140], [815, 300], [1070, 130]]);
    AddAssaultTarget(CMSN, 1865, 160, 350, 2, "$Target3$", 3, [[2065, 255], [2130, 310], [2150, 140]]);
    AddAssaultTarget(GSTA, 1910, 330, 350, 2, "$Target2$", 4, [[1815, 500], [2020, 450], [2170, 300]]);
    AddAssaultTarget(CCP2, 2345, 240, 350, 2, "$Target1$", 5, [[2150, 140], [2300, 480], [2350, 560]]);

    //Sprengsatz-Spawn
    SetupBombSpawnpoint([[1520, 300],[1520,340],[1560,460]]);

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
    {CreateFlag(1,655,330,GetTeamColor(1));}
    if(aTeams[2] == true)
    {CreateFlag(2,2210,310,GetTeamColor(2));}

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
    CreateObject(BRDR, 710, 0, -1)->Set(0);
    CreateObject(BRDR, 2165, 0, -1)->Set(1);

    //Hinweisschilder
    CreateObject(SGNP, 725, 350, -1);
    CreateObject(SGNP, 785, 650, -1);
    CreateObject(SGNP, 760, 530, -1);
    CreateObject(SGNP, 860, 150, -1);
    CreateObject(SGNP, 2050, 130, -1);
    CreateObject(SGNP, 2050, 450, -1);
    CreateObject(SGNP, 2160, 300, -1);

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
    iX = 1745; iY = 350;
    return 1;
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
    if(iTeam == 1)
      return [[350, 290], [420, 290]];
    if(iTeam == 2)
      return [[2355, 230], [2450, 200]];
    return 1;
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
    if(iTeam == 1)
      return [[835, 280], [885, 390]];
    if(iTeam == 2)
      return [[2025, 350], [2045, 230]];
    return 1;
  }
}