/*-- Wavebreaker --*/

#strict 2
#include CSTD

static aFlag,aSelfDefense,aInterior,aDoorWay,aFlagPosition;

func RecommendedGoals()			{return [GOCC, GHTF];}	//Spielzielempfehlung
public func AssaultDefenderTeam()	{return 2;}		//Verteidigerteam bei Assault


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(0,50,15);
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Einrichtungen
  aInterior = [];
  //Türverbindungen
  aDoorWay = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  //Neustart-Button setzen
  SetNextMission("ModernCombat.c4f\\Special.c4f\\CMC_Wavebreaker.c4s", "$RepeatButton$", "$RepeatButtonDesc$");
  return 1; 
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Rampen
  DrawMaterialQuad("Wall-Plate",1260,1320,2390,1320,2270,1350,1290,1350,true);
  DrawMaterialQuad("Wall-Metal2",1290,1350,2270,1350,2230,1360,1300,1360,true);

  DrawMaterialQuad("Wall-Stripes",1310,1140,1340,1140,1430,1170,1400,1170,true);

  DrawMaterialQuad("Wall-Metal2",1340,1300,1400,1280,1400,1300,1340,1300,true);
  DrawMaterialQuad("Wall-Plate",1400,1280,1460,1260,1460,1280,1400,1280,true);

  DrawMaterialQuad("Wall-Plate",1730,1260,1790,1280,1730,1280,1730,1260,true);
  DrawMaterialQuad("Wall-Metal2",1790,1280,1850,1300,1790,1300,1790,1280,true);

  DrawMaterialQuad("Wall-Stripes",2220,1170,2310,1140,2340,1140,2250,1170,true);
  DrawMaterialQuad("Wall-Bricks1",2810,1230,2840,1210,2970,1210,3000,1230,true);

  DrawMaterialQuad("Wall-Bricks1",3190,1760,3220,1750,3220,1760,3190,1760,true);
  DrawMaterialQuad("Wall-Bricks1",3290,1280,3350,1260,3350,1330,3290,1290,true);
  DrawMaterialQuad("Wall-Bricks1",3330,1720,3360,1690,3390,1690,3420,1720,true);
  DrawMaterialQuad("Wall-Bricks1",3510,960,3530,950,3530,960,3510,970,true);
  DrawMaterialQuad("Wall-Bricks1",3500,1720,3530,1690,3530,1720,3500,1720,true);
  DrawMaterialQuad("Wall-Bricks1",3670,950,3690,960,3690,970,3670,960,true);
  DrawMaterialQuad("Wall-Bricks1",3670,1690,3700,1720,3670,1720,3670,1690,true);
  DrawMaterialQuad("Wall-Bricks1",3780,1720,3810,1690,3840,1690,3870,1720,true);
  DrawMaterialQuad("Wall-Bricks1",3910,1280,3850,1260,3850,1330,3910,1290,true);
  DrawMaterialQuad("Wall-Bricks1",3980,1750,4010,1760,3980,1760,3980,1750,true);

  DrawMaterialQuad("Wall-Bricks1",4200,1230,4230,1210,4360,1210,4390,1230,true);
  DrawMaterialQuad("Wall-Stripes",4860,1140,4890,1140,4980,1170,4950,1170,true);

  DrawMaterialQuad("Wall-Metal2",5350,1300,5410,1280,5410,1300,5350,1300,true);
  DrawMaterialQuad("Wall-Plate",5410,1280,5470,1260,5470,1280,5410,1280,true);

  DrawMaterialQuad("Wall-Plate",5740,1260,5800,1280,5740,1280,5740,1260,true);
  DrawMaterialQuad("Wall-Metal2",5800,1280,5860,1300,5800,1300,5800,1280,true);

  DrawMaterialQuad("Wall-Stripes",5770,1170,5860,1140,5890,1140,5800,1170,true);

  DrawMaterialQuad("Wall-Metal2",4930,1350,5910,1350,5900,1360,4970,1360,true);
  DrawMaterialQuad("Wall-Plate",4810,1320,5940,1320,5910,1350,4930,1350,true);

  //Leitern
  CreateObject(LADR, 1255, 1260, -1)->Set(14, LADR_Left);
  CreateObject(LADR, 1480, 1238, -1)->Set(25);
  CreateObject(LADR, 1590, 1238, -1)->Set(25);
  CreateObject(LADR, 1720, 1250, -1)->Set(9);
  CreateObject(LADR, 1880, 1258, -1)->Set(10);
  CreateObject(LADR, 2040, 1258, -1)->Set(10);
  CreateObject(LADR, 2200, 1258, -1)->Set(10);
  CreateObject(LADR, 2395, 1260, -1)->Set(14, LADR_Right);

  CreateObject(LADR, 2790, 1262, -1)->Set(3, LADR_Left, "W");
  CreateObject(LADR, 3020, 1262, -1)->Set(3, LADR_Right, "W");

  CreateObject(LADR, 2830, 1190, -1)->Set(28, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 2940, 900, -1)->Set(4, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 2940, 1030, -1)->Set(6, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3025, 1215, -1)->Set(29, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");

  CreateObject(LADR, 3110, 885, -1)->Set(11, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3170, 1730, -1)->Set(4);
  CreateObject(LADR, 3210, 1165, -1)->Set(35, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3240, 1655, -1)->Set(3);

  CreateObject(LADR, 3320, 930, -1)->Set(11, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3330, 1230, -1)->Set(4, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3345, 1663, -1)->Set(22);
  CreateObject(LADR, 3390, 1452, -1)->Set(23);
  CreateObject(LADR, 3460, 960, -1)->Set(15, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3485, 1380, -1)->Set(14, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3485, 1550, -1)->Set(13, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3550, 920, -1)->Set(12);
  CreateObject(LADR, 3600, 1550, -1)->Set(35, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3650, 920, -1)->Set(12);
  CreateObject(LADR, 3715, 1380, -1)->Set(14, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3715, 1550, -1)->Set(13, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3740, 960, -1)->Set(15, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3810, 1452, -1)->Set(23);
  CreateObject(LADR, 3855, 1663, -1)->Set(22);
  CreateObject(LADR, 3870, 1230, -1)->Set(4, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");

  CreateObject(LADR, 3880, 930, -1)->Set(11, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3960, 1655, -1)->Set(3);
  CreateObject(LADR, 3990, 1165, -1)->Set(35, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 4030, 1730, -1)->Set(4);
  CreateObject(LADR, 4090, 885, -1)->Set(11, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");

  CreateObject(LADR, 4175, 1215, -1)->Set(29, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 4260, 900, -1)->Set(4, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 4260, 1030, -1)->Set(6, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 4370, 1190, -1)->Set(28, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");

  CreateObject(LADR, 4180, 1262, -1)->Set(3, LADR_Left, "W");
  CreateObject(LADR, 4410, 1262, -1)->Set(3, LADR_Right, "W");

  CreateObject(LADR, 4805, 1260, -1)->Set(14, LADR_Left);
  CreateObject(LADR, 5000, 1258, -1)->Set(10);
  CreateObject(LADR, 5160, 1258, -1)->Set(10);
  CreateObject(LADR, 5320, 1258, -1)->Set(10);
  CreateObject(LADR, 5480, 1250, -1)->Set(9);
  CreateObject(LADR, 5610, 1238, -1)->Set(25);
  CreateObject(LADR, 5720, 1238, -1)->Set(25);
  CreateObject(LADR, 5945, 1260, -1)->Set(14, LADR_Right);

  //Bodenluken
  CreateObject(HA4K, 1480, 1033, -1);
  CreateObject(HA4K, 1480, 1113, -1);
  CreateObject(HA4K, 1480, 1173, -1);
  CreateObject(HA4K, 1590, 1033, -1);
  CreateObject(HA4K, 1590, 1113, -1);
  CreateObject(HA4K, 1590, 1173, -1);
  CreateObject(HA4K, 1720, 1173, -1);
  CreateObject(HA4K, 1880, 1173, -1);
  CreateObject(HA4K, 2040, 1173, -1);
  CreateObject(HA4K, 2200, 1173, -1);

  CreateObject(HA4K, 2830, 963, -1);
  CreateObject(HA4K, 3170, 1693, -1);
  CreateObject(HA4K, 3390, 1263, -1);
  CreateObject(HA4K, 3390, 1343, -1);
  CreateObject(HA4K, 3810, 1263, -1);
  CreateObject(HA4K, 3810, 1343, -1);
  CreateObject(HA4K, 4030, 1693, -1);
  CreateObject(HA4K, 4370, 963, -1);

  CreateObject(HA4K, 5000, 1173, -1);
  CreateObject(HA4K, 5160, 1173, -1);
  CreateObject(HA4K, 5320, 1173, -1);
  CreateObject(HA4K, 5480, 1173, -1);
  CreateObject(HA4K, 5610, 1033, -1);
  CreateObject(HA4K, 5610, 1113, -1);
  CreateObject(HA4K, 5610, 1173, -1);
  CreateObject(HA4K, 5720, 1033, -1);
  CreateObject(HA4K, 5720, 1113, -1);
  CreateObject(HA4K, 5720, 1173, -1);

  //Große Bodenluken
  CreateObject(H24K, 3345, 1488, -1);
  CreateObject(H24K, 3545, 928, -1);
  CreateObject(H24K, 3655, 928, -1);
  CreateObject(H24K, 3855, 1488, -1);

  //Gerüste
  CreateObject(SFFG, 3290, 1840, -1)->Set(4);
  CreateObject(SFFG, 3830, 1840, -1)->Set(4);
  CreateObject(SFFG, 3910, 1840, -1)->Set(4);

  //Stahlbrücken
  CreateObject(_HBR, 3095, 992, -1)->SwitchMode();
  CreateObject(_HBR, 3295, 1642, -1)->SwitchMode();
  CreateObject(_HBR, 3565, 962, -1)->SwitchMode();
  CreateObject(_HBR, 3635, 962, -1)->SwitchMode();
  CreateObject(_HBR, 3905, 1642, -1)->SwitchMode();
  CreateObject(_HBR, 4105, 992, -1)->SwitchMode();

  //Explosivtanks
  CreateObject(XTNK, 1480, 1260, -1)->AutoRespawn();
  CreateObject(XTNK, 1590, 1260, -1)->AutoRespawn();
  CreateObject(XTNK, 5610, 1260, -1)->AutoRespawn();
  CreateObject(XTNK, 5720, 1260, -1)->AutoRespawn();

  //Glasscheiben
  CreateObject(PANE, 1422, 1081, -1);
  CreateObject(PANE, 1422, 1110, -1);
  CreateObject(PANE, 1648, 1081, -1);
  CreateObject(PANE, 1648, 1110, -1);

  CreateObject(PANE, 5552, 1081, -1);
  CreateObject(PANE, 5552, 1110, -1);
  CreateObject(PANE, 5778, 1081, -1);
  CreateObject(PANE, 5778, 1110, -1);

  //Kisten
  CreateObject(WCR2, 2770, 1090, -1)->AutoRespawn();
  CreateObject(WCR2, 2900, 1472, -1);
  CreateObject(WCR2, 3275, 1190, -1)->AutoRespawn();
  CreateObject(WCR2, 3290, 1630, -1);
  CreateObject(WCR2, 3910, 1630, -1);
  CreateObject(WCR2, 3925, 1190, -1)->AutoRespawn();
  CreateObject(WCR2, 4300, 1472, -1);
  CreateObject(WCR2, 4430, 1090, -1)->AutoRespawn();

  //Metallkiste
  CreateObject(MWCR, 3600, 1790, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 2790, 1090, -1)->AutoRespawn();
  CreateObject(BECR, 2900, 1490, -1)->AutoRespawn();
  CreateObject(BECR, 3540, 1790, -1)->AutoRespawn();
  CreateObject(BECR, 3600, 950, -1)->AutoRespawn();
  CreateObject(BECR, 3660, 1790, -1)->AutoRespawn();
  CreateObject(BECR, 4300, 1490, -1)->AutoRespawn();
  CreateObject(BECR, 4410, 1090, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 2855, 1210, -1)->AutoRespawn();
  CreateObject(GSBL, 3250, 1190, -1)->AutoRespawn();
  CreateObject(GSBL, 3950, 1190, -1)->AutoRespawn();
  CreateObject(GSBL, 4345, 1210, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 3230, 1190, -1)->AutoRespawn();
  CreateObject(PTNK, 3970, 1190, -1)->AutoRespawn();

  //Sandsackbarrieren
  CreateObject(SBBA, 2380, 1140, -1)->Right();
  CreateObject(SBBA, 3121, 1190, -1);
  CreateObject(SBBA, 3160, 790, -1)->Right();
  CreateObject(SBBA, 3240, 1690, -1)->Right();
  CreateObject(SBBA, 3491, 920, -1);
  CreateObject(SBBA, 3511, 860, -1);
  CreateObject(SBBA, 3689, 860, -1)->Right();
  CreateObject(SBBA, 3710, 920, -1)->Right();
  CreateObject(SBBA, 3961, 1690, -1);
  CreateObject(SBBA, 4040, 790, -1);
  CreateObject(SBBA, 4080, 1190, -1)->Right();
  CreateObject(SBBA, 4821, 1140, -1);

  //Schutztüren
  CreateObject(GDDR, 2897, 1210, -1)->SetColorDw(HSL(0, 100, 100));
  CreateObject(GDDR, 4303, 1210, -1)->SetColorDw(HSL(0, 100, 100));

  //Container
  CreateObject(CON1, 1780, 1171, -1);
  CreateObject(CON1, 1820, 1171, -1)->SetPerspective(2);
  CreateObject(CON1, 5380, 1171, -1)->SetPerspective(2);
  CreateObject(CON1, 5420, 1171, -1);

  //Steine
  CreateObject(STNE, 1515, 1445, -1)->Set(6);
  CreateObject(STNE, 2125, 1580, -1)->Set(1);
  CreateObject(STNE, 5250, 1490, -1)->Set(1);

  //Hydrauliktüren
  var door = CreateObject(SEDR,1459, 1030,-1);
  door->Open();
  door->Lock();
  door = CreateObject(SEDR,1459, 1170,-1);
  door->Open();
  door->Lock();
  door = CreateObject(SEDR,1611, 1030,-1);
  door->Open();
  door->Lock();
  door = CreateObject(SEDR,1611, 1170,-1);
  door->Open();
  door->Lock();

  door = CreateObject(SEDR,5589, 1030,-1);
  door->Open();
  door->Lock();
  door = CreateObject(SEDR,5589, 1170,-1);
  door->Open();
  door->Lock();
  door = CreateObject(SEDR,5741, 1030,-1);
  door->Open();
  door->Lock();
  door = CreateObject(SEDR,5741, 1170,-1);
  door->Open();
  door->Lock();

  //Wasseroberfläche ebnen
  DrawMaterialQuad("Water",1765,1270,3317,1270,3314,1272,1765,1272,1);
  DrawMaterialQuad("Water",3883,1270,5437,1270,5434,1272,3886,1272,1);

  //Nebel
  for(var i=0;i<75;++i)
    CreateParticle("Fog",RandomX(2680,4470),RandomX(710,955),0,0,RandomX(900,1700));
  for(var i=0;i<75;++i)
    CreateParticle("Fog",RandomX(2580,4620),RandomX(955,1200),0,0,RandomX(900,1700));
  for(var i=0;i<20;++i)
    CreateParticle("Fog",RandomX(3485,3715),RandomX(1250,1650),0,0,RandomX(900,1600));

  //Seegras und Muscheln entfernen
  for(var obj in FindObjects(Find_Or(Find_ID(SWD1), Find_ID(SWD2), Find_ID(SHEL)), Find_InRect(1250, 1130, 4700, 170)))
    RemoveObject(obj);
  for(var obj in FindObjects(Find_Or(Find_ID(SWD1), Find_ID(SWD2), Find_ID(SHEL)), Find_InRect(3250, 1690, 700, 120)))
    RemoveObject(obj);

  //Säulen
  aInterior[0] = CreateObject(PILR, 3505, 1040, -1);
  aInterior[0]->Set("CheckPillarCollapse",10);
  aInterior[1] = CreateObject(PILR, 3695, 1040, -1);
  aInterior[1]->Set("CheckPillarCollapse",10);

  //Thronraum
  aInterior[2] = CreateObject(SRTR, 3601, 971,-1);

  //Tore und Konsolen
  var autod = CreateObject(HNG3, 1423, 1110, -1);
  autod->Open();
  CreateObject(CONS, 1500, 1105, -1)->Set(autod);
  autod = CreateObject(HNG3, 1647, 1110, -1);
  autod->Open();
  CreateObject(CONS, 1570, 1105, -1)->Set(autod);
  autod = CreateObject(HNG3, 5553, 1110, -1);
  autod->Open();
  CreateObject(CONS, 5630, 1105, -1)->Set(autod);
  autod = CreateObject(HNG3, 5777, 1110, -1);
  autod->Open();
  CreateObject(CONS, 5700, 1105, -1)->Set(autod);

  //Verbundene Räume
  aDoorWay[00] = CreateObject(ROM3, 3135, 1760, -1);
  aDoorWay[01] = CreateObject(GAT3, 3185, 1150, -1);
  aDoorWay[01]->SetClrModulation(RGB(125,125,125));
  aDoorWay[01]->Connect(aDoorWay[00]);
  aDoorWay[02] = CreateObject(ROM3, 4065, 1760, -1);
  aDoorWay[03] = CreateObject(GAT3, 4015, 1150, -1);
  aDoorWay[03]->SetClrModulation(RGB(125,125,125));
  aDoorWay[03]->Connect(aDoorWay[02]);
  aDoorWay[04] = CreateObject(ROM3, 3145, 1190, -1);
  aDoorWay[05] = CreateObject(GAT3, 3165, 855, -1);
  aDoorWay[05]->SetClrModulation(RGB(125,125,125));
  aDoorWay[05]->Connect(aDoorWay[04]);
  aDoorWay[05] = CreateObject(ROM3, 4055, 1190, -1);
  aDoorWay[06] = CreateObject(GAT3, 4035, 855, -1);
  aDoorWay[06]->SetClrModulation(RGB(125,125,125));
  aDoorWay[06]->Connect(aDoorWay[05]);

  //Soundkulisse

  //Magie
  CreateObject(SE4K, 3000, 1050, -1)->Set("MagicElementEarth",4500,500);
  CreateObject(SE4K, 4200, 1050, -1)->Set("MagicElementEarth",4500,500);

  //Möven
  CreateObject(SE4K, 840, 1090, -1)->Set("Seagulls*.ogg",1000,500, 10,50);
  CreateObject(SE4K, 2800, 800, -1)->Set("Seagulls*.ogg",1000,500, 10,50);
  CreateObject(SE4K, 4400, 800, -1)->Set("Seagulls*.ogg",1000,500, 10,50);
  CreateObject(SE4K, 6360, 1090, -1)->Set("Seagulls*.ogg",1000,500, 10,50);

  //Wellen
  CreateObject(SE4K, 500, 1280, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 1210, 1280, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 2580, 1280, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 3240, 1280, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 3960, 1280, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 4620, 1280, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 5990, 1280, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 6700, 1280, -1)->Set("Wave*.ogg",100,50, 75);

  //Erdrutsche
  CreateObject(SE4K, 2960, 1560, -1)->Set("FallingDirt*.ogg",800,300);
  CreateObject(SE4K, 4240, 1560, -1)->Set("FallingDirt*.ogg",800,300);

  //Wind
  CreateObject(SE4K, 1900, 700, -1)->Set("WindSound*.ogg",300,100, 50);
  CreateObject(SE4K, 3600, 600, -1)->Set("WindSound*.ogg",300,100, 50);
  CreateObject(SE4K, 5300, 700, -1)->Set("WindSound*.ogg",300,100, 50);

  //Innenbereich
  CreateObject(SE4K, 1500, 550, -1)->Set("InteriorMetal*.ogg",1000,300, 75);
  CreateObject(SE4K, 1600, 550, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 3030, 1570, -1)->Set("InteriorStress*.ogg",700,100, 75);
  CreateObject(SE4K, 4170, 1570, -1)->Set("InteriorStress*.ogg",700,100, 75);
  CreateObject(SE4K, 4410, 550, -1)->Set("InteriorMetal*.ogg",1000,300, 75);
  CreateObject(SE4K, 4510, 550, -1)->Set("Interior*.ogg",700,100, 75);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  CreateObject(AMCT, 1770, 1142, -1)->Set(ABOX);
  CreateObject(AMCT, 2925, 1210, -1)->Set(ABOX);
  CreateObject(AMCT, 4030, 1760, -1)->Set(ABOX);
  CreateObject(AMCT, 4275, 1210, -1)->Set(ABOX);
  CreateObject(AMCT, 5430, 1142, -1)->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  CreateObject(AMCT, 1820, 1142, -1)->Set(GBOX);
  CreateObject(AMCT, 3170, 1760, -1)->Set(GBOX);
  CreateObject(AMCT, 5380, 1142, -1)->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MBOX, 1620, 1105);
  PlaceSpawnpoint(MBOX, 2880, 1485);
  PlaceSpawnpoint(MBOX, 4320, 1485);
  PlaceSpawnpoint(MBOX, 5580, 1105);

  //Versorgungskiste (Railgun)
  aInterior[4] = CreateObject(AMCT, 3600, 820, -1)->Set(RLGN,0,0,0,1);

  //Versorgungskisten (APW)
  CreateObject(AMCT, 2965, 1110, -1)->Set(ATWN);
  CreateObject(AMCT, 4245, 1110, -1)->Set(ATWN);

  //Versorgungskisten (Handgranaten)
  CreateObject(AMCT, 2995, 1760, -1)->Set(FRAG);
  CreateObject(AMCT, 4205, 1760, -1)->Set(FRAG);

  //Geschützstellungen
  CreateObject(GNET, 3330, 1630, -1)->Set(CNNW,90);
  CreateObject(GNET, 3150, 980, -1)->Set(CNNW,90);
  CreateObject(GNET, 3410, 1480, -1)->Set(CNNW,90);
  CreateObject(GNET, 3790, 1480, -1)->Set(CNNW,-90);
  CreateObject(GNET, 4050, 980, -1)->Set(CNNW,-90);
  CreateObject(GNET, 3880, 1630, -1)->Set(CNNW,-90);

  //MAV-Stationen
  CreateObject(MVSN, 2860, 960, -1)->Set(2865,860,1);
  CreateObject(MVSN, 4330, 960, -1)->Set(4335,860,1);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Labortische
  CreateObject(LTBL, 1450, 1110, -1);
  CreateObject(LTBL, 1620, 1110, -1);
  CreateObject(LTBL, 3280, 1840, -1);
  CreateObject(LTBL, 3920, 1840, -1);
  CreateObject(LTBL, 5580, 1110, -1);
  CreateObject(LTBL, 5750, 1110, -1);

  //Monitore
  CreateObject(MNI2, 1440, 1098, -1);
  CreateObject(MNI2, 1460, 1098, -1);
  CreateObject(MNI2, 1610, 1098, -1)->Off();
  CreateObject(MNI2, 1630, 1098, -1);

  CreateObject(MNI2, 3270, 1828, -1);
  CreateObject(MNI2, 3290, 1828, -1);

  CreateObject(MNI2, 3910, 1828, -1);
  CreateObject(MNI2, 3930, 1828, -1);

  CreateObject(MNI2, 5570, 1098, -1);
  CreateObject(MNI2, 5590, 1098, -1);
  CreateObject(MNI2, 5740, 1098, -1)->Off();
  CreateObject(MNI2, 5760, 1098, -1);

  //Jetwracks
  CreateObject(_JW1, 2000, 1173, -1);
  CreateObject(_JW2, 2090, 1175, -1)->AddSmokeEffect4K(40,-28,5,true,this);
  CreateObject(_JW1, 5110, 1173, -1);
  CreateObject(_JW2, 5200, 1175, -1)->AddSmokeEffect4K(40,-28,5,true,this);

  //Schiffswracks
  CreateObject(_WRK, 2380, 1570, -1);
  CreateObject(_WRK, 4740, 1570, -1);

  //Truhen
  CreateObject(CHST, 2400, 1560, -1);
  CreateObject(CHST, 4790, 1560, -1);

  //Kristalle
  CastObjects(CRYS,4,50,3290,1820);

  //Schwefel
  CastObjects(SPHR,4,50,3870,1820);

  //Kaputtes U-Boot
  CreateObject(SUBB, 3130, 1345, -1)->SetR(60);

  //Rohre
  CreateObject(PIPL, 1290, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1320, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1950, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1980, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 2110, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 2140, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 5060, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 5090, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 5220, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 5250, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 5880, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 5910, 1325, -1)->Up()->Up();

  //Notausgangslichter
  CreateObject(ETLT,3150, 1736, -1);
  CreateObject(ETLT,4050, 1736, -1);

  //Geländer
  CreateObject(RAI1, 1265, 1140, -1)->SetRail([1,3,1,3,1]);
  var rail = CreateObject(RAI1, 1280, 1260, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 19; i++)
   rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 1409, 1030, -1)->SetRail([3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,2,2,2,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3]);
  CreateObject(RAI1, 1463, 980, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  rail = CreateObject(RAI1, 1728, 1260, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 65; i++)
   rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 2318, 1140, -1)->SetRail([1,3,1,3,1]);
  CreateObject(RAI1, 3320, 1480, -1)->SetRail([1,3,1,3,1,3,1,3,3,3,3,3,3,3,3,3]);
  CreateObject(RAI1, 3368, 1260, -1)->SetRail([1,3,1,3,3,3,3,3]);
  CreateObject(RAI1, 3368, 1340, -1)->SetRail([1,3,1,3,3,3,3,3]);
  CreateObject(RAI1, 3528, 1690, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 3778, 1260, -1)->SetRail([3,3,3,3,3,1,3,1]);
  CreateObject(RAI1, 3778, 1340, -1)->SetRail([3,3,3,3,3,1,3,1]);
  CreateObject(RAI1, 3779, 1480, -1)->SetRail([3,3,3,3,3,3,3,3,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 4818, 1140, -1)->SetRail([1,3,1,3,1]);
  rail = CreateObject(RAI1, 4828, 1260, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 65; i++)
   rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 5539, 1030, -1)->SetRail([3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,2,2,2,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3]);
  CreateObject(RAI1, 5593, 980, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  rail = CreateObject(RAI1, 5738, 1260, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 19; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 5870, 1140, -1)->SetRail([1,3,1,3,1]);

  //Lüftungsgitter
  CreateObject(ENGT, 1355, 1260, -1)->AddSmokeEffect4K(100,0,-10,true,this);
  CreateObject(ENGT, 1800, 1260, -1)->AddSmokeEffect4K(100,0,-10,true,this);
  CreateObject(ENGT, 1960, 1260, -1);
  CreateObject(ENGT, 2120, 1260, -1);
  CreateObject(ENGT, 2295, 1260, -1);
  CreateObject(ENGT, 4905, 1260, -1);
  CreateObject(ENGT, 5080, 1260, -1);
  CreateObject(ENGT, 5240, 1260, -1);
  CreateObject(ENGT, 5400, 1260, -1)->AddSmokeEffect4K(100,0,-10,true,this);
  CreateObject(ENGT, 5845, 1260, -1)->AddSmokeEffect4K(100,0,-10,true,this);

  //Regale
  CreateObject(FRAM, 1440, 1170, -1);
  CreateObject(FRAM, 1630, 1170, -1);
  CreateObject(FRAM, 1650, 1170, -1);
  CreateObject(FRAM, 5550, 1170, -1);
  CreateObject(FRAM, 5570, 1170, -1);
  CreateObject(FRAM, 5760, 1170, -1);

  //Feuerlöscher
  CreateObject(FIE2, 1480, 1025, -1);
  CreateObject(FIE2, 3135, 1685, -1);
  CreateObject(FIE2, 4065, 1685, -1);
  CreateObject(FIE2, 5720, 1025, -1);

  //Satellitenschüsseln
  CreateObject(SADH, 1535, 980, -1);
  CreateObject(SADH, 5665, 980, -1);

  //Radare
  CreateObject(RADR, 1490, 980, -1);
  CreateObject(RADR, 1580, 980, -1)->SetPhase(10);
  CreateObject(RADR, 1758, 1030, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(RADR, 5438, 1030, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(RADR, 5620, 980, -1)->SetPhase(10);
  CreateObject(RADR, 5710, 980, -1);

  //Orientierungslichter
  CreateObject(OLGH, 1310, 1140, -1)->Set(2, 10, 1, 1, 30);
  CreateObject(OLGH, 1540, 980, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 1960, 1170, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 2120, 1170, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 2340, 1140, -1)->Set(2, 10, 1, 1, 30);
  CreateObject(OLGH, 4860, 1140, -1)->Set(2, 10, 1, 1, 30);
  CreateObject(OLGH, 5080, 1170, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 5240, 1170, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 5665, 980, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 5890, 1140, -1)->Set(2, 10, 1, 1, 30);

  //Bildschirme
  CreateObject(SCA2, 1535, 1095, -1)->SetClips([12]);
  CreateObject(SCA2, 3600, 875, -1)->SetClips([13]);
  CreateObject(SCA2, 3600, 1630, -1)->SetClips([12]);
  CreateObject(SCA2, 5665, 1095, -1)->SetClips([12]);

  //Verbranntes Schlösser
  CreateObject(CSB3, 3525, 1255, -1);
  CreateObject(CSB3, 3600, 1230, -1);
  CreateObject(CSB3, 3665, 1260, -1);

  //Verbrannte Zaubertürme
  CreateObject(WTRB, 2650, 1230, -1);
  CreateObject(WTRB, 3000, 900, -1);
  CreateObject(WTRB, 3285, 1190, -1);
  CreateObject(WTRB, 3915, 1190, -1);
  CreateObject(WTRB, 4200, 900, -1);
  CreateObject(WTRB, 4550, 1230, -1);

  //Schilder
  CreateObject(MSGN, 1282, 1260, -1);
  CreateObject(MSGN, 1410, 1260, -1);
  CreateObject(ESGN, 1500, 1025, -1);
  CreateObject(MSGN, 1705, 1170, -1);
  CreateObject(MSGN, 1735, 1170, -1);
  CreateObject(MSGN, 1780, 1260, -1);
  CreateObject(MSGN, 1865, 1170, -1);
  CreateObject(MSGN, 1895, 1170, -1);
  CreateObject(MSGN, 2025, 1170, -1);
  CreateObject(MSGN, 2055, 1170, -1);
  CreateObject(MSGN, 2185, 1170, -1);
  CreateObject(MSGN, 2215, 1170, -1);
  CreateObject(MSGN, 2370, 1260, -1);
  CreateObject(ESGN, 3155, 1685, -1);

  CreateObject(ESGN, 4045, 1685, -1);
  CreateObject(MSGN, 4832, 1260, -1);
  CreateObject(MSGN, 4985, 1170, -1);
  CreateObject(MSGN, 5015, 1170, -1);
  CreateObject(MSGN, 5420, 1260, -1);
  CreateObject(MSGN, 5145, 1170, -1);
  CreateObject(MSGN, 5175, 1170, -1);
  CreateObject(MSGN, 5305, 1170, -1);
  CreateObject(MSGN, 5335, 1170, -1);
  CreateObject(MSGN, 5465, 1170, -1);
  CreateObject(MSGN, 5495, 1170, -1);
  CreateObject(ESGN, 5700, 1025, -1);
  CreateObject(MSGN, 5790, 1260, -1);
  CreateObject(MSGN, 5920, 1260, -1);

  //Lianen
  SetCon(90, CreateObject(VINE, 2810, 1160, -1));
  SetCon(90, CreateObject(VINE, 2815, 970, -1));
  CreateObject(VINE, 2845, 980, -1);
  CreateObject(VINE, 3135, 830, -1);
  CreateObject(VINE, 3165, 930, -1);
  CreateObject(VINE, 3335, 1640, -1);
  CreateObject(VINE, 3390, 890, -1);
  CreateObject(VINE, 3410, 1270, -1);
  CreateObject(VINE, 3510, 1540, -1);
  CreateObject(VINE, 3690, 1540, -1);
  CreateObject(VINE, 3790, 1270, -1);
  CreateObject(VINE, 3810, 890, -1);
  CreateObject(VINE, 3865, 1640, -1);
  CreateObject(VINE, 4035, 930, -1);
  CreateObject(VINE, 4065, 830, -1);
  CreateObject(VINE, 4355, 980, -1);
  SetCon(90, CreateObject(VINE, 4385, 970, -1));
  SetCon(90, CreateObject(VINE, 4390, 1160, -1));

  //Bäume
  CreateObject(TRE3, 3365, 1265, -1);
  CreateObject(TRE3, 3550, 1695, -1);
  CreateObject(TRE3, 3650, 1695, -1);
  CreateObject(TRE3, 3835, 1265, -1);

  //Büsche
  CreateObject(BSH2, 2960, 1650, -1);
  CreateObject(BSH2, 3050, 1400, -1);
  CreateObject(BSH2, 3315, 1490, -1);
  CreateObject(BSH2, 3365, 1270, -1);
  CreateObject(BSH2, 3550, 1700, -1);
  CreateObject(BSH2, 3650, 1700, -1);
  CreateObject(BSH2, 3835, 1270, -1);
  CreateObject(BSH2, 3885, 1490, -1);
  CreateObject(BSH2, 4150, 1400, -1);
  CreateObject(BSH2, 4235, 1650, -1);

  //Banner
  CreateObject(BANR, 2795, 950, -1)->SetClrModulation(RGB(50,50,255));
  CreateObject(BANR, 2885, 860, -1)->SetClrModulation(RGB(50,50,255));
  CreateObject(BANR, 3165, 790, -1)->SetClrModulation(RGB(50,50,255));
  CreateObject(BANR, 4035, 790, -1)->SetClrModulation(RGB(255,0,0));
  CreateObject(BANR, 4315, 860, -1)->SetClrModulation(RGB(255,0,0));
  CreateObject(BANR, 4405, 950, -1)->SetClrModulation(RGB(255,0,0));

  //Spinde
  CreateObject(LCKR, 1435, 1170, -1);
  CreateObject(LCKR, 5765, 1170, -1);

  //Thron
  CreateObject(THRN, 3600, 920, -1);

  //Schildständer
  CreateObject(SHRC, 3370, 1481, -1)->SetPhase(1);
  CreateObject(SHRC, 3520, 921, -1)->SetPhase(3);
  CreateObject(SHRC, 3680, 921, -1)->SetPhase(2);
  CreateObject(SHRC, 3830, 1481, -1)->SetPhase(4);

  //Bogenständer
  CreateObject(BWRC, 2770, 1150, -1)->SetPhase(2);
  CreateObject(BWRC, 4010, 790, -1)->SetPhase(3);
  CreateObject(BWRC, 4340, 1210, -1)->SetPhase(3);
  CreateObject(BWRC, 4430, 1150, -1)->SetPhase(3);

  //Speerständer
  CreateObject(SPRC, 3190, 790, -1)->SetPhase(5);
  CreateObject(SPRC, 2860, 1210, -1)->SetPhase(3);

  //Schwertständer
  CreateObject(SWRC, 3270, 1720, -1)->SetPhase(5);
  CreateObject(SWRC, 3300, 1720, -1)->SetPhase(3);
  CreateObject(SWRC, 3900, 1720, -1)->SetPhase(1);
  CreateObject(SWRC, 3930, 1720, -1)->SetPhase(4);

  //Holzbehälter
  CreateObject(WODC, 3170, 1190, -1)->SetPhase(7);
  CreateObject(WODC, 3190, 1190, -1)->SetPhase(4);
  CreateObject(WODC, 3375, 1690, -1)->SetPhase(4);
  CreateObject(WODC, 3825, 1690, -1)->SetPhase(3);
  CreateObject(WODC, 4010, 1190, -1)->SetPhase(3);
  CreateObject(WODC, 4030, 1190, -1)->SetPhase(8);

  //Kerzen
  CreateObject(CNDL, 3145, 1660, -1);
  CreateObject(CNDL, 3165, 1660, -1);
  CreateObject(CNDL, 3185, 1660, -1);
  CreateObject(CNDL, 4015, 1660, -1);
  CreateObject(CNDL, 4035, 1660, -1);
  CreateObject(CNDL, 4055, 1660, -1);

  //Kerzen anzünden und in den Hintergrund verschieben
  for(var obj in FindObjects(Find_ID(CNDL)))
  {
    obj->SetCategory(C4D_StaticBack);
    obj->ControlDigDouble();
  }
}

/* Bei Säulenzerstörungen */

func CheckPillarCollapse(int iPlr)
{
  if(GetAction(aInterior[0]) == "Destroyed" && GetAction(aInterior[1]) == "Destroyed")
  {
    //Spielerbildschirme schütteln
    ShakeViewPort(100);

    //Effekte
    AddParticlesInRect("Smoke3",[10,20],[3430,850,50,40],[10,30],[200,400]);
    AddParticlesInRect("Smoke3",[10,20],[3580,790,40,20],[10,30],[200,400]);
    AddParticlesInRect("Smoke3",[10,20],[3770,850,50,40],[10,30],[200,400]);

    //Zerstörung planen
    Schedule(Format("GameCall(\"OnPillarCollapse\", %d)", iPlr), 200);
  }
}

func OnPillarCollapse(int iPlr)
{
  //Effekte
  var effect = CreateObject(TIM1,3600,890);
  effect->ShakeViewPort(800, this);
  effect->Sound("StructureHit*.ogg");
  effect->Schedule("RemoveObject()",100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,3480,870,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,3600,800,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,3720,870,60,100);

  //Material entfernen
  DrawMaterialQuad("FlySand",3510,960,3530,950,3530,960,3510,970);
  DrawMaterialQuad("FlySand",3670,950,3690,960,3690,970,3670,960);
  DigFreeRect(3510,950,180,20);

  //Lebewesen verletzen
  for(var obj in FindObjects(Find_OCF(OCF_Alive), Find_InRect(3480,820,240,150)))
    DoDmg(25, DMG_Melee, obj, 0, iPlr + 1);

  //Objekte entfernen
  for(var obj in FindObjects(Find_Or(Find_ID(SBBA), Find_ID(LADR), Find_ID(THRN), Find_ID(H24K), Find_ID(BECR), Find_ID(_HBR), Find_ID(FRME), Find_ID(SCA2), Find_ID(SHRC), Find_ID(RSPT)), Find_InRect(3480,820,240,150)))
  {
    if(!Random(2))
      obj->DecoExplode(RandomX(20,10));
    else
      RemoveObject(obj);
  }
  RemoveObject(aInterior[4]);

  //Trümmer verschleudern
  var debris = CreateObject(DBRS, 3480,870, iPlr);
  Fling(debris, RandomX(-3,-1), RandomX(-3,-1));
  SetRDir(RandomX(-20,20),debris);
  debris = CreateObject(DBRS, 3720,870, iPlr);
  Fling(debris, RandomX(4,2), RandomX(3,1));
  SetRDir(RandomX(-20,20),debris);

  aInterior[2]->Fall();
  aInterior[2]->SetOwner(iPlr);

  if(FindObject(GHTF))
  {
    aFlag[0]->SetObjectOrder(this,aInterior[2],1);

    if(aFlagPosition == 1 && ScriptCounter() < 250)
      goto(299);
  }
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(FindObject(GOCC))
  {
    if(pPoint == aFlag[0] && aSelfDefense[0])
    {
      aSelfDefense[0]->SetTeam(iTeam);
      aSelfDefense[1]->SetTeam(iTeam);
    }

    if(pPoint == aFlag[3] && aSelfDefense[4])
    {
      aSelfDefense[4]->SetTeam(iTeam);
      aSelfDefense[4]->TurnOn();

      aInterior[3]->TurnOn();
      aInterior[3]->ChangeLightColor(GetTeamColor(GetTeam(aSelfDefense[4])));
    }

    if(pPoint == aFlag[6] && aSelfDefense[2])
    {
      aSelfDefense[2]->SetTeam(iTeam);
      aSelfDefense[3]->SetTeam(iTeam);
    }
  }
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Blue Ressort.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Blue Ressort.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Aufzug
  CreateObject(LFTP,3600, 804, -1)->DisableVertical();

  //Haie
  CreateObject(SHRK, 1300, 1420, -1)->AutoRespawn();
  CreateObject(SHRK, 2230, 1420, -1)->AutoRespawn();
  CreateObject(SHRK, 3240, 1350, -1)->AutoRespawn();
  CreateObject(SHRK, 3960, 1350, -1)->AutoRespawn();
  CreateObject(SHRK, 4970, 1420, -1)->AutoRespawn();
  CreateObject(SHRK, 5900, 1420, -1)->AutoRespawn();

  //Piranha
  CreateObject(PIRA, 2580, 1400, -1)->AutoRespawn();
  CreateObject(PIRA, 3240, 1330, -1)->AutoRespawn();
  CreateObject(PIRA, 3475, 1770, -1)->AutoRespawn();
  CreateObject(PIRA, 3725, 1770, -1)->AutoRespawn();
  CreateObject(PIRA, 3960, 1330, -1)->AutoRespawn();
  CreateObject(PIRA, 4620, 1400, -1)->AutoRespawn();

  //OCC-Spielziel
  if(FindObject(GOCC))
  {
    //Flaggenposten
    aFlag[0] = CreateObject(OFPL,1960,1025,NO_OWNER);
    aFlag[0] -> AddSpawnPoint(1640, 1020);
    aFlag[0] -> AddSpawnPoint(1690, 1160);
    aFlag[0] -> AddSpawnPoint(1690, 1250);
    aFlag[0] -> AddSpawnPoint(1795, 1132);
    if(aTeams[1] == true)
    {
      aFlag[0]->Set("$Flag1$",70,10);
      aFlag[0]->Capture(1,1);
    }
    else
    {
      aFlag[0]->Set("$Flag1$",70);
    }

    aFlag[1] = CreateObject(OFPL,3060,1250,NO_OWNER);
    aFlag[1] -> SetClrModulation(RGB(150,150,150));
    aFlag[1] -> AddSpawnPoint(2780, 1140);
    aFlag[1] -> AddSpawnPoint(2810, 1080);
    aFlag[1] -> AddSpawnPoint(2900, 1130);
    aFlag[1]->Set("$Flag2$");

    aFlag[2] = CreateObject(OFPL,3365,690,NO_OWNER);
    aFlag[2] -> SetClrModulation(RGB(150,150,150));
    aFlag[2] -> AddSpawnPoint(2900,850);
    aFlag[2] -> AddSpawnPoint(3000,880);
    aFlag[2] -> AddSpawnPoint(3060,840);
    aFlag[2] -> AddSpawnPoint(3150,780);
    aFlag[2]->Set("$Flag3$",120);

    aFlag[3] = CreateObject(OFPL,3600,1260,NO_OWNER);
    aFlag[3] -> SetClrModulation(RGB(150,150,150));
    aFlag[3] -> AddSpawnPoint(3375,1330);
    aFlag[3] -> AddSpawnPoint(3490,1370);
    aFlag[3] -> AddSpawnPoint(3540,1370);
    aFlag[3] -> AddSpawnPoint(3660,1370);
    aFlag[3] -> AddSpawnPoint(3710,1370);
    aFlag[3] -> AddSpawnPoint(3825,1330);
    aFlag[3]->Set("$Flag4$");

    aFlag[4] = CreateObject(OFPL,3835,690,NO_OWNER);
    aFlag[4] -> SetClrModulation(RGB(150,150,150));
    aFlag[4] -> AddSpawnPoint(4050,780);
    aFlag[4] -> AddSpawnPoint(4140,840);
    aFlag[4] -> AddSpawnPoint(4200,880);
    aFlag[4] -> AddSpawnPoint(4300,850);
    aFlag[4]->Set("$Flag5$",120);

    aFlag[5] = CreateObject(OFPL,4140,1250,NO_OWNER);
    aFlag[5] -> SetClrModulation(RGB(150,150,150));
    aFlag[5] -> AddSpawnPoint(4300, 1130);
    aFlag[5] -> AddSpawnPoint(4390, 1080);
    aFlag[5] -> AddSpawnPoint(4420, 1140);
    aFlag[5]->Set("$Flag6$");

    aFlag[6] = CreateObject(OFPL,5240,1025,NO_OWNER);
    aFlag[6] -> AddSpawnPoint(5405, 1132);
    aFlag[6] -> AddSpawnPoint(5510, 1160);
    aFlag[6] -> AddSpawnPoint(5510, 1250);
    aFlag[6] -> AddSpawnPoint(5560, 1020);
    if(aTeams[2] == true)
    {
      aFlag[6]->Set("$Flag7$",70,10);
      aFlag[6]->Capture(2,1);
    }
    else
    {
      aFlag[6]->Set("$Flag7$",70);
    }

    //Grenzen setzen
    CreateObject(BRDR, 880, 0, -1)->Set(0,0,0,0,0,1);
    CreateObject(BRDR, 6320, 0, -1)->Set(1,0,0,0,0,1);
    CreateObject(BRDR, 0, 1400, -1)->Set(3,0,0,0,0,1);

    //Objekte entfernen
    for(var obj in FindObjects(Find_ID(LADR), Find_InRect(3380, 1260, 440, 200)))
      RemoveObject(obj);
    for(var obj in FindObjects(Find_ID(GNET), Find_InRect(3310, 1440, 580, 190)))
      RemoveObject(obj);

    //Boden
    DrawMaterialQuad("Earth-SMOOTH", 2595,1500, 2705,1430, 2745,1480, 2670,1570);
    DrawMaterialQuad("Earth-SMOOTH", 2830,1355, 2940,1355, 2900,1395, 2880,1395);
    DrawMaterialQuad("Earth-ROUGH", 3290,1400, 3150,1375, 3140,1385, 3260,1455);
    DrawMaterialQuad("Wall-Bricks1", 3420,1340, 3570,1340, 3530,1350, 3420,1350);
    DrawMaterialQuad("Wall-Bricks1", 3420,1340, 3520,1310, 3570,1310, 3570,1340);
    DrawMaterialQuad("Wall-Bricks1", 3780,1340, 3630,1340, 3670,1350, 3780,1350);
    DrawMaterialQuad("Wall-Bricks1", 3780,1340, 3680,1310, 3630,1310, 3630,1340);
    DrawMaterialQuad("Wall-Bricks1", 3380,1380, 3820,1380, 3850,1420, 3350,1420);
    DrawMaterialQuad("Earth-ROUGH", 3910,1400, 4060,1385, 4070,1395, 3935,1480);
    DrawMaterialQuad("Earth-SMOOTH", 4255,1355, 4365,1355, 4330,1395, 4285,1395);
    DrawMaterialQuad("Earth-SMOOTH", 4425,1455, 4495,1435, 4605,1495, 4530,1570);
    DrawMaterialQuad("Water-Liquid", 3320,1400, 3340,1410, 3480,1380, 3320,1380);
    DrawMaterialQuad("Water-Liquid", 3880,1400, 3860,1410, 3720,1380, 3880,1380);

    //Leitern
    CreateObject(LADR, 3390, 1372, -1)->Set(13);
    CreateObject(LADR, 3810, 1372, -1)->Set(13);

    //Gerüste
    CreateObject(SFFG, 1930, 1120, -1)->Set(5);
    CreateObject(SFFG, 1930, 1170, -1)->Set(5);
    CreateObject(SFFG, 1960, 1073, -1)->Set(5);
    CreateObject(SFFG, 1991, 1120, -1)->Set(5);
    CreateObject(SFFG, 1991, 1170, -1)->Set(5);
    CreateObject(SFFG, 3560, 1310, -1)->Set(3);
    CreateObject(SFFG, 3640, 1310, -1)->Set(2);
    CreateObject(SFFG, 5210, 1120, -1)->Set(5);
    CreateObject(SFFG, 5210, 1170, -1)->Set(5);
    CreateObject(SFFG, 5240, 1073, -1)->Set(5);
    CreateObject(SFFG, 5271, 1120, -1)->Set(5);
    CreateObject(SFFG, 5271, 1170, -1)->Set(5);

    //Verbandskiste
    CreateObject(BECR, 3600, 1380, -1)->AutoRespawn();

    //Hinweisschilder
    CreateObject(SGNP, 830, 1220, -1);
    CreateObject(SGNP, 6370, 1220, -1);

    //Artilleriebatterien
    CreateObject(ATBY,2340,1140,-1)->SetRotation(20);
    CreateObject(ATBY,4860,1140,-1)->SetRotation(-20);

    //Geschützstellungen
    CreateObject(GNET, 1650, 1030, -1)->Set(SATW,90);
    CreateObject(GNET, 2850, 860, -1)->Set(CNNW,-90);
    CreateObject(GNET, 4350, 860, -1)->Set(CNNW,90);
    CreateObject(GNET, 5550, 1030, -1)->Set(SATW,-90);

    //Patrouillenboote
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1820,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1920,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2020,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,3260,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3940,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5180,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5280,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5380,1270,-1),50*21);

    //Helikopter und Hinweisschilder
    if(!FindObject(NOHC))
    {
      SetupVehicleSpawn([APCE],DIR_Right,CreateObject(VSPW,1960,1140,-1),70*21);
      SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,2130,1140,-1),70*21);
      SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,5070,1140,-1),70*21);
      SetupVehicleSpawn([APCE],DIR_Left,CreateObject(VSPW,5235,1140,-1),70*21);

      var sign = CreateObject(SGNP, 1680, 1170, -1);
      sign->SetMode(1);
      sign->Set("Helicopter");
      sign = CreateObject(SGNP, 5930, 1140, -1);
      sign->SetMode(1);
      sign->Set("Helicopter");
    }

    //Aufzüge
    CreateObject(LFTP, 1535, 1270, -1);
    CreateObject(LFTP, 5665, 1270, -1);

    //Automaten
    var store = CreateObject(WPVM,1655, 1260,-1);
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
    store = CreateObject(WPVM,3600, 1690,-1);
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
    store = CreateObject(WPVM,5540, 1260,-1);
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

    //Selbstschussanlagen
    aSelfDefense[0] = CreateObject(SEGU, 1310, 1140, -1);
    aSelfDefense[0]->Set(0,1,1,180,0,1620);
    aSelfDefense[1] = CreateObject(SEGU, 2270, 1150, -1);
    aSelfDefense[1]->Set(0,1,1,180,0,2790);
    aSelfDefense[2] = CreateObject(SEGU, 4935, 1150, -1);
    aSelfDefense[2]->Set(0,1,1,180,4410,0);
    aSelfDefense[3] = CreateObject(SEGU, 5890, 1140, -1);
    aSelfDefense[3]->Set(0,1,1,180,5580);
    aSelfDefense[4] = CreateObject(SEGU, 3600, 750, -1);
    aSelfDefense[4]->Set(0,1,0,180,3220,3980);
    aSelfDefense[4]->SetCategory(C4D_StaticBack);

    //SSA Besitzer setzen
    if(aTeams[1] == true)
    {
      aSelfDefense[0]->SetTeam(1);
      aSelfDefense[1]->SetTeam(1);
    }
    if(aTeams[2] == true)
    {
      aSelfDefense[2]->SetTeam(2);
      aSelfDefense[3]->SetTeam(2);
    }

    //Lampe
    aInterior[3] = CreateObject(ALGH, 3600, 761, -1);
    aInterior[3]->SetR(180);

    //Kisten
    CreateObject(WCR2, 3580, 1672, -1);
    CreateObject(WCR2, 3580, 1690, -1);
    CreateObject(WCR2, 3600, 1672, -1);
    CreateObject(WCR2, 3600, 1690, -1);
    CreateObject(WCR2, 3620, 1672, -1);
    CreateObject(WCR2, 3620, 1690, -1);

    //Hinweisschild
    var sign = CreateObject(SGNP, 3645, 1260, -1);
    sign->Set("Turret");
    sign->SetCategory(C4D_StaticBack);

    //Holzbehälter
    CreateObject(WODC, 3495, 1380, -1)->SetPhase(4);
    CreateObject(WODC, 3515, 1380, -1)->SetPhase(6);
    CreateObject(WODC, 3685, 1380, -1)->SetPhase(1);
    CreateObject(WODC, 3705, 1380, -1)->SetPhase(9);

    //Steine
    CreateObject(STNE, 2605, 1495, -1)->Set(2);
    CreateObject(STNE, 2880, 1355, -1)->Set(1);
    CreateObject(STNE, 3290, 1400, -1)->Set(5);
    CreateObject(STNE, 3910, 1400, -1)->Set(5);
    CreateObject(STNE, 4320, 1355, -1)->Set(1);
    CreateObject(STNE, 4600, 1495, -1)->Set(2);
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Script starten
    ScriptGo(1);
    aFlagPosition = 2;

    //Flaggenposten
    aFlag[0] = CreateObject(OFPL, 3600,1250, -1);
    aFlag[0]->SetClrModulation(RGB(150,150,150));
    aFlag[0]->Set("$Flag4$",130);

    //Teamgrenzen
    CreateObject(BRDR, 3060, 0, -1)->Set(0,1,0,1,1);
    CreateObject(BRDR, 4140, 0, -1)->Set(1,1,0,1,2);

    //Grenzen setzen
    CreateObject(BRDR, 2390, 0, -1)->Set(0,0,0,0,0,1);
    CreateObject(BRDR, 4810, 0, -1)->Set(1,0,0,0,0,1);

    //Leitern
    CreateObject(LADR, 2650, 1255, -1)->Set(6);
    CreateObject(LADR, 4550, 1255, -1)->Set(6);

    //Sandsackbarrieren
    CreateObject(SBBA, 3540, 1690, -1);
    CreateObject(SBBA, 3660, 1690, -1)->Right();

    //Hinweisschilder
    CreateObject(SGNP, 2370, 1140, -1);
    CreateObject(SGNP, 4830, 1140, -1);

    //Gerüste
    CreateObject(SFFG, 2540, 1250, -1)->Set(4);
    CreateObject(SFFG, 2600, 1250, -1)->Set(4);
    CreateObject(SFFG, 4600, 1250, -1)->Set(4);
    CreateObject(SFFG, 4660, 1250, -1)->Set(4);

    //Versorgungskiste verschieben
    aInterior[4]->SetPosition(3600, 915);

    //Patrouillenboote
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2690,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2790,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4410,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4510,1270,-1),50*21);

    //Helikopter und Hinweisschilder
    if(!FindObject(NOHC))
    {
      SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,2580,1170,-1),70*21);
      SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,4620,1170,-1),70*21);

      var sign = CreateObject(SGNP, 2730, 1150, -1);
      sign->SetMode(1);
      sign->Set("Helicopter");
      sign = CreateObject(SGNP, 4470, 1150, -1);
      sign->SetMode(1);
      sign->Set("Helicopter");
    }
  }

  //AS-Spielziel
  if(FindObject(GASS))
  {
    //Zielobjekte
    AddAssaultTarget(PRCA, 2775, 1150, 30*30, 2, "$Target1$", 0, [[[3140, 790], [3170, 890], [3190, 1190]], [[1640, 1030], [1680, 1170], [1690, 1260]]]);
    AddAssaultTarget(SHTL, 3600, 1690, 30*30, 2, "$Target2$", 1, [[[4270, 1490], [4305, 1590], [4390, 1550]], [[1900, 1170], [2010, 1170], [2215, 1170]]]);
    AddAssaultTarget(PMP2, 4245, 1217, 0, 2, "$Target3$", 2, [[[4015, 790], [4325, 860], [4390, 960]], [[3050, 980], [3170, 1190], [3180, 890]]]);
    AddAssaultTarget(BKHK, 5235, 1170, 0, 2, "$Target4$", 3, [[[5760, 1170], [5770, 1030], [5865, 1140]], [[4040, 890], [4050, 790], [4200, 890]]]);
    AddAssaultTarget(CCP2, 5235, 1260, 0, 2, "$Target5$", 4, [[[5760, 1170], [5770, 1030], [5865, 1140]], [[4040, 890], [4050, 790], [4200, 890]]]);

    //Ziele verbinden
    ConnectAssaultTargets([3, 4]);

    //Grenzen setzen
    CreateObject(BRDR, 880, 0, -1)->Set(0);
    CreateObject(BRDR, 3300, 0, -1)->Set(1,1);
    CreateObject(BRDR, 0, 1350, -1)->Set(3,1);
    CreateObject(BRDR, 0, 440, -1)->Set(2,1);

    //Leitern
    CreateObject(LADR, 5975, 1206, -1)->Set(10);
    CreateObject(LADR, 6128, 1254, -1)->Set(16);

    //Sandsackbarrieren
    CreateObject(SBBA, 5510, 1260, -1);
    CreateObject(SBBA, 5540, 1260, -1)->Right();

    //Gerüste
    CreateObject(SFFG, 6020, 1170, -1)->Set(4);
    CreateObject(SFFG, 6020, 1220, -1)->Set(5);
    CreateObject(SFFG, 6020, 1270, -1)->Set(5);
    CreateObject(SFFG, 6081, 1170, -1)->Set(4);
    CreateObject(SFFG, 6081, 1220, -1)->Set(5);
    CreateObject(SFFG, 6081, 1270, -1)->Set(5);

    //Stahlbrücken
    CreateObject(_HBR, 4040, 1120, -1);
    CreateObject(_HBR, 4220, 1050, -1);

    //Metallkiste
    CreateObject(MWCR, 5060, 1170, -1);

    //Helikopter und Hinweisschilder
    if(!FindObject(NOHC))
    {
      SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,2130,1140,-1),70*21);

      var sign = CreateObject(SGNP, 1680, 1170, -1);
      sign->SetMode(1);
      sign->Set("Helicopter");
      sign = CreateObject(SGNP, 5520, 1170, -1);
      sign->SetMode(1);
      sign->Set("Helicopter");
    }

    //Aufzüge
    CreateObject(LFTP, 1535, 1270, -1);
    CreateObject(LFTP, 5665, 1270, -1);

    //Artilleriebatterien
    CreateObject(ATBY,2340,1140,-1)->SetRotation(20);
    CreateObject(ATBY,4860,1140,-1)->SetRotation(-20);

    //Geschützstellungen
    CreateObject(GNET, 1650, 1030, -1)->Set(SATW,90);
    CreateObject(GNET, 2850, 860, -1)->Set(CNNW,-90);
    CreateObject(GNET, 4350, 860, -1)->Set(CNNW,90);
    CreateObject(GNET, 5550, 1030, -1)->Set(SATW,-90);
    CreateObject(GNET, 5520, 1260, -1)->Set(LCAC,-90);

    //Patrouillenboote
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1820,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1920,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2020,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,3260,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3940,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,4040,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5280,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5380,1270,-1),50*21);

    //Selbstschussanlagen
    aSelfDefense[0] = CreateObject(SEGU, 1310, 1140, -1);
    aSelfDefense[0]->Set(0,1,1,180,0,1620);
    aSelfDefense[1] = CreateObject(SEGU, 5890, 1140, -1);
    aSelfDefense[1]->Set(0,1,1,180,5580);

    //SSA Besitzer setzen
    if(aTeams[1] == true)
      aSelfDefense[0]->SetTeam(1);
    if(aTeams[2] == true)
      aSelfDefense[1]->SetTeam(2);

    //Objekt entfernen
    RemoveObject(FindObject2(Find_ID(BWRC),Find_InRect(2755, 1120, 20, 20)));
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
    //Flaggen
    if(aTeams[1] == true)
      CreateFlag(1,2940,1110,GetTeamColor(1));
    if(aTeams[2] == true)
      CreateFlag(2,4260,1110,GetTeamColor(2));

    //Grenzen setzen
    CreateObject(BRDR, 2390, 0, -1)->Set(0,0,0,0,0,1);
    CreateObject(BRDR, 4810, 0, -1)->Set(1,0,0,0,0,1);

    //Leitern
    CreateObject(LADR, 2650, 1255, -1)->Set(6);
    CreateObject(LADR, 4550, 1255, -1)->Set(6);

    //Sandsackbarrieren
    CreateObject(SBBA, 3540, 1690, -1);
    CreateObject(SBBA, 3660, 1690, -1)->Right();

    //Hinweisschilder
    CreateObject(SGNP, 2370, 1140, -1);
    CreateObject(SGNP, 4830, 1140, -1);

    //Gerüste
    CreateObject(SFFG, 2540, 1250, -1)->Set(4);
    CreateObject(SFFG, 2600, 1250, -1)->Set(4);
    CreateObject(SFFG, 4600, 1250, -1)->Set(4);
    CreateObject(SFFG, 4660, 1250, -1)->Set(4);

    //Patrouillenboote
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2690,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2790,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4410,1270,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4510,1270,-1),50*21);

    //Helikopter und Hinweisschilder
    if(!FindObject(NOHC))
    {
      SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,2580,1170,-1),70*21);
      SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,4620,1170,-1),70*21);

      var sign = CreateObject(SGNP, 2730, 1150, -1);
      sign->SetMode(1);
      sign->Set("Helicopter");
      sign = CreateObject(SGNP, 4470, 1150, -1);
      sign->SetMode(1);
      sign->Set("Helicopter");
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
    CreateObject(BRDR, 880, 0, -1)->Set(0);
    CreateObject(BRDR, 4410, 0, -1)->Set(1,1);
    CreateObject(BRDR, 0, 440, -1)->Set(2,1);

    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),3200,0,1,1);
  }

  //Ziel 2
  if(iIndex == 1)
  {
    //Grenzen neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 880, 0, -1)->Set(0);
    CreateObject(BRDR, 4620, 0, -1)->Set(1,1);
    CreateObject(BRDR, 0, 440, -1)->Set(2,1);

    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),3870,0,1,1);

    //Spawnpoint entfernen
    RemoveObject(FindObject2(Find_ID(VSPW),Find_InRect(2119, 1169, 2, 2)));
  }

  //Ziel 3
  if(iIndex == 2)
  {
    //Grenzen neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 2660, 0, -1)->Set(0);
    CreateObject(BRDR, 6320, 0, -1)->Set(1,1);
    CreateObject(BRDR, 0, 440, -1)->Set(2,1);

    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),4400,0,1,1);

    //Helikopter
    if(!FindObject(NOHC))
      SetupVehicleSpawn([APCE],DIR_Left,CreateObject(VSPW,6035,1090,-1),70*21);
  }

  //Ziel 4 und 5
  if(iIndex == 3 || iIndex == 4)
  {
    if(fConnectedDestroyed)
    {
      //SSA zerstören
      aSelfDefense[1]->Disarm();
      aSelfDefense[1]->DecoExplode(30);

      //Rauch
      CreateParticle("GunSmoke",4905,1240,0,-10,300,1);
      CreateParticle("GunSmoke",5075,1240,0,-10,300,1);
      CreateParticle("GunSmoke",5235,1240,0,-10,300,1);
    }
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 2795; iY = 850;
    return 1;
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {if(FindObject(GASS)->GetRespawnPoint(iX, iY, iTeam)) return 1;}


  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    if(iTeam == 1)
      return [[2810, 1080], [2810, 1140], [2880, 950], [2885, 850]];
    if(iTeam == 2)
      return [[4315, 850], [4320, 950], [4390, 1080], [4390, 1140]];
    return 1;
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
    if(iTeam == 1)
      return [[2810, 950], [2870, 850], [2970, 900]];
    if(iTeam == 2)
      return [[4230, 900], [4330, 850], [4390, 950]];
    return 1;
  }
}

