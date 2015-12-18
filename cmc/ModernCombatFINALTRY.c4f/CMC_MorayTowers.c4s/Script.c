/*-- Moray Towers --*/

#strict 2
#include CSTD

static aFlag,aDoorWay,aSunFlare;

func RecommendedGoals()	{return [GOCC, GBAS];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Flaggen
  aFlag = [];
  //Türverbindungen
  aDoorWay = [];
  //Sonneneffekte
  aSunFlare = [];
  //Lichter färben
  Schedule("ChangeLightColors();", 3);
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

  //Rampen
  DrawMaterialQuad("Wall-Concrete2",990,690,1050,630,1050,640,990,700,true);
  DrawMaterialQuad("Wall-Concrete2",1050,550,1080,540,1080,550,1050,550,true);
  DrawMaterialQuad("Wall-Concrete2",1180,540,1220,550,1220,550,1180,550,true);
  DrawMaterialQuad("Wall-Concrete2",1220,1050,1310,1130,1310,1140,1220,1140,true);
  DrawMaterialQuad("Wall-Concrete2",1280,630,1330,680,1320,680,1280,640,true);
  DrawMaterialQuad("Wall-Concrete2",1320,1020,1360,1000,1360,1020,1320,1020,true);
  DrawMaterialQuad("Wall-Concrete2",1330,920,1380,970,1380,980,1330,930,true);
  DrawMaterialQuad("Wall-Concrete2",1370,720,1410,760,1400,760,1370,730,true);
  DrawMaterialQuad("Wall-Metal2",1510,1190,1550,1180,1550,1190,1510,1190,true);
  DrawMaterialQuad("Wall-Concrete2",1530,720,1550,700,1550,710,1530,730,true);
  DrawMaterialQuad("Wall-Concrete2",1410,1000,1440,1030,1430,1030,1410,1010,true);
  DrawMaterialQuad("Wall-Concrete2",1410,920,1440,890,1440,900,1410,930,true);
  DrawMaterialQuad("Wall-Concrete2",1470,860,1480,850,1480,860,1470,870,true);
  DrawMaterialQuad("Wall-Concrete2",2060,1150,2110,1170,2110,1170,2060,1170,true);
  DrawMaterialQuad("Wall-Concrete2",2170,1170,2190,1150,2190,1170,2170,1170,true);
  DrawMaterialQuad("Wall-Concrete2",2240,1150,2260,1170,2260,1170,2240,1170,true);
  DrawMaterialQuad("Wall-Concrete2",2320,1170,2370,1150,2370,1150,2370,1170,true);
  DrawMaterialQuad("Wall-Concrete2",2880,700,2900,720,2890,720,2880,710,true);
  DrawMaterialQuad("Wall-Metal2",2880,1180,2920,1190,2920,1190,2880,1190,true);
  DrawMaterialQuad("Wall-Concrete2",2950,850,2960,860,2950,860,2950,860,true);
  DrawMaterialQuad("Wall-Concrete2",2990,890,3020,920,3010,920,2990,900,true);
  DrawMaterialQuad("Wall-Concrete2",2990,1030,3020,1000,3030,1010,3000,1030,true);
  DrawMaterialQuad("Wall-Concrete2",3020,760,3060,720,3060,730,3030,760,true);
  DrawMaterialQuad("Wall-Concrete2",3050,970,3100,920,3100,930,3060,970,true);
  DrawMaterialQuad("Wall-Concrete2",3070,1000,3110,1020,3070,1020,3070,1020,true);
  DrawMaterialQuad("Wall-Concrete2",3100,680,3150,630,3150,640,3110,680,true);
  DrawMaterialQuad("Wall-Concrete2",3120,1130,3210,1050,3210,1140,3120,1140,true);
  DrawMaterialQuad("Wall-Concrete2",3210,550,3250,540,3250,550,3210,550,true);
  DrawMaterialQuad("Wall-Concrete2",3350,540,3380,550,3350,550,3350,540,true);
  DrawMaterialQuad("Wall-Concrete2",3380,630,3440,690,3430,690,3380,640,true);

  //Leitern
  CreateObject(LADR, 980, 880, -1)->Set(24);
  CreateObject(LADR, 1010, 660, -1)->Set(14);
  CreateObject(LADR, 1110, 530, -1)->Set(4);
  CreateObject(LADR, 1110, 933, -1)->Set(18);
  CreateObject(LADR, 1150, 620, -1)->Set(9);
  CreateObject(LADR, 1210, 1050, -1)->Set(17);
  CreateObject(LADR, 1235, 910, -1)->Set(34);
  CreateObject(LADR, 1305, 645, -1)->Set(11);
  CreateObject(LADR, 1345, 919, -1)->Set(24);
  CreateObject(LADR, 1390, 1128, -1)->Set(15);
  CreateObject(LADR, 1605, 847, -1)->Set(10);
  CreateObject(LADR, 1605, 1125, -1)->Set(11);
  CreateObject(LADR, 1670, 930, -1)->Set(10);

  CreateObject(LADR, 2095, 1110, -1)->Set(3);
  CreateObject(LADR, 2155, 1045, -1)->Set(6);
  CreateObject(LADR, 2180, 1150, -1)->Set(8);
  CreateObject(LADR, 2250, 1150, -1)->Set(8);
  CreateObject(LADR, 2275, 1045, -1)->Set(6);
  CreateObject(LADR, 2335, 1110, -1)->Set(3);

  CreateObject(LADR, 2760, 930, -1)->Set(10);
  CreateObject(LADR, 2825, 847, -1)->Set(10);
  CreateObject(LADR, 2825, 1125, -1)->Set(11);
  CreateObject(LADR, 3040, 1128, -1)->Set(15);
  CreateObject(LADR, 3085, 919, -1)->Set(24);
  CreateObject(LADR, 3125, 645, -1)->Set(11);
  CreateObject(LADR, 3195, 910, -1)->Set(34);
  CreateObject(LADR, 3220, 1050, -1)->Set(17);
  CreateObject(LADR, 3280, 620, -1)->Set(9);
  CreateObject(LADR, 3320, 530, -1)->Set(4);
  CreateObject(LADR, 3320, 933, -1)->Set(18);
  CreateObject(LADR, 3420, 660, -1)->Set(14);
  CreateObject(LADR, 3450, 880, -1)->Set(24);

  //Bodenluken
  CreateObject(HA4K, 1110, 493, -1);
  CreateObject(HA4K, 1150, 543, -1);
  CreateObject(HA4K, 1390, 1003, -1);
  CreateObject(HA4K, 3040, 1003, -1);
  CreateObject(HA4K, 3280, 543, -1);
  CreateObject(HA4K, 3320, 493, -1);

  //Große Bodenluken
  CreateObject(H24K, 1235, 638, -1);
  CreateObject(H24K, 1305, 558, -1);
  CreateObject(H24K, 1345, 728, -1);
  CreateObject(H24K, 1605, 768, -1);
  CreateObject(H24K, 1605, 1038, -1);

  CreateObject(H24K, 2155, 998, -1);
  CreateObject(H24K, 2275, 998, -1);

  CreateObject(H24K, 2825, 1038, -1);
  CreateObject(H24K, 2825, 768, -1);
  CreateObject(H24K, 3085, 728, -1);
  CreateObject(H24K, 3125, 558, -1);
  CreateObject(H24K, 3195, 638, -1);

  //Gerüste
  var scaf = CreateObject(SFFG, 1440, 1310, -1);
  scaf->Set(5);
  scaf->SetCategory(C4D_Background);
  CreateObject(SFFG, 1480, 1130, -1)->Set(5);
  CreateObject(SFFG, 1565, 980, -1)->Set(4);
  CreateObject(SFFG, 1565, 1030, -1)->Set(3);
  CreateObject(SFFG, 1620, 710, -1)->Set(4);
  CreateObject(SFFG, 1620, 760, -1)->Set(5);
  CreateObject(SFFG, 1645, 980, -1)->Set(2);
  CreateObject(SFFG, 1645, 1030, -1)->Set(4);

  CreateObject(SFFG, 1740, 1080, -1)->Set(4);
  CreateObject(SFFG, 1740, 1130, -1)->Set(2);
  CreateObject(SFFG, 1740, 1180, -1)->Set(4);
  CreateObject(SFFG, 1820, 1080, -1)->Set(3);
  CreateObject(SFFG, 1820, 1130, -1)->Set(4);
  CreateObject(SFFG, 1820, 1180, -1)->Set(2);

  CreateObject(SFFG, 1960, 980, -1)->Set(5);
  CreateObject(SFFG, 1960, 1030, -1)->Set(5);
  CreateObject(SFFG, 1960, 1080, -1)->Set(5);
  CreateObject(SFFG, 1960, 1130, -1)->Set(4);
  CreateObject(SFFG, 1960, 1180, -1)->Set(5);

  scaf = CreateObject(SFFG, 2150, 1310, -1);
  scaf->Set(5);
  scaf->SetCategory(C4D_Background);
  scaf = CreateObject(SFFG, 2335, 1530, -1);
  scaf->Set(5);
  scaf->SetCategory(C4D_Background);

  CreateObject(SFFG, 2470, 980, -1)->Set(5);
  CreateObject(SFFG, 2470, 1030, -1)->Set(5);
  CreateObject(SFFG, 2470, 1080, -1)->Set(5);
  CreateObject(SFFG, 2470, 1130, -1)->Set(4);
  CreateObject(SFFG, 2470, 1180, -1)->Set(5);

  CreateObject(SFFG, 2610, 1080, -1)->Set(2);
  CreateObject(SFFG, 2610, 1130, -1)->Set(4);
  CreateObject(SFFG, 2610, 1180, -1)->Set(3);

  CreateObject(SFFG, 2690, 1080, -1)->Set(4);
  CreateObject(SFFG, 2690, 1130, -1)->Set(3);
  CreateObject(SFFG, 2690, 1180, -1)->Set(4);

  CreateObject(SFFG, 2785, 980, -1)->Set(3);
  CreateObject(SFFG, 2785, 1030, -1)->Set(4);
  CreateObject(SFFG, 2810, 710, -1)->Set(4);
  CreateObject(SFFG, 2810, 760, -1)->Set(5);
  CreateObject(SFFG, 2865, 980, -1)->Set(4);
  CreateObject(SFFG, 2865, 1030, -1)->Set(2);
  CreateObject(SFFG, 2950, 1130, -1)->Set(5);
  scaf = CreateObject(SFFG, 2990, 1310, -1);
  scaf->Set(5);
  scaf->SetCategory(C4D_Background);

  //Stahlbrücken
  CreateObject(_HBR, 1105, 642, -1)->SwitchMode();
  CreateObject(_HBR, 1285, 932, -1)->SwitchMode();
  CreateObject(_HBR, 1345, 1142, -1)->SwitchMode();
  CreateObject(_HBR, 1445, 772, -1)->SwitchMode();
  CreateObject(_HBR, 1475, 1042, -1)->SwitchMode();
  CreateObject(_HBR, 1475, 1202, -1)->SwitchMode();
  CreateObject(_HBR, 1535, 862, -1)->SwitchMode();
  var bridge = CreateObject(_HBR, 1545, 1142, -1);
  bridge->SwitchMode();
  bridge->DoDmg(151);
  CreateObject(_HBR, 1925, 1192, -1)->SwitchMode();
  CreateObject(_HBR, 2505, 1192, -1)->SwitchMode();
  bridge = CreateObject(_HBR, 2885, 1142, -1);
  bridge->SwitchMode();
  bridge->DoDmg(151);
  CreateObject(_HBR, 2895, 862, -1)->SwitchMode();
  CreateObject(_HBR, 2955, 1042, -1)->SwitchMode();
  CreateObject(_HBR, 2955, 1202, -1)->SwitchMode();
  CreateObject(_HBR, 2985, 772, -1)->SwitchMode();
  CreateObject(_HBR, 3085, 1142, -1)->SwitchMode();
  CreateObject(_HBR, 3145, 932, -1)->SwitchMode();
  CreateObject(_HBR, 3325, 642, -1)->SwitchMode();

  //Explosivtanks
  CreateObject(XTNK, 1620, 760, -1)->AutoRespawn();
  CreateObject(XTNK, 1820, 1080, -1)->AutoRespawn();
  CreateObject(XTNK, 2140, 1080, -1)->AutoRespawn();
  CreateObject(XTNK, 2610, 1080, -1)->AutoRespawn();
  CreateObject(XTNK, 2290, 1080, -1)->AutoRespawn();
  CreateObject(XTNK, 2810, 760, -1)->AutoRespawn();

  //Kisten
  CreateObject(WCR2, 1850, 1130, -1)->AutoRespawn();
  CreateObject(WCR2, 2580, 1130, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 1340, 1190, -1)->AutoRespawn();
  CreateObject(MWCR, 1960, 1080, -1)->AutoRespawn();
  CreateObject(MWCR, 2470, 1080, -1)->AutoRespawn();
  CreateObject(MWCR, 3090, 1190, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 1170, 490, -1)->AutoRespawn();
  CreateObject(BECR, 1535, 930, -1)->AutoRespawn();
  CreateObject(BECR, 1640, 1180, -1)->AutoRespawn();
  CreateObject(BECR, 2790, 1180, -1)->AutoRespawn();
  CreateObject(BECR, 2895, 930, -1)->AutoRespawn();
  CreateObject(BECR, 3260, 490, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 1720, 1030, -1)->AutoRespawn();
  CreateObject(XWCR, 2710, 1030, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 1255, 630, -1)->AutoRespawn();
  CreateObject(HBRL, 1320, 920, -1)->AutoRespawn();
  CreateObject(HBRL, 1710, 1130, -1)->AutoRespawn();
  CreateObject(HBRL, 2720, 1130, -1)->AutoRespawn();
  CreateObject(HBRL, 3110, 920, -1)->AutoRespawn();
  CreateObject(HBRL, 3175, 630, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1620, 980, -1)->AutoRespawn();
  CreateObject(GSBL, 2810, 980, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 2140, 1170, -1)->AutoRespawn();
  CreateObject(PTNK, 2290, 1170, -1)->AutoRespawn();

  //Sandsackbarrieren
  CreateObject(SBBA, 1650, 760, -1)->Right();
  CreateObject(SBBA, 1650, 850, -1)->Right();
  CreateObject(SBBA, 1770, 1180, -1)->Right();
  CreateObject(SBBA, 1850, 1080, -1)->Right();

  CreateObject(SBBA, 2581, 1080, -1);
  CreateObject(SBBA, 2660, 1180, -1);
  CreateObject(SBBA, 2781, 760, -1);
  CreateObject(SBBA, 2781, 850, -1);

  //Schutztüren
  CreateObject(GDDR, 1085, 540, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1175, 540, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1325, 720, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1555, 760, -1)->SetColorDw(HSL(145, 210, 100, 127));

  CreateObject(GDDR, 2875, 760, -1);
  CreateObject(GDDR, 3105, 720, -1);
  CreateObject(GDDR, 3255, 540, -1);
  CreateObject(GDDR, 3345, 540, -1);

  //Glasscheiben
  CreateObject(PANE, 1053, 702, -1);
  CreateObject(PANE, 1053, 731, -1);
  CreateObject(PANE, 1053, 760, -1);

  CreateObject(PANE, 3377, 702, -1);
  CreateObject(PANE, 3377, 731, -1);
  CreateObject(PANE, 3377, 760, -1);

  //Gitter
  CreateObject(GTNG, 1377, 1000, -1);
  CreateObject(GTNG, 1497, 890, -1);
  CreateObject(GTNG, 2933, 890, -1);
  CreateObject(GTNG, 3053, 1000, -1);

  //Container
  CreateObject(CON1, 2020, 1181, -1)->SetPerspective(3);
  CreateObject(CON1, 2410, 1181, -1)->SetPerspective(3);

  //Sonne umplatzieren
  SetSunPosition(2215,850);

  //Sonneneffekte
  aSunFlare[0] = CreateObject(TIM1, 2000, 930, -1)->AddLight(300,RGBa(250,250,50,50),this());
  aSunFlare[1] = CreateObject(TIM1, 2120, 990, -1)->AddLight(400,RGBa(250,250,50,50),this());
  aSunFlare[2] = CreateObject(TIM1, 2215, 990, -1)->AddLight(500,RGBa(250,250,50,0),this());
  aSunFlare[3] = CreateObject(TIM1, 2310, 990, -1)->AddLight(400,RGBa(250,250,50,50),this());
  aSunFlare[4] = CreateObject(TIM1, 2430, 930, -1)->AddLight(300,RGBa(250,250,50,50),this());

  //Grenzen setzen
  CreateObject(BRDR, 690, 0, -1)->Set(0);
  CreateObject(BRDR, 3740, 0, -1)->Set(1);
  CreateObject(BRDR, 0, 1590, -1)->Set(3,0,1);

  //Sounds

  //Zikaden
  CreateObject(SE4K, 500, 500, -1)->Set("Cicada*.ogg",700,100);
  CreateObject(SE4K, 3930, 500, -1)->Set("Cicada*.ogg",700,100);

  //Wind
  CreateObject(SE4K, 1000, 300, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 2215, 450, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 3430, 300, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 1235, 600, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1470, 820, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1660, 1120, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2770, 1120, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2960, 820, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 3195, 600, -1)->Set("Interior*.ogg",670,105);

  //Rush Hour
  CreateObject(SE4K, 1360, 1410, -1)->Set("Traffic*.ogg",245,70);
  CreateObject(SE4K, 2215, 1530, -1)->Set("Traffic*.ogg",245,70);
  CreateObject(SE4K, 3070, 1410, -1)->Set("Traffic*.ogg",245,70);

  //Sirenen
  CreateObject(SE4K, 1000, 1410, -1)->Set("Siren*.ogg",4000,140);
  CreateObject(SE4K, 3430, 1410, -1)->Set("Siren*.ogg",4000,140);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Dragnin)
  CreateObject(AMCT, 1085, 760, -1)->Set(DGNN);
  CreateObject(AMCT, 3345, 760, -1)->Set(DGNN);

  //Kugeln
  PlaceSpawnpoint(ABOX, 1260, 715);
  PlaceSpawnpoint(ABOX, 2130, 1165);
  PlaceSpawnpoint(ABOX, 3170, 715);

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 1480, 885);
  PlaceSpawnpoint(GBOX, 2300, 1165);
  PlaceSpawnpoint(GBOX, 2950, 885);

  //Raketen
  PlaceSpawnpoint(MBOX, 1390, 1125);
  PlaceSpawnpoint(MBOX, 3040, 1125);

  //Versorgungskisten (APW)
  CreateObject(AMCT, 1675, 1180, -1)->Set(ATWN);
  CreateObject(AMCT, 2755, 1180, -1)->Set(ATWN);

  //Geschützstellungen
  CreateObject(GNET, 1645, 850, -1)->Set(LCAC,90);
  CreateObject(GNET, 2785, 850, -1)->Set(LCAC,-90);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Sonnenschirme
  CreateObject(SNSE, 1100, 760, -1);
  CreateObject(SNSE, 1170, 760, -1);
  CreateObject(SNSE, 1070, 1040, -1)->SetCategory(C4D_Background);
  CreateObject(SNSE, 2225, 1310, -1)->SetCategory(C4D_Background);
  CreateObject(SNSE, 2320, 1530, -1)->SetCategory(C4D_Background);
  CreateObject(SNSE, 3260, 760, -1);
  CreateObject(SNSE, 3360, 1040, -1)->SetCategory(C4D_Background);
  CreateObject(SNSE, 3330, 760, -1);

  //Waschbecken
  CreateObject(WSHB, 1560, 1295, -1)->SetCategory(C4D_Background);
  CreateObject(WSHB, 1560, 1415, -1)->SetCategory(C4D_Background);
  CreateObject(WSHB, 1560, 1525, -1)->SetCategory(C4D_Background);
  CreateObject(WSHB, 1560, 1635, -1)->SetCategory(C4D_Background);

  CreateObject(WSHB, 2890, 1295, -1)->SetCategory(C4D_Background);
  CreateObject(WSHB, 2890, 1415, -1)->SetCategory(C4D_Background);
  CreateObject(WSHB, 2890, 1525, -1)->SetCategory(C4D_Background);
  CreateObject(WSHB, 2890, 1635, -1)->SetCategory(C4D_Background);

  //Leitkegel
  CreateObject(TCCN, 1060, 630, -1);
  CreateObject(TCCN, 1270, 630, -1);
  CreateObject(TCCN, 1490, 760, -1);

  CreateObject(TCCN, 2120, 1080, -1);
  CreateObject(TCCN, 2140, 1080, -1);
  CreateObject(TCCN, 2150, 1170, -1);
  CreateObject(TCCN, 2160, 1080, -1);
  CreateObject(TCCN, 2180, 990, -1);

  CreateObject(TCCN, 2250, 990, -1);
  CreateObject(TCCN, 2270, 1080, -1);
  CreateObject(TCCN, 2280, 1170, -1);
  CreateObject(TCCN, 2290, 1080, -1);
  CreateObject(TCCN, 2310, 1080, -1);

  CreateObject(TCCN, 2940, 760, -1);
  CreateObject(TCCN, 3160, 630, -1);
  CreateObject(TCCN, 3370, 630, -1);

  //Topfpflanzen
  CreateObject(PLT3, 1070, 760, -1);
  CreateObject(PLT4, 1135, 753, -1);
  CreateObject(PLT3, 1200, 760, -1);
  CreateObject(PLT3, 3230, 760, -1);
  CreateObject(PLT4, 3295, 753, -1);
  CreateObject(PLT3, 3360, 760, -1);

  //Geländer
  CreateObject(RAI1, 1085, 490, -1)->SetRail([1,0,1,0,1,0,1]);
  CreateObject(RAI1, 1191, 1050, -1)->SetRail([3,1,3]);
  var rail = CreateObject(RAI1, 1484, 670, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1]);
  for(var i = 0; i <= 5; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 1570, 850, -1)->SetRail([1,0,1,0,1,0,1]);

  CreateObject(RAI1, 1585, 1130, -1)->SetRail([3,1,1,3]);
  CreateObject(RAI1, 2129, 990, -1)->SetRail([3,1,1,1,1,1,1,1,1,1,3]);
  CreateObject(RAI1, 2110, 1080, -1)->SetRail([3,1,1,3]);
  CreateObject(RAI1, 2275, 1080, -1)->SetRail([3,1,1,3]);
  CreateObject(RAI1, 2190, 1150, -1)->SetRail([3,1,1,3]);
  CreateObject(RAI1, 2800, 1130, -1)->SetRail([3,1,1,3]);

  CreateObject(RAI1, 2770, 850, -1)->SetRail([1,0,1,0,1,0,1]);
  rail = CreateObject(RAI1, 2904, 670, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1]);
  for(var i = 0; i <= 5; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 3211, 1050, -1)->SetRail([3,1,3]);
  CreateObject(RAI1, 3255, 490, -1)->SetRail([1,0,1,0,1,0,1]);

  //Radare
  CreateObject(RADR, 1150, 490, -1);
  CreateObject(RADR, 3280, 490, -1);

  //Automaten
  CreateObject(VGMN, 1231, 1035, -1)->SetCategory(C4D_Background);
  CreateObject(VGMN, 3120, 1035, -1)->SetCategory(C4D_Background);

  //Glastische
  CreateObject(GTBL, 1135, 760, -1);
  CreateObject(GTBL, 3295, 760, -1);

  //Wandlampen
  CreateObject(BLGH, 1285, 860, -1);
  CreateObject(BLGH, 1605, 910, -1);

  CreateObject(BLGH, 2825, 910, -1);
  CreateObject(BLGH, 3145, 860, -1);

  //Bildschirme
  CreateObject(SCA2, 1285, 905, -1);
  CreateObject(SCA2, 2215, 1145, -1);
  CreateObject(SCA2, 3145, 905, -1);

  //Pumpen
  CreateObject(PMP2, 1170, 1040, -1)->SetCategory(C4D_Background);
  CreateObject(PMP2, 1680, 1300, -1)->SetCategory(C4D_Background);
  CreateObject(PMP2, 1150, 630, -1);
  CreateObject(PMP2, 2135, 1640, -1)->SetCategory(C4D_Background);
  CreateObject(PMP2, 2320, 1420, -1)->SetCategory(C4D_Background);
  CreateObject(PMP2, 3280, 630, -1);
  CreateObject(PMP2, 2750, 1300, -1)->SetCategory(C4D_Background);
  CreateObject(PMP2, 3260, 1040, -1)->SetCategory(C4D_Background);

  //Lüftungsgitter
  CreateObject(ENGT, 1495, 760, -1)->SetCon(75);
  CreateObject(ENGT, 2935, 760, -1)->SetCon(75);

  //Rahmen
  CreateObject(FRME, 1080, 1032, -1);
  CreateObject(FRME, 1100, 1422, -1);
  CreateObject(FRME, 1170, 1422, -1);
  CreateObject(FRME, 1190, 1032, -1);
  CreateObject(FRME, 1280, 1032, -1);

  CreateObject(FRME, 1490, 1292, -1);
  CreateObject(FRME, 1490, 1412, -1);
  CreateObject(FRME, 1490, 1522, -1);
  CreateObject(FRME, 1490, 1632, -1);

  CreateObject(FRME, 1560, 1292, -1);
  CreateObject(FRME, 1560, 1412, -1);
  CreateObject(FRME, 1560, 1522, -1);
  CreateObject(FRME, 1560, 1632, -1);

  CreateObject(FRME, 1650, 1292, -1);
  CreateObject(FRME, 1650, 1412, -1);
  CreateObject(FRME, 1650, 1522, -1);
  CreateObject(FRME, 1650, 1632, -1);

  CreateObject(FRME, 1720, 1292, -1);
  CreateObject(FRME, 1720, 1412, -1);
  CreateObject(FRME, 1720, 1522, -1);
  CreateObject(FRME, 1720, 1632, -1);

  CreateObject(FRME, 1790, 1292, -1);
  CreateObject(FRME, 1790, 1412, -1);
  CreateObject(FRME, 1790, 1522, -1);
  CreateObject(FRME, 1790, 1632, -1);

  CreateObject(FRME, 2120, 1302, -1);
  CreateObject(FRME, 2120, 1412, -1);
  CreateObject(FRME, 2120, 1522, -1);
  CreateObject(FRME, 2120, 1632, -1);

  CreateObject(FRME, 2210, 1302, -1);
  CreateObject(FRME, 2210, 1412, -1);
  CreateObject(FRME, 2210, 1522, -1);
  CreateObject(FRME, 2210, 1632, -1);

  CreateObject(FRME, 2300, 1302, -1);
  CreateObject(FRME, 2300, 1412, -1);
  CreateObject(FRME, 2300, 1522, -1);
  CreateObject(FRME, 2300, 1632, -1);

  CreateObject(FRME, 2640, 1292, -1);
  CreateObject(FRME, 2640, 1412, -1);
  CreateObject(FRME, 2640, 1522, -1);
  CreateObject(FRME, 2640, 1632, -1);

  CreateObject(FRME, 2710, 1292, -1);
  CreateObject(FRME, 2710, 1412, -1);
  CreateObject(FRME, 2710, 1522, -1);
  CreateObject(FRME, 2710, 1632, -1);

  CreateObject(FRME, 2780, 1292, -1);
  CreateObject(FRME, 2780, 1412, -1);
  CreateObject(FRME, 2780, 1522, -1);
  CreateObject(FRME, 2780, 1632, -1);

  CreateObject(FRME, 2870, 1292, -1);
  CreateObject(FRME, 2870, 1412, -1);
  CreateObject(FRME, 2870, 1522, -1);
  CreateObject(FRME, 2870, 1632, -1);

  CreateObject(FRME, 2940, 1292, -1);
  CreateObject(FRME, 2940, 1412, -1);
  CreateObject(FRME, 2940, 1522, -1);
  CreateObject(FRME, 2940, 1632, -1);

  CreateObject(FRME, 3150, 1032, -1);
  CreateObject(FRME, 3240, 1032, -1);
  CreateObject(FRME, 3260, 1422, -1);
  CreateObject(FRME, 3330, 1422, -1);
  CreateObject(FRME, 3350, 1032, -1);

  //Scheinwerfer
  CreateObject(FLGH, 1345, 550, -1)->SetRotation(45);
  var light = CreateObject(FLGH, 1565, 720, -1);
  light->SetRotation(45);
  light->SetR(90);
  CreateObject(FLGH, 1740, 1030, -1)->SetRotation(45);
  CreateObject(FLGH, 2020, 1152, -1)->SetRotation(45);
  CreateObject(FLGH, 2410, 1152, -1)->SetRotation(-45);
  CreateObject(FLGH, 2690, 1030, -1)->SetRotation(-45);
  light = CreateObject(FLGH, 2865, 720, -1);
  light->SetRotation(-45);
  light->SetR(-90);
  CreateObject(FLGH, 3085, 550, -1)->SetRotation(-45);

  //Flaschen
  CreateObject(BOTL, 1285, 550, -1);
  CreateObject(BOTL, 1630, 980, -1);
  CreateObject(BOTL, 2800, 980, -1);
  CreateObject(BOTL, 3145, 550, -1);

  //Papierstapel
  CreateObject(PPSK, 1150, 630, -1);
  CreateObject(PPSK, 1170, 630, -1);
  CreateObject(PPSK, 1630, 760, -1);
  CreateObject(PPSK, 2050, 1150, -1);
  CreateObject(PPSK, 2380, 1150, -1);
  CreateObject(PPSK, 2800, 760, -1);
  CreateObject(PPSK, 3260, 630, -1);
  CreateObject(PPSK, 3280, 630, -1);

  //Zäune
  CreateObject(FENC, 1075, 920, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 1135, 920, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 1195, 920, -1)->SetCategory(C4D_Background);

  CreateObject(FENC, 1540, 1030, -1);
  CreateObject(FENC, 1600, 1030, -1);
  CreateObject(FENC, 1660, 1030, -1);
  CreateObject(FENC, 1720, 1030, -1);
  CreateObject(FENC, 1780, 1030, -1);

  CreateObject(FENC, 1780, 1310, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 1840, 1310, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 1900, 1310, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 1960, 1310, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 2020, 1310, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 2140, 1310, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 2215, 1310, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 2300, 1310, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 2410, 1310, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 2470, 1310, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 2530, 1310, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 2590, 1310, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 2650, 1310, -1)->SetCategory(C4D_Background);

  CreateObject(FENC, 2650, 1030, -1);
  CreateObject(FENC, 2710, 1030, -1);
  CreateObject(FENC, 2770, 1030, -1);
  CreateObject(FENC, 2830, 1030, -1);
  CreateObject(FENC, 2890, 1030, -1);

  CreateObject(FENC, 3235, 920, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 3295, 920, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 3355, 920, -1)->SetCategory(C4D_Background);

  //Regale
  CreateObject(FRAM, 1110, 540, -1);
  CreateObject(FRAM, 1130, 540, -1);

  CreateObject(FRAM, 1280, 1030, -1)->SetCategory(C4D_Background);
  CreateObject(FRAM, 1470, 1290, -1)->SetCategory(C4D_Background);
  CreateObject(FRAM, 1490, 1290, -1)->SetCategory(C4D_Background);
  CreateObject(FRAM, 1510, 1290, -1)->SetCategory(C4D_Background);

  CreateObject(FRAM, 2125, 1410, -1)->SetCategory(C4D_Background);
  CreateObject(FRAM, 2145, 1410, -1)->SetCategory(C4D_Background);
  CreateObject(FRAM, 2230, 1520, -1)->SetCategory(C4D_Background);
  CreateObject(FRAM, 2300, 1520, -1)->SetCategory(C4D_Background);
  CreateObject(FRAM, 2320, 1630, -1)->SetCategory(C4D_Background);

  CreateObject(FRAM, 2920, 1290, -1)->SetCategory(C4D_Background);
  CreateObject(FRAM, 2940, 1290, -1)->SetCategory(C4D_Background);
  CreateObject(FRAM, 2960, 1290, -1)->SetCategory(C4D_Background);
  CreateObject(FRAM, 3145, 1030, -1)->SetCategory(C4D_Background);

  CreateObject(FRAM, 3300, 540, -1);
  CreateObject(FRAM, 3320, 540, -1);

  //Schilder
  CreateObject(MSGN, 1365, 720, -1);
  CreateObject(MSGN, 1405, 1000, -1);
  CreateObject(MSGN, 1445, 890, -1);
  CreateObject(ESGN, 1538, 755, -1);
  CreateObject(ESGN, 2890, 755, -1);
  CreateObject(MSGN, 2985, 890, -1);
  CreateObject(MSGN, 3025, 1000, -1);
  CreateObject(MSGN, 3065, 720, -1);

  //Feuerlöscher
  CreateObject(FIE2, 1520, 755, -1);
  CreateObject(FIE2, 2910, 755, -1);

  //Poster
  CreateObject(PSTR, 1635, 1025, -1)->Set(3,0);
  CreateObject(PSTR, 2795, 1025, -1)->Set(3,2);

  //Straßenlichter
  CreateObject(SLGH, 1200, 545, -1);
  CreateObject(SLGH, 1360, 950, -1);
  CreateObject(SLGH, 1390, 740, -1);
  CreateObject(SLGH, 1890, 1180, -1);
  CreateObject(SLGH, 2540, 1180, -1);
  CreateObject(SLGH, 3040, 740, -1);
  CreateObject(SLGH, 3070, 950, -1);
  CreateObject(SLGH, 3230, 545, -1);

  //Notausgangslichter
  CreateObject(ETLT, 1150, 525, -1);
  CreateObject(ETLT, 3280, 525, -1);

  //Orientierungslichter
  CreateObject(OLGH, 1130, 490, -1)->Set(1, 50, 1, 1, 80);
  CreateObject(OLGH, 1190, 550, -1)->Set(1, 170, 1, 1, 80);
  CreateObject(OLGH, 1505, 670, -1)->Set(1, 25, 1, 1, 80);
  CreateObject(OLGH, 2215, 990, -1)->Set(2, 45, 1, 1, 30);
  CreateObject(OLGH, 2215, 1080, -1)->Set(2, 55, 1, 1, 30);
  CreateObject(OLGH, 2925, 670, -1)->Set(1, 25, 1, 1, 80);
  CreateObject(OLGH, 3240, 550, -1)->Set(1, 170, 1, 1, 80);
  CreateObject(OLGH, 3300, 490, -1)->Set(1, 50, 1, 1, 80);

  //Büsche
  CreateObject(BSH2, 1240, 560, -1);
  CreateObject(BSH2, 1260, 560, -1);
  CreateObject(BSH2, 1720, 1300, -1)->SetCategory(C4D_Background);
  CreateObject(BSH2, 2225, 1420, -1)->SetCategory(C4D_Background);
  CreateObject(BSH2, 2700, 1300, -1)->SetCategory(C4D_Background);
  CreateObject(BSH2, 3170, 560, -1);
  CreateObject(BSH2, 3190, 560, -1);

  //Bäume
  CreateObject(TRE2, 1250, 555, -1);
  CreateObject(TRE2, 3180, 555, -1);

  //Hinweisschilder
  CreateObject(WLSN, 1105, 662, -1)->Set("A");
  CreateObject(WLSN, 1475, 1195, -1)->Set("A");
  CreateObject(WLSN, 2955, 1195, -1)->Set("B");
  CreateObject(WLSN, 3325, 662, -1)->Set("B");

  //Banner
  CreateObject(BANR, 1035, 550, -1)->SetClrModulation(RGB(50,50,255));
  CreateObject(BANR, 1345, 550, -1)->SetClrModulation(RGB(50,50,255));
  CreateObject(BANR, 1555, 700, -1)->SetClrModulation(RGB(50,50,255));
  CreateObject(BANR, 2875, 700, -1)->SetClrModulation(RGB(255,0,0));
  CreateObject(BANR, 3085, 550, -1)->SetClrModulation(RGB(255,0,0));
  CreateObject(BANR, 3395, 550, -1)->SetClrModulation(RGB(255,0,0));

  //Ventillatoren
  CreateObject(MVNT, 1290, 830, -1);
  CreateObject(VENT, 1780, 1175, -1)->SetClrModulation(RGB(150,150,150));
  CreateObject(VEN3, 2095, 1220, -1)->SetCon(20);
  CreateObject(VEN3, 2335, 1220, -1)->SetCon(20);
  CreateObject(VENT, 2650, 1175, -1)->SetClrModulation(RGB(150,150,150));
  CreateObject(MVNT, 3140, 830, -1);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD03,500,250,-1);
  CreateObject(BD03,2000,350,-1);
  CreateObject(BD03,800,750,-1);
  CreateObject(BD03,1700,730,-1);
  CreateObject(BD10,300,1690,-1);
  CreateObject(BD10,1690,1690,-1);
  CreateObject(BD10,3080,1690,-1);
  CreateObject(BD10,100,1590,-1);
  CreateObject(BD10,1490,1590,-1);
  CreateObject(BD10,2880,1590,-1);
}

