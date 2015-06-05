/*-- Firewood --*/

#strict 2
#include CSTD

static aFlag;

func RecommendedGoals()	{return [GOCC, GCTF];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(0,15,15);
  //Bildschirmfärbung
  SetGamma(RGB(2,3,10),RGB(131,135,158),RGB(252,253,255));
  //Flaggen
  aFlag = [];
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

  //Rampen
  DrawMaterialQuad("Ashes-SWIRL", 1200, 618, 1240, 618, 1200, 620, 1240, 620, 1);
  DrawMaterialQuad("Wall-Unknown1",1390,491,1390,511,1440,491,1390,491,true);
  DrawMaterialQuad("Wall-Unknown1",1450,520,1450,500,1400,520,1450,520,true);
  DrawMaterialQuad("Wall-Unknown1",1831,520,1831,500,1881,520,1831,520,true);
  DrawMaterialQuad("Wall-Unknown1",1890,491,1890,511,1840,491,1890,491,true);
  DrawMaterialQuad("Ashes-SWIRL", 2030, 618, 2070, 618, 2030, 620, 2070, 620, 1);

  //Leitern
  CreateObject(LADR, 160, 490, -1)->Set(12, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 530, 388, -1)->Set(10);
  CreateObject(LADR, 600, 298, -1)->Set(15);
  CreateObject(LADR, 800, 582, -1)->Set(23);

  CreateObject(LADR, 1170, 510, -1)->Set(12, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 1485, 360, -1)->Set(9, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");

  CreateObject(LADR, 1590, 208, -1)->Set(10);
  CreateObject(LADR, 1640, 410, -1)->Set(24);
  CreateObject(LADR, 1640, 576, -1)->Set(6);
  CreateObject(LADR, 1690, 208, -1)->Set(10);

  CreateObject(LADR, 1795, 360, -1)->Set(9, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 2110, 510, -1)->Set(12, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");

  CreateObject(LADR, 2480, 582, -1)->Set(23);
  CreateObject(LADR, 2680, 298, -1)->Set(15);
  CreateObject(LADR, 2750, 388, -1)->Set(10);
  CreateObject(LADR, 3120, 490, -1)->Set(12, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");

  //Bodenluken
  CreateObject(HA4K, 530, 303, -1);
  CreateObject(HA4K, 600, 173, -1);
  CreateObject(HA4K, 600, 243, -1);
  CreateObject(HA4K, 800, 393, -1);
  CreateObject(HA4K, 1590, 123, -1);
  CreateObject(HA4K, 1640, 213, -1);
  CreateObject(HA4K, 1640, 523, -1);
  CreateObject(HA4K, 1690, 123, -1);
  CreateObject(HA4K, 2480, 393, -1);
  CreateObject(HA4K, 2680, 173, -1);
  CreateObject(HA4K, 2680, 243, -1);
  CreateObject(HA4K, 2750, 303, -1);

  //Sandsackbarrieren
  CreateObject(SBBA, 490, 300, -1)->Right();
  CreateObject(SBBA, 440, 300, -1);
  CreateObject(SBBA, 827, 390, -1)->Right();
  CreateObject(SBBA, 1665, 100, -1)->Right();
  CreateObject(SBBA, 1615, 100, -1);
  CreateObject(SBBA, 2453, 390, -1);
  CreateObject(SBBA, 2830, 300, -1)->Right();
  CreateObject(SBBA, 2785, 300, -1);

  //Glasscheiben
  CreateObject(PANE, 572, 211, -1);
  CreateObject(PANE, 572, 240, -1);

  CreateObject(PANE, 627, 211, -1);
  CreateObject(PANE, 627, 240, -1);

  CreateObject(PANE, 2652, 211, -1);
  CreateObject(PANE, 2652, 240, -1);

  CreateObject(PANE, 2707, 211, -1);
  CreateObject(PANE, 2707, 240, -1);

  //Steine
  CreateObject(STNE, 300, 550, -1);
  CreateObject(STNE, 2980, 550, -1);

  //Schutztüren
  CreateObject(GDDR, 405, 390, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 675, 390, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1485, 210, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 1795, 210, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 2605, 390, -1);
  CreateObject(GDDR, 2875, 390, -1);

  //Metallkisten
  CreateObject(MWCR, 710, 390, -1);
  CreateObject(MWCR, 2570, 390, -1);

  //Verbandskisten
  CreateObject(BECR, 1570, 120, -1)->AutoRespawn();
  CreateObject(BECR, 1600, 520, -1)->AutoRespawn();
  CreateObject(BECR, 1680, 520, -1)->AutoRespawn();
  CreateObject(BECR, 1710, 120, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 580, 170, -1)->AutoRespawn();
  CreateObject(PBRL, 2660, 170, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 730, 390, -1)->AutoRespawn();
  CreateObject(XBRL, 745, 390, -1)->AutoRespawn();
  CreateObject(XBRL, 1540, 510, -1)->AutoRespawn();
  CreateObject(XBRL, 1740, 510, -1)->AutoRespawn();
  CreateObject(XBRL, 2530, 390, -1)->AutoRespawn();
  CreateObject(XBRL, 2545, 390, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1175, 525, -1)->AutoRespawn();
  CreateObject(GSBL, 2105, 525, -1)->AutoRespawn();

  //Explosivtanks
  CreateObject(XTNK, 750, 390, -1)->AutoRespawn();
  CreateObject(XTNK, 1555, 510, -1)->AutoRespawn();
  CreateObject(XTNK, 1720, 510, -1)->AutoRespawn();
  CreateObject(XTNK, 2525, 390, -1)->AutoRespawn();

  //Stahlbrücken
  CreateObject(_HBR, 865, 402, -1)->SwitchMode();
  CreateObject(_HBR, 2415, 402, -1)->SwitchMode();

  //Piranha
  CreateObject(PIRA, 880, 680, -1)->AutoRespawn();
  CreateObject(PIRA, 1060, 650, -1)->AutoRespawn();
  CreateObject(PIRA, 1595, 670, -1)->AutoRespawn();
  CreateObject(PIRA, 2180, 655, -1)->AutoRespawn();
  CreateObject(PIRA, 2405, 680, -1)->AutoRespawn();

  //Sounds

  //Magie
  CreateObject(SE4K, 1125, 250, -1)->Set("MagicElementEarth",4500,500);
  CreateObject(SE4K, 2065, 250, -1)->Set("MagicElementEarth",4500,500);

  //Möven
  CreateObject(SE4K, 60, 205, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 3215, 205, -1)->Set("SeaSounds*.ogg",140,35);

  //Vögel
  CreateObject(SE4K, 320, 480, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 1225, 470, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 1395, 310, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 1940, 310, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 2050, 470, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 2960, 480, -1)->Set("BirdSong*",140,35);

  //Jungel
  CreateObject(SE4K, 270, 460, -1)->Set("Jungle*.ogg",300,50);
  CreateObject(SE4K, 1005, 320, -1)->Set("Jungle*.ogg",300,50);
  CreateObject(SE4K, 1425, 190, -1)->Set("Jungle*.ogg",300,50);
  CreateObject(SE4K, 1850, 190, -1)->Set("Jungle*.ogg",300,50);
  CreateObject(SE4K, 2270, 320, -1)->Set("Jungle*.ogg",300,50);
  CreateObject(SE4K, 3000, 460, -1)->Set("Jungle*.ogg",300,50);

  //Wellen
  CreateObject(SE4K, 880, 600, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 2400, 600, -1)->Set("Wave*.ogg",105,35);

  //Wind
  CreateObject(SE4K, 1065, 290, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 2215, 290, -1)->Set("WindSound*.ogg",3000,105);

  //Erdrutsche
  CreateObject(SE4K, 985, 520, -1)->Set("FallingDirt*.ogg",2500,250);
  CreateObject(SE4K, 2300, 520, -1)->Set("FallingDirt*.ogg",2500,250);

  //Lautsprecher
  CreateObject(SE4K, 465, 220, -1)->Set("Announce*.ogg",3000,200);
  CreateObject(SE4K, 1640, 20, -1)->Set("Announce*.ogg",3500,200);
  CreateObject(SE4K, 2810, 220, -1)->Set("Announce*.ogg",3000,200);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 555, 300, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 1615, 210, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 2725, 300, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Handgranaten)
  crate = CreateObject(AMCT, 1340, 520, -1);
  crate->Set(FRAG);
  crate = CreateObject(AMCT, 1945, 520, -1);
  crate->Set(FRAG);

  //Raketen
  PlaceSpawnpoint(MBOX, 580, 235);
  PlaceSpawnpoint(MBOX, 2700, 235);

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 1640, 575);

  //Patrouillenboote
  SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,675,609,-1),50*21);
  SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,1640,571,-1),50*21);
  SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2595,609,-1),50*21);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Verbrannte Zaubertürme
  CreateObject(WTRB, 1120, 290, -1)->SetR(-25);
  CreateObject(WTRB, 2065, 250, -1);

  //Ventillatoren
  CreateObject(VENT, 580, 290, -1)->SetCon(15);
  CreateObject(VENT, 2700, 290, -1)->SetCon(15);

  //Schilder
  CreateObject(ESGN, 460, 385, -1);
  CreateObject(MSGN, 915, 390, -1);
  CreateObject(MSGN, 1485, 120, -1);
  CreateObject(MSGN, 1795, 120, -1);
  CreateObject(MSGN, 2365, 390, -1);
  CreateObject(ESGN, 2820, 385, -1);

  //Bojen
  CreateObject(BUOY, 725, 633, -1);

  CreateObject(BUOY, 1590, 633, -1);
  CreateObject(BUOY, 1690, 633, -1);

  CreateObject(BUOY, 2540, 633, -1);

  //Regale
  CreateObject(FRAM, 565, 380, -1);
  CreateObject(FRAM, 590, 380, -1);
  CreateObject(FRAM, 615, 380, -1);

  CreateObject(FRAM, 1505, 200, -1);
  CreateObject(FRAM, 1530, 200, -1);
  CreateObject(FRAM, 1555, 200, -1);
  CreateObject(FRAM, 1720, 200, -1);
  CreateObject(FRAM, 1745, 200, -1);
  CreateObject(FRAM, 1770, 200, -1);

  CreateObject(FRAM, 2650, 380, -1);
  CreateObject(FRAM, 2675, 380, -1);
  CreateObject(FRAM, 2705, 380, -1);

  //Orientierungslichter
  CreateObject(OLGH, 600, 120, -1)->Set(1, 20, 1, 1, 60);
  CreateObject(OLGH, 2680, 120, -1)->Set(1, 20, 1, 1, 60);

  //Satellitenschüsseln
  CreateObject(SADH, 350, 310, -1);
  CreateObject(SADH, 2930, 310, -1);

  //Notausgangslichter
  CreateObject(ETLT, 660, 375, -1);
  CreateObject(ETLT, 1640, 195, -1);
  CreateObject(ETLT, 2620, 375, -1);

  //Apparaturen
  CreateObject(GADG, 540, 390, -1)->Set(3);
  CreateObject(GADG, 1570, 210, -1)->Set(1);
  CreateObject(GADG, 1710, 210, -1)->Set(1);
  CreateObject(GADG, 2740, 390, -1)->Set(3);

  //Flaschen
  CreateObject(BOTL, 1560, 210, -1);
  CreateObject(BOTL, 1720, 210, -1);

  //Geländer
  CreateObject(RAI1, 400, 390, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1482, 210, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 2600, 390, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);

  //Lianen
  SetCon(100, CreateObject(VINE, 120, 280, -1));
  SetCon(90, CreateObject(VINE, 315, 440, -1));
  SetCon(100, CreateObject(VINE, 540, 480, -1));
  SetCon(80, CreateObject(VINE, 580, 490, -1));
  SetCon(100, CreateObject(VINE, 1200, 340, -1));
  SetCon(70, CreateObject(VINE, 1235, 340, -1));
  SetCon(80, CreateObject(VINE, 1305, 360, -1));
  SetCon(90, CreateObject(VINE, 1445, 170, -1));
  SetCon(100, CreateObject(VINE, 1685, 310, -1));
  SetCon(60, CreateObject(VINE, 1910, 210, -1));
  SetCon(90, CreateObject(VINE, 1930, 380, -1));
  SetCon(50, CreateObject(VINE, 1935, 380, -1));
  SetCon(40, CreateObject(VINE, 1965, 360, -1));
  SetCon(60, CreateObject(VINE, 1970, 360, -1));
  SetCon(80, CreateObject(VINE, 2045, 480, -1));
  SetCon(40, CreateObject(VINE, 2260, 360, -1));
  SetCon(70, CreateObject(VINE, 2685, 490, -1));
  SetCon(50, CreateObject(VINE, 2695, 490, -1));
  SetCon(100, CreateObject(VINE, 2870, 470, -1));
  SetCon(70, CreateObject(VINE, 2970, 440, -1));
  SetCon(100, CreateObject(VINE, 2995, 440, -1));
  SetCon(80, CreateObject(VINE, 3000, 440, -1));
  SetCon(100, CreateObject(VINE, 3200, 350, -1));
  SetCon(80, CreateObject(VINE, 3230, 370, -1));

  //Büsche
  CreateObject(BSH2, 315, 460, -1);
  CreateObject(BSH2, 1095, 520, -1);
  CreateObject(BSH2, 1135, 520, -1);
  CreateObject(BSH2, 1235, 360, -1);
  CreateObject(BSH2, 1270, 110, -1);
  CreateObject(BSH2, 1930, 400, -1);
  CreateObject(BSH2, 2010, 110, -1);
  CreateObject(BSH2, 2145, 520, -1);
  CreateObject(BSH2, 2185, 520, -1);
  CreateObject(BSH2, 2700, 510, -1);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Getaway.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,465,300,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(190,380);
   aFlag[0] -> AddSpawnPoint(430,380);
   aFlag[0] -> AddSpawnPoint(660,380);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,1115,510,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(945,450);
   aFlag[1] -> AddSpawnPoint(1035,400);
   aFlag[1] -> AddSpawnPoint(1265,560);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,1639,100,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(1425,250);
   aFlag[2] -> AddSpawnPoint(1665,200);
   aFlag[2] -> AddSpawnPoint(1855,250);
   if(aTeams[3] == true)
   {
    aFlag[2]->Set("$Flag3$",100,2);
    aFlag[2]->Capture(3,1);
   }
   else
   {
    aFlag[2]->Set("$Flag3$",0,2);
   }

   aFlag[3] = CreateObject(OFPL,1641,520,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(1465,370);
   aFlag[3] -> AddSpawnPoint(1580,390);
   aFlag[3] -> AddSpawnPoint(1700,390);
   aFlag[3] -> AddSpawnPoint(1815,370);
   if(aTeams[4] == true)
   {
    aFlag[3]->Set("$Flag4$",100,2);
    aFlag[3]->Capture(4,1);
   }
   else
   {
    aFlag[3]->Set("$Flag4$",0,2);
   }

   aFlag[4] = CreateObject(OFPL,2165,510,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(2245,400);
   aFlag[4] -> AddSpawnPoint(2355,460);
   aFlag[4] -> AddSpawnPoint(2015,560);
   aFlag[4]->Set("$Flag5$",0,2);

   aFlag[5] = CreateObject(OFPL,2810,300,NO_OWNER);
   aFlag[5] -> AddSpawnPoint(2620,380);
   aFlag[5] -> AddSpawnPoint(2850,380);
   aFlag[5] -> AddSpawnPoint(3090,380);
   if(aTeams[2] == true)
   {
    aFlag[5]->Set("$Flag6$",100,2);
    aFlag[5]->Capture(2,1);
   }
   else
   {
    aFlag[5]->Set("$Flag6$",0,2);
   }
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,465,300,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,2810,300,GetTeamColor(2));}
   if(aTeams[3] == true)
   {CreateFlag(3,1640,100,GetTeamColor(3));}
   if(aTeams[4] == true)
   {CreateFlag(4,1640,580,GetTeamColor(4));}

   //Objekt entfernen
   RemoveObject(FindObject2(Find_ID(SPNP),Find_InRect(1620, 550, 40, 30)));
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 1640; iY = 210;
   return 1;
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(iTeam == 1)
   {
    return [[90, 440], [225, 530]];
   }
   if(iTeam == 2)
   {
    return [[3055, 530], [3190, 440]];
   }
   if(iTeam == 3)
   {
    return [[1535, 200], [1745, 200]];
   }
   if(iTeam == 4)
   {
    return [[1460, 370], [1820, 370]];
   }
   return 1;
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    return [[370, 300], [450, 380]];
   }
   if(iTeam == 2)
   {
    return [[2830, 380], [2900, 300]];
   }
   if(iTeam == 3)
   {
    return [[1490, 110], [1790, 110]];
   }
   if(iTeam == 4)
   {
    return [[1555, 400], [1725, 400]];
   }
   return 1;
  }
  
  return _inherited(iX, iY, iTeam, ...);
}