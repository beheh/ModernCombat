/*-- Babel Tower --*/

#strict
#include CSTD

static aFlag,aSelfDefense;

public func SpecificEquipment()	{return [[JTPK, 1]];}	//Zusatzausr�stung: Jetpack
func RecommendedGoals()		{return [GOCC];}	//Spielzielempfehlung


/* OP Flaggenanordnung */

public func OccupationDir()
{
  return GOCC_Vertical;
}

/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetPlayList("CMC_WaitingforSpawn.ogg");
  Music("CMC_WaitingforSpawn.ogg",1);
  //Bildschirmf�rbung
  SetGamma(RGB(15,15,15),RGB(118,118,118),RGB(215,215,215));
  //Himmelparallaxit�t
  SetSkyParallax(0,15,15);
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausr�stung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  return(1);
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Aufz�ge
  CreateObject(LFTP, 415, 900, -1);
  CreateObject(LFTP, 755, 1250, -1);
  CreateObject(LFTP, 585, 1650, -1);

  //Leitern
  CreateObject(LADR, 566, 286, -1)->Set(15);
  CreateObject(LADR, 606, 286, -1)->Set(15);

  CreateObject(LADR, 440, 1578, -1)->Set(10);
  CreateObject(LADR, 730, 1578, -1)->Set(10);

  CreateObject(LADR, 445, 1902, -1)->Set(12);
  CreateObject(LADR, 728, 1902, -1)->Set(12);

  //Bodenluken
  CreateObject(HA4K, 540, 873, -1);
  CreateObject(HA4K, 630, 873, -1);
  CreateObject(H24K, 415, 1378, -1);
  CreateObject(H24K, 755, 1378, -1);
  CreateObject(HA4K, 440, 1493, -1);
  CreateObject(HA4K, 730, 1493, -1);

  //Ger�ste
  CreateObject(SFFG, 145, 740, -1)->Set(4);
  CreateObject(SFFG, 145, 790, -1)->Set(5);
  CreateObject(SFFG, 145, 840, -1)->Set(4);
  CreateObject(SFFG, 145, 890, -1)->Set(5);
  CreateObject(SFFG, 145, 940, -1)->Set(4);
  CreateObject(SFFG, 145, 990, -1)->Set(5);
  CreateObject(SFFG, 145, 1040, -1)->Set(5);
  CreateObject(SFFG, 145, 1090, -1)->Set(5);
  CreateObject(SFFG, 145, 1140, -1)->Set(2);
  CreateObject(SFFG, 145, 1190, -1)->Set(3);
  CreateObject(SFFG, 145, 1240, -1)->Set(2);
  CreateObject(SFFG, 145, 1290, -1)->Set(3);
  CreateObject(SFFG, 145, 1340, -1)->Set(5);
  CreateObject(SFFG, 145, 1390, -1)->Set(4);
  CreateObject(SFFG, 145, 1440, -1)->Set(5);
  CreateObject(SFFG, 145, 1490, -1)->Set(4);
  CreateObject(SFFG, 145, 1540, -1)->Set(5);
  CreateObject(SFFG, 145, 1590, -1)->Set(3);
  CreateObject(SFFG, 145, 1640, -1)->Set(2);
  CreateObject(SFFG, 145, 1690, -1)->Set(3);
  CreateObject(SFFG, 145, 1740, -1)->Set(2);
  CreateObject(SFFG, 145, 1790, -1);
  CreateObject(SFFG, 145, 1840, -1)->Set(5);
  CreateObject(SFFG, 145, 1890, -1)->Set(5);
  CreateObject(SFFG, 145, 1940, -1)->Set(5);

  CreateObject(SFFG, 145, 2010, -1)->Set(5);
  CreateObject(SFFG, 145, 2060, -1)->Set(4);
  CreateObject(SFFG, 145, 2110, -1)->Set(5);
  CreateObject(SFFG, 145, 2160, -1)->Set(5);
  CreateObject(SFFG, 145, 2210, -1)->Set(5);
  CreateObject(SFFG, 145, 2260, -1)->Set(5);
  CreateObject(SFFG, 145, 2310, -1)->Set(5);
  CreateObject(SFFG, 145, 2360, -1)->Set(5);
  CreateObject(SFFG, 145, 2410, -1)->Set(5);
  CreateObject(SFFG, 145, 2460, -1)->Set(5);


  CreateObject(SFFG, 285, 520, -1)->Set(4);
  CreateObject(SFFG, 285, 570, -1)->Set(4);
  CreateObject(SFFG, 285, 620, -1)->Set(4);
  CreateObject(SFFG, 285, 670, -1);
  CreateObject(SFFG, 285, 720, -1)->Set(5);
  CreateObject(SFFG, 285, 770, -1)->Set(5);
  CreateObject(SFFG, 285, 820, -1)->Set(5);
  CreateObject(SFFG, 285, 870, -1)->Set(5);
  CreateObject(SFFG, 285, 920, -1)->Set(5);

  CreateObject(SFFG, 285, 990, -1)->Set(5);
  CreateObject(SFFG, 285, 1040, -1)->Set(4);
  CreateObject(SFFG, 285, 1090, -1)->Set(5);
  CreateObject(SFFG, 285, 1140, -1)->Set(4);
  CreateObject(SFFG, 285, 1190, -1)->Set(5);
  CreateObject(SFFG, 285, 1240, -1)->Set(5);
  CreateObject(SFFG, 285, 1290, -1)->Set(4);
  CreateObject(SFFG, 285, 1340, -1)->Set(5);
  CreateObject(SFFG, 285, 1390, -1)->Set(5);
  CreateObject(SFFG, 285, 1440, -1)->Set(5);
  CreateObject(SFFG, 285, 1490, -1)->Set(5);
  CreateObject(SFFG, 285, 1540, -1)->Set(5);
  CreateObject(SFFG, 285, 1590, -1)->Set(4);
  CreateObject(SFFG, 285, 1640, -1)->Set(5);
  CreateObject(SFFG, 285, 1690, -1)->Set(5);
  CreateObject(SFFG, 285, 1740, -1)->Set(5);
  CreateObject(SFFG, 285, 1790, -1)->Set(4);
  CreateObject(SFFG, 285, 1840, -1)->Set(5);
  CreateObject(SFFG, 285, 1890, -1)->Set(4);
  CreateObject(SFFG, 285, 1940, -1)->Set(5);
  CreateObject(SFFG, 285, 1990, -1)->Set(5);
  CreateObject(SFFG, 285, 2040, -1)->Set(5);
  CreateObject(SFFG, 285, 2090, -1)->Set(5);

  CreateObject(SFFG, 285, 2160, -1)->Set(5);
  CreateObject(SFFG, 285, 2210, -1)->Set(5);
  CreateObject(SFFG, 285, 2260, -1)->Set(5);
  CreateObject(SFFG, 285, 2310, -1)->Set(5);
  CreateObject(SFFG, 285, 2360, -1)->Set(5);
  CreateObject(SFFG, 285, 2410, -1)->Set(5);
  CreateObject(SFFG, 285, 2460, -1)->Set(5);


  CreateObject(SFFG, 885, 520, -1)->Set(4);
  CreateObject(SFFG, 885, 570, -1)->Set(4);
  CreateObject(SFFG, 885, 620, -1)->Set(4);
  CreateObject(SFFG, 885, 670, -1);
  CreateObject(SFFG, 885, 720, -1)->Set(5);
  CreateObject(SFFG, 885, 770, -1)->Set(5);
  CreateObject(SFFG, 885, 820, -1)->Set(5);
  CreateObject(SFFG, 885, 870, -1)->Set(5);
  CreateObject(SFFG, 885, 920, -1)->Set(5);

  CreateObject(SFFG, 885, 990, -1)->Set(5);
  CreateObject(SFFG, 885, 1040, -1)->Set(4);
  CreateObject(SFFG, 885, 1090, -1)->Set(5);
  CreateObject(SFFG, 885, 1140, -1)->Set(4);
  CreateObject(SFFG, 885, 1190, -1)->Set(5);
  CreateObject(SFFG, 885, 1240, -1)->Set(5);
  CreateObject(SFFG, 885, 1290, -1)->Set(4);
  CreateObject(SFFG, 885, 1340, -1)->Set(5);
  CreateObject(SFFG, 885, 1390, -1)->Set(5);
  CreateObject(SFFG, 885, 1440, -1)->Set(5);
  CreateObject(SFFG, 885, 1490, -1)->Set(5);
  CreateObject(SFFG, 885, 1540, -1)->Set(5);
  CreateObject(SFFG, 885, 1590, -1)->Set(4);
  CreateObject(SFFG, 885, 1640, -1)->Set(5);
  CreateObject(SFFG, 885, 1690, -1)->Set(5);
  CreateObject(SFFG, 885, 1740, -1)->Set(5);
  CreateObject(SFFG, 885, 1790, -1)->Set(4);
  CreateObject(SFFG, 885, 1840, -1)->Set(5);
  CreateObject(SFFG, 885, 1890, -1)->Set(4);
  CreateObject(SFFG, 885, 1940, -1)->Set(5);
  CreateObject(SFFG, 885, 1990, -1)->Set(5);
  CreateObject(SFFG, 885, 2040, -1)->Set(5);
  CreateObject(SFFG, 885, 2090, -1)->Set(5);

  CreateObject(SFFG, 885, 2160, -1)->Set(5);
  CreateObject(SFFG, 885, 2210, -1)->Set(5);
  CreateObject(SFFG, 885, 2260, -1)->Set(5);
  CreateObject(SFFG, 885, 2310, -1)->Set(5);
  CreateObject(SFFG, 885, 2360, -1)->Set(5);
  CreateObject(SFFG, 885, 2410, -1)->Set(5);
  CreateObject(SFFG, 885, 2460, -1)->Set(5);


  CreateObject(SFFG, 1025, 740, -1)->Set(4);
  CreateObject(SFFG, 1025, 790, -1)->Set(5);
  CreateObject(SFFG, 1025, 840, -1)->Set(4);
  CreateObject(SFFG, 1025, 890, -1)->Set(5);
  CreateObject(SFFG, 1025, 940, -1)->Set(4);
  CreateObject(SFFG, 1025, 990, -1)->Set(5);
  CreateObject(SFFG, 1025, 1040, -1)->Set(5);
  CreateObject(SFFG, 1025, 1090, -1)->Set(5);
  CreateObject(SFFG, 1025, 1140, -1)->Set(3);
  CreateObject(SFFG, 1025, 1190, -1)->Set(2);
  CreateObject(SFFG, 1025, 1240, -1)->Set(3);
  CreateObject(SFFG, 1025, 1290, -1)->Set(2);
  CreateObject(SFFG, 1025, 1340, -1)->Set(5);
  CreateObject(SFFG, 1025, 1390, -1)->Set(4);
  CreateObject(SFFG, 1025, 1440, -1)->Set(5);
  CreateObject(SFFG, 1025, 1490, -1)->Set(4);
  CreateObject(SFFG, 1025, 1540, -1)->Set(5);

  CreateObject(SFFG, 1025, 1590, -1)->Set(2);
  CreateObject(SFFG, 1025, 1640, -1)->Set(3);
  CreateObject(SFFG, 1025, 1690, -1)->Set(2);
  CreateObject(SFFG, 1025, 1740, -1)->Set(3);
  CreateObject(SFFG, 1025, 1790, -1);
  CreateObject(SFFG, 1025, 1840, -1)->Set(5);
  CreateObject(SFFG, 1025, 1890, -1)->Set(5);
  CreateObject(SFFG, 1025, 1940, -1)->Set(5);

  CreateObject(SFFG, 1025, 2010, -1)->Set(5);
  CreateObject(SFFG, 1025, 2060, -1)->Set(4);
  CreateObject(SFFG, 1025, 2110, -1)->Set(5);
  CreateObject(SFFG, 1025, 2160, -1)->Set(5);
  CreateObject(SFFG, 1025, 2210, -1)->Set(5);
  CreateObject(SFFG, 1025, 2260, -1)->Set(5);
  CreateObject(SFFG, 1025, 2310, -1)->Set(5);
  CreateObject(SFFG, 1025, 2360, -1)->Set(5);
  CreateObject(SFFG, 1025, 2410, -1)->Set(5);
  CreateObject(SFFG, 1025, 2460, -1)->Set(5);

  //Rampen
  DrawMaterialQuad("Wall-Concrete3",500,1940,530,1930,530,1940,515,1940,true);
  DrawMaterialQuad("Wall-Concrete3",530,1930,560,1920,560,1930,545,1930,true);

  DrawMaterialQuad("Wall-Concrete3",641,1930,611,1920,611,1930,626,1930,true);
  DrawMaterialQuad("Wall-Concrete3",671,1940,641,1930,641,1940,656,1940,true);

  //Sprungschanzen
  CreateObject (JMPD, 300, 920, -1)->Set(100, 10);
  CreateObject (JMPD, 870, 920, -1)->Set(100, -10);

  CreateObject (JMPD, 240, 2090, -1)->Set(90, 15);
  CreateObject (JMPD, 930, 2090, -1)->Set(90, -15);

  //Glasscheiben
  //Links
  CreateObject(_WIN, 372, 800, -1);
  CreateObject(_WIN, 372, 950, -1);
  CreateObject(_WIN, 372, 1000, -1);
  CreateObject(_WIN, 372, 1100, -1);
  CreateObject(_WIN, 372, 1150, -1);
  CreateObject(_WIN, 372, 1320, -1);
  CreateObject(_WIN, 372, 1370, -1);
  CreateObject(_WIN, 372, 1440, -1);
  CreateObject(_WIN, 372, 1490, -1);
  CreateObject(_WIN, 372, 1690, -1);
  CreateObject(_WIN, 372, 1750, -1);
  CreateObject(_WIN, 372, 1800, -1);
  CreateObject(_WIN, 372, 1850, -1);

  //Rechts
  CreateObject(_WIN, 798, 800, -1);
  CreateObject(_WIN, 798, 950, -1);
  CreateObject(_WIN, 798, 1000, -1);
  CreateObject(_WIN, 798, 1100, -1);
  CreateObject(_WIN, 798, 1150, -1);
  CreateObject(_WIN, 798, 1320, -1);
  CreateObject(_WIN, 798, 1370, -1);
  CreateObject(_WIN, 798, 1440, -1);
  CreateObject(_WIN, 798, 1490, -1);
  CreateObject(_WIN, 798, 1690, -1);
  CreateObject(_WIN, 798, 1750, -1);
  CreateObject(_WIN, 798, 1800, -1);
  CreateObject(_WIN, 798, 1850, -1);

  //Kisten
  CreateObject(WCR2, 655, 870, -1)->AutoRespawn();
  CreateObject(WCR2, 585, 1120, -1)->AutoRespawn();
  CreateObject(WCR2, 775, 1490, -1)->AutoRespawn();
  CreateObject(WCR2, 470, 1690, -1)->AutoRespawn();

  //Metallkiste
  CreateObject(MWCR, 515, 660, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 260, 620, -1);
  CreateObject(BECR, 605, 1370, -1)->AutoRespawn();
  CreateObject(BECR, 910, 620, -1);

  //Explosive Kisten
  CreateObject(XWCR, 620, 320, -1)->AutoRespawn();
  CreateObject(XWCR, 530, 980, -1)->AutoRespawn();
  CreateObject(XWCR, 520, 1250, -1)->AutoRespawn();
  CreateObject(XWCR, 790, 1940, -1)->AutoRespawn();

  //Benzinf�sser
  CreateObject(PBRL, 640, 720, -1)->AutoRespawn();
  CreateObject(PBRL, 625, 1120, -1)->AutoRespawn();
  CreateObject(PBRL, 635, 1120, -1)->AutoRespawn();
  CreateObject(PBRL, 400, 1610, -1)->AutoRespawn();

  //Explosivf�sser
  CreateObject(XBRL, 170, 1090, -1);
  CreateObject(XBRL, 560, 1370, -1)->AutoRespawn();
  CreateObject(XBRL, 490, 1690, -1)->AutoRespawn();
  CreateObject(XBRL, 1000, 1090, -1);

  //Gasflaschen
  CreateObject(GSBL, 495, 660, -1)->AutoRespawn();
  CreateObject(GSBL, 575, 1370, -1)->AutoRespawn();
  CreateObject(GSBL, 585, 1370, -1)->AutoRespawn();
  CreateObject(GSBL, 470, 1800, -1)->AutoRespawn();

  //Schutzt�ren
  CreateObject(GDDR, 450, 870, -1);
  CreateObject(GDDR, 720, 870, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 450, 980, -1);
  CreateObject(GDDR, 720, 980, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 475, 1490, -1);
  CreateObject(GDDR, 695, 1490, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 475, 1610, -1);
  CreateObject(GDDR, 695, 1610, -1)->SetColorDw(HSL(145, 210, 100, 127));

  //Grenze
  CreateObject(BRDR, 0, 2410, -1)->Set(3,0,1);

  //Tore und Konsolen
  var autod = CreateObject (HNG3, 450, 1250, -1);
  autod->Close();
  CreateObject(CONS, 560, 1245, -1)->Set(autod);
  autod = CreateObject (HNG3, 720, 1250, -1);
  autod->Open();
  CreateObject(CONS, 610, 1245, -1)->Set(autod);
  autod = CreateObject (HNG3, 450, 1370, -1);
  autod->Open();
  CreateObject(CONS, 385, 1365, -1)->Set(autod);
  autod = CreateObject (HNG3, 720, 1370, -1);
  autod->Close();
  CreateObject(CONS, 780, 1365, -1)->Set(autod);
  autod = CreateObject (HNG3, 460, 1800, -1);
  autod->Close();
  CreateObject(CONS, 545, 1795, -1)->Set(autod);
  autod = CreateObject (HNG3, 460, 1940, -1);
  autod->Open();
  CreateObject(CONS, 490, 1935, -1)->Set(autod);
  autod = CreateObject (HNG3, 710, 1800, -1);
  autod->Close();
  CreateObject(CONS, 625, 1795, -1)->Set(autod);
  autod = CreateObject (HNG3, 710, 1940, -1);
  autod->Open();
  CreateObject(CONS, 680, 1935, -1)->Set(autod);

  //Hangartore und Konsolen
  autod = CreateObject (HNG1, 270, 1950, -1);
  autod->Open();
  CreateObject(CONS, 115, 1935, -1)->Set(autod);
  autod = CreateObject (HNG1, 900, 1950, -1);
  autod->Open();
  CreateObject (CONS, 1045, 1935, -1)->Set(autod);

  //Selbstschussanlagen und Konsolen
  //Basis 2
  aSelfDefense[0] = CreateObject (SEGU, 465, 698, -1);
  aSelfDefense[0]->Arm(MISA);
  CreateObject(CONS, 465, 645, -1)->Set(aSelfDefense[0]);

  aSelfDefense[1] = CreateObject (SEGU, 705, 698, -1);
  aSelfDefense[1]->Arm(MISA);
  CreateObject(CONS, 705, 645, -1)->Set(aSelfDefense[1]);

  //Basis 4
  aSelfDefense[2] = CreateObject (SEGU, 480, 1318, -1);
  aSelfDefense[2]->Arm(MISA);
  CreateObject (CONS, 530, 1360, -1)->Set(aSelfDefense[2]);

  aSelfDefense[3] = CreateObject (SEGU, 690, 1318, -1);
  aSelfDefense[3]->Arm(MISA);
  CreateObject(CONS, 640, 1360, -1)->Set(aSelfDefense[3]);

  //Sounds

  //Wind
  CreateObject(SE4K, 585, 150, -1)->Set("WindSound*.ogg",400,105);
  CreateObject(SE4K, 265, 520, -1)->Set("WindSound*.ogg",500,105);
  CreateObject(SE4K, 900, 520, -1)->Set("WindSound*.ogg",500,105);
  CreateObject(SE4K, 145, 870, -1)->Set("WindSound*.ogg",600,105);
  CreateObject(SE4K, 1025, 870, -1)->Set("WindSound*.ogg",600,105);
  CreateObject(SE4K, 225, 1325, -1)->Set("WindSound*.ogg",700,105);
  CreateObject(SE4K, 945, 1325, -1)->Set("WindSound*.ogg",700,105);
  CreateObject(SE4K, 165, 1855, -1)->Set("WindSound*.ogg",800,105);
  CreateObject(SE4K, 1005, 1855, -1)->Set("WindSound*.ogg",800,105);
  CreateObject(SE4K, 240, 2110, -1)->Set("WindSound*.ogg",1000,105);
  CreateObject(SE4K, 930, 2110, -1)->Set("WindSound*.ogg",1000,105);

  //Rush Hour
  CreateObject(SE4K, 225, 2090, -1)->Set("Traffic*.ogg",245,70);
  CreateObject(SE4K, 945, 2090, -1)->Set("Traffic*.ogg",245,70);

  //Sirenen
  CreateObject(SE4K, 20, 2090, -1)->Set("Siren*.ogg",280,140);
  CreateObject(SE4K, 1150, 2090, -1)->Set("Siren*.ogg",280,140);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 570, 320, -1);
  crate->Set(ABOX);
  var crate = CreateObject(AMCT, 730, 1940, -1);
  crate->Set(ABOX);

  //Kugeln
  PlaceSpawnpoint(ABOX, 740, 800);
  PlaceSpawnpoint(ABOX, 390, 1360);

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 300, 465);
  PlaceSpawnpoint(GBOX, 870, 1835);

  //Raketen
  PlaceSpawnpoint(MBOX, 585, 490);
  PlaceSpawnpoint(MBOX, 585, 1550);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Radio
  CreateObject(RDIO, 120, 1640, -1);

  //Orientierungslichter
  CreateObject(OLGH, 585, 760, -1)->Set(3, 15, 1, 1, 40);
  CreateObject(OLGH, 585, 1400, -1)->Set(5, 15, 1, 1, 40);
  CreateObject(OLGH, 585, 1830, -1)->Set(2, 15, 1, 1, 40);

  //Radare
  CreateObject(RADR, 460, 320, -1);
  CreateObject(RADR, 710, 320, -1);

  //Flutlichter
  CreateObject(FLGH, 460, 320, -1)->SetRotation(65);
  CreateObject(FLGH, 710, 320, -1);
  CreateObject(FLGH, 285, 470, -1)->SetRotation(65);
  CreateObject(FLGH, 885, 470, -1)->SetRotation(-65);

  //Gl�hbirne
  CreateObject(LBGH, 585, 900, -1);

  //Wandlampe
  CreateObject(BLGH, 585, 1320, -1);

  //Laborlichter
  CreateObject(LLGH, 540, 810, -1);
  CreateObject(LLGH, 630, 810, -1);
  CreateObject(LLGH, 530, 1460, -1);
  CreateObject(LLGH, 640, 1460, -1);
  CreateObject(LLGH, 515, 1740, -1);
  CreateObject(LLGH, 655, 1740, -1);
  CreateObject(LLGH, 515, 1880, -1);
  CreateObject(LLGH, 655, 1880, -1);

  //Deckenlichter
  CreateObject(CLGH, 585, 175, -1);
  CreateObject(CLGH, 585, 345, -1);
  CreateObject(CLGH, 585, 435, -1);
  CreateObject(CLGH, 585, 525, -1);
  CreateObject(CLGH, 585, 615, -1);

  //Waschbecken
  CreateObject(WSHB, 510, 980, -1);
  CreateObject(WSHB, 540, 980, -1);
  CreateObject(WSHB, 630, 980, -1);
  CreateObject(WSHB, 660, 980, -1);

  //Spinde
  CreateObject(LCKR, 400, 810, -1);
  CreateObject(LCKR, 760, 810, -1);
  CreateObject(LCKR, 510, 1610, -1);
  CreateObject(LCKR, 650, 1610, -1);

  //Pflanzen
  CreateObject(PLNT, 375, 700, -1);
  CreateObject(PLNT, 795, 700, -1);
  CreateObject(PLNT, 380, 950, -1);
  CreateObject(PLNT, 790, 950, -1);
  CreateObject(PLNT, 380, 1100, -1);
  CreateObject(PLNT, 790, 1100, -1);
  CreateObject(PLNT, 380, 1250, -1);
  CreateObject(PLNT, 790, 1250, -1);
  CreateObject(PLNT, 390, 1690, -1);
  CreateObject(PLNT, 390, 1800, -1);
  CreateObject(PLNT, 785, 1690, -1);
  CreateObject(PLNT, 785, 1800, -1);
  CreateObject(PLNT, 640, 1675, -1);

  //Apparaturen
  CreateObject(GADG, 565, 870, -1)->Set(1);
  CreateObject(GADG, 605, 870, -1)->Set(1);
  CreateObject(GADG, 435, 1370, -1)->Set(3);
  CreateObject(GADG, 735, 1370, -1)->Set(3);

  //Gel�nder
  CreateObject(RAI1, 390, 720, -1)->SetRail([1,1]);
  CreateObject(RAI1, 740, 720, -1)->SetRail([1,1]);
  CreateObject(RAI1, 370, 1610, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 740, 1610, -1)->SetRail([1,1,1]);

  //Automaten
  CreateObject(VGMN, 500, 870, -1);
  CreateObject(VGMN, 487, 1800, -1)->Set(2);

  //L�ftungsgitter
  CreateObject(ENGT, 585, 1370, -1);

  //Bildschirme
  CreateObject(SCA2, 585, 810, -1);
  CreateObject(SCR3, 530, 840, -1);
  CreateObject(SCR3, 640, 840, -1);
  CreateObject(SCA2, 585, 1490, -1)->SetAction("Clonk");
  CreateObject(SCR3, 530, 1900, -1);
  CreateObject(SCR3, 640, 1900, -1);

  //Topfpflanzen
  CreateObject(PLT3, 585, 870, -1);
  CreateObject(PLT2, 520, 1120, -1);
  CreateObject(PLT2, 650, 1120, -1);
  CreateObject(PLT3, 415, 1610, -1);
  CreateObject(PLT3, 755, 1610, -1);
  CreateObject(PLT2, 400, 1940, -1);
  CreateObject(PLT2, 770, 1940, -1);

  //Flaschen
  CreateObject(BOTL, 770, 1472, -1);
  CreateObject(BOTL, 540, 1678, -1);

  //Regale
  CreateObject(FRAM, 745, 810, -1);
  CreateObject(FRAM, 475, 870, -1);
  CreateObject(FRAM, 605, 1120, -1);
  CreateObject(FRAM, 625, 1120, -1);
  CreateObject(FRAM, 390, 1490, -1);
  CreateObject(FRAM, 410, 1490, -1);
  CreateObject(FRAM, 430, 1490, -1);
  CreateObject(FRAM, 685, 1800, -1);
  CreateObject(FRAM, 705, 1800, -1);

  //Tische
  CreateObject(GTBL, 585, 1560, -1);
  CreateObject(GTBL, 530, 1690, -1);
  CreateObject(GTBL, 640, 1690, -1);

  //Schilder
  CreateObject(WLSN, 180, 1930, -1)->SetPhase(2);
  CreateObject(ESGN, 440, 800, -1);
  CreateObject(ESGN, 730, 800, -1);
  CreateObject(ESGN, 490, 1235, -1);
  CreateObject(ESGN, 680, 1235, -1);
  CreateObject(WLSN, 920, 910, -1)->SetPhase(2);

  //Ventillatoren
  CreateObject(VENT, 500, 1670, -1)->SetCon(20);
  CreateObject(VENT, 670, 1670, -1)->SetCon(20);
}

