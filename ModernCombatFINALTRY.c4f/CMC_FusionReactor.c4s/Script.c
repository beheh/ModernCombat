/*-- Fusion Reactor --*/

#strict 2
#include CSTD

static aFlag,aSelfDefense,aDoorWay;

func RecommendedGoals()	{return [GOCC, GHTF];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(0,20,15);
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
  return 1;
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Rampen
  DrawMaterialQuad("Wall-Concrete1",400,330,400,300,460,330,400,330,true);
  DrawMaterialQuad("Wall-PlateBlue",410,930,410,880,460,930,410,930,true);

  DrawMaterialQuad("Wall-Concrete2",640,380,640,440,670,380,640,380,true);
  DrawMaterialQuad("Wall-Concrete2",640,750,640,690,670,750,640,750,true);

  DrawMaterialQuad("Wall-Concrete2",670,310,670,380,750,310,670,310,true);
  DrawMaterialQuad("Wall-Concrete2",670,820,670,750,750,820,670,820,true);

  DrawMaterialQuad("Wall-Concrete2",770,260,770,300,880,260,770,260,true);
  DrawMaterialQuad("Wall-Concrete2",770,870,770,830,880,870,770,870,true);

  DrawMaterialQuad("Wall-Concrete2",880,250,880,260,900,250,880,250,true);
  DrawMaterialQuad("Wall-Concrete2",880,880,880,870,900,880,880,880,true);

  DrawMaterialQuad("Wall-Concrete1",860,130,860,190,880,130,860,130,true);
  DrawMaterialQuad("Wall-Concrete1",860,940,860,1000,880,940,860,940,true);

  DrawMaterialQuad("Wall-Concrete1",970,130,970,190,950,130,970,130,true);
  DrawMaterialQuad("Wall-Concrete1",970,940,970,1000,950,940,970,940,true);

  DrawMaterialQuad("Wall-Concrete2",950,250,950,260,930,250,950,250,true);
  DrawMaterialQuad("Wall-Concrete2",950,880,950,870,930,880,950,880,true);

  DrawMaterialQuad("Wall-Concrete2",1060,260,1060,300,950,260,1060,260,true);
  DrawMaterialQuad("Wall-Concrete2",1060,870,1060,830,950,870,1060,870,true);

  DrawMaterialQuad("Wall-Concrete2",1160,310,1160,380,1080,310,1160,310,true);
  DrawMaterialQuad("Wall-Concrete2",1160,820,1160,750,1080,820,1160,820,true);

  DrawMaterialQuad("Wall-Concrete2",1190,380,1190,440,1160,380,1190,380,true);
  DrawMaterialQuad("Wall-Concrete2",1190,750,1190,690,1160,750,1190,750,true);

  DrawMaterialQuad("Wall-Concrete1",1430,330,1430,300,1370,330,1430,330,true);
  DrawMaterialQuad("Wall-PlateYellow",1420,930,1420,880,1370,930,1420,930,true);

  //Leitern
  CreateObject(LADR, 425, 860, -1)->Set(33);
  CreateObject(LADR, 475, 904, -1)->Set(71);
  CreateObject(LADR, 535, 576, -1)->Set(30);

  CreateObject(LADR, 655, 328, -1)->Set(9);
  CreateObject(LADR, 655, 922, -1)->Set(7);

  CreateObject(LADR, 760, 930, -1)->Set(84);
  CreateObject(LADR, 885, 638, -1)->Set(5);

  CreateObject(LADR, 915, 300, -1)->Set(8);
  CreateObject(LADR, 915, 929, -1)->Set(29);
  CreateObject(LADR, 945, 638, -1)->Set(5);
  CreateObject(LADR, 1070, 930, -1)->Set(84);

  CreateObject(LADR, 1175, 328, -1)->Set(9);
  CreateObject(LADR, 1175, 922, -1)->Set(7);

  CreateObject(LADR, 1295, 576, -1)->Set(30);
  CreateObject(LADR, 1355, 904, -1)->Set(71);
  CreateObject(LADR, 1405, 860, -1)->Set(33);

  //Bodenluken
  CreateObject(HA4K, 760, 253, -1);
  CreateObject(HA4K, 760, 593, -1);
  CreateObject(HA4K, 760, 693, -1);
  CreateObject(HA4K, 760, 833, -1);

  CreateObject(HA4K, 1070, 253, -1);
  CreateObject(HA4K, 1070, 593, -1);
  CreateObject(HA4K, 1070, 693, -1);
  CreateObject(HA4K, 1070, 833, -1);

  //Große Bodenluken
  CreateObject(H24K, 425, 598, -1);
  CreateObject(H24K, 425, 800, -1);

  CreateObject(H24K, 475, 338, -1);
  CreateObject(H24K, 475, 528, -1);
  CreateObject(H24K, 475, 598, -1);
  CreateObject(H24K, 475, 800, -1);

  CreateObject(H24K, 535, 338, -1);
  CreateObject(H24K, 535, 528, -1);

  CreateObject(H24K, 655, 258, -1);
  CreateObject(H24K, 655, 868, -1);

  CreateObject(H24K, 915, 238, -1);
  CreateObject(H24K, 915, 698, -1);
  CreateObject(H24K, 915, 888, -1);

  CreateObject(H24K, 1175, 258, -1);
  CreateObject(H24K, 1175, 868, -1);

  CreateObject(H24K, 1295, 338, -1);
  CreateObject(H24K, 1295, 528, -1);

  CreateObject(H24K, 1355, 338, -1);
  CreateObject(H24K, 1355, 528, -1);
  CreateObject(H24K, 1355, 598, -1);
  CreateObject(H24K, 1355, 800, -1);

  CreateObject(H24K, 1405, 598, -1);
  CreateObject(H24K, 1405, 800, -1);

  //Hydrauliktüren
  var door = CreateObject(SLDR, 30, 300, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  door = CreateObject(SLDR, 30, 880, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  door = CreateObject(SLDR, 1800, 300, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  door = CreateObject(SLDR, 1800, 880, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  //Schutztüren
  CreateObject(GDDR, 290, 880, -1)->Lock();
  CreateObject(GDDR, 350, 300, -1)->Lock();
  CreateObject(GDDR, 830, 230, -1)->SetColorDw(HSL(80, 150, 100, 127));
  CreateObject(GDDR, 830, 930, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1000, 230, -1);
  CreateObject(GDDR, 1000, 930, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 1480, 300, -1)->Lock();
  CreateObject(GDDR, 1540, 880, -1)->Lock();

  //Container
  CreateObject(CON1, 90, 528, -1)->SetPerspective(1);
  CreateObject(CON1, 90, 556, -1)->SetPerspective(1);
  CreateObject(CON1, 90, 584, -1)->SetPerspective(1);
  CreateObject(CON1, 90, 612, -1)->SetPerspective(1);
  CreateObject(CON1, 90, 640, -1)->SetPerspective(1);
  CreateObject(CON1, 145, 528, -1)->SetPerspective();
  CreateObject(CON1, 145, 556, -1)->SetPerspective();
  CreateObject(CON1, 145, 584, -1)->SetPerspective();
  CreateObject(CON1, 145, 612, -1)->SetPerspective();
  CreateObject(CON1, 145, 640, -1)->SetPerspective();
  CreateObject(CON1, 200, 528, -1)->SetPerspective(3);
  CreateObject(CON1, 200, 556, -1)->SetPerspective(3);
  CreateObject(CON1, 200, 584, -1)->SetPerspective(3);
  CreateObject(CON1, 200, 612, -1)->SetPerspective(3);
  CreateObject(CON1, 200, 640, -1)->SetPerspective(3);

  CreateObject(CON1, 800, 590, -1)->SetPerspective(3);
  CreateObject(CON1, 832, 562, -1)->SetPerspective(3);
  CreateObject(CON1, 860, 690, -1)->SetPerspective();
  CreateObject(CON1, 970, 690, -1)->SetPerspective();
  CreateObject(CON1, 998, 562, -1)->SetPerspective(1);
  CreateObject(CON1, 1028, 590, -1)->SetPerspective(1);

  CreateObject(CON1, 1630, 528, -1)->SetPerspective(1);
  CreateObject(CON1, 1630, 556, -1)->SetPerspective(1);
  CreateObject(CON1, 1630, 584, -1)->SetPerspective(1);
  CreateObject(CON1, 1630, 612, -1)->SetPerspective(1);
  CreateObject(CON1, 1630, 640, -1)->SetPerspective(1);
  CreateObject(CON1, 1685, 528, -1)->SetPerspective();
  CreateObject(CON1, 1685, 556, -1)->SetPerspective();
  CreateObject(CON1, 1685, 584, -1)->SetPerspective();
  CreateObject(CON1, 1685, 612, -1)->SetPerspective();
  CreateObject(CON1, 1685, 640, -1)->SetPerspective();
  CreateObject(CON1, 1740, 528, -1)->SetPerspective(3);
  CreateObject(CON1, 1740, 556, -1)->SetPerspective(3);
  CreateObject(CON1, 1740, 584, -1)->SetPerspective(3);
  CreateObject(CON1, 1740, 612, -1)->SetPerspective(3);
  CreateObject(CON1, 1740, 640, -1)->SetPerspective(3);

  //Rahmen
  CreateObject(FRME, 500, 312, -1);
  CreateObject(FRME, 550, 862, -1);
  CreateObject(FRME, 710, 542, -1);
  CreateObject(FRME, 1120, 542, -1);
  CreateObject(FRME, 1280, 862, -1);
  CreateObject(FRME, 1330, 312, -1);

  //Glasscheiben
  CreateObject(PANE, 492, 831, -1);
  CreateObject(PANE, 492, 860, -1);
  CreateObject(PANE, 552, 230, -1);
  CreateObject(PANE, 552, 250, -1);

  CreateObject(PANE, 618, 470, -1);
  CreateObject(PANE, 618, 690, -1);
  CreateObject(PANE, 1212, 470, -1);
  CreateObject(PANE, 1212, 690, -1);

  CreateObject(PANE, 1278, 230, -1);
  CreateObject(PANE, 1278, 250, -1);
  CreateObject(PANE, 1338, 831, -1);
  CreateObject(PANE, 1338, 860, -1);

  //Kisten
  CreateObject(WCR2, 160, 300, -1);
  CreateObject(WCR2, 160, 880, -1);
  CreateObject(WCR2, 580, 572, -1);
  CreateObject(WCR2, 580, 590, -1);
  CreateObject(WCR2, 600, 572, -1);
  CreateObject(WCR2, 600, 590, -1);
  CreateObject(WCR2, 1230, 572, -1);
  CreateObject(WCR2, 1230, 590, -1);
  CreateObject(WCR2, 1250, 572, -1);
  CreateObject(WCR2, 1250, 590, -1);
  CreateObject(WCR2, 1640, 300, -1);
  CreateObject(WCR2, 1640, 880, -1);

  //Verbandskisten
  CreateObject(BECR, 240, 640, -1)->AutoRespawn();
  CreateObject(BECR, 1590, 640, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 180, 300, -1);
  CreateObject(MWCR, 180, 880, -1);
  CreateObject(MWCR, 600, 330, -1)->AutoRespawn();
  CreateObject(MWCR, 600, 860, -1)->AutoRespawn();
  CreateObject(MWCR, 685, 470, -1);
  CreateObject(MWCR, 1145, 470, -1);
  CreateObject(MWCR, 1230, 330, -1)->AutoRespawn();
  CreateObject(MWCR, 1230, 860, -1)->AutoRespawn();
  CreateObject(MWCR, 1620, 300, -1);
  CreateObject(MWCR, 1620, 880, -1);

  //Stahlbrücken
  CreateObject(_HBR, 657, 482, -1);
  CreateObject(_HBR, 915, 132, -1);
  CreateObject(_HBR, 915, 942, -1);
  CreateObject(_HBR, 1174, 482, -1);

  //Gasflaschen
  CreateObject(GSBL, 475, 930, -1)->AutoRespawn();
  CreateObject(GSBL, 680, 250, -1)->AutoRespawn();
  CreateObject(GSBL, 740, 590, -1)->AutoRespawn();
  CreateObject(GSBL, 1090, 590, -1)->AutoRespawn();
  CreateObject(GSBL, 1150, 250, -1)->AutoRespawn();
  CreateObject(GSBL, 1355, 930, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 580, 330, -1)->AutoRespawn();
  CreateObject(XWCR, 1250, 330, -1)->AutoRespawn();

  //Gerüste
  CreateObject(SFFG, 835, 440, -1)->Set(1);
  CreateObject(SFFG, 835, 490, -1)->Set(3);
  CreateObject(SFFG, 835, 540, -1)->Set(5);
  CreateObject(SFFG, 835, 590, -1)->Set(5);
  CreateObject(SFFG, 1000, 440, -1)->Set(1);
  CreateObject(SFFG, 1000, 490, -1)->Set(2);
  CreateObject(SFFG, 1000, 540, -1)->Set(5);
  CreateObject(SFFG, 1000, 590, -1)->Set(5);

  //Paletten
  CreateObject(PLLT, 80, 300, -1);
  CreateObject(PLLT, 80, 880, -1);
  CreateObject(PLLT, 380, 590, -1);
  CreateObject(PLLT, 1450, 590, -1);
  CreateObject(PLLT, 1750, 300, -1);
  CreateObject(PLLT, 1750, 880, -1);

  //Verbundene Räume
  aDoorWay[00] = CreateObject(ROOM, 865, 120, -1);
  aDoorWay[01] = CreateObject(ROOM, 855, 1070, -1);
  aDoorWay[00]->Connect(aDoorWay[01]);
  aDoorWay[02] = CreateObject(ROOM, 965, 120, -1);
  aDoorWay[03] = CreateObject(ROOM, 975, 1070, -1);
  aDoorWay[02]->Connect(aDoorWay[03]);

  //Selbstschussanlagen
  aSelfDefense[0] = CreateObject(SEGU, 358, 832, -1);
  aSelfDefense[0]->Set(0,0,0,-90);
  aSelfDefense[0]->SetAutoRepair(1000);

  aSelfDefense[1] = CreateObject(SEGU, 1412, 262, -1);
  aSelfDefense[1]->Set(0,0,0,90);
  aSelfDefense[1]->SetAutoRepair(1000);

  aSelfDefense[2] = CreateObject(SEGU, 1472, 832, -1);
  aSelfDefense[2]->Set(0,0,0,90);
  aSelfDefense[2]->SetAutoRepair(1000);

  aSelfDefense[3] = CreateObject(SEGU, 418, 262, -1);
  aSelfDefense[3]->Set(0,0,0,-90);
  aSelfDefense[3]->SetAutoRepair(1000);

  //Grenzen
  CreateObject(BRDR, 200, 0, -1)->Set(0);
  CreateObject(BRDR, 1630, 0, -1)->Set(1);

  //Sounds

  //Hallen
  CreateObject(SE4K, 150, 570, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 915, 30, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 915, 1100, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1680, 570, -1)->Set("Interior*.ogg",665,105);

  //Metallhallen
  CreateObject(SE4K, 350, 430, -1)->Set("InteriorMetal*.ogg",1000,350);
  CreateObject(SE4K, 1480, 700, -1)->Set("InteriorMetal*.ogg",1000,350);

  //Metro
  CreateObject(SE4K, 30, 570, -1)->Set("Metro*.ogg",1000,200);
  CreateObject(SE4K, 1800, 570, -1)->Set("Metro*.ogg",1000,200);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 380, 640, -1);
  crate->Set(ABOX);
  var crate = CreateObject(AMCT, 1450, 640, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  crate = CreateObject(AMCT, 380, 520, -1);
  crate->Set(GBOX);
  crate = CreateObject(AMCT, 1450, 520, -1);
  crate->Set(GBOX);

  //Versorgungskiste (Raketen)
  crate = CreateObject(AMCT, 835, 535, -1);
  crate->Set(MBOX);

  //Versorgungskiste (Dragnin)
  crate = CreateObject(AMCT, 970, 661, -1);
  crate->Set(DGNN);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Notausgangslichter
  CreateObject(ETLT,270, 865, -1);
  CreateObject(ETLT,320, 285, -1);
  CreateObject(ETLT,1510, 285, -1);
  CreateObject(ETLT,1570, 865, -1);

  //Leitkegel
  CreateObject(TCCN, 310, 880, -1)->Light();
  CreateObject(TCCN, 320, 880, -1);
  CreateObject(TCCN, 330, 880, -1);
  CreateObject(TCCN, 340, 880, -1);
  CreateObject(TCCN, 370, 300, -1)->Light();
  CreateObject(TCCN, 380, 300, -1);
  CreateObject(TCCN, 390, 300, -1);
  CreateObject(TCCN, 820, 838, -1)->SetR(20);
  CreateObject(TCCN, 840, 848, -1)->SetR(20);
  CreateObject(TCCN, 860, 858, -1)->SetR(20);
  CreateObject(TCCN, 880, 868, -1)->SetR(20);
  CreateObject(TCCN, 950, 868, -1)->SetR(-20);
  CreateObject(TCCN, 970, 858, -1)->SetR(-20);
  CreateObject(TCCN, 990, 848, -1)->SetR(-20);
  CreateObject(TCCN, 1010, 838, -1)->SetR(-20);
  CreateObject(TCCN, 1440, 300, -1);
  CreateObject(TCCN, 1450, 300, -1);
  CreateObject(TCCN, 1460, 300, -1)->Light();
  CreateObject(TCCN, 1490, 880, -1);
  CreateObject(TCCN, 1500, 880, -1);
  CreateObject(TCCN, 1510, 880, -1);
  CreateObject(TCCN, 1520, 880, -1)->Light();

  //Bildschirme
  CreateObject(SCA1, 380, 860, -1)->SetAction("Medic");
  CreateObject(SCA1, 435, 290, -1)->SetAction("CMC");
  CreateObject(SCR3, 790, 230, -1);
  CreateObject(SCR3, 790, 910, -1);
  CreateObject(SCA2, 595, 315, -1)->SetClips([8,9,10,11]);
  CreateObject(SCA2, 1235, 315, -1)->SetClips([8,9,10,11]);
  CreateObject(SCR3, 1040, 230, -1);
  CreateObject(SCR3, 1040, 910, -1);
  CreateObject(SCA1, 1395, 290, -1)->SetAction("Medic");
  CreateObject(SCA1, 1450, 860, -1)->SetAction("Grenade");

  //Labortische
  CreateObject(LTBL, 595, 250, -1);
  CreateObject(LTBL, 595, 930, -1);
  CreateObject(LTBL, 710, 250, -1);
  CreateObject(LTBL, 710, 930, -1);
  CreateObject(LTBL, 1120, 250, -1);
  CreateObject(LTBL, 1120, 930, -1);
  CreateObject(LTBL, 1235, 250, -1);
  CreateObject(LTBL, 1235, 930, -1);

  //Monitore
  CreateObject(MNI2, 585, 238, -1);
  CreateObject(MNI2, 585, 918, -1);
  CreateObject(MNI2, 605, 238, -1);
  CreateObject(MNI2, 605, 918, -1);
  CreateObject(MNI2, 700, 238, -1);
  CreateObject(MNI2, 700, 918, -1);
  CreateObject(MNI2, 720, 238, -1);
  CreateObject(MNI2, 720, 918, -1);
  CreateObject(MNI2, 1110, 238, -1);
  CreateObject(MNI2, 1110, 918, -1);
  CreateObject(MNI2, 1130, 238, -1);
  CreateObject(MNI2, 1130, 918, -1);
  CreateObject(MNI2, 1225, 238, -1);
  CreateObject(MNI2, 1225, 918, -1);
  CreateObject(MNI2, 1245, 238, -1);
  CreateObject(MNI2, 1245, 918, -1);

  //Ventillatoren
  CreateObject(VENT, 835, 430, -1);
  CreateObject(VENT, 835, 830, -1)->SetPhase(10);
  CreateObject(VENT, 995, 430, -1)->SetPhase(20);
  CreateObject(VENT, 995, 830, -1)->SetPhase(30);

  //Zäune
  CreateObject(FENC, 20, 640, -1)->Set(2);
  CreateObject(FENC, 80, 640, -1);
  CreateObject(FENC, 140, 640, -1);
  CreateObject(FENC, 200, 640, -1);
  CreateObject(FENC, 260, 640, -1)->Set(2);
  CreateObject(FENC, 320, 640, -1)->Set(2);
  CreateObject(FENC, 1510, 640, -1)->Set(2);
  CreateObject(FENC, 1570, 640, -1)->Set(2);
  CreateObject(FENC, 1630, 640, -1);
  CreateObject(FENC, 1690, 640, -1);
  CreateObject(FENC, 1750, 640, -1);
  CreateObject(FENC, 1810, 640, -1)->Set(2);

  //Geländer
  CreateObject(RAI1, -1, 300, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 0, 880, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 492, 860, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 552, 250, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 672, 590, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 732, 690, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1192, 250, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1192, 860, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1430, 300, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1420, 880, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);

  //Lüftungsgitter
  CreateObject(ENGT, 915, 20, -1);
  CreateObject(ENGT, 915, 60, -1);
  CreateObject(ENGT, 915, 100, -1);
  CreateObject(ENGT, 915, 180, -1);
  CreateObject(ENGT, 915, 1000, -1);
  CreateObject(ENGT, 915, 1040, -1);
  CreateObject(ENGT, 915, 1150, -1);

  //Büsche
  CreateObject(BSH2, 1560, 1100, -1);
  CreateObject(BSH2, 1615, 1070, -1)->SetR(-50);
  CreateObject(BSH2, 1660, 1060, -1)->SetR(70);

  //Automaten
  CreateObject(VGMN, 550, 930, -1);
  CreateObject(VGMN, 1280, 930, -1)->Set(3);

  //Regale
  CreateObject(FRAM, 625, 930, -1);
  CreateObject(FRAM, 645, 930, -1);
  CreateObject(FRAM, 665, 930, -1);
  CreateObject(FRAM, 790, 250, -1);
  CreateObject(FRAM, 810, 250, -1);
  CreateObject(FRAM, 1020, 250, -1);
  CreateObject(FRAM, 1040, 250, -1);
  CreateObject(FRAM, 1160, 930, -1);
  CreateObject(FRAM, 1180, 930, -1);
  CreateObject(FRAM, 1200, 930, -1);

  //Topfpflanzen
  CreateObject(PLT3, 360, 880, -1);
  CreateObject(PLT3, 740, 250, -1);
  CreateObject(PLT3, 1090, 250, -1);
  CreateObject(PLT3, 1470, 880, -1);

  //Poster
  CreateObject(PSTR, 510, 920, -1)->Set(3);
  CreateObject(PSTR, 1320, 920, -1)->Set(2);

  //Papierstapel
  CreateObject(PPSK, 625, 250, -1);
  CreateObject(PPSK, 740, 930, -1);
  CreateObject(PPSK, 1090, 930, -1);
  CreateObject(PPSK, 1205, 250, -1);

  //Apparaturen
  CreateObject(GADG, 210, 300, -1)->Set(1);
  CreateObject(GADG, 210, 880, -1)->Set(1);
  CreateObject(GADG, 230, 300, -1)->Set(1);
  CreateObject(GADG, 230, 880, -1)->Set(1);
  CreateObject(GADG, 825, 690, -1)->Set(1);
  CreateObject(GADG, 1005, 690, -1)->Set(1);
  CreateObject(GADG, 1570, 300, -1)->Set(1);
  CreateObject(GADG, 1570, 880, -1)->Set(1);
  CreateObject(GADG, 1590, 300, -1)->Set(1);
  CreateObject(GADG, 1590, 880, -1)->Set(1);

  //Wandlampen
  CreateObject(BLGH, 290, 570, -1);
  CreateObject(BLGH, 450, 570, -1);
  CreateObject(BLGH, 535, 220, -1);
  CreateObject(BLGH, 655, 820, -1);

  CreateObject(BLGH, 915, 200, -1);
  CreateObject(BLGH, 915, 550, -1);
  CreateObject(BLGH, 915, 660, -1);
  CreateObject(BLGH, 915, 955, -1);

  CreateObject(BLGH, 1175, 820, -1);
  CreateObject(BLGH, 1295, 220, -1);
  CreateObject(BLGH, 1380, 570, -1);
  CreateObject(BLGH, 1540, 570, -1);

  //Glühbirnen
  CreateObject(LBGH, 30, 490, -1);
  CreateObject(LBGH, 1800, 490, -1);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[0])
    aSelfDefense[3]->SetTeam(iTeam);

  if(pPoint == aFlag[1])
    aSelfDefense[0]->SetTeam(iTeam);

  if(pPoint == aFlag[3])
    aSelfDefense[1]->SetTeam(iTeam);

  if(pPoint == aFlag[4])
    aSelfDefense[2]->SetTeam(iTeam);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Eurocorps.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //SSA Besitzer setzen
  if(aTeams[1] == true)
  {aSelfDefense[0]->SetTeam(1);}
  if(aTeams[2] == true)
  {aSelfDefense[1]->SetTeam(2);}
  if(aTeams[3] == true)
  {aSelfDefense[2]->SetTeam(3);}
  if(aTeams[4] == true)
  {aSelfDefense[3]->SetTeam(4);}

  //SSA anschalten
  aSelfDefense[0]->TurnOn();
  aSelfDefense[1]->TurnOn();
  aSelfDefense[2]->TurnOn();
  aSelfDefense[3]->TurnOn();

  //OC-Spielziel
  if(FindObject(GOCC))
  {
    //Flaggenposten
    aFlag[0] = CreateObject(OFPL,505,330,NO_OWNER);
    aFlag[0] -> AddSpawnPoint(380, 290);
    aFlag[0] -> AddSpawnPoint(685, 290);
    aFlag[0] -> AddSpawnPoint(625, 320);
    if(aTeams[4] == true)
    {
      aFlag[0]->Set("$Flag1$",0,4);
      aFlag[0]->Capture(4,1);
    }
    else
    {
      aFlag[0]->Set("$Flag1$");
    }

    aFlag[1] = CreateObject(OFPL,565,860,NO_OWNER);
    aFlag[1] -> AddSpawnPoint(325, 870);
    aFlag[1] -> AddSpawnPoint(685, 850);
    aFlag[1] -> AddSpawnPoint(630, 850);
    if(aTeams[1] == true)
    {
      aFlag[1]->Set("$Flag2$",0,4);
      aFlag[1]->Capture(1,1);
    }
    else
    {
      aFlag[1]->Set("$Flag2$");
    }

    aFlag[2] = CreateObject(OFPL,915,590,NO_OWNER);
    aFlag[2] -> AddSpawnPoint(800, 680);
    aFlag[2] -> AddSpawnPoint(1030, 680);
    aFlag[2]->Set("$Flag3$");

    aFlag[3] = CreateObject(OFPL,1325,330,NO_OWNER);
    aFlag[3] -> AddSpawnPoint(1450, 290);
    aFlag[3] -> AddSpawnPoint(1145, 290);
    aFlag[3] -> AddSpawnPoint(1205, 320);
    if(aTeams[2] == true)
    {
      aFlag[3]->Set("$Flag5$",0,4);
      aFlag[3]->Capture(2,1);
    }
    else
    {
      aFlag[3]->Set("$Flag5$");
    }

    aFlag[4] = CreateObject(OFPL,1260,860,NO_OWNER);
    aFlag[4] -> AddSpawnPoint(1505, 870);
    aFlag[4] -> AddSpawnPoint(1145, 850);
    aFlag[4] -> AddSpawnPoint(1205, 850);
    if(aTeams[3] == true)
    {
      aFlag[4]->Set("$Flag4$",0,4);
      aFlag[4]->Capture(3,1);
    }
    else
    {
      aFlag[4]->Set("$Flag4$");
    }

    //Alarmleuchten
    var warn = CreateObject(ALGH, 655, 210, -1);
    warn->SetR(-180);
    AddGOCCWarnEffect(warn,aFlag[0]);
    warn = CreateObject(ALGH, 555, 280, -1);
    warn->SetR(-180);
    AddGOCCWarnEffect(warn,aFlag[0]);

    warn = CreateObject(ALGH, 575, 780, -1);
    warn->SetR(-180);
    AddGOCCWarnEffect(warn,aFlag[1]);
    warn = CreateObject(ALGH, 700, 890, -1);
    warn->SetR(-180);
    AddGOCCWarnEffect(warn,aFlag[1]);

    warn = CreateObject(ALGH, 790, 610, -1);
    warn->SetR(-180);
    AddGOCCWarnEffect(warn,aFlag[2]);
    warn = CreateObject(ALGH, 1040, 610, -1);
    warn->SetR(-180);
    AddGOCCWarnEffect(warn,aFlag[2]);

    warn = CreateObject(ALGH, 1175, 210, -1);
    warn->SetR(-180);
    AddGOCCWarnEffect(warn,aFlag[3]);
    warn = CreateObject(ALGH, 1275, 280, -1);
    warn->SetR(-180);
    AddGOCCWarnEffect(warn,aFlag[3]);

    warn = CreateObject(ALGH, 1255, 780, -1);
    warn->SetR(-180);
    AddGOCCWarnEffect(warn,aFlag[4]);
    warn = CreateObject(ALGH, 1130, 890, -1);
    warn->SetR(-180);
    AddGOCCWarnEffect(warn,aFlag[4]);
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Flaggenposten
    var flag = CreateObject(OFPL, 915,590, -1);
    flag->~Set("$Flag3$",150);
  }

  //MR-Spielziel
  if(FindObject(GMNR))
  {
    //Geldsäcke
    AddMoneySpawn(450, 585, [15]);
    AddMoneySpawn(915, 225, [15]);
    AddMoneySpawn(915, 580, [20]);
    AddMoneySpawn(915, 925, [15]);
    AddMoneySpawn(1380, 585, [15]);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 915; iY = 730;
    return 1;
  }

  //HTF/MR/LMS/DM-Spielziel
  if(FindObject(GHTF) || FindObject(GMNR) || FindObject(GLMS) || FindObject(GTDM))
  {
    if(iTeam == 1)
      return [[320, 870], [685, 850]];
    if(iTeam == 2)
      return [[1450, 290], [1200, 320]];
    if(iTeam == 3)
      return [[1145, 850], [1510, 870]];
    if(iTeam == 4)
      return [[380, 290], [620, 320]];
    return 1;
  }
}