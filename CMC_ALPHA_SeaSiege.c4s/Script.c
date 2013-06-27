/*-- ALPHA - Sea Siege --*/

#strict
#include CSTD

static aFlag,aSelfDefense,aStationary,aTowerInterior, doorw, pRoom;

func RecommendedGoals()			{return [GOCC, GASS];}	//Spielzielempfehlung
public func AssaultDefenderTeam()	{return 2;}		//Verteidigerteam bei Assault


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Bildschirmfärbung
  SetGamma(RGB(7,6,0), RGB(152,147,128), RGB(255,254,236) );
  //Flaggen
  aFlag = [];
  //Geschützstellungen
  aStationary = [];
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
  return(1); 
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Leitern
  CreateObject(LADR, 2090, 1490, -1)->Set(14, LADR_Left);
  CreateObject(LADR, 2340, 1010, -1)->Set(4);
  CreateObject(LADR, 2340, 1616, -1)->Set(36);
  CreateObject(LADR, 2445, 1616, -1)->Set(36);
  CreateObject(LADR, 2450, 1010, -1)->Set(4);
  CreateObject(LADR, 2690, 1490, -1)->Set(14, LADR_Right);

  CreateObject(LADR, 4045, 1716, -1)->Set(16);
  CreateObject(LADR, 4100, 1572, -1)->Set(18);
  CreateObject(LADR, 4970, 1002, -1)->Set(18);
  CreateObject(LADR, 5080, 1002, -1)->Set(18);
  CreateObject(LADR, 5950, 1572, -1)->Set(18);
  CreateObject(LADR, 6005, 1716, -1)->Set(16);

  CreateObject(LADR, 7360, 1490, -1)->Set(14, LADR_Right);
  CreateObject(LADR, 7600, 1010, -1)->Set(4);
  CreateObject(LADR, 7605, 1616, -1)->Set(36);
  CreateObject(LADR, 7710, 1616, -1)->Set(36);
  CreateObject(LADR, 7710, 1010, -1)->Set(4);
  CreateObject(LADR, 7960, 1490, -1)->Set(14, LADR_Left);

/*
  CreateObject(LADR, 3530, 1260, -1)->Set(17, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3635, 1055, -1)->Set(5, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3910, 640, -1)->Set(7, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 4240, 795, -1)->Set(26, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 4380, 805, -1)->Set(7, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 4650, 1070, -1)->Set(19, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 5425, 1267, -1)->Set(4);
  CreateObject(LADR, 5575, 1267, -1)->Set(4);
*/
  //Bodenluken
  CreateObject(HA4K, 2090, 1373, -1);
  CreateObject(HA4K, 2340, 973, -1);
  CreateObject(HA4K, 2340, 1323, -1);
  CreateObject(HA4K, 2340, 1483, -1);
  CreateObject(HA4K, 2450, 973, -1);
  CreateObject(HA4K, 2690, 1373, -1);

  CreateObject(HA4K, 4100, 1423, -1);
  CreateObject(HA4K, 4100, 1523, -1);
  CreateObject(HA4K, 4970, 853, -1);
  CreateObject(HA4K, 4970, 943, -1);
  CreateObject(HA4K, 5080, 853, -1);
  CreateObject(HA4K, 5080, 943, -1);
  CreateObject(HA4K, 5950, 1423, -1);
  CreateObject(HA4K, 5950, 1523, -1);

  CreateObject(HA4K, 7360, 1373, -1);
  CreateObject(HA4K, 7600, 973, -1);
  CreateObject(HA4K, 7710, 973, -1);
  CreateObject(HA4K, 7710, 1323, -1);
  CreateObject(HA4K, 7710, 1483, -1);

  //Große Bodenluken
  CreateObject(H24K, 2035, 1378, -1);
  CreateObject(H24K, 2445, 1328, -1);
  CreateObject(H24K, 2445, 1488, -1);
  CreateObject(H24K, 2745, 1378, -1);
  CreateObject(H24K, 4035, 1428, -1);
  CreateObject(H24K, 4045, 1588, -1);
  CreateObject(H24K, 4165, 1428, -1);
  CreateObject(H24K, 5885, 1428, -1);
  CreateObject(H24K, 6005, 1588, -1);
  CreateObject(H24K, 6015, 1428, -1);
  CreateObject(H24K, 7305, 1378, -1);
  CreateObject(H24K, 7605, 1328, -1);
  CreateObject(H24K, 7605, 1488, -1);
  CreateObject(H24K, 8015, 1378, -1);