/* Bei Relaunch */
 
public func OnClassSelection(object pClonk, int iTeam)
{
  AddEffect("SpawnParachute", pClonk, 1, 10);
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

/* Lichterfärbung */

public func ChangeLightColors()
{
  //Lichtkegel blau färben
  for(var cone in FindObjects(Find_ID(LGHC)))
    cone->ChangeColor(RGB(50,50,255));
}

/* Sonnenuntergang */

global func FxSunsetStart(object pTarget, int iEffectNumber)
{
  EffectVar(0, pTarget, iEffectNumber) = LandscapeWidth() / 2;
  EffectVar(1, pTarget, iEffectNumber) = LandscapeHeight() / 2-50;
  EffectVar(2, pTarget, iEffectNumber) = 0;

  return 1;
}

global func FxSunsetTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var sunx = EffectVar(0, pTarget, iEffectNumber),
      suny = EffectVar(1, pTarget, iEffectNumber),
      clr1 = EffectVar(2, pTarget, iEffectNumber);

  //Sonnenuntergang
  if(suny >= 945)
  {
    //Alle Sonneneffekte entfernen
    RemoveAll(LENS);
    RemoveObject(aSunFlare[0]);
    RemoveObject(aSunFlare[1]);
    RemoveObject(aSunFlare[2]);
    RemoveObject(aSunFlare[3]);
    RemoveObject(aSunFlare[4]);

    //Dunkelheit erhöhen
    if(GetDarkness() < 3)
      FadeDarkness(3,10);

    return -1;
  }
  else
  {
    //Sonne repositionieren
    SetSunPosition(sunx, suny);

    suny ++;

    //Neue Position speichern
    EffectVar(0, pTarget, iEffectNumber) = sunx;
    EffectVar(1, pTarget, iEffectNumber) = suny;
  }

  //Himmel färben
  clr1 ++;
  SetSkyFade(BoundBy(163-clr1,0,163),BoundBy(191-clr1,0,191),BoundBy(198-clr1,0,198), 255,153,0);
  EffectVar(2, pTarget, iEffectNumber) = clr1;
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Toward the Flag.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Breaching.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Sonnenuntergang starten
  CreateObject(TIM1)->AddEffect("Sunset",this,1,150);

  //Helikopter und Hinweisschilder
  if(!FindObject(NOHC))
  {
    SetupVehicleSpawn([APCE],DIR_Left,CreateObject(VSPW,2215,960,-1),100*21);

    var sign = CreateObject(SGNP, 2130, 990, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
  }

  //OCC-Spielziel
  if(FindObject(GOCC))
  {
    //Flaggenposten
    aFlag[0] = CreateObject(OFPL,1475,1130,NO_OWNER);
    aFlag[0] -> AddSpawnPoint(1245,910);
    aFlag[0] -> AddSpawnPoint(1455,880);
    aFlag[0] -> AddSpawnPoint(1590,920);
    aFlag[0]->Set("$Flag1$");

    aFlag[1] = CreateObject(OFPL,1505,760,NO_OWNER);
    aFlag[1] -> AddSpawnPoint(1130,0);
    aFlag[1] -> AddSpawnPoint(1190,0);
    aFlag[1] -> AddSpawnPoint(1250,0);
    if(aTeams[1] == true)
    {
      aFlag[1]->Set("$Flag2$",0,4);
      aFlag[1]->Capture(1,1);
    }
    else
    {
      aFlag[1]->Set("$Flag2$");
    }

    aFlag[2] = CreateObject(OFPL,2215,1150,NO_OWNER);
    aFlag[2] -> AddSpawnPoint(2110,0);
    aFlag[2] -> AddSpawnPoint(2215,0);
    aFlag[2] -> AddSpawnPoint(2320,0);
    aFlag[2]->Set("$Flag3$");

    aFlag[3] = CreateObject(OFPL,2925,760,NO_OWNER);
    aFlag[3] -> AddSpawnPoint(3180,0);
    aFlag[3] -> AddSpawnPoint(3240,0);
    aFlag[3] -> AddSpawnPoint(3300,0);
    if(aTeams[2] == true)
    {
      aFlag[3]->Set("$Flag4$",0,4);
      aFlag[3]->Capture(2,1);
    }
    else
    {
      aFlag[3]->Set("$Flag4$");
    }

    aFlag[4] = CreateObject(OFPL,2955,1130,NO_OWNER);
    aFlag[4] -> AddSpawnPoint(2840,920);
    aFlag[4] -> AddSpawnPoint(2975,880);
    aFlag[4] -> AddSpawnPoint(3185,910);
    aFlag[4]->Set("$Flag5$");

    //Selbstschussanlagen
    var selfdef = CreateObject(SEGU, 1340, 550, -1);
    selfdef->Set(0,1,1,180,0,1660);
    selfdef->SetTeam(1);
    selfdef = CreateObject(SEGU, 3090, 550, -1);
    selfdef->Set(0,1,1,180,2770,4430);
    selfdef->SetTeam(2);
  }

  //BAS-Spielziel
  if(FindObject(GBAS))
  {
    //Strukturen
    AddAssaultTarget(PMP2, 1505, 730, 400, 1, "$Target1$", 0, [[1130, 0], [1190, 0], [1250, 0]]);
    AddAssaultTarget(CMSN, 1385, 920, 400, 1, "$Target2$", 1, [[1070, 630], [1180, 630], [1280, 550]]);
    AddAssaultTarget(TELE, 1475, 1130, 400, 1, "$Target3$", 2, [[1070, 760], [1180, 630], [1280, 550]]);
    AddAssaultTarget(TELE, 2955, 1130, 400, 2, "$Target3$", 3, [[3150, 550], [3250, 630], [3360, 760]]);
    AddAssaultTarget(CMSN, 3045, 920, 400, 2, "$Target2$", 4, [[3150, 550], [3250, 630], [3360, 630]]);
    AddAssaultTarget(PMP2, 2925, 730, 400, 2, "$Target1$", 5, [[3180, 0], [3240, 0], [3300, 0]]);

    //Sprengsatz-Spawn
    SetupBombSpawnpoint([[2215, 990],[2215,1150]]);

    //Selbstschussanlagen
    var selfdef = CreateObject(SEGU, 1340, 550, -1);
    selfdef->Set(0,1,1,180,0,1660);
    selfdef->SetTeam(1);
    selfdef = CreateObject(SEGU, 3090, 550, -1);
    selfdef->Set(0,1,1,180,2770,4430);
    selfdef->SetTeam(2);
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Flaggenposten
    var flag = CreateObject(OFPL, 2215,1150, -1);
    flag->~Set("$Flag2$");

    //Teamgrenzen
    CreateObject(BRDR, 1660, 0, -1)->Set(0,1,0,1,1);
    CreateObject(BRDR, 2770, 0, -1)->Set(1,1,0,1,2);

    //Selbstschussanlagen
    var selfdef = CreateObject(SEGU, 1395, 745, -1);
    selfdef->Set(0,1,1,-130,1350,1660);
    selfdef->SetTeam(1);
    selfdef = CreateObject(SEGU, 3035, 745, -1);
    selfdef->Set(0,1,1,130,2770,3080);
    selfdef->SetTeam(2);
    selfdef->SetR(130);

    //Verbundene Räume
    aDoorWay[00] = CreateObject(GAT3, 1505, 710, -1);
    aDoorWay[01] = CreateObject(GAT3, 1605, 1180, -1);
    aDoorWay[00]->Connect(aDoorWay[01]);
    aDoorWay[02] = CreateObject(GAT3, 2825, 1180, -1);
    aDoorWay[03] = CreateObject(GAT3, 2925, 710, -1);
    aDoorWay[02]->Connect(aDoorWay[03]);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
    //Flaggen
    if(aTeams[1] == true)
    {CreateFlag(1,1455,890,GetTeamColor(1));}
    if(aTeams[2] == true)
    {CreateFlag(2,2975,890,GetTeamColor(2));}

    //Selbstschussanlagen
    var selfdef = CreateObject(SEGU, 1340, 550, -1);
    selfdef->Set(0,1,1,180,0,1660);
    selfdef->SetTeam(1);
    selfdef = CreateObject(SEGU, 3090, 550, -1);
    selfdef->Set(0,1,1,180,2770,4430);
    selfdef->SetTeam(2);
  }

  //LMS-Spielziel
  if(FindObject(GLMS))
  {
    //Verbundene Räume
    aDoorWay[00] = CreateObject(GAT3, 1505, 720, -1);
    aDoorWay[01] = CreateObject(GAT3, 1605, 1180, -1);
    aDoorWay[00]->Connect(aDoorWay[01]);
    aDoorWay[02] = CreateObject(GAT3, 2825, 1180, -1);
    aDoorWay[03] = CreateObject(GAT3, 2925, 720, -1);
    aDoorWay[02]->Connect(aDoorWay[03]);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 2215; iY = 980;
    return 1;
  }

  //HTF/CTF/LMS-Spielziel
  if(FindObject(GHTF) || FindObject(GCTF) || FindObject(GLMS))
  {
    if(iTeam == 1)
      return [[1120, 0], [1220, 0], [1320, 0]];
    if(iTeam == 2)
      return [[3110, 0], [3210, 0], [3310, 0]];
    return 1;
  }
}