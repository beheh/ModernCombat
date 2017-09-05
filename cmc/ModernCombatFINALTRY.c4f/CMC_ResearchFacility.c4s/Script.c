/*-- Research Facility --*/

#strict 2
#include CSTD

static aFlag,aStationary,aSelfDefense,aDoor,aDoorWay,aFlagPosition;

func RecommendedGoals()			{return [GASS, GHTF];}	//Spielzielempfehlung
public func AssaultDefenderTeam()	{return 2;}		//Verteidigerteam bei Assault


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(0,45,15);
  //Bildschirmfärbung
  SetGamma(RGB(7,6,0),RGB(152,147,128),RGB(255,254,236));
  //Flaggen
  aFlag = [];
  //Geschützstellungen
  aStationary = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Türen
  aDoor = [];
  //Türverbindungen
  aDoorWay = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  //Neustart-Button setzen
  SetNextMission("ModernCombat.c4f\\CMC_ResearchFacility.c4s", "$RepeatButton$", "$RepeatButtonDesc$");
  return 1; 
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Rampen
  DrawMaterialQuad("Wall-Concrete2",1141,810,1141,800,1161,810,1141,810,true);
  DrawMaterialQuad("Wall-Stripes",1210,170,1235,190,1235,190,1210,190,true);
  DrawMaterialQuad("Wall-Stripes",1220,1660,1240,1640,1240,1700,1220,1680,true);
  DrawMaterialQuad("Wall-Stripes",1400,1640,1420,1660,1420,1680,1400,1700,true);
  DrawMaterialQuad("Wall-Stripes",1501,1890,1501,1870,1531,1890,1501,1890,true);
  DrawMaterialQuad("Wall-Stripes",1510,821,1510,811,1440,821,1510,821,true);
  DrawMaterialQuad("BackWall-Grid1",1520,1899,1542,1899,1542,1901,1520,1901,true);
  DrawMaterialQuad("Wall-Concrete3",1991,1110,1991,1100,1961,1110,1991,1110,true);
  DrawMaterialQuad("Wall-Stripes",2021,820,2021,810,2091,820,2021,820,true);
  DrawMaterialQuad("Wall-Concrete2",2640,1270,2670,1290,2640,1290,2640,1270,true);

  //Leitern
  CreateObject(LADR, 230, 694, -1)->Set(27);
  CreateObject(LADR, 280, 796, -1)->Set(6);
  CreateObject(LADR, 325, 1007, -1)->Set(10);
  CreateObject(LADR, 325, 1955, -1)->Set(33);
  CreateObject(LADR, 405, 545, -1)->Set(6);
  CreateObject(LADR, 480, 1777, -1)->Set(31);
  CreateObject(LADR, 575, 545, -1)->Set(6);
  CreateObject(LADR, 525, 900, -1)->Set(8);
  CreateObject(LADR, 690, 712, -1)->Set(18);
  CreateObject(LADR, 710, 1154, -1)->Set(17);
  CreateObject(LADR, 785, 1955, -1)->Set(9);
  CreateObject(LADR, 835, 1955, -1)->Set(10);
  CreateObject(LADR, 840, 534, -1)->Set(37);
  CreateObject(LADR, 870, 984, -1)->Set(7);
  CreateObject(LADR, 1020, 522, -1)->Set(13);
  CreateObject(LADR, 1050, 278, -1)->Set(5);
  CreateObject(LADR, 1105, 133, -1)->Set(6);
  CreateObject(LADR, 1110, 988, -1)->Set(50);
  CreateObject(LADR, 1020, 1750, -1)->Set(49);
  CreateObject(LADR, 1130, 1346, -1)->Set(41);
  CreateObject(LADR, 1175, 1907, -1)->Set(11);
  CreateObject(LADR, 1240, 1344, -1)->Set(131);
  CreateObject(LADR, 1307, 153, -1)->Set(6);
  CreateObject(LADR, 1320, 1772, -1)->Set(3);
  CreateObject(LADR, 1367, 253, -1)->Set(6);
  CreateObject(LADR, 1420, 790, -1)->Set(20);
  CreateObject(LADR, 1460, 988, -1)->Set(5);
  CreateObject(LADR, 1520, 702, -1)->Set(9);
  CreateObject(LADR, 1530, 1530, -1)->Set(27);
  CreateObject(LADR, 1565, 1623, -1)->Set(7);
  CreateObject(LADR, 1565, 1875, -1)->Set(16);
  CreateObject(LADR, 1575, 1080, -1)->Set(8);
  CreateObject(LADR, 1580, 920, -1)->Set(8);
  CreateObject(LADR, 1620, 702, -1)->Set(9);
  CreateObject(LADR, 1700, 1075, -1)->Set(28);
  CreateObject(LADR, 1760, 1510, -1)->Set(19);
  CreateObject(LADR, 1830, 1075, -1)->Set(28);
  CreateObject(LADR, 1905, 1860, -1)->Set(14);
  CreateObject(LADR, 1940, 702, -1)->Set(9);
  CreateObject(LADR, 1950, 904, -1)->Set(6);
  CreateObject(LADR, 2040, 702, -1)->Set(9);
  CreateObject(LADR, 2140, 790, -1)->Set(20);
  CreateObject(LADR, 2140, 1014, -1)->Set(12);
  CreateObject(LADR, 2140, 1542, -1)->Set(28);
  CreateObject(LADR, 2400, 1422, -1)->Set(23);
  CreateObject(LADR, 2410, 1076, -1)->Set(41);
  CreateObject(LADR, 2575, 721, -1)->Set(13);
  CreateObject(LADR, 2730, 1530, -1)->Set(28);
  CreateObject(LADR, 2880, 886, -1)->Set(76);
  CreateObject(LADR, 2935, 881, -1)->Set(4);
  CreateObject(LADR, 2995, 1089, -1)->Set(30);
  CreateObject(LADR, 3200, 1065, -1)->Set(27);
  CreateObject(LADR, 3065, 881, -1)->Set(4);
  CreateObject(LADR, 3130, 884, -1)->Set(17);
  CreateObject(LADR, 3145, 1275, -1)->Set(21);
  CreateObject(LADR, 3325, 1083, -1)->Set(37);

  //Stahlbrücken
  CreateObject(_HBR, 635, 1792, -1)->SwitchMode();
  CreateObject(_HBR, 810, 1792, -1)->SwitchMode();
  CreateObject(_HBR, 1577, 952, -1);
  CreateObject(_HBR, 1955, 922, -1);
  CreateObject(_HBR, 2335, 1832, -1)->SwitchMode();
  CreateObject(_HBR, 3070, 992, -1);
  CreateObject(_HBR, 3141, 922, -1);
  CreateObject(_HBR, 3141, 992, -1);

  //Bodenluken
  CreateObject(HA4K, 230, 473, -1);
  aDoor[01] = CreateObject(HA4K, 280, 743, -1);
  CreateObject(HA4K, 450, 563, -1);
  CreateObject(HA4K, 690, 563, -1);
  aDoor[10] = CreateObject(HA4K, 710, 1013, -1);
  CreateObject(HA4K, 840, 233, -1);
  CreateObject(HA4K, 870, 923, -1);
  CreateObject(HA4K, 1020, 413, -1);
  CreateObject(HA4K, 1020, 1563, -1);
  CreateObject(HA4K, 1050, 233, -1);
  CreateObject(HA4K, 1110, 583, -1);
  aDoor[02] = CreateObject(HA4K, 1110, 713, -1);
  CreateObject(HA4K, 1110, 803, -1);
  CreateObject(HA4K, 1110, 893, -1);
  CreateObject(HA4K, 1130, 1013, -1);
  CreateObject(HA4K, 1240, 293, -1);
  CreateObject(HA4K, 1240, 523, -1);
  CreateObject(HA4K, 1240, 813, -1);
  CreateObject(HA4K, 1240, 1013, -1);
  CreateObject(HA4K, 1320, 1743, -1);
  CreateObject(HA4K, 1460, 943, -1);
  CreateObject(HA4K, 2060, 1783, -1);
  CreateObject(HA4K, 2110, 1783, -1);
  CreateObject(HA4K, 2140, 913, -1);
  CreateObject(HA4K, 2140, 1023, -1);
  CreateObject(HA4K, 2140, 1313, -1);
  aDoor[06] = CreateObject(HA4K, 2730, 1303, -1);
  aDoor[07] = CreateObject(HA4K, 2960, 1303, -1);
  aDoor[08] = CreateObject(HA4K, 3150, 1303, -1);
  CreateObject(HA4K, 2270, 553, -1);
  CreateObject(HA4K, 2270, 1023, -1);
  CreateObject(HA4K, 2400, 553, -1);
  CreateObject(HA4K, 2400, 1233, -1);
  CreateObject(HA4K, 2410, 743, -1);
  CreateObject(HA4K, 2410, 913, -1);
  CreateObject(HA4K, 2880, 273, -1);
  CreateObject(HA4K, 2880, 553, -1);
  CreateObject(HA4K, 2880, 743, -1);
  CreateObject(HA4K, 3130, 743, -1);
  CreateObject(HA4K, 3220, 563, -1);
  aDoor[12] = CreateObject(HA4K, 1020, 1663, -1);

  //Große Bodenluken
  CreateObject(H24K, 325, 928, -1);
  CreateObject(H24K, 405, 498, -1);
  CreateObject(H24K, 575, 498, -1);
  CreateObject(H24K, 745, 1788, -1);
  CreateObject(H24K, 945, 1788, -1);
  CreateObject(H24K, 1565, 1568, -1);
  CreateObject(H24K, 1565, 1748, -1);
  CreateObject(H24K, 2575, 618, -1);
  CreateObject(H24K, 3325, 788, -1);
  CreateObject(H24K, 3325, 918, -1);
  CreateObject(H24K, 3365, 1168, -1)->Lock();
  aDoor[11] = CreateObject(H24K, 3145, 1108, -1);

  //Glasscheiben
  CreateObject(PANE, 712, 490, -1);
  CreateObject(PANE, 732, 810, -1);
  CreateObject(PANE, 732, 860, -1);
  CreateObject(PANE, 732, 1080, -1);
  CreateObject(PANE, 732, 1130, -1);
  CreateObject(PANE, 738, 490, -1);
  CreateObject(PAN2, 745, 300, -1);
  CreateObject(PAN2, 745, 360, -1);
  CreateObject(PAN2, 745, 420, -1);
  CreateObject(PANE, 748, 810, -1);
  CreateObject(PANE, 748, 860, -1);
  CreateObject(PANE, 748, 1080, -1);
  CreateObject(PANE, 748, 1130, -1);
  CreateObject(PANE, 822, 269, -1);
  CreateObject(PANE, 822, 298, -1);
  CreateObject(PANE, 1158, 140, -1);
  CreateObject(PANE, 1212, 140, -1);
  CreateObject(PAN2, 1665, 1896, -1);
  CreateObject(PAN2, 1685, 1896, -1);
  CreateObject(PAN2, 1705, 1896, -1);
  CreateObject(PAN2, 1885, 1896, -1);
  CreateObject(PAN2, 1905, 1896, -1);
  CreateObject(PAN2, 1925, 1896, -1);
  CreateObject(PANE, 2392, 669, -1);
  CreateObject(PANE, 2392, 698, -1);
  CreateObject(PANE, 2392, 779, -1);
  CreateObject(PANE, 2392, 808, -1);
  CreateObject(PANE, 2392, 837, -1);
  CreateObject(PANE, 2392, 866, -1);
  CreateObject(PANE, 2392, 949, -1);
  CreateObject(PANE, 2392, 978, -1);
  CreateObject(PANE, 2392, 1007, -1);
  CreateObject(PANE, 2392, 1036, -1);
  CreateObject(PANE, 2862, 309, -1);
  CreateObject(PANE, 2862, 338, -1);
  CreateObject(PANE, 2862, 367, -1);
  CreateObject(PANE, 2862, 396, -1);
  CreateObject(PANE, 2862, 425, -1);
  CreateObject(PANE, 2862, 454, -1);
  CreateObject(PANE, 2862, 483, -1);
  CreateObject(PANE, 2862, 512, -1);
  CreateObject(PANE, 2862, 589, -1);
  CreateObject(PANE, 2862, 618, -1);
  CreateObject(PANE, 2862, 647, -1);
  CreateObject(PANE, 2862, 676, -1);
  CreateObject(PANE, 2862, 705, -1);
  CreateObject(PANE, 2862, 779, -1);
  CreateObject(PANE, 2862, 808, -1);
  CreateObject(PANE, 2862, 837, -1);

  //Fenster
  var window = CreateObject(WNDW, 1075, 171, -1);
  window->Set();
  window->DoDmg(100);
  window->AddSmokeEffect4K(50,-15,-25,true,this);
  CreateObject(WNDW, 1335, 191, -1)->Set(1);
  CreateObject(WNDW, 1355, 1381, -1)->Set(1);
  CreateObject(WNDW, 1395, 291, -1)->Set(1);
  CreateObject(WNDW, 2185, 1451, -1)->Set();

  //Schutztüren
  CreateObject(GDDR, 350, 1360, -1)->DoDmg(85);
  CreateObject(GDDR, 385, 740, -1)->DoDmg(85);
  CreateObject(GDDR, 385, 830, -1)->DoDmg(85);
  CreateObject(GDDR, 385, 920, -1)->DoDmg(85);
  CreateObject(GDDR, 420, 1360, -1)->DoDmg(85);
  CreateObject(GDDR, 500, 230, -1)->Lock();
  CreateObject(GDDR, 505, 740, -1)->SetColorDw(HSL(0,0, 220));
  CreateObject(GDDR, 505, 830, -1)->SetColorDw(HSL(0,0, 220));
  CreateObject(GDDR, 505, 920, -1)->SetColorDw(HSL(0,0, 220));
  CreateObject(GDDR, 525, 490, -1)->SetColorDw(HSL(80, 150, 100, 127));
  CreateObject(GDDR, 525, 560, -1)->SetColorDw(HSL(80, 150, 100, 127));
  CreateObject(GDDR, 550, 230, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 725, 560, -1)->SetColorDw(HSL(80, 150, 100, 127));
  CreateObject(GDDR, 825, 740, -1)->SetColorDw(HSL(0,0, 220));
  CreateObject(GDDR, 825, 830, -1)->SetColorDw(HSL(0,0, 220));
  CreateObject(GDDR, 825, 920, -1)->SetColorDw(HSL(0,0, 220));
  CreateObject(GDDR, 930, 560, -1)->SetColorDw(HSL(80, 150, 100, 127));
  CreateObject(GDDR, 935, 740, -1)->DoDmg(85);
  CreateObject(GDDR, 935, 830, -1)->DoDmg(85);
  CreateObject(GDDR, 935, 920, -1)->DoDmg(85);
  CreateObject(GDDR, 1150, 280, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1265, 520, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1270, 290, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 2755, 140, -1)->Lock();
  CreateObject(GDDR, 3220, 910, -1)->SetColorDw(HSL(0, 0, 100));
  CreateObject(GDDR, 3220, 1100, -1)->SetColorDw(HSL(0, 0, 100));

  //Gitter
  CreateObject(GTNG, 1135, 710, -1);
  CreateObject(GTNG, 1135, 800, -1);
  CreateObject(GTNG, 1135, 890, -1)->DoDmg(85);
  CreateObject(GTNG, 1270, 230, -1)->DoDmg(85);
  CreateObject(GTNG, 1290, 1280, -1);
  CreateObject(GTNG, 1347, 610, -1);
  CreateObject(GTNG, 1397, 940, -1);
  CreateObject(GTNG, 1595, 1740, -1);
  CreateObject(GTNG, 1627, 1740, -1);
  CreateObject(GTNG, 1993, 1020, -1);
  CreateObject(GTNG, 2183, 610, -1);
  CreateObject(GTNG, 2223, 1230, -1);
  CreateObject(GTNG, 2257, 1780, -1);
  CreateObject(GTNG, 2297, 1230, -1)->DoDmg(85);
  CreateObject(GTNG, 2307, 1020, -1);
  CreateObject(GTNG, 2327, 610, -1)->DoDmg(85);
  CreateObject(GTNG, 2383, 610, -1)->DoDmg(85);
  CreateObject(GTNG, 2455, 1270, -1)->DoDmg(85);
  CreateObject(GTNG, 2635, 1270, -1);
  CreateObject(GTNG, 2777, 550, -1);
  CreateObject(GTNG, 2777, 610, -1);
  CreateObject(GTNG, 2913, 550, -1);
  CreateObject(GTNG, 3143, 780, -1);

  //Dekoschleusen
  CreateObject(GAT1, 1640, 700, -1);
  CreateObject(GAT1, 1890, 700, -1);

  //Paletten
  CreateObject(PLLT, 280, 920, -1);
  CreateObject(PLLT, 1540, 1110, -1);
  CreateObject(PLLT, 1490, 1740, -1);

  //Benzinfässer
  CreateObject(PBRL, 330, 1190, -1)->AutoRespawn();
  CreateObject(PBRL, 430, 1110, -1)->AutoRespawn();
  CreateObject(PBRL, 440, 1110, -1)->AutoRespawn();
  CreateObject(PBRL, 1940, 1110, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 450, 830, -1)->AutoRespawn();
  CreateObject(XBRL, 460, 830, -1)->AutoRespawn();
  CreateObject(XBRL, 880, 1780, -1)->AutoRespawn();
  CreateObject(XBRL, 2685, 1100, -1)->AutoRespawn();
  CreateObject(XBRL, 2730, 270, -1)->AutoRespawn();

  //Giftfässer
  CreateObject(TBRL, 890, 1780, -1)->AutoRespawn();
  CreateObject(TBRL, 320, 1190, -1)->AutoRespawn();
  CreateObject(TBRL, 1200, 1740, -1)->AutoRespawn();
  CreateObject(TBRL, 1210, 1740, -1)->AutoRespawn();
  CreateObject(TBRL, 1500, 1360, -1);
  CreateObject(TBRL, 1500, 1510, -1);
  CreateObject(TBRL, 1890, 1360, -1);
  CreateObject(TBRL, 2420, 1230, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 365, 230, -1);
  CreateObject(GSBL, 365, 435, -1)->SetR(90);
  CreateObject(GSBL, 460, 1110, -1)->AutoRespawn();
  CreateObject(GSBL, 630, 1190, -1)->AutoRespawn();
  CreateObject(GSBL, 680, 230, -1)->AutoRespawn();
  CreateObject(GSBL, 690, 230, -1)->AutoRespawn();
  CreateObject(GSBL, 1480, 1870, -1)->AutoRespawn();
  CreateObject(GSBL, 1950, 1890, -1)->AutoRespawn();
  CreateObject(GSBL, 2570, 270, -1);
  CreateObject(GSBL, 2695, 1100, -1)->AutoRespawn();
  CreateObject(GSBL, 3110, 1100, -1)->AutoRespawn();

  //Explosivtanks
  CreateObject(XTNK, 585, 1190, -1)->AutoRespawn();
  CreateObject(XTNK, 1430, 1740, -1)->AutoRespawn();
  CreateObject(XTNK, 1460, 1660, -1)->AutoRespawn();
  CreateObject(XTNK, 1600, 1110, -1)->AutoRespawn();
  CreateObject(XTNK, 1680, 1110, -1)->AutoRespawn();
  CreateObject(XTNK, 1850, 1110, -1)->AutoRespawn();
  CreateObject(XTNK, 1930, 1110, -1)->AutoRespawn();

  //Container
  CreateObject(CON1, 1160, 1661, -1)->SetPerspective(3);
  CreateObject(CON1, 1160, 1741, -1);
  CreateObject(CON1, 1440, 1871, -1)->SetPerspective(3);
  CreateObject(CON1, 1560, 1661, -1);
  CreateObject(CON1, 1606, 1891, -1)->SetPerspective(2);
  CreateObject(CON1, 1650, 1560, -1)->SetPerspective(2);
  CreateObject(CON1, 1690, 1561, -1);
  CreateObject(CON1, 1800, 1560, -1);
  CreateObject(CON1, 1825, 1533, -1)->SetPerspective(2);
  CreateObject(CON1, 1855, 1561, -1)->SetPerspective(3);
  CreateObject(CON1, 1980, 1890, -1)->SetPerspective(2);
  CreateObject(CON1, 2015, 1100, -1)->SetPerspective(2);
  CreateObject(CON1, 2105, 1100, -1);
  CreateObject(CON1, 2935, 1100, -1);
  CreateObject(CON1, 2935, 1073, -1)->SetPerspective(2);
  CreateObject(CON1, 3316, 1160, -1)->SetPerspective(2);

  //Explosive Kisten
  CreateObject(XWCR, 640, 740, -1)->AutoRespawn();
  CreateObject(XWCR, 970, 410, -1)->AutoRespawn();
  CreateObject(XWCR, 1030, 1780, -1)->AutoRespawn();
  CreateObject(XWCR, 1055, 180, -1);
  CreateObject(XWCR, 1340, 1560, -1)->AutoRespawn();
  CreateObject(XWCR, 1370, 1010, -1)->AutoRespawn();
  CreateObject(XWCR, 1370, 1852, -1);
  CreateObject(XWCR, 1430, 820, -1);
  CreateObject(XWCR, 2000, 1560, -1)->AutoRespawn();
  CreateObject(XWCR, 2090, 910, -1)->AutoRespawn();
  CreateObject(XWCR, 2100, 820, -1);
  CreateObject(XWCR, 3230, 1290, -1)->AutoRespawn();
  CreateObject(XWCR, 2630, 140, -1)->AutoRespawn();

  //Kisten
  CreateObject(WCR2, 270, 1290, -1)->AutoRespawn();
  CreateObject(WCR2, 270, 1430, -1)->AutoRespawn();
  CreateObject(WCR2, 290, 1110, -1)->AutoRespawn();
  CreateObject(WCR2, 290, 1190, -1)->AutoRespawn();
  CreateObject(WCR2, 290, 1430, -1)->AutoRespawn();
  CreateObject(WCR2, 310, 1110, -1)->AutoRespawn();
  CreateObject(WCR2, 310, 1430, -1)->AutoRespawn();
  CreateObject(WCR2, 350, 490, -1)->AutoRespawn();
  CreateObject(WCR2, 650, 1190, -1)->AutoRespawn();
  CreateObject(WCR2, 660, 1172, -1);
  CreateObject(WCR2, 670, 1190, -1)->AutoRespawn();
  CreateObject(WCR2, 1140, 1560, -1);
  CreateObject(WCR2, 1310, 610, -1);
  CreateObject(WCR2, 1305, 810, -1);
  CreateObject(WCR2, 1315, 792, -1);
  CreateObject(WCR2, 1325, 810, -1);
  CreateObject(WCR2, 1340, 1870, -1);
  CreateObject(WCR2, 1360, 1560, -1)->AutoRespawn();
  CreateObject(WCR2, 1360, 1870, -1);
  CreateObject(WCR2, 1380, 1870, -1);
  CreateObject(WCR2, 1390, 1010, -1)->AutoRespawn();
  CreateObject(WCR2, 1410, 820, -1)->AutoRespawn();
  CreateObject(WCR2, 1410, 1010, -1);
  CreateObject(WCR2, 1430, 1010, -1);
  CreateObject(WCR2, 1520, 1660, -1)->AutoRespawn();
  CreateObject(WCR2, 1600, 1310, -1)->AutoRespawn();
  CreateObject(WCR2, 1765, 1840, -1);
  CreateObject(WCR2, 2070, 910, -1)->AutoRespawn();
  CreateObject(WCR2, 2075, 892, -1);
  CreateObject(WCR2, 2110, 1072, -1);
  CreateObject(WCR2, 2120, 820, -1)->AutoRespawn();
  CreateObject(WCR2, 2170, 1050, -1);
  CreateObject(WCR2, 2220, 1560, -1);
  CreateObject(WCR2, 2240, 1523, -1);
  CreateObject(WCR2, 2240, 1542, -1);
  CreateObject(WCR2, 2240, 1560, -1);
  CreateObject(WCR2, 2250, 810, -1);
  CreateObject(WCR2, 2260, 792, -1);
  CreateObject(WCR2, 2260, 1523, -1);
  CreateObject(WCR2, 2260, 1542, -1);
  CreateObject(WCR2, 2260, 1560, -1);
  CreateObject(WCR2, 2270, 810, -1);
  CreateObject(WCR2, 2285, 610, -1);
  CreateObject(WCR2, 2896, 1064, -1);
  CreateObject(WCR2, 2896, 1082, -1);
  CreateObject(WCR2, 3040, 1064, -1);
  CreateObject(WCR2, 3058, 1064, -1);
  CreateObject(WCR2, 3058, 1082, -1);
  CreateObject(WCR2, 3058, 1100, -1);
  CreateObject(WCR2, 3076, 1064, -1);
  CreateObject(WCR2, 3076, 1082, -1);
  CreateObject(WCR2, 3076, 1100, -1);
  CreateObject(WCR2, 3094, 1064, -1);
  CreateObject(WCR2, 3094, 1082, -1);
  CreateObject(WCR2, 3210, 1290, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 300, 830, -1)->AutoRespawn();
  CreateObject(MWCR, 890, 920, -1)->AutoRespawn();
  CreateObject(MWCR, 2405, 1100, -1)->AutoRespawn();
  CreateObject(MWCR, 2730, 1100, -1)->AutoRespawn();
  CreateObject(MWCR, 2896, 1100, -1);
  CreateObject(MWCR, 3040, 1082, -1);
  CreateObject(MWCR, 3040, 1100, -1);
  CreateObject(MWCR, 3094, 1100, -1);
  CreateObject(MWCR, 3275, 910, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 350, 920, -1)->AutoRespawn();
  CreateObject(BECR, 510, 1360, -1)->AutoRespawn();
  CreateObject(BECR, 980, 1870, -1)->AutoRespawn();
  CreateObject(BECR, 1200, 1660, -1)->AutoRespawn();
  CreateObject(BECR, 1360, 1110, -1)->AutoRespawn();
  CreateObject(BECR, 1600, 1410, -1)->AutoRespawn();
  CreateObject(BECR, 1865, 1840, -1);
  CreateObject(BECR, 2070, 1560, -1)->AutoRespawn();
  CreateObject(BECR, 2090, 1072, -1);
  CreateObject(BECR, 2750, 910, -1)->AutoRespawn();
  CreateObject(BECR, 3100, 980, -1);
  CreateObject(BECR, 3480, 910, -1)->AutoRespawn();

  //Gerüste
  CreateObject(SFFG, 860, 510, -1)->Set(5);
  CreateObject(SFFG, 860, 560, -1)->Set(5);
  CreateObject(SFFG, 880, 1780, -1)->Set(5);
  CreateObject(SFFG, 940, 1780, -1)->Set(3);
  CreateObject(SFFG, 1250, 1870, -1)->Set(5);
  CreateObject(SFFG, 1280, 1690, -1)->Set(3);
  CreateObject(SFFG, 1280, 1740, -1)->Set(4);
  CreateObject(SFFG, 1360, 1690, -1)->Set(4);
  CreateObject(SFFG, 1360, 1740, -1)->Set(2);
  CreateObject(SFFG, 1480, 1360, -1)->Set(3);
  CreateObject(SFFG, 1480, 1410, -1)->Set(2);
  CreateObject(SFFG, 1480, 1460, -1)->Set(3);
  CreateObject(SFFG, 1480, 1510, -1)->Set(2);
  CreateObject(SFFG, 1480, 1560, -1)->Set(3);
  CreateObject(SFFG, 1630, 1360, -1)->Set(4);
  CreateObject(SFFG, 1630, 1410, -1)->Set(5);
  CreateObject(SFFG, 1630, 1460, -1)->Set(4);
  CreateObject(SFFG, 1630, 1510, -1);
  CreateObject(SFFG, 1630, 1560, -1)->Set(5);
  CreateObject(SFFG, 1640, 1010, -1)->Set(5);
  CreateObject(SFFG, 1640, 1060, -1)->Set(4);
  CreateObject(SFFG, 1640, 1110, -1)->Set(5);
  CreateObject(SFFG, 1710, 1410, -1)->Set(4);
  CreateObject(SFFG, 1710, 1460, -1)->Set(2);
  CreateObject(SFFG, 1710, 1510, -1)->Set(4);
  CreateObject(SFFG, 1710, 1560, -1)->Set(5);
  CreateObject(SFFG, 1735, 1790, -1)->Set(4);
  CreateObject(SFFG, 1735, 1840, -1)->Set(5);
  CreateObject(SFFG, 1735, 1890, -1);
  CreateObject(SFFG, 1765, 1010, -1)->Set(2);
  CreateObject(SFFG, 1765, 1060, -1);
  CreateObject(SFFG, 1765, 1110, -1)->Set(3);
  CreateObject(SFFG, 1790, 1410, -1)->Set(5);
  CreateObject(SFFG, 1790, 1460, -1)->Set(5);
  CreateObject(SFFG, 1790, 1510, -1)->Set(5);
  CreateObject(SFFG, 1790, 1560, -1)->Set(5);
  CreateObject(SFFG, 1795, 1840, -1)->Set(5);
  CreateObject(SFFG, 1855, 1790, -1)->Set(4);
  CreateObject(SFFG, 1855, 1840, -1)->Set(4);
  CreateObject(SFFG, 1855, 1890, -1);
  CreateObject(SFFG, 1870, 1410, -1);
  CreateObject(SFFG, 1870, 1460, -1)->Set(3);
  CreateObject(SFFG, 1870, 1510, -1)->Set(4);
  CreateObject(SFFG, 1870, 1560, -1)->Set(5);
  CreateObject(SFFG, 1890, 1010, -1)->Set(5);
  CreateObject(SFFG, 1890, 1060, -1)->Set(4);
  CreateObject(SFFG, 1890, 1110, -1)->Set(5);
  CreateObject(SFFG, 2060, 1890, -1);
  CreateObject(SFFG, 2080, 1100, -1)->Set(5);
  CreateObject(SFFG, 2140, 1100, -1)->Set(5);
  CreateObject(SFFG, 2200, 1100, -1)->Set(4);
  CreateObject(SFFG, 2260, 1100, -1)->Set(4);
  CreateObject(SFFG, 2780, 1100, -1)->Set(5);
  CreateObject(SFFG, 2840, 1100, -1)->Set(5);

  //Sandsackbarrieren
  CreateObject(SBBA, 600, 230, -1)->Right();
  CreateObject(SBBA, 620, 1010, -1)->Right();
  CreateObject(SBBA, 700, 230, -1)->Right();
  CreateObject(SBBA, 945, 1010, -1);
  CreateObject(SBBA, 1000, 1010, -1)->Right();
  CreateObject(SBBA, 1725, 1110, -1);
  CreateObject(SBBA, 1731, 1890, -1);
  CreateObject(SBBA, 1805, 1110, -1)->Right();
  CreateObject(SBBA, 1830, 1740, -1);
  CreateObject(SBBA, 1860, 1890, -1)->Right();
  CreateObject(SBBA, 2285, 1100, -1)->Right();
  CreateObject(SBBA, 2300, 910, -1)->Right();
  CreateObject(SBBA, 2315, 1560, -1);
  CreateObject(SBBA, 2680, 140, -1)->Right();
  CreateObject(SBBA, 2750, 270, -1)->Right();
  CreateObject(SBBA, 2575, 1100, -1);
  CreateObject(SBBA, 2585, 910, -1);

  //Hinweisschilder
  var sign = CreateObject(SNPT, 985, 180, -1);
  sign->SetAction("Sign6");
  sign->Light();
  var sign = CreateObject(SNPT, 2700, 140, -1);
  sign->SetAction("Sign6");
  sign->Light();

  //Schilder
  CreateObject(WLSN, 785, 225, -1)->Set("Heights");
  CreateObject(WLSN, 1065, 1375, -1)->Set("Heights");
  CreateObject(WLSN, 1185, 225, -1)->Set("Heights");
  CreateObject(WLSN, 2355, 735, -1)->Set("Heights");
  CreateObject(WLSN, 2825, 140, -1)->Set("Heights");

  //Wald
  CreateObject(TRE1, 585, 105,-1);
  CreateObject(GRAS, 575, 100, -1);
  CreateObject(GRAS, 590, 100, -1);
  CreateObject(TRE1, 2300, 380,-1)->SetR(-10);
  CreateObject(GRAS, 2295, 380, -1);
  CreateObject(GRAS, 2310, 370, -1);
  CreateObject(GRAS, 2325, 360, -1);
  CreateObject(TRE1, 2990, 105,-1)->SetR(10);
  CreateObject(GRAS, 2970, 80, -1);
  CreateObject(GRAS, 2980, 80, -1);
  CreateObject(GRAS, 2990, 80, -1);

  //Steine
  CreateObject(STNE, 280, 70, -1);
  CreateObject(STNE, 650, 90, -1);
  CreateObject(STNE, 690, 90, -1);
  CreateObject(STNE, 1510, 510, -1);
  CreateObject(STNE, 2035, 500, -1);
  CreateObject(STNE, 2480, 170, -1);
  CreateObject(STNE, 3100, 60, -1);
  CreateObject(STNE, 3250, 70, -1);

  //Glastische
  CreateObject(GTBL, 340, 740, -1);
  CreateObject(GTBL, 460, 490, -1);
  CreateObject(GTBL, 420, 830, -1);
  CreateObject(GTBL, 465, 740, -1);
  CreateObject(GTBL, 610, 920, -1);
  CreateObject(GTBL, 640, 490, -1);
  CreateObject(GTBL, 690, 920, -1);
  CreateObject(GTBL, 900, 830, -1);
  CreateObject(GTBL, 2640, 910, -1);
  CreateObject(GTBL, 2700, 910, -1);
  CreateObject(GTBL, 3370, 910, -1);

  //Labortisch
  CreateObject(LTBL, 1110, 280, -1);

  //Monitore
  CreateObject(MNI2, 310, 1412, -1);
  CreateObject(MNI2, 478, 728, -1);
  CreateObject(MNI2, 905, 818, -1);
  CreateObject(MNI2, 1100, 268, -1);
  CreateObject(MNI2, 1120, 268, -1);
  CreateObject(MNI2, 1337, 1852, -1);
  CreateObject(MNI2, 2635, 898, -1);
  CreateObject(MNI2, 2650, 898, -1);
  CreateObject(MNI2, 2695, 898, -1);

  //Lampen
  CreateObject(CLGH,315,685,-1);
  CreateObject(CLGH,315,775,-1);
  CreateObject(CLGH,315,865,-1);
  CreateObject(LLGH,380,1760,-1);
  CreateObject(CLGH,430,445,-1);
  CreateObject(CLGH,430,505,-1);
  CreateObject(BLGH,580,1140,-1);
  CreateObject(CLGH,630,445,-1);
  CreateObject(CLGH,630,505,-1);
  CreateObject(LLGH,780,1760,-1);
  CreateObject(LLGH,880,1760,-1);
  CreateObject(LLGH,980,1760,-1);
  CreateObject(BLGH,1020,530,-1);
  CreateObject(CLGH,1020,685,-1);
  CreateObject(CLGH,1020,775,-1);
  CreateObject(CLGH,1020,865,-1);
  CreateObject(LLGH,1000,1850,-1);
  CreateObject(STLH,1130,95,-1);
  CreateObject(CLGH,1185,845,-1);
  CreateObject(LLGH,1320,1675,-1);
  CreateObject(LLGH,1200,1850,-1);
  CreateObject(STLH,1235,205,-1);
  CreateObject(STLH,1255,115,-1);
  CreateObject(STLH,1320,215,-1);
  CreateObject(LBGH,1335,1200, -1);
  CreateObject(LLGH,1415,1075,-1);
  CreateObject(LBGH,2120,1200, -1);
  CreateObject(LLGH,2140,1075,-1);
  CreateObject(LLGH,2470,1075,-1);
  CreateObject(LLGH,2590,210,-1);
  CreateObject(CLGH,2680,1335,-1);
  CreateObject(STLH,2730,1257,-1);
  CreateObject(LLGH,2880,210,-1);
  CreateObject(CLGH,2930,1335,-1);
  CreateObject(STLH,2960,1257,-1);
  CreateObject(BLGH,3020,890,-1);
  CreateObject(BLGH,3020,1050,-1);
  CreateObject(STLH,3150,1257,-1);
  CreateObject(CLGH,3175,1335,-1);
  CreateObject(BLGH,2270,1380,-1);
  CreateObject(CLGH,3420,865,-1);

  //Generator
  var generator = CreateObject(GNRT, 1795, 1890, -1);
  generator->SetCategory(C4D_StaticBack);
  generator->TurnOn();

  //Verbundene Räume
  aDoorWay[22] = CreateObject(GAT3, 250, 465, -1);
  aDoorWay[23] = CreateObject(GAT3, 300, 735, -1);
  aDoorWay[22]->Connect(aDoorWay[23]);
  aDoorWay[00] = CreateObject(GAT3, 380, 1005, -1);
  aDoorWay[01] = CreateObject(GAT3, 480, 1105, -1);
  aDoorWay[00]->Connect(aDoorWay[01]);
  aDoorWay[02] = CreateObject(GAT3, 660, 639, -1);
  aDoorWay[02]->SetClrModulation(RGB(125,125,205));
  aDoorWay[03] = CreateObject(GAT3, 840, 639, -1);
  aDoorWay[03]->SetClrModulation(RGB(125,125,205));
  aDoorWay[02]->Connect(aDoorWay[03]);
  aDoorWay[04] = CreateObject(ROM2, 4575, 900, -1);
  aDoorWay[05] = CreateObject(ROM2, 4530, 790, -1);
  aDoorWay[04]->Connect(aDoorWay[05]);
  aDoorWay[06] = CreateObject(GAT1, 4575, 525, -1);
  aDoorWay[07] = CreateObject(ROOM, 5390, 940, -1);
  aDoorWay[06]->Connect(aDoorWay[07]);
  aDoorWay[08] = CreateObject(GAT3, 950, 1180, -1);
  aDoorWay[08]->SetClrModulation(RGB(125,125,205));
  aDoorWay[09] = CreateObject(GAT3, 972, 1005, -1);
  aDoorWay[08]->Connect(aDoorWay[09]);
  aDoorWay[10] = CreateObject(GAT3, 955, 1395, -1);
  aDoorWay[10]->SetClrModulation(RGB(125,125,205));
  aDoorWay[11] = CreateObject(GAT3, 2460, 1525, -1);
  aDoorWay[11]->SetClrModulation(RGB(125,125,205));
  aDoorWay[10]->Connect(aDoorWay[11]);
  aDoorWay[12] = CreateObject(GAT3, 1125, 1375, -1);
  aDoorWay[13] = CreateObject(GAT3, 1300, 1555, -1);
  aDoorWay[12]->Connect(aDoorWay[13]);
  aDoorWay[14] = CreateObject(GAT3, 2390, 1445, -1);
  aDoorWay[15] = CreateObject(GAT3, 2680, 1285, -1);
  aDoorWay[14]->Connect(aDoorWay[15]);
  aDoorWay[16] = CreateObject(GAT3, 2320, 1445, -1);
  aDoorWay[17] = CreateObject(GAT3, 2345, 1555, -1);
  aDoorWay[16]->Connect(aDoorWay[17]);
  aDoorWay[18] = CreateObject(GAT3, 880, 669, -1);
  aDoorWay[18]->SetClrModulation(RGB(125,125,205));
  aDoorWay[19] = CreateObject(GAT3, 880, 1041, -1);
  aDoorWay[19]->SetClrModulation(RGB(125,125,205));
  aDoorWay[18]->Connect(aDoorWay[19]);
  aDoorWay[24] = CreateObject(GAT1, 1420, 455, -1);
  aDoorWay[25] = CreateObject(GAT3, 1325, 609, -1);
  aDoorWay[24]->Connect(aDoorWay[25]);
  aDoorWay[26] = CreateObject(GAT1, 2140, 455, -1);
  aDoorWay[27] = CreateObject(GAT3, 2225, 609, -1);
  aDoorWay[26]->Connect(aDoorWay[27]);
  aDoorWay[28] = CreateObject(GAT1, 860, 1740, -1);
  aDoorWay[29] = CreateObject(GAT3, 2390, 1815, -1);
  aDoorWay[28]->Connect(aDoorWay[29]);
  aDoorWay[33] = CreateObject(GAT1, 1795, 1610, -1);
  aDoorWay[34] = CreateObject(GAT3, 2085, 1779, -1);
  aDoorWay[33]->Connect(aDoorWay[34]);

  //Stahltore und Konsolen
  aDoorWay[39] = CreateObject(HNG4, 1440, 540, -1);
  aDoorWay[40] = CreateObject(HNG4, 2080, 540, -1);
  var autod = CreateObject(HNG4, 1450, 850, -1);
  CreateObject(CONS, 1500, 935, -1)->Set(autod);
  autod = CreateObject(HNG4, 2080, 850, -1);
  CreateObject(CONS, 2020, 905, -1)->Set(autod);
  aDoorWay[41] = CreateObject(HNG4, 2990, 520, -1);

  //Kräne
  CreateObject(CRN1, 1770, 620, -1)->Set(53);
  CreateObject(CRN1, 2925, 960, -1)->Set(40);

  //Selbstschussanlagen
  aSelfDefense[0] = CreateObject(SEGU, 1765, 879, -1);
  aSelfDefense[0]->Arm(MISA);
  aSelfDefense[0]->SetAutoRepair(1500);

  aSelfDefense[1] = CreateObject(SEGU, 2658, 1010, -1);
  aSelfDefense[1]->SetR(-90);
  aSelfDefense[1]->Arm(MISA);
  aSelfDefense[1]->SetAutoRepair(1500);

  //Soundkulisse

  //Lava
  CreateObject(SE4K, 400, 2030, -1)->Set("Lava*",1200,300, 75);
  CreateObject(SE4K, 1100, 2030, -1)->Set("Lava*",1200,300, 75);
  CreateObject(SE4K, 1800, 2030, -1)->Set("Lava*",1200,300, 75);
  CreateObject(SE4K, 2900, 2030, -1)->Set("Lava*",1200,300, 75);

  //Erdrutsche
  CreateObject(SE4K, 480, 1600, -1)->Set("FallingDirt*.ogg",800,200, 75);
  CreateObject(SE4K, 1240, 1900, -1)->Set("FallingDirt*.ogg",800,200, 75);
  CreateObject(SE4K, 2830, 1870, -1)->Set("FallingDirt*.ogg",800,200, 75);

  //Wind
  CreateObject(SE4K, 1860, 200, -1)->Set("WindSound*.ogg",1000,200, 75);

  //Innenbereich
  CreateObject(SE4K, 580, 600, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 600, 1100, -1)->Set("InteriorMetal*.ogg",1000,300, 25,50);
  CreateObject(SE4K, 620, 1100, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 1230, 220, -1)->Set("InteriorStress*.ogg",700,100, 75);
  CreateObject(SE4K, 1770, 700, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 1770, 1000, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 1800, 1400, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 2015, 1370, -1)->Set("InteriorMetal*.ogg",1000,300, 25,50);
  CreateObject(SE4K, 2380, 850, -1)->Set("InteriorStress*.ogg",700,100, 75);
  CreateObject(SE4K, 3740, 210, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 3080, 1170, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 3380, 980, -1)->Set("InteriorMetal*.ogg",1000,300, 25,50);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Dragninautomaten
  CreateObject(DGND, 1000, 1780, -1);
  CreateObject(DGND, 2190, 1820, -1);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Leitkegel
  CreateObject(TCCN, 295, 1780, -1);
  CreateObject(TCCN, 385, 1780, -1);
  CreateObject(TCCN, 1125, 1870, -1);
  CreateObject(TCCN, 1215, 1870, -1);
  CreateObject(TCCN, 1575, 1110, -1);
  CreateObject(TCCN, 1705, 1110, -1);
  CreateObject(TCCN, 1720, 820, -1);
  CreateObject(TCCN, 1810, 820, -1);
  CreateObject(TCCN, 1825, 1110, -1);
  CreateObject(TCCN, 1705, 1110, -1);
  CreateObject(TCCN, 2405, 1820, -1);
  CreateObject(TCCN, 2465, 1820, -1);
  CreateObject(TCCN, 3345, 1160, -1);

  //Topfpflanzen
  CreateObject(PLT4, 370, 490, -1);
  CreateObject(PLT4, 385, 490, -1);
  CreateObject(PLT4, 375, 560, -1);
  CreateObject(PLT4, 390, 560, -1);
  CreateObject(PLT4, 405, 560, -1);
  CreateObject(PLT4, 420, 818, -1);
  CreateObject(PLT3, 470, 560, -1);
  CreateObject(PLT3, 490, 560, -1);
  CreateObject(PLT4, 450, 478, -1);
  CreateObject(PLT4, 465, 478, -1);
  CreateObject(PLT3, 555, 490, -1);
  CreateObject(PLT4, 630, 478, -1);
  CreateObject(PLT4, 645, 478, -1);
  CreateObject(PLT4, 645, 560, -1);
  CreateObject(PLT4, 653, 478, -1);
  CreateObject(PLT4, 660, 560, -1);
  CreateObject(PLT4, 675, 560, -1);
  CreateObject(PLT3, 680, 490, -1);
  CreateObject(PLT3, 700, 490, -1);
  CreateObject(PLT3, 720, 920, -1);
  CreateObject(PLT3, 850, 830, -1);
  CreateObject(PLT3, 905, 560, -1);
  CreateObject(PLT3, 2860, 270, -1);

  //Geländer
  CreateObject(RAI1, 355, 490, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 555, 490, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 355, 560, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 555, 560, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 900, 180, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 945, 560, -1)->SetRail([1,3,1]);
  CreateObject(RAI1, 1210, 520, -1)->SetRail([1,3,1]);
  CreateObject(RAI1, 1364, 1110, -1)->SetRail([1,3,1,1,1,3,1]);
  var rail = CreateObject(RAI1, 1448, 720, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 67; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 1714, 820, -1)->SetRail([1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2108, 1310, -1)->SetRail([1,3,1,3,1]);
  CreateObject(RAI1, 2440, 740, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2655, 270, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,2,2,2,3,1]);
  CreateObject(RAI1, 2750, 100, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2910, 740, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);

  //Radare
  CreateObject(RADR, 1255, 70, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(RADR, 2645, 110, -1)->SetClrModulation(RGB(125,125,125));

  //Bildschirme
  CreateObject(SCR3, 485, 545, -1);
  CreateObject(SCA2, 600, 890, -1);
  CreateObject(SCR2, 660, 545, -1);
  var screen = CreateObject(SCR3, 870, 485, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen->SetPhase(6);
  CreateObject(SCA2, 690, 890, -1)->SetAction("Clonk");
  CreateObject(SCR3, 870, 505, -1)->SetClrModulation(RGB(250,10,10));
  CreateObject(SCR1, 870, 545, -1);
  CreateObject(SCR2, 1000, 1550, -1);
  CreateObject(SCR1, 1135, 160, -1);
  CreateObject(SCR3, 1272, 160, -1);
  CreateObject(SCA2, 1280, 1335, -1);
  CreateObject(SCR3, 1320, 250, -1)->SetClrModulation(RGB(255,255,255));
  CreateObject(SCR3, 1430, 930, -1);
  CreateObject(SCR3, 2220, 900, -1)->SetClrModulation(RGB(250,10,10));
  screen = CreateObject(SCR3, 2260, 900, -1);
  screen->SetClrModulation(RGB(250,10,10));
  screen->SetPhase(6);
  CreateObject(SCA2, 2270, 1430, -1);
  screen = CreateObject(SCR3, 2300, 900, -1);
  screen->SetClrModulation(RGB(250,10,10));
  screen->SetPhase(3);
  screen = CreateObject(SCR3, 2410, 900, -1);
  screen->SetClrModulation(RGB(250,10,10));
  screen->SetPhase(12);
  screen = CreateObject(SCR3, 2450, 900, -1);
  screen->SetClrModulation(RGB(250,10,10));
  screen->SetPhase(9);
  screen = CreateObject(SCR3, 2490, 900, -1);
  screen->SetClrModulation(RGB(250,10,10));
  screen->SetPhase(18);
  screen = CreateObject(SCR3, 2530, 900, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen->SetPhase(15);
  screen = CreateObject(SCR3, 2570, 900, -1);
  screen->SetClrModulation(RGB(250,10,10));
  screen->SetPhase(6);
  CreateObject(SCA2, 2640, 1080, -1);
  screen = CreateObject(SCR3, 2625, 200, -1);
  screen->SetClrModulation(RGB(250,10,10));
  screen->SetPhase(3);
  screen = CreateObject(SCR3, 2625, 221, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen->SetPhase(12);
  screen = CreateObject(SCR3, 2695, 200, -1);
  screen->SetClrModulation(RGB(250,10,10));
  screen->SetPhase(9);
  screen = CreateObject(SCR3, 2695, 221, -1);
  screen->SetClrModulation(RGB(250,10,10));
  screen->SetPhase(18);
  screen = CreateObject(SCR3, 2765, 200, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen->SetPhase(15);
  CreateObject(SCR3, 2765, 221, -1)->SetClrModulation(RGB(250,10,10));

  //Waschbecken
  CreateObject(WSHB, 460, 920, -1);
  CreateObject(WSHB, 480, 920, -1);
  CreateObject(WSHB, 1045, 740, -1);

  //Toilettenkabinen
  CreateObject(TOIC, 410, 920, -1);
  CreateObject(TOIC, 430, 920, -1);
  CreateObject(TOIC, 1000, 740, -1);
  CreateObject(TOIC, 1020, 740, -1);

  //Spinde
  CreateObject(LCKR, 360, 560, -1);
  CreateObject(LCKR, 410, 740, -1);
  CreateObject(LCKR, 425, 740, -1);
  CreateObject(LCKR, 980, 830, -1);
  CreateObject(LCKR, 1050, 580, -1);
  CreateObject(LCKR, 1050, 410, -1);
  CreateObject(LCKR, 1060, 740, -1);
  CreateObject(LCKR, 1065, 580, -1);
  CreateObject(LCKR, 1065, 410, -1);
  CreateObject(LCKR, 1080, 580, -1);
  CreateObject(LCKR, 1085, 410, -1);
  CreateObject(LCKR, 1127, 580, -1);
  CreateObject(LCKR, 1150, 580, -1)->SetR(-35);
  CreateObject(LCKR, 1242, 1380, -1)->SetR(35);
  CreateObject(LCKR, 2280, 1450, -1)->SetR(35);
  CreateObject(LCKR, 2300, 1450, -1);

  //Straßenlichter
  CreateObject(SLGH, 1135, 80, -1);
  CreateObject(SLGH, 1255, 100, -1);
  CreateObject(SLGH, 1380, 1560, -1);
  CreateObject(SLGH, 1440, 820, -1);
  CreateObject(SLGH, 1600, 1560, -1);
  CreateObject(SLGH, 1640, 1110, -1);
  CreateObject(SLGH, 1890, 1110, -1);
  CreateObject(SLGH, 1960, 1560, -1);
  CreateObject(SLGH, 1550, 510, -1);
  CreateObject(SLGH, 1970, 510, -1);
  CreateObject(SLGH, 2090, 820, -1);
  CreateObject(SLGH, 2760, 100, -1);

  //Orientierungslichter
  CreateObject(OLGH, 2660, 140, -1)->Set(8, 10, 1, 1, 15);

  //Notausgangslichter
  CreateObject(ETLT, 580, 205, -1);
  CreateObject(ETLT, 1220, 545, -1);
  CreateObject(ETLT, 2780, 125, -1);

  //Anlagen
  CreateObject(CCP2, 1135, 170, -1);
  CreateObject(CCP1, 1280, 190, -1);
  CreateObject(CGLO, 1320, 290, -1);
  CreateObject(CCP1, 1385, 820, -1);
  CreateObject(CCP1, 2235, 1450, -1);

  //Papierstapel
  CreateObject(PPSK, 260, 740, -1);
  CreateObject(PPSK, 325, 727, -1);
  CreateObject(PPSK, 335, 727, -1);
  CreateObject(PPSK, 350, 727, -1);
  CreateObject(PPSK, 465, 727, -1);
  CreateObject(PPSK, 885, 902, -1);
  CreateObject(PPSK, 890, 817, -1);
  CreateObject(PPSK, 980, 740, -1);
  CreateObject(PPSK, 1155, 170, -1);
  CreateObject(PPSK, 1220, 290, -1);
  CreateObject(PPSK, 1230, 290, -1);
  CreateObject(PPSK, 1285, 290, -1);
  CreateObject(PPSK, 1310, 1380, -1);
  CreateObject(PPSK, 1350, 290, -1);
  CreateObject(PPSK, 2255, 1450, -1);
  CreateObject(PPSK, 3355, 897, -1);
  CreateObject(PPSK, 3370, 897, -1);

  //Regale
  CreateObject(FRAM, 262, 740, -1);
  CreateObject(FRAM, 262, 830, -1);
  CreateObject(FRAM, 840, 920, -1);
  CreateObject(FRAM, 860, 920, -1);
  CreateObject(FRAM, 960, 740, -1);
  CreateObject(FRAM, 1035, 920, -1);
  CreateObject(FRAM, 1090, 220, -1);
  CreateObject(FRAM, 1120, 220, -1);
  CreateObject(FRAM, 1125, 270, -1);
  CreateObject(FRAM, 1150, 220, -1);
  CreateObject(FRAM, 1240, 260, -1);
  CreateObject(FRAM, 1240, 280, -1);
  CreateObject(FRAM, 2620, 250, -1);
  CreateObject(FRAM, 2650, 250, -1);
  CreateObject(FRAM, 1290, 1365, -1);
  CreateObject(FRAM, 1290, 1380, -1);

  //Betten
  CreateObject(BED2, 865, 740, -1);
  CreateObject(BED2, 900, 740, -1);
  CreateObject(BED2, 1000, 920, -1);
  CreateObject(BED2, 1020, 830, -1);

  //Automaten
  CreateObject(VGMN, 650, 920, -1);
  CreateObject(VGMN, 1140, 230, -1)->Set(2);
  CreateObject(VGMN, 1270, 1380, -1)->Set(3);
  CreateObject(VGMN, 2590, 270, -1)->Set(3);

  //Ventillatoren
  CreateObject(MVNT, 560, 830, -1)->SetCon(65);
  CreateObject(MVNT, 665, 1130, -1)->SetCon(80);
  CreateObject(MVNT, 690, 830, -1)->SetCon(65);
  CreateObject(MVNT, 875, 830, -1)->SetCon(95);
  CreateObject(VENT, 1210, 930, -1)->SetCon(60);
  CreateObject(VENT, 1490, 700, -1)->SetCon(70);
  CreateObject(VENT, 1550, 1420, -1)->SetClrModulation(RGB(210,210,210));
  CreateObject(VENT, 1690, 1690, -1)->SetCon(62);
  CreateObject(VENT, 1900, 1690, -1)->SetCon(62);
  CreateObject(VENT, 2010, 1420, -1)->SetClrModulation(RGB(210,210,210));
  CreateObject(VENT, 2070, 700, -1)->SetCon(70);
  CreateObject(VENT, 2580, 550, -1)->SetCon(30);
  CreateObject(VENT, 2790, 1080, -1)->SetClrModulation(RGB(150,150,200));
  CreateObject(VENT, 3120, 1080, -1)->SetClrModulation(RGB(150,150,200));
  CreateObject(VENT, 3220, 560, -1)->SetCon(40);
  var vent = CreateObject(VENT, 3370, 905, -1);
  vent->SetClrModulation(RGB(150,150,200));
  vent->SetCon(30);
  vent = CreateObject(VENT, 3460, 905, -1);
  vent->SetClrModulation(RGB(150,150,200));
  vent->SetCon(30);

  //Glasröhren
  CreateObject(GLST, 2760, 1100, -1);
  CreateObject(GLST, 2790, 1100, -1);

  //Schilder
  CreateObject(HSGN, 1415, 1170, -1)->SetCon(50);
  CreateObject(ESGN, 1140, 405, -1);
  CreateObject(MSGN, 1515, 810, -1);
  CreateObject(MSGN, 2015, 810, -1);
  CreateObject(ESGN, 2110, 905, -1);

  //Feuerlöscher
  CreateObject(FIE2, 1100, 405, -1);
  CreateObject(FIE2, 1110, 405, -1);
  CreateObject(FIE2, 1120, 405, -1);
  CreateObject(FIE2, 2130, 905, -1);
  CreateObject(FIE2, 2140, 905, -1);
  CreateObject(FIE2, 2150, 905, -1);

  //Poster
  CreateObject(PSTR, 670, 230, -1)->Set(2,1);
  CreateObject(PSTR, 900, 230, -1)->Set(2,2);
  CreateObject(PSTR, 900, 720, -1)->Set(0,1);
  CreateObject(PSTR, 1000, 230, -1)->Set(2,3);
  CreateObject(PSTR, 1020, 900, -1)->Set(0,2);
  CreateObject(PSTR, 1375, 790, -1)->Set(3,1);
  CreateObject(PSTR, 1570, 1000, -1)->Set(3,0);
  CreateObject(PSTR, 2045, 792, -1)->Set(3,2);

  //Apparaturen
  CreateObject(GADG, 720, 490, -1)->Set(1);
  CreateObject(GADG, 740, 810, -1)->Set(1);
  CreateObject(GADG, 740, 860, -1)->Set(1);
  CreateObject(GADG, 740, 1080, -1)->Set(1);
  CreateObject(GADG, 740, 1130, -1)->Set(1);
  CreateObject(GADG, 730, 490, -1)->Set(1);

  //Zäune
  CreateObject(FENC, 1550, 510, -1);
  CreateObject(FENC, 1980, 510, -1);

  //Satellitenschüssel
  CreateObject(SADH, 2150, 480, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 480, 1600, -1);
  CreateObject(ENGT, 480, 1700, -1);
  CreateObject(ENGT, 620, 230, -1)->SetCon(98);
  CreateObject(ENGT, 720, 230, -1)->SetCon(98);
  CreateObject(ENGT, 730, 640, -1);
  CreateObject(ENGT, 810, 1750, -1);
  CreateObject(ENGT, 810, 1850, -1);
  CreateObject(ENGT, 850, 230, -1)->SetCon(98);
  CreateObject(ENGT, 950, 230, -1)->SetCon(98);
  CreateObject(ENGT, 1050, 230, -1)->SetCon(98);
  CreateObject(ENGT, 1235, 360, -1);
  CreateObject(ENGT, 1235, 410, -1);
  CreateObject(ENGT, 1235, 460, -1);
  CreateObject(ENGT, 1235, 510, -1);
  CreateObject(ENGT, 1640, 1890, -1);
  CreateObject(ENGT, 1645, 955, -1);
  CreateObject(ENGT, 1660, 1740, -1);
  CreateObject(ENGT, 1785, 955, -1);
  CreateObject(ENGT, 1925, 955, -1);
  CreateObject(ENGT, 1930, 1740, -1);
  CreateObject(ENGT, 1950, 1890, -1);
  CreateObject(ENGT, 2090, 1085, -1);
  CreateObject(ENGT, 2190, 1085, -1);
  CreateObject(ENGT, 2290, 1085, -1);
  CreateObject(ENGT, 2420, 1085, -1);
  CreateObject(ENGT, 2520, 1085, -1);

  //Scheinwerfer
  CreateObject(FLGH, 1630, 1890, -1)->SetRotation(45);
  CreateObject(FLGH, 1960, 1890, -1)->SetRotation(-45);
  CreateObject(FLGH, 3195, 1100, -1)->SetRotation(-45);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD03,700,400,-1)->SetClrModulation(RGB(150,120,100));
  CreateObject(BD03,2200,600,-1)->SetClrModulation(RGB(150,120,100));
}

/* Generatorereignisse */

func PowerSupplyOn()
{
  //Lichter einschalten
  for(var light in FindObjects(Find_Func("IsLamp"),Find_Not(Find_ID(FLHH))))
    light->~TurnOn();

  //Dunkelheit verringern
  if(GetDarkness() >= 5)
    SetDarkness(0);

  return 1;
}

func PowerSupplyOff()
{
  //Lichter ausschalten
  for(var light in FindObjects(Find_Func("IsLamp"),Find_Not(Find_ID(FLHH))))
    light->~TurnOff();

  //Dunkelheit erhöhen
  if(GetDarkness() < 5)
    SetDarkness(5);

  return 1;
}

/* Bei Relaunch */

public func OnClassSelection(object pClonk, int iClass)
{
  //AS-Spielziel
  if(FindObject(GASS))
    if(GetPlayerTeam(GetOwner(pClonk)) == 1)
      if(GetAssaultTarget(0,1))
        AddEffect("SpawnParachute", pClonk, 1, 10);
  return _inherited(pClonk, iClass, ...);
}

/* Fallschirmeffekt */

global func FxSpawnParachuteTimer(object pTarget)
{
  //Ziel im Freien?
  if(!Contained(pTarget))
  {
    //Ziel ist festem Boden zu nahe: Abbruch
    var x = GetX(pTarget), y = GetY(pTarget), xdir = GetXDir(pTarget, 100), ydir = GetYDir(pTarget, 100);
    SimFlight(x, y, xdir, ydir, 0, 0, 0, 100);
    if(Distance(xdir, ydir) < 700)
      return -1;

    //Ansonsten Fallschirm erstellen
    CreateObject(PARA,0,0,GetOwner(pTarget))->Set(pTarget);
    AddEffect("Flying", pTarget, 101, 5);

    //Effekt
    Sound("Airstrike2", 0, pTarget);

    return -1;
  }
  //Ansonsten abwarten
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Warpfield.ogg;CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Warpfield.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //AS-Spielziel
  if(FindObject(GASS))
  {
    //Zielobjekte
    AddAssaultTarget(CCP2, 870, 560, 30*30, 2, "$Target1$", 0, [[[430, 740], [555, 740], [895, 740], [975, 740]], [[370, -100], [570, -100], [870, -100]]]);
    AddAssaultTarget(CMSN, 470, 830, 30*30, 2, "$Target2$", 1, [[[340, 1110], [445, 1190], [735, 1190]], [[480, 230], [1135, 170], [1310, 290]]]);
    AddAssaultTarget(CCP1, 875, 830, 30*30, 2, "$Target3$", 2, [[[340, 1110], [445, 1190], [735, 1190]], [[480, 230], [1135, 170], [1310, 290]]]);
    AddAssaultTarget(PMP2, 1770, 820, 30*30, 2, "$Target4$", 3, [[[2440, 740], [2440, 910], [2440, 1100]], [[680, 560], [870, 560], [1045, 580]]]);
    AddAssaultTarget(GSTA, 1765, 1063, 30*30, 2, "$Target5$", 4, [[[2440, 740], [2440, 910], [2440, 1100]], [[680, 560], [870, 560], [1045, 580]]]);
    AddAssaultTarget(CMSN, 2780, 910, 30*30, 2, "$Target2$", 5, [[[3290, 780], [3410, 910], [3310, 1100]], [[1370, 820], [1480, 940], [1510, 1010]]]);
    AddAssaultTarget(LBPC, 2840, 1100, 30*30, 2, "$Target6$", 6, [[[3290, 780], [3410, 910], [3310, 1100]], [[1370, 820], [1480, 940], [1510, 1010]]]);
    AddAssaultTarget(PMP2, 1830, 1360, 30*30, 2, "$Target7$", 7, [[[1110, 1560], [1110, 1660], [1340, 1640]], [[2957, 1073], [3245, 910], [3410, 910]]]);
    AddAssaultTarget(GNRT, 1795, 1890, 30*30, 2, "$Target8$", 8, [[[2200, 1780], [2280, 1820], [2480, 1820]], [[1900, 1560], [2100, 1560], [2160, 1310]]]);

    //Ziele verbinden
    ConnectAssaultTargets([1, 2]);
    ConnectAssaultTargets([3, 4]);
    ConnectAssaultTargets([5, 6]);

    //Grenzen setzen
    CreateObject(BRDR, 210, 0, -1)->Set(0,1);
    CreateObject(BRDR, 1360, 0, -1)->Set(1,1);
    CreateObject(BRDR, 0, 760, -1)->Set(3,1);

    //Aufzüge
    CreateObject(LFTP, 785, 244, -1)->SetLimits(235,670);
    CreateObject(LFTP, 785, 810, -1)->SetLimits(745,1320);
    CreateObject(LFTP, 1065, 1394, -1)->SetLimits(1385,1910);
    CreateObject(LFTP, 1185, 184, -1)->SetLimits(175,820);
    CreateObject(LFTP, 1185, 1024, -1)->SetLimits(1015,1390);
    CreateObject(LFTP, 1415, 1570, -1)->SetLimits(1315,1570);
    CreateObject(LFTP, 1550, 734, -1)->DisableVertical();
    CreateObject(LFTP, 2085, 1324, -1)->DisableVertical();
    CreateObject(LFTP, 2355, 754, -1)->SetLimits(745,1460);
    CreateObject(LFTP, 2825, 920, -1)->SetLimits(555,920);
    CreateObject(LFTP, 3275, 1300, -1)->SetLimits(1105,1300);

    //Automaten
    var store = CreateObject(WPVM,610, 560,-1);
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
    store = CreateObject(WPVM,2170, 1560,-1);
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

    //Boden
    DrawMaterialQuad("Wall-Stripes", 760,750, 810,750, 810,760, 760,760);
    DrawMaterialQuad("Wall-Stripes", 1160,750, 1230,750, 1230,760, 1160,760);
    DrawMaterialQuad("Wall-Stripes", 1250,750, 1260,750, 1260,760, 1250,760);

    DrawMaterialQuad("Wall-Metal4", 330,1200, 440,1200, 440,1240, 330,1240);
    DrawMaterialQuad("Wall-Stripes", 360,1200, 410,1200, 410,1210, 360,1210);
    DrawMaterialQuad("Wall-Concrete2", 340,1240, 430,1240, 430,1250, 340,1250);

    DrawMaterialQuad("Wall-Stripes", 760,1200, 810,1200, 810,1210, 760,1210);
    DrawMaterialQuad("Wall-Metal4", 750,1210, 820,1210, 820,1240, 750,1240);
    DrawMaterialQuad("Wall-Bricks1", 750,1240, 820,1240, 820,1280, 750,1280);
    DrawMaterialQuad("Earth-ROUGH", 750,1280, 820,1280, 820,1331, 750,1331);

    DrawMaterialQuad("Wall-Stripes", 1110,1200, 1120,1200, 1120,1230, 1110,1230);
    DrawMaterialQuad("Wall-Stripes", 1140,1200, 1230,1200, 1230,1230, 1140,1230);
    DrawMaterialQuad("Wall-Stripes", 1250,1200, 1260,1200, 1260,1230, 1250,1230);

    DrawMaterialQuad("Wall-Stripes", 1380,1110, 1450,1110, 1450,1120, 1380,1120);
    DrawMaterialQuad("Wall-Plate", 1370,1120, 1460,1120, 1460,1210, 1370,1210);
    DrawMaterialQuad("Wall-Stripes", 1380,1210, 1450,1210, 1450,1221, 1380,1221);

    DrawMaterialQuad("Wall-Stripes", 2320,1110, 2390,1110, 2390,1120, 2320,1120);
    DrawMaterialQuad("Wall-Bricks1", 2290,1120, 2440,1120, 2440,1160, 2290,1160);
    DrawMaterialQuad("Wall-Concrete2", 2300,1160, 2430,1160, 2430,1170, 2300,1170);

    DrawMaterialQuad("Wall-Stripes", 3250,1100, 3300,1100, 3300,1110, 3250,1110);

    DrawMaterialQuad("Wall-Stripes", 1030,1670, 1090,1670, 1090,1680, 1030,1680);

    //Verbundene Räume
    aDoorWay[20] = CreateObject(ROOM, 700, 1190, -1);
    aDoorWay[21] = CreateObject(ROOM, 1080, 1110, -1);
    aDoorWay[20]->Connect(aDoorWay[21]);

    //Leiter
    CreateObject(LADR, 385, 1195, -1)->Set(17);

    //Container
    CreateObject(CON1, 1285, 610, -1)->SetPerspective(2);
    CreateObject(CON1, 1275, 810, -1)->SetPerspective(2);
    CreateObject(CON1, 1276, 940, -1)->SetPerspective(2);
    CreateObject(CON1, 1276, 1110, -1)->SetPerspective(2);
    CreateObject(CON1, 1290, 1010, -1)->SetPerspective();

    //Sandsackbarriere
    CreateObject(SBBA, 2111, 1820, -1)->Left();

    //Geschützstellungen
    aStationary[0] = CreateObject(GNET, 2020, 810, -1);
    aStationary[0]->Set(LCAC,-90);
    aStationary[1] = CreateObject(GNET, 2015, 1072, -1);
    aStationary[1]->Set(LCAC,-90);
    aStationary[2] = CreateObject(GNET, 2580, 1100, -1);
    aStationary[2]->Set(SATW,-90);
    aStationary[3] = CreateObject(GNET, 1110, 1870, -1);
    aStationary[3]->Set(SATW,90);
    aStationary[4] = CreateObject(GNET, 2500, 1820, -1);
    aStationary[4]->Set(SATW,-90);
    aStationary[5] = CreateObject(GNET, 1830, 1790, -1);
    aStationary[5]->Set(0,-90);

    //Hinweisschilder
    var sign = CreateObject(SGNP, 1555, 1010, -1);
    sign->SetMode(1);
    sign->Set("Turret");
    sign = CreateObject(SGNP, 2650, 1100, -1);
    sign->SetMode(1);
    sign->Set("Turret");

    //Versorgungskisten (Dragnin)
    CreateObject(AMCT, 1110, 280, -1)->Set(DGNN);
    CreateObject(AMCT, 2870, 910, -1)->Set(DGNN);

    //Versorgungskisten (Handgranaten)
    CreateObject(AMCT, 1022, 580, -1)->Set(FGRN);
    CreateObject(AMCT, 365, 830, -1)->Set(FGRN);

    //Versorgungskiste (Phosphorgranaten)
    CreateObject(AMCT, 870, 1560, -1)->Set(PGRN);
    CreateObject(AMCT, 2220, 1820, -1)->Set(PGRN);

    //Versorgungskisten (Revolver)
    CreateObject(AMCT, 385, 230, -1)->Set(RVLR);
    CreateObject(AMCT, 1830, 1640, -1)->Set(RVLR);

    //Versorgungskiste (APW)
    CreateObject(AMCT, 1330, 290, -1)->Set(ATWN);

    //Bodenluken
    aDoor[04] = CreateObject(HA4K, 1130, 1203, -1);
    aDoor[03] = CreateObject(HA4K, 1240, 713, -1);
    aDoor[05] = CreateObject(HA4K, 1240, 1203, -1);

    //Bodenluken schließen
    aDoor[01]->Lock();
    aDoor[02]->Lock();
    aDoor[03]->Lock();
    aDoor[04]->Lock();
    aDoor[05]->Lock();
    aDoor[07]->Lock();
    aDoor[08]->Lock();
    aDoor[11]->Lock();
    aDoor[12]->Lock();

    //Stahltore schließen und öffnen
    aDoorWay[39]->ControlRight();
    aDoorWay[40]->ControlLeft();
    aDoorWay[41]->ControlLeft();

    //Konsolen für Selbstschussanlagen
    CreateObject(CONS, 1485, 1105, -1)->Set(aSelfDefense[0]);
    CreateObject(CONS, 2490, 905, -1)->Set(aSelfDefense[1]);

    //SSA Besitzer setzen
    if(aTeams[2] == true)
      {aSelfDefense[0]->SetTeam(2); aSelfDefense[1]->SetTeam(2);}

    //SSA aktivieren
    aSelfDefense[0]->TurnOn();
    aSelfDefense[1]->TurnOn();

    //Alle Lampen deaktivieren
    PowerSupplyOff();

    //Lampen einschalten
    for(var obj in FindObjects(Find_Or(Find_ID(ETLT), Find_ID(STLH), Find_ID(BLGH), Find_ID(CLGH)), Find_InRect(250,80,1090,670)))
      obj->TurnOn();

    //Objekte entfernen
    RemoveAll(HSGN);
    RemoveObject(aDoorWay[10]);
    RemoveObject(aDoorWay[11]);

    //Munition
    if(!FindObject(NOAM))
    {
      //Versorgungskisten (Kugeln)
      CreateObject(AMCT, 525, 230, -1)->Set(ABOX);
      CreateObject(AMCT, 580, 740, -1)->Set(ABOX);
      CreateObject(AMCT, 1910, 1560, -1)->Set(ABOX);
      CreateObject(AMCT, 2260, 910, -1)->Set(ABOX);

      //Kugeln
      PlaceSpawnpoint(ABOX, 2055, 1885);
      PlaceSpawnpoint(ABOX, 3300, 905);

      //Gewehrgranaten
      PlaceSpawnpoint(GBOX, 1290, 1375);
      PlaceSpawnpoint(GBOX, 1535, 1885);
      PlaceSpawnpoint(GBOX, 2055, 1095);
      PlaceSpawnpoint(GBOX, 3220, 785);

      //Raketen
      PlaceSpawnpoint(MBOX, 960, 825);
      PlaceSpawnpoint(MBOX, 1055, 405);
      PlaceSpawnpoint(MBOX, 1145, 1905);
      PlaceSpawnpoint(MBOX, 1415, 1105);
      PlaceSpawnpoint(MBOX, 1865, 1455);
      PlaceSpawnpoint(MBOX, 2860, 735);
    }
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Flaggenposten
    var flag = CreateObject(OFPL, 1765,820, -1);
    flag->~Set("$Flag5$");

    //Teamgrenzen
    CreateObject(BRDR, 1310, 0, -1)->Set(0,1,0,1,1);
    CreateObject(BRDR, 2240, 0, -1)->Set(1,1,0,1,2);

    //Grenzen setzen
    CreateObject(BRDR, 760, 0, -1)->Set(0);
    CreateObject(BRDR, 2900, 0, -1)->Set(1);
    CreateObject(BRDR, 0, 1120, -1)->Set(3);

    //Leitern
    CreateObject(LADR, 1660, 510, -1)->Set(6);
    CreateObject(LADR, 1871, 510, -1)->Set(6);

    //Sandsackbarrieren
    CreateObject(SBBA, 1676, 460, -1);
    CreateObject(SBBA, 1850, 460, -1)->Right();

    //Aufzüge
    CreateObject(LFTP, 785, 244, -1)->SetLimits(235,670);
    CreateObject(LFTP, 785, 754, -1)->SetLimits(745,1320);
    CreateObject(LFTP, 1185, 184, -1)->SetLimits(175,820);
    CreateObject(LFTP, 1550, 734, -1)->DisableVertical();
    CreateObject(LFTP, 2355, 754, -1)->SetLimits(745,1460);
    CreateObject(LFTP, 2825, 154, -1)->SetLimits(145,920);

    //Automaten
    var store = CreateObject(WPVM,1765, 1110,-1);
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

    //Stahltore und Konsolen
    CreateObject(CONS, 1540, 505, -1)->Set(aDoorWay[39]);
    CreateObject(CONS, 1985, 505, -1)->Set(aDoorWay[40]);

    //Boden
    DrawMaterialQuad("Wall-Stripes", 760,1020, 810,1020, 810,1040, 760,1040);

    DrawMaterialQuad("Wall-Stripes", 1110,1110, 1120,1110, 1120,1130, 1110,1130);
    DrawMaterialQuad("Wall-Stripes", 1140,1110, 1160,1110, 1160,1130, 1140,1130);
    DrawMaterialQuad("Wall-Stripes", 1140,1120, 1230,1120, 1230,1130, 1140,1130);
    DrawMaterialQuad("Wall-Stripes", 1210,1110, 1230,1110, 1230,1120, 1210,1120);
    DrawMaterialQuad("Wall-Stripes", 1250,1110, 1260,1110, 1260,1120, 1250,1120);

    DrawMaterialQuad("Wall-Stripes", 1380,1110, 1450,1110, 1450,1120, 1380,1120);
    DrawMaterialQuad("Wall-Plate", 1370,1120, 1460,1120, 1460,1210, 1370,1210);
    DrawMaterialQuad("Wall-Stripes", 1380,1210, 1450,1210, 1450,1221, 1380,1221);

    DrawMaterialQuad("Wall-Stripes", 2320,1110, 2390,1110, 2390,1120, 2320,1120);
    DrawMaterialQuad("Wall-Bricks1", 2290,1120, 2440,1120, 2440,1160, 2290,1160);
    DrawMaterialQuad("Wall-Concrete2", 2300,1160, 2430,1160, 2430,1170, 2300,1170);

    DrawMaterialQuad("Wall-Stripes", 3250,1100, 3300,1100, 3300,1110, 3250,1110);

    //Verbundene Räume
    aDoorWay[30] = CreateObject(GAT1, 926, 130, -1);
    aDoorWay[31] = CreateObject(ROOM, 1080, 1110, -1);
    aDoorWay[30]->Connect(aDoorWay[31]);
    aDoorWay[32] = CreateObject(GAT1, 2575, 90, -1);
    aDoorWay[33] = CreateObject(GAT3, 2245, 1050, -1);
    aDoorWay[32]->Connect(aDoorWay[33]);
    aDoorWay[35] = CreateObject(ROOM, 2750, 740, -1);
    aDoorWay[36] = CreateObject(ROOM, 2520, 1100, -1);
    aDoorWay[35]->Connect(aDoorWay[36]);

    //Bodenluken
    CreateObject(HA4K, 1130, 1113, -1)->Lock();
    CreateObject(HA4K, 1240, 1113, -1)->Lock();

    //Bodenluke schließen
    aDoor[10]->Lock();
    aDoor[11]->Lock();

    //Geschützstellungen
    CreateObject(GNET, 845, 1010, -1)->Set(SATW,90);
    CreateObject(GNET, 1290, 100, -1)->Set(SATW,90);
    CreateObject(GNET, 2560, 140, -1)->Set(SATW,-90);
    CreateObject(GNET, 2580, 1100, -1)->Set(SATW,-90);

    //Hinweisschilder
    var sign = CreateObject(SGNP, 730, 230, -1);
    sign->SetMode(1);
    sign = CreateObject(SGNP, 730, 740, -1);
    sign->SetMode(1);
    sign = CreateObject(SGNP, 730, 1010, -1);
    sign->SetMode(1);
    CreateObject(SGNP, 765, 90, -1);
    sign = CreateObject(SGNP, 1555, 1010, -1);
    sign->SetMode(1);
    sign->Set("Turret");
    sign = CreateObject(SGNP, 2840, 1100, -1);
    sign->SetMode(1);
    sign = CreateObject(SGNP, 2900, 550, -1);
    sign->SetMode(1);
    sign = CreateObject(SGNP, 2900, 740, -1);
    sign->SetMode(1);
    CreateObject(SGNP, 2950, 90, -1);

    //Gerüste
    CreateObject(SFFG, 1705, 510, -1)->Set(4);
    CreateObject(SFFG, 1765, 410, -1)->Set(4);
    CreateObject(SFFG, 1765, 460, -1)->Set(5);
    CreateObject(SFFG, 1765, 510, -1)->Set(5);
    CreateObject(SFFG, 1825, 510, -1)->Set(4);

    //Gerüst anpassen
    for(var obj in FindObjects(Find_ID(SFFG), Find_InRect(1740,1020,50,50)))
      obj->Set(5);

    //Objekte entfernen
    RemoveObject(aSelfDefense[0]);
    RemoveObject(aSelfDefense[1]);
    RemoveAll(HSGN);
    RemoveObject(aDoorWay[0]);
    RemoveObject(aDoorWay[1]);
    RemoveObject(aDoorWay[8]);
    RemoveObject(aDoorWay[9]);

    //Versorgungskiste (Revolver)
    CreateObject(AMCT, 1140, 580, -1)->Set(RVLR);
    CreateObject(AMCT, 2440, 910, -1)->Set(RVLR);

    //Versorgungskiste (APW)
    CreateObject(AMCT, 1760, 520, -1)->Set(ATWN);

    //Munition
    if(!FindObject(NOAM))
    {
      //Versorgungskiste (Kugeln)
      CreateObject(AMCT, 975, 1010, -1)->Set(ABOX);
      CreateObject(AMCT, 1640, 1110, -1)->Set(ABOX);
      CreateObject(AMCT, 2545, 1100, -1)->Set(ABOX);

      //Versorgungskiste (Gewehrgranaten)
      CreateObject(AMCT, 1890, 1110, -1)->Set(GBOX);

      //Gewehrgranaten
      PlaceSpawnpoint(GBOX, 1700, 455);

      //Raketen
      PlaceSpawnpoint(MBOX, 1820, 455);
    }
  }

  //DM/LMS/MR/OCC-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS) || FindObject(GMNR) || FindObject(GOCC))
  {
    //OCC-Spielziel
    if(FindObject(GOCC))
    {
      //Flaggenposten
      aFlag[0] = CreateObject(OFPL,575,920,NO_OWNER);
      aFlag[0] -> AddSpawnPoint(430,550);
      aFlag[0] -> AddSpawnPoint(605,480);
      aFlag[0] -> AddSpawnPoint(660,550);
      if(aTeams[1] == true)
      {
        aFlag[0]->Set("$Flag1$");
        aFlag[0]->Capture(1,1);
      }
      else
      {
        aFlag[0]->Set("$Flag1$");
      }

      aFlag[1] = CreateObject(OFPL,1250,190,NO_OWNER);
      aFlag[1] -> AddSpawnPoint(365,100);
      aFlag[1] -> AddSpawnPoint(505,120);
      aFlag[1] -> AddSpawnPoint(625,80);
      if(aTeams[1] == true)
      {
        aFlag[1]->Set("$Flag2$");
        aFlag[1]->Capture(1,1);
      }
      else
      {
        aFlag[1]->Set("$Flag2$");
      }

      aFlag[2] = CreateObject(OFPL,1765,820,NO_OWNER);
      aFlag[2] -> AddSpawnPoint(1280,600);
      aFlag[2] -> AddSpawnPoint(1470,500);
      aFlag[2] -> AddSpawnPoint(2070,490);
      aFlag[2] -> AddSpawnPoint(2290,540);
      aFlag[2]->Set("$Flag3$");

      aFlag[3] = CreateObject(OFPL,2730,270,NO_OWNER);
      aFlag[3] -> AddSpawnPoint(2560,130);
      aFlag[3] -> AddSpawnPoint(2795,90);
      aFlag[3] -> AddSpawnPoint(3040,90);
      if(aTeams[2] == true)
      {
        aFlag[3]->Set("$Flag4$");
        aFlag[3]->Capture(2,1);
      }
      else
      {
        aFlag[3]->Set("$Flag4$");
      }

      aFlag[4] = CreateObject(OFPL,2860,1100,NO_OWNER);
      aFlag[4] -> AddSpawnPoint(3290, 770);
      aFlag[4] -> AddSpawnPoint(3400, 900);
      aFlag[4] -> AddSpawnPoint(3340, 1090);
      if(aTeams[2] == true)
      {
        aFlag[4]->Set("$Flag5$");
        aFlag[4]->Capture(2,1);
      }
      else
      {
        aFlag[4]->Set("$Flag5$");
      }
    }

    //MR-Spielziel
    if(FindObject(GMNR))
    {
      //Geldsäcke
      AddMoneySpawn(960, 555, [15]);
      AddMoneySpawn(580, 915, [15]);
      AddMoneySpawn(1325, 1005, [15]);
      AddMoneySpawn(1760, 515, [20]);
      AddMoneySpawn(1765, 815, [20]);
      AddMoneySpawn(2195, 1095, [15]);
      AddMoneySpawn(3160, 1095, [15]);
      AddMoneySpawn(2620, 735, [15]);
    }

    //Grenzen setzen
    CreateObject(BRDR, 210, 0, -1)->Set(0);
    CreateObject(BRDR, 3490, 0, -1)->Set(1);
    CreateObject(BRDR, 0, 1210, -1)->Set(3);

    //Aufzüge
    CreateObject(LFTP, 785, 244, -1)->SetLimits(235,670);
    CreateObject(LFTP, 785, 754, -1)->SetLimits(745,1320);
    CreateObject(LFTP, 1185, 184, -1)->SetLimits(175,820);
    CreateObject(LFTP, 1550, 734, -1)->DisableVertical();
    CreateObject(LFTP, 2355, 754, -1)->SetLimits(745,1460);
    CreateObject(LFTP, 2825, 154, -1)->SetLimits(145,920);

    //Stahltore und Konsolen
    CreateObject(CONS, 1540, 505, -1)->Set(aDoorWay[39]);
    CreateObject(CONS, 1985, 505, -1)->Set(aDoorWay[40]);

    //Boden
    DrawMaterialQuad("Wall-Metal4", 330,1200, 440,1200, 440,1240, 330,1240);
    DrawMaterialQuad("Wall-Stripes", 360,1200, 410,1200, 410,1210, 360,1210);
    DrawMaterialQuad("Wall-Concrete2", 340,1240, 430,1240, 430,1250, 340,1250);

    DrawMaterialQuad("Wall-Concrete2", 440,560, 460,560, 460,600, 440,600);

    DrawMaterialQuad("Wall-Stripes", 760,1200, 810,1200, 810,1210, 760,1210);
    DrawMaterialQuad("Wall-Metal4", 750,1210, 820,1210, 820,1240, 750,1240);
    DrawMaterialQuad("Wall-Bricks1", 750,1240, 820,1240, 820,1280, 750,1280);
    DrawMaterialQuad("Earth-ROUGH", 750,1280, 820,1280, 820,1331, 750,1331);

    DrawMaterialQuad("Wall-Concrete3", 1110,1110, 1140,1110, 1160,1120, 1110,1120);
    DrawMaterialQuad("Wall-Concrete3", 1260,1110, 1230,1110, 1210,1120, 1260,1120);
    DrawMaterialQuad("Wall-Plate", 1100,1120, 1260,1120, 1260,1230, 1100,1230);
    DrawMaterialQuad("Wall-Concrete2", 1110,1230, 1260,1230, 1260,1240, 1110,1240);

    DrawMaterialQuad("Wall-Stripes", 1380,1110, 1450,1110, 1450,1120, 1380,1120);
    DrawMaterialQuad("Wall-Plate", 1370,1120, 1460,1120, 1460,1210, 1370,1210);
    DrawMaterialQuad("Wall-Stripes", 1380,1210, 1450,1210, 1450,1221, 1380,1221);

    DrawMaterialQuad("Wall-Stripes", 2320,1110, 2390,1110, 2390,1120, 2320,1120);
    DrawMaterialQuad("Wall-Bricks1", 2290,1120, 2440,1120, 2440,1160, 2290,1160);
    DrawMaterialQuad("Wall-Concrete2", 2300,1160, 2430,1160, 2430,1170, 2300,1170);

    DrawMaterialQuad("Wall-Concrete3", 3130,1100, 3160,1100, 3160,1110, 3130,1110);
    DrawMaterialQuad("Wall-Metal4", 3130,1110, 3160,1110, 3160,1120, 3130,1120);
    DrawMaterialQuad("Wall-Bricks1", 3130,1120, 3160,1120, 3160,1160, 3130,1160);

    DrawMaterialQuad("Wall-Stripes", 3250,1100, 3300,1100, 3300,1120, 3250,1120);

    //Automaten
    var store = CreateObject(WPVM,610, 560,-1);
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
    store = CreateObject(WPVM,2840, 1100,-1);
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

    //Artilleriebatterien
    CreateObject(ATBY,1185,100,-1);
    CreateObject(ATBY,2825,100,-1);

    //Geschützstellungen
    CreateObject(GNET, 610, 1010, -1)->Set(SATW,90);
    CreateObject(GNET, 2580, 1100, -1)->Set(SATW,-90);

    //Versorgungskisten (Dragnin)
    CreateObject(AMCT, 1110, 280, -1)->Set(DGNN);
    CreateObject(AMCT, 2870, 910, -1)->Set(DGNN);

    //Versorgungskisten (Handgranaten)
    CreateObject(AMCT, 365, 830, -1)->Set(FGRN);
    CreateObject(AMCT, 1022, 580, -1)->Set(FGRN);
    CreateObject(AMCT, 2440, 910, -1)->Set(FGRN);
    CreateObject(AMCT, 3425, 910, -1)->Set(FGRN);

    //Versorgungskiste (Phosphorgranaten)
    CreateObject(AMCT, 1765, 820, -1)->Set(PGRN);

    //Versorgungskiste (Railgun)
    CreateObject(AMCT, 1760, 520, -1)->Set(RLGN,0,0,0,1);

    //Versorgungskisten (Revolver)
    CreateObject(AMCT, 385, 230, -1)->Set(RVLR);
    CreateObject(AMCT, 2620, 270, -1)->Set(RVLR);

    //Versorgungskisten (APW)
    CreateObject(AMCT, 515, 1190, -1)->Set(ATWN);
    CreateObject(AMCT, 3275, 1100, -1)->Set(ATWN);

    //Hinweisschilder
    CreateObject(SGNP, 65, 90, -1);
    CreateObject(SGNP, 210, 40, -1);
    CreateObject(SGNP, 3370, 30, -1);
    CreateObject(SGNP, 3590, 70, -1);

    //Objekte entfernen
    RemoveObject(aSelfDefense[0]);
    RemoveObject(aSelfDefense[1]);
    RemoveAll(HSGN);
    RemoveObject(aDoorWay[8]);
    RemoveObject(aDoorWay[9]);
    RemoveObject(aDoor[11]);
    RemoveObject(FindObject2(Find_ID(HA4K),Find_InRect(435, 540, 30, 30)));
    for(var obj in FindObjects(Find_ID(LADR), Find_InRect(1120, 290, 130, 1060)))
      RemoveObject(obj);
    for(var obj in FindObjects(Find_ID(LADR), Find_InRect(3135, 1100, 20, 180)))
      RemoveObject(obj);

    //Leitern
    CreateObject(LADR, 385, 1195, -1)->Set(17);
    CreateObject(LADR, 1130, 1098, -1)->Set(10);

    CreateObject(LADR, 1240, 1098, -1)->Set(100);

    //Munition
    if(!FindObject(NOAM))
    {
      //Versorgungskisten (Kugeln)
      CreateObject(AMCT, 525, 230, -1)->Set(ABOX);
      CreateObject(AMCT, 1640, 1110, -1)->Set(ABOX);
      CreateObject(AMCT, 2645, 270, -1)->Set(ABOX);

      //Versorgungskiste (Gewehrgranaten)
      CreateObject(AMCT, 1890, 1110, -1)->Set(GBOX);

      //Kugeln
      PlaceSpawnpoint(ABOX, 740, 915);
      PlaceSpawnpoint(ABOX, 2780, 905);

      //Gewehrgranaten
      PlaceSpawnpoint(GBOX, 580, 735);
      PlaceSpawnpoint(GBOX, 3220, 785);

      //Raketen
      PlaceSpawnpoint(MBOX, 960, 825);
      PlaceSpawnpoint(MBOX, 2220, 905);
    }
  }
}

/* Assault Zerstörung */

public func OnAssaultTargetDestruction(object pTarget, int iTeam, int iIndex, bool fConnectedDestroyed)
{
  //Ziel 1
  if(!iIndex)
  {
    //Grenzen neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 210, 0, -1)->Set(0,1);
    CreateObject(BRDR, 1360, 0, -1)->Set(1,1);
    CreateObject(BRDR, 0, 1210, -1)->Set(3,1);

    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),0,740,3,1);

    //Lampen ausschalten
    for(var obj in FindObjects(Find_Or(Find_ID(ETLT), Find_ID(STLH), Find_ID(BLGH), Find_ID(CLGH)), Find_InRect(250,80,1090,670)))
    {
      obj->EMPShock();
      obj->TurnOff();
    }

    //Lampen einschalten
    for(var obj in FindObjects(Find_Or(Find_ID(CLGH), Find_ID(LLGH), Find_ID(BLGH)), Find_InRect(250,760,820,440)))
      obj->TurnOn();

    //Türen öffnen
    aDoor[01]->Open();
    aDoor[02]->Open();

    //Objekt entfernen
    RemoveObject(aDoor[03]);

    //Türverbindung schließen
    aDoorWay[22]->SealEntrance();
    aDoorWay[23]->SealEntrance();

    //Boden entfernen
    DrawMaterialQuad("Backwall-Stripes", 760,750, 810,750, 810,760, 760,760);
    DrawMaterialQuad("Backwall-Stripes", 1160,750, 1230,750, 1230,760, 1160,760);
    DrawMaterialQuad("Backwall-Stripes", 1250,750, 1260,750, 1260,760, 1250,760);

    //Explosionen
    CreateObject(TIM1, 785,755)->DecoExplode(30);
    CreateObject(TIM1, 1185,755)->DecoExplode(30);
    CreateObject(TIM1, 1230,755)->DecoExplode(30);
  }

  //Ziel 2 und 3
  if(iIndex == 1 || iIndex == 2)
  {
    if(iIndex == 2)
    {
      //Ventillatoreneffekte
      for(var obj in FindObjects(Find_Or(Find_ID(MVNT), Find_ID(VENT))))
        obj->AddSmokeEffect4K(75,0,-10,true,this);
    }

    if(fConnectedDestroyed)
    {
      //Grenzen neu setzen
      RemoveAll(BRDR);
      CreateObject(BRDR, 210, 0, -1)->Set(0,1);
      CreateObject(BRDR, 2640, 0, -1)->Set(1,1);
      CreateObject(BRDR, 0, 1210, -1)->Set(3,1);

      //Teamgrenze setzen
      FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),1260,0,1,1);

      //Container zerstören
      for(var obj in FindObjects(Find_ID(CON1), Find_InRect(1250,570,80,540)))
        obj->DecoExplode(35);

      //Stahltore öffnen
      aDoorWay[39]->ControlLeft();
      aDoorWay[40]->ControlRight();

      //Lampen ausschalten
      for(var obj in FindObjects(Find_Or(Find_ID(CLGH), Find_ID(LLGH), Find_ID(BLGH)), Find_InRect(250,760,820,440)))
      {
        obj->EMPShock();
       obj->TurnOff();
      }

      //Lampen einschalten
      for(var obj in FindObjects(Find_Or(Find_ID(SLGH), Find_ID(LLGH)), Find_InRect(1390,440,1090,660)))
        obj->TurnOn();
    }
  }

  //Ziel 4 und 5
  if(iIndex == 3 || iIndex == 4)
  {
    if(fConnectedDestroyed)
    {
      //Grenzen neu setzen
      RemoveAll(BRDR);
      CreateObject(BRDR, 750, 0, -1)->Set(0,1);
      CreateObject(BRDR, 3490, 0, -1)->Set(1,1);
      CreateObject(BRDR, 0, 1210, -1)->Set(3,1);
      CreateObject(BRDR, 0, 410, -1)->Set(2,1);

      //Teamgrenze setzen
      FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),2180,0,1,1);

      //Lampen ausschalten
      for(var obj in FindObjects(Find_Or(Find_ID(SLGH), Find_ID(LLGH)), Find_InRect(1390,440,1090,660)))
      {
        obj->EMPShock();
        obj->TurnOff();
      }

      //Lampen einschalten
      for(var obj in FindObjects(Find_Or(Find_ID(ETLT), Find_ID(LLGH), Find_ID(BLGH), Find_ID(STLH)), Find_InRect(2560,100,630,1170)))
        obj->TurnOn();

      //SSA zerstören
      aSelfDefense[0]->Disarm();
      aSelfDefense[0]->DecoExplode(30);

      //Geschützstellungen entfernen
      aStationary[0]->DecoExplode(30);
      aStationary[1]->DecoExplode(30);
    }
  }

  //Ziel 6 und 7
  if(iIndex == 5 || iIndex == 6)
  {
    if(fConnectedDestroyed)
    {
      //Grenzen neu setzen
      RemoveAll(BRDR);
      CreateObject(BRDR, 840, 0, -1)->Set(0,1);
      CreateObject(BRDR, 3490, 0, -1)->Set(1,1);
      CreateObject(BRDR, 0, 1680, -1)->Set(3,1);
      CreateObject(BRDR, 0, 770, -1)->Set(2,1);

      //Teamgrenze setzen
      FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),0,1250,3,1);

      //Lampen ausschalten
      for(var obj in FindObjects(Find_Or(Find_ID(ETLT), Find_ID(LLGH), Find_ID(BLGH), Find_ID(STLH)), Find_InRect(2560,100,630,1170)))
      {
        obj->EMPShock();
        obj->TurnOff();
      }

      //Lampen einschalten
      for(var obj in FindObjects(Find_Or(Find_ID(LLGH), Find_ID(SLGH), Find_ID(CLGH)), Find_InRect(370,1270,2820,290)))
        obj->TurnOn();

      //Bodenluke öffnen
      aDoor[11]->Open();

      //SSA zerstören
      aSelfDefense[1]->Disarm();
      aSelfDefense[1]->DecoExplode(30);

      //Geschützstellung entfernen
      aStationary[2]->DecoExplode(30);

      //Türverbindung schließen
      aDoorWay[20]->SealEntrance();
      aDoorWay[21]->SealEntrance();

      //Boden entfernen
      DrawMaterialQuad("Backwall-Stripes", 3250,1100, 3300,1100, 3300,1110, 3250,1110);

      //Explosionen
      CreateObject(TIM1, 3275,1100)->DecoExplode(30);
    }
  }

  //Ziel 8
  if(iIndex == 7)
  {
    //Grenzen neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 840, 0, -1)->Set(0,1);
    CreateObject(BRDR, 3490, 0, -1)->Set(1,1);
    CreateObject(BRDR, 0, 1970, -1)->Set(3,1);
    CreateObject(BRDR, 0, 880, -1)->Set(2,1);

    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),0,1620,3,1);

    //Bodenluke öffnen
    aDoor[12]->Open();

    //Lampen ausschalten
    for(var obj in FindObjects(Find_Or(Find_ID(LLGH), Find_ID(SLGH), Find_ID(CLGH)), Find_InRect(370,1270,2820,290)))
    {
      obj->EMPShock();
      obj->TurnOff();
    }

    //Lampen einschalten
    for(var obj in FindObjects(Find_Or(Find_ID(LLGH), Find_ID(SLGH), Find_ID(CLGH)), Find_InRect(760,1650,610,210)))
      obj->TurnOn();

    //Boden entfernen
    DrawMaterialQuad("Backwall-Stripes", 1030,1670, 1090,1670, 1090,1680, 1030,1680);

    //Explosion
    CreateObject(TIM1, 1065,1675)->DecoExplode(30);

  }

  //Ziel 9
  if(iIndex == 8)
  {
    //Generator ausschalten
    var generator = FindObject(GNRT);
    generator->TurnOff();
    generator->SetClrModulation(RGBa(100,100,100,50));
    CreateObject(TIM1, 1700,1790)->AddSmokeEffect4K(40,0,0,true,this);
    CreateObject(TIM1, 1760,1830)->AddSmokeEffect4K(70,0,0,true,this);
    CreateObject(TIM1, 1850,1840)->AddSmokeEffect4K(100,0,0,true,this);
    CreateObject(TIM1, 1935,1850)->AddSmokeEffect4K(40,0,0,true,this);

    //Explosionen
    CreateObject(TIM1, 1640,1610)->DecoExplode(30);
    CreateObject(TIM1, 1640,1790)->DecoExplode(30);
    CreateObject(TIM1, 1950,1610)->DecoExplode(30);
    CreateObject(TIM1, 1950,1790)->DecoExplode(30);

    //Risse
    CreateObject(FAUD, 1615, 1850, -1);
    CreateObject(FAUD, 1620, 1900, -1);
    CreateObject(FAUD, 1770, 1750, -1);
    CreateObject(FAUD, 1900, 1750, -1);
    CreateObject(FAUD, 1975, 1850, -1);

    //Lampen ausschalten
    for(var obj in FindObjects(Find_Or(Find_ID(LLGH), Find_ID(SLGH), Find_ID(CLGH)), Find_InRect(760,1650,610,210)))
    {
      obj->EMPShock();
      obj->TurnOff();
    }

    //Geschützstellungen entfernen
    aStationary[3]->DecoExplode(30);
    aStationary[4]->DecoExplode(30);
    aStationary[5]->DecoExplode(30);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 1240; iY = 180;
    return 1;
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {if(FindObject(GASS)->GetRespawnPoint(iX, iY, iTeam)) return 1;}

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    if(iTeam == 1)
      return [[865, 730], [865, 820], [855, 910]];
    if(iTeam == 2)
      return [[2650, 1090], [2665, 730], [2670, 900]];
    return 1;
  }

  //MR/LMS/DM-Spielziel
  if(FindObject(GMNR) || FindObject(GLMS) || FindObject(GTDM))
  {
    if(iTeam == 1)
      return [[370, 100], [575, 220], [770, 80], [1110, 220], [1310, 280]];
    if(iTeam == 2)
      return [[2560, 130], [2590, 260], [2760, 90], [2900, 260], [3025, 90]];
    return 1;
  }
}