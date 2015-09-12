/*-- Macross Frontier --*/

#strict 2
#include CSTD

static aDoorWay;

func RecommendedGoals()	{return [GHTF, GTDM];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(1,50,15);
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

  //Leitern
  CreateObject(LADR, 295, 635, -1)->Set(10);
  CreateObject(LADR, 506, 597, -1)->Set(5);
  CreateObject(LADR, 521, 510, -1)->Set(9);
  CreateObject(LADR, 681, 510, -1)->Set(9);
  CreateObject(LADR, 861, 466, -1)->Set(11);

  CreateObject(LADR, 1091, 386, -1)->Set(11);
  CreateObject(LADR, 1271, 430, -1)->Set(9);
  CreateObject(LADR, 1432, 430, -1)->Set(9);
  CreateObject(LADR, 1448, 518, -1)->Set(5);
  CreateObject(LADR, 1657, 555, -1)->Set(10);

  //Bodenluken
  CreateObject(HA4K, 520, 433, -1);
  CreateObject(HA4K, 680, 433, -1);
  CreateObject(HA4K, 860, 373, -1);
  CreateObject(HA4K, 930, 643, -1);
  CreateObject(HA4K, 990, 643, -1);
  CreateObject(HA4K, 1010, 563, -1);
  CreateObject(HA4K, 1090, 293, -1);
  CreateObject(HA4K, 1270, 353, -1);
  CreateObject(HA4K, 1430, 353, -1);

  //Hydrauliktüren
  var door = CreateObject(SLDR, 255, 640, -1);
  door->Lock();
  door->SetMaxDamage(-1);
  door = CreateObject(SLDR, 1055, 700, -1);
  door->Lock();
  door->SetMaxDamage(-1);
  CreateObject(PKEY, 1040, 695, -1)->Set(door,1337);
  door = CreateObject(SLDR, 1695, 560, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  //Schutztüren
  CreateObject(GDDR, 85, 640, -1);
  CreateObject(GDDR, 1865, 560, -1);

  //Rampen
  DrawMaterialQuad("Wall-Concrete2",560,550,590,540,590,550,575,550,true);
  DrawMaterialQuad("Wall-Concrete2",651,550,621,540,621,550,636,550,true);

  DrawMaterialQuad("Wall-Concrete2",661,670,631,660,631,670,646,670,true);
  DrawMaterialQuad("Wall-Concrete2",691,680,661,670,661,680,676,680,true);
  DrawMaterialQuad("Wall-Concrete2",631,660,601,650,601,660,616,660,true);
  DrawMaterialQuad("Wall-Concrete2",601,650,571,640,571,650,586,650,true);

  DrawMaterialQuad("Wall-Concrete2",690,630,720,620,720,630,705,630,true);
  DrawMaterialQuad("Wall-Concrete2",760,550,790,540,790,550,775,550,true);

  DrawMaterialQuad("Wall-Concrete2",830,660,860,650,860,660,845,660,true);
  DrawMaterialQuad("Wall-Concrete2",860,650,890,640,890,650,875,650,true);
  DrawMaterialQuad("Wall-Concrete2",800,670,830,660,830,670,815,670,true);
  DrawMaterialQuad("Wall-Concrete2",770,680,800,670,800,680,785,680,true);

  DrawMaterialQuad("Wall-Concrete2",1151,590,1121,580,1121,590,1136,590,true);
  DrawMaterialQuad("Wall-Concrete2",1181,600,1151,590,1151,600,1166,600,true);
  DrawMaterialQuad("Wall-Concrete2",1121,580,1091,570,1091,580,1106,580,true);
  DrawMaterialQuad("Wall-Concrete2",1091,570,1061,560,1061,570,1076,570,true);

  DrawMaterialQuad("Wall-Concrete2",1191,470,1161,460,1161,470,1176,470,true);
  DrawMaterialQuad("Wall-Concrete2",1261,551,1231,541,1231,551,1246,551,true);

  DrawMaterialQuad("Wall-Concrete2",1290,590,1320,580,1320,590,1305,590,true);
  DrawMaterialQuad("Wall-Concrete2",1260,600,1290,590,1290,600,1275,600,true);
  DrawMaterialQuad("Wall-Concrete2",1320,580,1350,570,1350,580,1335,580,true);
  DrawMaterialQuad("Wall-Concrete2",1350,570,1380,560,1380,570,1365,570,true);

  DrawMaterialQuad("Wall-Concrete2",1300,470,1330,460,1330,470,1315,470,true);
  DrawMaterialQuad("Wall-Concrete2",1391,470,1361,460,1361,470,1376,470,true);

  //Rohre
  CreateObject(PIPL, 0, 380, -1)->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 555, 620, -1)->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 930, 420, -1)->Right()->Right()->Right()->Right();
  CreateObject(PIPL, 1305, 540, -1)->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 1950, 320, -1)->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid();

  //Sprungschanzen
  CreateObject(JMPD, 905, 640, -1)->Set(85, -9);
  CreateObject(JMPD, 1040, 560, -1)->Set(91, 13);

  //Glasscheiben
  CreateObject(PANE, 315, 540, -1);
  CreateObject(PANE, 315, 570, -1);

  CreateObject(PANE, 778, 620, -1);
  CreateObject(PANE, 1172, 540, -1);

  CreateObject(PANE, 1630, 460, -1);
  CreateObject(PANE, 1630, 490, -1);

  //Autos
  var car = CreateObject(CCAR, 40, 260, -1);
  car->SetPhase(1);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  car = CreateObject(CCAR, 140, 260, -1);
  car->SetPhase(1);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  car = CreateObject(CCAR, 240, 260, -1);
  car->SetPhase(1);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  car = CreateObject(CCAR, 1690, 240, -1);
  car->SetPhase(0);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  car = CreateObject(CCAR, 1790, 240, -1);
  car->SetPhase(0);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  car = CreateObject(CCAR, 1890, 240, -1);
  car->SetPhase(0);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);

  //Kisten
  CreateObject(WCR2, 10, 622, -1);
  CreateObject(WCR2, 10, 640, -1);
  CreateObject(WCR2, 605, 502, -1);
  CreateObject(WCR2, 605, 520, -1);
  CreateObject(WCR2, 645, 430, -1)->AutoRespawn();
  CreateObject(WCR2, 1180, 290, -1)->AutoRespawn();
  CreateObject(WCR2, 1345, 422, -1);
  CreateObject(WCR2, 1345, 440, -1);

  //Metallkisten
  CreateObject(MWCR, 30, 640, -1);
  CreateObject(MWCR, 505, 640, -1)->AutoRespawn();
  CreateObject(MWCR, 1440, 560, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 1020, 700, -1)->AutoRespawn();
  CreateObject(BECR, 1940, 510, -1);

  //Explosive Kisten
  CreateObject(XWCR, 590, 430, -1)->AutoRespawn();
  CreateObject(XWCR, 1310, 350, -1)->AutoRespawn();
  CreateObject(XWCR, 1895, 460, -1);

  //Gerüste
  CreateObject(SFFG, 1920, 510, -1)->Set(3);
  CreateObject(SFFG, 1920, 560, -1)->Set(2);

  //Benzinfässer
  CreateObject(PBRL, 665, 660, -1)->AutoRespawn();
  CreateObject(PBRL, 700, 680, -1)->AutoRespawn();
  CreateObject(PBRL, 715, 680, -1)->AutoRespawn();
  CreateObject(PBRL, 1240, 600, -1)->AutoRespawn();
  CreateObject(PBRL, 1255, 600, -1)->AutoRespawn();
  CreateObject(PBRL, 1295, 587, -1)->AutoRespawn();
  CreateObject(PBRL, 1800, 560, -1);

  //Explosivfässer
  CreateObject(XBRL, 1150, 290, -1)->AutoRespawn();
  CreateObject(XBRL, 1160, 290, -1)->AutoRespawn();

  //Phosphorfass
  CreateObject(HBRL, 920, 700, -1)->AutoRespawn();

  //Explosivtanks
  CreateObject(XTNK, 715, 430, -1)->AutoRespawn();
  CreateObject(XTNK, 1390, 350, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 180, 640, -1);
  CreateObject(GSBL, 240, 640, -1);
  CreateObject(GSBL, 660, 430, -1)->AutoRespawn();
  CreateObject(GSBL, 1040, 700, -1)->AutoRespawn();
  CreateObject(GSBL, 1420, 560, -1)->AutoRespawn();

  //Steine
  CreateObject(STNE, 345, 255, -1);
  CreateObject(STNE, 1400, 810, -1);
  CreateObject(STNE, 1590, 240, -1);

  //Tore und Konsolen
  var autod = CreateObject(HNG3, 810, 540, -1);
  autod->Open();
  CreateObject(CONS, 670, 540, -1)->Set(autod);
  var autod = CreateObject(HNG3, 1140, 460, -1);
  autod->Open();
  CreateObject(CONS, 1280, 460, -1)->Set(autod);

  //Verbundene Räume
  aDoorWay[00] = CreateObject(GAT1, 976, 510, -1);
  aDoorWay[01] = CreateObject(ROOM, 1095, 700, -1);
  aDoorWay[00]->Connect(aDoorWay[01]);

  //Grenzen
  CreateObject(BRDR, 260, 0, -1)->Set(0);
  CreateObject(BRDR, 1695, 0, -1)->Set(1);

  //Sounds

  //Wind
  CreateObject(SE4K, 785, 350, -1)->Set("WindSound*.ogg",775,250);

  //Rush Hour
  CreateObject(SE4K, 220, 260, -1)->Set("Traffic*.ogg",245,70);
  CreateObject(SE4K, 1710, 240, -1)->Set("Traffic*.ogg",245,70);

  //Hallen
  CreateObject(SE4K, 690, 590, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1260, 510, -1)->Set("Interior*.ogg",665,105);

  //Lautsprecher
  CreateObject(SE4K, 980, 450, -1)->Set("Announce*.ogg",3000,200);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 275, 550, -1);
  crate->Set(ABOX);
  var crate = CreateObject(AMCT, 1675, 460, -1);
  crate->Set(ABOX);

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 970, 690);

  //Raketen
  PlaceSpawnpoint(MBOX, 960, 575);

  //Revolver
  var wpn = CreateObject(RVLR, 1120, 688, -1);
  wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));
  wpn = CreateObject(RVLR, 1135, 688, -1);
  wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));
  wpn = CreateObject(RVLR, 1150, 688, -1);
  wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Radios
  CreateObject(RDIO, 30, 622, -1);
  CreateObject(RDIO, 1005, 700, -1);

  //Leitkegel
  CreateObject(TCCN, 610, 650, -1)->Light();
  CreateObject(TCCN, 640, 660, -1)->Light();
  CreateObject(TCCN, 825, 540, -1);
  CreateObject(TCCN, 1125, 460, -1);
  CreateObject(TCCN, 1310, 580, -1)->Light();
  CreateObject(TCCN, 1340, 570, -1)->Light();

  //Glastisch
  CreateObject(GTBL, 1135, 700, -1);

  //Labortische
  CreateObject(LTBL, 385, 640, -1);
  CreateObject(LTBL, 1570, 560, -1);

  //Monitore
  CreateObject(MNI2, 375, 628, -1);
  CreateObject(MNI2, 395, 628, -1);
  CreateObject(MNI2, 1130, 688, -1);
  CreateObject(MNI2, 1560, 548, -1);
  CreateObject(MNI2, 1580, 548, -1);

  //Apparaturen
  CreateObject(GADG, 410, 640, -1);
  CreateObject(GADG, 760, 620, -1);
  CreateObject(GADG, 1075, 700, -1)->Set(1);
  CreateObject(GADG, 1190, 540, -1);
  CreateObject(GADG, 1540, 560, -1);

  //Papierstapel
  CreateObject(PPSK, 315,640, -1);
  CreateObject(PPSK, 1145, 687, -1);
  CreateObject(PPSK, 1640, 560, -1);

  //Straßenlichter
  CreateObject(SLGH, 640, 430, -1);
  CreateObject(SLGH, 1260, 350, -1);

  //Notausgangslichter
  CreateObject(ETLT, 220, 625, -1);
  CreateObject(ETLT, 1095, 660, -1);
  CreateObject(ETLT, 1720, 545, -1);

  //Verkehrsampeln
  CreateObject(TLGH, 290, 260, -1);
  CreateObject(TLGH, 1660, 240, -1);

  //Schilder
  CreateObject(WLSN, 640, 650, -1);
  CreateObject(MSGN, 800, 670, -1);
  CreateObject(MSGN, 830, 660, -1);
  CreateObject(MSGN, 860, 650, -1);
  CreateObject(ESGN, 1005, 635, -1);
  CreateObject(MSGN, 1090, 570, -1);
  CreateObject(MSGN, 1120, 580, -1);
  CreateObject(MSGN, 1150, 590, -1);
  CreateObject(WLSN, 1325, 570, -1);

  //Alarmlampen
  CreateObject(ALGH, 910, 370, -1)->TurnOn();
  CreateObject(ALGH, 1040, 290, -1)->TurnOn();

  //Radare
  CreateObject(RADR, 775, 370, -1)->SetPhase(10);
  CreateObject(RADR, 1125, 290, -1);

  //Zäune
  CreateObject(FENC, 610, 430, -1);
  CreateObject(FENC, 670, 430, -1);
  CreateObject(FENC, 1230, 350, -1);
  CreateObject(FENC, 1290, 350, -1);

  //Regale
  CreateObject(FRAM, 100, 640, -1);
  CreateObject(FRAM, 120, 640, -1);
  CreateObject(FRAM, 140, 640, -1);
  CreateObject(FRAM, 160, 640, -1);
  CreateObject(FRAM, 200, 640, -1);
  CreateObject(FRAM, 220, 640, -1);
  CreateObject(FRAM, 940, 700, -1);
  CreateObject(FRAM, 960, 700, -1);
  CreateObject(FRAM, 980, 700, -1);
  CreateObject(FRAM, 1720, 560, -1);
  CreateObject(FRAM, 1740, 560, -1);
  CreateObject(FRAM, 1760, 560, -1);
  CreateObject(FRAM, 1780, 560, -1);
  CreateObject(FRAM, 1820, 560, -1);
  CreateObject(FRAM, 1840, 560, -1);

  //Scheinwerfer
  CreateObject(FLGH, 565, 410, -1)->SetRotation(-30);
  CreateObject(FLGH, 815, 350, -1)->SetRotation(40);
  CreateObject(FLGH, 1335, 330, -1)->SetRotation(50);

  //Glühbirnen
  CreateObject(LBGH, 40, 510, -1);
  CreateObject(LBGH, 1225, 390, -1);
  CreateObject(LBGH, 720, 470, -1);
  CreateObject(LBGH, 1910, 430, -1);

  //Wandlampen
  CreateObject(BLGH, 385, 570, -1);
  CreateObject(BLGH, 1565, 470, -1);

  //Spinde
  CreateObject(LCKR, 330, 640, -1);
  CreateObject(LCKR, 350, 640, -1);
  CreateObject(LCKR, 1605, 560, -1);
  CreateObject(LCKR, 1625, 560, -1);

  //Topfpflanzen
  CreateObject(PLT3, 470, 640, -1);
  CreateObject(PLT3, 1480, 560, -1);

  //Geländer
  CreateObject(RAI1, -2, 260, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1620, 240, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);

  //Lüftungsgitter
  CreateObject(ENGT, 720, 560, -1);
  CreateObject(ENGT, 1230, 480, -1);

  //Feuerlöscher
  CreateObject(FIE2, 540, 635, -1);
  CreateObject(FIE2, 990, 635, -1);
  CreateObject(FIE2, 1410, 555, -1);

  //Poster
  CreateObject(PSTR, 960, 637, -1)->Set(3);

  //Bildschirme
  var screen = CreateObject(SCR3, 300, 600, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR3, 470, 600, -1);
  screen->SetClrModulation(RGB(255,255,255));
  CreateObject(SCA2, 913, 525, -1)->SetClips([13]);
  CreateObject(SCA2, 1038, 525, -1)->SetClips([12]);
  screen = CreateObject(SCR3, 1480, 520, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR3, 1650, 520, -1);
  screen->SetClrModulation(RGB(255,255,255));

  //Büsche
  CreateObject(BSH2, 400, 390, -1);
  CreateObject(BSH2, 460, 285, -1);
  CreateObject(BSH2, 945, 400, -1);
  CreateObject(BSH2, 1500, 320, -1);

  //Ventillatoren
  CreateObject(VENT, 550, 535, -1)->SetCon(40);
  CreateObject(VENT, 1400, 460, -1)->SetCon(40);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD08,600,730,-1)->SetClrModulation(RGB(125,125,125));
  CreateObject(BD08,1500,730,-1)->SetClrModulation(RGB(125,125,125));
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Grenade.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Toward the Flag.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Rock Go On.ogg");

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Flaggenposten
    var flag = CreateObject(OFPL, 995,560, -1);
    flag->~Set("$Flag1$");

    //Objekte entfernen
    RemoveAll(JPTP);
    RemoveAll(JMPD);

    //Zusätzliche Munition
    if(!FindObject(NOAM))
    {
      //Versorgungskiste (Kugeln)
      var crate = CreateObject(AMCT, 960, 640, -1);
      crate->Set(ABOX);
    }
  }

  //MR-Spielziel
  if(FindObject(GMNR))
  {
    //Geldsäcke
    AddMoneySpawn(810, 440, [20]);
    AddMoneySpawn(980, 250, [20]);
    AddMoneySpawn(1010, 630, [20]);
    AddMoneySpawn(1140, 360, [20]);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 985; iY = 550;
    return 1;
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Gesonderte Spawnpoints bei 2 Teams
    if(GetActiveTeamCount() == 2)
    {
      if(iTeam == GetActiveTeamByIndex(0))
        return [[420, 400], [420, 630], [450, 340]];
      if(iTeam == GetActiveTeamByIndex(1))
        return [[1465, 285], [1495, 320], [1530, 550]];
    }
    return 1;
  }
  else
    return [[540, 420], [540, 540], [540, 630], [1410, 340], [1410, 460], [1410, 550]];

  //MR/LMS/DM-Spielziel
  if(FindObject(GMNR) || FindObject(GLMS) || FindObject(GTDM))
  {
    //Gesonderte Spawnpoints bei 2 Teams
    if(GetActiveTeamCount() == 2)
    {
      if(iTeam == GetActiveTeamByIndex(0))
        return [[420, 400], [420, 630], [450, 340]];
      if(iTeam == GetActiveTeamByIndex(1))
        return [[1465, 285], [1495, 320], [1530, 550]];
      return 1;
    }
    else
      return [[540, 540], [550, 630], [730, 610], [1220, 530], [1420, 460], [1400, 550]];
  }
}