/*
  //Rampen
  DrawMaterialQuad("Wall-Plate",1300,1321,1300,1361,1260,1321,1300,1321,true);

  DrawMaterialQuad("Wall-PlateBlue",1341,1150,1341,1140,1371,1150,1341,1150,true);
  DrawMaterialQuad("Wall-Plate",1371,1160,1371,1150,1401,1160,1371,1160,true);
  DrawMaterialQuad("Wall-PlateBlue",1401,1171,1401,1161,1431,1171,1401,1171,true);

  DrawMaterialQuad("Wall-Metal2",1371,1300,1371,1290,1341,1300,1371,1300,true);
  DrawMaterialQuad("Wall-Metal2",1400,1290,1400,1280,1370,1290,1400,1290,true);
  DrawMaterialQuad("Wall-Plate",1430,1280,1430,1270,1400,1280,1430,1280,true);
  DrawMaterialQuad("Wall-Plate",1460,1270,1460,1260,1430,1270,1460,1270,true);

  DrawMaterialQuad("Wall-Plate",1731,1270,1731,1260,1761,1270,1731,1270,true);
  DrawMaterialQuad("Wall-Plate",1761,1281,1761,1271,1791,1281,1761,1281,true);
  DrawMaterialQuad("Wall-Metal2",1791,1291,1791,1281,1821,1291,1791,1291,true);
  DrawMaterialQuad("Wall-Metal2",1821,1301,1821,1291,1851,1301,1821,1301,true);

  DrawMaterialQuad("Wall-PlateBlue",2252,1170,2252,1160,2222,1170,2252,1170,true);
  DrawMaterialQuad("Wall-Plate",2280,1160,2280,1150,2250,1160,2280,1160,true);
  DrawMaterialQuad("Wall-PlateBlue",2311,1150,2311,1140,2281,1150,2311,1150,true);

  DrawMaterialQuad("Wall-Plate",2231,1321,2231,1361,2391,1321,2231,1321,true);

  DrawMaterialQuad("Wall-Stripes", 2290, 1279, 2350, 1279, 2290, 1280, 2350, 1280, 1);
  DrawMaterialQuad("Rock-ROUGH", 3130, 1278, 3180, 1278, 3130, 1280, 3180, 1280, 1);
  DrawMaterialQuad("Rock-ROUGH", 3350, 1278, 3400, 1278, 3350, 1280, 3400, 1280, 1);
  DrawMaterialQuad("Rock-ROUGH", 3500, 1278, 3570, 1278, 3500, 1280, 3570, 1280, 1);
  DrawMaterialQuad("Granite-Rock", 4570, 1228, 4630, 1228, 4570, 1230, 4630, 1230, 1);
*/
  //Glasscheiben
  CreateObject(_WIN, 4062, 1520, -1);
  CreateObject(_WIN, 5988, 1520, -1);
/*
  //Kisten
  CreateObject(WCR2, 1690, 1170, -1)->AutoRespawn();
  CreateObject(WCR2, 1690, 1260, -1)->AutoRespawn();
  CreateObject(WCR2, 3640, 521, -1);
  CreateObject(WCR2, 3700, 571, -1);
*/
  //Metallkisten
  CreateObject(MWCR, 2270, 1480, -1)->AutoRespawn();
  CreateObject(MWCR, 7780, 1480, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 4210, 1520, -1)->AutoRespawn();
  CreateObject(BECR, 5840, 1520, -1)->AutoRespawn();
/*
  //Explosive Kisten
  CreateObject(XWCR, 1845, 1170, -1)->AutoRespawn();
  CreateObject(XWCR, 3600, 868, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 1620, 1260, -1)->AutoRespawn();
  CreateObject(PBRL, 3660, 850, -1)->AutoRespawn();
  CreateObject(PBRL, 3670, 292, -1);
  CreateObject(PBRL, 4505, 938, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 1600, 1260, -1)->AutoRespawn();
  CreateObject(XBRL, 3280, 1208, -1)->AutoRespawn();
  CreateObject(XBRL, 4605, 918, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1635, 1030, -1)->AutoRespawn();
  CreateObject(GSBL, 4220, 818, -1)->AutoRespawn();
  CreateObject(GSBL, 5160, 1238, -1)->AutoRespawn();
*/
  //Stahlbrücken
  CreateObject(_HBR, 4815, 862, -1)->SwitchMode();
  CreateObject(_HBR, 4905, 862, -1)->SwitchMode();
  CreateObject(_HBR, 5025, 862, -1)->SwitchMode();
  CreateObject(_HBR, 5145, 862, -1)->SwitchMode();
  CreateObject(_HBR, 5235, 862, -1)->SwitchMode();

  //Gerüste
  CreateObject(SFFG, 4920, 700, -1)->Set(5);
  CreateObject(SFFG, 4920, 750, -1)->Set(3);
  CreateObject(SFFG, 4920, 800, -1)->Set(2);
  CreateObject(SFFG, 4920, 850, -1)->Set(4);
  CreateObject(SFFG, 5130, 700, -1)->Set(5);
  CreateObject(SFFG, 5130, 750, -1)->Set(2);
  CreateObject(SFFG, 5130, 800, -1)->Set(3);
  CreateObject(SFFG, 5130, 850, -1)->Set(4);

