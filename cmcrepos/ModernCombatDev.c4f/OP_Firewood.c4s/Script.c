/*-- Firewood --*/

#strict
#include CSTD

static aFlag;


/* Regelvoreinstellung */

func ChooserRuleConfig()
{
  return [NOFF,WPST,NODR,MCSL];
}

/* Initalisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_*.ogg");
  Music("CMC_Cave Voyage.ogg");
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
  CreateObject(LADR, 530, 390, -1)->Set(10);
  CreateObject(LADR, 600, 300, -1)->Set(15);
  CreateObject(LADR, 1170, 510, -1)->Set(12);
  CreateObject(LADR, 1485, 360, -1)->Set(9);
  CreateObject(LADR, 1590, 210, -1)->Set(10);
  CreateObject(LADR, 1640, 410, -1)->Set(24);
  CreateObject(LADR, 1640, 600, -1)->Set(9);
  CreateObject(LADR, 1690, 210, -1)->Set(10);
  CreateObject(LADR, 1795, 360, -1)->Set(9);
  CreateObject(LADR, 2110, 510, -1)->Set(12);
  CreateObject(LADR, 2680, 300, -1)->Set(15);
  CreateObject(LADR, 2750, 390, -1)->Set(10);
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

  //Explosivtanks
  CreateObject(XTNK, 765, 390, -1)->AutoRespawn();
  CreateObject(XTNK, 1555, 510, -1)->AutoRespawn();
  CreateObject(XTNK, 1720, 510, -1)->AutoRespawn();
  CreateObject(XTNK, 2505, 390, -1)->AutoRespawn();

  //Stahlbrücken
  CreateObject(_HBR, 865, 402, -1);
  CreateObject(_HBR, 2415, 402, -1);

  //Flaggen
  aFlag[0] = CreateObject(OFPL,465,300,NO_OWNER);
    aFlag[0]->Set("$Flag1$",100);
    aFlag[0]->Capture(1);

  aFlag[1] = CreateObject(OFPL,1640,100,NO_OWNER);
    aFlag[1]->Set("$Flag2$",0);

  aFlag[2] = CreateObject(OFPL,1640,520,NO_OWNER);
    aFlag[2]->Set("$Flag3$",0);

  aFlag[3] = CreateObject(OFPL,2810,300,NO_OWNER);
    aFlag[3]->Set("$Flag4$",100);
    aFlag[3]->Capture(2);

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
  tmp = CreateObject(ABOX, 555, 300, -1);
  tmp->Set(AMOC);
  tmp = CreateObject(ABOX, 1615, 210, -1);
  tmp->Set(AMOC);
  tmp = CreateObject(ABOX, 2725, 300, -1);
  tmp->Set(AMOC);

  //Munitionskisten (Handgranaten)
  tmp = CreateObject(ABOX, 1340, 520, -1);
  tmp->Set(FRAG);
  tmp = CreateObject(ABOX, 1945, 520, -1);
  tmp->Set(FRAG);

  //Abwehrschild
  PlaceSpawnpoint(RSHL, 1640, 410);

  //Motorboote
  SetupVehicleSpawn([INFL],CreateObject(VSPW,670,610,-1),10*10,1);
  SetupVehicleSpawn([INFL],CreateObject(VSPW,2590,610,-1),10*10,2);
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  iX = 1640; iY = 200;
}