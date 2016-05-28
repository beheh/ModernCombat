/*-- Yuki --*/

#strict 2
#include CSTD

static aDoor1,aDoor2,aLamp1,aLamp2,aLamp3,aFlag;

func RecommendedGoals()	{return [GOCC, GBAS];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(1,50,15);
  //Hydrauliktüren A
  aDoor1 = [];
  //Hydrauliktüren B
  aDoor2 = [];
  //Lampen A
  aLamp1 = [];
  //Lampen B
  aLamp2 = [];
  //Lampen C
  aLamp3 = [];
  //Flaggen
  aFlag = [];
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
  CreateObject(LADR, 505, 671, -1)->Set(18);
  CreateObject(LADR, 540, 505, -1)->Set(13);
  CreateObject(LADR, 610, 710, -1)->Set(24);
  CreateObject(LADR, 655, 490, -1)->Set(33);
  CreateObject(LADR, 765, 690, -1)->Set(21);
  CreateObject(LADR, 890, 730, -1)->Set(25);
  CreateObject(LADR, 1290, 750, -1)->Set(30);
  CreateObject(LADR, 1430, 750, -1)->Set(40);
  CreateObject(LADR, 1820, 810, -1)->Set(40);
  CreateObject(LADR, 1890, 730, -1)->Set(45);
  CreateObject(LADR, 1970, 805, -1)->Set(10);
  CreateObject(LADR, 1995, 620, -1)->Set(15);
  CreateObject(LADR, 2105, 620, -1)->Set(17);
  CreateObject(LADR, 2130, 360, -1)->Set(14);
  CreateObject(LADR, 2225, 550, -1)->Set(21);
  CreateObject(LADR, 2390, 560, -1)->Set(9);

  //Bodenluken
  CreateObject(HA4K, 760, 723, -1);
  CreateObject(HA4K, 790, 403, -1);
  CreateObject(HA4K, 1820, 753, -1);
  CreateObject(HA4K, 2390, 483, -1);

  //Große Bodenluken
  CreateObject(H24K, 505, 528, -1);
  CreateObject(H24K, 655, 408, -1);

  //Kiste
  CreateObject(WCR2, 2050, 240, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 1340, 680, -1)->AutoRespawn();
  CreateObject(MWCR, 1860, 830, -1);

  //Verbandskisten
  CreateObject(BECR, 1050, 570, -1)->AutoRespawn();
  CreateObject(BECR, 1840, 810, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 1480, 580, -1)->AutoRespawn();
  CreateObject(XWCR, 2145, 480, -1)->AutoRespawn();

  //Container
  CreateObject(CON1, 1980, 690, -1)->SetGraphics("5");

  //Giftfässer
  CreateObject(TBRL, 1220, 750, -1)->AutoRespawn();
  CreateObject(TBRL, 1460, 750, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 1000, 550, -1)->AutoRespawn();
  CreateObject(PBRL, 1780, 560, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 1035, 760, -1)->AutoRespawn();
  CreateObject(XBRL, 1640, 380, -1);
  CreateObject(XBRL, 1700, 380, -1);
  CreateObject(XBRL, 2185, 380, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 1525, 430, -1)->AutoRespawn();
  CreateObject(HBRL, 1920, 560, -1);

  //Benzintank
  CreateObject(XTNK, 1100, 610, -1);

  //Phosphortank
  CreateObject(PTNK, 1500, 750, -1)->AutoRespawn();

  //Stahlbrücken
  CreateObject(_HBR, 595, 412, -1)->SwitchMode();
  CreateObject(_HBR, 1745, 762, -1)->SwitchMode();

  //Hydrauliktüren A
  aDoor1[0] = CreateObject(SLDR, 705, 520, -1);
  aDoor1[0]->Lock();
  aDoor1[0]->Open();
  aDoor1[0]->SetMaxDamage(-1);
  aDoor1[1] = CreateObject(SLDR, 1155, 350, -1);
  aDoor1[1]->Lock();
  aDoor1[1]->Open();
  aDoor1[1]->SetMaxDamage(-1);
  aDoor1[2] = CreateObject(SLDR, 1155, 750, -1);
  aDoor1[2]->Lock();
  aDoor1[2]->Open();
  aDoor1[2]->SetMaxDamage(-1);
  aDoor1[3] = CreateObject(SLDR, 1605, 380, -1);
  aDoor1[3]->Lock();
  aDoor1[3]->Open();
  aDoor1[3]->SetMaxDamage(-1);
  aDoor1[4] = CreateObject(SLDR, 1605, 750, -1);
  aDoor1[4]->Lock();
  aDoor1[4]->Open();
  aDoor1[4]->SetMaxDamage(-1);
  aDoor1[5] = CreateObject(SLDR, 2035, 500, -1);
  aDoor1[5]->Lock();
  aDoor1[5]->Open();
  aDoor1[5]->SetMaxDamage(-1);

  //Hydrauliktüren B
  aDoor2[0] = CreateObject(SLDR, 705, 400, -1);
  aDoor2[0]->Lock();
  aDoor2[0]->Close();
  aDoor2[0]->SetMaxDamage(-1);
  aDoor2[1] = CreateObject(SLDR, 705, 720, -1);
  aDoor2[1]->Lock();
  aDoor2[1]->Close();
  aDoor2[1]->SetMaxDamage(-1);
  aDoor2[2] = CreateObject(SLDR, 1155, 610, -1);
  aDoor2[2]->Lock();
  aDoor2[2]->Close();
  aDoor2[2]->SetMaxDamage(-1);
  aDoor2[3] = CreateObject(SLDR, 1605, 590, -1);
  aDoor2[3]->Lock();
  aDoor2[3]->Close();
  aDoor2[3]->SetMaxDamage(-1);
  aDoor2[4] = CreateObject(SLDR, 2035, 380, -1);
  aDoor2[4]->Lock();
  aDoor2[4]->Close();
  aDoor2[4]->SetMaxDamage(-1);
  aDoor2[5] = CreateObject(SLDR, 2035, 640, -1);
  aDoor2[5]->Lock();
  aDoor2[5]->Close();
  aDoor2[5]->SetMaxDamage(-1);

  //Lampen A
  aLamp1[0] = CreateObject(ALGH, 734, 599, -1);
  aLamp1[0]->SetR(90);
  aLamp1[0]->TurnOff();
  aLamp1[0]->ChangeLightColor(RGB(0,255,255));
  aLamp1[1] = CreateObject(ALGH, 1190, 679, -1);
  aLamp1[1]->SetR(180);
  aLamp1[1]->TurnOff();
  aLamp1[1]->ChangeLightColor(RGB(0,255,255));
  aLamp1[2] = CreateObject(ALGH, 1650, 669, -1);
  aLamp1[2]->SetR(180);
  aLamp1[2]->TurnOff();
  aLamp1[2]->ChangeLightColor(RGB(0,255,255));
  aLamp1[3] = CreateObject(ALGH, 2140, 499, -1);
  aLamp1[3]->SetR(180);
  aLamp1[3]->TurnOff();
  aLamp1[3]->ChangeLightColor(RGB(0,255,255));

  //Lampen B
  aLamp2[0] = CreateObject(ALGH, 734, 619, -1);
  aLamp2[0]->SetR(90);
  aLamp2[0]->TurnOff();
  aLamp2[0]->ChangeLightColor(RGB(255,255,50));
  aLamp2[1] = CreateObject(ALGH, 1210, 679, -1);
  aLamp2[1]->SetR(180);
  aLamp2[1]->TurnOff();
  aLamp2[1]->ChangeLightColor(RGB(255,255,50));
  aLamp2[2] = CreateObject(ALGH, 1670, 669, -1);
  aLamp2[2]->SetR(180);
  aLamp2[2]->TurnOff();
  aLamp2[2]->ChangeLightColor(RGB(255,255,50));
  aLamp2[3] = CreateObject(ALGH, 2160, 499, -1);
  aLamp2[3]->SetR(180);
  aLamp2[3]->TurnOff();
  aLamp2[3]->ChangeLightColor(RGB(255,255,50));

  //Lampen C
  aLamp3[0] = CreateObject(ALGH, 734, 639, -1);
  aLamp3[0]->SetR(90);
  aLamp3[0]->TurnOff();
  aLamp3[0]->ChangeLightColor(RGB(255,50,50));
  aLamp3[1] = CreateObject(ALGH, 1230, 679, -1);
  aLamp3[1]->SetR(180);
  aLamp3[1]->TurnOff();
  aLamp3[1]->ChangeLightColor(RGB(255,50,50));
  aLamp3[2] = CreateObject(ALGH, 1690, 669, -1);
  aLamp3[2]->SetR(180);
  aLamp3[2]->TurnOff();
  aLamp3[2]->ChangeLightColor(RGB(255,50,50));
  aLamp3[3] = CreateObject(ALGH, 2180, 499, -1);
  aLamp3[3]->SetR(180);
  aLamp3[3]->TurnOff();
  aLamp3[3]->ChangeLightColor(RGB(255,50,50));

  //Grenzen
  CreateObject(BRDR, 460, 0, -1)->Set(0);
  CreateObject(BRDR, 2430, 0, -1)->Set(1);
  CreateObject(BRDR, 0, 1100, -1)->Set(3,0,1);

  //Soundkulisse

  //Möven
  CreateObject(SE4K, 1370, 180, -1)->Set("Seagulls*.ogg",1000,500, 10,25);

  //Vögel
  CreateObject(SE4K, 560, 190, -1)->Set("BirdSong*",200,50);
  CreateObject(SE4K, 905, 255, -1)->Set("BirdSong*",200,50);
  CreateObject(SE4K, 1770, 305, -1)->Set("BirdSong*",200,50);
  CreateObject(SE4K, 2340, 130, -1)->Set("BirdSong*",200,50);

  //Erdrutsche
  CreateObject(SE4K, 800, 870, -1)->Set("FallingDirt*.ogg",900,200, 75);
  CreateObject(SE4K, 1370, 870, -1)->Set("FallingDirt*.ogg",900,200, 75);
  CreateObject(SE4K, 1920, 870, -1)->Set("FallingDirt*.ogg",900,200, 75);

  //Wind
  CreateObject(SE4K, 900, 130, -1)->Set("WindSound*.ogg",800,300, 75);
  CreateObject(SE4K, 1380, 100, -1)->Set("WindSound*.ogg",800,300, 50,75);
  CreateObject(SE4K, 1810, 110, -1)->Set("WindSound*.ogg",800,300, 75);

  //Innenbereich
  CreateObject(SE4K, 540, 400, -1)->Set("InteriorMetal*.ogg",1000,300, 25,50);
  CreateObject(SE4K, 750, 325, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 925, 575, -1)->Set("InteriorStress*.ogg",700,100, 75);
  CreateObject(SE4K, 1360, 525, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 1360, 920, -1)->Set("InteriorMetal*.ogg",1000,300, 25,50);
  CreateObject(SE4K, 1850, 605, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 1940, 335, -1)->Set("InteriorStress*.ogg",700,100, 75);
  CreateObject(SE4K, 2260, 415, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 2460, 400, -1)->Set("InteriorMetal*.ogg",1000,300, 25,50);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 935, 660, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 1975, 660, -1);
  crate->Set(ABOX);

  //Versorgungskiste (Gewehrgranaten)
  crate = CreateObject(AMCT, 1500, 430, -1);
  crate->Set(GBOX);

  //Versorgungskisten (Dragnin)
  crate = CreateObject(AMCT, 1070, 760, -1);
  crate->Set(DGNN);
  crate = CreateObject(AMCT, 1895, 830, -1);
  crate->Set(DGNN);

  //Raketen
  PlaceSpawnpoint(MBOX, 1520, 575);

  //Automat
  var store = CreateObject(WPVM,1360, 680,-1);
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
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Leitkegel
  CreateObject(TCCN, 795, 720, -1)->Light();
  CreateObject(TCCN, 925, 750, -1)->Light();
  CreateObject(TCCN, 1325, 770, -1)->Light();
  CreateObject(TCCN, 1395, 750, -1)->Light();

  //Labortisch
  CreateObject(LTBL, 1980, 380, -1);

  //Monitore
  CreateObject(MNI2, 1970, 368, -1);
  CreateObject(MNI2, 1990, 368, -1)->Off();

  //Bildschirme
  var screen = CreateObject(SCA2, 830, 660, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen->SetClips([12]);
  CreateObject(SCR3, 1665, 725, -1);

  //Orientierungslichter
  CreateObject(OLGH, 705, 170, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 1155, 180, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 1605, 180, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 2035, 180, -1)->Set(2, 15, 1, 1, 40);

  //Anlagen
  CreateObject(CCP2, 810, 540, -1);
  CreateObject(CGLO, 1655, 590, -1);
  CreateObject(CCP1, 2075, 640, -1);

  //Risse
  CreateObject(FAUD, 1060, 345, -1);
  CreateObject(FAUD, 1085, 340, -1);
  CreateObject(FAUD, 1110, 320, -1);
  CreateObject(FAUD, 1505, 580, -1);
  CreateObject(FAUD, 1980, 640, -1);

  //Automat
  CreateObject(VGMN, 1940, 830, -1)->Set(3);

  //Radare
  CreateObject(RADR, 700, 170, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(RADR, 2035, 180, -1)->SetClrModulation(RGB(125,125,125));

  //Nebel
  CreateParticle("Fog", 590, 500,0,0,400+Random(100));
  CreateParticle("Fog", 595, 530,0,0,900+Random(100));
  CreateParticle("Fog", 605, 495,0,0,400+Random(100));
  CreateParticle("Fog", 655, 500,0,0,200+Random(100));

  CreateParticle("Fog", 855, 270,0,0,900+Random(100));
  CreateParticle("Fog", 895, 280,0,0,400+Random(100));
  CreateParticle("Fog", 895, 280,0,0,900+Random(100));
  CreateParticle("Fog", 910, 250,0,0,450+Random(100));
  CreateParticle("Fog", 950, 250,0,0,500+Random(100));

  CreateParticle("Fog", 1310, 200,0,0,850+Random(100));
  CreateParticle("Fog", 1350, 160,0,0,900+Random(100));
  CreateParticle("Fog", 1355, 180,0,0,400+Random(100));
  CreateParticle("Fog", 1360, 220,0,0,500+Random(100));
  CreateParticle("Fog", 1360, 255,0,0,250+Random(100));
  CreateParticle("Fog", 1385, 255,0,0,300+Random(100));
  CreateParticle("Fog", 1370, 270,0,0,450+Random(100));
  CreateParticle("Fog", 1410, 205,0,0,950+Random(100));

  CreateParticle("Fog", 2105, 595,0,0,900+Random(100));
  CreateParticle("Fog", 2110, 630,0,0,400+Random(100));
  CreateParticle("Fog", 2130, 600,0,0,500+Random(100));
  CreateParticle("Fog", 2140, 620,0,0,300+Random(100));
  CreateParticle("Fog", 2140, 640,0,0,950+Random(100));
  CreateParticle("Fog", 2185, 595,0,0,500+Random(100));

  //Zäune
  CreateObject(FENC, 820, 540, -1)->Set(2);
  CreateObject(FENC, 1100, 610, -1);
  CreateObject(FENC, 1490, 750, -1);
  CreateObject(FENC, 1550, 750, -1)->Set(2);
  CreateObject(FENC, 1660, 750, -1);
  CreateObject(FENC, 1980, 380, -1);
  CreateObject(FENC, 2090, 380, -1)->Set(2);
  CreateObject(FENC, 2150, 380, -1);

  //Regale
  CreateObject(FRAM, 1060, 750, -1);
  CreateObject(FRAM, 1080, 750, -1);
  CreateObject(FRAM, 1100, 750, -1);
  CreateObject(FRAM, 1410, 625, -1);
  CreateObject(FRAM, 1410, 640, -1);
  CreateObject(FRAM, 1410, 655, -1);
  CreateObject(FRAM, 1410, 670, -1);
  CreateObject(FRAM, 1450, 625, -1);
  CreateObject(FRAM, 1450, 640, -1);
  CreateObject(FRAM, 1450, 655, -1);
  CreateObject(FRAM, 1450, 670, -1);
  CreateObject(FRAM, 1500, 580, -1);
  CreateObject(FRAM, 1520, 580, -1);
  CreateObject(FRAM, 1870, 800, -1);
  CreateObject(FRAM, 1890, 800, -1);
  CreateObject(FRAM, 1910, 800, -1);
  CreateObject(FRAM, 2070, 380, -1);
  CreateObject(FRAM, 2090, 380, -1);

  //Feuerlöscher
  CreateObject(FIE2, 685, 515, -1);
  CreateObject(FIE2, 1120, 755, -1);
  CreateObject(FIE2, 1635, 745, -1);
  CreateObject(FIE2, 2195, 475, -1);

  //Apparaturen
  CreateObject(GADG, 1695, 750, -1)->Set(1);
  CreateObject(GADG, 2000, 380, -1)->Set(1);

  //Schilder
  CreateObject(MSGN, 745, 720, -1);
  CreateObject(MSGN, 775, 720, -1);
  CreateObject(WLSN, 1050, 520, -1);
  CreateObject(MSGN, 1805, 750, -1);
  CreateObject(MSGN, 1835, 750, -1);

  //Scheinwerfer
  CreateObject(FLGH, 740, 720, -1)->SetRotation(30);
  CreateObject(FLGH, 1025, 550, -1)->SetRotation(-40);
  CreateObject(FLGH, 1915, 560, -1)->SetRotation(-15);
  CreateObject(FLGH, 2370, 480, -1)->SetRotation(-55);

  //Glühbirnen
  CreateObject(LAT1, 150, 680, -1);
  CreateObject(LAT1, 255, 620, -1);
  CreateObject(LAT1, 315, 425, -1);
  CreateObject(LAT1, 450, 190, -1);

  CreateObject(LAT1, 645, 585, -1);
  CreateObject(LAT1, 770, 440, -1);
  CreateObject(LAT1, 805, 550, -1);

  CreateObject(LAT1, 1285, 470, -1);
  CreateObject(LAT1, 1335, 450, -1);
  CreateObject(LAT1, 1475, 340, -1);

  CreateObject(LAT1, 1770, 595, -1);
  CreateObject(LAT1, 1785, 405, -1);
  CreateObject(LAT1, 1820, 410, -1);
  CreateObject(LAT1, 1925, 570, -1);

  CreateObject(LAT1, 2185, 410, -1);
  CreateObject(LAT1, 2250, 310, -1);

  CreateObject(LAT1, 2455, 185, -1);
  CreateObject(LAT1, 2585, 345, -1);
  CreateObject(LAT1, 2605, 525, -1);

  //Spinde
  CreateObject(LCKR, 520, 690, -1);
  CreateObject(LCKR, 538, 690, -1)->SetR(-30);
  CreateObject(LCKR, 1800, 810, -1);

  //Geländer
  CreateObject(RAI1, 519,520, -1)->SetRail([1,1]);
  CreateObject(RAI1, 730, 720, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 767,400, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1330, 680, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1720, 560, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 2130, 480, -1)->SetRail([1,1,1]);

  //Ventillatoren
  CreateObject(MVNT, 705, 220, -1)->SetCon(50);
  CreateObject(MVNT, 1155, 230, -1)->SetCon(50);
  CreateObject(MVNT, 1605, 230, -1)->SetCon(50);
  CreateObject(MVNT, 2035, 230, -1)->SetCon(50);

  //Glasröhren
  CreateObject(GLST, 745, 401, -1);
  CreateObject(GLST, 765, 401, -1);
  CreateObject(GLST, 810, 401, -1);

  CreateObject(GLST, 1195, 750, -1);
  CreateObject(GLST, 1230, 750, -1);

  CreateObject(GLST, 1645, 750, -1);
  CreateObject(GLST, 1680, 750, -1);

  CreateObject(GLST, 1950, 380, -1);

  //Computer
  CreateObject(LBPC, 1490, 430, -1);

  //Büsche
  CreateObject(BSH2, 275, 490, -1);
  CreateObject(BSH2, 590, 355, -1);
  CreateObject(BSH2, 935, 600, -1);
  CreateObject(BSH2, 1195, 800, -1);
  CreateObject(BSH2, 1740, 620, -1);
  CreateObject(BSH2, 1975, 455, -1);
  CreateObject(BSH2, 2070, 510, -1);
  CreateObject(BSH2, 2420, 465, -1);
  CreateObject(BSH2, 2725, 730, -1);

  //Notausgangslichter
  CreateObject(ETLT, 820, 500, -1);
  CreateObject(ETLT, 1550, 705, -1);
  CreateObject(ETLT, 1865, 495, -1);

  //Gasflaschen
  CreateObject(GSBL, 1085, 610, -1)->AutoRespawn();
  CreateObject(GSBL, 1500, 580, -1)->AutoRespawn();
  CreateObject(GSBL, 2000, 660, -1);

  //Steine
  CreateObject(STNE, 135, 670, -1)->Set(2);
  CreateObject(STNE, 420, 520, -1)->Set(4);
  CreateObject(STNE, 950, 310, -1)->Set(3);
  CreateObject(STNE, 1010, 800, -1)->Set(0);
  CreateObject(STNE, 1720, 840, -1)->Set(5);
  CreateObject(STNE, 2300, 490, -1)->Set(2);
  CreateObject(STNE, 2570, 660, -1)->Set(1);
}

func CreateOptionalFeatures()
{
  //Hintergrund
  CreateObject(BD01,900,1000,-1)->SetClrModulation(RGB(125,125,125));
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Script starten
  ScriptGo(1);

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Infiltration.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OCC-Spielziel
  if(FindObject(GOCC))
  {
    //Flaggenposten
    aFlag[0] = CreateObject(OFPL,640,520,NO_OWNER);
    aFlag[0] -> AddSpawnPoint(530,680);
    aFlag[0] -> AddSpawnPoint(670,640);
    aFlag[0] -> AddSpawnPoint(670,710);
    aFlag[0] -> AddSpawnPoint(760,390);

    if(aTeams[1] == true)
    {
      aFlag[0]->Set("$Flag1$",0,4);
      aFlag[0]->Capture(1,1);
    }
     else
    {
      aFlag[0]->Set("$Flag1$");
    }

    aFlag[1] = CreateObject(OFPL,1110,350,NO_OWNER);
    aFlag[1] -> AddSpawnPoint(915,280);
    aFlag[1] -> AddSpawnPoint(1030,540);
    aFlag[1] -> AddSpawnPoint(1255,360);
    aFlag[1]->Set("$Flag2$");

    aFlag[2] = CreateObject(OFPL,1540,750,NO_OWNER);
    aFlag[2] -> AddSpawnPoint(1300,760);
    aFlag[2] -> AddSpawnPoint(1495,650);
    aFlag[2] -> AddSpawnPoint(1700,740);
    aFlag[2]->Set("$Flag3$");

    aFlag[3] = CreateObject(OFPL,2025,240,NO_OWNER);
    aFlag[3] -> AddSpawnPoint(1905,250);
    aFlag[3] -> AddSpawnPoint(1950,370);
    aFlag[3] -> AddSpawnPoint(1990,370);
    aFlag[3]->Set("$Flag4$");

    aFlag[4] = CreateObject(OFPL,2350,480,NO_OWNER);
    aFlag[4] -> AddSpawnPoint(2080,550);
    aFlag[4] -> AddSpawnPoint(2175,600);
    aFlag[4] -> AddSpawnPoint(2290,570);
    aFlag[4] -> AddSpawnPoint(2370,570);
    if(aTeams[2] == true)
    {
      aFlag[4]->Set("$Flag5$",0,4);
      aFlag[4]->Capture(2,1);
    }
    else
    {
      aFlag[4]->Set("$Flag5$");
    }
  }

  //BAS-Spielziel
  if(FindObject(GBAS))
  {
    //Zielobjekte
    AddAssaultTarget(RADR, 650, 520, 350, 1, "$Target1$", 0, [[670, 650], [710, 230], [830, 270]]);
    AddAssaultTarget(CMSN, 845, 720, 350, 1, "$Target2$", 1, [[820, 540], [855, 400], [1025, 550]]);
    AddAssaultTarget(CMSN, 1870, 690, 350, 2, "$Target2$", 2, [[1755, 340], [1920, 830], [1935, 380]]);
    AddAssaultTarget(RADR, 2340, 480, 350, 2, "$Target1$", 3, [[1970, 250], [2315, 580], [2345, 250]]);

    //Sprengsatz-Spawn
    SetupBombSpawnpoint([[1340, 400],[1360,680],[1410,750]]);
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Flaggenposten
    var flag = CreateObject(OFPL, 1360,680, -1);
    flag->~Set("$Flag3$");

    //Teamgrenzen
    CreateObject(BRDR, 1120, 0, -1)->Set(0,1,0,1,1);
    CreateObject(BRDR, 1640, 0, -1)->Set(1,1,0,1,2);

    //Waffenautomat entfernen
    RemoveAll(WPVM);

    //Zusätzliche Munition
    if(!FindObject(NOAM))
    {
      //Versorgungskiste (Kugeln)
      var crate = CreateObject(AMCT, 1510, 660, -1);
      crate->Set(ABOX);
    }
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
    //Flaggen
    if(aTeams[1] == true)
    {CreateFlag(1,835,540,GetTeamColor(1));}
    if(aTeams[2] == true)
    {CreateFlag(2,1940,560,GetTeamColor(2));}

    //Boden
    DrawMaterialQuad("Wall-Stripes", 790,540, 850,540, 850,551, 790,551);
    DrawMaterialQuad("Wall-Stripes", 1900,560, 1960,560, 1960,571, 1900,571);

    //Objekte entfernen
    RemoveObject(FindObject2(Find_ID(HBRL),Find_InRect(1910, 540, 20, 30)));
    RemoveObject(FindObject2(Find_ID(FENC),Find_InRect(790, 490, 60, 50)));
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 1360; iY = 670;
    return 1;
  }

  //HTF/LMS/DM-Spielziel
  if(FindObject(GHTF) || FindObject(GLMS) || FindObject(GTDM))
  {
    if(iTeam == 1)
      return [[705, 220], [645, 510], [670, 640]];
    if(iTeam == 2)
      return [[2080, 230], [2340, 470], [2170, 600]];
    return 1;
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
    if(iTeam == 1)
      return [[670, 640], [705, 220], [770, 250]];
    else
      return [[2025, 230], [2175, 600], [2275, 470]];
    return 1;
  }
}

/* Türensteuerung */

protected func Script220()
{
  aLamp1[0]->TurnOn();
  aLamp1[1]->TurnOn();
  aLamp1[2]->TurnOn();
  aLamp1[3]->TurnOn();
}

protected func Script230()
{
  aLamp1[0]->TurnOff();
  aLamp1[1]->TurnOff();
  aLamp1[2]->TurnOff();
  aLamp1[3]->TurnOff();

  aLamp2[0]->TurnOn();
  aLamp2[1]->TurnOn();
  aLamp2[2]->TurnOn();
  aLamp2[3]->TurnOn();

  Sound("Announce*.ogg",1);
}

protected func Script240()
{
  aLamp2[0]->TurnOff();
  aLamp2[1]->TurnOff();
  aLamp2[2]->TurnOff();
  aLamp2[3]->TurnOff();

  aLamp3[0]->TurnOn();
  aLamp3[1]->TurnOn();
  aLamp3[2]->TurnOn();
  aLamp3[3]->TurnOn();
}

protected func Script250()
{
  aLamp3[0]->TurnOff();
  aLamp3[1]->TurnOff();
  aLamp3[2]->TurnOff();
  aLamp3[3]->TurnOff();

  EventInfo4K(0,Format("$MsgDoor$"),STDR, 0, 0, 0, "Info_Event.ogg");

  aDoor2[0]->Open();
  aDoor2[0]->AddLightFlash(100,0,0,RGB(0,255,255));
  aDoor2[1]->Open();
  aDoor2[1]->AddLightFlash(100,0,0,RGB(0,255,255));
  aDoor2[2]->Open();
  aDoor2[2]->AddLightFlash(100,0,0,RGB(0,255,255));
  aDoor2[3]->Open();
  aDoor2[3]->AddLightFlash(100,0,0,RGB(0,255,255));
  aDoor2[4]->Open();
  aDoor2[4]->AddLightFlash(100,0,0,RGB(0,255,255));
  aDoor2[5]->Open();
  aDoor2[5]->AddLightFlash(100,0,0,RGB(0,255,255));

  aDoor1[0]->Close();
  aDoor1[0]->AddLightFlash(100,0,0,RGB(255,50,50));
  aDoor1[1]->Close();
  aDoor1[1]->AddLightFlash(100,0,0,RGB(255,50,50));
  aDoor1[2]->Close();
  aDoor1[2]->AddLightFlash(100,0,0,RGB(255,50,50));
  aDoor1[3]->Close();
  aDoor1[3]->AddLightFlash(100,0,0,RGB(255,50,50));
  aDoor1[4]->Close();
  aDoor1[4]->AddLightFlash(100,0,0,RGB(255,50,50));
  aDoor1[5]->Close();
  aDoor1[5]->AddLightFlash(100,0,0,RGB(255,50,50));

  return(1);
}

protected func Script470()
{
  aLamp1[0]->TurnOn();
  aLamp1[1]->TurnOn();
  aLamp1[2]->TurnOn();
  aLamp1[3]->TurnOn();
}

protected func Script480()
{
  aLamp1[0]->TurnOff();
  aLamp1[1]->TurnOff();
  aLamp1[2]->TurnOff();
  aLamp1[3]->TurnOff();

  aLamp2[0]->TurnOn();
  aLamp2[1]->TurnOn();
  aLamp2[2]->TurnOn();
  aLamp2[3]->TurnOn();

  Sound("Announce*.ogg",1);
}

protected func Script490()
{
  aLamp2[0]->TurnOff();
  aLamp2[1]->TurnOff();
  aLamp2[2]->TurnOff();
  aLamp2[3]->TurnOff();

  aLamp3[0]->TurnOn();
  aLamp3[1]->TurnOn();
  aLamp3[2]->TurnOn();
  aLamp3[3]->TurnOn();
}

protected func Script500()
{
  aLamp3[0]->TurnOff();
  aLamp3[1]->TurnOff();
  aLamp3[2]->TurnOff();
  aLamp3[3]->TurnOff();

  EventInfo4K(0,Format("$MsgDoor$"),STDR, 0, 0, 0, "Info_Event.ogg");

  aDoor1[0]->Open();
  aDoor1[0]->AddLightFlash(100,0,0,RGB(0,255,255));
  aDoor1[1]->Open();
  aDoor1[1]->AddLightFlash(100,0,0,RGB(0,255,255));
  aDoor1[2]->Open();
  aDoor1[2]->AddLightFlash(100,0,0,RGB(0,255,255));
  aDoor1[3]->Open();
  aDoor1[3]->AddLightFlash(100,0,0,RGB(0,255,255));
  aDoor1[4]->Open();
  aDoor1[4]->AddLightFlash(100,0,0,RGB(0,255,255));
  aDoor1[5]->Open();
  aDoor1[5]->AddLightFlash(100,0,0,RGB(0,255,255));

  aDoor2[0]->Close();
  aDoor2[0]->AddLightFlash(100,0,0,RGB(255,50,50));
  aDoor2[1]->Close();
  aDoor2[1]->AddLightFlash(100,0,0,RGB(255,50,50));
  aDoor2[2]->Close();
  aDoor2[2]->AddLightFlash(100,0,0,RGB(255,50,50));
  aDoor2[3]->Close();
  aDoor2[3]->AddLightFlash(100,0,0,RGB(255,50,50));
  aDoor2[4]->Close();
  aDoor2[4]->AddLightFlash(100,0,0,RGB(255,50,50));
  aDoor2[5]->Close();
  aDoor2[5]->AddLightFlash(100,0,0,RGB(255,50,50));

  goto(0);
}