/*
  CreateObject(SFFG, 5470, 1280, -1)->Set(4);
  var plat = CreateObject(SFFG, 5470, 1330, -1);
  plat->Set(5);
  plat->SetClrModulation(RGB(125,125,250));
  CreateObject(SFFG, 5530, 1280, -1)->Set(4);
  plat = CreateObject(SFFG, 5530, 1330, -1);
  plat->Set(5);
  plat->SetClrModulation(RGB(125,125,250));
*/

  //Haie
  CreateObject(SHRK, 600, 1100, -1)->AutoRespawn();
  CreateObject(SHRK, 600, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 1200, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 3500, 1100, -1)->AutoRespawn();
  CreateObject(SHRK, 3500, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 6600, 1100, -1)->AutoRespawn();
  CreateObject(SHRK, 6600, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 8830, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 9430, 1100, -1)->AutoRespawn();
  CreateObject(SHRK, 9430, 1400, -1)->AutoRespawn();

  //Piranha
  CreateObject(PIRA, 2000, 1100, -1)->AutoRespawn();
  CreateObject(PIRA, 3000, 1100, -1)->AutoRespawn();
  CreateObject(PIRA, 3000, 1400, -1)->AutoRespawn();
  CreateObject(PIRA, 7000, 1100, -1)->AutoRespawn();
  CreateObject(PIRA, 7000, 1400, -1)->AutoRespawn();
  CreateObject(PIRA, 8000, 1100, -1)->AutoRespawn();

/*
  //Container
  CreateObject(CON1, 1780, 1171, -1)->SetGraphics("5");
  var container = CreateObject(CON1, 1820, 1171, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  container = CreateObject(CON1, 4530, 940, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");

  //Hydrauliktüren
  CreateObject(SEDR,1459, 1030,-1);
  CreateObject(SEDR,1459, 1170,-1);
  CreateObject(SEDR,1611, 1030,-1);
  CreateObject(SEDR,1611, 1170,-1);

*/

  //Explosivtanks
  CreateObject(XTNK, 2500, 1480, -1)->AutoRespawn();
  CreateObject(XTNK, 7550, 1480, -1)->AutoRespawn();

/*

  //Tore und Konsolen
  var autod = CreateObject (HNG3, 1433, 1110, -1);
  autod->Open();
  CreateObject (CONS, 1500, 1105, -1)->Set(autod);

  autod = CreateObject (HNG3, 1637, 1110, -1);
  autod->Open();
  CreateObject (CONS, 1570, 1105, -1)->Set(autod);

  //Selbstschussanlagen
  aSelfDefense[0] = CreateObject(SEGU, 1310, 1142, -1);
    aSelfDefense[0]->Arm(MISA);
    aSelfDefense[0]->SetR(180);
    CreateObject(CONS, 1420, 1025, -1)->Set(aSelfDefense[0]);

  aSelfDefense[1] = CreateObject(SEGU, 2340, 1142, -1);
    aSelfDefense[1]->Arm(MISA);
    aSelfDefense[1]->SetR(180);
    CreateObject(CONS, 1650, 1025, -1)->Set(aSelfDefense[1]);

*/

  //Seegras und Muscheln entfernen
  for(var obj in FindObjects(Find_Or(Find_ID(SWD1), Find_ID(SWD2), Find_ID(SHEL)), Find_InRect(2290, 950, 5650, 30)))
    RemoveObject(obj);

  //Verbundene Räume
  var doorw = CreateObject(GAT1, 2395, 935, -1);
  CreateObject(ROOM, 2365, 1320, -1)->Connect(doorw);
  var doorw = CreateObject(GAT1, 7655, 935, -1);
  CreateObject(ROOM, 7685, 1320, -1)->Connect(doorw);

  //Sonne umplazieren
  FindObject(LENS)->SetPosition(5025,145);

  //Sendemast
  var tower = CreateObject(AATR, 5025, 940, -1);
  tower->AddNode(4410, 1185, 2, CreateObject(REHR, 4405, 1190, -1), 0, 2);
  tower->AddNode(4748, 1135, 0, CreateObject(REHR, 4750, 1140, -1), 0, 2);
  tower->AddNode(5298, 1135, 0, CreateObject(REHR, 5300, 1140, -1), 0, 2);
  tower->AddNode(5640, 1185, 2, CreateObject(REHR, 5645, 1190, -1), 0, 2);

  //Sounds

  //Wind
  CreateObject(SE4K, 1100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 2100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 3100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 4100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 5100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 6100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 7100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 8100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 9100, 300, -1)->Set("WindSound*.ogg",3000,105);

  //Hallen
  CreateObject(SE4K, 2390, 1510, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 3990, 1620, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 6060, 1620, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 7660, 1510, -1)->Set("Interior*.ogg",665,105);

  //Möven
  CreateObject(SE4K, 1000, 950, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 2000, 950, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 3000, 950, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 7050, 950, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 8050, 950, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 9050, 950, -1)->Set("SeaSounds*.ogg",140,35);

  //Erdrutsche
  CreateObject(SE4K, 4710, 1380, -1)->Set("FallingDirt*.ogg",950,250);
  CreateObject(SE4K, 5340, 1380, -1)->Set("FallingDirt*.ogg",950,250);

  //Wellen
  CreateObject(SE4K, 500, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1000, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1500, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 2000, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 2500, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 3500, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 4000, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 4500, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 5000, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 5500, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 6000, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 6500, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 7000, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 7500, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 8000, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 8500, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 9000, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 9500, 1050, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 10000, 1050, -1)->Set("Wave*.ogg",105,35);

  //Lautsprecher
  CreateObject(SE4K, 5025, 720, -1)->Set("Announce*.ogg",3000,200);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 2240, 1480, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 3530, 1580, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 6520, 1580, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 7820, 1480, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  var crate = CreateObject(AMCT, 2290, 1630, -1);
  crate->Set(GBOX);
  crate = CreateObject(AMCT, 4890, 1460, -1);
  crate->Set(GBOX);
  crate = CreateObject(AMCT, 5160, 1460, -1);
  crate->Set(GBOX);
  crate = CreateObject(AMCT, 7760, 1630, -1);
  crate->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MBOX, 2720, 1365);
  PlaceSpawnpoint(MBOX, 7330, 1365);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");
