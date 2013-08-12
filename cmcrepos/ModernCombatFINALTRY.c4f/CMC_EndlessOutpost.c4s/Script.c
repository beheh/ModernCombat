/*-- Endless Outpost --*/

#strict
#include CSTD

static aFlag,aStationary,aDoor,aSelfDefense,aLamp;

func RecommendedGoals()			{return [GASS];}	//Spielzielempfehlung
public func AssaultDefenderTeam()	{return 2;}		//Verteidigerteam bei Assault


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Bildschirmfärbung
  SetGamma(RGB(0,0,0), RGB(80,80,80), RGB(200,200,200));
  //Flaggen
  aFlag = [];
  //Geschützstellungen
  aStationary = [];
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
  DrawMaterialQuad("Wall-Concrete1",980,450,1010,440,1010,450,995,450,true);
  DrawMaterialQuad("Wall-Concrete1",1010,440,1040,430,1040,440,1025,440,true);
  DrawMaterialQuad("Wall-Concrete3",1070,420,1100,410,1100,420,1085,420,true);
  DrawMaterialQuad("Wall-Concrete1",1040,430,1070,420,1070,430,1055,430,true);

  DrawMaterialQuad("Wall-Concrete3",1261,420,1231,410,1231,420,1246,420,true);
  DrawMaterialQuad("Wall-Concrete1",1291,430,1261,420,1261,430,1276,430,true);

  DrawMaterialQuad("Wall-Concrete1",1450,370,1480,360,1480,370,1465,370,true);
  DrawMaterialQuad("Wall-Concrete1",1551,371,1551,361,1581,371,1551,371,true);

  DrawMaterialQuad("Wall-Concrete1",1410,571,1440,561,1440,571,1425,571,true);
  DrawMaterialQuad("Wall-Concrete1",1551,570,1521,560,1521,570,1536,570,true);

  DrawMaterialQuad("Wall-Plate",1620,560,1650,550,1650,560,1635,560,true);

  DrawMaterialQuad("Wall-Metal2",2381,510,2351,500,2351,510,2366,510,true);
  DrawMaterialQuad("Wall-Metal2",2351,500,2321,490,2321,500,2336,500,true);
  DrawMaterialQuad("Wall-Metal2",2661,520,2631,510,2631,520,2646,520,true);

  DrawMaterialQuad("Wall-Concrete3",2881,580,2881,570,2851,580,2881,580,true);
  DrawMaterialQuad("Wall-Concrete3",2911,570,2911,560,2881,570,2911,570,true);

  //Leitern
  CreateObject(LADR, 470, 622, -1)->Set(33);
  CreateObject(LADR, 1120, 530, -1)->Set(14);
  CreateObject(LADR, 1250, 400, -1)->Set(29);
  CreateObject(LADR, 1385, 570, -1)->Set(17);
  CreateObject(LADR, 1440, 420, -1)->Set(19);
  CreateObject(LADR, 1605, 568, -1)->Set(45);
  CreateObject(LADR, 1785, 520, -1)->Set(15);
  CreateObject(LADR, 1881, 280, -1)->Set(9);
  CreateObject(LADR, 2070, 273, -1)->Set(7);
  CreateObject(LADR, 2270, 578, -1)->Set(10);
  CreateObject(LADR, 2385, 320, -1)->Set(14);
  CreateObject(LADR, 2480, 480, -1)->Set(33);
  CreateObject(LADR, 2520, 480, -1)->Set(20);
  CreateObject(LADR, 2680, 490, -1)->Set(21);
  CreateObject(LADR, 2685, 221, -1)->Set(13);
  CreateObject(LADR, 2710, 490, -1)->Set(34);
  CreateObject(LADR, 2890, 500, -1)->Set(11);
  CreateObject(LADR, 2940, 500, -1)->Set(36);
  CreateObject(LADR, 2960, 560, -1)->Set(7);
  CreateObject(LADR, 3000, 320, -1)->Set(13);
  CreateObject(LADR, 3020, 210, -1)->Set(12);

  //Bodenluken
  CreateObject(HA4K, 470, 353, -1);
  CreateObject(HA4K, 470, 453, -1);
  CreateObject(HA4K, 1120, 413, -1);
  CreateObject(HA4K, 1250, 163, -1);
  CreateObject(HA4K, 1880, 203, -1);
  CreateObject(HA4K, 2480, 213, -1);
  CreateObject(HA4K, 2670, 533, -1);
  CreateObject(HA4K, 2750, 533, -1);
  CreateObject(HA4K, 2940, 353, -1);
  CreateObject(HA4K, 2940, 413, -1);
  CreateObject(HA4K, 2960, 503, -1);
  CreateObject(HA4K, 3000, 213, -1);
  CreateObject(HA4K, 3020, 113, -1);

  //Große Bodenluken
  CreateObject(H24K, 1385, 438, -1);
  CreateObject(H24K, 1605, 208, -1);
  CreateObject(H24K, 1605, 448, -1);
  aDoor[11] = CreateObject(H24K, 2385, 118, -1);
  aDoor[11]->Lock();
  CreateObject(H24K, 2685, 118, -1);
  CreateObject(H24K, 2875, 118, -1);

  //Gerüste
  CreateObject(SFFG, 410, 300, -1)->Set(5);
  CreateObject(SFFG, 410, 350, -1)->Set(5);
  CreateObject(SFFG, 490, 300, -1)->Set(2);
  CreateObject(SFFG, 490, 350, -1)->Set(3);
  CreateObject(SFFG, 570, 250, -1)->Set(3);
  CreateObject(SFFG, 570, 300, -1);
  CreateObject(SFFG, 570, 350, -1);
  CreateObject(SFFG, 1480, 560, -1)->Set(3);
  CreateObject(SFFG, 2570, 410, -1)->Set(5);
  CreateObject(SFFG, 2570, 460, -1)->Set(3);
  CreateObject(SFFG, 2570, 510, -1)->Set(4);
  CreateObject(SFFG, 2840, 450, -1);
  CreateObject(SFFG, 2840, 500, -1)->Set(4);
  CreateObject(SFFG, 3160, 110, -1)->Set(3);
  CreateObject(SFFG, 3240, 110, -1);

  //Schutztüren
  CreateObject(GDDR, 1215, 410, -1)->Lock();
  CreateObject(GDDR, 1275, 550, -1)->Lock();
  CreateObject(GDDR, 1365, 550, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 1465, 260, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 1465, 430, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 1565, 260, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 1645, 260, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 2095, 290, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 2195, 210, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 2195, 290, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 2305, 400, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 2650, 110, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 2910, 110, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 2915, 210, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 2915, 410, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 2915, 500, -1)->SetColorDw(HSL(40, 210, 100, 127));

  //Hydrauliktüren
  CreateObject(SEDR,2470, 110,-1)->Lock();
  CreateObject(SEDR,2960, 410,-1)->Lock();

  //Stahlbrücken
  CreateObject(_HBR, 115, 462, -1)->SwitchMode();
  CreateObject(_HBR, 215, 462, -1)->SwitchMode();
  CreateObject(_HBR, 315, 462, -1)->SwitchMode();
  CreateObject(_HBR, 705, 462, -1)->SwitchMode();
  CreateObject(_HBR, 805, 462, -1)->SwitchMode();
  CreateObject(_HBR, 905, 462, -1)->SwitchMode();
  CreateObject(_HBR, 2754, 332, -1);
  CreateObject(_HBR, 2864, 222, -1);

  //Kisten
  CreateObject(WCR2, 410, 350, -1)->AutoRespawn();
  CreateObject(WCR2, 1290, 550, -1);
  CreateObject(WCR2, 1520, 324, -1);
  CreateObject(WCR2, 1520, 342, -1);
  CreateObject(WCR2, 1520, 360, -1);
  CreateObject(WCR2, 1860, 70, -1);
  CreateObject(WCR2, 1905, 200, -1)->AutoRespawn();
  CreateObject(WCR2, 2215, 580, -1);
  CreateObject(WCR2, 2440, 580, -1);
  CreateObject(WCR2, 2400, 456, -1);
  CreateObject(WCR2, 2400, 474, -1);
  CreateObject(WCR2, 2400, 492, -1);
  CreateObject(WCR2, 2400, 510, -1);
  CreateObject(WCR2, 2420, 492, -1);
  CreateObject(WCR2, 2440, 510, -1);
  CreateObject(WCR2, 2460, 510, -1);
  CreateObject(WCR2, 2510, 110, -1);
  CreateObject(WCR2, 2545, 320, -1);
  CreateObject(WCR2, 2655, 320, -1);
  CreateObject(WCR2, 2805, 320, -1);
  CreateObject(WCR2, 2815, 500, -1);

  //Metallkisten
  CreateObject(MWCR, 550, 300, -1);
  CreateObject(MWCR, 1590, 570, -1);
  CreateObject(MWCR, 1610, 570, -1);
  CreateObject(MWCR, 2310, 320, -1)->AutoRespawn();
  CreateObject(MWCR, 2460, 580, -1);
  CreateObject(MWCR, 2810, 580, -1);
  CreateObject(MWCR, 3110, 110, -1);

  //Verbandskisten
  CreateObject(BECR, 755, 450, -1)->AutoRespawn();
  CreateObject(BECR, 1500, 360, -1)->AutoRespawn();
  CreateObject(BECR, 1860, 52, -1);
  CreateObject(BECR, 2195, 580, -1);
  CreateObject(BECR, 2420, 510, -1);
  CreateObject(BECR, 2810, 450, -1);
  CreateObject(BECR, 2960, 110, -1);

  //Gasflaschen
  CreateObject(GSBL, 1420, 430, -1)->AutoRespawn();
  CreateObject(GSBL, 2040, 520, -1)->AutoRespawn();
  CreateObject(GSBL, 2605, 110, -1)->AutoRespawn();
  CreateObject(GSBL, 3000, 560, -1)->AutoRespawn();
  CreateObject(GSBL, 3190, 110, -1);

  //Hinweisschilder
  var sign = CreateObject(SNPT, 70, 450, -1);
  sign->SetAction("Sign6");
  sign->Light();
  CreateObject(SGNP, 265, 450, -1);

  //Explosive Kisten
  CreateObject(XWCR, 1945, 320, -1)->AutoRespawn();
  CreateObject(XWCR, 2550, 210, -1)->AutoRespawn();
  CreateObject(XWCR, 3070, 410, -1);

  //Benzinfässer
  CreateObject(PBRL, 880, 450, -1);
  CreateObject(PBRL, 2725, 530, -1);
  CreateObject(PBRL, 2985, 560, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 1660, 550, -1)->AutoRespawn();
  CreateObject(XBRL, 2290, 320, -1)->AutoRespawn();
  CreateObject(XBRL, 2735, 530, -1);
  CreateObject(XBRL, 2820, 320, -1);
  CreateObject(XBRL, 2830, 320, -1);

  //Phosphorfässer
  CreateObject(HBRL, 2120, 290, -1)->AutoRespawn();
  CreateObject(HBRL, 2565, 320, -1);
  CreateObject(HBRL, 2620, 110, -1)->AutoRespawn();

  //Benzintanks
  CreateObject(XTNK, 2010, 520, -1)->AutoRespawn();
  CreateObject(XTNK, 2140, 210, -1)->AutoRespawn();
  CreateObject(XTNK, 3200, 410, -1);

  //Palette
  CreateObject(PLLT, 1290, 160, -1);

  //Container
  var container = CreateObject(CON1, 390, 450, -1);
  container->SetPerspective();
  container->SetGraphics("5");
  CreateObject(CON1, 620, 450, -1)->SetGraphics("5");
  container = CreateObject(CON1, 857, 450, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  CreateObject(CON1, 1885, 550, -1)->SetGraphics("5");
  container = CreateObject(CON1, 1895, 520, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  CreateObject(CON1, 2080, 520, -1)->SetGraphics("5");
  CreateObject(CON1, 2570, 110, -1)->SetGraphics("5");
  CreateObject(CON1, 3070, 110, -1)->SetGraphics("5");
  container = CreateObject(CON1, 3090, 83, -1);
  container->SetPerspective();
  container->SetGraphics("5");
  container = CreateObject(CON1, 3120, 410, -1);
  container->SetPerspective();
  container->SetGraphics("5");
  CreateObject(CON1, 3130, 382, -1)->SetGraphics("5");
  CreateObject(CON1, 3230, 410, -1)->SetGraphics("5");

  //Autos
  CreateObject(CCAR, 215, 450, -1);
  CreateObject(CCAR, 950, 450, -1);

  //Glasscheiben
  CreateObject(PANE, 1202, 240, -1);
  CreateObject(PANE, 1202, 300, -1);
  CreateObject(PANE, 1202, 360, -1);

  CreateObject(PANE, 1482, 310, -1);
  CreateObject(PANE, 1482, 335, -1);
  CreateObject(PANE, 1482, 360, -1);

  CreateObject(PANE, 1547, 310, -1);
  CreateObject(PANE, 1547, 335, -1);
  CreateObject(PANE, 1547, 360, -1);

  CreateObject(PANE, 1842, 140, -1);

  CreateObject(PAN2, 2225, 215, -1);
  CreateObject(PAN2, 2305, 215, -1);

  CreateObject(PAN2, 2445, 325, -1);

  CreateObject(PAN2, 2575, 215, -1);
  CreateObject(PAN2, 2605, 215, -1);
  CreateObject(PAN2, 2635, 215, -1);

  //Steine
  CreateObject(STNE, 70, 680, -1)->Set(4);
  CreateObject(STNE, 160, 730, -1)->Set(2);
  CreateObject(STNE, 275, 680, -1)->Set(1);
  CreateObject(STNE, 620, 690, -1)->Set(1);
  CreateObject(STNE, 1135, 665, -1)->Set(5);

  //Hydrauliküren
  aDoor[00] = CreateObject(SEDR,1625, 560,-1);
   aDoor[00]->Lock();
  aDoor[01] = CreateObject(SEDR,1645, 430,-1);
   aDoor[01]->Lock();
  aDoor[02] = CreateObject(SEDR,1755, 260,-1);
   aDoor[02]->Lock();
  aDoor[03] = CreateObject(SEDR,1850, 200,-1);
   aDoor[03]->Lock();
  aDoor[04] = CreateObject(HNG2,1835,140,-1);

  aDoor[05] = CreateObject(HNG2,1755,550,-1);
  aDoor[06] = CreateObject(HA4K, 1840, 323, -1);
   aDoor[06]->Lock();
  aDoor[07] = CreateObject(HA4K, 2050, 293, -1);
   aDoor[07]->Lock();

  aDoor[08] = CreateObject(SEDR,2250,110,-1);
   aDoor[08]->Lock();
  aDoor[09] = CreateObject(HA4K, 2270, 493, -1);
   aDoor[09]->Lock();
  aDoor[10] = CreateObject(HNG2,2305,490,-1);

  //Lampen
  aLamp[00]=CreateObject(BLGH,1280,300,-1);
  aLamp[01]=CreateObject(BLGH,1380,300,-1);
  aLamp[02]=CreateObject(CLGH,1445,475,-1);
  aLamp[03]=CreateObject(CLGH,1525,475,-1);
  aLamp[04]=CreateObject(LLGH,1583,380,-1);
  aLamp[05]=CreateObject(LLGH,1627,295,-1);
  aLamp[06]=CreateObject(STLH,1830,225,-1);
  aLamp[07]=CreateObject(STLH,2050,95,-1);
  aLamp[08]=CreateObject(BLGH,1700,305,-1);
  aLamp[09]=CreateObject(BLGH,1700,390,-1);
  aLamp[10]=CreateObject(BLGH,2265,270,-1);
  aLamp[11]=CreateObject(BLGH,1940,440,-1);
  aLamp[12]=CreateObject(BLGH,2160,440,-1);
  aLamp[13]=CreateObject(STLH,2355,360,-1);
  aLamp[14]=CreateObject(ALGH,2600,140,-1);
  aLamp[14]->SetR(180);
  aLamp[15]=CreateObject(ALGH,2600,350,-1);
  aLamp[15]->SetR(180);

  //Selbstschussanlagen und Konsolen
  aSelfDefense[0]=CreateObject(SEGU,1345,249,-1);
  aSelfDefense[0]->Arm(MISA);
  CreateObject(CONS, 1330, 540, -1)->Set(aSelfDefense[0]);
  aSelfDefense[1]=CreateObject(SEGU,1895,369,-1);
  aSelfDefense[1]->Arm(MISA);

  //Sounds

  //Wind
  CreateObject(SE4K, 710, 380, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 1400, 330, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1720, 380, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1970, 440, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2050, 190, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2440, 280, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2765, 400, -1)->Set("Interior*.ogg",670,105);

  //Rush Hour
  CreateObject(SE4K, 200, 400, -1)->Set("Traffic*.ogg",245,70);

  //Sirenen
  CreateObject(SE4K, 100, 400, -1)->Set("Siren*.ogg",4000,140);

  //Eulen
  CreateObject(SE4K, 800, 620, -1)->Set("Owl.wav",1400,35);

  //Erdrutsche
  CreateObject(SE4K, 430, 560, -1)->Set("FallingDirt*.ogg",950,250);
  CreateObject(SE4K, 1160, 580, -1)->Set("FallingDirt*.ogg",850,250);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject (AMCT, 535, 450, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 1490, 260, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 1680, 550, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 2120, 210, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 2235, 490, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 2775, 580, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  crate = CreateObject (AMCT, 505, 450, -1);
  crate->Set(GBOX);
  crate = CreateObject (AMCT, 1490, 430, -1);
  crate->Set(GBOX);

  //Versorgungskiste (Splittergranaten)
  crate = CreateObject(AMCT, 2430, 210, -1);
  crate->Set(FRAG);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Denkmal
  CreateObject(MUMT, 1120, 410, -1);

  //Radios
  CreateObject(RDIO, 460, 300, -1);
  CreateObject(RDIO, 1980, 57, -1);

  //Leitkegel
  CreateObject(TCCN, 1000, 440, -1);
  CreateObject(TCCN, 1030, 430, -1);
  CreateObject(TCCN, 1060, 420, -1);
  CreateObject(TCCN, 1090, 410, -1);

  //Nebel
  CreateParticle("Fog", 130, 660,0,0,900+Random(100));
  CreateParticle("Fog", 250, 650,0,0,1000+Random(100));
  CreateParticle("Fog", 280, 620,0,0,900+Random(100));
  CreateParticle("Fog", 510, 600,0,0,900+Random(100));
  CreateParticle("Fog", 580, 640,0,0,400+Random(100));
  CreateParticle("Fog", 660, 630,0,0,700+Random(100));
  CreateParticle("Fog", 790, 610,0,0,1000+Random(100));
  CreateParticle("Fog", 915, 610,0,0,500+Random(100));
  CreateParticle("Fog", 990, 570,0,0,1000+Random(100));
  CreateParticle("Fog", 1110, 550,0,0,500+Random(100));

  //Bildschirme
  CreateObject(SCA1, 1395, 350, -1)->SetAction("Eurocorps");
  var screen = CreateObject(SCR3, 1395, 380, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCA2, 1970, 150, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR3, 1980, 180, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR1, 1980, 230, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR3, 1980, 260, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR3, 2375, 310, -1);
  screen->SetClrModulation(RGB(255,0,0));
  CreateObject(SCA1, 2405, 100, -1)->SetAction("Eurocorps");
  screen = CreateObject(SCR3, 2415, 310, -1);
  screen->SetClrModulation(RGB(255,0,0));
  screen->SetPhase(5);
  screen = CreateObject(SCR3, 2600, 210, -1);
  screen->SetClrModulation(RGB(255,255,20));
  screen = CreateObject(SCR3, 2640, 210, -1);
  screen->SetClrModulation(RGB(255,255,20));
  screen->SetPhase(10);
  screen = CreateObject(SCR3, 2805, 450, -1);
  screen->SetClrModulation(RGB(00,150,250));
  screen = CreateObject(SCA2, 2835, 400, -1);
  screen->SetAction("Scan2");
  screen->SetClrModulation(RGB(00,150,250));
  screen = CreateObject(SCR3, 2845, 450, -1);
  screen->SetClrModulation(RGB(00,150,250));
  screen->SetPhase(15);

  //Labortische
  CreateObject(LTBL,1810, 320, -1);
  CreateObject(LTBL,1910, 320, -1);
  CreateObject(LTBL,1975, 70, -1);
  CreateObject(LTBL,2010, 290, -1);
  CreateObject(LTBL,2700, 530, -1);

  //Flaschen
  CreateObject(BOTL, 410, 332, -1);
  CreateObject(BOTL, 2025, 278, -1);
  CreateObject(BOTL, 2975, 110, -1);
  CreateObject(BOTL, 2980, 110, -1);

  //Monitore
  CreateObject(MNI2, 1820, 308, -1);
  CreateObject(MNI2, 1900, 308, -1);
  CreateObject(MNI2, 1920, 308, -1)->Off();
  CreateObject(MNI2, 2000, 278, -1);
  CreateObject(MNI2, 2690, 518, -1);
  CreateObject(MNI2, 2705, 518, -1)->Off();
  CreateObject(MNI2, 2955, 92, -1);

  //Tisch
  CreateObject(GTBL, 2340, 110, -1);

  //Topfpflanzen
  CreateObject(PLNT, 1220, 240, -1);
  CreateObject(PLNT, 1220, 300, -1);
  CreateObject(PLNT, 1220, 360, -1);
  CreateObject(PLT3, 1300, 430, -1);
  CreateObject(PLT3, 1360, 430, -1);
  CreateObject(PLNT, 1800, 308, -1);
  CreateObject(PLNT, 1855, 140, -1);
  CreateObject(PLT3, 1875, 320, -1);
  CreateObject(PLNT, 2015, 278, -1);
  CreateObject(PLT3, 2340, 400, -1);
  CreateObject(PLNT, 2355, 98, -1);

  //Geländer
  CreateObject(RAI1, -5, 450, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1650, 550, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 1660, 430, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1720, 320, -1)->SetRail([1]);
  CreateObject(RAI1, 1860, 200, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 2080, 210, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 2209, 320, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI2, 2350, 320, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI2, 2400, 210, -1)->SetRail([1,1,1,1,1,1,1,1]);
  CreateObject(RAI2, 2530, 320, -1)->SetRail([1,1,1,1,1,1,1]);
  CreateObject(RAI2, 2530, 510, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI2, 2720, 210, -1)->SetRail([1,1,1]);
  CreateObject(RAI2, 2790, 320, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI2, 2800, 500, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 2705, 110, -1)->SetRail([1,1,1,1,1,1,1,1]);

  //Ventillatoren
  CreateObject(MVNT, 2265, 250, -1);
  CreateObject(VENT, 2980, 180, -1)->SetCon(15);
  CreateObject(VENT, 2980, 210, -1)->SetCon(15);
  CreateObject(VENT, 2990, 470, -1)->SetCon(23);
  CreateObject(VENT, 2990, 500, -1)->SetCon(23);

  //Dekoschleusen
  CreateObject(GAT1, 1325, 350, -1);
  CreateObject(GAT1, 3070, 360, -1);
  CreateObject(GAT1, 3170, 360, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 1830, 440, -1);
  CreateObject(ENGT, 2015, 70, -1);
  CreateObject(ENGT, 2050, 440, -1);
  CreateObject(ENGT, 2270, 440, -1);
  CreateObject(ENGT, 2990, 110, -1);

  //Automaten
  CreateObject(VGMN, 2295, 110, -1);
  CreateObject(VGMN, 2380, 400, -1)->Set(3);

  //Wald
  CreateObject(TRE3,165,458,-1);
  CreateObject(TRE3,265,458,-1);
  CreateObject(TRE3,355,458,-1);
  CreateObject(TRE3,665,458,-1);
  CreateObject(TRE3,755,458,-1);
  CreateObject(TRE3,855,458,-1);
  CreateObject(TRE3,945,458,-1);
  CreateObject(TRE1,1094,567,-1);
  CreateObject(TRE4,1165,555,-1);
  CreateObject(TRE4,942,626,-1);
  CreateObject(TRE2,912,615,-1);
  CreateObject(TRE4,921,606,-1);
  CreateObject(TRB4,909,618,-1);
  CreateObject(TRE1,923,606,-1);
  CreateObject(TRB4,906,627,-1);
  CreateObject(TRE1,901,628,-1);
  var tree = CreateObject(TRE4,921,606,-1);
  tree->SetCon(59);
  CreateObject(TRE3,946,625,-1);
  CreateObject(TRB4,952,625,-1);
  CreateObject(TRE2,976,607,-1);
  CreateObject(TRE2,1077,582,-1);
  CreateObject(BSH2,1139,568,-1);
  CreateObject(BSH2,497,658,-1);
  tree = CreateObject(TRE1,527,657,-1);
  tree->SetCon(40);
  CreateObject(TRE4,502,656,-1);
  CreateObject(TRE4,512,657,-1);
  CreateObject(TRE4,527,656,-1);
  CreateObject(TRB4,569,674,-1);
  CreateObject(SE4K,610,620,-1);
  tree = CreateObject(TRE3,688,664,-1);
  tree->SetCon(54);
  CreateObject(TRE4,688,665,-1);
  CreateObject(TRE4,655,661,-1);
  CreateObject(TRE2,690,667,-1);
  CreateObject(TRE1,672,658,-1);
  tree = CreateObject(TRE4,690,667,-1);
  tree->SetCon(64);
  CreateObject(TRE4,675,656,-1);
  tree = CreateObject(GRAS,676,652,-1);
  tree->SetR(8);
  tree->SetCon(59);
  CreateObject(TRB4,707,674,-1);
  CreateObject(TRE2,700,667,-1);
  CreateObject(TRB4,774,656,-1);
  tree = CreateObject(GRAS,772,653,-1);
  tree->SetR(-15);
  tree->SetCon(58);
  CreateObject(BSH2,837,655,-1);
  CreateObject(TRE2,827,657,-1);
  CreateObject(TRE3,834,653,-1);
  tree = CreateObject(GRAS,832,651,-1);
  tree->SetR(-1);
  tree->SetCon(43);
  tree = CreateObject(TRE1,886,651,-1);
  tree->SetCon(40);
  tree = CreateObject(GRAS,880,653,-1);
  tree->SetR(-30);
  tree->SetCon(55);
  CreateObject(TRE4,894,632,-1);
  CreateObject(TRE4,893,634,-1);
  tree = CreateObject(GRAS,892,631,-1);
  tree->SetR(-27);
  tree->SetCon(56);
  tree = CreateObject(TRE4,946,627,-1);
  tree->SetCon(36);
  tree = CreateObject(TRE4,904,627,-1);
  tree->SetCon(38);
  tree = CreateObject(TRE2,908,622,-1);
  tree->SetCon(30);
  tree = CreateObject(GRAS,904,623,-1);
  tree->SetR(-18);
  tree->SetCon(56);
  CreateObject(BSH2,1074,640,-1);
  CreateObject(TRB4,1055,641,-1);
  CreateObject(BSH2,549,666,-1);
  tree = CreateObject(TRE4,544,661,-1);
  tree->SetCon(30);
  CreateObject(BSH2,553,667,-1);
  tree = CreateObject(TRE4,576,666,-1);
  tree->SetCon(11);
  tree = CreateObject(BSH2,697,666,-1);
  tree->SetCon(10);
  tree = CreateObject(GRAS,688,661,-1);
  tree->SetR(11);
  tree->SetCon(58);
  CreateObject(BSH2,690,667,-1);
  tree = CreateObject(GRAS,736,700,-1);
  tree->SetR(13);
  tree->SetCon(60);
  CreateObject(TRE2,712,686,-1);
  CreateObject(BSH2,703,670,-1);
  tree = CreateObject(BSH2,738,705,-1);
  tree->SetCon(59);
  tree = CreateObject(BSH2,731,699,-1);
  tree->SetCon(15);
  CreateObject(TRE4,712,686,-1);
  tree = CreateObject(GRAS,712,682,-1);
  tree->SetR(23);
  tree->SetCon(55);
  tree = CreateObject(GRAS,724,693,-1);
  tree->SetR(10);
  tree->SetCon(60);
  tree = CreateObject(GRAS,700,661,-1);
  tree->SetR(8);
  tree->SetCon(44);
  CreateObject(BSH2,820,705,-1);

  //Büsche
  CreateObject(BSH2, 50, 600, -1);
  CreateObject(BSH2, 305, 680, -1);
  CreateObject(BSH2, 440, 580, -1);
  CreateObject(BSH2, 620, 380, -1);
  CreateObject(BSH2, 820, 705, -1);
  CreateObject(BSH2, 1110, 615, -1);
  CreateObject(BSH2, 1255, 520, -1);
  CreateObject(BSH2, 1915, 630, -1);
  CreateObject(BSH2, 2825, 640, -1);

  //Radare
  CreateObject(RADR, 1255, 160, -1);
  CreateObject(RADR, 2185, 30, -1);

  //Zäune
  CreateObject(FENC, 1390, 200, -1);
  CreateObject(FENC, 1450, 200, -1);
  CreateObject(FENC, 1510, 200, -1);
  CreateObject(FENC, 1570, 200, -1);
  CreateObject(FENC, 1630, 200, -1);
  CreateObject(FENC, 1690, 200, -1);
  CreateObject(FENC, 1750, 200, -1);
  CreateObject(FENC, 1810, 200, -1);

  //Regale
  CreateObject(FRAM, 1815, 250, -1);
  CreateObject(FRAM, 1840, 250, -1);
  CreateObject(FRAM, 1815, 270, -1);
  CreateObject(FRAM, 1840, 270, -1);
  CreateObject(FRAM, 1815, 290, -1);
  CreateObject(FRAM, 1840, 290, -1);

  CreateObject(FRAM, 2035, 130, -1);
  CreateObject(FRAM, 2065, 130, -1);
  CreateObject(FRAM, 2035, 150, -1);
  CreateObject(FRAM, 2065, 150, -1);
  CreateObject(FRAM, 2035, 170, -1);
  CreateObject(FRAM, 2065, 170, -1);
  CreateObject(FRAM, 2035, 190, -1);
  CreateObject(FRAM, 2065, 190, -1);
  CreateObject(FRAM, 2035, 210, -1);
  CreateObject(FRAM, 2065, 210, -1);
  CreateObject(FRAM, 2035, 230, -1);
  CreateObject(FRAM, 2035, 250, -1);
  CreateObject(FRAM, 2035, 270, -1);

  CreateObject(FRAM, 2155, 110, -1);
  CreateObject(FRAM, 2175, 110, -1);
  CreateObject(FRAM, 2195, 110, -1);
  CreateObject(FRAM, 2215, 110, -1);

  //Feuerlöscher
  CreateObject(FIE2, 1970, 285, -1);
  CreateObject(FIE2, 2960, 345, -1);

  //Straßenlichter
  CreateObject(SLGH, 180, 450, -1);
  CreateObject(SLGH, 280, 450, -1);
  CreateObject(SLGH, 415, 450, -1);
  CreateObject(SLGH, 485, 450, -1);
  CreateObject(SLGH, 565, 450, -1);
  CreateObject(SLGH, 670, 450, -1);
  CreateObject(SLGH, 770, 450, -1);
  CreateObject(SLGH, 870, 450, -1);
  CreateObject(SLGH, 970, 450, -1);

  //Notausgangslichter
  CreateObject(ETLT, 1270, 390, -1);
  CreateObject(ETLT, 1620, 235, -1);
  CreateObject(ETLT, 1880, 170, -1);
  CreateObject(ETLT, 2930, 395, -1);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD03,900,200,-1)->SetClrModulation(RGB(150,150,150));
  CreateObject(BD10,300,870,-1)->SetClrModulation(RGB(130,130,130));
  CreateObject(BD10,1690,870,-1)->SetClrModulation(RGB(130,130,130));
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
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Striking Force.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,630,422,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(440,340);
   aFlag[0] -> AddSpawnPoint(520,640);
   aFlag[0] -> AddSpawnPoint(800,640);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,1330,430,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(1330,150);
   aFlag[1] -> AddSpawnPoint(1450,190);
   aFlag[1] -> AddSpawnPoint(1540,190);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,2070,493,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(2340,390);
   aFlag[2] -> AddSpawnPoint(2380,390);
   aFlag[2] -> AddSpawnPoint(2180,480);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,1985,290,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(2230,310);
   aFlag[3] -> AddSpawnPoint(2020,60);
   aFlag[3] -> AddSpawnPoint(2190,100);
   if(aTeams[2] == true)
   {
    aFlag[3]->Set("$Flag4$",100,2);
    aFlag[3]->Capture(2,1);
   }
   else
   {
    aFlag[3]->Set("$Flag4$",0,2);
   }

   //Grenzen setzen
   CreateObject(BRDR, 360, 0, -1)->Set(0);
   CreateObject(BRDR, 2410, 0, -1)->Set(1);

   //Türen öffnen
   aDoor[00]->Open();
   aDoor[01]->Open();
   aDoor[02]->Open();
   aDoor[03]->Open();
   aDoor[04]->Open();

   aDoor[05]->Open();
   aDoor[06]->Unlock();
   aDoor[07]->Unlock();

   //Objekt entfernen
   RemoveObject(FindObject2(Find_ID(AMCT),Find_InRect(1480, 240, 40, 40)));

   //Geschützstellungen
   CreateObject(GNET, 605, 350, -1)->Set(0,90,1);
   CreateObject(GNET, 2090, 210, -1)->Set(0,-90,1);

   //Hinweisschilder
   var sign = CreateObject(SGNP, 1180, 410, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 1810, 550, -1);
   sign->SetMode(1);
   sign->Set("Turret");

   //Konsole erstellen
   CreateObject(CONS, 2210, 485, -1)->Set(aSelfDefense[1]);

   //Leitern
   CreateObject(LADR, 1815, 173, -1)->Set(12);
   CreateObject(LADR, 2050, 457, -1)->Set(20);
   CreateObject(LADR, 2105, 190, -1)->Set(9);

   //Metallkisten
   CreateObject(MWCR, 2130, 490, -1);
   CreateObject(MWCR, 2150, 490, -1);
   CreateObject(MWCR, 2260, 490, -1);
   CreateObject(MWCR, 2280, 454, -1);
   CreateObject(MWCR, 2280, 472, -1);
   CreateObject(MWCR, 2280, 490, -1);

   //Kisten
   CreateObject(WCR2, 1140, 410, -1);
   CreateObject(WCR2, 1160, 410, -1);
   CreateObject(WCR2, 1710, 180, -1);
   CreateObject(WCR2, 2140, 472, -1);

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Raketen
    PlaceSpawnpoint(MBOX, 1230, 155);
    PlaceSpawnpoint(MBOX, 1830, 385);

    //Gewehrgranaten
    PlaceSpawnpoint(GBOX, 1930, 545);
   }
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {
   //Zielobjekte
   AddAssaultTarget(CCP2, 1330, 430, 30*30, 2, "$Target1$", 0, [[[1450, 200], [1550, 200], [1650, 200]], [[530, 350], [570, 350], [570, 450]]]);
   AddAssaultTarget(PMP2, 1550, 560, 30*30, 2, "$Target2$", 1, [[[1450, 200], [1550, 200], [1650, 200]], [[530, 350], [570, 350], [570, 450]]]);
   AddAssaultTarget(CMSN, 2265, 320, 30*30, 2, "$Target3$", 2, [[[1940, 70], [2410, 110], [2450, 110]], [[1510, 260], [1530, 360],[1530, 430]]]);
   AddAssaultTarget(LBPC, 2080, 493, 30*30, 2, "$Target4$", 3, [[[2150, 210], [2210, 110], [2250, 320]], [[1450, 200], [1400, 180], [1510, 260]]]);
   AddAssaultTarget(CCP2, 2580, 510, 25*30, 2, "$Target5$", 4, [[[2280, 110], [2450, 110]], [[1955, 550], [1870, 522], [1820, 390]]]);
   AddAssaultTarget(LBPC, 2600, 320, 30*30, 2, "$Target4$", 5, [[[2280, 110], [2450, 110]], [[1955, 550], [1870, 522], [1820, 390]]]);
   AddAssaultTarget(CMSN, 2750, 210, 25*30, 2, "$Target6$", 6, [[[2280, 110], [2450, 110]], [[1955, 550], [1870, 522], [1820, 390]]]);
   AddAssaultTarget(GSTA, 2860, 320, 25*30, 2, "$Target7$", 7, [[[2280, 110], [2450, 110]], [[1955, 550], [1870, 522], [1820, 390]]]);
   AddAssaultTarget(CCP1, 2855, 500, 25*30, 2, "$Target1$", 8, [[[2280, 110], [2450, 110]], [[1955, 550], [1870, 522], [1820, 390]]]);

   //Ziele verbinden
   ConnectAssaultTargets([0, 1]);
   ConnectAssaultTargets([4, 5, 6, 7, 8]);

   //Grenze setzen
   CreateObject(BRDR, 360, 0, -1)->Set(0);

   //SSA Besitzer setzen
   if(aTeams[2] == true)
   {aSelfDefense[0]->SetTeam(2); aSelfDefense[1]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();

   //Konsole
   CreateObject(CONS, 1835, 540, -1)->Set(aSelfDefense[1]);

   //Geschützstellungen
   CreateObject(GNET, 605, 350, -1)->Set(0,90,1);
   aStationary[0] = CreateObject(GNET, 1210, 360, -1);
   aStationary[0] -> Set(0,-90);
   aStationary[1] = CreateObject(GNET, 2090, 210, -1);
   aStationary[1] -> Set(0,-90);
   aStationary[2] = CreateObject(GNET, 2130, 490, -1);
   aStationary[2] -> Set(0,-90,1);
   aStationary[3] = CreateObject(GNET, 2815, 450, -1);
   aStationary[3] -> Set(0,-90);

   //Versorgungskiste (APW)
   var crate = CreateObject (AMCT, 1340, 160, -1);
   crate->Set(ATWN);

   //Hinweisschilder
   var sign = CreateObject(SGNP, 1180, 410, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 1810, 550, -1);
   sign->SetMode(1);
   sign->Set("Turret");

   //Objekte entfernen
   RemoveObject(FindObject2(Find_ID(PANE),Find_InRect(1200, 330, 805, 360)));
   RemoveObject(FindObject2(Find_ID(BECR),Find_InRect(2800, 430, 20, 20)));
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   //Grenzen setzen
   CreateObject(BRDR, 360, 0, -1)->Set(0);
   CreateObject(BRDR, 2410, 0, -1)->Set(1);

   //Türen öffnen
   aDoor[00]->Open();
   aDoor[01]->Open();
   aDoor[02]->Open();
   aDoor[03]->Open();
   aDoor[04]->Open();

   aDoor[05]->Open();
   aDoor[06]->Unlock();
   aDoor[07]->Unlock();

   //Objekte entfernen
   RemoveObject(aSelfDefense[0]);
   RemoveObject(aSelfDefense[1]);
   RemoveObject(FindObject2(Find_ID(AMCT),Find_InRect(1480, 240, 40, 40)));

   //Geschützstellungen
   CreateObject(GNET, 605, 350, -1)->Set(0,90,1);
   CreateObject(GNET, 2090, 210, -1)->Set(0,-90,1);

   //Leiter
   CreateObject(LADR, 2050, 457, -1)->Set(20);

   //Metallkisten
   CreateObject(MWCR, 2130, 490, -1);
   CreateObject(MWCR, 2150, 490, -1);
   CreateObject(MWCR, 2260, 490, -1);
   CreateObject(MWCR, 2280, 454, -1);
   CreateObject(MWCR, 2280, 472, -1);
   CreateObject(MWCR, 2280, 490, -1);

   //Kisten
   CreateObject(WCR2, 1140, 410, -1);
   CreateObject(WCR2, 1160, 410, -1);
   CreateObject(WCR2, 1710, 180, -1);
   CreateObject(WCR2, 2140, 472, -1);

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Raketen
    PlaceSpawnpoint(MBOX, 1230, 155);
    PlaceSpawnpoint(MBOX, 1830, 385);

    //Gewehrgranaten
    PlaceSpawnpoint(GBOX, 1930, 545);
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
    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),1700,0,1,1);

    //Rauch
    CreateParticle("GunSmoke",1325,340,0,-10,350,1);
    Smoke(925, 340, 30);

    //Türen öffnen
    aDoor[00]->Open();
    aDoor[01]->Open();
    aDoor[02]->Open();
    aDoor[03]->Open();
    aDoor[04]->Open();
    aDoor[08]->Open();

    //SSA zerstören
    aSelfDefense[0]->Disarm();
    DecoExplode(30, aSelfDefense[0]);

    //Geschützstellung entfernen
    aStationary[0]->DecoExplode(30);

    //Lampen deaktivieren
    aLamp[00]->EMPShock();
    aLamp[01]->EMPShock();
    aLamp[02]->EMPShock();
    aLamp[03]->EMPShock();
   }
  }

  //Ziel 3
  if(iIndex == 2)
  {
   //Grenze neu setzen
   RemoveAll(BRDR);
   CreateObject(BRDR, 1100, 0, -1)->Set(0,1);

   //Teamgrenze setzen
   FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),0,440,3,1);

   //Rauch
   CreateParticle("GunSmoke",2220,170,0,-10,200,1);
   Smoke(2225, 170, 30);
   CreateParticle("GunSmoke",2245,170,0,-10,250,1);
   CreateParticle("GunSmoke",2260,170,0,-10,190,1);
   Smoke(2270, 170, 30);
   CreateParticle("GunSmoke",2280,170,0,-10,210,1);
   CreateParticle("GunSmoke",2285,170,0,-10,270,1);

   CreateParticle("GunSmoke",2225,290,0,-10,260,1);
   Smoke(2230, 290, 30);
   CreateParticle("GunSmoke",2245,290,0,-10,220,1);
   CreateParticle("GunSmoke",2255,290,0,-10,240,1);
   CreateParticle("GunSmoke",2275,290,0,-10,230,1);
   Smoke(2270, 1880, 30);
   CreateParticle("GunSmoke",2300,290,0,-10,210,1);

   //Objekt entfernen
   FindObject(MVNT)->DecoExplode(30);

   //Türen öffnen
   aDoor[05]->Open();
   aDoor[06]->Unlock();
   aDoor[07]->Unlock();

   //Geschützstellung entfernen
   aStationary[1]->DecoExplode(30);

   //Lampen deaktivieren
   aLamp[04]->EMPShock();
   aLamp[05]->EMPShock();
   aLamp[06]->EMPShock();
   aLamp[07]->EMPShock();
   aLamp[08]->EMPShock();
   aLamp[09]->EMPShock();
   aLamp[10]->EMPShock();

   //Risse
   CreateObject(FAUD, 2240, 280, -1);
   CreateObject(FAUD, 2245, 170, -1);
   CreateObject(FAUD, 2250, 225, -1);
   CreateObject(FAUD, 2290, 290, -1);
   CreateObject(FAUD, 2300, 180, -1);
  }

  //Ziel 4
  if(iIndex == 3)
  {
   //Grenze neu setzen
   RemoveAll(BRDR);
   CreateObject(BRDR, 1580, 0, -1)->Set(0,1);

   //Teamgrenze setzen
   FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),2320,0,1,1);

   //Rauch
   CreateParticle("GunSmoke",1830,435,0,-10,350,1);
   Smoke(1830, 435, 30);
   CreateParticle("GunSmoke",2050,435,0,-10,350,1);
   Smoke(2050, 435, 30);
   CreateParticle("GunSmoke",2270,435,0,-10,350,1);
   Smoke(2270, 435, 30);

   //Türen öffnen
   aDoor[09]->Unlock();
   aDoor[10]->Open();
   aDoor[11]->Open();

   //SSA zerstören
   aSelfDefense[1]->Disarm();
   DecoExplode(30, aSelfDefense[1]);

   //Geschützstellung entfernen
   aStationary[2]->DecoExplode(30);

   //Lampen deaktivieren
   aLamp[11]->TurnOff();
   aLamp[12]->TurnOff();
   aLamp[13]->TurnOff();

   //Lampen aktivieren
   aLamp[14]->TurnOn();
   aLamp[15]->TurnOn();

   //Risse
   CreateObject(FAUD, 2150, 470, -1);
   CreateObject(FAUD, 2205, 430, -1);
   CreateObject(FAUD, 2260, 460, -1);
  }

  //Ziel 5
  if(iIndex == 4)
  {
   //Risse
   CreateObject(FAUD, 2540, 490, -1);
   CreateObject(FAUD, 2585, 470, -1);
   CreateObject(FAUD, 2600, 520, -1);
   CreateObject(FAUD, 2620, 495, -1);
   CreateObject(FAUD, 2660, 485, -1);

   //Rauch
   CreateParticle("GunSmoke",2530,510,0,-10,450,1);
   CreateParticle("GunSmoke",2560,510,0,-10,450,1);
   CreateParticle("GunSmoke",2590,510,0,-10,450,1);
   CreateParticle("GunSmoke",2530,520,0,-10,250,1);
   CreateParticle("GunSmoke",2560,520,-10,250,1);
   CreateParticle("GunSmoke",2590,520,0,-10,250,1);
   Smoke(2580, 490, 50);
  }

  //Ziel 6
  if(iIndex == 5)
  {
   //Risse
   CreateObject(FAUD, 2550, 290, -1);
   CreateObject(FAUD, 2600, 310, -1);

   //Rauch
   CreateParticle("GunSmoke",2565,320,0,-10,450,1);
   CreateParticle("GunSmoke",2595,320,0,-10,450,1);
   CreateParticle("GunSmoke",2625,320,0,-10,450,1);

   CreateParticle("GunSmoke",2565,330,0,-10,250,1);
   CreateParticle("GunSmoke",2595,330,0,-10,250,1);
   CreateParticle("GunSmoke",2625,330,0,-10,250,1);
   Smoke(2600, 305, 50);
  }

  //Ziel 7
  if(iIndex == 6)
  {
   //Risse
   CreateObject(FAUD, 2700, 215, -1);
   CreateObject(FAUD, 2805, 270, -1);

   //Rauch
   CreateParticle("GunSmoke",2730,210,0,-10,450,1);
   CreateParticle("GunSmoke",2750,210,0,-10,450,1);
   CreateParticle("GunSmoke",2770,210,0,-10,450,1);

   CreateParticle("GunSmoke",2730,230,0,-10,250,1);
   CreateParticle("GunSmoke",2750,230,0,-10,250,1);
   CreateParticle("GunSmoke",2770,230,0,-10,250,1);
   Smoke(2755, 210, 50);
  }

  //Ziel 8
  if(iIndex == 7)
  {
   //Risse
   CreateObject(FAUD, 2850, 325, -1);
   CreateObject(FAUD, 2870, 410, -1);

   //Rauch
   CreateParticle("GunSmoke",2800,320,0,-10,450,1);
   CreateParticle("GunSmoke",2830,320,0,-10,450,1);
   CreateParticle("GunSmoke",2860,320,0,-10,450,1);

   CreateParticle("GunSmoke",2800,330,0,-10,250,1);
   CreateParticle("GunSmoke",2830,330,0,-10,250,1);
   CreateParticle("GunSmoke",2860,330,0,-10,250,1);
   Smoke(2840, 310, 50);
  }

  //Ziel 9
  if(iIndex == 8)
  {
   //Risse
   CreateObject(FAUD, 2810, 500, -1);
   CreateObject(FAUD, 2860, 510, -1);

   //Rauch
   CreateParticle("GunSmoke",2810,500,0,-10,450,1);
   CreateParticle("GunSmoke",2840,500,0,-10,450,1);
   CreateParticle("GunSmoke",2870,500,0,-10,450,1);

   CreateParticle("GunSmoke",2810,510,0,-10,250,1);
   CreateParticle("GunSmoke",2840,510,0,-10,250,1);
   CreateParticle("GunSmoke",2870,510,0,-10,250,1);
   Smoke(2855, 490, 50);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX =1160; iY = 300;
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
    return [[520, 340], [520, 440]];
   }
   if(iTeam == 2)
   {
    return [[2140, 200], [2140, 280]];
   }
   return(1);
  }
}