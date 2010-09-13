/*-- Firewood --*/

#strict
#include CSTD

static aFlag;


/* Initalisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg");
  Music("CMC_Getaway.ogg");
  //Bildschirmfärbung
  SetGamma(RGB(2,3,10), RGB(131,135,158), RGB(252,253,255) );
  //Flaggen
  aFlag = [];
  //Szenario einrichten
  CreateFurniture();
  //Ausrüstung plazieren
  CreateEquipment();
  return(1); 
}

/* Plazierungslisten */

func CreateFurniture()
{
  Log("$CreatingFurniture$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Leitern
  CreateObject(LADR, 160, 490, -1)->Set(12);
  CreateObject(LADR, 530, 388, -1)->Set(10);
  CreateObject(LADR, 600, 298, -1)->Set(15);

  CreateObject(LADR, 1170, 510, -1)->Set(12);
  CreateObject(LADR, 1485, 360, -1)->Set(9);

  CreateObject(LADR, 1590, 208, -1)->Set(10);
  CreateObject(LADR, 1640, 410, -1)->Set(24);
  CreateObject(LADR, 1640, 600, -1)->Set(9);
  CreateObject(LADR, 1690, 208, -1)->Set(10);

  CreateObject(LADR, 1795, 360, -1)->Set(9);
  CreateObject(LADR, 2110, 510, -1)->Set(12);

  CreateObject(LADR, 2680, 298, -1)->Set(15);
  CreateObject(LADR, 2750, 388, -1)->Set(10);
  CreateObject(LADR, 3120, 490, -1)->Set(12);

  //Bodenlucken
  CreateObject(HA4K, 530, 303, -1);
  CreateObject(HA4K, 600, 173, -1);
  CreateObject(HA4K, 600, 243, -1);
  CreateObject(HA4K, 1590, 123, -1);
  CreateObject(HA4K, 1640, 213, -1);
  CreateObject(HA4K, 1640, 523, -1);
  CreateObject(HA4K, 1690, 123, -1);
  CreateObject(HA4K, 2680, 173, -1);
  CreateObject(HA4K, 2680, 243, -1);
  CreateObject(HA4K, 2750, 303, -1);

  //Orientierungslichter
  CreateObject(OLGH, 600, 120, -1)->Set(1, 20, 1, 1, 60);
  CreateObject(OLGH, 2680, 120, -1)->Set(1, 20, 1, 1, 60);

  //Sandsackbarrieren
  CreateObject(SBBA, 490, 300, -1)->Right();
  CreateObject(SBBA, 440, 300, -1);
  CreateObject(SBBA, 815, 390, -1)->Right();
  CreateObject(SBBA, 1665, 100, -1)->Right();
  CreateObject(SBBA, 1615, 100, -1);
  CreateObject(SBBA, 2465, 390, -1);
  CreateObject(SBBA, 2830, 300, -1)->Right();
  CreateObject(SBBA, 2785, 300, -1);

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

  //Schilder
  CreateObject(ESGN, 460, 385, -1);
  CreateObject(ESGN, 2820, 385, -1);

  //Geländer
  CreateObject(RAI1, 400, 390, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1482, 210, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 2600, 390, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);

  //Glasscheiben
  CreateObject(_WIN, 572, 211, -1);
  CreateObject(_WIN, 572, 240, -1);

  CreateObject(_WIN, 627, 211, -1);
  CreateObject(_WIN, 627, 240, -1);

  CreateObject(_WIN, 2652, 211, -1);
  CreateObject(_WIN, 2652, 240, -1);

  CreateObject(_WIN, 2707, 211, -1);
  CreateObject(_WIN, 2707, 240, -1);

  //Büsche
  CreateObject(BSH2, 315, 460, -1);
  CreateObject(BSH2, 1235, 360, -1);
  CreateObject(BSH2, 1930, 400, -1);
  CreateObject(BSH2, 2700, 510, -1);

  //Steine
  CreateObject(STNE, 300, 550, -1);
  CreateObject(STNE, 2980, 550, -1);

  //Ventillatoren
  CreateObject(VENT, 580, 290, -1)->SetCon(15);
  CreateObject(VENT, 2700, 290, -1)->SetCon(15);

  //Markierungsschilder
  CreateObject(MSGN, 915, 390, -1);
  CreateObject(MSGN, 2375, 390, -1);

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

  //Stahltüren
  CreateObject(STDR, 405, 390, -1);
  CreateObject(STDR, 675, 390, -1);
  CreateObject(STDR, 1485, 210, -1);
  CreateObject(STDR, 1795, 210, -1);
  CreateObject(STDR, 2605, 390, -1);
  CreateObject(STDR, 2875, 390, -1);

  //Metallkisten
  CreateObject(MWCR, 720, 390, -1);
  CreateObject(MWCR, 2560, 390, -1);

  //Kisten
  CreateObject(WCR2, 1570, 120, -1);
  CreateObject(WCR2, 1600, 520, -1)->AutoRespawn();
  CreateObject(WCR2, 1680, 520, -1)->AutoRespawn();
  CreateObject(WCR2, 1710, 120, -1);

  //Benzinfässer
  CreateObject(PBRL, 580, 170, -1)->AutoRespawn();
  CreateObject(PBRL, 2660, 170, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 740, 390, -1)->AutoRespawn();
  CreateObject(XBRL, 755, 390, -1)->AutoRespawn();
  CreateObject(XBRL, 1540, 510, -1)->AutoRespawn();
  CreateObject(XBRL, 1740, 510, -1)->AutoRespawn();
  CreateObject(XBRL, 2520, 390, -1)->AutoRespawn();
  CreateObject(XBRL, 2535, 390, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1175, 525, -1)->AutoRespawn();
  CreateObject(GSBL, 2105, 525, -1)->AutoRespawn();

  //Explosivtanks
  CreateObject(XTNK, 765, 390, -1)->AutoRespawn();
  CreateObject(XTNK, 1555, 510, -1)->AutoRespawn();
  CreateObject(XTNK, 1720, 510, -1)->AutoRespawn();
  CreateObject(XTNK, 2505, 390, -1)->AutoRespawn();

  //Stahlbrücken
  CreateObject(_HBR, 865, 402, -1);
  CreateObject(_HBR, 2415, 402, -1);

  //Sounds

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
  CreateObject(SE4K, 465, 220, -1)->Set("Announce*.ogg",1600,150);
  CreateObject(SE4K, 1640, 20, -1)->Set("Announce*.ogg",1600,140);
  CreateObject(SE4K, 2810, 220, -1)->Set("Announce*.ogg",1600,150);
}

func CreateEquipment()
{
  var tmp;
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Munitionskisten (Kugeln)
  tmp = CreateObject(AMCT, 555, 300, -1);
  tmp->Set(ABOX);
  tmp = CreateObject(AMCT, 2725, 300, -1);
  tmp->Set(ABOX);

  //Munitionskisten (Handgranaten)
  tmp = CreateObject(AMCT, 1340, 520, -1);
  tmp->Set(FRAG);
  tmp = CreateObject(AMCT, 1945, 520, -1);
  tmp->Set(FRAG);

  //Munitionskiste (Raketen)
  tmp = CreateObject(AMCT, 1615, 210, -1);
  tmp->Set(MIAP);

  //Motorboote
  SetupVehicleSpawn([INFL],DIR_Right,CreateObject(VSPW,670,620,-1),50*21,300);
  SetupVehicleSpawn([INFL],DIR_Right,CreateObject(VSPW,1640,620,-1),50*21,300);
  SetupVehicleSpawn([INFL],DIR_Left,CreateObject(VSPW,2590,620,-1),50*21,300);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(aTeams[1] == true)
   {CreateFlag(1,465,300,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,2810,300,GetTeamColor(2));}
   if(aTeams[3] == true)
   {CreateFlag(3,1640,100,GetTeamColor(3));}
   if(aTeams[4] == true)
   {CreateFlag(4,1660,520,GetTeamColor(4));}
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggen
   aFlag[0] = CreateObject(OFPL,465,300,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(190,380);
   aFlag[0] -> AddSpawnPoint(430,380);
   aFlag[0] -> AddSpawnPoint(690,380);
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
   aFlag[1] -> AddSpawnPoint(1050,400);
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
   aFlag[4] -> AddSpawnPoint(2235,400);
   aFlag[4] -> AddSpawnPoint(2355,460);
   aFlag[4] -> AddSpawnPoint(2015,560);
   aFlag[4]->Set("$Flag5$",0,2);

   aFlag[5] = CreateObject(OFPL,2810,300,NO_OWNER);
   aFlag[5] -> AddSpawnPoint(2585,380);
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
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //DM/LMS-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS))
  {
   if(iTeam == 1)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 370; iY = 300; }
    if(!--rand)
     { iX = 450; iY = 380; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 2830; iY = 380; }
    if(!--rand)
     { iX = 2900; iY = 300; }
   }
   if(iTeam == 3)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 1490; iY = 110; }
    if(!--rand)
     { iX = 1790; iY = 110; }
   }
   if(iTeam == 4)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 1555; iY = 400; }
    if(!--rand)
     { iX = 1725; iY = 400; }
   }
   return(1);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(iTeam == 1)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 90; iY = 440; }
    if(!--rand)
     { iX = 225; iY = 530; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 3055; iY = 530; }
    if(!--rand)
     { iX = 3190; iY = 440; }
   }
   if(iTeam == 3)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 1535; iY = 200; }
    if(!--rand)
     { iX = 1745; iY = 200; }
   }
   if(iTeam == 4)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 1460; iY = 370; }
    if(!--rand)
     { iX = 1820; iY = 370; }
   }
   return(1);
  }

  //Startsicht
  iX = 1640; iY = 210;
}