/*
  //Radio
  CreateObject(RDIO, 1460, 980, -1);

  //Labortische
  CreateObject(LTBL, 1450, 1110, -1);
  CreateObject(LTBL, 1620, 1110, -1);

  //Monitore
  CreateObject(MNI2, 1440, 1098, -1);
  CreateObject(MNI2, 1460, 1098, -1);
  CreateObject(MNI2, 1610, 1098, -1)->Off();
  CreateObject(MNI2, 1630, 1098, -1);
  CreateObject(MNI2, 1690, 1152, -1);

  //Bildschirm
  CreateObject(SCA2, 1535, 1095, -1);
*/
  //Jetwrack
  CreateObject(_JW1, 3010, 1675, -1)->SetR(30);
  CreateObject(_JW2, 3180, 1795, -1)->SetR(20);

  //Kaputte U-Boote
  CreateObject(SUBB, 675, 1735, -1)->SetR(-10);
  CreateObject(SUBB, 6390, 1815, -1)->SetR(20);

  //Schiffswracks
  CreateObject(_WRK, 1630, 1770, -1);
  CreateObject(_WRK, 8380, 1770, -1);

  //Truhen
  CreateObject(CHST, 1500, 1750, -1);
  CreateObject(CHST, 8180, 1730, -1);

  //Kaputte Boote
  CreateObject(SLBB, 3900, 1795, -1)->SetR(-40);
  CreateObject(SLBB, 7100, 1770, -1)->SetR(10);
