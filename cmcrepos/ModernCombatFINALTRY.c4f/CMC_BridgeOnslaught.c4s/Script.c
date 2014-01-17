/*-- Bridge Onslaught --*/

#strict
#include CSTD

static aFlag,aSelfDefense,switchright,switchleft;

func RecommendedGoals()	{return [GBAS, GHTF];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(0, 15, 15);
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
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
  CreateObject(LADR, 400, 630, -1)->Set(24);
  CreateObject(LADR, 495, 421, -1)->Set(18);
  CreateObject(LADR, 585, 775, -1)->Set(16);
  CreateObject(LADR, 755, 623, -1)->Set(22);
  CreateObject(LADR, 810, 778, -1)->Set(15);

  CreateObject(LADR, 1305, 621, -1)->Set(38);
  CreateObject(LADR, 1365, 791, -1)->Set(18);
  CreateObject(LADR, 1425, 621, -1)->Set(38);

  CreateObject(LADR, 1920, 778, -1)->Set(15);
  CreateObject(LADR, 1975, 623, -1)->Set(22);
  CreateObject(LADR, 2145, 775, -1)->Set(16);
  CreateObject(LADR, 2235, 421, -1)->Set(18);
  CreateObject(LADR, 2330, 630, -1)->Set(24);

  //Rampen
  DrawMaterialQuad("Wall-Concrete3",330,440,360,430,360,440,345,440,true);
  DrawMaterialQuad("Wall-Concrete3",471,440,441,430,441,440,456,440,true);

  DrawMaterialQuad("Wall-Concrete3",331,641,361,631,361,641,346,641,true);
  DrawMaterialQuad("Wall-Concrete3",470,641,440,631,440,641,455,641,true);

  DrawMaterialQuad("Wall-Concrete3",801,650,771,640,771,650,786,650,true);
  DrawMaterialQuad("Wall-Concrete3",820,650,850,640,850,650,835,650,true);

  DrawMaterialQuad("Wall-Concrete3",1911,650,1881,640,1881,650,1896,650,true);
  DrawMaterialQuad("Wall-Concrete3",1930,650,1960,640,1960,650,1945,650,true);

  DrawMaterialQuad("Wall-Concrete3",2261,641,2291,631,2291,641,2276,641,true);
  DrawMaterialQuad("Wall-Concrete3",2400,641,2370,631,2370,641,2385,641,true);

  DrawMaterialQuad("Wall-Concrete3",2260,440,2290,430,2290,440,2275,440,true);
  DrawMaterialQuad("Wall-Concrete3",2401,440,2371,430,2371,440,2386,440,true);

  //Bodenluken
  CreateObject(HA4K, 400, 433, -1);
  CreateObject(HA4K, 810, 653, -1);

  CreateObject(HA4K, 1920, 653, -1);
  CreateObject(HA4K, 2330, 433, -1);

  //Große Bodenluken
  CreateObject(H24K, 585, 648, -1);
  CreateObject(H24K, 755, 448, -1);

  CreateObject(H24K, 1305, 318, -1);
  CreateObject(H24K, 1305, 448, -1);
  CreateObject(H24K, 1305, 528, -1);

  CreateObject(H24K, 1365, 648, -1);

  CreateObject(H24K, 1425, 318, -1);
  CreateObject(H24K, 1425, 448, -1);
  CreateObject(H24K, 1425, 528, -1);

  CreateObject(H24K, 1975, 448, -1);
  CreateObject(H24K, 2145, 648, -1);

  //Glasscheiben
  CreateObject(PANE, 1262, 510, -1);
  CreateObject(PANE, 1262, 520, -1);
  CreateObject(PANE, 1468, 510, -1);
  CreateObject(PANE, 1468, 520, -1);

  //Explosivtanks
  CreateObject(XTNK, 230, 440, -1)->AutoRespawn();
  CreateObject(XTNK, 1200, 800, -1)->AutoRespawn();
  CreateObject(XTNK, 1530, 800, -1)->AutoRespawn();
  CreateObject(XTNK, 2500, 440, -1)->AutoRespawn();

  //Container
  var container = CreateObject(CON1, 25, 442, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  CreateObject(CON1, 75, 440, -1)->SetGraphics("5");
  container = CreateObject(CON1, 120, 640, -1);
  container->SetPerspective();
  container->SetGraphics("5");
  container = CreateObject(CON1, 250, 640, -1);
  container->SetPerspective(3);
  container->SetGraphics("5");
  container = CreateObject(CON1, 265, 614, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  container = CreateObject(CON1, 530, 440, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  CreateObject(CON1, 530, 640, -1)->SetGraphics("5");
  container = CreateObject(CON1, 1365, 440, -1);
  container->SetPerspective(3);
  container->SetGraphics("5");
  container = CreateObject(CON1, 2200, 440, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  CreateObject(CON1, 2200, 640, -1)->SetGraphics("5");
  container = CreateObject(CON1, 2475, 642, -1);
  container->SetPerspective();
  container->SetGraphics("5");
  container = CreateObject(CON1, 2500, 614, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  CreateObject(CON1, 2530, 642, -1)->SetGraphics("5");
  container = CreateObject(CON1, 2680, 440, -1);
  container->SetPerspective(3);
  container->SetGraphics("5");

  //Autos
  var car = CreateObject(CCAR, 50, 641, -1);
  car->SetPhase(0);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  CreateObject(CCAR, 695, 640, -1);
  CreateObject(CCAR, 2035, 640, -1);
  var car = CreateObject(CCAR, 2610, 641, -1);
  car->SetPhase(0);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  var car = CreateObject(CCAR, 2680, 641, -1);
  car->SetPhase(0);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);

  //Kisten
  CreateObject(WCR2, 610, 640, -1)->AutoRespawn();
  CreateObject(WCR2, 915, 640, -1);
  CreateObject(WCR2, 1270, 440, -1)->AutoRespawn();
  CreateObject(WCR2, 1400, 640, -1)->AutoRespawn();
  CreateObject(WCR2, 1415, 640, -1);
  CreateObject(WCR2, 1405, 622, -1);
  CreateObject(WCR2, 1815, 640, -1);
  CreateObject(WCR2, 2120, 640, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 75, 413, -1);
  CreateObject(MWCR, 295, 640, -1);
  CreateObject(MWCR, 570, 800, -1)->AutoRespawn();
  CreateObject(MWCR, 650, 270, -1)->AutoRespawn();
  CreateObject(MWCR, 1065, 640, -1)->AutoRespawn();
  CreateObject(MWCR, 1395, 310, -1)->AutoRespawn();
  CreateObject(MWCR, 1665, 640, -1)->AutoRespawn();
  CreateObject(MWCR, 2082, 270, -1)->AutoRespawn();
  CreateObject(MWCR, 2160, 800, -1)->AutoRespawn();
  CreateObject(MWCR, 2635, 440, -1);

  //Verbandskisten
  CreateObject(BECR, 590, 800, -1)->AutoRespawn();
  CreateObject(BECR, 2140, 800, -1)->AutoRespawn();

  //Sandsackbarrieren
  CreateObject(SBBA, 790, 800, -1)->Right();
  CreateObject(SBBA, 960, 640, -1);
  CreateObject(SBBA, 1160, 800, -1);
  CreateObject(SBBA, 1260, 640, -1);
  CreateObject(SBBA, 1470, 640, -1)->Right();
  CreateObject(SBBA, 1570, 800, -1)->Right();
  CreateObject(SBBA, 1770, 640, -1)->Right();
  CreateObject(SBBA, 1940, 800, -1);

  //Stahlbrücken
  CreateObject(_HBR, 595, 452, -1)->SwitchMode();
  CreateObject(_HBR, 685, 452, -1)->SwitchMode();
  CreateObject(_HBR, 825, 452, -1)->SwitchMode();
  CreateObject(_HBR, 925, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1025, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1125, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1225, 452, -1)->SwitchMode();
  CreateObject(_HBR, 915, 652, -1)->SwitchMode();
  CreateObject(_HBR, 1115, 652, -1)->SwitchMode();
  CreateObject(_HBR, 1215, 652, -1)->SwitchMode();

  CreateObject(_HBR, 1515, 652, -1)->SwitchMode();
  CreateObject(_HBR, 1615, 652, -1)->SwitchMode();
  CreateObject(_HBR, 1815, 652, -1)->SwitchMode();
  CreateObject(_HBR, 1505, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1605, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1705, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1805, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1905, 452, -1)->SwitchMode();
  CreateObject(_HBR, 2045, 452, -1)->SwitchMode();
  CreateObject(_HBR, 2135, 452, -1)->SwitchMode();

  //Gasflaschen
  CreateObject(GSBL, 540, 613, -1)->AutoRespawn();
  CreateObject(GSBL, 630, 800, -1)->AutoRespawn();
  CreateObject(GSBL, 995, 439, -1);
  CreateObject(GSBL, 1045, 640, -1)->AutoRespawn();
  CreateObject(GSBL, 1685, 640, -1)->AutoRespawn();
  CreateObject(GSBL, 1735, 439, -1);
  CreateObject(GSBL, 2100, 800, -1)->AutoRespawn();
  CreateObject(GSBL, 2190, 613, -1)->AutoRespawn();

  //Explosive Kiste
  CreateObject(XWCR, 975, 440, -1)->AutoRespawn();
  CreateObject(XWCR, 1755, 440, -1)->AutoRespawn();

  //Giftfässer
  CreateObject(TBRL, 365, 430, -1)->AutoRespawn();
  CreateObject(TBRL, 2365, 430, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 990, 800, -1)->AutoRespawn();
  CreateObject(XBRL, 1225, 800, -1)->AutoRespawn();
  CreateObject(XBRL, 1350, 310, -1)->AutoRespawn();
  CreateObject(XBRL, 1370, 310, -1)->AutoRespawn();
  CreateObject(XBRL, 1290, 640, -1)->AutoRespawn();
  CreateObject(XBRL, 1505, 800, -1)->AutoRespawn();
  CreateObject(XBRL, 1745, 800, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 1360, 310, -1)->AutoRespawn();
  CreateObject(HBRL, 1300, 640, -1)->AutoRespawn();

  //Paletten
  CreateObject(PLLT, 560, 270, -1);
  CreateObject(PLLT, 2170, 270, -1);

  //Tore
  CreateObject (HNG2, 175, 440, -1);
  CreateObject (HNG2, 365, 630, -1);
  CreateObject (HNG2, 2365, 630, -1);
  CreateObject (HNG2, 2555, 440, -1);

  //Steine
  CreateObject(STNE, 1290, 800, -1)->Set(0);
  CreateObject(STNE, 1450, 800, -1)->Set(1);

  //Hydrauliktüren
  var door = CreateObject(SLDR, 270, 530, -1);
  door->Lock();
  door->SetMaxDamage(-1);
  door->SetSwitchLock(DIR_Right);
  door = CreateObject(SLDR, 2460, 530, -1);
  door->Lock();
  door->SetMaxDamage(-1);
  door->SetSwitchLock(DIR_Left);

  //Verbundene Räume
  var doorw = CreateObject(ROOM, 200, 530, -1);
  CreateObject(ROOM, 2530, 530, -1)->Connect(doorw);

  //Grenze
  CreateObject(BRDR, 0, 850, -1)->Set(3,0,1);

  //Selbstschussanlagen und Konsolen
  aSelfDefense[0] = CreateObject(SEGU, 590, 329, -1);
  CreateObject(CONS, 310, 525, -1)->Set(aSelfDefense[0]);

  aSelfDefense[1] = CreateObject(SEGU, 1252, 556, -1);
  aSelfDefense[1]->Set(0,0,0,90);
  CreateObject(CONS, 1280, 515, -1)->Set(aSelfDefense[1]);

  aSelfDefense[2] = CreateObject(SEGU, 1478, 556, -1);
  aSelfDefense[2]->Set(0,0,0,-90);
  CreateObject(CONS, 1450, 515, -1)->Set(aSelfDefense[2]);

  aSelfDefense[3] = CreateObject(SEGU, 2140, 329, -1);
  CreateObject(CONS, 2420, 525, -1)->Set(aSelfDefense[3]);

  //Hinweisschilder
  var sign = CreateObject(SNPT, 190, 655, -1);
  sign->SetMode(1);
  sign->SetAction("Sign6");
  sign = CreateObject(SNPT, 2530, 655, -1);
  sign->SetMode(1);
  sign->SetAction("Sign6");

  //Grenzen
  CreateObject(BRDR, 170, 0, -1)->Set(0);
  CreateObject(BRDR, 2560, 0, -1)->Set(1);

  //Sounds

  //Wind
  CreateObject(SE4K, 580, 170, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 1365, 170, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 2150, 170, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 90, 410, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 540, 560, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 960, 670, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1365, 550, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1770, 670, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2190, 560, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2635, 410, -1)->Set("Interior*.ogg",665,105);

  //Rush Hour
  CreateObject(SE4K, 1075, 880, -1)->Set("Traffic*.ogg",245,70);
  CreateObject(SE4K, 1655, 880, -1)->Set("Traffic*.ogg",245,70);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject (AMCT, 530, 730, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 2200, 730, -1);
  crate->Set(ABOX);

  //Versorgungskiste (Gewehrgranaten)
  crate = CreateObject (AMCT, 1380, 411, -1);
  crate->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MBOX, 755, 715);
  PlaceSpawnpoint(MBOX, 1975, 715);

  //Automaten
  var store = CreateObject(WPVM,1365, 520,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(BWTH,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(ATWN,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
  store->AddWare(SRBL,-1);

  //Artilleriebatterien
  CreateObject(ATBY,1260,310,-1)->SetRotation(-20);
  CreateObject(ATBY,1470,310,-1)->SetRotation(20);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Leitkegel
  CreateObject(TCCN, 385, 430, -1);
  CreateObject(TCCN, 415, 430, -1);
  CreateObject(TCCN, 850, 800, -1);
  CreateObject(TCCN, 870, 800, -1);
  CreateObject(TCCN, 890, 800, -1)->Light();
  CreateObject(TCCN, 1840, 800, -1)->Light();
  CreateObject(TCCN, 1860, 800, -1);
  CreateObject(TCCN, 1880, 800, -1);
  CreateObject(TCCN, 2315, 430, -1);
  CreateObject(TCCN, 2345, 430, -1);

  //Geländer
  CreateObject(RAI1, 505, 270, -1)->SetRail([1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 725, 440, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1227, 310, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1945, 440, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 2075, 270, -1)->SetRail([1,1,1,1,1,1,1,1]);

  //Radare
  CreateObject(RADR, 1020, 310, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(RADR, 1700, 310, -1)->SetClrModulation(RGB(125,125,125));

  //Spinde
  CreateObject(LCKR, 220, 530, -1);
  CreateObject(LCKR, 240, 530, -1);
  CreateObject(LCKR, 2490, 530, -1);
  CreateObject(LCKR, 2510, 530, -1);

  //Automaten
  CreateObject(VGMN, 615, 800, -1)->Set(3);
  CreateObject(VGMN, 2115, 800, -1);

  //Glastische
  CreateObject(GTBL, 660, 800, -1);
  CreateObject(GTBL, 2065, 800, -1);

  //Wandlampen
  CreateObject(BLGH, 635, 400, -1);
  CreateObject(BLGH, 635, 600, -1);
  CreateObject(BLGH, 1075, 400, -1);

  CreateObject(BLGH, 1655, 400, -1);
  CreateObject(BLGH, 2095, 400, -1);
  CreateObject(BLGH, 2095, 600, -1);

  //Notausgangslichter
  CreateObject(ETLT, 750, 770, -1);
  CreateObject(ETLT, 1980, 770, -1);

  //Verkehrsampeln
  CreateObject(TLGH, 260, 440, -1);
  CreateObject(TLGH, 425, 630, -1);
  CreateObject(TLGH, 2305, 630, -1);
  CreateObject(TLGH, 2470, 440, -1);

  //Bildschirme
  CreateObject(SCR3, 360, 520, -1);
  CreateObject(SCA2, 640, 765, -1)->SetAction("News");
  CreateObject(SCA2, 700, 765, -1)->SetAction("Clonk");
  CreateObject(SCA2, 2030, 765, -1)->SetAction("Wompf");
  CreateObject(SCA2, 2090, 765, -1)->SetAction("News");
  CreateObject(SCR3, 2370, 520, -1);

  //Monitore
  CreateObject(MNI2, 650, 788, -1);
  CreateObject(MNI2, 665, 788, -1);
  CreateObject(MNI2, 2055, 788, -1);
  CreateObject(MNI2, 2070, 788, -1);

  //Flaschen
  CreateObject(BOTL, 550, 612, -1);
  CreateObject(BOTL, 1360, 412, -1);
  CreateObject(BOTL, 2180, 612, -1);

  //Dekoschleusen
  CreateObject(GAT1, 1075, 520, -1);
  CreateObject(GAT1, 1655, 520, -1);

  //Scheinwerfer
  CreateObject(FLH2, 190, 440, -1)->SetRotation(30);
  CreateObject(FLH2, 2535, 440, -1)->SetRotation(-30);

  //Zäune
  CreateObject(FENC, 210, 440, -1)->Set(2);
  CreateObject(FENC, 270, 440, -1);
  CreateObject(FENC, 330, 440, -1);
  CreateObject(FENC, 470, 640, -1)->Set(2);
  CreateObject(FENC, 530, 640, -1);
  CreateObject(FENC, 590, 640, -1);
  CreateObject(FENC, 650, 640, -1);
  CreateObject(FENC, 710, 640, -1);
  CreateObject(FENC, 2020, 640, -1);
  CreateObject(FENC, 2080, 640, -1);
  CreateObject(FENC, 2140, 640, -1);
  CreateObject(FENC, 2200, 640, -1);
  CreateObject(FENC, 2260, 640, -1)->Set(2);
  CreateObject(FENC, 2400, 440, -1);
  CreateObject(FENC, 2460, 440, -1);
  CreateObject(FENC, 2520, 440, -1)->Set(2);

  //Regale
  CreateObject(FRAM, 755, 800, -1);
  CreateObject(FRAM, 775, 800, -1);
  CreateObject(FRAM, 1330, 520, -1);
  CreateObject(FRAM, 1400, 520, -1);
  CreateObject(FRAM, 1950, 800, -1);
  CreateObject(FRAM, 1970, 800, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 40, 430, -1);
  CreateObject(ENGT, 140, 430, -1);
  CreateObject(ENGT, 40, 630, -1);
  CreateObject(ENGT, 140, 630, -1);
  CreateObject(ENGT, 240, 630, -1);
  CreateObject(ENGT, 340, 630, -1);
  CreateObject(ENGT, 2390, 630, -1);
  CreateObject(ENGT, 2490, 630, -1);
  CreateObject(ENGT, 2590, 630, -1);
  CreateObject(ENGT, 2690, 630, -1);
  CreateObject(ENGT, 2590, 430, -1);
  CreateObject(ENGT, 2690, 430, -1);

  //Feuerlöscher
  CreateObject(FIE2, 115, 435, -1);
  CreateObject(FIE2, 315, 635, -1);
  CreateObject(FIEX, 480, 725, -1);
  CreateObject(FIEX, 2250, 725, -1);
  CreateObject(FIE2, 2415, 635, -1);
  CreateObject(FIE2, 2615, 435, -1);

  //Büsche
  CreateObject(BSH2, 505, 515, -1);
  CreateObject(BSH2, 1350, 815, -1)->SetR(-50);
  CreateObject(BSH2, 1365, 815, -1)->SetR(90);
  CreateObject(BSH2, 1390, 820, -1);
  CreateObject(BSH2, 1455, 570, -1);
  CreateObject(BSH2, 1780, 820, -1);
  CreateObject(BSH2, 2190, 515, -1);

  //Ventillatoren
  CreateObject(VEN3, 490, 595, -1)->SetCon(20);
  var vent = CreateObject(VEN3, 510, 595, -1);
  vent->SetCon(20);
  vent->SetPhase(4);
  vent = CreateObject(VEN3, 530, 595, -1);
  vent->SetCon(20);
  vent->SetPhase(7);
  CreateObject(VENT, 975, 595, -1)->SetCon(20);
  CreateObject(VENT, 1075, 595, -1)->SetCon(20);
  CreateObject(VENT, 1175, 595, -1)->SetCon(20);
  CreateObject(VENT, 1555, 595, -1)->SetCon(20);
  CreateObject(VENT, 1655, 595, -1)->SetCon(20);
  CreateObject(VENT, 1755, 595, -1)->SetCon(20);
  vent = CreateObject(VEN3, 2200, 595, -1);
  vent->SetCon(20);
  vent->SetPhase(7);
  vent = CreateObject(VEN3, 2220, 595, -1);
  vent->SetCon(20);
  vent->SetPhase(4);
  CreateObject(VEN3, 2240, 595, -1)->SetCon(20);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  var back = CreateObject(BD10,300,900,-1);
  back->SetClrModulation(RGB(130,130,130));
  back->Local()=65;
  back->Local(1)=80;
  back = CreateObject(BD10,1690,900,-1);
  back->SetClrModulation(RGB(130,130,130));
  back->Local()=65;
  back->Local(1)=80;
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[0])
   aSelfDefense[0]->SetTeam(iTeam);

  if(pPoint == aFlag[2])
  {
   aSelfDefense[1]->SetTeam(iTeam);
   aSelfDefense[2]->SetTeam(iTeam);
  }

  if(pPoint == aFlag[4])
   aSelfDefense[3]->SetTeam(iTeam);
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
   aFlag[0] = CreateObject(OFPL,380,430,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(330,520);
   aFlag[0] -> AddSpawnPoint(520,260);
   aFlag[0] -> AddSpawnPoint(780,430);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,715,800,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(485,630);
   aFlag[1] -> AddSpawnPoint(630,630);
   aFlag[1] -> AddSpawnPoint(860,630);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,1365,413,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(1330,300);
   aFlag[2] -> AddSpawnPoint(1400,510);
   aFlag[2] -> AddSpawnPoint(1315,790);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,2015,800,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(1870,630);
   aFlag[3] -> AddSpawnPoint(2100,630);
   aFlag[3] -> AddSpawnPoint(2245,630);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,2350,430,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(2400,520);
   aFlag[4] -> AddSpawnPoint(1950,430);
   aFlag[4] -> AddSpawnPoint(2210,260);
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
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[3]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[3]->TurnOn();

   //Hinweisschilder
   var sign = CreateObject(SGNP, 990, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 1740, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
  }

  //Base Assault-Spielziel
  if(FindObject(GBAS))
  {
   //Zielobjekte
   AddAssaultTarget(CMSN, 710, 800, 400, 1, "$Flag2$", 0, [[640, 640], [420, 630]]);
   AddAssaultTarget(CCP2, 490, 440, 300, 1, "$Flag1$", 1, [[230, 440], [330, 530]]);

   AddAssaultTarget(CMSN, 2010, 800, 400, 2, "$Flag4$", 0, [[2090, 640], [2310, 630]]);
   AddAssaultTarget(CCP2, 2240, 440, 300, 2, "$Flag5$", 1, [[2400, 530], [2500, 440]]);

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[3]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[3]->TurnOn();

   //Hinweisschilder
   var sign = CreateObject(SGNP, 990, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 1740, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SNPT, 400, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();
   sign = CreateObject(SNPT, 2330, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 1365,413, -1);
   flag->~Set("$Flag3$");

   //Teamgrenzen
   CreateObject(BRDR, 900, 0, -1)->Set(0,1,0,1,1);
   CreateObject(BRDR, 1830, 0, -1)->Set(1,1,0,1,2);

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[3]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[3]->TurnOn();

   //Hinweisschilder
   var sign = CreateObject(SGNP, 990, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 1740, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SNPT, 400, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();
   sign = CreateObject(SNPT, 2330, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Versorgungskiste (Kugeln)
    var crate = CreateObject(AMCT, 1330, 640, -1);
    crate->Set(ABOX);

    //Raketen
    PlaceSpawnpoint(MBOX, 1330, 305);
   }
  }

  //MR-Spielziel
  if(FindObject(GMNR))
  {
   //Geldsäcke
   AddMoneySpawn(1075, 435, [20]);
   AddMoneySpawn(1365, 305, [20]);
   AddMoneySpawn(1365, 805, [20]);
   AddMoneySpawn(1655, 435, [20]);

   //Teamgrenzen
   CreateObject(BRDR, 900, 0, -1)->Set(0,1,0,1,1);
   CreateObject(BRDR, 1830, 0, -1)->Set(1,1,0,1,2);

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[3]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[3]->TurnOn();

   //Hinweisschilder
   var sign = CreateObject(SGNP, 990, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 1740, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SNPT, 400, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();
   sign = CreateObject(SNPT, 2330, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,765,640,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,1965,640,GetTeamColor(2));}

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[3]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[3]->TurnOn();

   //Hinweisschilder
   var sign = CreateObject(SGNP, 990, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 1740, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SNPT, 400, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();
   sign = CreateObject(SNPT, 2330, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();
  }

  //DM/LMS-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS))
  {
   //Objekte entfernen
   RemoveObject(aSelfDefense[0]);
   RemoveObject(aSelfDefense[3]);

   //Hinweisschilder
   var sign = CreateObject(SGNP, 990, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 1740, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 1365; iY = 570;
   return(1);
  }

  //HTF/MR/CTF/LMS/DM-Spielziel
  if(FindObject(GHTF) || FindObject(GMNR) || FindObject(GCTF) || FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    return [[420, 620], [490, 430], [620, 790]];
   }
   if(iTeam == 2)
   {
    return [[2110, 790], [2240, 430], [2310, 620]];
   }
   return(1);
  }
}