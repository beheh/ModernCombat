/*-- Endless Outpost --*/

#strict
#include CSTD

static aFlag,aDoor,aSelfDefense,aLamp;


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetPlayList("CMC_WaitingforSpawn.ogg");
  Music("CMC_WaitingforSpawn.ogg",1);
  //Bildschirmfärbung
  SetGamma(RGB(0,0,0), RGB(80,80,80), RGB(200,200,200));
  //Flaggen
  aFlag = [];
  //Türen
  aDoor = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Lampen
  aLamp = [];
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

  //Rampen
  DrawMaterialQuad("Wall-Concrete1",580,450,610,440,610,450,595,450,true);
  DrawMaterialQuad("Wall-Concrete1",610,440,640,430,640,440,625,440,true);
  DrawMaterialQuad("Wall-Concrete3",670,420,700,410,700,420,685,420,true);
  DrawMaterialQuad("Wall-Concrete1",640,430,670,420,670,430,655,430,true);

  DrawMaterialQuad("Wall-Concrete3",861,420,831,410,831,420,846,420,true);
  DrawMaterialQuad("Wall-Concrete1",891,430,861,420,861,430,876,430,true);

  DrawMaterialQuad("Wall-Concrete1",1050,370,1080,360,1080,370,1065,370,true);
  DrawMaterialQuad("Wall-Concrete1",1151,371,1151,361,1181,371,1151,371,true);

  DrawMaterialQuad("Wall-Concrete1",1010,571,1040,561,1040,571,1025,571,true);
  DrawMaterialQuad("Wall-Concrete1",1151,570,1121,560,1121,570,1136,570,true);

  DrawMaterialQuad("Wall-Plate",1220,560,1250,550,1250,560,1235,560,true);

  DrawMaterialQuad("Wall-Metal2",1981,510,1951,500,1951,510,1966,510,true);
  DrawMaterialQuad("Wall-Metal2",1951,500,1921,490,1921,500,1936,500,true);
  DrawMaterialQuad("Wall-Metal2",2261,520,2231,510,2231,520,2246,520,true);

  DrawMaterialQuad("Wall-Concrete3",2481,580,2481,570,2451,580,2481,580,true);
  DrawMaterialQuad("Wall-Concrete3",2511,570,2511,560,2481,570,2511,570,true);

  //Leitern
  CreateObject(LADR, 70, 622, -1)->Set(33);
  CreateObject(LADR, 720, 530, -1)->Set(14);
  CreateObject(LADR, 850, 400, -1)->Set(29);
  CreateObject(LADR, 985, 570, -1)->Set(17);
  CreateObject(LADR, 1040, 420, -1)->Set(19);
  CreateObject(LADR, 1205, 568, -1)->Set(45);
  CreateObject(LADR, 1385, 520, -1)->Set(15);
  CreateObject(LADR, 1481, 280, -1)->Set(9);
  CreateObject(LADR, 1670, 273, -1)->Set(7);
  CreateObject(LADR, 1870, 578, -1)->Set(10);
  CreateObject(LADR, 1985, 320, -1)->Set(14);
  CreateObject(LADR, 2080, 480, -1)->Set(33);
  CreateObject(LADR, 2120, 480, -1)->Set(20);
  CreateObject(LADR, 2280, 490, -1)->Set(21);
  CreateObject(LADR, 2285, 221, -1)->Set(13);
  CreateObject(LADR, 2310, 490, -1)->Set(34);
  CreateObject(LADR, 2490, 500, -1)->Set(11);
  CreateObject(LADR, 2540, 500, -1)->Set(36);
  CreateObject(LADR, 2560, 560, -1)->Set(7);
  CreateObject(LADR, 2600, 320, -1)->Set(13);
  CreateObject(LADR, 2620, 210, -1)->Set(12);

  //Bodenluken
  CreateObject(HA4K, 70, 353, -1);
  CreateObject(HA4K, 70, 453, -1);
  CreateObject(HA4K, 720, 413, -1);
  CreateObject(HA4K, 1480, 203, -1);
  CreateObject(HA4K, 2080, 213, -1);
  CreateObject(HA4K, 2270, 533, -1);
  CreateObject(HA4K, 2350, 533, -1);
  CreateObject(HA4K, 2540, 353, -1);
  CreateObject(HA4K, 2540, 413, -1);
  CreateObject(HA4K, 2560, 503, -1);
  CreateObject(HA4K, 2600, 213, -1);
  CreateObject(HA4K, 2620, 113, -1);

  //Große Bodenluken
  CreateObject(H24K, 985, 438, -1);
  CreateObject(H24K, 1205, 448, -1);
  CreateObject(H24K, 1985, 118, -1);
  CreateObject(H24K, 2285, 118, -1);
  CreateObject(H24K, 2475, 118, -1);

  //Gerüste
  CreateObject(SFFG, 1080, 560, -1)->Set(3);
  CreateObject(SFFG, 10, 300, -1)->Set(5);
  CreateObject(SFFG, 10, 350, -1)->Set(5);
  CreateObject(SFFG, 90, 300, -1)->Set(2);
  CreateObject(SFFG, 90, 350, -1)->Set(3);
  CreateObject(SFFG, 170, 250, -1)->Set(3);
  CreateObject(SFFG, 170, 300, -1);
  CreateObject(SFFG, 170, 350, -1);

  CreateObject(SFFG, 2170, 410, -1)->Set(5);
  CreateObject(SFFG, 2170, 460, -1)->Set(3);
  CreateObject(SFFG, 2170, 510, -1)->Set(4);

  CreateObject(SFFG, 2440, 450, -1);
  CreateObject(SFFG, 2440, 500, -1)->Set(4);

  CreateObject(SFFG, 2760, 110, -1)->Set(3);
  CreateObject(SFFG, 2840, 110, -1);

  //Schutztüren
  CreateObject(GDDR, 815, 410, -1)->Lock();
  CreateObject(GDDR, 875, 550, -1)->Lock();
  CreateObject(GDDR, 965, 550, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1065, 260, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1065, 430, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1165, 260, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1245, 260, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1445, 200, -1)->Lock();
  CreateObject(GDDR, 1695, 290, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1795, 210, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1795, 290, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1905, 400, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 2250, 110, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 2510, 110, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 2515, 210, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 2515, 410, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 2515, 500, -1)->SetColorDw(HSL(200, 150, 100, 127));

  //Hydrauliktüren
  CreateObject(SEDR,2070, 110,-1)->Lock();
  CreateObject(SEDR,2560, 410,-1)->Lock();

  //Stahlbrücken
  CreateObject(_HBR, 305, 462, -1);
  CreateObject(_HBR, 405, 462, -1);
  CreateObject(_HBR, 505, 462, -1);
  CreateObject(_HBR, 2355, 332, -1);
  CreateObject(_HBR, 2465, 222, -1);

  //Kisten
  CreateObject(WCR2, 10, 350, -1)->AutoRespawn();
  CreateObject(WCR2, 1100, 360, -1);
  CreateObject(WCR2, 1460, 70, -1);
  CreateObject(WCR2, 1505, 200, -1)->AutoRespawn();
  CreateObject(WCR2, 1540, 70, -1)->AutoRespawn();
  CreateObject(WCR2, 1815, 580, -1);
  CreateObject(WCR2, 2040, 580, -1);
  CreateObject(WCR2, 2000, 456, -1);
  CreateObject(WCR2, 2000, 474, -1);
  CreateObject(WCR2, 2000, 492, -1);
  CreateObject(WCR2, 2000, 510, -1);
  CreateObject(WCR2, 2020, 492, -1);
  CreateObject(WCR2, 2040, 510, -1);
  CreateObject(WCR2, 2060, 510, -1);
  CreateObject(WCR2, 2110, 110, -1);
  CreateObject(WCR2, 2145, 320, -1);
  CreateObject(WCR2, 2255, 320, -1);
  CreateObject(WCR2, 2405, 320, -1);
  CreateObject(WCR2, 2415, 500, -1);

  //Metallkisten
  CreateObject(MWCR, 1190, 570, -1);
  CreateObject(MWCR, 1210, 570, -1);
  CreateObject(MWCR, 1910, 320, -1)->AutoRespawn();
  CreateObject(MWCR, 2060, 580, -1);
  CreateObject(MWCR, 2410, 580, -1);
  CreateObject(MWCR, 2710, 110, -1);

  //Verbandskisten
  CreateObject(BECR, 355, 450, -1)->AutoRespawn();
  CreateObject(BECR, 1460, 52, -1);
  CreateObject(BECR, 1795, 580, -1);
  CreateObject(BECR, 2020, 510, -1);
  CreateObject(BECR, 2410, 450, -1);
  CreateObject(BECR, 2560, 110, -1);

  //Gasflaschen
  CreateObject(GSBL, 1020, 430, -1)->AutoRespawn();
  CreateObject(GSBL, 1640, 520, -1)->AutoRespawn();
  CreateObject(GSBL, 2205, 110, -1)->AutoRespawn();
  CreateObject(GSBL, 2600, 560, -1)->AutoRespawn();
  CreateObject(GSBL, 2790, 110, -1);

  //Explosive Kisten
  CreateObject(XWCR, 1545, 320, -1)->AutoRespawn();
  CreateObject(XWCR, 2150, 210, -1)->AutoRespawn();
  CreateObject(XWCR, 2670, 410, -1);

  //Benzinfässer
  CreateObject(PBRL, 480, 450, -1);
  CreateObject(PBRL, 2325, 530, -1);
  CreateObject(PBRL, 2585, 560, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 1260, 550, -1)->AutoRespawn();
  CreateObject(XBRL, 1890, 320, -1)->AutoRespawn();
  CreateObject(XBRL, 2335, 530, -1);
  CreateObject(XBRL, 2420, 320, -1);
  CreateObject(XBRL, 2430, 320, -1);

  //Phosphorfässer
  CreateObject(HBRL, 1720, 290, -1)->AutoRespawn();
  CreateObject(HBRL, 2165, 320, -1);
  CreateObject(HBRL, 2220, 110, -1)->AutoRespawn();

  //Benzintanks
  CreateObject(XTNK, 1610, 520, -1)->AutoRespawn();
  CreateObject(XTNK, 1740, 210, -1)->AutoRespawn();
  CreateObject(XTNK, 2800, 410, -1);

  //Palette
  CreateObject(PLLT, 890, 160, -1);

  //Container
  CreateObject(CON1, 220, 450, -1);
  CreateObject(CON1, 457, 450, -1)->SetPerspective(2);
  CreateObject(CON1, 1485, 550, -1);
  CreateObject(CON1, 1495, 520, -1)->SetPerspective(2);
  CreateObject(CON1, 1680, 520, -1);
  CreateObject(CON1, 2170, 110, -1);
  CreateObject(CON1, 2670, 110, -1);
  CreateObject(CON1, 2690, 83, -1)->SetPerspective();
  CreateObject(CON1, 2720, 410, -1)->SetPerspective();
  CreateObject(CON1, 2730, 382, -1);
  CreateObject(CON1, 2830, 410, -1);

  //Glasscheiben
  CreateObject(_WIN, 802, 240, -1);
  CreateObject(_WIN, 802, 300, -1);
  CreateObject(_WIN, 802, 360, -1);

  CreateObject(_WIN, 1082, 310, -1);
  CreateObject(_WIN, 1082, 335, -1);
  CreateObject(_WIN, 1082, 360, -1);

  CreateObject(_WIN, 1147, 310, -1);
  CreateObject(_WIN, 1147, 335, -1);
  CreateObject(_WIN, 1147, 360, -1);

  CreateObject(_WIN, 1442, 140, -1);

  //Steine
  CreateObject(STNE, 220, 690, -1);
  CreateObject(STNE, 735, 665, -1);

  //Hydrauliküren
  aDoor[00] = CreateObject(SEDR,1225, 560,-1);
   aDoor[00]->Lock();
  aDoor[01] = CreateObject(SEDR,1245, 430,-1);
   aDoor[01]->Lock();
  aDoor[02] = CreateObject(SEDR,1355, 260,-1);
   aDoor[02]->Lock();
  aDoor[03] = CreateObject(H24K, 1205, 208, -1);
   aDoor[03]->Lock();
  aDoor[04] = CreateObject(HA4K, 850, 163, -1);
   aDoor[04]->Lock();
  aDoor[05] = CreateObject(HNG2,1355,550,-1);
  aDoor[06] = CreateObject(HNG2,1905,490,-1);
  aDoor[07] = CreateObject(SEDR,1850,110,-1);
   aDoor[07]->Lock();
  aDoor[08] = CreateObject(HA4K, 1440, 323, -1);
   aDoor[08]->Lock();
  aDoor[09] = CreateObject(HA4K, 1650, 293, -1);
   aDoor[09]->Lock();
  aDoor[10] = CreateObject(HA4K, 1870, 493, -1);
   aDoor[10]->Lock();

  //Lampen
  aLamp[00]=CreateObject(BLGH,880,300,-1);
  aLamp[01]=CreateObject(BLGH,980,300,-1);
  aLamp[02]=CreateObject(CLGH,1045,475,-1);
  aLamp[03]=CreateObject(CLGH,1125,475,-1);
  aLamp[04]=CreateObject(LLGH,1183,380,-1);
  aLamp[05]=CreateObject(LLGH,1227,295,-1);
  aLamp[06]=CreateObject(STLH,1430,225,-1);
  aLamp[07]=CreateObject(STLH,1650,95,-1);
  aLamp[08]=CreateObject(BLGH,1300,305,-1);
  aLamp[09]=CreateObject(BLGH,1300,390,-1);
  aLamp[10]=CreateObject(BLGH,1865,270,-1);
  aLamp[11]=CreateObject(BLGH,1540,440,-1);
  aLamp[12]=CreateObject(BLGH,1760,440,-1);
  aLamp[13]=CreateObject(STLH,1955,360,-1);

  //Selbstschussanlagen und Konsolen
  aSelfDefense[0]=CreateObject(SEGU,945,249,-1);
  aSelfDefense[0]->Arm(MISA);
  CreateObject(CONS, 1050, 550, -1)->Set(aSelfDefense[0]);

  aSelfDefense[1]=CreateObject(SEGU,1495,369,-1);
  aSelfDefense[1]->Arm(MISA);

  //Sounds

  //Wind
  CreateObject(SE4K, 310, 380, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 1000, 330, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1320, 380, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1570, 440, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1650, 190, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2040, 280, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2365, 400, -1)->Set("Interior*.ogg",670,105);

  //Eulen
  CreateObject(SE4K, 400, 620, -1)->Set("Owl.wav",1400,35);

  //Erdrutsche
  CreateObject(SE4K, 30, 560, -1)->Set("FallingDirt*.ogg",950,250);
  CreateObject(SE4K, 760, 580, -1)->Set("FallingDirt*.ogg",850,250);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject (AMCT, 135, 450, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 1090, 260, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 1280, 550, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 1720, 210, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 1835, 490, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 2375, 580, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  crate = CreateObject (AMCT, 105, 450, -1);
  crate->Set(GBOX);
  crate = CreateObject (AMCT, 1090, 430, -1);
  crate->Set(GBOX);

  //Versorgungskiste (Splittergranaten)
  crate = CreateObject(AMCT, 2030, 210, -1);
  crate->Set(FRAG);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Denkmal
  CreateObject(MUMT, 720, 410, -1);

  //Radios
  CreateObject(RDIO, 60, 300, -1);
  CreateObject(RDIO, 1580, 57, -1);

  //Leitkegel
  CreateObject(TCCN, 600, 440, -1);
  CreateObject(TCCN, 630, 430, -1);
  CreateObject(TCCN, 660, 420, -1);
  CreateObject(TCCN, 690, 410, -1);

  //Nebel
  CreateParticle("Fog", 110, 600,0,0,900+Random(100));
  CreateParticle("Fog", 180, 640,0,0,400+Random(100));
  CreateParticle("Fog", 260, 630,0,0,700+Random(100));
  CreateParticle("Fog", 390, 610,0,0,1000+Random(100));
  CreateParticle("Fog", 515, 610,0,0,500+Random(100));
  CreateParticle("Fog", 590, 570,0,0,1000+Random(100));
  CreateParticle("Fog", 710, 550,0,0,500+Random(100));

  //Bildschirme
  CreateObject(SCA1, 995, 350, -1)->SetAction("Grenade");
  var screen = CreateObject(SCR3, 995, 380, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCA2, 1570, 150, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR3, 1580, 180, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR1, 1580, 230, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR3, 1580, 260, -1);
  screen->SetClrModulation(RGB(255,255,255));
  CreateObject(SCA2, 2005, 100, -1)->SetAction("Wompf");
  screen = CreateObject(SCR3, 2200, 210, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCA2, 2435, 400, -1);
  screen->SetAction("Scan2");
  screen = CreateObject(SCR3, 2445, 450, -1);
  screen->SetClrModulation(RGB(255,255,255));

  //Labortische
  CreateObject(LTBL,1410, 320, -1);
  CreateObject(LTBL,1510, 320, -1);
  CreateObject(LTBL,1575, 70, -1);
  CreateObject(LTBL,1610, 290, -1);
  CreateObject(LTBL,2300, 530, -1);

  //Flaschen
  CreateObject(BOTL, 10, 332, -1);
  CreateObject(BOTL, 1625, 278, -1);
  CreateObject(BOTL, 2575, 110, -1);
  CreateObject(BOTL, 2580, 110, -1);

  //Monitore
  CreateObject(MNI2, 1420, 308, -1);
  CreateObject(MNI2, 1500, 308, -1);
  CreateObject(MNI2, 1520, 308, -1)->Off();
  CreateObject(MNI2, 1600, 278, -1);
  CreateObject(MNI2, 2290, 518, -1);
  CreateObject(MNI2, 2305, 518, -1)->Off();
  CreateObject(MNI2, 2555, 92, -1);

  //Tische
  CreateObject(GTBL, 940, 430, -1);
  CreateObject(GTBL, 1940, 400, -1);
  CreateObject(GTBL, 1940, 110, -1);

  //Pflanzen
  CreateObject(PLNT, 820, 240, -1);
  CreateObject(PLNT, 820, 300, -1);
  CreateObject(PLNT, 820, 360, -1);
  CreateObject(PLNT, 1400, 308, -1);
  CreateObject(PLNT, 1455, 140, -1);
  CreateObject(PLNT, 1615, 278, -1);
  CreateObject(PLNT, 1955, 98, -1);

  //Geländer
  CreateObject(RAI1, 160, 450, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1250, 550, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 1260, 430, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1320, 320, -1)->SetRail([1]);
  CreateObject(RAI1, 1460, 200, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1680, 210, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 1809, 320, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI2, 1950, 320, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI2, 2000, 210, -1)->SetRail([1,1,1,1,1,1,1,1]);
  CreateObject(RAI2, 2130, 320, -1)->SetRail([1,1,1,1,1,1,1]);
  CreateObject(RAI2, 2130, 510, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI2, 2320, 210, -1)->SetRail([1,1,1]);
  CreateObject(RAI2, 2390, 320, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI2, 2400, 500, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 2305, 110, -1)->SetRail([1,1,1,1,1,1,1,1]);

  //Ventillatoren
  CreateObject(VENT, 2580, 180, -1)->SetCon(15);
  CreateObject(VENT, 2580, 210, -1)->SetCon(15);
  CreateObject(VENT, 2590, 470, -1)->SetCon(23);
  CreateObject(VENT, 2590, 500, -1)->SetCon(23);

  //Dekoschleusen
  CreateObject(GAT1, 925, 350, -1);
  CreateObject(GAT1, 2200, 260, -1);
  CreateObject(GAT1, 2200, 445, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 1430, 440, -1);
  CreateObject(ENGT, 1615, 70, -1);
  CreateObject(ENGT, 1650, 440, -1);
  CreateObject(ENGT, 1870, 440, -1);
  CreateObject(ENGT, 2590, 110, -1);

  //Automaten
  CreateObject(VGMN, 1895, 110, -1);
  CreateObject(VGMN, 1980, 400, -1)->Set(3);

  //Schild
  CreateObject(WLSN, 1865, 310, -1);

  //Wald
  CreateObject(TRE3,255,457,-1);
  CreateObject(TRE3,357,458,-1);
  CreateObject(TRE3,455,457,-1);
  CreateObject(TRE3,555,457,-1);
  CreateObject(TRE1,694,567,-1);
  CreateObject(TRE4,765,555,-1);
  CreateObject(SE4K,30,560,-1);
  CreateObject(SE4K,290,570,-1);
  CreateObject(TRE4,542,626,-1);
  CreateObject(TRE2,512,615,-1);
  CreateObject(TRE4,521,606,-1);
  CreateObject(TRB4,509,618,-1);
  CreateObject(TRE1,523,606,-1);
  CreateObject(TRB4,506,627,-1);
  CreateObject(TRE1,501,628,-1);
  var tree = CreateObject(TRE4,521,606,-1);
  tree->SetCon(59);
  CreateObject(TRE3,546,625,-1);
  CreateObject(TRB4,552,625,-1);
  CreateObject(TRE2,576,607,-1);
  CreateObject(TRE2,677,582,-1);
  CreateObject(BSH2,739,568,-1);
  CreateObject(BSH2,97,658,-1);
  tree = CreateObject(TRE1,127,657,-1);
  tree->SetCon(40);
  CreateObject(TRE4,102,656,-1);
  CreateObject(TRE4,112,657,-1);
  CreateObject(TRE4,127,656,-1);
  CreateObject(TRB4,169,674,-1);
  CreateObject(SE4K,210,620,-1);
  tree = CreateObject(TRE3,288,664,-1);
  tree->SetCon(54);
  CreateObject(TRE4,288,665,-1);
  CreateObject(TRE4,255,661,-1);
  CreateObject(TRE2,290,667,-1);
  CreateObject(TRE1,272,658,-1);
  tree = CreateObject(TRE4,290,667,-1);
  tree->SetCon(64);
  CreateObject(TRE4,275,656,-1);
  tree = CreateObject(GRAS,276,652,-1);
  tree->SetR(8);
  tree->SetCon(59);
  CreateObject(TRB4,307,674,-1);
  CreateObject(TRE2,300,667,-1);
  CreateObject(TRB4,374,656,-1);
  tree = CreateObject(GRAS,372,653,-1);
  tree->SetR(-15);
  tree->SetCon(58);
  CreateObject(BSH2,437,655,-1);
  CreateObject(TRE2,427,657,-1);
  CreateObject(TRE3,434,653,-1);
  tree = CreateObject(GRAS,432,651,-1);
  tree->SetR(-1);
  tree->SetCon(43);
  tree = CreateObject(TRE1,486,651,-1);
  tree->SetCon(40);
  tree = CreateObject(GRAS,480,653,-1);
  tree->SetR(-30);
  tree->SetCon(55);
  CreateObject(TRE4,494,632,-1);
  CreateObject(TRE4,493,634,-1);
  tree = CreateObject(GRAS,492,631,-1);
  tree->SetR(-27);
  tree->SetCon(56);
  tree = CreateObject(TRE4,546,627,-1);
  tree->SetCon(36);
  tree = CreateObject(TRE4,504,627,-1);
  tree->SetCon(38);
  tree = CreateObject(TRE2,508,622,-1);
  tree->SetCon(30);
  tree = CreateObject(GRAS,504,623,-1);
  tree->SetR(-18);
  tree->SetCon(56);
  CreateObject(BSH2,674,640,-1);
  CreateObject(TRB4,655,641,-1);
  CreateObject(STNE,735,665,-1);
  CreateObject(BSH2,149,666,-1);
  tree = CreateObject(TRE4,144,661,-1);
  tree->SetCon(30);
  CreateObject(BSH2,153,667,-1);
  tree = CreateObject(TRE4,176,666,-1);
  tree->SetCon(11);
  CreateObject(STNE,220,690,-1);
  tree = CreateObject(BSH2,297,666,-1);
  tree->SetCon(10);
  tree = CreateObject(GRAS,288,661,-1);
  tree->SetR(11);
  tree->SetCon(58);
  CreateObject(BSH2,290,667,-1);
  tree = CreateObject(GRAS,336,700,-1);
  tree->SetR(13);
  tree->SetCon(60);
  CreateObject(TRE2,312,686,-1);
  CreateObject(BSH2,303,670,-1);
  tree = CreateObject(BSH2,338,705,-1);
  tree->SetCon(59);
  tree = CreateObject(BSH2,331,699,-1);
  tree->SetCon(15);
  CreateObject(TRE4,312,686,-1);
  tree = CreateObject(GRAS,312,682,-1);
  tree->SetR(23);
  tree->SetCon(55);
  tree = CreateObject(GRAS,324,693,-1);
  tree->SetR(10);
  tree->SetCon(60);
  tree = CreateObject(GRAS,300,661,-1);
  tree->SetR(8);
  tree->SetCon(44);
  CreateObject(BSH2,420,705,-1);

  //Büsche
  CreateObject(BSH2, 220, 380, -1);
  CreateObject(BSH2, 25, 570, -1);
  CreateObject(BSH2, 420, 705, -1);
  CreateObject(BSH2, 710, 615, -1);
  CreateObject(BSH2, 855, 520, -1);
  CreateObject(BSH2, 1515, 630, -1);
  CreateObject(BSH2, 2425, 640, -1);

  //Radare
  CreateObject(RADR, 855, 160, -1);
  CreateObject(RADR, 1785, 30, -1);

  //Zäune
  CreateObject(FENC, 30, 450, -1)->Set(2);
  CreateObject(FENC, 90, 450, -1);

  CreateObject(FENC, 990, 200, -1);
  CreateObject(FENC, 1050, 200, -1);
  CreateObject(FENC, 1110, 200, -1);
  CreateObject(FENC, 1170, 200, -1);
  CreateObject(FENC, 1230, 200, -1);
  CreateObject(FENC, 1290, 200, -1);
  CreateObject(FENC, 1350, 200, -1);
  CreateObject(FENC, 1410, 200, -1);

  //Topfpflanzen
  CreateObject(PLT3, 900, 430, -1);
  CreateObject(PLT2, 1475, 320, -1);
  CreateObject(PLT2, 1950, 400, -1);

  //Regale
  CreateObject(FRAM, 1415, 250, -1);
  CreateObject(FRAM, 1440, 250, -1);
  CreateObject(FRAM, 1415, 270, -1);
  CreateObject(FRAM, 1440, 270, -1);
  CreateObject(FRAM, 1415, 290, -1);
  CreateObject(FRAM, 1440, 290, -1);

  CreateObject(FRAM, 1635, 130, -1);
  CreateObject(FRAM, 1665, 130, -1);
  CreateObject(FRAM, 1635, 150, -1);
  CreateObject(FRAM, 1665, 150, -1);
  CreateObject(FRAM, 1635, 170, -1);
  CreateObject(FRAM, 1665, 170, -1);
  CreateObject(FRAM, 1635, 190, -1);
  CreateObject(FRAM, 1665, 190, -1);
  CreateObject(FRAM, 1635, 210, -1);
  CreateObject(FRAM, 1665, 210, -1);
  CreateObject(FRAM, 1635, 230, -1);
  CreateObject(FRAM, 1635, 250, -1);
  CreateObject(FRAM, 1635, 270, -1);

  CreateObject(FRAM, 1755, 110, -1);
  CreateObject(FRAM, 1775, 110, -1);
  CreateObject(FRAM, 1795, 110, -1);
  CreateObject(FRAM, 1815, 110, -1);

  //Straßenlichter
  CreateObject(SLGH, 270, 450, -1);
  CreateObject(SLGH, 370, 450, -1);
  CreateObject(SLGH, 470, 450, -1);
  CreateObject(SLGH, 570, 450, -1);

  //Notausgangslichter
  CreateObject(ETLT, 870, 390, -1);
  CreateObject(ETLT, 1220, 235, -1);
  CreateObject(ETLT, 1480, 170, -1);
  CreateObject(ETLT, 2530, 395, -1);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[1])
   aSelfDefense[0]->SetTeam(iTeam);

  if(pPoint == aFlag[2])
   aSelfDefense[1]->SetTeam(iTeam);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Striking Force.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //DM/LMS-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS))
  {
   //Grenze setzen
   CreateObject(BRDR, 2010, 0, -1)->Set(1);

   //Türen öffnen
   aDoor[00]->Unlock();
   aDoor[01]->Unlock();
   aDoor[02]->Unlock();
   aDoor[03]->Unlock();
   aDoor[04]->Unlock();
   aDoor[05]->Open();
   aDoor[08]->Unlock();
   aDoor[09]->Unlock();
   aDoor[10]->Lock();

   //Objekte entfernen
   RemoveObject(aSelfDefense[0]);
   RemoveObject(aSelfDefense[1]);
   RemoveObject(FindObject2(Find_ID(AMCT),Find_InRect(1080, 240, 40, 40)));

   //Leiter
   CreateObject(LADR, 1650, 457, -1)->Set(20);

   //Metallkisten
   CreateObject(MWCR, 1730, 490, -1);
   CreateObject(MWCR, 1750, 490, -1);
   CreateObject(MWCR, 1860, 490, -1);
   CreateObject(MWCR, 1880, 454, -1);
   CreateObject(MWCR, 1880, 472, -1);
   CreateObject(MWCR, 1880, 490, -1);

   //Kisten
   CreateObject(WCR2, 740, 410, -1);
   CreateObject(WCR2, 760, 410, -1);
   CreateObject(WCR2, 1120, 324, -1);
   CreateObject(WCR2, 1120, 342, -1);
   CreateObject(WCR2, 1120, 360, -1);
   CreateObject(WCR2, 1310, 180, -1);
   CreateObject(WCR2, 1740, 472, -1);

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Raketen
    PlaceSpawnpoint(MBOX, 830, 155);
    PlaceSpawnpoint(MBOX, 1430, 385);

    //Gewehrgranaten
    PlaceSpawnpoint(GBOX, 1530, 545);
   }
  }

  //Assault-Spielziel
  if (FindObject(GASS))
  {
   //Zielobjekte
   AddAssaultTarget(CCP2, 1120, 360, 230, 2, "$Target1$", 0, [[[1320, 260], [1300, 260], [1130, 260]], [[130, 350], [170, 350], [150, 450]]]);
   AddAssaultTarget(MVNT, 1865, 250, 230, 2, "$Target2$", 1, [[[1570, 70], [1590, 70], [1830, 110]], [[1100, 430], [1140, 430], [1280, 550]]]);
   AddAssaultTarget(LBPC, 1795, 490, 230, 2, "$Target3$", 2, [[[1980, 400]], [[1280, 430], [1280, 550]]]);
   AddAssaultTarget(CCP2, 2180, 510, 120, 2, "$Target4$", 3, [[[1880, 110], [2050, 110]], [[1950, 400], [1470, 522], [1420, 390]]]);
   AddAssaultTarget(LBPC, 2200, 320, 120, 2, "$Target5$", 4, [[[1880, 110], [2050, 110]], [[1950, 400], [1470, 522], [1420, 390]]]);
   AddAssaultTarget(CMSN, 2350, 210, 120, 2, "$Target6$", 5, [[[1880, 110], [2050, 110]], [[1950, 400], [1470, 522], [1420, 390]]]);
   AddAssaultTarget(GSTA, 2460, 320, 120, 2, "$Target7$", 6, [[[1880, 110], [2050, 110]], [[1950, 400], [1470, 522], [1420, 390]]]);
   AddAssaultTarget(CCP1, 2455, 500, 120, 2, "$Target8$", 7, [[[1880, 110], [2050, 110]], [[1950, 400], [1470, 522], [1420, 390]]]);

   //Ziele verbinden
   ConnectAssaultTargets([3, 4, 5, 6, 7]);

   //SSA Besitzer setzen
   if(aTeams[2] == true)
   {aSelfDefense[0]->SetTeam(2); aSelfDefense[1]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();

   //Versorgungskiste (APW)
   var crate = CreateObject (AMCT, 940, 160, -1);
   crate->Set(ATWN);

   //Hinweisschilder
   var sign = CreateObject(SGNP, 780, 410, -1);
   sign->SetPhase(1);
   sign->SetMode(1);
   sign = CreateObject(SGNP, 1410, 550, -1);
   sign->SetPhase(1);
   sign->SetMode(1);
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,230,422,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(40,340);
   aFlag[0] -> AddSpawnPoint(120,640);
   aFlag[0] -> AddSpawnPoint(400,640);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,910,430,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(930,150);
   aFlag[1] -> AddSpawnPoint(1050,190);
   aFlag[1] -> AddSpawnPoint(1140,190);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,1670,493,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(1940,390);
   aFlag[2] -> AddSpawnPoint(1980,390);
   aFlag[2] -> AddSpawnPoint(1780,480);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,1585,290,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(1830,310);
   aFlag[3] -> AddSpawnPoint(1620,60);
   aFlag[3] -> AddSpawnPoint(1790,100);
   if(aTeams[2] == true)
   {
    aFlag[3]->Set("$Flag4$",100,2);
    aFlag[3]->Capture(2,1);
   }
   else
   {
    aFlag[3]->Set("$Flag4$",0,2);
   }

   //Grenze setzen
   CreateObject(BRDR, 2010, 0, -1)->Set(1);

   //Türen öffnen
   aDoor[00]->Unlock();
   aDoor[01]->Unlock();
   aDoor[02]->Unlock();
   aDoor[03]->Unlock();
   aDoor[04]->Unlock();
   aDoor[05]->Open();
   aDoor[08]->Unlock();
   aDoor[09]->Unlock();
   aDoor[10]->Lock();

   //Objekt entfernen
   RemoveObject(FindObject2(Find_ID(AMCT),Find_InRect(1080, 240, 40, 40)));

   //Hinweisschilder
   var sign = CreateObject(SGNP, 780, 410, -1);
   sign->SetPhase(1);
   sign->SetMode(1);
   sign = CreateObject(SGNP, 1410, 550, -1);
   sign->SetPhase(1);
   sign->SetMode(1);

   //Konsole erstellen
   CreateObject(CONS, 1810, 485, -1)->Set(aSelfDefense[1]);

   //Leitern
   CreateObject(LADR, 1425, 173, -1)->Set(12);
   CreateObject(LADR, 1650, 457, -1)->Set(20);
   CreateObject(LADR, 1705, 190, -1)->Set(9);

   //Metallkisten
   CreateObject(MWCR, 1730, 490, -1);
   CreateObject(MWCR, 1750, 490, -1);
   CreateObject(MWCR, 1860, 490, -1);
   CreateObject(MWCR, 1880, 454, -1);
   CreateObject(MWCR, 1880, 472, -1);
   CreateObject(MWCR, 1880, 490, -1);

   //Kisten
   CreateObject(WCR2, 740, 410, -1);
   CreateObject(WCR2, 760, 410, -1);
   CreateObject(WCR2, 1120, 324, -1);
   CreateObject(WCR2, 1120, 342, -1);
   CreateObject(WCR2, 1120, 360, -1);
   CreateObject(WCR2, 1310, 180, -1);
   CreateObject(WCR2, 1740, 472, -1);

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Raketen
    PlaceSpawnpoint(MBOX, 830, 155);
    PlaceSpawnpoint(MBOX, 1430, 385);

    //Gewehrgranaten
    PlaceSpawnpoint(GBOX, 1530, 545);
   }
  }
}