/*
  //Regale
  CreateObject(FRAM, 1440, 1170, -1);
  CreateObject(FRAM, 1630, 1170, -1);
  CreateObject(FRAM, 1650, 1170, -1);

*/
  //Radare
  CreateObject(RADR, 2395, 900, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(RADR, 7655, 900, -1)->SetClrModulation(RGB(125,125,125));
/*
  //Schilder
  CreateObject(MSGN, 1288, 1260, -1);
  CreateObject(MSGN, 1428, 1260, -1);
  CreateObject(ESGN, 1500, 1025, -1);
  CreateObject(MSGN, 1710, 1170, -1);
  CreateObject(MSGN, 1745, 1170, -1);
  CreateObject(MSGN, 1776, 1260, -1);
  CreateObject(MSGN, 1870, 1170, -1);
  CreateObject(MSGN, 1905, 1170, -1);
  CreateObject(MSGN, 2030, 1170, -1);
  CreateObject(MSGN, 2065, 1170, -1);
  CreateObject(MSGN, 2190, 1170, -1);
  CreateObject(MSGN, 2225, 1170, -1);
  CreateObject(MSGN, 2376, 1260, -1);

  //Apparaturen
  CreateObject(GADG, 4690, 1240, -1)->Set(1);
  CreateObject(GADG, 4700, 1240, -1)->Set(1);
  CreateObject(GADG, 4710, 1240, -1)->Set(1);

  //Orientierungslichter
  CreateObject(OLGH, 1310, 1140, -1)->Set(2, 10, 1, 1, 30);
  CreateObject(OLGH, 1540, 980, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 1960, 1170, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 2120, 1170, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 2340, 1140, -1)->Set(2, 10, 1, 1, 30);

  //Bäume
  CreateObject(TRE1, 3455, 895, -1)->SetR(-15);
  CreateObject(TRE1, 3935, 575, -1)->SetR(-10);
  CreateObject(TRE1, 4280, 615, -1)->SetR(10);
  CreateObject(TRE1, 4630, 925, -1)->SetR(15);

  //Büsche
  CreateObject(BSH2, 3430, 990, -1);
  CreateObject(BSH2, 3780, 1080, -1);
  CreateObject(BSH2, 3880, 1070, -1);
  CreateObject(BSH2, 3870, 720, -1);
  CreateObject(BSH2, 3980, 545, -1);
  CreateObject(BSH2, 4000, 540, -1);
  CreateObject(BSH2, 4020, 540, -1);
  CreateObject(BSH2, 4040, 545, -1);
  CreateObject(BSH2, 4300, 670, -1);
  CreateObject(BSH2, 4365, 1010, -1);
  CreateObject(BSH2, 4390, 1020, -1);
  CreateObject(BSH2, 4430, 1040, -1);
  CreateObject(BSH2, 4600, 1000, -1);
  CreateObject(BSH2, 4760, 1100, -1);
  CreateObject(BSH2, 5445, 1250, -1);
  CreateObject(BSH2, 5520, 1255, -1);
*/
  //Steine
  CreateObject(STNE, 305, 1670, -1);
  CreateObject(STNE, 1920, 1720, -1);
  CreateObject(STNE, 2770, 1730, -1);
  CreateObject(STNE, 3655, 1790, -1);
  CreateObject(STNE, 4410, 1630, -1);
  CreateObject(STNE, 6060, 1700, -1);
  CreateObject(STNE, 6890, 1780, -1);
  CreateObject(STNE, 8550, 1750, -1);
  CreateObject(STNE, 9400, 1730, -1);
/*
  //Spind
  CreateObject(LCKR, 1435, 1170, -1);

  //Rohre
  CreateObject(PIPL, 1290, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1320, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1950, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1980, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 2110, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 2140, 1325, -1)->Up()->Up();

  //Geländer
  CreateObject(RAI1, 1409, 1030, -1)->SetRail([3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,2,2,2,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3]);
  CreateObject(RAI1, 1265, 1140, -1)->SetRail([1,3,1,3,1]);
  var rail = CreateObject(RAI1, 1280, 1260, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 19; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 1463, 980, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  rail = CreateObject(RAI1, 1728, 1260, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 65; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 2318, 1140, -1)->SetRail([1,3,1,3,1]);
*/
  //Lüftungsgitter
  CreateObject(ENGT, 2470, 1320, -1);
  CreateObject(ENGT, 3760, 1570, -1);
  CreateObject(ENGT, 4200, 1570, -1);
  CreateObject(ENGT, 5850, 1570, -1);
  CreateObject(ENGT, 6290, 1570, -1);
  CreateObject(ENGT, 7580, 1320, -1);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD03,500,250,-1);
  CreateObject(BD03,2000,350,-1);
  CreateObject(BD03,3500,200,-1);
}

/* Bei Turmzusammenfall */