/* Flaggensteuerung */

protected func Script250()
{
  EventInfo4K(0,Format("$MsgFlagChanging$"),SM21, 0, 0, 0, "Info_Objective.ogg");
  aFlag[0]->AddSmokeEffect4K(50,0,-10);
}

protected func Script300()
{
  RemoveEffect("IntWreckSmoke4K",aFlag[0]);
  if(aFlagPosition == 1)
  {
    if(!Random(2))
    {
      aFlag[0]->MoveFlagpost(3600,1250,"$Flag4$",130);
      aFlag[0]->SetClrModulation(RGB(150,150,150));
      aFlagPosition = 2;
    }
    else
    {
      aFlag[0]->MoveFlagpost(3600,1690,"$Flag9$",70);
      aFlagPosition = 3;
    }
  }
  else
  if(aFlagPosition == 2)
  {
    if(GetAction(aInterior[2]) == "Be" && !Random(2))
    {
      aFlag[0]->MoveFlagpost(3600,820,"$Flag8$",100);
      aFlagPosition = 1;
    }
    else
    {
      aFlag[0]->MoveFlagpost(3600,1690,"$Flag9$",70);
      aFlagPosition = 3;
    }
  }
  else
  if(aFlagPosition == 3)
  {
    if(GetAction(aInterior[2]) == "Be" && !Random(2))
    {
      aFlag[0]->MoveFlagpost(3600,820,"$Flag8$",100);
      aFlagPosition = 1;
    }
    else
    {
      aFlag[0]->MoveFlagpost(3600,1250,"$Flag4$",130);
      aFlag[0]->SetClrModulation(RGB(150,150,150));
      aFlagPosition = 2;
    }
  }

  EventInfo4K(0,Format("$MsgFlagChanged$", GetName(aFlag[0])),SM21, 0, 0, 0, "Info_Objective.ogg");

  goto(0);
}