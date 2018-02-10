/*-- Mankel Overpass --*/

#strict 2
#include CSTD

static aFlag,aSelfDefense,aDoorWay,aFlagPosition;

func RecommendedGoals()	{return [GHTF];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(1,40,15);
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Türverbindungen
  aDoorWay = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  //Neustart-Button setzen
  SetNextMission("ModernCombat.c4f\\Special.c4f\\CMC_MankelOverpass.c4s", "$RepeatButton$", "$RepeatButtonDesc$");
  return 1;
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Rampen
  DrawMaterialQuad("BackWall-Unknown1",1240,120,1240,130,750,740,740,740,true);
  DrawMaterialQuad("BackWall-Unknown1",1240,310,1240,320,900,740,890,740,true);
  DrawMaterialQuad("BackWall-Unknown1",1240,500,1240,510,1050,740,1040,740,true);

  DrawMaterialQuad("BackWall-Unknown1",1360,120,1360,130,1850,740,1860,740,true);
  DrawMaterialQuad("BackWall-Unknown1",1360,310,1360,320,1700,740,1710,740,true);
  DrawMaterialQuad("BackWall-Unknown1",1360,500,1360,510,1550,740,1560,740,true);


  DrawMaterialQuad("BackWall-Unknown1",2360,120,2360,130,1870,740,1860,740,true);
  DrawMaterialQuad("BackWall-Unknown1",2360,310,2360,320,2030,740,2020,740,true);
  DrawMaterialQuad("BackWall-Unknown1",2360,500,2360,510,2170,740,2160,740,true);

  DrawMaterialQuad("BackWall-Unknown1",2480,120,2480,130,2970,740,2980,740,true);
  DrawMaterialQuad("BackWall-Unknown1",2480,310,2480,320,2820,740,2830,740,true);
  DrawMaterialQuad("BackWall-Unknown1",2480,500,2480,510,2670,740,2680,740,true);


  DrawMaterialQuad("BackWall-Unknown1",3400,120,3400,130,2910,740,2900,740,true);
  DrawMaterialQuad("BackWall-Unknown1",3400,310,3400,320,3060,740,3050,740,true);
  DrawMaterialQuad("BackWall-Unknown1",3400,500,3400,510,3210,740,3200,740,true);

  DrawMaterialQuad("BackWall-Unknown1",3520,120,3520,130,4010,740,4020,740,true);
  DrawMaterialQuad("BackWall-Unknown1",3520,310,3520,320,3870,740,3880,740,true);
  DrawMaterialQuad("BackWall-Unknown1",3520,500,3520,510,3720,740,3730,740,true);


  DrawMaterialQuad("BackWall-Unknown1",4520,120,4520,130,4030,740,4020,740,true);
  DrawMaterialQuad("BackWall-Unknown1",4520,310,4520,320,4180,740,4170,740,true);
  DrawMaterialQuad("BackWall-Unknown1",4520,500,4520,510,4330,740,4320,740,true);

  DrawMaterialQuad("BackWall-Unknown1",4640,120,4640,130,5130,740,5140,740,true);
  DrawMaterialQuad("BackWall-Unknown1",4640,310,4640,320,4980,740,4990,740,true);
  DrawMaterialQuad("BackWall-Unknown1",4640,500,4640,510,4830,740,4840,740,true);


  DrawMaterialQuad("Wall-Concrete3",1040,780,1070,790,1040,790,1040,780,true);
  DrawMaterialQuad("Wall-Concrete3",1140,790,1170,780,1170,790,1140,790,true);
  DrawMaterialQuad("Wall-Stripes",1240,740,1270,730,1270,740,1240,740,true);
  DrawMaterialQuad("Wall-Stripes",1240,940,1270,930,1270,940,1240,940,true);
  DrawMaterialQuad("Wall-Concrete3",1250,780,1280,790,1250,790,1250,780,true);
  DrawMaterialQuad("Wall-Concrete3",1320,790,1350,780,1350,790,1320,790,true);
  DrawMaterialQuad("Wall-Stripes",1330,730,1360,740,1330,740,1330,730,true);
  DrawMaterialQuad("Wall-Stripes",1330,930,1360,940,1330,940,1330,930,true);

  DrawMaterialQuad("Wall-Stripes",1750,740,1780,730,1780,740,1750,740,true);
  DrawMaterialQuad("Wall-Stripes",1820,730,1850,740,1820,740,1820,730,true);
  DrawMaterialQuad("Wall-Metal2",1880,1030,1970,1020,1970,1030,1880,1030,true);

  DrawMaterialQuad("Wall-Stripes",2380,740,2390,730,2450,730,2460,740,true);

  DrawMaterialQuad("Wall-Concrete3",2820,1060,2855,1070,2810,1070,2810,1060,true);
  DrawMaterialQuad("Wall-Concrete3",2850,860,2920,870,2850,870,2850,860,true);
  DrawMaterialQuad("Wall-Concrete3",2850,1370,2890,1380,2850,1380,2850,1370,true);
  DrawMaterialQuad("Wall-Concrete3",2870,1420,2930,1400,2950,1400,3010,1420,true);
  DrawMaterialQuad("Wall-Concrete3",2880,810,2900,800,2900,810,2880,810,true);
  DrawMaterialQuad("Wall-Concrete2",2880,820,3000,820,2960,830,2920,830,true);
  DrawMaterialQuad("Wall-Concrete3",2980,800,3000,810,2980,810,2980,800,true);
  DrawMaterialQuad("Wall-Concrete3",2960,870,3030,860,3030,870,2960,870,true);
  DrawMaterialQuad("Wall-Concrete3",2990,1380,3030,1370,3030,1380,2990,1380,true);
  DrawMaterialQuad("Wall-Concrete3",3025,1070,3060,1060,3060,1070,3025,1070,true);

  DrawMaterialQuad("Wall-Stripes",3420,740,3430,730,3490,730,3500,740,true);

  DrawMaterialQuad("Wall-Metal2",3910,1020,4000,1030,3910,1030,3910,1020,true);
  DrawMaterialQuad("Wall-Stripes",4030,740,4060,730,4060,740,4030,740,true);
  DrawMaterialQuad("Wall-Stripes",4100,730,4130,740,4100,740,4100,730,true);

  DrawMaterialQuad("Wall-Stripes",4520,740,4550,730,4550,740,4520,740,true);
  DrawMaterialQuad("Wall-Stripes",4610,730,4640,740,4610,740,4610,730,true);
  DrawMaterialQuad("Wall-Stripes",4520,940,4550,930,4550,940,4520,940,true);
  DrawMaterialQuad("Wall-Stripes",4610,930,4640,940,4610,940,4610,930,true);
  DrawMaterialQuad("Wall-Concrete3",4530,780,4560,790,4530,790,4530,780,true);
  DrawMaterialQuad("Wall-Concrete3",4600,790,4630,780,4630,790,4600,790,true);
  DrawMaterialQuad("Wall-Concrete3",4710,780,4740,790,4710,790,4710,780,true);
  DrawMaterialQuad("Wall-Concrete3",4810,790,4840,780,4840,790,4810,790,true);

  //Leitern
  CreateObject(LADR, 1030, 771, -1)->Set(3);
  CreateObject(LADR, 1165, 1451, -1)->Set(11);
  CreateObject(LADR, 1210, 771, -1)->Set(3);
  CreateObject(LADR, 1225, 1451, -1)->Set(3);
  CreateObject(LADR, 1300, 1386, -1)->Set(81);
  CreateObject(LADR, 1335, 1451, -1)->Set(3);
  CreateObject(LADR, 1370, 1011, -1)->Set(3);
  CreateObject(LADR, 1490, 1010, -1)->Set(4);
  CreateObject(LADR, 1800, 874, -1)->Set(17);

  CreateObject(LADR, 1740, 955, -1)->Set(27);
  CreateObject(LADR, 1860, 955, -1)->Set(26);

  CreateObject(LADR, 2285, 1451, -1)->Set(11);
  CreateObject(LADR, 2345, 1451, -1)->Set(3);
  CreateObject(LADR, 2370, 804, -1)->Set(7);
  CreateObject(LADR, 2455, 1451, -1)->Set(3);
  CreateObject(LADR, 2470, 804, -1)->Set(7);

  CreateObject(LADR, 2800, 1060, -1)->Set(40);
  CreateObject(LADR, 2890, 710, -1)->Set(15);
  CreateObject(LADR, 2940, 1022, -1)->Set(18);
  CreateObject(LADR, 2990, 710, -1)->Set(15);
  CreateObject(LADR, 3080, 1060, -1)->Set(40);

  CreateObject(LADR, 3425, 1451, -1)->Set(3);
  CreateObject(LADR, 3410, 804, -1)->Set(7);
  CreateObject(LADR, 3510, 804, -1)->Set(7);
  CreateObject(LADR, 3535, 1451, -1)->Set(3);
  CreateObject(LADR, 3595, 1451, -1)->Set(11);

  CreateObject(LADR, 4020, 955, -1)->Set(26);
  CreateObject(LADR, 4140, 955, -1)->Set(27);

  CreateObject(LADR, 4080, 874, -1)->Set(17);
  CreateObject(LADR, 4390, 1010, -1)->Set(4);
  CreateObject(LADR, 4510, 1011, -1)->Set(3);
  CreateObject(LADR, 4545, 1451, -1)->Set(3);
  CreateObject(LADR, 4580, 1386, -1)->Set(81);
  CreateObject(LADR, 4655, 1451, -1)->Set(3);
  CreateObject(LADR, 4670, 771, -1)->Set(3);
  CreateObject(LADR, 4715, 1451, -1)->Set(11);
  CreateObject(LADR, 4850, 771, -1)->Set(3);

  //Bodenluken
  CreateObject(HA4K, 1030, 743, -1);
  CreateObject(HA4K, 1210, 743, -1);
  CreateObject(HA4K, 1300, 733, -1);
  CreateObject(HA4K, 1300, 933, -1);
  CreateObject(HA4K, 1300, 983, -1);
  CreateObject(HA4K, 1370, 983, -1);

  CreateObject(HA4K, 1800, 733, -1);
  CreateObject(HA4K, 1860, 743, -1);

  CreateObject(HA4K, 2370, 743, -1);
  CreateObject(HA4K, 2470, 743, -1);

  CreateObject(HA4K, 2800, 863, -1);
  CreateObject(HA4K, 2880, 1043, -1);
  CreateObject(HA4K, 2940, 873, -1);
  CreateObject(HA4K, 3000, 1043, -1);
  CreateObject(HA4K, 3080, 863, -1);

  CreateObject(HA4K, 3410, 743, -1);
  CreateObject(HA4K, 3510, 743, -1);

  CreateObject(HA4K, 4020, 743, -1);
  CreateObject(HA4K, 4080, 733, -1);

  CreateObject(HA4K, 4510, 983, -1);
  CreateObject(HA4K, 4580, 733, -1);
  CreateObject(HA4K, 4580, 933, -1);
  CreateObject(HA4K, 4580, 983, -1);
  CreateObject(HA4K, 4670, 743, -1);
  CreateObject(HA4K, 4850, 743, -1);

  //Große Bodenluken
  CreateObject(H24K, 1225, 1428, -1);
  CreateObject(H24K, 1335, 1428, -1);
  CreateObject(H24K, 2345, 1428, -1);
  CreateObject(H24K, 2455, 1428, -1);
  CreateObject(H24K, 2855, 1428, -1);
  CreateObject(H24K, 3025, 1428, -1);
  CreateObject(H24K, 3425, 1428, -1);
  CreateObject(H24K, 3535, 1428, -1);
  CreateObject(H24K, 4545, 1428, -1);
  CreateObject(H24K, 4655, 1428, -1);

  //Gerüste
  CreateObject(SFFG, 1800, 735, -1)->Set(4);
  CreateObject(SFFG, 2910, 690, -1)->Set(5);
  CreateObject(SFFG, 2900, 740, -1)->Set(5);
  CreateObject(SFFG, 2940, 642, -1)->Set(4);
  CreateObject(SFFG, 2970, 690, -1)->Set(5);
  CreateObject(SFFG, 2980, 740, -1)->Set(5);
  CreateObject(SFFG, 4080, 735, -1)->Set(4);

  //Stahlbrücken
  CreateObject(_HBR, 1106, 802, -1)->SwitchMode();
  CreateObject(_HBR, 1844, 1042, -1)->SwitchMode();
  CreateObject(_HBR, 2754, 1072, -1);
  var bridge = CreateObject(_HBR, 2940, 1082, -1);
  bridge->SwitchMode();
  bridge->DoDmg(151);
  CreateObject(_HBR, 3127, 1072, -1);
  CreateObject(_HBR, 4034, 1042, -1)->SwitchMode();
  CreateObject(_HBR, 4774, 802, -1)->SwitchMode();

  //Steine
  CreateObject(STNE, 1080, 1600, -1);
  CreateObject(STNE, 1865, 1630, -1);
  CreateObject(STNE, 2510, 1610, -1);
  CreateObject(STNE, 3545, 1660, -1);
  CreateObject(STNE, 4845, 1630, -1);

  //Haie
  CreateObject(SHRK, 940, 1520, -1)->AutoRespawn();
  CreateObject(SHRK, 1660, 1520, -1)->AutoRespawn();

  CreateObject(SHRK, 2060, 1520, -1)->AutoRespawn();
  CreateObject(SHRK, 2780, 1520, -1)->AutoRespawn();

  CreateObject(SHRK, 3100, 1520, -1)->AutoRespawn();
  CreateObject(SHRK, 3820, 1520, -1)->AutoRespawn();

  CreateObject(SHRK, 4220, 1520, -1)->AutoRespawn();
  CreateObject(SHRK, 4940, 1520, -1)->AutoRespawn();

  //Explosivtanks
  CreateObject(XTNK, 2340, 1360, -1)->AutoRespawn();
  CreateObject(XTNK, 3540, 1360, -1)->AutoRespawn();

  //Kisten
  CreateObject(WCR2, 2860, 740, -1)->AutoRespawn();
  CreateObject(WCR2, 3020, 740, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 2350, 712, -1)->AutoRespawn();
  CreateObject(MWCR, 3530, 712, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 1190, 1420, -1)->AutoRespawn();
  CreateObject(BECR, 1280, 930, -1)->AutoRespawn();
  CreateObject(BECR, 1760, 900, -1)->AutoRespawn();
  CreateObject(BECR, 4120, 900, -1)->AutoRespawn();
  CreateObject(BECR, 4600, 930, -1)->AutoRespawn();
  CreateObject(BECR, 4690, 1420, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 2420, 559, -1)->AutoRespawn();
  CreateObject(XWCR, 3460, 559, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 2400, 559, -1);
  CreateObject(HBRL, 3480, 559, -1);

  //Gasflaschen
  CreateObject(GSBL, 2450, 402, -1);
  CreateObject(GSBL, 2400, 1420, -1);
  CreateObject(GSBL, 3480, 1420, -1);
  CreateObject(GSBL, 3430, 402, -1);

  //Phosphortanks
  CreateObject(PTNK, 2440, 820, -1)->AutoRespawn();
  CreateObject(PTNK, 2940, 1040, -1)->AutoRespawn();
  CreateObject(PTNK, 3440, 820, -1)->AutoRespawn();

  //Sandsackbarrieren
  CreateObject(SBBA, 1470, 1030, -1)->Right();
  CreateObject(SBBA, 1490, 1420, -1)->Right();
  CreateObject(SBBA, 1751, 1030, -1);

  CreateObject(SBBA, 2351, 820, -1);
  CreateObject(SBBA, 2385, 172, -1);
  CreateObject(SBBA, 2385, 353, -1);
  CreateObject(SBBA, 2455, 172, -1)->Right();
  CreateObject(SBBA, 2455, 353, -1)->Right();
  CreateObject(SBBA, 2490, 820, -1)->Right();
  CreateObject(SBBA, 2540, 1420, -1)->Right();
  CreateObject(SBBA, 2822, 740, -1);
  CreateObject(SBBA, 2822, 820, -1);
  CreateObject(SBBA, 3058, 740, -1)->Right();
  CreateObject(SBBA, 3058, 820, -1)->Right();
  CreateObject(SBBA, 3341, 1420, -1);
  CreateObject(SBBA, 3391, 820, -1);
  CreateObject(SBBA, 3425, 172, -1);
  CreateObject(SBBA, 3425, 353, -1);
  CreateObject(SBBA, 3495, 172, -1)->Right();
  CreateObject(SBBA, 3495, 353, -1)->Right();
  CreateObject(SBBA, 3530, 820, -1)->Right();

  CreateObject(SBBA, 4130, 1030, -1)->Right();
  CreateObject(SBBA, 4391, 1420, -1);
  CreateObject(SBBA, 4411, 1030, -1);

  //Hinweisschilder
  var sign = CreateObject(SNPT, 600, 740, -1);
  sign->SetCategory(C4D_StaticBack);
  sign->SetAction("Sign6");
  sign->Light();
  var sign = CreateObject(SNPT, 5290, 740, -1);
  sign->SetCategory(C4D_StaticBack);
  sign->SetAction("Sign6");
  sign->Light();

  //Schutztüren
  CreateObject(GDDR, 1355, 780, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 2840, 860, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 3040, 860, -1);
  CreateObject(GDDR, 4525, 780, -1);

  //Container
  CreateObject(CON1, 1290, 1421, -1)->SetPerspective(0);
  CreateObject(CON1, 2332, 741, -1);
  CreateObject(CON1, 2425, 1421, -1)->SetPerspective(2);
  CreateObject(CON1, 3456, 1421, -1)->SetPerspective(2);
  CreateObject(CON1, 3548, 741, -1);
  CreateObject(CON1, 4590, 1421, -1)->SetPerspective(0);

  //Seegras und Muscheln entfernen
  for(var obj in FindObjects(Find_Or(Find_ID(SWD1), Find_ID(SWD2), Find_ID(SHEL)), Find_InRect(0, 1350, LandscapeWidth(), 80)))
    RemoveObject(obj);

  //Wasseroberfläche ebnen
  DrawMaterialQuad("Water",1160,1460,2880,1460,2880,1461,1160,1461,1);
  DrawMaterialQuad("Water",3000,1460,4720,1460,4720,1461,3000,1461,1);

  //Grenzen setzen
  CreateObject(BRDR, 600, 0, -1)->Set(0);
  CreateObject(BRDR, 5290, 0, -1)->Set(1);
  CreateObject(BRDR, 0, 1530, -1)->Set(3);

  //Sprungschanzen
  CreateObject(JMPD, 2010, 1000, -1)->Set(132, 15);
  CreateObject(JMPD, 3870, 1000, -1)->Set(132, -15);

  //Seilrutschen
  CreateZipLine(1340, 690, 1720, 800);
  CreateZipLine(2460, 690, 2780, 766);

  CreateZipLine(2780, 820, 1840, 930);
  CreateZipLine(3100, 820, 4040, 930);

  CreateZipLine(3100, 766, 3420, 690);
  CreateZipLine(4160, 800, 4540, 690);

  //Tore und Konsolen
  var autod = CreateObject(HNG3, 2840, 820, -1);
  autod->Open();
  CreateObject(CONS, 2890, 860, -1)->Set(autod);
  autod = CreateObject(HNG3, 3040, 820, -1);
  autod->Open();
  CreateObject(CONS, 2990, 860, -1)->Set(autod);

  //Sendemäste
  var tower = CreateObject(AATR, 2420, 730, -1);
  tower->SwitchDestructability();
  tower = CreateObject(AATR, 3460, 730, -1);
  tower->SwitchDestructability();

  //Verbundene Räume
  aDoorWay[08] = CreateObject(GAT1, 2420, 320, -1);
  aDoorWay[09] = CreateObject(ROOM, 2420, 730, -1);
  aDoorWay[08]->Connect(aDoorWay[09]);

  aDoorWay[10] = CreateObject(ROOM, 2420, 353, -1);
  aDoorWay[11] = CreateObject(GAT1, 2420, 120, -1);
  aDoorWay[10]->Connect(aDoorWay[11]);

  aDoorWay[12] = CreateObject(GAT1, 3460, 320, -1);
  aDoorWay[13] = CreateObject(ROOM, 3460, 730, -1);
  aDoorWay[12]->Connect(aDoorWay[13]);

  aDoorWay[14] = CreateObject(ROOM, 3460, 353, -1);
  aDoorWay[15] = CreateObject(GAT1, 3460, 120, -1);
  aDoorWay[14]->Connect(aDoorWay[15]);

  aDoorWay[16] = CreateObject(ROOM, 1410, 1030, -1);
  aDoorWay[17] = CreateObject(ROOM, 1780, 1030, -1);
  aDoorWay[16]->Connect(aDoorWay[17]);

  aDoorWay[18] = CreateObject(ROOM, 4100, 1030, -1);
  aDoorWay[19] = CreateObject(ROOM, 4470, 1030, -1);
  aDoorWay[18]->Connect(aDoorWay[19]);

  aDoorWay[20] = CreateObject(GAT1, 2820, 1030, -1);
  aDoorWay[21] = CreateObject(GAT1, 2845, 1370, -1);
  aDoorWay[20]->Connect(aDoorWay[21]);

  aDoorWay[22] = CreateObject(GAT1, 3060, 1030, -1);
  aDoorWay[23] = CreateObject(GAT1, 3035, 1370, -1);
  aDoorWay[22]->Connect(aDoorWay[23]);

  //Soundkulisse

  //Rush Hour
  CreateObject(SE4K, 1000, 900, -1)->Set("Traffic*.ogg",300,100, 25,75);
  CreateObject(SE4K, 4890, 900, -1)->Set("Traffic*.ogg",300,100, 25,75);

  //Sirenen
  CreateObject(SE4K, 1000, 1000, -1)->Set("Siren*.ogg",2000,200, 75);
  CreateObject(SE4K, 4890, 1000, -1)->Set("Siren*.ogg",2000,200, 75);

  //Wind
  CreateObject(SE4K, 1000, 300, -1)->Set("WindSound*.ogg",400,100, 50,75);
  CreateObject(SE4K, 2940, 150, -1)->Set("WindSound*.ogg",500,100, 25,50);
  CreateObject(SE4K, 4890, 300, -1)->Set("WindSound*.ogg",400,100, 50,75);

  //Hallen
  CreateObject(SE4K, 1300, 350, -1)->Set("Interior*.ogg",700,100);
  CreateObject(SE4K, 1300, 1150, -1)->Set("InteriorStress*.ogg",1000,300, 75);
  CreateObject(SE4K, 2420, 350, -1)->Set("Interior*.ogg",700,100);
  CreateObject(SE4K, 2420, 1150, -1)->Set("InteriorMetal*.ogg",700,100, 75);
  CreateObject(SE4K, 3460, 350, -1)->Set("Interior*.ogg",700,100);
  CreateObject(SE4K, 3460, 1150, -1)->Set("InteriorMetal*.ogg",700,100, 75);
  CreateObject(SE4K, 4580, 350, -1)->Set("InteriorStress*.ogg",1000,300, 75);
  CreateObject(SE4K, 4580, 1150, -1)->Set("Interior*.ogg",700,100);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  CreateObject(AMCT, 1260, 980, -1)->Set(ABOX);
  CreateObject(AMCT, 2310, 1360, -1)->Set(ABOX);
  CreateObject(AMCT, 3570, 1360, -1)->Set(ABOX);
  CreateObject(AMCT, 4620, 980, -1)->Set(ABOX);

  //Versorgungskisten (Railgun)
  CreateObject(AMCT, 1820, 900, -1)->Set(RLGN,0,0,0,1);
  CreateObject(AMCT, 4060, 900, -1)->Set(RLGN,0,0,0,1);

  //Versorgungskisten (APW)
  CreateObject(AMCT, 1340, 980, -1)->Set(ATWN);
  CreateObject(AMCT, 4540, 980, -1)->Set(ATWN);

  //Versorgungskiste (Dragnin)
  CreateObject(AMCT, 2940, 1400, -1)->Set(DGNN);

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 1760, 975);
  PlaceSpawnpoint(GBOX, 4120, 975);

  //Raketen
  PlaceSpawnpoint(MBOX, 965, 775);
  PlaceSpawnpoint(MBOX, 4915, 775);

  //Geschützstellungen
  CreateObject(GNET, 1250, 1360, -1)->Set(SATW,90);
  CreateObject(GNET, 2380, 1360, -1)->Set(SATW);
  CreateObject(GNET, 2490, 820, -1)->Set(SBYW,90);
  CreateObject(GNET, 2800, 1420, -1)->Set(SBYW,-90);
  CreateObject(GNET, 3080, 1420, -1)->Set(SBYW,90);
  CreateObject(GNET, 3390, 820, -1)->Set(SBYW,-90);
  CreateObject(GNET, 3500, 1360, -1)->Set(SATW);
  CreateObject(GNET, 4630, 1360, -1)->Set(SATW,-90);

  //Patrouillenboote
  SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1140,1459,-1),50*21);
  SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1225,1459,-1),50*21);
  SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2845,1459,-1),50*21);
  SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3035,1459,-1),50*21);
  SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4655,1459,-1),50*21);
  SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4730,1459,-1),50*21);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Denkmal
  CreateObject(MUMT, 2940, 740, -1);

  //Leitkegel
  CreateObject(TCCN, 2905, 740, -1);
  CreateObject(TCCN, 2905, 592, -1);
  CreateObject(TCCN, 2974, 592, -1);
  CreateObject(TCCN, 2975, 740, -1);

  //Geländer
  CreateObject(RAI1, 1170, 1420, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1410, 1420, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1967, 980, -1);
  CreateObject(RAI3, 1995, 980, -1)->SetRail([3,1,3]);
  CreateObject(RAI1, 2028, 980, -1);
  CreateObject(RAI1, 2293, 1420, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2438, 1420, -1)->SetRail([1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2810, 1420, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 3010, 1420, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 3337, 1420, -1)->SetRail([1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 3503, 1420, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 3827, 980, -1);
  CreateObject(RAI3, 3855, 980, -1)->SetRail([3,1,3]);
  CreateObject(RAI1, 3888, 980, -1);
  CreateObject(RAI1, 4387, 1420, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 4623, 1420, -1)->SetRail([1,3,1,3,1,3,1]);

  //Lüftungsgitter
  CreateObject(ENGT, 1260, 1500, -1)->SetClrModulation(RGB(100,100,255));
  CreateObject(ENGT, 1260, 1530, -1)->SetClrModulation(RGB(100,100,255));
  CreateObject(ENGT, 1340, 1500, -1)->SetClrModulation(RGB(100,100,255));
  CreateObject(ENGT, 1340, 1530, -1)->SetClrModulation(RGB(100,100,255));

  CreateObject(ENGT, 2380, 1500, -1)->SetClrModulation(RGB(100,100,255));
  CreateObject(ENGT, 2380, 1530, -1)->SetClrModulation(RGB(100,100,255));
  CreateObject(ENGT, 2460, 1500, -1)->SetClrModulation(RGB(100,100,255));
  CreateObject(ENGT, 2460, 1530, -1)->SetClrModulation(RGB(100,100,255));

  CreateObject(ENGT, 2940, 800, -1);

  CreateObject(ENGT, 3420, 1500, -1)->SetClrModulation(RGB(100,100,255));
  CreateObject(ENGT, 3420, 1530, -1)->SetClrModulation(RGB(100,100,255));
  CreateObject(ENGT, 3500, 1500, -1)->SetClrModulation(RGB(100,100,255));
  CreateObject(ENGT, 3500, 1530, -1)->SetClrModulation(RGB(100,100,255));

  CreateObject(ENGT, 4540, 1500, -1)->SetClrModulation(RGB(100,100,255));
  CreateObject(ENGT, 4540, 1530, -1)->SetClrModulation(RGB(100,100,255));
  CreateObject(ENGT, 4620, 1500, -1)->SetClrModulation(RGB(100,100,255));
  CreateObject(ENGT, 4620, 1530, -1)->SetClrModulation(RGB(100,100,255));

  //Scheinwerfer
  CreateObject(FLGH, 1410, 1420, -1)->SetRotation(45);
  CreateObject(FLGH, 2410, 1360, -1)->SetRotation(45);
  CreateObject(FLGH, 3470, 1360, -1)->SetRotation(-45);
  CreateObject(FLGH, 4470, 1420, -1)->SetRotation(-45);

  //Zäune
  CreateObject(FENC, 30, 740, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 90, 740, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 150, 740, -1)->SetCategory(C4D_Background);

  CreateObject(FENC, 210, 980, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 270, 980, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 330, 980, -1)->SetCategory(C4D_Background);

  CreateObject(FENC, 390, 740, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 450, 740, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 510, 740, -1)->SetCategory(C4D_Background);

  CreateObject(FENC, 570, 980, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 630, 980, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 690, 980, -1)->SetCategory(C4D_Background);

  CreateObject(FENC, 750, 740, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 810, 740, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 870, 740, -1)->SetCategory(C4D_Background);

  CreateObject(FENC, 1040, 740, -1)->Set(2);
  CreateObject(FENC, 1100, 740, -1);
  CreateObject(FENC, 1160, 740, -1);

  CreateObject(FENC, 1390, 980, -1);
  CreateObject(FENC, 4490, 980, -1);

  CreateObject(FENC, 4720, 740, -1);
  CreateObject(FENC, 4780, 740, -1);
  CreateObject(FENC, 4840, 740, -1)->Set(2);

  CreateObject(FENC, 5020, 740, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 5080, 740, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 5140, 740, -1)->SetCategory(C4D_Background);

  CreateObject(FENC, 5200, 980, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 5260, 980, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 5320, 980, -1)->SetCategory(C4D_Background);

  CreateObject(FENC, 5380, 740, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 5440, 740, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 5500, 740, -1)->SetCategory(C4D_Background);

  CreateObject(FENC, 5560, 980, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 5620, 980, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 5680, 980, -1)->SetCategory(C4D_Background);

  CreateObject(FENC, 5740, 740, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 5800, 740, -1)->SetCategory(C4D_Background);
  CreateObject(FENC, 5860, 740, -1)->SetCategory(C4D_Background);

  //Schilder
  CreateObject(ESGN, 1010, 775, -1);
  CreateObject(MSGN, 1195, 980, -1);
  CreateObject(MSGN, 1475, 980, -1);
  CreateObject(ESGN, 1820, 975, -1);
  CreateObject(MSGN, 2775, 860, -1);
  CreateObject(MSGN, 3105, 860, -1);
  CreateObject(ESGN, 4060, 975, -1);
  CreateObject(MSGN, 4405, 980, -1);
  CreateObject(MSGN, 4685, 980, -1);
  CreateObject(ESGN, 4870, 775, -1);

  //Feuerlöscher
  CreateObject(FIE2, 980, 770, -1);
  CreateObject(FIE2, 990, 770, -1);
  CreateObject(FIE2, 1840, 970, -1);
  CreateObject(FIE2, 4040, 970, -1);
  CreateObject(FIE2, 4890, 770, -1);
  CreateObject(FIE2, 4900, 770, -1);

  //Poster
  CreateObject(PSTR, 1260, 900, -1)->Set(3,2);
  CreateObject(PSTR, 1785, 970, -1)->Set(3,1);
  CreateObject(PSTR, 2940, 870, -1)->Set(3,0);
  CreateObject(PSTR, 4095, 970, -1)->Set(3,1);
  CreateObject(PSTR, 4620, 900, -1)->Set(3,2);

  //Straßenlichter
  CreateObject(SLGH, 270, 740, -1)->TurnOff();
  CreateObject(SLGH, 450, 980, -1)->TurnOff();

  CreateObject(SLGH, 1230, 740, -1);
  CreateObject(SLGH, 1880, 740, -1);
  CreateObject(SLGH, 2490, 740, -1);

  CreateObject(SLGH, 2830, 740, -1);
  CreateObject(SLGH, 2860, 1040, -1);
  CreateObject(SLGH, 3020, 1040, -1);
  CreateObject(SLGH, 3050, 740, -1);

  CreateObject(SLGH, 3390, 740, -1);
  CreateObject(SLGH, 4000, 740, -1);
  CreateObject(SLGH, 4650, 740, -1);

  CreateObject(SLGH, 5440, 980, -1)->TurnOff();
  CreateObject(SLGH, 5620, 740, -1)->TurnOff();

  //Notausgangslichter
  CreateObject(ETLT, 1260, 705, -1);
  CreateObject(ETLT, 2460, 705, -1);
  CreateObject(ETLT, 3420, 705, -1);
  CreateObject(ETLT, 4620, 705, -1);

  //Orientierungslichter
  CreateObject(OLGH, 1120, 740, -1)->Set(8, 10, 1, 1, 10);
  CreateObject(OLGH, 1300, 120, -1)->Set(2, 30, 1, 1, 80);
  CreateObject(OLGH, 1300, 500, -1)->Set(2, 30, 1, 1, 80);
  CreateObject(OLGH, 1950, 740, -1)->Set(8, 10, 1, 1, 10);
  CreateObject(OLGH, 2420, 120, -1)->Set(2, 30, 1, 1, 80);
  CreateObject(OLGH, 2420, 500, -1)->Set(2, 30, 1, 1, 80);
  CreateObject(OLGH, 2940, 1040, -1)->Set(9, 10, 1, 1, 10);
  CreateObject(OLGH, 3460, 120, -1)->Set(2, 30, 1, 1, 80);
  CreateObject(OLGH, 3460, 500, -1)->Set(2, 30, 1, 1, 80);
  CreateObject(OLGH, 3930, 740, -1)->Set(8, 10, 1, 1, 10);
  CreateObject(OLGH, 4580, 120, -1)->Set(2, 30, 1, 1, 80);
  CreateObject(OLGH, 4580, 500, -1)->Set(2, 30, 1, 1, 80);
  CreateObject(OLGH, 4760, 740, -1)->Set(8, 10, 1, 1, 10);

  //Hinweisschilder
  CreateObject(WLSN, 1825, 860, -1)->Set("A");
  CreateObject(WLSN, 1845, 1025, -1)->Set("A");
  CreateObject(WLSN, 4035, 1025, -1)->Set("B");
  CreateObject(WLSN, 4055, 860, -1)->Set("B");

  //Banner
  var flag = CreateObject(BANR, 1230, 500, -1);
  flag->SetClrModulation(RGB(50,50,255));
  flag->SetCategory(C4D_StaticBack);
  flag = CreateObject(BANR, 1300, 1330, -1);
  flag->SetClrModulation(RGB(50,50,255));
  flag->SetCategory(C4D_StaticBack);
  flag = CreateObject(BANR, 1370, 500, -1);
  flag->SetClrModulation(RGB(50,50,255));
  flag->SetCategory(C4D_StaticBack);

  flag = CreateObject(BANR, 2350, 500, -1);
  flag->SetClrModulation(RGB(50,50,255));
  flag->SetCategory(C4D_StaticBack);
  flag = CreateObject(BANR, 2420, 1330, -1);
  flag->SetClrModulation(RGB(50,50,255));
  flag->SetCategory(C4D_StaticBack);
  flag = CreateObject(BANR, 2490, 500, -1);
  flag->SetClrModulation(RGB(50,50,255));
  flag->SetCategory(C4D_StaticBack);

  flag = CreateObject(BANR, 3390, 500, -1);
  flag->SetClrModulation(RGB(255,0,0));
  flag->SetCategory(C4D_StaticBack);
  flag = CreateObject(BANR, 3460, 1330, -1);
  flag->SetClrModulation(RGB(255,0,0));
  flag->SetCategory(C4D_StaticBack);
  flag = CreateObject(BANR, 3530, 500, -1);
  flag->SetClrModulation(RGB(255,0,0));
  flag->SetCategory(C4D_StaticBack);

  flag = CreateObject(BANR, 4510, 500, -1);
  flag->SetClrModulation(RGB(255,0,0));
  flag->SetCategory(C4D_StaticBack);
  flag = CreateObject(BANR, 4580, 1330, -1);
  flag->SetClrModulation(RGB(255,0,0));
  flag->SetCategory(C4D_StaticBack);
  flag = CreateObject(BANR, 4650, 500, -1);
  flag->SetClrModulation(RGB(255,0,0));
  flag->SetCategory(C4D_StaticBack);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD03,1300,250,-1);
  CreateObject(BD03,2420,350,-1);
  CreateObject(BD03,3460,200,-1);
  CreateObject(BD03,4580,300,-1);
  CreateObject(BD10,100,1460,-1);
  CreateObject(BD10,1490,1460,-1);
  CreateObject(BD10,2880,1460,-1);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(FindObject(GOCC))
  {
    if(pPoint == aFlag[0])
      aSelfDefense[0]->SetTeam(iTeam);

    if(pPoint == aFlag[6])
      aSelfDefense[1]->SetTeam(iTeam);
  }
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Enemy Approaching.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Helikopter und Hinweisschilder
  if(!FindObject(NOHC))
  {
    SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,1125,710,-1),100*21);
    SetupVehicleSpawn([APCE],DIR_Right,CreateObject(VSPW,1960,710,-1),100*21);
    SetupVehicleSpawn([APCE],DIR_Right,CreateObject(VSPW,2940,1010,-1),100*21,80);
    SetupVehicleSpawn([APCE],DIR_Left,CreateObject(VSPW,3920,710,-1),100*21);
    SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,4755,710,-1),100*21);

    var sign = CreateObject(SGNP, 1230, 740, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
    sign = CreateObject(SGNP, 1880, 740, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
    sign = CreateObject(SGNP, 4000, 740, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
    sign = CreateObject(SGNP, 4650, 740, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
  }

  //Verbundene Räume
  aDoorWay[00] = CreateObject(GAT1, 1340, 860, -1);
  aDoorWay[01] = CreateObject(GAT1, 1340, 1525, -1);
  aDoorWay[01]->SetClrModulation(RGB(125,125,205));
  aDoorWay[00]->Connect(aDoorWay[01]);

  aDoorWay[02] = CreateObject(GAT1, 2380, 680, -1);
  aDoorWay[03] = CreateObject(GAT1, 2380, 1525, -1);
  aDoorWay[03]->SetClrModulation(RGB(125,125,205));
  aDoorWay[02]->Connect(aDoorWay[03]);

  aDoorWay[04] = CreateObject(GAT1, 3500, 680, -1);
  aDoorWay[05] = CreateObject(GAT1, 3500, 1525, -1);
  aDoorWay[05]->SetClrModulation(RGB(125,125,205));
  aDoorWay[04]->Connect(aDoorWay[05]);

  aDoorWay[06] = CreateObject(GAT1, 4540, 860, -1);
  aDoorWay[07] = CreateObject(GAT1, 4540, 1525, -1);
  aDoorWay[07]->SetClrModulation(RGB(125,125,205));
  aDoorWay[06]->Connect(aDoorWay[07]);

  //OCC-Spielziel
  if(FindObject(GOCC))
  {
    //Flaggenposten
    aFlag[0] = CreateObject(OFPL,1440,1420,NO_OWNER);
    aFlag[0] -> AddSpawnPoint(1010,770);
    aFlag[0] -> AddSpawnPoint(1190,770);
    aFlag[0] -> AddSpawnPoint(1320,920);
    if(aTeams[1] == true)
    {
      aFlag[0]->Set("$Flag1$",0,4);
      aFlag[0]->Capture(1,1);
    }
    else
    {
      aFlag[0]->Set("$Flag1$");
    }

    aFlag[1] = CreateObject(OFPL,1935,980,NO_OWNER);
    aFlag[1] -> AddSpawnPoint(1780,675);
    aFlag[1] -> AddSpawnPoint(1780,720);
    aFlag[1] -> AddSpawnPoint(1900,730);
    aFlag[1]->Set("$Flag2$");

    aFlag[2] = CreateObject(OFPL,2445,354,NO_OWNER);
    aFlag[2] -> AddSpawnPoint(2310,1410);
    aFlag[2] -> AddSpawnPoint(2410,1350);
    aFlag[2] -> AddSpawnPoint(2500,1410);
    aFlag[2]->Set("$Flag3$");

    aFlag[3] = CreateObject(OFPL,2940,800,NO_OWNER);
    aFlag[3] -> AddSpawnPoint(2865,1060);
    aFlag[3] -> AddSpawnPoint(2905,1060);
    aFlag[3] -> AddSpawnPoint(2975,1060);
    aFlag[3] -> AddSpawnPoint(3015,1060);
    aFlag[3]->Set("$Flag4$");

    aFlag[4] = CreateObject(OFPL,3435,354,NO_OWNER);
    aFlag[4] -> AddSpawnPoint(3380,1410);
    aFlag[4] -> AddSpawnPoint(3470,1350);
    aFlag[4] -> AddSpawnPoint(3570,1410);
    aFlag[4]->Set("$Flag5$");

    aFlag[5] = CreateObject(OFPL,3945,980,NO_OWNER);
    aFlag[5] -> AddSpawnPoint(3980,730);
    aFlag[5] -> AddSpawnPoint(4100,675);
    aFlag[5] -> AddSpawnPoint(4100,720);
    aFlag[5]->Set("$Flag6$");

    aFlag[6] = CreateObject(OFPL,4440,1420,NO_OWNER);
    aFlag[6] -> AddSpawnPoint(4560,920);
    aFlag[6] -> AddSpawnPoint(4700,770);
    aFlag[6] -> AddSpawnPoint(4870,770);
    if(aTeams[2] == true)
    {
      aFlag[6]->Set("$Flag7$",0,4);
      aFlag[6]->Capture(2,1);
    }
    else
    {
      aFlag[6]->Set("$Flag7$");
    }

    //Selbstschussanlagen
    aSelfDefense[0] = CreateObject(SEGU, 1345, 735, -1);
    aSelfDefense[0]->Set(0,1,1,190,950,1750);
    aSelfDefense[0]->SetTeam(1);
    aSelfDefense[1] = CreateObject(SEGU, 4535, 735, -1);
    aSelfDefense[1]->Set(0,1,1,170,4130,4930);
    aSelfDefense[1]->SetTeam(2);
  }

  //BAS-Spielziel
  if(FindObject(GBAS))
  {
    //Strukturen
    AddAssaultTarget(PMP2, 1380, 1410, 400, 1, "$Target1$", 0, [[1180, 770], [1280, 720], [1320, 920]]);
    AddAssaultTarget(CMSN, 1800, 685, 400, 1, "$Target2$", 1, [[1800, 900], [1800, 980], [1950, 1020]]);
    AddAssaultTarget(PMP2, 2400, 820, 400, 1, "$Target3$", 2, [[2380, 1420], [2500, 1420]]);
    AddAssaultTarget(PMP2, 3480, 820, 400, 2, "$Target3$", 3, [[3380, 1420], [3500, 1420]]);
    AddAssaultTarget(CMSN, 4080, 685, 400, 2, "$Target2$", 4, [[3930, 1020], [4080, 980], [4080, 900]]);
    AddAssaultTarget(PMP2, 4500, 1410, 400, 2, "$Target1$", 5, [[4560, 920], [4600, 720], [4700, 770]]);

    //Sprengsatz-Spawn
    SetupBombSpawnpoint([[2940, 592],[2940, 800],[2940,1040],[2940,1400]]);

    //Selbstschussanlagen
    var selfdef = CreateObject(SEGU, 1345, 735, -1);
    selfdef->Set(0,1,1,190,950,1750);
    selfdef->SetTeam(1);
    selfdef = CreateObject(SEGU, 4535, 735, -1);
    selfdef->Set(0,1,1,170,4130,4930);
    selfdef->SetTeam(2);
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Script starten
    ScriptGo(1);
    aFlagPosition = 2;

    //Flaggenposten
    aFlag[0] = CreateObject(OFPL, 2940,800, -1);
    aFlag[0]->~Set("$Flag4$");

    //Teamgrenzen
    CreateObject(BRDR, 2030, 0, -1)->Set(0,1,0,1,1);
    CreateObject(BRDR, 3850, 0, -1)->Set(1,1,0,1,2);

    //Selbstschussanlagen
    var selfdef = CreateObject(SEGU, 1410, 980, -1);
    selfdef->Set(0,1,1,180,950,1750);
    selfdef->SetTeam(1);
    selfdef = CreateObject(SEGU, 4460, 980, -1);
    selfdef->Set(0,1,1,180,4130,4930);
    selfdef->SetTeam(2);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 2940; iY = 580;
    return 1;
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    if(iTeam == 1)
      return [[1180, 770], [1280, 720], [1320, 920]];
    if(iTeam == 2)
      return [[4560, 920], [4600, 720], [4700, 770]];
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
      aFlag[0]->MoveFlagpost(2940,800,"$Flag4$");
      aFlagPosition = 2;
    }
    else
    {
      aFlag[0]->MoveFlagpost(2940,1400,"$Flag9$");
      aFlagPosition = 3;
    }
  }
  else
  if(aFlagPosition == 2)
  {
    if(!Random(2))
    {
      aFlag[0]->MoveFlagpost(2940,592,"$Flag8$");
      aFlagPosition = 1;
    }
    else
    {
      aFlag[0]->MoveFlagpost(2940,1400,"$Flag9$");
      aFlagPosition = 3;
    }
  }
  else
  if(aFlagPosition == 3)
  {
    if(!Random(2))
    {
      aFlag[0]->MoveFlagpost(2940,592,"$Flag8$");
      aFlagPosition = 1;
    }
    else
    {
      aFlag[0]->MoveFlagpost(2940,800,"$Flag4$");
      aFlagPosition = 2;
    }
  }

  EventInfo4K(0,Format("$MsgFlagChanged$", GetName(aFlag[0])),SM21, 0, 0, 0, "Info_Objective.ogg");

  goto(0);
}