func OnTowerCollapse()
{
  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten verschieben
   aFlag[2]->MoveFlagpost(5025,940,1,"$Flag6$");
  }
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{

}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Blue Ressort.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Blue Ressort.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,2390,1480,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(2305,1310);
   aFlag[0] -> AddSpawnPoint(2485,1310);
   aFlag[0] -> AddSpawnPoint(2390,1310);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,3985,1580,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(4065,1410);
   aFlag[1] -> AddSpawnPoint(4135,1410);
   aFlag[1] -> AddSpawnPoint(4180,1510);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,5025,564,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(4780,1230);
   aFlag[2] -> AddSpawnPoint(4910,1450);
   aFlag[2] -> AddSpawnPoint(5140,1450);
   aFlag[2] -> AddSpawnPoint(5270,1230);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,6065,1580,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(5870,1510);
   aFlag[3] -> AddSpawnPoint(5915,1410);
   aFlag[3] -> AddSpawnPoint(5985,1410);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,7660,1480,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(7565,1310);
   aFlag[4] -> AddSpawnPoint(7660,1310);
   aFlag[4] -> AddSpawnPoint(7745,1310);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag4$",0,2);
   }

   //Helikopter und Hinweisschild
   if(!FindObject(NOHC))
   {
    SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,2340,940,-1),100*21);
    SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,7710,940,-1),100*21);

    var sign = CreateObject(SGNP, 2470, 970, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
    sign = CreateObject(SGNP, 7580, 970, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
   }

/*
   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1); aSelfDefense[1]->SetTeam(1);}

   //SSA aktivieren
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();
*/

   //Grenzen setzen
   CreateObject(BRDR, 960, 0, -1)->Set(0);
   CreateObject(BRDR, 9090, 0, -1)->Set(1);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2220,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2520,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4900,940,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,5150,940,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,7530,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,7830,1009,-1),50*21);
  }

  //Assault-Spielziel
  if (FindObject(GASS))
  {
   //Zielobjekte
   AddAssaultTarget(GSTA, 3260, 1210, 30*30, 2, "$Target1$", 0, [[[3550, 1278], [3690, 850], [3755, 800]], [[1640, 1260], [1660, 1170], [2120, 1170]]]);
   AddAssaultTarget(GSTA, 3700, 1050, 30*30, 2, "$Target1$", 1, [[[3550, 1278], [3690, 850], [3755, 800]], [[1640, 1260], [1660, 1170], [2120, 1170]]]);
   AddAssaultTarget(RADR, 4010, 530, 30*30, 2, "$Target2$", 2, [[[4300, 970], [4465, 1050], [4530, 911]], [[1640, 1260], [1660, 1170], [2120, 1170]]]);
   AddAssaultTarget(CMSN, 4245, 820, 30*30, 2, "$Target3$", 3, [[[4300, 970], [4465, 1050], [4530, 911]], [[1640, 1260], [1660, 1170], [2120, 1170]]]);
   AddAssaultTarget(HUT3, 4760, 1090, 30*30, 2, "$Target4$", 4, [[[4995, 1240], [5105, 1190], [5280, 1260]], [[3690, 850], [3805, 790], [3890, 660]]]);
   AddAssaultTarget(CMSN, 4710, 1240, 30*30, 2, "$Target5$", 5, [[[4995, 1240], [5105, 1190], [5280, 1260]], [[3690, 850], [3805, 790], [3890, 660]]]);
   AddAssaultTarget(LBPC, 4180, 1270, 30*30, 2, "$Target3$", 6, [[[4995, 1240], [5105, 1190], [5280, 1260]], [[3690, 850], [3805, 790], [3890, 660]]]);

   //Ziele verbinden
   ConnectAssaultTargets([0, 1]);
   ConnectAssaultTargets([2, 3]);
   ConnectAssaultTargets([4, 5, 6]);

   //Leitern
   CreateObject(LADR, 4310, 670, -1)->Set(15, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
   CreateObject(LADR, 4330, 715, -1)->Set(5, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");

   //Container
   CreateObject(CON1, 4215, 582, -1);
   CreateObject(CON1, 4265, 582, -1);
   CreateObject(CON1, 4280, 610, -1)->SetPerspective(2);

   //Metallkisten
   CreateObject(MWCR, 4290, 720, -1);
   CreateObject(MWCR, 4300, 702, -1);
   CreateObject(MWCR, 4310, 720, -1);

   //Steine
   CreateObject(STNE, 3710, 1300, -1)->Set(2);
   CreateObject(STNE, 3760, 1310, -1)->Set(3);
   CreateObject(STNE, 3820, 1215, -1)->Set(1);
   CreateObject(STNE, 3770, 1288, -1)->Set(3);
   CreateObject(STNE, 3835, 1340, -1);
   CreateObject(STNE, 3890, 1300, -1)->Set(3);

   //Boden
   DrawMaterialQuad("Wall-Stripes", 4130,1270, 4230,1270, 4230,1280, 4130,1280);

   //Erde
   Schedule("CastPXS(\"Earth\", 1250, 10, 3820, 1280)", 15, 8);

   //Rauch
   Schedule("CastSmoke(\"Smoke3\", 2,30,3820,1270,1000,1200,RGBa(150,40,0,0))", 10, 8);
   Schedule("CastSmoke(\"GunSmoke\", 2, 50, 3820, 1270, 1000, 1500, RGBa(250, 250, 250, 0))", 10, 8);

   //Sound
   Sound("Blast3");
   Sound("Earthquake");

   //Grenzen setzen
   CreateObject(BRDR, 910, 0, -1)->Set(0);
   CreateObject(BRDR, 3900, 0, -1)->Set(1,1);
   CreateObject(BRDR, 0, 700, -1)->Set(2,1);

   //Hinweisschilder
   CreateObject(SGNP, 830, 1220, -1);
   CreateObject(SGNP, 6010, 1210, -1);

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1); aSelfDefense[1]->SetTeam(1);}

   //SSA aktivieren
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();

   //Versorgungskiste (APW)
   var crate = CreateObject(AMCT, 5540, 1230, -1);
   crate->Set(ATWN);

   //Geschützstellungen
   aStationary[0] = CreateObject(GNET, 1795, 1141, -1);
   aStationary[0] -> Set(0,90,1);
   aStationary[1] = CreateObject(GNET, 3410, 1130, -1);
   aStationary[1] -> Set(0,-90,1);
   aStationary[2] = CreateObject(GNET, 3460, 890, -1);
   aStationary[2] -> Set(SATW,-90,1);
   aStationary[3] = CreateObject(GNET, 3880, 660, -1);
   aStationary[3] -> Set(SATW,-90);
   aStationary[4] = CreateObject(GNET, 5060, 1170, -1);
   aStationary[4] -> Set(SATW);
   aTowerInterior[0] = CreateObject(GNET, 3640, 473, -1);
   aTowerInterior[0] -> Set(SATW,-90);
   aTowerInterior[1] = CreateObject(GNET, 3700, 473, -1);
   aTowerInterior[1] -> Set(SATW,90);

   //MAV-Station
   CreateObject(MVSN, 4805, 1210, -1)->Set(4885,1165,1);

   //Helikopter und Hinweisschilder
   if(!FindObject(NOHC))
   {
    SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,1970,1140,-1),50*21);
    SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,5495,1200,-1),50*21);

    var sign = CreateObject(SGNP, 1640, 1170, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
    CreateObject(SGNP, 5405, 1270, -1)->Set("Helicopter");
   }

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1821,1270,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1891,1270,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,3610,1269,-1),50*21);

   //Objekt entfernen
   RemoveObject(FindObject2(Find_ID(LADR),Find_InRect(4230, 580, 20, 200)));
  }

  //HTF-Spielziel
  if (FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 4010,530, -1);
   flag->~Set("$Flag3$");

   //Grenzen setzen
   CreateObject(BRDR, 2890, 0, -1)->Set(0);
   CreateObject(BRDR, 6010, 0, -1)->Set(1);

   //Teamgrenzen
   CreateObject(BRDR, 3620, 0, -1)->Set(0,1,0,1,1);
   CreateObject(BRDR, 4600, 0, -1)->Set(1,1,0,1,2);

   //Hinweisschild
   CreateObject(SGNP, 6010, 1210, -1);

   //Leitern
   CreateObject(LADR, 2965, 1267, -1)->Set(4);
   CreateObject(LADR, 3115, 1267, -1)->Set(4);

   //Gerüste
   CreateObject(SFFG, 3010, 1280, -1)->Set(4);
   var plat = CreateObject(SFFG, 3010, 1330, -1);
   plat->Set(5);
   plat->SetClrModulation(RGB(125,125,250));
   CreateObject(SFFG, 3070, 1280, -1)->Set(4);
   plat = CreateObject(SFFG, 3070, 1330, -1);
   plat->Set(5);
   plat->SetClrModulation(RGB(125,125,250));

   //Boje
   CreateObject(BUOY, 2890, 1293,  -1);

   //Büsche
   CreateObject(BSH2, 3030, 1260, -1);
   CreateObject(BSH2, 3050, 1275, -1);

   //Helikopter und Hinweisschilder
   if(!FindObject(NOHC))
   {
    SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,3045,1200,-1),50*21);
    SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,5495,1200,-1),50*21);

    CreateObject(SGNP, 3160, 1280, -1)->Set("Helicopter");
    CreateObject(SGNP, 5405, 1270, -1)->Set("Helicopter");
   }

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3210,1269,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4235,1269,-1),50*21);
  }

  //MR-Spielziel
  if (FindObject(GMNR))
  {
   //Geldsäcke
   AddMoneySpawn(3780, 980, [20]);
   AddMoneySpawn(4040, 1270, [20]);
   AddMoneySpawn(4080, 550, [20]);
   AddMoneySpawn(4480, 930, [20]);

   //Grenzen setzen
   CreateObject(BRDR, 3130, 0, -1)->Set(0);
   CreateObject(BRDR, 5310, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 3160, 1280, -1);
   CreateObject(SGNP, 3260, 1210, -1);
   CreateObject(SGNP, 3420, 1130, -1);
   CreateObject(SGNP, 5065, 1170, -1);
   CreateObject(SGNP, 5100, 1250, -1);
   CreateObject(SGNP, 5180, 1240, -1);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3210,1269,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4235,1269,-1),50*21);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,3710,1050,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,4480,940,GetTeamColor(2));}

   //Erde
   Schedule("CastPXS(\"Earth\", 1250, 10, 3960, 1090)", 10, 2);
   Schedule("CastPXS(\"Earth\", 1250, 10, 4250, 1060)", 10, 4);

   //Rauch
   Schedule("CastSmoke(\"Smoke3\", 2,30,3980,1090,1000,1200,RGBa(150,40,0,0))", 10, 8);
   Schedule("CastSmoke(\"Smoke3\", 2,30,4250,1060,1000,1200,RGBa(150,40,0,0))", 10, 8);
   Schedule("CastSmoke(\"GunSmoke\", 2, 50, 3980, 1090, 1000, 1500, RGBa(250, 250, 250, 0))", 10, 8);
   Schedule("CastSmoke(\"GunSmoke\", 2, 50, 4250, 1060, 1000, 1500, RGBa(250, 250, 250, 0))", 10, 8);

   //Sound
   Sound("Blast3");
   Sound("Earthquake");

   //Grenzen setzen
   CreateObject(BRDR, 3130, 0, -1)->Set(0);
   CreateObject(BRDR, 5310, 0, -1)->Set(1);

   //Container
   CreateObject(CON1, 4420, 860, -1);
   CreateObject(CON1, 4420, 889, -1)->SetPerspective(2);
   CreateObject(CON1, 4125, 640, -1)->SetPerspective(2);

   //Hinweisschilder
   CreateObject(SGNP, 3160, 1280, -1);
   CreateObject(SGNP, 3260, 1210, -1);
   CreateObject(SGNP, 3420, 1130, -1);
   CreateObject(SGNP, 5065, 1170, -1);
   CreateObject(SGNP, 5100, 1250, -1);
   CreateObject(SGNP, 5180, 1240, -1);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3210,1269,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4235,1269,-1),50*21);

   //Spawnpoint entfernen
   RemoveObject(FindObject2(Find_ID(RSPT),Find_InRect(4119, 1069, 2, 2)));
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   //Grenzen setzen
   CreateObject(BRDR, 3130, 0, -1)->Set(0);
   CreateObject(BRDR, 5310, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 3160, 1280, -1);
   CreateObject(SGNP, 3260, 1210, -1);
   CreateObject(SGNP, 3420, 1130, -1);
   CreateObject(SGNP, 5065, 1170, -1);
   CreateObject(SGNP, 5100, 1250, -1);
   CreateObject(SGNP, 5180, 1240, -1);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3210,1269,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4235,1269,-1),50*21);
  }
}

