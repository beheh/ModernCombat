/*-- Operation Métro --*/

#strict
#include CSTD

static aFlag,aStationary;

func RecommendedGoals()			{return [GASS, GMNR];}	//Spielzielempfehlung
public func AssaultDefenderTeam()	{return 2;}		//Verteidigerteam bei Assault


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(0,15);
  //Flaggen
  aFlag = [];
  //Geschützstellungen
  aStationary = [];
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
  DrawMaterialQuad("WeakWall-Plate",760,241,760,331,850,241,760,241,true);
  DrawMaterialQuad("Wall-Plate",1290,370,1290,400,1260,370,1290,370,true);
  DrawMaterialQuad("Wall-Plate",1330,370,1330,300,1400,370,1330,370,true);
  DrawMaterialQuad("WeakWall-Plate",1410,240,1410,260,1390,240,1410,240,true);

  DrawMaterialQuad("Wall-Plate",1610,440,1610,370,1680,440,1610,440,true);
  DrawMaterialQuad("Wall-Plate",1470,500,1470,440,1530,500,1470,500,true);
  DrawMaterialQuad("Wall-Plate",1790,500,1790,440,1730,500,1790,500,true);
  DrawMaterialQuad("WeakWall-Plate",1760,260,1760,280,1740,260,1760,260,true);

  DrawMaterialQuad("WeakWall-Plate",2240,260,2240,280,2260,260,2240,260,true);
  DrawMaterialQuad("Wall-Plate",2390,440,2390,370,2320,440,2390,440,true);

  DrawMaterialQuad("Wall-Plate",2260,580,2260,440,2400,580,2260,580,true);
  DrawMaterialQuad("Wall-Plate",2480,620,2480,580,2520,620,2480,620,true);
  DrawMaterialQuad("Wall-Plate",2760,620,2760,580,2720,620,2760,620,true);
  DrawMaterialQuad("Wall-Plate",2980,580,2980,440,2840,580,2980,580,true);

  DrawMaterialQuad("Wall-Plate",2850,440,2850,370,2920,440,2850,440,true);
  DrawMaterialQuad("WeakWall-Plate",3000,260,3000,280,2980,260,3000,260,true);

  DrawMaterialQuad("Wall-Plate",3450,500,3450,440,3510,500,3450,500,true);
  DrawMaterialQuad("WeakWall-Plate",3480,260,3480,280,3500,260,3480,260,true);
  DrawMaterialQuad("Wall-Plate",3630,440,3630,370,3560,440,3630,440,true);
  DrawMaterialQuad("Wall-Plate",3770,500,3770,440,3710,500,3770,500,true);
  DrawMaterialQuad("WeakWall-Plate",3830,240,3830,260,3850,240,3830,240,true);
  DrawMaterialQuad("Wall-Plate",3910,370,3910,300,3840,370,3910,370,true);
  DrawMaterialQuad("Wall-Plate",3950,370,3950,400,3980,370,3950,370,true);
  DrawMaterialQuad("WeakWall-Plate",4480,240,4480,330,4390,240,4480,240,true);

  //Leitern
  CreateObject(LADR, 1790, 380, -1)->Set(9);
  CreateObject(LADR, 1930, 332, -1)->Set(3);
  CreateObject(LADR, 2070, 332, -1)->Set(3);
  CreateObject(LADR, 2210, 332, -1)->Set(3);

  CreateObject(LADR, 2545, 408, -1)->Set(4);
  CreateObject(LADR, 2695, 408, -1)->Set(4);

  CreateObject(LADR, 3030, 332, -1)->Set(3);
  CreateObject(LADR, 3170, 332, -1)->Set(3);
  CreateObject(LADR, 3310, 332, -1)->Set(3);
  CreateObject(LADR, 3450, 380, -1)->Set(9);

  //Bodenluken
  CreateObject(HA4K, 1790, 303, -1);
  CreateObject(HA4K, 1930, 303, -1);
  CreateObject(HA4K, 2070, 303, -1);
  CreateObject(HA4K, 2210, 303, -1);

  CreateObject(HA4K, 3030, 303, -1);
  CreateObject(HA4K, 3170, 303, -1);
  CreateObject(HA4K, 3310, 303, -1);
  CreateObject(HA4K, 3450, 303, -1);

  //Glasscheiben
  CreateObject(PANE, 522, 360, -1);
  CreateObject(PANE, 522, 389, -1);

  CreateObject(PANE, 848, 273, -1);
  CreateObject(PANE, 848, 302, -1);
  CreateObject(PANE, 848, 331, -1);
  CreateObject(PANE, 848, 360, -1);
  CreateObject(PANE, 848, 389, -1);

  CreateObject(PANE, 1032, 273, -1);
  CreateObject(PANE, 1032, 302, -1);
  CreateObject(PANE, 1032, 331, -1);
  CreateObject(PANE, 1032, 360, -1);
  CreateObject(PANE, 1032, 389, -1);

  CreateObject(PANE, 2048, 360, -1);
  CreateObject(PANE, 2048, 389, -1);

  CreateObject(PANE, 2232, 360, -1);
  CreateObject(PANE, 2232, 389, -1);

  CreateObject(PANE, 3008, 360, -1);
  CreateObject(PANE, 3008, 389, -1);

  CreateObject(PANE, 3192, 360, -1);
  CreateObject(PANE, 3192, 389, -1);

  CreateObject(PANE, 4208, 273, -1);
  CreateObject(PANE, 4208, 302, -1);
  CreateObject(PANE, 4208, 331, -1);
  CreateObject(PANE, 4208, 360, -1);
  CreateObject(PANE, 4208, 389, -1);

  CreateObject(PANE, 4392, 273, -1);
  CreateObject(PANE, 4392, 302, -1);
  CreateObject(PANE, 4392, 331, -1);
  CreateObject(PANE, 4392, 360, -1);
  CreateObject(PANE, 4392, 389, -1);

  CreateObject(PANE, 4718, 360, -1);
  CreateObject(PANE, 4718, 389, -1);

  //U-Bahnzüge
  CreateObject(MOTN, 480, 460, -1);
  CreateObject(MOTN, 2090, 460, -1);

  CreateObject(MOTN, 2570, 640, -1);
  CreateObject(MOTN, 2670, 640, -1);

  CreateObject(MOTN, 3150, 460, -1);
  CreateObject(MOTN, 4760, 460, -1);

  //Phosphorfässer
  CreateObject(HBRL, 1310, 300, -1)->AutoRespawn();
  CreateObject(HBRL, 2470, 580, -1)->AutoRespawn();
  CreateObject(HBRL, 2770, 580, -1)->AutoRespawn();
  CreateObject(HBRL, 3930, 300, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1540, 370, -1)->AutoRespawn();
  CreateObject(GSBL, 2425, 370, -1)->AutoRespawn();
  CreateObject(GSBL, 2815, 370, -1)->AutoRespawn();
  CreateObject(GSBL, 3700, 370, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 2500, 370, -1)->AutoRespawn();
  CreateObject(XWCR, 2740, 370, -1)->AutoRespawn();

  //Kisten
  CreateObject(WCR2, 770, 440, -1)->AutoRespawn();
  CreateObject(WCR2, 780, 422, -1);
  CreateObject(WCR2, 790, 440, -1)->AutoRespawn();
  CreateObject(WCR2, 1130, 300, -1);
  CreateObject(WCR2, 1230, 440, -1)->AutoRespawn();

  CreateObject(WCR2, 1700, 440, -1)->AutoRespawn();
  CreateObject(WCR2, 1710, 422, -1);
  CreateObject(WCR2, 3530, 440, -1)->AutoRespawn();
  CreateObject(WCR2, 3540, 422, -1);

  CreateObject(WCR2, 4010, 440, -1)->AutoRespawn();
  CreateObject(WCR2, 4110, 300, -1);
  CreateObject(WCR2, 4450, 440, -1)->AutoRespawn();
  CreateObject(WCR2, 4460, 422, -1);
  CreateObject(WCR2, 4470, 440, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 1550, 440, -1)->AutoRespawn();
  CreateObject(BECR, 1940, 390, -1);
  CreateObject(BECR, 3305, 390, -1);
  CreateObject(BECR, 3690, 440, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 1570, 440, -1)->AutoRespawn();
  CreateObject(MWCR, 1720, 440, -1)->AutoRespawn();
  CreateObject(MWCR, 3550, 440, -1)->AutoRespawn();
  CreateObject(MWCR, 3670, 440, -1)->AutoRespawn();

  //Paletten
  CreateObject(PLLT, 580, 440, -1);
  CreateObject(PLLT, 2460, 370, -1);
  CreateObject(PLLT, 2780, 370, -1);
  CreateObject(PLLT, 4660, 440, -1);

  //Gerüste
  CreateObject(SFFG, 1840, 440, -1)->Set(5);
  CreateObject(SFFG, 1900, 390, -1)->Set(3);
  CreateObject(SFFG, 1900, 440, -1)->Set(4);
  CreateObject(SFFG, 1960, 440, -1)->Set(5);
  CreateObject(SFFG, 2440, 530, -1)->Set(3);
  CreateObject(SFFG, 2440, 580, -1)->Set(4);

  CreateObject(SFFG, 2800, 530, -1)->Set(2);
  CreateObject(SFFG, 2800, 580, -1)->Set(4);
  CreateObject(SFFG, 3280, 440, -1)->Set(5);
  CreateObject(SFFG, 3340, 390, -1)->Set(2);
  CreateObject(SFFG, 3340, 440, -1)->Set(4);
  CreateObject(SFFG, 3400, 440, -1)->Set(5);

  //Stahlbrücken
  CreateObject(_HBR, 30, 360, -1);
  CreateObject(_HBR, 100, 360, -1);
  CreateObject(_HBR, 170, 360, -1);
  CreateObject(_HBR, 240, 360, -1);
  CreateObject(_HBR, 310, 360, -1);
  CreateObject(_HBR, 380, 360, -1);
  CreateObject(_HBR, 450, 360, -1);

  CreateObject(_HBR, 940, 330, -1);
  var bridge = CreateObject(_HBR, 1365, 350, -1);
  bridge->SetR(45);
  bridge->SetIrreparable();
  bridge = CreateObject(_HBR, 1500, 485, -1);
  bridge->SetR(45);
  bridge->SetIrreparable();
  bridge = CreateObject(_HBR, 1645, 420, -1);
  bridge->SetR(45);
  bridge->SetIrreparable();
  bridge = CreateObject(_HBR, 1760, 485, -1);
  bridge->SetR(-45);
  bridge->SetIrreparable();

  bridge = CreateObject(_HBR, 2305, 500, -1);
  bridge->SetR(45);
  bridge->SetIrreparable();
  bridge = CreateObject(_HBR, 2355, 550, -1);
  bridge->SetR(45);
  bridge->SetIrreparable();
  bridge = CreateObject(_HBR, 2355, 420, -1);
  bridge->SetR(-45);
  bridge->SetIrreparable();
  CreateObject(_HBR, 2585, 510, -1);
  CreateObject(_HBR, 2655, 510, -1);
  bridge = CreateObject(_HBR, 2885, 420, -1);
  bridge->SetR(45);
  bridge->SetIrreparable();
  bridge = CreateObject(_HBR, 2885, 550, -1);
  bridge->SetR(-45);
  bridge->SetIrreparable();
  bridge = CreateObject(_HBR, 2935, 500, -1);
  bridge->SetR(-45);
  bridge->SetIrreparable();

  bridge = CreateObject(_HBR, 3480, 485, -1);
  bridge->SetR(45);
  bridge->SetIrreparable();
  bridge = CreateObject(_HBR, 3595, 420, -1);
  bridge->SetR(-45);
  bridge->SetIrreparable();
  bridge = CreateObject(_HBR, 3740, 485, -1);
  bridge->SetR(-45);
  bridge->SetIrreparable();
  bridge = CreateObject(_HBR, 3875, 350, -1);
  bridge->SetR(-45);
  bridge->SetIrreparable();
  CreateObject(_HBR, 4300, 330, -1);

  CreateObject(_HBR, 4790, 360, -1);
  CreateObject(_HBR, 4860, 360, -1);
  CreateObject(_HBR, 4930, 360, -1);
  CreateObject(_HBR, 5000, 360, -1);
  CreateObject(_HBR, 5070, 360, -1);
  CreateObject(_HBR, 5140, 360, -1);
  CreateObject(_HBR, 5210, 360, -1);

  //Schutt
  CreateObject(DEB1, 860, 460, -1)->AddSmokeEffect4K(40,-2,-5,true,this);
  var derb = CreateObject(DEB1, 4380, 460, -1);
  derb->AddSmokeEffect4K(40,-2,-5,true,this);
  derb->SetDir(1);

  //Schutztüren
  CreateObject(GDDR, 1350, 440, -1)->Lock();
  CreateObject(GDDR, 3890, 440, -1)->Lock();

  //Verbundene Räume
  var doorw = CreateObject(ROOM, 1200, 300, -1);
  CreateObject(ROOM, 1200, 440, -1)->Connect(doorw);
  doorw = CreateObject(ROOM, 4040, 300, -1);
  CreateObject(ROOM, 4040, 440, -1)->Connect(doorw);

  //Sounds

  //Hallen
  CreateObject(SE4K, 400, 400, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1600, 400, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 3400, 400, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 4600, 400, -1)->Set("Interior*.ogg",665,105);

  //Metro
  CreateObject(SE4K, 1000, 400, -1)->Set("Metro*.ogg",800,150);
  CreateObject(SE4K, 2200, 400, -1)->Set("Metro*.ogg",800,150);
  CreateObject(SE4K, 2800, 400, -1)->Set("Metro*.ogg",800,150);
  CreateObject(SE4K, 4000, 400, -1)->Set("Metro*.ogg",800,150);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Raketen
  PlaceSpawnpoint(MBOX, 1860, 295);
  PlaceSpawnpoint(MBOX, 3380, 295);

  //Versorgungskisten (Kugeln)
  var crate = CreateObject (AMCT, 1120, 440, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 4120, 440, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 2600, 440, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  crate = CreateObject (AMCT, 1150, 440, -1);
  crate->Set(GBOX);
  crate = CreateObject (AMCT, 4090, 440, -1);
  crate->Set(GBOX);
  crate = CreateObject (AMCT, 2640, 440, -1);
  crate->Set(GBOX);

  //Versorgungskisten (Dragnin)
  crate = CreateObject (AMCT, 1930, 440, -1);
  crate->Set(DGNN);
  crate = CreateObject (AMCT, 3310, 440, -1);
  crate->Set(DGNN);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Topfpflanzen
  CreateObject(PLT3, 610, 440, -1);
  CreateObject(PLT3, 730, 440, -1);
  CreateObject(PLT3, 1170, 300, -1);
  CreateObject(PLT3, 1170, 440, -1);
  CreateObject(PLT3, 1290, 300, -1);
  CreateObject(PLT3, 1290, 440, -1);
  CreateObject(PLT3, 2410, 370, -1);
  CreateObject(PLT3, 2830, 370, -1);
  CreateObject(PLT3, 3950, 300, -1);
  CreateObject(PLT3, 3950, 440, -1);
  CreateObject(PLT3, 4070, 300, -1);
  CreateObject(PLT3, 4070, 440, -1);
  CreateObject(PLT3, 4510, 440, -1);
  CreateObject(PLT3, 4630, 440, -1);

  //Notausgangslichter
  CreateObject(ETLT,1150, 255, -1);
  CreateObject(ETLT,1150, 385, -1);
  CreateObject(ETLT,4090, 255, -1);
  CreateObject(ETLT,4090, 385, -1);

  //Automaten
  var vmachine = CreateObject(VGMN, 1480, 370, -1);
  vmachine->SetAction("Destroyed");
  vmachine->AddSmokeEffect4K(40,-2,-5,true,this);
  CreateObject(VGMN, 1520, 370, -1)->Set(3);
  vmachine = CreateObject(VGMN, 3720, 370, -1);
  vmachine->SetAction("Destroyed");
  vmachine->AddSmokeEffect4K(40,-2,-5,true,this);
  vmachine->Set(3);
  CreateObject(VGMN, 3760, 370, -1)->Set(2);

  //Schilder
  CreateObject(MSGN, 940, 440, -1);
  CreateObject(MSGN, 4300, 440, -1);

  //Geländer
  CreateObject(RAI1, 1120, 300, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1590, 370, -1);
  var rail = CreateObject(RAI1, 1610, 370, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 11; i++)
   rail->SetClrModulation(RGB(200, 200, 200), this, i);

  CreateObject(RAI1, 2240, 440, -1);
  rail = CreateObject(RAI1, 2260, 440, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1]);
  for(var i = 0; i <= 7; i++)
   rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 2392, 370, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2703, 370, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  rail = CreateObject(RAI1, 2920, 440, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1]);
  for(var i = 0; i <= 7; i++)
   rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 2980, 440, -1);

  rail = CreateObject(RAI1, 3530, 370, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 11; i++)
   rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 3630, 370, -1);
  CreateObject(RAI1, 3915, 300, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);

  //Deckenlichter
  CreateObject(CLGH, 550, 335, -1);
  CreateObject(CLGH, 730, 335, -1);

  CreateObject(CLGH, 1170, 245, -1);
  CreateObject(CLGH, 1170, 375, -1);
  CreateObject(CLGH, 1290, 245, -1);

  CreateObject(CLGH, 1810, 335, -1);
  CreateObject(CLGH, 1990, 335, -1)->TurnOff();
  CreateObject(CLGH, 3250, 335, -1)->TurnOff();
  CreateObject(CLGH, 3430, 335, -1);

  CreateObject(CLGH, 3950, 245, -1);
  CreateObject(CLGH, 4070, 245, -1);
  CreateObject(CLGH, 4070, 375, -1);

  CreateObject(CLGH, 4510, 335, -1);
  CreateObject(CLGH, 4690, 335, -1);

  //Wandlampen
  CreateObject(BLGH, 1680, 320, -1);
  CreateObject(BLGH, 2320, 320, -1);
  CreateObject(BLGH, 2440, 530, -1)->TurnOff();
  CreateObject(BLGH, 2500, 320, -1)->TurnOff();
  CreateObject(BLGH, 2740, 320, -1)->TurnOff();
  CreateObject(BLGH, 2800, 530, -1)->TurnOff();
  CreateObject(BLGH, 2920, 320, -1);
  CreateObject(BLGH, 3560, 320, -1);

  //Leitkegel
  CreateObject(TCCN, 545, 440, -1)->Light();
  CreateObject(TCCN, 615, 440, -1);
  CreateObject(TCCN, 750, 440, -1);
  CreateObject(TCCN, 810, 440, -1);
  CreateObject(TCCN, 1150, 300, -1);
  CreateObject(TCCN, 1460, 370, -1);
  CreateObject(TCCN, 1480, 370, -1);
  CreateObject(TCCN, 1500, 370, -1);
  CreateObject(TCCN, 1530, 440, -1)->Light();
  CreateObject(TCCN, 1675, 430, -1)->SetR(40);
  CreateObject(TCCN, 1735, 440, -1);

  CreateObject(TCCN, 3510, 440, -1);
  CreateObject(TCCN, 3565, 430, -1)->SetR(-40);
  CreateObject(TCCN, 3700, 370, -1);
  CreateObject(TCCN, 3710, 440, -1)->Light();
  CreateObject(TCCN, 3720, 370, -1);
  CreateObject(TCCN, 3740, 370, -1);
  CreateObject(TCCN, 4090, 300, -1);
  CreateObject(TCCN, 4430, 440, -1);
  CreateObject(TCCN, 4490, 440, -1);
  CreateObject(TCCN, 4625, 440, -1);
  CreateObject(TCCN, 4695, 440, -1)->Light();

  //Feuerlöscher
  CreateObject(FIE2, 2290, 435, -1);
  CreateObject(FIE2, 2950, 435, -1);

  //Bildschirme
  CreateObject(SCR3, 490, 380, -1)->SetClrModulation(RGB(250,10,10));
  var screen = CreateObject(SCR3, 875, 380, -1);
  screen->SetClrModulation(RGB(250,10,10));
  screen->SetPhase(5);
  screen = CreateObject(SCR3, 1005, 380, -1);
  screen->SetClrModulation(RGB(250,10,10));
  screen->SetPhase(10);
  CreateObject(SCR3, 1440, 310, -1)->SetClrModulation(RGB(250,10,10));
  CreateObject(SCA2, 1500, 320, -1)->SetAction("Wompf");
  screen = CreateObject(SCR3, 1560, 310, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen->SetPhase(5);
  CreateObject(SCA2, 1590, 500, -1)->SetAction("News");
  CreateObject(SCA2, 1650, 500, -1)->SetAction("Clonk");
  screen = CreateObject(SCR3, 2075, 380, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen->SetPhase(5);
  CreateObject(SCR3, 2205, 380, -1)->SetClrModulation(RGB(250,10,10));

  CreateObject(SCA1, 2440, 340, -1)->SetAction("Grenade");
  CreateObject(SCR3, 2510, 560, -1)->SetClrModulation(RGB(250,200,10));
  CreateObject(SCR3, 2730, 560, -1)->SetClrModulation(RGB(250,200,10));
  CreateObject(SCA1, 2800, 340, -1)->SetAction("Medic");

  CreateObject(SCR3, 3035, 380, -1)->SetClrModulation(RGB(250,10,10));
  screen = CreateObject(SCR3, 3165, 380, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen->SetPhase(5);
  CreateObject(SCA2, 3590, 500, -1)->SetAction("Wompf");
  CreateObject(SCA2, 3650, 500, -1)->SetAction("News");
  CreateObject(SCR3, 3680, 310, -1)->SetClrModulation(RGB(250,10,10));
  CreateObject(SCA2, 3740, 320, -1)->SetAction("Clonk");
  screen = CreateObject(SCR3, 3800, 310, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen->SetPhase(10);
  var screen = CreateObject(SCR3, 4235, 380, -1);
  screen->SetClrModulation(RGB(250,10,10));
  screen->SetPhase(5);
  CreateObject(SCR3, 4365, 380, -1);
  CreateObject(SCR3, 4750, 380, -1)->SetClrModulation(RGB(250,10,10));

  //Büsche
  var bush = CreateObject(BSH2, 1700, 500, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  bush = CreateObject(BSH2, 1720, 500, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  CreateObject(BSH2, 1830, 440, -1);
  CreateObject(BSH2, 1850, 440, -1);
  bush = CreateObject(BSH2, 1890, 440, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  bush = CreateObject(BSH2, 1910, 440, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  bush = CreateObject(BSH2, 1950, 440, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  bush = CreateObject(BSH2, 1970, 440, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);

  CreateObject(BSH2, 2430, 580, -1);
  CreateObject(BSH2, 2450, 580, -1);
  CreateObject(BSH2, 2790, 580, -1);
  CreateObject(BSH2, 2810, 580, -1);

  bush = CreateObject(BSH2, 3270, 440, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  bush = CreateObject(BSH2, 3290, 440, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  bush = CreateObject(BSH2, 3330, 440, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  bush = CreateObject(BSH2, 3350, 440, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  CreateObject(BSH2, 3390, 440, -1);
  CreateObject(BSH2, 3410, 440, -1);
  bush = CreateObject(BSH2, 3520, 500, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  bush = CreateObject(BSH2, 3540, 500, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);

  //Hinweisschilder
  CreateObject(SGNP, 420, 460, -1)->SetMode(1);
  CreateObject(WLSN, 640, 400, -1)->Set("A");
  CreateObject(WLSN, 1080, 400, -1)->Set("A");
  CreateObject(WLSN, 1960, 390, -1)->Set("B");
  CreateObject(WLSN, 2320, 390, -1)->Set("B");
  CreateObject(WLSN, 2920, 390, -1)->Set("C");
  CreateObject(WLSN, 3280, 390, -1)->Set("C");
  CreateObject(WLSN, 4160, 400, -1)->Set("D");
  CreateObject(WLSN, 4600, 400, -1)->Set("D");
  CreateObject(SGNP, 4820, 460, -1)->SetMode(1);

  //Labortische
  CreateObject(LTBL, 1400, 440, -1);
  CreateObject(LTBL, 1440, 440, -1);
  CreateObject(LTBL, 3800, 440, -1);
  CreateObject(LTBL, 3840, 440, -1);

  //Monitore
  CreateObject(MNI2, 1400, 428, -1);
  CreateObject(MNI2, 3840, 428, -1);

  //Ventillatoren
  CreateObject(VEN3, 1790, 300, -1)->SetCon(15);
  CreateObject(VEN3, 1930, 300, -1)->SetCon(15);
  CreateObject(VEN3, 2070, 300, -1)->SetCon(15);
  CreateObject(VEN3, 2210, 300, -1)->SetCon(15);

  CreateObject(VEN3, 3030, 300, -1)->SetCon(15);
  CreateObject(VEN3, 3170, 300, -1)->SetCon(15);
  CreateObject(VEN3, 3310, 300, -1)->SetCon(15);
  CreateObject(VEN3, 3450, 300, -1)->SetCon(15);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Eurocorps.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,1420,370,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(1020, 450);
   aFlag[0] -> AddSpawnPoint(1160, 290);
   aFlag[0] -> AddSpawnPoint(1290, 430);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,2190,460,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(1880,380);
   aFlag[1] -> AddSpawnPoint(1880,430);
   aFlag[1] -> AddSpawnPoint(2410,570);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,3050,460,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(3360,380);
   aFlag[2] -> AddSpawnPoint(3360,430);
   aFlag[2] -> AddSpawnPoint(2830,570);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,3820,370,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(4220, 450);
   aFlag[3] -> AddSpawnPoint(4080, 290);
   aFlag[3] -> AddSpawnPoint(3950, 430);
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
   CreateObject(BRDR, 520, 0, -1)->Set(0);
   CreateObject(BRDR, 4720, 0, -1)->Set(1);

   //Geschützstellungen
   CreateObject(GNET, 830, 440, -1)->Set(0,90,1);
   CreateObject(GNET, 1600, 370, -1)->Set(0,90,1);
   CreateObject(GNET, 3640, 370, -1)->Set(0,-90,1);
   CreateObject(GNET, 4410, 440, -1)->Set(0,-90,1);

   //Versorgungskisten (APW)
   crate = CreateObject (AMCT, 1650, 500, -1);
   crate->Set(ATWN);
   crate = CreateObject (AMCT, 3590, 500, -1);
   crate->Set(ATWN);
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {
   //Zielobjekte
   AddAssaultTarget(CMSN, 1900, 440, 30*30, 2, "$Target1$", 0, [[[2570, 440], [2440, 530], [2410, 580]], [[1170, 300], [1170, 440], [1290, 440]]]);
   AddAssaultTarget(CMSN, 2620, 370, 30*30, 2, "$Target1$", 1, [[[3020, 460], [3160, 440], [3220, 440]], [[1800, 440], [1870, 390], [1590, 370]]]);
   AddAssaultTarget(CMSN, 2670, 620, 30*30, 2, "$Target2$", 2, [[[3020, 460], [3160, 440], [3220, 440]], [[1800, 440], [1870, 390], [1590, 370]]]);
   AddAssaultTarget(CMSN, 3680, 370, 30*30, 2, "$Target1$", 3, [[[4160, 440], [4220, 460], [4340, 460]], [[2780, 530], [2570, 440], [2620, 620]]]);
   AddAssaultTarget(CMSN, 3800, 440, 30*30, 2, "$Target2$", 4, [[[4160, 440], [4220, 460], [4340, 460]], [[2780, 530], [2570, 440], [2620, 620]]]);

   //Ziele verbinden
   ConnectAssaultTargets([1, 2]);
   ConnectAssaultTargets([3, 4]);

   //Grenzen setzen
   CreateObject(BRDR, 520, 0, -1)->Set(0);
   CreateObject(BRDR, 2780, 0, -1)->Set(1,1);

   //Gerüste
   CreateObject(SFFG, 2960, 390, -1)->Set(4);
   CreateObject(SFFG, 2960, 440, -1)->Set(5);

   //Metallkiste
   CreateObject(MWCR, 2990, 340, -1)->AutoRespawn();

   //Boden
   DrawMaterialQuad("BackWall-Stripes", 2590,370, 2650,370, 2650,380, 2590,380);

   //Objekte entfernen
   RemoveObject(FindObject2(Find_ID(AMCT),Find_InRect(1910, 420, 30, 30)));
   RemoveObject(FindObject2(Find_ID(AMCT),Find_InRect(4590, 420, 30, 30)));
   RemoveObject(FindObject2(Find_ID(LTBL),Find_InRect(3780, 410, 30, 30)));

   //Geschützstellungen
   aStationary[0] = CreateObject(GNET, 830, 440, -1);
   aStationary[0] -> Set(0,90,1);
   aStationary[1] = CreateObject(GNET, 2770, 530, -1);
   aStationary[1] -> Set(0,-90);
   aStationary[2] = CreateObject(GNET, 3100, 440, -1);
   aStationary[2] -> Set(0,-90);

   //Sandsackbarrieren
   CreateObject(SBBA, 2080, 408, -1);
   CreateObject(SBBA, 2400, 370, -1);
   CreateObject(SBBA, 2560, 588, -1);
   CreateObject(SBBA, 2660, 588, -1);
   CreateObject(SBBA, 3140, 408, -1);
   CreateObject(SBBA, 3640, 500, -1);
   CreateObject(SBBA, 3920, 300, -1);

   //Phosphorfässer
   CreateObject(HBRL, 3520, 500, -1);
   CreateObject(HBRL, 3530, 500, -1);
   CreateObject(HBRL, 3590, 500, -1);

   //Metallkisten
   CreateObject(MWCR, 1570, 370, -1);
   CreateObject(MWCR, 1625, 482, -1);
   CreateObject(MWCR, 1630, 500, -1);
   CreateObject(MWCR, 2010, 440, -1);
   CreateObject(MWCR, 3250, 440, -1);
   CreateObject(MWCR, 3640, 370, -1);
   CreateObject(MWCR, 3990, 440, -1);

   //Versorgungskiste (APW)
   var crate = CreateObject (AMCT, 3920, 440, -1);
   crate->Set(ATWN);
  }

  //Base Assault-Spielziel
  if(FindObject(GBAS))
  {
   //Zielobjekte
   AddAssaultTarget(CMSN, 1560, 370, 300, 1, "$Target2$", 0, [[1050, 440], [1290, 440], [1470, 440]]);
   AddAssaultTarget(CMSN, 2090, 440, 300, 1, "$Target1$", 1, [[1840, 300], [1870, 390], [1955, 300]]);
   AddAssaultTarget(CMSN, 3150, 440, 300, 2, "$Target1$", 2, [[3285, 300], [3370, 390], [3400, 300]]);
   AddAssaultTarget(CMSN, 3680, 370, 300, 2, "$Target2$", 3, [[3770, 440], [3950, 440], [4070, 440]]);

   //Sprengsatz-Spawn
   SetupBombSpawnpoint([[2620, 440],[2620,620]]);

   //Grenzen setzen
   CreateObject(BRDR, 950, 0, -1)->Set(0);
   CreateObject(BRDR, 4290, 0, -1)->Set(1);
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 2620, 620, -1);
   flag->~Set("$Flag5$");

   //Grenzen setzen
   CreateObject(BRDR, 950, 0, -1)->Set(0);
   CreateObject(BRDR, 4290, 0, -1)->Set(1);

   //Teamgrenzen
   CreateObject(BRDR, 1810, 0, -1)->Set(0,1,0,1,1);
   CreateObject(BRDR, 3430, 0, -1)->Set(1,1,0,1,2);

   //Geschützstellungen
   CreateObject(GNET, 1600, 370, -1)->Set(0,90,1);
   CreateObject(GNET, 3640, 370, -1)->Set(0,-90,1);

   //Versorgungskisten (APW)
   crate = CreateObject (AMCT, 1650, 500, -1);
   crate->Set(ATWN);
   crate = CreateObject (AMCT, 3590, 500, -1);
   crate->Set(ATWN);
  }

  //MR-Spielziel
  if(FindObject(GMNR))
  {
   //Geldsäcke
   AddMoneySpawn(2040, 435, [20]);
   AddMoneySpawn(2620, 435, [20]);
   AddMoneySpawn(2620, 615, [20]);
   AddMoneySpawn(3200, 435, [20]);

   //Grenzen setzen
   CreateObject(BRDR, 950, 0, -1)->Set(0);
   CreateObject(BRDR, 4290, 0, -1)->Set(1);

   //Teamgrenzen
   CreateObject(BRDR, 1810, 0, -1)->Set(0,1,0,1,1);
   CreateObject(BRDR, 3430, 0, -1)->Set(1,1,0,1,2);

   //Geschützstellungen
   CreateObject(GNET, 1600, 370, -1)->Set(0,90,1);
   CreateObject(GNET, 3640, 370, -1)->Set(0,-90,1);

   //Versorgungskisten (APW)
   crate = CreateObject (AMCT, 1650, 500, -1);
   crate->Set(ATWN);
   crate = CreateObject (AMCT, 3590, 500, -1);
   crate->Set(ATWN);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,1960,440,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,3280,440,GetTeamColor(2));}

   //Grenzen setzen
   CreateObject(BRDR, 1030, 0, -1)->Set(0);
   CreateObject(BRDR, 4210, 0, -1)->Set(1);

   //Geschützstellungen
   CreateObject(GNET, 1600, 370, -1)->Set(0,90,1);
   CreateObject(GNET, 3640, 370, -1)->Set(0,-90,1);

   //Versorgungskisten (APW)
   crate = CreateObject (AMCT, 1650, 500, -1);
   crate->Set(ATWN);
   crate = CreateObject (AMCT, 3590, 500, -1);
   crate->Set(ATWN);
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   //Grenzen setzen
   CreateObject(BRDR, 520, 0, -1)->Set(0);
   CreateObject(BRDR, 4720, 0, -1)->Set(1);

   //Geschützstellungen
   CreateObject(GNET, 1600, 370, -1)->Set(0,90,1);
   CreateObject(GNET, 3640, 370, -1)->Set(0,-90,1);

   //Versorgungskisten (APW)
   crate = CreateObject (AMCT, 1650, 500, -1);
   crate->Set(ATWN);
   crate = CreateObject (AMCT, 3590, 500, -1);
   crate->Set(ATWN);
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
   CreateObject(BRDR, 520, 0, -1)->Set(0);
   CreateObject(BRDR, 3430, 0, -1)->Set(1,1);

   //Teamgrenze setzen
   FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),2130,0,1,1);
  }

  //Ziel 2 und 3
  if(iIndex == 1 || iIndex == 2)
  {
   if(fConnectedDestroyed)
   {
    //Grenzen neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 1610, 0, -1)->Set(0);
    CreateObject(BRDR, 4720, 0, -1)->Set(1,1);

    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),3000,0,1,1);

    //Geschützstellung entfernen
    aStationary[1]->DecoExplode(30);
   }
  }

  //Ziel 3 und 4
  if(iIndex == 3 || iIndex == 4)
  {
   if(fConnectedDestroyed)
   {
    //Geschützstellung entfernen
    aStationary[2]->DecoExplode(30);
   }
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 1110; iY = 320;
   return(1);
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {if(FindObject(GASS)->GetRespawnPoint(iX, iY, iTeam)) return 1;}

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(iTeam == 1)
   {
    return [[1560, 490], [1580, 360], [1680, 490]];
   }
   if(iTeam == 2)
   {
    return [[3560, 490], [3660, 360], [3680, 490]];
   }
   return(1);
  }

  //HTF/MR/LMS/DM-Spielziel
  if(FindObject(GHTF) || FindObject(GMNR) || FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    return [[1290, 430], [1470, 360], [1470, 430]];
   }
   if(iTeam == 2)
   {
    return [[3770, 360], [3770, 430], [3950, 430]];
   }
   return(1);
  }
}