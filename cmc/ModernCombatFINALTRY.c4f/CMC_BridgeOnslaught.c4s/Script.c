/*-- Bridge Onslaught --*/

#strict 2
#include CSTD

static aFlag,aSelfDefense;

func RecommendedGoals()	{return [GBAS, GHTF];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(0,15,15);
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
  return 1;
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Leitern
  CreateObject(LADR, 600, 726, -1)->Set(36);
  CreateObject(LADR, 695, 421, -1)->Set(18);
  CreateObject(LADR, 785, 775, -1)->Set(16);
  CreateObject(LADR, 955, 623, -1)->Set(22);
  CreateObject(LADR, 1010, 778, -1)->Set(15);

  CreateObject(LADR, 1370, 768, -1)->Set(15);
  CreateObject(LADR, 1505, 621, -1)->Set(38);
  CreateObject(LADR, 1565, 791, -1)->Set(18);
  CreateObject(LADR, 1625, 621, -1)->Set(38);
  CreateObject(LADR, 1760, 768, -1)->Set(15);

  CreateObject(LADR, 2120, 778, -1)->Set(15);
  CreateObject(LADR, 2175, 623, -1)->Set(22);
  CreateObject(LADR, 2345, 775, -1)->Set(16);
  CreateObject(LADR, 2435, 421, -1)->Set(18);
  CreateObject(LADR, 2530, 726, -1)->Set(36);

  //Rampen
  DrawMaterialQuad("Wall-Concrete3",360,541,360,531,340,541,360,541,true);

  DrawMaterialQuad("Wall-Concrete3",530,440,560,430,560,440,545,440,true);
  DrawMaterialQuad("Wall-Concrete2",571,740,571,730,541,740,571,740,true);
  DrawMaterialQuad("Wall-Concrete3",671,440,641,430,641,440,656,440,true);

  DrawMaterialQuad("Wall-Concrete3",531,641,561,631,561,641,546,641,true);
  DrawMaterialQuad("Wall-Concrete3",670,641,640,631,640,641,655,641,true);

  DrawMaterialQuad("Wall-Concrete3",1001,650,971,640,971,650,986,650,true);
  DrawMaterialQuad("Wall-Concrete3",1020,650,1050,640,1050,650,1035,650,true);

  DrawMaterialQuad("Wall-Concrete3",2111,650,2081,640,2081,650,2096,650,true);
  DrawMaterialQuad("Wall-Concrete3",2130,650,2160,640,2160,650,2145,650,true);

  DrawMaterialQuad("Wall-Concrete3",2461,641,2491,631,2491,641,2476,641,true);
  DrawMaterialQuad("Wall-Concrete3",2600,641,2570,631,2570,641,2585,641,true);

  DrawMaterialQuad("Wall-Concrete3",2460,440,2490,430,2490,440,2475,440,true);
  DrawMaterialQuad("Wall-Concrete2",2561,740,2561,730,2591,740,2561,740,true);
  DrawMaterialQuad("Wall-Concrete3",2601,440,2571,430,2571,440,2586,440,true);

  DrawMaterialQuad("Wall-Concrete3",2770,541,2770,531,2790,541,2770,541,true);

  //Bodenluken
  CreateObject(HA4K, 600, 433, -1);
  CreateObject(HA4K, 600, 634, -1);
  CreateObject(HA4K, 1010, 653, -1);
  CreateObject(HA4K, 1370, 643, -1);

  CreateObject(HA4K, 1760, 643, -1);
  CreateObject(HA4K, 2120, 653, -1);
  CreateObject(HA4K, 2530, 433, -1);
  CreateObject(HA4K, 2530, 634, -1);

  //Große Bodenluken
  CreateObject(H24K, 785, 648, -1);
  CreateObject(H24K, 955, 448, -1);

  CreateObject(H24K, 1505, 318, -1);
  CreateObject(H24K, 1505, 448, -1);
  CreateObject(H24K, 1505, 528, -1);

  CreateObject(H24K, 1565, 648, -1);

  CreateObject(H24K, 1625, 318, -1);
  CreateObject(H24K, 1625, 448, -1);
  CreateObject(H24K, 1625, 528, -1);

  CreateObject(H24K, 2175, 448, -1);
  CreateObject(H24K, 2345, 648, -1);

  //Glasscheiben
  CreateObject(PANE, 1462, 510, -1);
  CreateObject(PANE, 1462, 520, -1);
  CreateObject(PANE, 1668, 510, -1);
  CreateObject(PANE, 1668, 520, -1);

  //Explosivtanks
  CreateObject(XTNK, 430, 440, -1)->AutoRespawn();
  CreateObject(XTNK, 1400, 800, -1)->AutoRespawn();
  CreateObject(XTNK, 1730, 800, -1)->AutoRespawn();
  CreateObject(XTNK, 2700, 440, -1)->AutoRespawn();

  //Container
  var container = CreateObject(CON1, 225, 442, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  CreateObject(CON1, 275, 440, -1)->SetGraphics("5");
  container = CreateObject(CON1, 320, 640, -1);
  container->SetPerspective();
  container->SetGraphics("5");
  container = CreateObject(CON1, 450, 640, -1);
  container->SetPerspective(3);
  container->SetGraphics("5");
  container = CreateObject(CON1, 465, 614, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  container = CreateObject(CON1, 730, 440, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  container = CreateObject(CON1, 1275, 440, -1);
  container->SetPerspective(2);
  CreateObject(CON1, 730, 640, -1)->SetGraphics("5");
  container = CreateObject(CON1, 1565, 440, -1);
  container->SetPerspective(3);
  container->SetGraphics("5");
  container = CreateObject(CON1, 1855, 440, -1);
  container->SetPerspective(2);
  container = CreateObject(CON1, 2400, 440, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  CreateObject(CON1, 2400, 640, -1)->SetGraphics("5");
  container = CreateObject(CON1, 2675, 642, -1);
  container->SetPerspective();
  container->SetGraphics("5");
  container = CreateObject(CON1, 2700, 614, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  CreateObject(CON1, 2730, 642, -1)->SetGraphics("5");
  container = CreateObject(CON1, 2880, 440, -1);
  container->SetPerspective(3);
  container->SetGraphics("5");

  //Autos
  var car = CreateObject(CCAR, 50, 641, -1);
  car->SetPhase(0);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  car = CreateObject(CCAR, 60, 441, -1);
  car->SetPhase(0);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  car = CreateObject(CCAR, 150, 641, -1);
  car->SetPhase(0);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  car = CreateObject(CCAR, 160, 441, -1);
  car->SetPhase(0);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  car = CreateObject(CCAR, 250, 641, -1);
  car->SetPhase(0);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  CreateObject(CCAR, 895, 640, -1);
  CreateObject(CCAR, 2235, 640, -1);
  car = CreateObject(CCAR, 2810, 641, -1);
  car->SetPhase(1);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  car = CreateObject(CCAR, 2880, 641, -1);
  car->SetPhase(1);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  car = CreateObject(CCAR, 2970, 441, -1);
  car->SetPhase(1);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  car = CreateObject(CCAR, 2980, 641, -1);
  car->SetPhase(1);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  car = CreateObject(CCAR, 3070, 441, -1);
  car->SetPhase(1);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  car = CreateObject(CCAR, 3080, 641, -1);
  car->SetPhase(1);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);

  //Kisten
  CreateObject(WCR2, 810, 640, -1)->AutoRespawn();
  CreateObject(WCR2, 1115, 640, -1);
  CreateObject(WCR2, 1470, 440, -1)->AutoRespawn();
  CreateObject(WCR2, 1600, 640, -1)->AutoRespawn();
  CreateObject(WCR2, 1615, 640, -1);
  CreateObject(WCR2, 1605, 622, -1);
  CreateObject(WCR2, 1660, 440, -1)->AutoRespawn();
  CreateObject(WCR2, 2015, 640, -1);
  CreateObject(WCR2, 2320, 640, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 275, 413, -1);
  CreateObject(MWCR, 495, 640, -1);
  CreateObject(MWCR, 770, 800, -1)->AutoRespawn();
  CreateObject(MWCR, 850, 270, -1)->AutoRespawn();
  CreateObject(MWCR, 1265, 640, -1)->AutoRespawn();
  CreateObject(MWCR, 1595, 310, -1)->AutoRespawn();
  CreateObject(MWCR, 1865, 640, -1)->AutoRespawn();
  CreateObject(MWCR, 2282, 270, -1)->AutoRespawn();
  CreateObject(MWCR, 2360, 800, -1)->AutoRespawn();
  CreateObject(MWCR, 2835, 440, -1);

  //Verbandskisten
  CreateObject(BECR, 790, 800, -1)->AutoRespawn();
  CreateObject(BECR, 1480, 710, -1)->AutoRespawn();
  CreateObject(BECR, 1650, 710, -1)->AutoRespawn();
  CreateObject(BECR, 2340, 800, -1)->AutoRespawn();

  //Gerüste
  CreateObject(SFFG, 965, 440, -1)->Set(5);
  CreateObject(SFFG, 1025, 440, -1)->Set(4);
  CreateObject(SFFG, 2105, 440, -1)->Set(4);
  CreateObject(SFFG, 2165, 440, -1)->Set(5);

  //Sandsackbarrieren
  CreateObject(SBBA, 990, 800, -1)->Right();
  CreateObject(SBBA, 1050, 390, -1)->Right();
  CreateObject(SBBA, 1160, 640, -1);
  CreateObject(SBBA, 1360, 800, -1);
  CreateObject(SBBA, 1460, 640, -1);
  CreateObject(SBBA, 1670, 640, -1)->Right();
  CreateObject(SBBA, 1770, 800, -1)->Right();
  CreateObject(SBBA, 1970, 640, -1)->Right();
  CreateObject(SBBA, 2076, 390, -1);
  CreateObject(SBBA, 2140, 800, -1);

  //Stahlbrücken
  CreateObject(_HBR, 795, 452, -1)->SwitchMode();
  CreateObject(_HBR, 885, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1025, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1125, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1225, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1325, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1425, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1115, 652, -1)->SwitchMode();
  CreateObject(_HBR, 1315, 652, -1)->SwitchMode();
  CreateObject(_HBR, 1425, 652, -1)->SwitchMode();

  CreateObject(_HBR, 1705, 652, -1)->SwitchMode();
  CreateObject(_HBR, 1815, 652, -1)->SwitchMode();
  CreateObject(_HBR, 2015, 652, -1)->SwitchMode();
  CreateObject(_HBR, 1705, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1805, 452, -1)->SwitchMode();
  CreateObject(_HBR, 1905, 452, -1)->SwitchMode();
  CreateObject(_HBR, 2005, 452, -1)->SwitchMode();
  CreateObject(_HBR, 2105, 452, -1)->SwitchMode();
  CreateObject(_HBR, 2245, 452, -1)->SwitchMode();
  CreateObject(_HBR, 2335, 452, -1)->SwitchMode();

  //Gasflaschen
  CreateObject(GSBL, 740, 613, -1)->AutoRespawn();
  CreateObject(GSBL, 830, 800, -1)->AutoRespawn();
  CreateObject(GSBL, 1195, 439, -1);
  CreateObject(GSBL, 1245, 640, -1)->AutoRespawn();
  CreateObject(GSBL, 1885, 640, -1)->AutoRespawn();
  CreateObject(GSBL, 1935, 439, -1);
  CreateObject(GSBL, 2300, 800, -1)->AutoRespawn();
  CreateObject(GSBL, 2390, 613, -1)->AutoRespawn();

  //Explosive Kiste
  CreateObject(XWCR, 1175, 440, -1)->AutoRespawn();
  CreateObject(XWCR, 1955, 440, -1)->AutoRespawn();

  //Rohre
  CreateObject(PIPL, 0, 515, -1)->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 360, 515, -1)->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 2770, 515, -1)->Solid()->Left()->Solid()->Left()->Solid();
  CreateObject(PIPL, 3130, 515, -1)->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid();

  //Raucheffekte an Rohrenden
  for(var obj in FindObjects(Find_ID(PIPL), Find_InRect(280, 470, 100, 70)))
   obj->AddSmokeEffect4K(40,-2,-5,true,this);
  for(var obj in FindObjects(Find_ID(PIPL), Find_InRect(2750, 470, 100, 70)))
   obj->AddSmokeEffect4K(40,-2,-5,true,this);

  //Giftfässer
  CreateObject(TBRL, 565, 430, -1)->AutoRespawn();
  CreateObject(TBRL, 2565, 430, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 1190, 800, -1)->AutoRespawn();
  CreateObject(XBRL, 1425, 800, -1)->AutoRespawn();
  CreateObject(XBRL, 1550, 310, -1)->AutoRespawn();
  CreateObject(XBRL, 1570, 310, -1)->AutoRespawn();
  CreateObject(XBRL, 1490, 640, -1)->AutoRespawn();
  CreateObject(XBRL, 1705, 800, -1)->AutoRespawn();
  CreateObject(XBRL, 1945, 800, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 1560, 310, -1)->AutoRespawn();
  CreateObject(HBRL, 1500, 640, -1)->AutoRespawn();

  //Paletten
  CreateObject(PLLT, 760, 270, -1);
  CreateObject(PLLT, 2370, 270, -1);

  //Tore
  CreateObject(HNG2, 375, 440, -1);
  CreateObject(HNG2, 565, 630, -1);
  CreateObject(HNG2, 2565, 630, -1);
  CreateObject(HNG2, 2755, 440, -1);

  //Steine
  CreateObject(STNE, 1490, 800, -1)->Set(0);
  CreateObject(STNE, 1650, 800, -1)->Set(1);

  //Hydrauliktüren
  CreateObject(SLDR, 290, 540, -1)->Lock();
  var door = CreateObject(SLDR, 470, 530, -1);
  door->Lock();
  door->SetMaxDamage(-1);
  door->SetSwitchLock(DIR_Right);
  door = CreateObject(SLDR, 2660, 530, -1);
  door->Lock();
  door->SetMaxDamage(-1);
  door->SetSwitchLock(DIR_Left);
  CreateObject(SLDR, 2840, 540, -1)->Lock();

  //Verbundene Räume
  var doorw = CreateObject(ROOM, 400, 530, -1);
  CreateObject(ROOM, 2730, 530, -1)->Connect(doorw);

  //Selbstschussanlagen und Konsolen
  aSelfDefense[0] = CreateObject(SEGU, 790, 329, -1);
  CreateObject(CONS, 510, 525, -1)->Set(aSelfDefense[0]);

  aSelfDefense[1] = CreateObject(SEGU, 1452, 556, -1);
  aSelfDefense[1]->Set(0,0,0,90);
  CreateObject(CONS, 1450, 795, -1)->Set(aSelfDefense[1]);

  aSelfDefense[2] = CreateObject(SEGU, 1678, 556, -1);
  aSelfDefense[2]->Set(0,0,0,-90);
  CreateObject(CONS, 1680, 795, -1)->Set(aSelfDefense[2]);

  aSelfDefense[3] = CreateObject(SEGU, 2340, 329, -1);
  CreateObject(CONS, 2620, 525, -1)->Set(aSelfDefense[3]);

  //Hinweisschilder
  var sign = CreateObject(SNPT, 390, 655, -1);
  sign->SetMode(1);
  sign->SetAction("Sign6");
  sign = CreateObject(SNPT, 2730, 655, -1);
  sign->SetMode(1);
  sign->SetAction("Sign6");

  //Grenzen
  CreateObject(BRDR, 0, 850, -1)->Set(3,0,1);
  CreateObject(BRDR, 290, 0, -1)->Set(0);
  CreateObject(BRDR, 2840, 0, -1)->Set(1);

  //Sounds

  //Wind
  CreateObject(SE4K, 780, 170, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 1565, 170, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 2350, 170, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 290, 410, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 740, 560, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1160, 670, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1565, 550, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1970, 670, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2390, 560, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2835, 410, -1)->Set("Interior*.ogg",665,105);

  //Rush Hour
  CreateObject(SE4K, 1275, 880, -1)->Set("Traffic*.ogg",245,70);
  CreateObject(SE4K, 1855, 880, -1)->Set("Traffic*.ogg",245,70);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 730, 730, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 2400, 730, -1);
  crate->Set(ABOX);

  //Versorgungskiste (Gewehrgranaten)
  crate = CreateObject(AMCT, 1665, 310, -1);
  crate->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MBOX, 955, 715);
  PlaceSpawnpoint(MBOX, 1465, 305);
  PlaceSpawnpoint(MBOX, 2175, 715);

  //Automaten
  var store = CreateObject(WPVM,1565, 520,-1);
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
  CreateObject(ATBY,815,270,-1)->SetRotation(20);
  CreateObject(ATBY,2315,270,-1)->SetRotation(-20);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Räume
  CreateObject(ROM2, 260, 540, -1);
  CreateObject(ROM2, 2870, 540, -1);

  //Leitkegel
  CreateObject(TCCN, 275, 540, -1);
  CreateObject(TCCN, 366, 530, -1)->Light();
  CreateObject(TCCN, 375, 530, -1);
  CreateObject(TCCN, 585, 430, -1);
  CreateObject(TCCN, 615, 430, -1);
  CreateObject(TCCN, 1050, 800, -1);
  CreateObject(TCCN, 1070, 800, -1);
  CreateObject(TCCN, 1090, 800, -1)->Light();
  CreateObject(TCCN, 2040, 800, -1)->Light();
  CreateObject(TCCN, 2060, 800, -1);
  CreateObject(TCCN, 2080, 800, -1);
  CreateObject(TCCN, 2515, 430, -1);
  CreateObject(TCCN, 2545, 430, -1);
  CreateObject(TCCN, 2755, 530, -1);
  CreateObject(TCCN, 2764, 530, -1)->Light();
  CreateObject(TCCN, 2855, 540, -1);

  //Geländer
  CreateObject(RAI1, 241, 540, -1)->SetRail([1,1]);
  CreateObject(RAI1, 705, 270, -1)->SetRail([1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 925, 440, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1427, 310, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1430, 800, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1474, 710, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1614, 710, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1660, 800, -1)->SetRail([1,1]);
  CreateObject(RAI1, 2145, 440, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 2275, 270, -1)->SetRail([1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 2846, 540, -1)->SetRail([1,1]);

  //Radare
  CreateObject(RADR, 1220, 310, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(RADR, 1900, 310, -1)->SetClrModulation(RGB(125,125,125));

  //Spinde
  CreateObject(LCKR, 420, 530, -1);
  CreateObject(LCKR, 440, 530, -1);
  CreateObject(LCKR, 2690, 530, -1);
  CreateObject(LCKR, 2710, 530, -1);

  //Automaten
  CreateObject(VGMN, 815, 800, -1)->Set(3);
  CreateObject(VGMN, 2315, 800, -1);

  //Glastische
  CreateObject(GTBL, 860, 800, -1);
  CreateObject(GTBL, 2265, 800, -1);

  //Wandlampen
  CreateObject(BLGH, 835, 400, -1);
  CreateObject(BLGH, 835, 600, -1);
  CreateObject(BLGH, 1275, 400, -1);

  CreateObject(BLGH, 1855, 400, -1);
  CreateObject(BLGH, 2295, 400, -1);
  CreateObject(BLGH, 2295, 600, -1);

  //Notausgangslichter
  CreateObject(ETLT, 950, 770, -1);
  CreateObject(ETLT, 2180, 770, -1);

  //Verkehrsampeln
  CreateObject(TLGH, 460, 440, -1);
  CreateObject(TLGH, 625, 630, -1);
  CreateObject(TLGH, 2505, 630, -1);
  CreateObject(TLGH, 2670, 440, -1);

  //Bildschirme
  CreateObject(SCR3, 560, 520, -1);
  CreateObject(SCA2, 840, 765, -1)->SetAction("News");
  CreateObject(SCA2, 900, 765, -1)->SetAction("Clonk");
  CreateObject(SCA2, 2230, 765, -1)->SetAction("Wompf");
  CreateObject(SCA2, 2290, 765, -1)->SetAction("News");
  CreateObject(SCR3, 2570, 520, -1);

  //Monitore
  CreateObject(MNI2, 850, 788, -1);
  CreateObject(MNI2, 865, 788, -1);
  CreateObject(MNI2, 2255, 788, -1);
  CreateObject(MNI2, 2270, 788, -1);

  //Flaschen
  CreateObject(BOTL, 750, 612, -1);
  CreateObject(BOTL, 1560, 412, -1);
  CreateObject(BOTL, 2380, 612, -1);

  //Dekoschleusen
  CreateObject(GAT1, 1275, 520, -1);
  CreateObject(GAT1, 1855, 520, -1);

  //Scheinwerfer
  CreateObject(FLGH, 390, 440, -1)->SetRotation(30);
  CreateObject(FLGH, 2735, 440, -1)->SetRotation(-30);

  //Zäune
  CreateObject(FENC, 410, 440, -1)->Set(2);
  CreateObject(FENC, 470, 440, -1);
  CreateObject(FENC, 530, 440, -1);
  CreateObject(FENC, 670, 640, -1)->Set(2);
  CreateObject(FENC, 730, 640, -1);
  CreateObject(FENC, 790, 640, -1);
  CreateObject(FENC, 850, 640, -1);
  CreateObject(FENC, 910, 640, -1);
  CreateObject(FENC, 2220, 640, -1);
  CreateObject(FENC, 2280, 640, -1);
  CreateObject(FENC, 2340, 640, -1);
  CreateObject(FENC, 2400, 640, -1);
  CreateObject(FENC, 2460, 640, -1)->Set(2);
  CreateObject(FENC, 2600, 440, -1);
  CreateObject(FENC, 2660, 440, -1);
  CreateObject(FENC, 2720, 440, -1)->Set(2);

  //Regale
  CreateObject(FRAM, 955, 800, -1);
  CreateObject(FRAM, 975, 800, -1);
  CreateObject(FRAM, 1530, 520, -1);
  CreateObject(FRAM, 1600, 520, -1);
  CreateObject(FRAM, 2150, 800, -1);
  CreateObject(FRAM, 2170, 800, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 40, 430, -1);
  CreateObject(ENGT, 40, 630, -1);
  CreateObject(ENGT, 140, 430, -1);
  CreateObject(ENGT, 140, 630, -1);
  CreateObject(ENGT, 240, 430, -1);
  CreateObject(ENGT, 240, 630, -1);
  CreateObject(ENGT, 340, 430, -1);
  CreateObject(ENGT, 340, 630, -1);
  CreateObject(ENGT, 440, 630, -1);
  CreateObject(ENGT, 540, 630, -1);
  CreateObject(ENGT, 2590, 630, -1);
  CreateObject(ENGT, 2690, 630, -1);
  CreateObject(ENGT, 2790, 430, -1);
  CreateObject(ENGT, 2790, 630, -1);
  CreateObject(ENGT, 2890, 430, -1);
  CreateObject(ENGT, 2890, 630, -1);
  CreateObject(ENGT, 2990, 430, -1);
  CreateObject(ENGT, 2990, 630, -1);
  CreateObject(ENGT, 3090, 430, -1);
  CreateObject(ENGT, 3090, 630, -1);

  //Feuerlöscher
  CreateObject(FIE2, 315, 435, -1);
  CreateObject(FIE2, 515, 635, -1);
  CreateObject(FIEX, 680, 725, -1);
  CreateObject(FIEX, 2450, 725, -1);
  CreateObject(FIE2, 2615, 635, -1);
  CreateObject(FIE2, 2815, 435, -1);

  //Büsche
  CreateObject(BSH2, 705, 515, -1);
  CreateObject(BSH2, 1540, 820, -1)->SetR(90);
  CreateObject(BSH2, 1590, 820, -1);
  CreateObject(BSH2, 1655, 570, -1);
  CreateObject(BSH2, 1980, 820, -1);
  CreateObject(BSH2, 2390, 515, -1);

  //Ventillatoren
  CreateObject(VEN3, 630, 725, -1)->SetCon(20);
  CreateObject(VEN3, 650, 725, -1)->SetCon(20);
  CreateObject(VEN3, 690, 595, -1)->SetCon(20);
  var vent = CreateObject(VEN3, 710, 595, -1);
  vent->SetCon(20);
  vent->SetPhase(4);
  vent = CreateObject(VEN3, 730, 595, -1);
  vent->SetCon(20);
  vent->SetPhase(7);
  CreateObject(VENT, 1175, 595, -1)->SetCon(20);
  CreateObject(VENT, 1275, 595, -1)->SetCon(20);
  CreateObject(VENT, 1375, 595, -1)->SetCon(20);
  CreateObject(VENT, 1755, 595, -1)->SetCon(20);
  CreateObject(VENT, 1855, 595, -1)->SetCon(20);
  CreateObject(VENT, 1955, 595, -1)->SetCon(20);
  vent = CreateObject(VEN3, 2400, 595, -1);
  vent->SetCon(20);
  vent->SetPhase(7);
  vent = CreateObject(VEN3, 2420, 595, -1);
  vent->SetCon(20);
  vent->SetPhase(4);
  CreateObject(VEN3, 2440, 595, -1)->SetCon(20);
  CreateObject(VEN3, 2480, 725, -1)->SetCon(20);
  CreateObject(VEN3, 2500, 725, -1)->SetCon(20);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  var back = CreateObject(BD10,500,900,-1);
  back->SetClrModulation(RGB(130,130,130));
  back->Local()=65;
  back->Local(1)=80;
  back = CreateObject(BD10,1890,900,-1);
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
   aFlag[0] = CreateObject(OFPL,580,430,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(530,520);
   aFlag[0] -> AddSpawnPoint(720,260);
   aFlag[0] -> AddSpawnPoint(980,430);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,915,800,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(685,630);
   aFlag[1] -> AddSpawnPoint(830,630);
   aFlag[1] -> AddSpawnPoint(1060,630);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,1565,413,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(1510,700);
   aFlag[2] -> AddSpawnPoint(1515,790);
   aFlag[2] -> AddSpawnPoint(1530,630);
   aFlag[2] -> AddSpawnPoint(1625,790);
   aFlag[2] -> AddSpawnPoint(1620,700);

   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,2215,800,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(2070,630);
   aFlag[3] -> AddSpawnPoint(2300,630);
   aFlag[3] -> AddSpawnPoint(2445,630);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,2550,430,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(2600,520);
   aFlag[4] -> AddSpawnPoint(2150,430);
   aFlag[4] -> AddSpawnPoint(2410,260);
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
   var sign = CreateObject(SGNP, 1190, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 1940, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
  }

  //Base Assault-Spielziel
  if(FindObject(GBAS))
  {
   //Zielobjekte
   AddAssaultTarget(CCP2, 840, 440, 350, 1, "$Target1$", 0, [[430, 440], [530, 530]]);
   AddAssaultTarget(CMSN, 1030, 800, 350, 1, "$Target2$", 1, [[580, 730], [620, 630]]);
   AddAssaultTarget(PMP2, 1060, 605, 350, 1, "$Target3$", 2, [[530, 530], [620, 630]]);
   AddAssaultTarget(PMP2, 2070, 605, 350, 2, "$Target3$", 3, [[2600, 530], [2510, 630]]);
   AddAssaultTarget(CMSN, 2100, 800, 350, 2, "$Target2$", 4, [[2550, 730], [2510, 630]]);
   AddAssaultTarget(CCP2, 2290, 440, 350, 2, "$Target1$", 5, [[2600, 530], [2700, 440]]);

   //Sprengsatz-Spawn
   SetupBombSpawnpoint([[1565, 310],[1565, 410],[1565,520],[1565,810]]);

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[3]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[3]->TurnOn();

   //Hinweisschilder
   var sign = CreateObject(SGNP, 1190, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 1940, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SNPT, 600, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();
   sign = CreateObject(SNPT, 2530, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();

   //Objekte entfernen
   RemoveObject(FindObject2(Find_ID(CONS),Find_InRect(500, 510, 30, 30)));
   RemoveObject(FindObject2(Find_ID(CONS),Find_InRect(2610, 510, 30, 30)));
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 1565,413, -1);
   flag->~Set("$Flag3$");

   //Teamgrenzen
   CreateObject(BRDR, 1100, 0, -1)->Set(0,1,0,1,1);
   CreateObject(BRDR, 2030, 0, -1)->Set(1,1,0,1,2);

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[3]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[3]->TurnOn();

   //Hinweisschilder
   var sign = CreateObject(SGNP, 1190, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 1940, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SNPT, 600, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();
   sign = CreateObject(SNPT, 2530, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Versorgungskiste (Kugeln)
    var crate = CreateObject(AMCT, 1530, 640, -1);
    crate->Set(ABOX);
   }
  }

  //MR-Spielziel
  if(FindObject(GMNR))
  {
   //Geldsäcke
   AddMoneySpawn(1275, 405, [15]);
   AddMoneySpawn(1370, 635, [15]);
   AddMoneySpawn(1565, 305, [20]);
   AddMoneySpawn(1565, 805, [20]);
   AddMoneySpawn(1760, 635, [15]);
   AddMoneySpawn(1855, 405, [15]);

   //Teamgrenzen
   CreateObject(BRDR, 1100, 0, -1)->Set(0,1,0,1,1);
   CreateObject(BRDR, 2030, 0, -1)->Set(1,1,0,1,2);

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[3]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[3]->TurnOn();

   //Hinweisschilder
   var sign = CreateObject(SGNP, 1190, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 1940, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SNPT, 600, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();
   sign = CreateObject(SNPT, 2530, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,965,640,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,2165,640,GetTeamColor(2));}

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[3]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[3]->TurnOn();

   //Hinweisschilder
   var sign = CreateObject(SGNP, 1190, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 1940, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SNPT, 600, 430, -1);
   sign->SetAction("Sign2");
   sign->Light();
   sign = CreateObject(SNPT, 2530, 430, -1);
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
   var sign = CreateObject(SGNP, 1190, 640, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 1940, 640, -1);
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
   iX = 1565; iY = 570;
   return 1;
  }

  //HTF/MR/CTF/LMS/DM-Spielziel
  if(FindObject(GHTF) || FindObject(GMNR) || FindObject(GCTF) || FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    return [[620, 620], [690, 430], [820, 790]];
   }
   if(iTeam == 2)
   {
    return [[2310, 790], [2440, 430], [2510, 620]];
   }
   return 1;
  }
}