/* Assault Zerstörung */

public func OnAssaultTargetDestruction(object pTarget, int iTeam, int iIndex, bool fConnectedDestroyed)
{
  //Ziel 1 und 2
  if (!iIndex || iIndex == 1)
  {
   if(fConnectedDestroyed)
   {
    //Grenze neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 910, 0, -1)->Set(0,1);
    CreateObject(BRDR, 4580, 0, -1)->Set(1,1);

    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),3760,0,1,1);

    //Geschützstellungen entfernen
    aStationary[1]->DecoExplode(30);
    aStationary[2]->DecoExplode(30);
   }
  }

  //Ziel 3 und 4
  if (iIndex == 2 || iIndex == 3)
  {
   if(fConnectedDestroyed)
   {
    //Grenze neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 910, 0, -1)->Set(0,1);
    CreateObject(BRDR, 5970, 0, -1)->Set(1,1);

    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),4370,0,1,1);

    //Spawnpoints entfernen
    RemoveObject(FindObject2(Find_ID(VSPW),Find_InRect(1969, 1139, 2, 2)));
    RemoveObject(FindObject2(Find_ID(VSPW),Find_InRect(3609,1279, 2, 2)));

    //Geschützstellung entfernen
    aStationary[3]->DecoExplode(30);
   }
  }

  //Ziel 5, 6 und 7
  if (iIndex == 4 || iIndex == 5 || iIndex == 6)
  {
   if(fConnectedDestroyed)
   {
    //Geschützstellung entfernen
    aStationary[4]->DecoExplode(30);
   }
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 5025; iY = 760;
   return(1);
  }

  //HTF/MR/CTF/LMS/DM-Spielziel
  if(FindObject(GHTF) || FindObject(GMNR) || FindObject(GCTF) || FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    return [[3150, 1270], [3315, 1210], [3380, 1270], [3420, 1120], [3550, 1270]];
   }
   if(iTeam == 2)
   {
    return [[4720, 1230], [4810, 1200], [4820, 1090], [4890, 1160]];
   }
   return(1);
  }
}