/* Bei Flaggen�bernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[1])
  {
    if(aSelfDefense[0])
      aSelfDefense[0]->SetTeam(iTeam);
      
    if(aSelfDefense[1])
      aSelfDefense[1]->SetTeam(iTeam);
  }
  
  if(pPoint == aFlag[3])
  {
    if(aSelfDefense[2])
      aSelfDefense[2]->SetTeam(iTeam);
      
    if(aSelfDefense[3])
      aSelfDefense[3]->SetTeam(iTeam);
  }
}

/* Regelw�hler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Friendly Unit.ogg");

  //Verz�gerte Hinweisnachricht ausgeben
  Schedule("EventInfo4K(0,Format(\"$MsgJetpack$\"),JTPK, 0, 0, 0, \"PriorityInfo.ogg\");", 100);

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,585,160,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(270, 510);
   aFlag[0] -> AddSpawnPoint(610, 490);
   aFlag[0] -> AddSpawnPoint(890, 510);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,585,720,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(400, 800);
   aFlag[1] -> AddSpawnPoint(770, 800);
   aFlag[1] -> AddSpawnPoint(1025, 680);
   if(aTeams[1] == true)
   {
    aFlag[1]->Set("$Flag2$",100,2);
    aFlag[1]->Capture(1,1);
   }
   else
   {
    aFlag[1]->Set("$Flag2$",0,2);
   }

   aFlag[2] = CreateObject(OFPL,145,890,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(145, 780);
   aFlag[2] -> AddSpawnPoint(270, 980);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,585,1250,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(270, 1230);
   aFlag[3] -> AddSpawnPoint(700, 1110);
   aFlag[3] -> AddSpawnPoint(900, 1230);
   if(aTeams[2] == true)
   {
    aFlag[3]->Set("$Flag4$",100,2);
    aFlag[3]->Capture(2,1);
   }
   else
   {
    aFlag[3]->Set("$Flag4$",0,2);
   }

   aFlag[4] = CreateObject(OFPL,1025,1440,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(750, 1480);
   aFlag[4] -> AddSpawnPoint(900, 1530);
   aFlag[4] -> AddSpawnPoint(1030, 1330);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag5$",0,2);
   }

   aFlag[5] = CreateObject(OFPL,145,1940,NO_OWNER);
   aFlag[5] -> AddSpawnPoint(145, 2000);
   aFlag[5] -> AddSpawnPoint(270, 1730);
   aFlag[5] -> AddSpawnPoint(270, 1830);
   if(aTeams[2] == true)
   {
    aFlag[5]->Set("$Flag6$",100,2);
    aFlag[5]->Capture(2,1);
   }
   else
   {
    aFlag[5]->Set("$Flag6$",0,2);
   }

   aFlag[6] = CreateObject(OFPL,585,1920,NO_OWNER);
   aFlag[6] -> AddSpawnPoint(480, 1790);
   aFlag[6] -> AddSpawnPoint(540, 1680);
   aFlag[6] -> AddSpawnPoint(690, 1790);
   if(aTeams[2] == true)
   {
    aFlag[6]->Set("$Flag7$",100,2);
    aFlag[6]->Capture(2,1);
   }
   else
   {
    aFlag[6]->Set("$Flag7$",0,2);
   }

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1); aSelfDefense[1]->SetTeam(1);}

   //SSA aktivieren
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();

   //Alarmleuchten
   //Flaggenposten 1
   var warn = CreateObject (ALGH, 525, 320, -1);
   AddGOCCWarnEffect(warn,aFlag[0]);
   warn = CreateObject (ALGH, 645, 320, -1);
   AddGOCCWarnEffect(warn,aFlag[0]);

   //Flaggenposten 2
   warn = CreateObject (ALGH, 380, 700, -1);
   AddGOCCWarnEffect(warn,aFlag[1]);
   warn = CreateObject (ALGH, 790, 700, -1);
   AddGOCCWarnEffect(warn,aFlag[1]);

   //Flaggenposten 4
   warn = CreateObject (ALGH, 490, 1189, -1);
    warn->SetR(-180);
   AddGOCCWarnEffect(warn,aFlag[3]);
   warn = CreateObject (ALGH, 680, 1189, -1);
    warn->SetR(-180);
   AddGOCCWarnEffect(warn,aFlag[3]);

   //Flaggenposten 7
   warn = CreateObject (ALGH, 380, 1889, -1);
    warn->SetR(-180);
   AddGOCCWarnEffect(warn,aFlag[6]);
   warn = CreateObject (ALGH, 790, 1889, -1);
    warn->SetR(-180);
   AddGOCCWarnEffect(warn,aFlag[6]);
  }

  //Assault-Spielziel
  if (FindObject(GASS))
  {
   //Zielobjekte
   AddAssaultTarget(CMSN, 440, 1610, 230, 2, "$Target1$", 0, [[[420, 1490], [750, 1490]], [[440, 1940], [585, 1920], [730, 1940]]]);
   AddAssaultTarget(CCP1, 730, 1610, 230, 2, "$Target2$", 1, [[[420, 1490], [750, 1490]], [[440, 1940], [585, 1920], [730, 1940]]]);
   AddAssaultTarget(LBPC, 585, 1250, 230, 2, "$Target3$", 2, [[[475, 980], [695, 980]], [[430, 1610], [740, 1610]]]);
   AddAssaultTarget(CCP2, 585, 870,  230, 2, "$Target4$", 3, [[[445, 660], [585, 590], [725, 660]], [[280, 1240], [890, 1240]]]);

   //Ziele verbinden
   ConnectAssaultTargets([0, 1]);

   //Grenze setzen
   CreateObject(BRDR, 0, 1370, -1)->Set(2,1);

   //Objekt entfernen
   RemoveObject(FindObject2(Find_ID(PLT3),Find_InRect(570, 820, 40, 40)));

   //Gesch�tzstellungen
   CreateObject(GNET, 280, 1740, -1);
   CreateObject(GNET, 890, 1740, -1);

   //SSA Besitzer setzen
   if(aTeams[2] == true)
   {aSelfDefense[0]->SetTeam(2); aSelfDefense[1]->SetTeam(2); aSelfDefense[2]->SetTeam(2); aSelfDefense[3]->SetTeam(2);}

   //SSA aktivieren
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();
   aSelfDefense[2]->TurnOn();
   aSelfDefense[3]->TurnOn();
  }
}

/* Assault Zerst�rung */

public func OnAssaultTargetDestruction(object pTarget, int iTeam, int iIndex, bool fConnectedDestroyed)
{
  //Ziel 1 und 2
  if (!iIndex || iIndex == 1)
  {
   if(fConnectedDestroyed)
   {
    //Grenze neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 0, 920, -1)->Set(2,1);
   }
  }

  //Ziel 3
  if (iIndex == 2)
  {
   //SSA zerst�ren
   aSelfDefense[2]->Disarm();
   DecoExplode(30, aSelfDefense[2]);
   aSelfDefense[3]->Disarm();
   DecoExplode(30, aSelfDefense[3]);

   //Grenze neu setzen
   RemoveAll(BRDR);
   CreateObject(BRDR, 0, 520, -1)->Set(2,1);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 585; iY = 1420;
   return(1);
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {if(FindObject(GASS)->GetRespawnPoint(iX, iY, iTeam)) return 1;}

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    return [[460, 310], [710, 310]];
   }
   if(iTeam == 2)
   {
    return [[500, 1680], [670, 1680]];
   }
  }
}