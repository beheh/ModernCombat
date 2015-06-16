/*-- Babel Tower --*/

#strict 2
#include CSTD

static aFlag,aTowerInterior,aSelfDefense;

public func SpecificEquipment()		{return [[JTPK, 1]];}	//Zusatzausrüstung: Jetpack
public func RecommendedGoals()		{return [GOCC];}	//Spielzielempfehlung
public func AssaultDefenderTeam()	{return 2;}		//Verteidigerteam bei Assault


/* OP Flaggenanordnung */

public func OccupationDir()
{
  return GOCC_Vertical;
}

/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(1,20,15);
  //Bildschirmfärbung
  SetGamma(RGB(15,15,15),RGB(118,118,118),RGB(215,215,215));
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Turmobjekte
  aTowerInterior = [];
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

  //Aufzüge
  CreateObject(LFTP, 1015, 900, -1);
  CreateObject(LFTP, 1355, 1250, -1);
  CreateObject(LFTP, 1185, 1650, -1);

  //Leitern
  aTowerInterior[0] = CreateObject(LADR, 1130, 286, -1);
  aTowerInterior[0] -> Set(15);
  aTowerInterior[1] = CreateObject(LADR, 1240, 286, -1);
  aTowerInterior[1] -> Set(15);
  CreateObject(LADR, 1110, 632, -1)->Set(38);
  CreateObject(LADR, 1260, 632, -1)->Set(38);
  CreateObject(LADR, 1040, 1578, -1)->Set(10);
  CreateObject(LADR, 1330, 1578, -1)->Set(10);
  CreateObject(LADR, 1045, 1902, -1)->Set(12);
  CreateObject(LADR, 1328, 1902, -1)->Set(12);

  //Bodenluken
  CreateObject(HA4K, 1110, 323, -1);
  CreateObject(HA4K, 1260, 323, -1);
  CreateObject(HA4K, 1110, 413, -1);
  CreateObject(HA4K, 1260, 413, -1);
  CreateObject(HA4K, 1140, 873, -1);
  CreateObject(HA4K, 1230, 873, -1);
  CreateObject(H24K, 1015, 1378, -1);
  CreateObject(H24K, 1355, 1378, -1);
  CreateObject(HA4K, 1040, 1493, -1);
  CreateObject(HA4K, 1330, 1493, -1);

  //Gerüste
  //Links
  CreateObject(SFFG, 625, 1990, -1)->Set(4);
  CreateObject(SFFG, 625, 2040, -1)->Set(4);
  CreateObject(SFFG, 625, 2090, -1)->Set(5);
  CreateObject(SFFG, 625, 2140, -1)->Set(5);
  CreateObject(SFFG, 625, 2190, -1)->Set(5);
  CreateObject(SFFG, 625, 2240, -1)->Set(5);
  CreateObject(SFFG, 625, 2290, -1)->Set(5);
  CreateObject(SFFG, 625, 2340, -1)->Set(5);
  CreateObject(SFFG, 625, 2390, -1)->Set(5);
  CreateObject(SFFG, 625, 2440, -1)->Set(5);
  CreateObject(SFFG, 625, 2490, -1)->Set(5);

  CreateObject(SFFG, 685, 1990, -1)->Set(4);
  CreateObject(SFFG, 685, 2040, -1)->Set(4);
  CreateObject(SFFG, 685, 2240, -1)->Set(5);
  CreateObject(SFFG, 685, 2440, -1)->Set(5);

  CreateObject(SFFG, 745, 990, -1)->Set(5);
  CreateObject(SFFG, 745, 1040, -1)->Set(5);
  CreateObject(SFFG, 745, 1090, -1)->Set(5);
  CreateObject(SFFG, 745, 1140, -1)->Set(5);
  CreateObject(SFFG, 745, 1190, -1)->Set(5);
  CreateObject(SFFG, 745, 1240, -1)->Set(5);
  CreateObject(SFFG, 745, 1290, -1)->Set(4);
  CreateObject(SFFG, 745, 1340, -1)->Set(4);
  CreateObject(SFFG, 745, 1390, -1)->Set(5);
  CreateObject(SFFG, 745, 1440, -1)->Set(5);
  CreateObject(SFFG, 745, 1490, -1);
  CreateObject(SFFG, 745, 1540, -1)->Set(5);
  CreateObject(SFFG, 745, 1590, -1)->Set(5);
  CreateObject(SFFG, 745, 1640, -1)->Set(4);
  CreateObject(SFFG, 745, 1690, -1)->Set(5);
  CreateObject(SFFG, 745, 1740, -1)->Set(5);
  CreateObject(SFFG, 745, 1790, -1)->Set(4);
  CreateObject(SFFG, 745, 1840, -1)->Set(5);
  CreateObject(SFFG, 745, 1890, -1)->Set(5);
  CreateObject(SFFG, 745, 1940, -1)->Set(5);
  CreateObject(SFFG, 745, 1990, -1)->Set(4);
  CreateObject(SFFG, 745, 2040, -1)->Set(4);
  CreateObject(SFFG, 745, 2090, -1)->Set(5);
  CreateObject(SFFG, 745, 2140, -1)->Set(5);
  CreateObject(SFFG, 745, 2190, -1)->Set(5);
  CreateObject(SFFG, 745, 2240, -1)->Set(5);
  CreateObject(SFFG, 745, 2290, -1)->Set(5);
  CreateObject(SFFG, 745, 2340, -1)->Set(5);
  CreateObject(SFFG, 745, 2390, -1)->Set(5);
  CreateObject(SFFG, 745, 2440, -1)->Set(5);
  CreateObject(SFFG, 745, 2490, -1)->Set(5);


  CreateObject(SFFG, 885, 520, -1)->Set(5);
  CreateObject(SFFG, 885, 570, -1)->Set(5);
  CreateObject(SFFG, 885, 620, -1)->Set(5);
  CreateObject(SFFG, 885, 670, -1)->Set(5);
  CreateObject(SFFG, 885, 720, -1)->Set(5);
  CreateObject(SFFG, 885, 770, -1)->Set(5);
  CreateObject(SFFG, 885, 820, -1)->Set(5);
  CreateObject(SFFG, 885, 870, -1)->Set(5);
  CreateObject(SFFG, 885, 920, -1)->Set(5);

  CreateObject(SFFG, 885, 990, -1)->Set(5);
  CreateObject(SFFG, 885, 1040, -1)->Set(5);
  CreateObject(SFFG, 885, 1090, -1)->Set(5);
  CreateObject(SFFG, 885, 1140, -1)->Set(5);
  CreateObject(SFFG, 885, 1190, -1)->Set(5);
  CreateObject(SFFG, 885, 1240, -1)->Set(5);
  CreateObject(SFFG, 885, 1290, -1)->Set(4);
  CreateObject(SFFG, 885, 1340, -1)->Set(4);
  CreateObject(SFFG, 885, 1390, -1)->Set(5);
  CreateObject(SFFG, 885, 1440, -1)->Set(3);
  CreateObject(SFFG, 885, 1490, -1)->Set(2);
  CreateObject(SFFG, 885, 1540, -1)->Set(5);
  CreateObject(SFFG, 885, 1590, -1)->Set(5);
  CreateObject(SFFG, 885, 1640, -1)->Set(4);
  CreateObject(SFFG, 885, 1690, -1)->Set(5);
  CreateObject(SFFG, 885, 1740, -1)->Set(5);
  CreateObject(SFFG, 885, 1790, -1)->Set(4);
  CreateObject(SFFG, 885, 1840, -1)->Set(5);
  CreateObject(SFFG, 885, 1890, -1)->Set(4);
  CreateObject(SFFG, 885, 1940, -1)->Set(5);
  CreateObject(SFFG, 885, 1990, -1)->Set(5);
  CreateObject(SFFG, 885, 2040, -1)->Set(5);
  CreateObject(SFFG, 885, 2090, -1)->Set(5);
  CreateObject(SFFG, 885, 2140, -1)->Set(4);
  CreateObject(SFFG, 885, 2190, -1)->Set(5);
  CreateObject(SFFG, 885, 2240, -1)->Set(5);
  CreateObject(SFFG, 885, 2290, -1)->Set(5);
  CreateObject(SFFG, 885, 2340, -1)->Set(5);
  CreateObject(SFFG, 885, 2390, -1)->Set(5);
  CreateObject(SFFG, 885, 2440, -1)->Set(5);
  CreateObject(SFFG, 885, 2490, -1)->Set(5);

  //Rechts
  CreateObject(SFFG, 1485, 520, -1)->Set(5);
  CreateObject(SFFG, 1485, 570, -1)->Set(5);
  CreateObject(SFFG, 1485, 620, -1)->Set(5);
  CreateObject(SFFG, 1485, 670, -1)->Set(5);
  CreateObject(SFFG, 1485, 720, -1)->Set(5);
  CreateObject(SFFG, 1485, 770, -1)->Set(5);
  CreateObject(SFFG, 1485, 820, -1)->Set(5);
  CreateObject(SFFG, 1485, 870, -1)->Set(5);
  CreateObject(SFFG, 1485, 920, -1)->Set(5);

  CreateObject(SFFG, 1485, 990, -1)->Set(5);
  CreateObject(SFFG, 1485, 1040, -1)->Set(5);
  CreateObject(SFFG, 1485, 1090, -1)->Set(5);
  CreateObject(SFFG, 1485, 1140, -1)->Set(5);
  CreateObject(SFFG, 1485, 1190, -1)->Set(5);
  CreateObject(SFFG, 1485, 1240, -1)->Set(5);
  CreateObject(SFFG, 1485, 1290, -1)->Set(4);
  CreateObject(SFFG, 1485, 1340, -1)->Set(4);
  CreateObject(SFFG, 1485, 1390, -1)->Set(5);
  CreateObject(SFFG, 1485, 1440, -1)->Set(2);
  CreateObject(SFFG, 1485, 1490, -1)->Set(3);
  CreateObject(SFFG, 1485, 1540, -1)->Set(5);
  CreateObject(SFFG, 1485, 1590, -1)->Set(5);
  CreateObject(SFFG, 1485, 1640, -1)->Set(4);
  CreateObject(SFFG, 1485, 1690, -1)->Set(5);
  CreateObject(SFFG, 1485, 1740, -1)->Set(5);
  CreateObject(SFFG, 1485, 1790, -1)->Set(4);
  CreateObject(SFFG, 1485, 1840, -1)->Set(5);
  CreateObject(SFFG, 1485, 1890, -1)->Set(4);
  CreateObject(SFFG, 1485, 1940, -1)->Set(5);
  CreateObject(SFFG, 1485, 1990, -1)->Set(5);
  CreateObject(SFFG, 1485, 2040, -1)->Set(5);
  CreateObject(SFFG, 1485, 2090, -1)->Set(5);
  CreateObject(SFFG, 1485, 2140, -1)->Set(4);
  CreateObject(SFFG, 1485, 2190, -1)->Set(5);
  CreateObject(SFFG, 1485, 2240, -1)->Set(5);
  CreateObject(SFFG, 1485, 2290, -1)->Set(5);
  CreateObject(SFFG, 1485, 2340, -1)->Set(5);
  CreateObject(SFFG, 1485, 2390, -1)->Set(5);
  CreateObject(SFFG, 1485, 2440, -1)->Set(5);
  CreateObject(SFFG, 1485, 2490, -1)->Set(5);


  CreateObject(SFFG, 1625, 990, -1)->Set(5);
  CreateObject(SFFG, 1625, 1040, -1)->Set(5);
  CreateObject(SFFG, 1625, 1090, -1)->Set(5);
  CreateObject(SFFG, 1625, 1140, -1)->Set(5);
  CreateObject(SFFG, 1625, 1190, -1)->Set(5);
  CreateObject(SFFG, 1625, 1240, -1)->Set(5);
  CreateObject(SFFG, 1625, 1290, -1)->Set(4);
  CreateObject(SFFG, 1625, 1340, -1)->Set(4);
  CreateObject(SFFG, 1625, 1390, -1)->Set(5);
  CreateObject(SFFG, 1625, 1440, -1)->Set(5);
  CreateObject(SFFG, 1625, 1490, -1);
  CreateObject(SFFG, 1625, 1540, -1)->Set(5);
  CreateObject(SFFG, 1625, 1590, -1)->Set(5);
  CreateObject(SFFG, 1625, 1640, -1)->Set(4);
  CreateObject(SFFG, 1625, 1690, -1)->Set(5);
  CreateObject(SFFG, 1625, 1740, -1)->Set(5);
  CreateObject(SFFG, 1625, 1790, -1)->Set(4);
  CreateObject(SFFG, 1625, 1840, -1)->Set(5);
  CreateObject(SFFG, 1625, 1890, -1)->Set(5);
  CreateObject(SFFG, 1625, 1940, -1)->Set(5);
  CreateObject(SFFG, 1625, 1990, -1)->Set(4);
  CreateObject(SFFG, 1625, 2040, -1)->Set(4);
  CreateObject(SFFG, 1625, 2090, -1)->Set(5);
  CreateObject(SFFG, 1625, 2140, -1)->Set(5);
  CreateObject(SFFG, 1625, 2190, -1)->Set(5);
  CreateObject(SFFG, 1625, 2240, -1)->Set(5);
  CreateObject(SFFG, 1625, 2290, -1)->Set(5);
  CreateObject(SFFG, 1625, 2340, -1)->Set(5);
  CreateObject(SFFG, 1625, 2390, -1)->Set(5);
  CreateObject(SFFG, 1625, 2440, -1)->Set(5);
  CreateObject(SFFG, 1625, 2490, -1)->Set(5);

  CreateObject(SFFG, 1685, 1990, -1)->Set(4);
  CreateObject(SFFG, 1685, 2040, -1)->Set(4);
  CreateObject(SFFG, 1685, 2240, -1)->Set(5);
  CreateObject(SFFG, 1685, 2440, -1)->Set(5);

  CreateObject(SFFG, 1745, 1990, -1)->Set(4);
  CreateObject(SFFG, 1745, 2040, -1)->Set(4);
  CreateObject(SFFG, 1745, 2090, -1)->Set(5);
  CreateObject(SFFG, 1745, 2140, -1)->Set(5);
  CreateObject(SFFG, 1745, 2190, -1)->Set(5);
  CreateObject(SFFG, 1745, 2240, -1)->Set(5);
  CreateObject(SFFG, 1745, 2290, -1)->Set(5);
  CreateObject(SFFG, 1745, 2340, -1)->Set(5);
  CreateObject(SFFG, 1745, 2390, -1)->Set(5);
  CreateObject(SFFG, 1745, 2440, -1)->Set(5);
  CreateObject(SFFG, 1745, 2490, -1)->Set(5);

  //Rampen
  DrawMaterialQuad("Wall-Concrete3",1120,420,1120,410,1130,420,1120,420,true);
  DrawMaterialQuad("Wall-Concrete3",1250,420,1250,410,1240,420,1250,420,true);

  DrawMaterialQuad("Wall-Concrete3",1100,1940,1130,1930,1130,1940,1115,1940,true);
  DrawMaterialQuad("Wall-Concrete3",1130,1930,1160,1920,1160,1930,1145,1930,true);

  DrawMaterialQuad("Wall-Concrete3",1241,1930,1211,1920,1211,1930,1226,1930,true);
  DrawMaterialQuad("Wall-Concrete3",1271,1940,1241,1930,1241,1940,1256,1940,true);

  //Sprungschanzen
  CreateObject(JMPD, 900, 920, -1)->Set(100, 10);
  CreateObject(JMPD, 1470, 920, -1)->Set(100, -10);
  CreateObject(JMPD, 860, 2090, -1)->Set(90, 15);
  CreateObject(JMPD, 880, 2090, -1)->Set(90, -15);
  CreateObject(JMPD, 1490, 2090, -1)->Set(90, 15);
  CreateObject(JMPD, 1510, 2090, -1)->Set(90, -15);

  //Rahmen
  CreateObject(FRME, 1070, 1082, -1);
  CreateObject(FRME, 1140, 1082, -1);
  CreateObject(FRME, 1230, 1082, -1);
  CreateObject(FRME, 1300, 1082, -1);

  //Glasscheiben
  //Links
  CreateObject(PANE, 972, 950, -1);
  CreateObject(PANE, 972, 1000, -1);
  CreateObject(PANE, 972, 1052, -1);
  CreateObject(PANE, 972, 1081, -1);
  CreateObject(PANE, 972, 1110, -1);
  CreateObject(PANE, 972, 1160, -1);
  CreateObject(PANE, 972, 1320, -1);
  CreateObject(PANE, 972, 1370, -1);
  CreateObject(PANE, 972, 1440, -1);
  CreateObject(PANE, 972, 1490, -1);
  CreateObject(PANE, 972, 1690, -1);
  CreateObject(PANE, 972, 1750, -1);
  CreateObject(PANE, 972, 1800, -1);
  CreateObject(PANE, 972, 1850, -1);

  //Rechts
  CreateObject(PANE, 1398, 950, -1);
  CreateObject(PANE, 1398, 1000, -1);
  CreateObject(PANE, 1398, 1052, -1);
  CreateObject(PANE, 1398, 1081, -1);
  CreateObject(PANE, 1398, 1110, -1);
  CreateObject(PANE, 1398, 1160, -1);
  CreateObject(PANE, 1398, 1320, -1);
  CreateObject(PANE, 1398, 1370, -1);
  CreateObject(PANE, 1398, 1440, -1);
  CreateObject(PANE, 1398, 1490, -1);
  CreateObject(PANE, 1398, 1690, -1);
  CreateObject(PANE, 1398, 1750, -1);
  CreateObject(PANE, 1398, 1800, -1);
  CreateObject(PANE, 1398, 1850, -1);

  CreateObject(PAN2, 1015, 1806, -1);
  CreateObject(PAN2, 1115, 1256, -1);
  CreateObject(PAN2, 1255, 1256, -1);
  CreateObject(PAN2, 1355, 1806, -1);

  //Kiste
  CreateObject(WCR2, 1255, 870, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 1115, 660, -1)->AutoRespawn();
  CreateObject(MWCR, 1185, 1120, -1)->AutoRespawn();
  CreateObject(MWCR, 860, 1840, -1)->AutoRespawn();
  CreateObject(MWCR, 1510, 1840, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 1185, 1370, -1)->AutoRespawn();
  CreateObject(BECR, 910, 1590, -1)->AutoRespawn();
  CreateObject(BECR, 1460, 1590, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 1130, 980, -1)->AutoRespawn();
  CreateObject(XWCR, 720, 1290, -1)->AutoRespawn();
  CreateObject(XWCR, 1645, 1290, -1)->AutoRespawn();
  CreateObject(XWCR, 980, 1940, -1)->AutoRespawn();
  CreateObject(XWCR, 1390, 1940, -1)->AutoRespawn();
  CreateObject(XWCR, 685, 1990, -1)->AutoRespawn();
  CreateObject(XWCR, 1685, 1990, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 1240, 720, -1)->AutoRespawn();
  CreateObject(PBRL, 1225, 1120, -1)->AutoRespawn();
  CreateObject(PBRL, 1235, 1120, -1)->AutoRespawn();
  CreateObject(PBRL, 1000, 1610, -1)->AutoRespawn();
  CreateObject(PBRL, 1370, 1610, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 1160, 1370, -1)->AutoRespawn();
  CreateObject(XBRL, 1210, 1370, -1)->AutoRespawn();
  CreateObject(XBRL, 720, 1440, -1);
  CreateObject(XBRL, 730, 1440, -1);
  CreateObject(XBRL, 1635, 1440, -1);
  CreateObject(XBRL, 1645, 1440, -1);
  CreateObject(XBRL, 1090, 1690, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1095, 660, -1)->AutoRespawn();
  CreateObject(GSBL, 1075, 1370, -1)->AutoRespawn();
  CreateObject(GSBL, 1295, 1370, -1)->AutoRespawn();
  CreateObject(GSBL, 720, 1562, -1)->AutoRespawn();
  CreateObject(GSBL, 1650, 1562, -1)->AutoRespawn();
  CreateObject(GSBL, 1080, 1800, -1)->AutoRespawn();
  CreateObject(GSBL, 1290, 1800, -1)->AutoRespawn();

  //Stahlbrücken
  CreateObject(_HBR, 815, 1302, -1)->SwitchMode();
  CreateObject(_HBR, 1555, 1302, -1)->SwitchMode();
  CreateObject(_HBR, 815, 1602, -1)->SwitchMode();
  CreateObject(_HBR, 1555, 1602, -1)->SwitchMode();
  CreateObject(_HBR, 815, 1752, -1)->SwitchMode();
  CreateObject(_HBR, 1555, 1752, -1)->SwitchMode();

  //Container
  CreateObject(CON1, 725, 1241, -1)->SetPerspective(2);
  CreateObject(CON1, 1645, 1241, -1)->SetPerspective(2);
  CreateObject(CON1, 735, 1591, -1);
  CreateObject(CON1, 1635, 1591, -1)->SetPerspective(3);

  //Schutztüren
  CreateObject(GDDR, 1050, 870, -1);
  CreateObject(GDDR, 1320, 870, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1050, 980, -1);
  CreateObject(GDDR, 1320, 980, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1075, 1490, -1);
  CreateObject(GDDR, 1295, 1490, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1075, 1610, -1);
  CreateObject(GDDR, 1295, 1610, -1)->SetColorDw(HSL(145, 210, 100, 127));

  //Grenze
  CreateObject(BRDR, 0, 2410, -1)->Set(3,0,1);

  //Säulen
  CreateObject(PILR, 980, 1250, -1)->Set("PreparePillarCollapseL1");
  CreateObject(PILR, 1390, 1250, -1)->Set("PreparePillarCollapseR1");

  CreateObject(PILR, 980, 1610, -1)->Set("PreparePillarCollapseL2");
  CreateObject(PILR, 1390, 1610, -1)->Set("PreparePillarCollapseR2");

  //Tore und Konsolen
  var autod = CreateObject(HNG3, 1045, 1250, -1);
  autod->Close();
  CreateObject(CONS, 1160, 1245, -1)->Set(autod);
  autod = CreateObject(HNG3, 1325, 1250, -1);
  autod->Open();
  CreateObject(CONS, 1210, 1245, -1)->Set(autod);
  autod = CreateObject(HNG3, 1045, 1370, -1);
  autod->Open();
  CreateObject(CONS, 985, 1365, -1)->Set(autod);
  autod = CreateObject(HNG3, 1325, 1370, -1);
  autod->Close();
  CreateObject(CONS, 1380, 1365, -1)->Set(autod);

  autod = CreateObject(HNG3, 1065, 1800, -1);
  autod->Close();
  CreateObject(CONS, 1145, 1795, -1)->Set(autod);
  autod = CreateObject(HNG3, 1305, 1800, -1);
  autod->Close();
  CreateObject(CONS, 1225, 1795, -1)->Set(autod);
  autod = CreateObject(HNG3, 1065, 1940, -1);
  autod->Open();
  CreateObject(CONS, 1090, 1935, -1)->Set(autod);
  autod = CreateObject(HNG3, 1305, 1940, -1);
  autod->Open();
  CreateObject(CONS, 1280, 1935, -1)->Set(autod);

  //Hangartore und Konsolen
  autod = CreateObject(HNG1, 870, 1950, -1);
  autod->Open();
  CreateObject(CONS, 770, 1935, -1)->Set(autod);
  autod = CreateObject(HNG1, 1500, 1950, -1);
  autod->Open();
  CreateObject(CONS, 1590, 1935, -1)->Set(autod);

  //Selbstschussanlagen und Konsolen
  //Basis 2
  aSelfDefense[0] = CreateObject(SEGU, 1065, 700, -1);
  CreateObject(CONS, 1065, 645, -1)->Set(aSelfDefense[0]);

  aSelfDefense[1] = CreateObject(SEGU, 1305, 700, -1);
  CreateObject(CONS, 1305, 645, -1)->Set(aSelfDefense[1]);

  //Basis 3
  aSelfDefense[2] = CreateObject(SEGU, 1080, 1320, -1);
  CreateObject(CONS, 1130, 1360, -1)->Set(aSelfDefense[2]);

  aSelfDefense[3] = CreateObject(SEGU, 1290, 1320, -1);
  CreateObject(CONS, 1240, 1360, -1)->Set(aSelfDefense[3]);

  //Basis 4
  aSelfDefense[4] = CreateObject(SEGU, 745, 1765, -1);
  CreateObject(CONS, 770, 1985, -1)->Set(aSelfDefense[4]);

  aSelfDefense[5] = CreateObject(SEGU, 1625, 1765, -1);
  CreateObject(CONS, 1605, 1985, -1)->Set(aSelfDefense[5]);

  //Sendemast
  var tower = CreateObject(AATR, 1185, 720, -1);
  tower->AddNode(1047, 310, 0, CreateObject(REHR, 1040, 320, -1), 0, 1);
  tower->AddNode(1325, 310, 0, CreateObject(REHR, 1330, 320, -1), 0, 1);

  //Sounds

  //Wind
  CreateObject(SE4K, 1185, 150, -1)->Set("WindSound*.ogg",400,105);
  CreateObject(SE4K, 865, 520, -1)->Set("WindSound*.ogg",500,105);
  CreateObject(SE4K, 1500, 520, -1)->Set("WindSound*.ogg",500,105);
  CreateObject(SE4K, 745, 870, -1)->Set("WindSound*.ogg",600,105);
  CreateObject(SE4K, 1625, 870, -1)->Set("WindSound*.ogg",600,105);
  CreateObject(SE4K, 825, 1325, -1)->Set("WindSound*.ogg",700,105);
  CreateObject(SE4K, 1545, 1325, -1)->Set("WindSound*.ogg",700,105);
  CreateObject(SE4K, 765, 1855, -1)->Set("WindSound*.ogg",800,105);
  CreateObject(SE4K, 1605, 1855, -1)->Set("WindSound*.ogg",800,105);
  CreateObject(SE4K, 840, 2110, -1)->Set("WindSound*.ogg",1000,105);
  CreateObject(SE4K, 1530, 2110, -1)->Set("WindSound*.ogg",1000,105);

  //Rush Hour
  CreateObject(SE4K, 825, 2090, -1)->Set("Traffic*.ogg",245,70);
  CreateObject(SE4K, 1545, 2090, -1)->Set("Traffic*.ogg",245,70);

  //Sirenen
  CreateObject(SE4K, 620, 2090, -1)->Set("Siren*.ogg",280,140);
  CreateObject(SE4K, 1750, 2090, -1)->Set("Siren*.ogg",280,140);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 1155, 441, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 740, 1740, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  crate = CreateObject(AMCT, 1215, 441, -1);
  crate->Set(GBOX);
  crate = CreateObject(AMCT, 1620, 1740, -1);
  crate->Set(GBOX);

  //Versorgungskisten (APW)
  aTowerInterior[7] = CreateObject(AMCT, 1185, 392, -1);
  aTowerInterior[7] -> Set(ATWN);
  crate = CreateObject(AMCT, 1075, 1120, -1);
  crate->Set(ATWN);

  //Raketen
  PlaceSpawnpoint(MBOX, 1230, 975);
  PlaceSpawnpoint(MBOX, 1075, 1245);

  //Geschützstellungen
  aTowerInterior[2] = CreateObject(GNET, 1150, 162, -1);
  aTowerInterior[2] -> Set(LCAC,-90);
  aTowerInterior[3] = CreateObject(GNET, 1220, 162, -1);
  aTowerInterior[3] -> Set(LCAC,90);
  CreateObject(GNET, 1045, 410, -1)->Set(0,-90);
  CreateObject(GNET, 1325, 410, -1)->Set(0,90);
  CreateObject(GNET, 980, 800, -1)->Set(0,-90);
  CreateObject(GNET, 1390, 800, -1)->Set(0,90);
  CreateObject(GNET, 865, 1240, -1)->Set(LCAC);
  CreateObject(GNET, 1505, 1240, -1)->Set(LCAC);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Sonnenschirme
  CreateObject(SNSE, 1085, 320, -1);
  CreateObject(SNSE, 1285, 320, -1)->SetColorDw(HSL(145, 210, 100, 127));

  //Radio
  CreateObject(RDIO, 720, 1640, -1);

  //Räume
  CreateObject(ROM2, 1170, 2230, -1);
  CreateObject(ROM2, 1200, 2230, -1);

  //Orientierungslichter
  CreateObject(OLGH, 1185, 760, -1)->Set(3, 15, 1, 1, 40);
  CreateObject(OLGH, 1185, 1400, -1)->Set(5, 15, 1, 1, 40);
  CreateObject(OLGH, 1185, 1830, -1)->Set(2, 15, 1, 1, 40);

  //Scheinwerfer
  CreateObject(FLGH, 1060, 320, -1)->SetRotation(10);
  CreateObject(FLGH, 1310, 320, -1)->SetRotation(-10);
  var light = CreateObject(FLGH, 885, 472, -1);
  light ->SetRotation(65);
  light -> SetCategory(C4D_StaticBack);
  light = CreateObject(FLGH, 1485, 472, -1);
  light ->SetRotation(-65);
  light -> SetCategory(C4D_StaticBack);
  light = CreateObject(FLGH, 620, 2192, -1);
  light ->SetRotation(-50);
  light -> SetCategory(C4D_StaticBack);
  light = CreateObject(FLGH, 1745, 2192, -1);
  light ->SetRotation(50);
  light -> SetCategory(C4D_StaticBack);

  //Glühbirne
  CreateObject(LBGH, 1185, 900, -1);

  //Wandlampe
  CreateObject(BLGH, 1185, 1320, -1);

  //Laborlichter
  CreateObject(LLGH, 1140, 810, -1);
  CreateObject(LLGH, 1230, 810, -1);
  CreateObject(LLGH, 1130, 1460, -1);
  CreateObject(LLGH, 1240, 1460, -1);
  CreateObject(LLGH, 1115, 1740, -1);
  CreateObject(LLGH, 1255, 1740, -1);
  CreateObject(LLGH, 1115, 1880, -1);
  CreateObject(LLGH, 1255, 1880, -1);

  //Deckenlichter
  aTowerInterior[4] = CreateObject(CLGH, 1185, 170, -1);
  aTowerInterior[5] = CreateObject(CLGH, 1185, 448, -1);
  aTowerInterior[6] = CreateObject(CLGH, 1185, 612, -1);
  CreateObject(CLGH, 1130, 2102, -1);
  CreateObject(CLGH, 1240, 2102, -1);

  //Waschbecken
  CreateObject(WSHB, 1110, 980, -1);
  CreateObject(WSHB, 1260, 980, -1);

  //Toilettenkabinen
  CreateObject(TOIC, 1130, 980, -1);
  CreateObject(TOIC, 1150, 980, -1);
  CreateObject(TOIC, 1220, 980, -1);
  CreateObject(TOIC, 1240, 980, -1);

  //Spinde
  CreateObject(LCKR, 1000, 810, -1);
  CreateObject(LCKR, 1360, 810, -1);
  CreateObject(LCKR, 1110, 1610, -1);
  CreateObject(LCKR, 1250, 1610, -1);

  //Topfpflanzen
  CreateObject(PLT4, 975, 700, -1);
  CreateObject(PLT4, 1395, 700, -1);
  CreateObject(PLT3, 1185, 870, -1);
  CreateObject(PLT4, 980, 950, -1);
  CreateObject(PLT4, 1390, 950, -1);
  CreateObject(PLT4, 980, 1100, -1);
  CreateObject(PLT4, 1390, 1100, -1);
  CreateObject(PLT3, 1120, 1120, -1);
  CreateObject(PLT3, 1250, 1120, -1);
  CreateObject(PLT4, 980, 1250, -1);
  CreateObject(PLT4, 1390, 1250, -1);
  CreateObject(PLT3, 1150, 1560, -1);
  CreateObject(PLT3, 1220, 1560, -1);
  CreateObject(PLT3, 1015, 1610, -1);
  CreateObject(PLT3, 1355, 1610, -1);
  CreateObject(PLT4, 990, 1690, -1);
  CreateObject(PLT3, 1220, 1690, -1);
  CreateObject(PLT4, 1385, 1690, -1);
  CreateObject(PLT4, 990, 1800, -1);
  CreateObject(PLT4, 1385, 1800, -1);
  CreateObject(PLT3, 1000, 1940, -1);
  CreateObject(PLT3, 1165, 1920, -1);
  CreateObject(PLT3, 1205, 1920, -1);
  CreateObject(PLT3, 1370, 1940, -1);
  CreateObject(PLT3, 1130, 2230, -1);
  CreateObject(PLT3, 1240, 2230, -1);

  //Apparaturen
  CreateObject(GADG, 1165, 870, -1)->Set(1);
  CreateObject(GADG, 1205, 870, -1)->Set(1);
  CreateObject(GADG, 1035, 1370, -1)->Set(3);
  CreateObject(GADG, 1335, 1370, -1)->Set(3);

  //Geländer
  CreateObject(RAI1, 990, 720, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1340, 720, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1084, 980, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1209, 980, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 968, 1250, -1);
  CreateObject(RAI1, 1155, 1250, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1378, 1250, -1);
  CreateObject(RAI1, 980, 1370, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1330, 1370, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 970, 1610, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1340, 1610, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 586, 1940, -1)->SetRail([1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1597, 1940, -1)->SetRail([1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1110, 2230, -1)->SetRail([1,1,1,1,1,1,1,1]);

  //Automaten
  CreateObject(VGMN, 1100, 870, -1);
  CreateObject(VGMN, 1087, 1800, -1)->Set(2);

  //Lüftungsgitter
  CreateObject(ENGT, 1185, 1370, -1);

  //Bildschirme
  CreateObject(SCA2, 1185, 810, -1)->SetAction("News");
  CreateObject(SCR3, 1130, 840, -1);
  CreateObject(SCR3, 1240, 840, -1)->SetPhase(10);
  CreateObject(SCA1, 1015, 1460, -1);
  CreateObject(SCA1, 1355, 1460, -1)->SetPhase(10);
  CreateObject(SCA1, 1185, 1490, -1)->SetAction("CMC");
  CreateObject(SCA1, 1185, 1530, -1)->SetAction("Grenade");
  CreateObject(SCR3, 1130, 1905, -1)->SetClrModulation(RGB(255,0,0));
  CreateObject(SCR3, 1240, 1905, -1)->SetClrModulation(RGB(00,150,250));

  //Büsche
  var bush = CreateObject(BSH2, 1065, 1120, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  bush = CreateObject(BSH2, 1085, 1120, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  bush = CreateObject(BSH2, 1285, 1120, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  bush = CreateObject(BSH2, 1305, 1120, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);

  //Flaschen
  CreateObject(BOTL, 1370, 1490, -1);
  CreateObject(BOTL, 1140, 1678, -1);

  //Papierstapel
  CreateObject(PPSK, 1115, 1677, -1);
  CreateObject(PPSK, 1130, 1677, -1);

  //Regale
  CreateObject(FRAM, 1345, 810, -1);
  CreateObject(FRAM, 1075, 870, -1);
  CreateObject(FRAM, 1205, 1120, -1);
  CreateObject(FRAM, 1225, 1120, -1);
  CreateObject(FRAM, 990, 1490, -1);
  CreateObject(FRAM, 1010, 1490, -1);
  CreateObject(FRAM, 1030, 1490, -1);
  CreateObject(FRAM, 1240, 1690, -1);
  CreateObject(FRAM, 1260, 1690, -1);
  CreateObject(FRAM, 1280, 1690, -1);
  CreateObject(FRAM, 1300, 1690, -1);
  CreateObject(FRAM, 1285, 1800, -1);

  //Feuerlöscher
  CreateObject(FIE2, 1055, 805, -1);
  CreateObject(FIE2, 1315, 805, -1);
  CreateObject(FIE2, 1090, 1245, -1);
  CreateObject(FIE2, 1280, 1245, -1);

  //Tisch
  CreateObject(GTBL, 1130, 1690, -1);

  //Schilder
  CreateObject(ESGN, 1040, 800, -1);
  CreateObject(ESGN, 1330, 800, -1);
  CreateObject(WLSN, 1520, 910, -1)->Set("Heights");
  CreateObject(ESGN, 1090, 1230, -1);
  CreateObject(ESGN, 1280, 1230, -1);
  CreateObject(WLSN, 920, 1930, -1)->Set("Heights");
  CreateObject(WLSN, 1460, 1930, -1)->Set("Heights");

  //Ventillatoren
  CreateObject(VENT, 1125, 1200, -1)->SetCon(35);
  CreateObject(VENT, 1245, 1200, -1)->SetCon(35);
  CreateObject(VENT, 1125, 1340, -1)->SetCon(35);
  CreateObject(VENT, 1245, 1340, -1)->SetCon(35);
  CreateObject(VENT, 1100, 1670, -1)->SetCon(20);
  CreateObject(VENT, 1270, 1670, -1)->SetCon(20);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD03,700,400,-1)->SetClrModulation(RGBa(10,10,10,5));
  CreateObject(BD03,1200,800,-1)->SetClrModulation(RGBa(25,25,25,5));
  CreateObject(BD03,900,1000,-1)->SetClrModulation(RGBa(40,40,40,5));
  CreateObject(BD03,1185,1500,-1)->SetClrModulation(RGBa(40,40,40,5));
  CreateObject(BD03,1000,2000,-1);
  CreateObject(BD03,2100,2100,-1);
  CreateObject(BD03,1180,2200,-1);
  CreateObject(BD03,800,2300,-1);
  CreateObject(BD10,300,2460,-1)->SetClrModulation(RGB(180,180,180));
  CreateObject(BD10,1690,2460,-1)->SetClrModulation(RGB(180,180,180));
}

/* Bei Säulenzerstörungen */

func PreparePillarCollapseL1(int iPlr)
{
  //Spielerbildschirme schütteln
  ShakeViewPort(100);

  //Effekte
  AddParticlesInRect("Smoke3",[10,20],[970,900,20,280],[10,30],[200,400]);
  AddParticlesInRect("Smoke",[10,20],[970,900,20,280],[10,25],[200,400]);

  //Zerstörung planen
  Schedule(Format("GameCall(\"OnPillarCollapseL1\", %d)", iPlr), 200);
}

func OnPillarCollapseL1(int iPlr)
{
  //Effekte
  var effect = CreateObject(TIM1,980,1020);
  effect->ShakeViewPort(800, this);
  effect->Sound("StructureHit*.ogg");
  effect->Schedule("RemoveObject()",100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,980,940,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,980,990,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,980,1040,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,980,1090,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,980,1140,60,100);

  //Hintergrund zeichnen
  DrawMaterialQuad("BackWall-Column1",970,900,990,900,990,1181,970,1181,true);

  //Lebewesen verletzen
  for(var obj in FindObjects(Find_OCF(OCF_Alive), Find_InRect(970,900,20,280)))
   DoDmg(200, DMG_Explosion, obj, 0, iPlr + 1);

  //Dekoration entfernen
  for(var obj in FindObjects(Find_Or(Find_ID(PANE), Find_ID(PLT4)), Find_InRect(970,920,20,240)))
   RemoveObject(obj);

  //Trümmer verschleudern
  var debris = CreateObject(DBRS, 980,940, iPlr);
  Fling(debris, RandomX(-4,-2), RandomX(-2,1));
  SetRDir(RandomX(-20,20),debris);
  debris = CreateObject(DBRS, 980,1040, iPlr);
  Fling(debris, RandomX(-4,-2), RandomX(-2,1));
  SetRDir(RandomX(-20,20),debris);
  debris = CreateObject(DBRS, 980,1040, iPlr);
  Fling(debris, RandomX(2,4), RandomX(-2,1));
  SetRDir(RandomX(-20,20),debris);
  debris = CreateObject(DBRS, 980,1140, iPlr);
  Fling(debris, RandomX(-4,-2), RandomX(-2,1));
  SetRDir(RandomX(-20,20),debris);
}

func PreparePillarCollapseL2(int iPlr)
{
  //Spielerbildschirme schütteln
  ShakeViewPort(100);

  //Effekte
  AddParticlesInRect("Smoke3",[5,10],[970,1290,10,231],[10,30],[200,400]);
  AddParticlesInRect("Smoke",[5,10],[970,1290,10,231],[10,25],[200,400]);

  //Zerstörung planen
  Schedule(Format("GameCall(\"OnPillarCollapseL2\", %d)", iPlr), 200);
}

func OnPillarCollapseL2(int iPlr)
{
  //Effekte
  var effect = CreateObject(TIM1,980,1400);
  effect->ShakeViewPort(800, this);
  effect->Sound("StructureHit*.ogg");
  effect->Schedule("RemoveObject()",100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,970,1330,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,970,1450,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,1005,1500,60,100);

  //Hintergrund zeichnen
  DrawMaterialQuad("BackWall-Column1",970,1290,980,1290,980,1521,970,1521,true);
  DrawMaterialQuad("BackWall-Column1",980,1500,1030,1500,1030,1521,980,1521,true);

  //Lebewesen verletzen
  for(var obj in FindObjects(Find_OCF(OCF_Alive), Find_InRect(970,1290,10,231)))
   DoDmg(200, DMG_Explosion, obj, 0, iPlr + 1);

  //Dekoration entfernen
  for(var obj in FindObjects(Find_ID(PANE), Find_InRect(970,1290,10,200)))
   RemoveObject(obj);

  //Trümmer verschleudern
  var debris = CreateObject(DBRS, 970,1330, iPlr);
  Fling(debris, RandomX(-4,-2), RandomX(-2,1));
  SetRDir(RandomX(-20,20),debris);
  debris = CreateObject(DBRS, 970,1450, iPlr);
  debris->Sound("StructureIntegrity*.ogg");
  Fling(debris, RandomX(-4,-2), RandomX(-2,1));
  SetRDir(RandomX(-20,20),debris);
  debris = CreateObject(DBRS, 970,1450, iPlr);
  Fling(debris, RandomX(2,4), RandomX(-2,1));
  SetRDir(RandomX(-20,20),debris);
}

func PreparePillarCollapseR1(int iPlr)
{
  //Spielerbildschirme schütteln
  ShakeViewPort(100);

  //Effekte
  AddParticlesInRect("Smoke3",[5,10],[1380,900,20,280],[10,30],[200,400]);
  AddParticlesInRect("Smoke",[5,10],[1380,900,20,280],[10,25],[200,400]);

  //Zerstörung planen
  Schedule(Format("GameCall(\"OnPillarCollapseR1\", %d)", iPlr), 200);
}

func OnPillarCollapseR1(int iPlr)
{
  //Effekte
  var effect = CreateObject(TIM1,1390,1020);
  effect->ShakeViewPort(800, this);
  effect->Sound("StructureHit*.ogg");
  effect->Schedule("RemoveObject()",100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,1390,940,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,1390,990,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,1390,1040,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,1390,1090,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,1390,1140,60,100);

  //Hintergrund zeichnen
  DrawMaterialQuad("BackWall-Column1",1380,900,1400,900,1400,1181,1380,1181,true);

  //Lebewesen verletzen
  for(var obj in FindObjects(Find_OCF(OCF_Alive), Find_InRect(1380,900,20,280)))
   DoDmg(200, DMG_Explosion, obj, 0, iPlr + 1);

  //Dekoration entfernen
  for(var obj in FindObjects(Find_Or(Find_ID(PANE), Find_ID(PLT4)), Find_InRect(1380,920,20,240)))
   RemoveObject(obj);

  //Trümmer verschleudern
  var debris = CreateObject(DBRS, 1390,940, iPlr);
  Fling(debris, RandomX(2,4), RandomX(-2,1));
  SetRDir(RandomX(-20,20),debris);
  debris = CreateObject(DBRS, 1390,1040, iPlr);
  debris->Sound("StructureIntegrity*.ogg");
  Fling(debris, RandomX(2,4), RandomX(-2,1));
  SetRDir(RandomX(-20,20),debris);
  debris = CreateObject(DBRS, 1390,1040, iPlr);
  Fling(debris, RandomX(-4,-2), RandomX(-2,1));
  SetRDir(RandomX(-20,20),debris);
  debris = CreateObject(DBRS, 1390,1140, iPlr);
  Fling(debris, RandomX(2,4), RandomX(-2,1));
  SetRDir(RandomX(-20,20),debris);
}

func PreparePillarCollapseR2(int iPlr)
{
  //Spielerbildschirme schütteln
  ShakeViewPort(100);

  //Effekte
  AddParticlesInRect("Smoke3",[5,10],[1390,1290,10,231],[10,30],[200,400]);
  AddParticlesInRect("Smoke",[5,10],[1390,1290,10,231],[10,25],[200,400]);

  //Zerstörung planen
  Schedule(Format("GameCall(\"OnPillarCollapseR2\", %d)", iPlr), 200);
}

func OnPillarCollapseR2(int iPlr)
{
  //Effekte
  var effect = CreateObject(TIM1,1390,1400);
  effect->ShakeViewPort(800, this);
  effect->Sound("StructureHit*.ogg");
  effect->Schedule("RemoveObject()",100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,1400,1330,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,1400,1450,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,1400,1500,60,100);

  //Hintergrund zeichnen
  DrawMaterialQuad("BackWall-Column1",1340,1500,1390,1500,1390,1521,1340,1521,true);
  DrawMaterialQuad("BackWall-Column1",1390,1290,1401,1290,1401,1521,1390,1521,true);

  //Lebewesen verletzen
  for(var obj in FindObjects(Find_OCF(OCF_Alive), Find_InRect(1390,1290,10,231)))
   DoDmg(200, DMG_Explosion, obj, 0, iPlr + 1);

  //Dekoration entfernen
  for(var obj in FindObjects(Find_ID(PANE), Find_InRect(1390,1290,10,200)))
   RemoveObject(obj);

  //Trümmer verschleudern
  var debris = CreateObject(DBRS, 1400,1330, iPlr);
  Fling(debris, RandomX(2,4), RandomX(-2,1));
  SetRDir(RandomX(-20,20),debris);
  debris = CreateObject(DBRS, 1400,1450, iPlr);
  debris->Sound("StructureIntegrity*.ogg");
  Fling(debris, RandomX(2,4), RandomX(-2,1));
  SetRDir(RandomX(-20,20),debris);
  debris = CreateObject(DBRS, 1400,1450, iPlr);
  Fling(debris, RandomX(-4,-2), RandomX(-2,1));
  SetRDir(RandomX(-20,20),debris);
}

/* Bei Turmzusammenfall */

func OnTowerCollapse()
{
  EventInfo4K(0,Format("$MsgCollapse$"),AATR, 0, 0, 0, "Info_Event.ogg");

  //Objekte entfernen
  if(aTowerInterior[0]) aTowerInterior[0]->DecoExplode(30);
  if(aTowerInterior[1]) aTowerInterior[1]->DecoExplode(30);
  if(aTowerInterior[2]) aTowerInterior[2]->DecoExplode(30);
  if(aTowerInterior[3]) aTowerInterior[3]->DecoExplode(30);
  if(aTowerInterior[4]) aTowerInterior[4]->DecoExplode(30);
  if(aTowerInterior[5]) aTowerInterior[5]->DecoExplode(30);
  if(aTowerInterior[6]) aTowerInterior[6]->DecoExplode(30);
  if(aTowerInterior[7]) aTowerInterior[7]->DecoExplode(30);

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten verschieben
   aFlag[0]->MoveFlagpost(1185,320);
  }
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[1])
  {
   if(aSelfDefense[0])
    aSelfDefense[0]->SetTeam(iTeam);

   if(aSelfDefense[1])
    aSelfDefense[1]->SetTeam(iTeam);
  }

  if(pPoint == aFlag[2])
  {
   if(aSelfDefense[2])
    aSelfDefense[2]->SetTeam(iTeam);

   if(aSelfDefense[3])
    aSelfDefense[3]->SetTeam(iTeam);
  }

  if(pPoint == aFlag[4])
  {
   if(aSelfDefense[4])
    aSelfDefense[4]->SetTeam(iTeam);

   if(aSelfDefense[5])
    aSelfDefense[5]->SetTeam(iTeam);
  }
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Matrix.ogg;CMC_Moving Squad.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_Techno.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Friendly Unit.ogg");

  //Verzögerte Hinweisnachricht ausgeben
  Schedule("EventInfo4K(0,Format(\"$MsgJetpack$\"),JTPK, 0, 0, 0, \"Info_Event.ogg\");", 100);

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,1185,162,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(1075, 310);
   aFlag[0] -> AddSpawnPoint(1075, 400);
   aFlag[0] -> AddSpawnPoint(1295, 310);
   aFlag[0] -> AddSpawnPoint(1295, 400);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,1185,720,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(1020, 800);
   aFlag[1] -> AddSpawnPoint(870, 910);
   aFlag[1] -> AddSpawnPoint(1500, 910);
   aFlag[1] -> AddSpawnPoint(1350, 800);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,1185,1250,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(755, 1280);
   aFlag[2] -> AddSpawnPoint(880, 1280);
   aFlag[2] -> AddSpawnPoint(1485, 1280);
   aFlag[2] -> AddSpawnPoint(1615, 1280);
   if(aTeams[2] == true)
   {
    aFlag[2]->Set("$Flag3$",100,2);
    aFlag[2]->Capture(2,1);
   }
   else
   {
    aFlag[2]->Set("$Flag3$",0,2);
   }

   aFlag[3] = CreateObject(OFPL,1185,1560,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(770, 1580);
   aFlag[3] -> AddSpawnPoint(870, 1580);
   aFlag[3] -> AddSpawnPoint(1490, 1580);
   aFlag[3] -> AddSpawnPoint(1595, 1580);
   if(aTeams[2] == true)
   {
    aFlag[3]->Set("$Flag4$",100,2);
    aFlag[3]->Capture(2,1);
   }
   else
   {
    aFlag[3]->Set("$Flag4$",0,2);
   }

   aFlag[4] = CreateObject(OFPL,1185,1920,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(880, 1830);
   aFlag[4] -> AddSpawnPoint(990, 1680);
   aFlag[4] -> AddSpawnPoint(1295, 1680);
   aFlag[4] -> AddSpawnPoint(1380, 1680);
   aFlag[4] -> AddSpawnPoint(1480, 1830);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag5$",0,2);
   }

   //SSA Besitzer setzen
   if(aTeams[2] == true)
   {aSelfDefense[2]->SetTeam(2); aSelfDefense[3]->SetTeam(2); aSelfDefense[4]->SetTeam(2); aSelfDefense[5]->SetTeam(2);}

   //SSA aktivieren
   aSelfDefense[2]->TurnOn();
   aSelfDefense[3]->TurnOn();
   aSelfDefense[4]->TurnOn();
   aSelfDefense[5]->TurnOn();

   //Alarmleuchten
   //Flaggenposten 1
   var warn = CreateObject(ALGH, 1090, 320, -1);
   AddGOCCWarnEffect(warn,aFlag[0]);
   warn = CreateObject(ALGH, 1280, 320, -1);
   AddGOCCWarnEffect(warn,aFlag[0]);

   //Flaggenposten 2
   warn = CreateObject(ALGH, 985, 700, -1);
   AddGOCCWarnEffect(warn,aFlag[1]);
   warn = CreateObject(ALGH, 1385, 700, -1);
   AddGOCCWarnEffect(warn,aFlag[1]);

   //Flaggenposten 3
   warn = CreateObject(ALGH, 1105, 1280, -1);
    warn->SetR(90);
   AddGOCCWarnEffect(warn,aFlag[2]);
   warn = CreateObject(ALGH, 1265, 1280, -1);
    warn->SetR(-90);
   AddGOCCWarnEffect(warn,aFlag[2]);

   //Flaggenposten 4
   warn = CreateObject(ALGH, 1125, 1505, -1);
    warn->SetR(90);
   AddGOCCWarnEffect(warn,aFlag[3]);
   warn = CreateObject(ALGH, 1245, 1505, -1);
    warn->SetR(-90);
   AddGOCCWarnEffect(warn,aFlag[3]);

   //Flaggenposten 5
   warn = CreateObject(ALGH, 1090, 1889, -1);
    warn->SetR(-180);
   AddGOCCWarnEffect(warn,aFlag[4]);
   warn = CreateObject(ALGH, 1280, 1889, -1);
    warn->SetR(-180);
   AddGOCCWarnEffect(warn,aFlag[4]);

   //Helikopter und Hinweisschilder
   if(!FindObject(NOBH))
   {
    SetupVehicleSpawn([APCE],DIR_Right,CreateObject(VSPW,670,1910,-1),70*21);
    SetupVehicleSpawn([APCE],DIR_Left,CreateObject(VSPW,1700,1910,-1),70*21);

    var sign = CreateObject(SGNP, 720, 1940, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
    sign = CreateObject(SGNP, 1650, 1940, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
   }
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {
   //Zielobjekte
   AddAssaultTarget(CMSN, 1005, 1490, 25*30, 2, "$Target1$", 0, [[[755, 1240], [1185, 1250], [1610, 1240]], [[1040, 1940], [1185, 1920], [1330, 1940]]]);
   AddAssaultTarget(CCP1, 1365, 1490, 25*30, 2, "$Target2$", 1, [[[755, 1240], [1185, 1250], [1610, 1240]], [[1040, 1940], [1185, 1920], [1330, 1940]]]);
   AddAssaultTarget(CCP2, 1185, 870, 25*30, 2, "$Target3$", 2, [[[1075, 320], [1075, 410], [1295, 320], [1295, 410]], [[1110, 1490], [1185, 1560], [1260, 1490]]]);
   AddAssaultTarget(RADR, 1185, 720,  25*30, 2, "$Target4$", 3, [[[1075, 320], [1075, 410], [1295, 320], [1295, 410]], [[1110, 1490], [1185, 1560], [1260, 1490]]]);

   //Ziele verbinden
   ConnectAssaultTargets([0, 1]);
   ConnectAssaultTargets([2, 3]);

   //Grenze setzen
   CreateObject(BRDR, 0, 1120, -1)->Set(2,1);

   //Objekt entfernen
   RemoveObject(FindObject2(Find_ID(PLT3),Find_InRect(1170, 850, 20, 20)));
   RemoveObject(FindObject2(Find_ID(BTOL),Find_InRect(1350, 1450, 40, 40)));
   RemoveObject(FindObject2(Find_ID(WCR2),Find_InRect(1350, 1450, 40, 40)));

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[4]->SetTeam(1); aSelfDefense[5]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[0]->SetTeam(2); aSelfDefense[1]->SetTeam(2);}

   //SSA aktivieren
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();
   aSelfDefense[4]->TurnOn();
   aSelfDefense[5]->TurnOn();

   //Objekte entfernen
   RemoveObject(aSelfDefense[2]);
   RemoveObject(aSelfDefense[3]);

   //Helikopter und Hinweisschild
   if(!FindObject(NOBH))
   {
    SetupVehicleSpawn([APCE],DIR_Right,CreateObject(VSPW,670,1910,-1),70*21);

    var sign = CreateObject(SGNP, 720, 1940, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
   }
  }

  //LMS-Spielziel
  if(FindObject(GLMS))
  {
   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1); aSelfDefense[1]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[4]->SetTeam(2); aSelfDefense[5]->SetTeam(2);}

   //SSA aktivieren
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();
   aSelfDefense[4]->TurnOn();
   aSelfDefense[5]->TurnOn();

   //Objekte entfernen
   RemoveObject(aSelfDefense[2]);
   RemoveObject(aSelfDefense[3]);

   //Helikopter und Hinweisschilder
   if(!FindObject(NOBH))
   {
    SetupVehicleSpawn([APCE],DIR_Right,CreateObject(VSPW,670,1910,-1),70*21);
    SetupVehicleSpawn([APCE],DIR_Left,CreateObject(VSPW,1700,1910,-1),70*21);

    var sign = CreateObject(SGNP, 720, 1940, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
    sign = CreateObject(SGNP, 1650, 1940, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
   }
  }
}

/* Assault Zerstörung */

public func OnAssaultTargetDestruction(object pTarget, int iTeam, int iIndex, bool fConnectedDestroyed)
{
  //Ziel 1 und 2
  if(!iIndex || iIndex == 1)
  {
   if(fConnectedDestroyed)
   {
    //Grenze entfernen
    RemoveAll(BRDR);

    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),0,1370,2,1);

    //Lampen ausschalten
    for(var obj in FindObjects(Find_ID(LLGH), Find_InRect(1110,1440,150,460)))
    {
     obj->EMPShock();
     obj->TurnOff();
    }
   }

   //Säule zerstören
   if(!iIndex)
   {
    var pObj;
    if(pObj = FindObject2(Find_ID(PILR),Find_Local("iName","PreparePillarCollapseL2"),Find_Not(Find_Action("Destroyed"))))
     DoDamage(500,pObj);
   }
   else
   {
    var pObj;
    if(pObj = FindObject2(Find_ID(PILR),Find_Local("iName","PreparePillarCollapseR2"),Find_Not(Find_Action("Destroyed"))))
     DoDamage(500,pObj);
   }
  }

  //Ziel 3
  if(iIndex == 2)
  {
   //SSA zerstören
   aSelfDefense[0]->Disarm();
   DecoExplode(30, aSelfDefense[0]);
   aSelfDefense[1]->Disarm();
   DecoExplode(30, aSelfDefense[1]);

   //Lampen ausschalten
   for(var obj in FindObjects(Find_Or(Find_ID(BULB), Find_ID(BLGH), Find_ID(LLGH)), Find_InRect(1130,790,110,530)))
   {
    obj->EMPShock();
    obj->TurnOff();
   }
  }

  //Ziel 4
  if(iIndex == 3)
  {
   //Sendemast zerstören
   var pTower;
   if(pTower = FindObject2(Find_ID(AATR),Find_Not(Find_Func("IsDestroyed")))) 
    pTower->~PrepareCollapse();

   //Lampen ausschalten
   for(var obj in FindObjects(Find_ID(CLGH), Find_InRect(1180,160,10,460)))
   {
    obj->EMPShock();
    obj->TurnOff();
   }
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 1185; iY = 1310;
   return 1;
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {if(FindObject(GASS)->GetRespawnPoint(iX, iY, iTeam)) return 1;}

  //LMS-Spielziel
  if(FindObject(GLMS))
  {
   if(iTeam == 1)
   {
    return [[1075, 310], [1075, 400], [1295, 310], [1295, 400]];
   }
   if(iTeam == 2)
   {
    return [[990, 1680], [1040, 1930], [1185, 1910], [1330, 1930], [1295, 1680]];
   }
  }
}