/* Assault Zerstörung */

public func OnAssaultTargetDestruction(object pTarget, int iTeam, int iIndex)
{
  //Ziel 1
  if (!iIndex)
  {
   //Rauch
   CreateParticle("GunSmoke",925,340,0,-10,350,1);
   Smoke(925, 340, 30);

   //Türen öffnen
   aDoor[00]->Open();
   aDoor[01]->Open();
   aDoor[02]->Open();
   aDoor[03]->Unlock();
   aDoor[04]->Open();

   //SSA zerstören
   aSelfDefense[0]->Disarm();
   DecoExplode(30, aSelfDefense[0]);

   //Lampen deaktivieren
   aLamp[00]->EMPShock();
   aLamp[01]->EMPShock();
   aLamp[02]->EMPShock();
   aLamp[03]->EMPShock();
  }

  //Ziel 2
  if (iIndex == 1)
  {
   //Grenze setzen
   CreateObject(BRDR, 700, 0, -1)->Set(0,1);

   //Rauch
   CreateParticle("GunSmoke",1820,170,0,-10,200,1);
   Smoke(1825, 170, 30);
   CreateParticle("GunSmoke",1845,170,0,-10,250,1);
   CreateParticle("GunSmoke",1860,170,0,-10,190,1);
   Smoke(1870, 170, 30);
   CreateParticle("GunSmoke",1880,170,0,-10,210,1);
   CreateParticle("GunSmoke",1885,170,0,-10,270,1);

   CreateParticle("GunSmoke",1825,290,0,-10,260,1);
   Smoke(1830, 290, 30);
   CreateParticle("GunSmoke",1845,290,0,-10,220,1);
   CreateParticle("GunSmoke",1855,290,0,-10,240,1);
   CreateParticle("GunSmoke",1875,290,0,-10,230,1);
   Smoke(1870, 1880, 30);
   CreateParticle("GunSmoke",1900,290,0,-10,210,1);

   //Türen öffnen
   aDoor[05]->Open();
   aDoor[08]->Open();
   aDoor[09]->Open();

   //Lampen ausschalten
   aLamp[04]->EMPShock();
   aLamp[05]->EMPShock();
   aLamp[06]->EMPShock();
   aLamp[07]->EMPShock();
   aLamp[08]->EMPShock();
   aLamp[09]->EMPShock();
   aLamp[10]->EMPShock();

   //Risse
   CreateObject(FAUD, 1845, 170, -1)->Set(2);
   CreateObject(FAUD, 1900, 180, -1)->Set(1);
   CreateObject(FAUD, 1850, 225, -1)->Set(3);
   CreateObject(FAUD, 1840, 280, -1)->Set(2);
   CreateObject(FAUD, 1890, 290, -1)->Set(3);
  }

  //Ziel 3
  if (iIndex == 2)
  {
   //Grenze neu setzen
   RemoveAll(BRDR);
   CreateObject(BRDR, 960, 0, -1)->Set(0,1);

   //Rauch
   CreateParticle("GunSmoke",1430,435,0,-10,350,1);
   Smoke(1430, 435, 30);
   CreateParticle("GunSmoke",1650,435,0,-10,350,1);
   Smoke(1650, 435, 30);
   CreateParticle("GunSmoke",1870,435,0,-10,350,1);
   Smoke(1870, 435, 30);

   //Türen öffnen
   aDoor[06]->Open();
   aDoor[07]->Open();
   aDoor[10]->Open();

   //SSA zerstören
   aSelfDefense[1]->Disarm();
   DecoExplode(30, aSelfDefense[1]);

   //Lampen deaktivieren
   aLamp[11]->TurnOff();
   aLamp[12]->TurnOff();
   aLamp[13]->TurnOff();

   //Risse
   CreateObject(FAUD, 1805, 430, -1)->Set(2);
   CreateObject(FAUD, 1750, 470, -1)->Set(1);
   CreateObject(FAUD, 1860, 460, -1)->Set(3);
  }

  //Ziel 4
  if (iIndex == 3)
  {
   //Risse
   CreateObject(FAUD, 2140, 490, -1)->Set(2);
   CreateObject(FAUD, 2185, 470, -1)->Set(1);
   CreateObject(FAUD, 2200, 520, -1)->Set(3);
   CreateObject(FAUD, 2220, 495, -1)->Set(2);
   CreateObject(FAUD, 2260, 485, -1)->Set(3);

   //Rauch
   CreateParticle("GunSmoke",2130,510,0,-10,450,1);
   CreateParticle("GunSmoke",2160,510,0,-10,450,1);
   CreateParticle("GunSmoke",2190,510,0,-10,450,1);
   CreateParticle("GunSmoke",2130,520,0,-10,250,1);
   CreateParticle("GunSmoke",2160,520,-10,250,1);
   CreateParticle("GunSmoke",2190,520,0,-10,250,1);
   Smoke(2180, 490, 50);
  }

  //Ziel 5
  if (iIndex == 4)
  {
   //Risse
   CreateObject(FAUD, 2150, 290, -1)->Set(3);
   CreateObject(FAUD, 2200, 310, -1)->Set(1);

   //Rauch
   CreateParticle("GunSmoke",2165,320,0,-10,450,1);
   CreateParticle("GunSmoke",2195,320,0,-10,450,1);
   CreateParticle("GunSmoke",2225,320,0,-10,450,1);

   CreateParticle("GunSmoke",2165,330,0,-10,250,1);
   CreateParticle("GunSmoke",2195,330,0,-10,250,1);
   CreateParticle("GunSmoke",2225,330,0,-10,250,1);
   Smoke(2200, 305, 50);
  }

  //Ziel 6
  if (iIndex == 5)
  {
   //Risse
   CreateObject(FAUD, 2300, 215, -1)->Set(2);
   CreateObject(FAUD, 2405, 270, -1)->Set(3);

   //Rauch
   CreateParticle("GunSmoke",2330,210,0,-10,450,1);
   CreateParticle("GunSmoke",2350,210,0,-10,450,1);
   CreateParticle("GunSmoke",2370,210,0,-10,450,1);

   CreateParticle("GunSmoke",2330,230,0,-10,250,1);
   CreateParticle("GunSmoke",2350,230,0,-10,250,1);
   CreateParticle("GunSmoke",2370,230,0,-10,250,1);
   Smoke(2355, 210, 50);
  }

  //Ziel 7
  if (iIndex == 6)
  {
   //Risse
   CreateObject(FAUD, 2450, 325, -1)->Set(1);
   CreateObject(FAUD, 2470, 410, -1)->Set(2);

   //Rauch
   CreateParticle("GunSmoke",2400,320,0,-10,450,1);
   CreateParticle("GunSmoke",2430,320,0,-10,450,1);
   CreateParticle("GunSmoke",2460,320,0,-10,450,1);

   CreateParticle("GunSmoke",2400,330,0,-10,250,1);
   CreateParticle("GunSmoke",2430,330,0,-10,250,1);
   CreateParticle("GunSmoke",2460,330,0,-10,250,1);
   Smoke(2440, 310, 50);
  }

  //Ziel 8
  if (iIndex == 7)
  {
   //Risse
   CreateObject(FAUD, 2410, 500, -1)->Set(2);
   CreateObject(FAUD, 2460, 510, -1)->Set(1);

   //Rauch
   CreateParticle("GunSmoke",2410,500,0,-10,450,1);
   CreateParticle("GunSmoke",2440,500,0,-10,450,1);
   CreateParticle("GunSmoke",2470,500,0,-10,450,1);

   CreateParticle("GunSmoke",2410,510,0,-10,250,1);
   CreateParticle("GunSmoke",2440,510,0,-10,250,1);
   CreateParticle("GunSmoke",2470,510,0,-10,250,1);
   Smoke(2455, 490, 50);
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
     { iX = 120; iY = 340; }
    if(!--rand)
     { iX = 120; iY = 440; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 1740; iY = 200; }
    if(!--rand)
     { iX = 1740; iY = 280; }
   }
   return(1);
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {if(FindObject(GASS)->GetRespawnPoint(iX, iY, iTeam)) return 1;}

  //Startsicht
  iX =760; iY = 300;
}