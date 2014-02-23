/*-- Island Strike --*/

#strict
#include CSTD

static aFlag,aStationary,aSelfDefense,aArtillery;

func RecommendedGoals()			{return [GOCC, GASS];}	//Spielzielempfehlung
public func AssaultDefenderTeam()	{return 2;}		//Verteidigerteam bei Assault


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(1,20,15);
  //Bildschirmfärbung
  SetGamma(RGB(7,6,0),RGB(152,147,128),RGB(255,254,236));
  //Flaggen
  aFlag = [];
  //Geschützstellungen
  aStationary = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Artillerie
  aArtillery = [];
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

  //Aufzüge
  CreateObject(LFTP, 1025, 390, -1);
  CreateObject(LFTP, 2745, 655, -1);
  CreateObject(LFTP, 3655, 655, -1);
  var liftp = CreateObject(LFTP, 3785, 445, -1);
  liftp->DisableVertical();
  CreateObject(LFTP, 4375, 655, -1);
  CreateObject(LFTP, 5665, 645, -1);
  CreateObject(LFTP, 6405, 655, -1);

  //Leitern
  CreateObject(LADR, 495, 604, -1)->Set(7);
  CreateObject(LADR, 695, 604, -1)->Set(7);
  CreateObject(LADR, 925, 553, -1)->Set(22);
  CreateObject(LADR, 1295, 594, -1)->Set(6);
  CreateObject(LADR, 1405, 594, -1)->Set(6);
  CreateObject(LADR, 1595, 594, -1)->Set(6);
  CreateObject(LADR, 1715, 594, -1)->Set(6);
  CreateObject(LADR, 2515, 531, -1)->Set(11);
  CreateObject(LADR, 3505, 740, -1)->Set(20);
  CreateObject(LADR, 3605, 484, -1)->Set(6);
  CreateObject(LADR, 4445, 460, -1)->Set(3);
  CreateObject(LADR, 4680, 638, -1)->Set(8, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 4780, 524, -1)->Set(12);
  CreateObject(LADR, 5120, 637, -1)->Set(16);
  CreateObject(LADR, 5310, 637, -1)->Set(16);
  CreateObject(LADR, 5525, 636, -1)->Set(16);
  CreateObject(LADR, 6635, 534, -1)->Set(11);
  CreateObject(LADR, 7240, 602, -1)->Set(8);
  CreateObject(LADR, 7280, 602, -1)->Set(8);
  CreateObject(LADR, 7320, 602, -1)->Set(8);
  CreateObject(LADR, 7365, 540, -1)->Set(12);
  CreateObject(LADR, 7425, 578, -1)->Set(14);
  CreateObject(LADR, 7670, 475, -1)->Set(6);

  //Stahlbrücken
  CreateObject(_HBR, 5215, 512, -1)->SwitchMode();
  CreateObject(_HBR, 5215, 582, -1)->SwitchMode();
  CreateObject(_HBR, 5447, 582, -1);

  //Bodenluken
  CreateObject(HA4K, 2960, 473, -1);
  CreateObject(HA4K, 2890, 613, -1);
  CreateObject(HA4K, 1100, 573, -1);
  CreateObject(HA4K, 4780, 423, -1);
  CreateObject(HA4K, 5120, 503, -1);
  CreateObject(HA4K, 5120, 573, -1);
  CreateObject(HA4K, 5310, 503, -1);
  CreateObject(HA4K, 5310, 573, -1);
  CreateObject(HA4K, 5760, 453, -1);
  CreateObject(HA4K, 5760, 503, -1);
  CreateObject(HA4K, 6260, 613, -1);
  CreateObject(HA4K, 7200, 463, -1);
  CreateObject(HA4K, 7240, 533, -1);
  CreateObject(HA4K, 7280, 533, -1);
  CreateObject(HA4K, 7320, 533, -1);
  CreateObject(HA4K, 7670, 423, -1);

  //Rampen
  DrawMaterialQuad("Wall-Concrete3",1150,500,1180,490,1180,500,1165,500,true);
  DrawMaterialQuad("Wall-Concrete2",2450,611,2480,601,2480,611,2465,611,true);
  DrawMaterialQuad("Wall-Concrete2",2681,610,2651,600,2651,610,2666,610,true);
  DrawMaterialQuad("Wall-Concrete3",4621,500,4591,490,4591,500,4606,500,true);
  DrawMaterialQuad("Wall-Concrete1",4651,510,4621,500,4621,510,4636,510,true);
  DrawMaterialQuad("Wall-Concrete2",4400,640,4430,630,4430,640,4415,640,true);
  DrawMaterialQuad("Wall-Bricks2",5500,690,5530,680,5530,690,5515,690,true);
  DrawMaterialQuad("Wall-Bricks2",5581,690,5551,680,5551,690,5566,690,true);
  DrawMaterialQuad("Wall-PlateRed",6060,360,6030,350,6030,360,6045,360,true);
  DrawMaterialQuad("Wall-PlateRed",6110,360,6080,350,6080,360,6095,360,true);
  DrawMaterialQuad("Wall-PlateRed",6160,360,6130,350,6130,360,6145,360,true);
  DrawMaterialQuad("Wall-PlateRed",6211,360,6181,350,6181,360,6196,360,true);
  DrawMaterialQuad("Wall-Concrete3",6211,490,6181,480,6181,490,6196,490,true);
  DrawMaterialQuad("Wall-Concrete1",6241,500,6211,490,6211,500,6226,500,true);
  DrawMaterialQuad("Wall-Concrete1",6271,510,6241,500,6241,510,6256,510,true);
  DrawMaterialQuad("Wall-Concrete2",6470,610,6500,600,6500,610,6485,610,true);
  DrawMaterialQuad("Wall-Concrete2",6701,610,6671,600,6671,610,6686,610,true);

  //Glasscheiben
  CreateObject(PANE, 982, 430, -1);
  CreateObject(PANE, 1068, 430, -1);
  CreateObject(PANE, 5092, 570, -1);
  CreateObject(PANE, 5148, 570, -1);
  CreateObject(PANE, 5282, 570, -1);
  CreateObject(PANE, 5338, 570, -1);

  //Kisten
  CreateObject(WCR2, 3605, 600, -1)->AutoRespawn();
  CreateObject(WCR2, 3775, 510, -1)->AutoRespawn();
  CreateObject(WCR2, 4250, 510, -1);
  CreateObject(WCR2, 4265, 510, -1)->AutoRespawn();
  CreateObject(WCR2, 6290, 720, -1)->AutoRespawn();
  CreateObject(WCR2, 6310, 720, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 1510, 500, -1)->AutoRespawn();
  CreateObject(MWCR, 4570, 490, -1)->AutoRespawn();
  CreateObject(MWCR, 5030, 570, -1)->AutoRespawn();
  CreateObject(MWCR, 5370, 570, -1)->AutoRespawn();
  CreateObject(MWCR, 7550, 610, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 1275, 500, -1)->AutoRespawn();
  CreateObject(BECR, 4400, 430, -1)->AutoRespawn();
  CreateObject(BECR, 5050, 570, -1)->AutoRespawn();
  CreateObject(BECR, 7325, 410, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 5560, 500, -1)->AutoRespawn();
  CreateObject(XWCR, 7550, 592, -1);

  //Benzinfässer
  CreateObject(PBRL, 520, 540, -1)->AutoRespawn();
  CreateObject(PBRL, 530, 540, -1)->AutoRespawn();
  CreateObject(PBRL, 1475, 500, -1)->AutoRespawn();
  CreateObject(PBRL, 1485, 500, -1)->AutoRespawn();
  CreateObject(PBRL, 1100, 370, -1)->AutoRespawn();
  CreateObject(PBRL, 1170, 570, -1)->AutoRespawn();
  CreateObject(PBRL, 2815, 691, -1)->AutoRespawn();
  CreateObject(PBRL, 3450, 730, -1)->AutoRespawn();
  CreateObject(PBRL, 3585, 600, -1)->AutoRespawn();
  CreateObject(PBRL, 4330, 510, -1)->AutoRespawn();
  CreateObject(PBRL, 4485, 680, -1)->AutoRespawn();
  CreateObject(PBRL, 6220, 610, -1)->AutoRespawn();
  CreateObject(PBRL, 7790, 420, -1)->AutoRespawn();
  CreateObject(PBRL, 7800, 420, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 1255, 500, -1)->AutoRespawn();
  CreateObject(XBRL, 2945, 610, -1)->AutoRespawn();
  CreateObject(XBRL, 2955, 610, -1)->AutoRespawn();
  CreateObject(XBRL, 3755, 510, -1)->AutoRespawn();
  CreateObject(XBRL, 7225, 530, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 4240, 680, -1)->AutoRespawn();
  CreateObject(GSBL, 5760, 650, -1)->AutoRespawn();

  //Gerüste
  CreateObject(SFFG, 5120, 450, -1)->Set(4);
  CreateObject(SFFG, 5120, 500, -1)->Set(5);
  CreateObject(SFFG, 5310, 450, -1)->Set(5);
  CreateObject(SFFG, 5310, 500, -1)->Set(5);

  //Haie
  CreateObject(SHRK, 100, 660, -1)->AutoRespawn();
  CreateObject(SHRK, 200, 660, -1)->AutoRespawn();
  CreateObject(SHRK, 500, 660, -1)->AutoRespawn();
  CreateObject(SHRK, 600, 660, -1)->AutoRespawn();

  CreateObject(SHRK, 1500, 645, -1)->AutoRespawn();
  CreateObject(SHRK, 1600, 645, -1)->AutoRespawn();
  CreateObject(SHRK, 1800, 650, -1)->AutoRespawn();
  CreateObject(SHRK, 1900, 650, -1)->AutoRespawn();

  CreateObject(SHRK, 3930, 680, -1)->AutoRespawn();
  CreateObject(SHRK, 4000, 680, -1)->AutoRespawn();
  CreateObject(SHRK, 4040, 700, -1)->AutoRespawn();
  CreateObject(SHRK, 4100, 700, -1)->AutoRespawn();

  CreateObject(SHRK, 4700, 685, -1)->AutoRespawn();
  CreateObject(SHRK, 4800, 685, -1)->AutoRespawn();
  CreateObject(SHRK, 5100, 685, -1)->AutoRespawn();
  CreateObject(SHRK, 5200, 685, -1)->AutoRespawn();

  CreateObject(SHRK, 7200, 650, -1)->AutoRespawn();
  CreateObject(SHRK, 7300, 650, -1)->AutoRespawn();
  CreateObject(SHRK, 7600, 650, -1)->AutoRespawn();
  CreateObject(SHRK, 7700, 650, -1)->AutoRespawn();

  //Container
  CreateObject(CON1, 2830, 720, -1);
  CreateObject(CON1, 6089, 480, -1)->SetPerspective();
  CreateObject(CON1, 6146, 480, -1);
  CreateObject(CON1, 7500, 610, -1)->SetPerspective(2);

  //Schutztüren
  CreateObject(GDDR, 990, 370, -1);
  CreateObject(GDDR, 1060, 370, -1);
  CreateObject(GDDR, 2650, 510, -1);
  CreateObject(GDDR, 2700, 720, -1);
  CreateObject(GDDR, 2810, 510, -1);
  CreateObject(GDDR, 2990, 610, -1);
  CreateObject(GDDR, 2990, 720, -1);
  CreateObject(GDDR, 3420, 730, -1);
  CreateObject(GDDR, 3450, 620, -1);
  CreateObject(GDDR, 3690, 630, -1);
  CreateObject(GDDR, 4310, 510, -1);
  CreateObject(GDDR, 4430, 490, -1);
  CreateObject(GDDR, 4520, 680, -1);
  CreateObject(GDDR, 5610, 500, -1);
  CreateObject(GDDR, 5860, 610, -1);
  CreateObject(GDDR, 6160, 610, -1);
  CreateObject(GDDR, 6160, 720, -1);
  CreateObject(GDDR, 6340, 510, -1);
  CreateObject(GDDR, 6450, 720, -1);
  CreateObject(GDDR, 6500, 510, -1);
  CreateObject(GDDR, 7320, 460, -1);

  //Sandsackbarrieren
  CreateObject(SBBA, 1380, 540, -1)->Right();
  CreateObject(SBBA, 1690, 540, -1)->Right();
  CreateObject(SBBA, 2660, 440, -1);
  CreateObject(SBBA, 3800, 510, -1)->Right();
  CreateObject(SBBA, 4204, 509, -1);
  CreateObject(SBBA, 5075, 500, -1);
  CreateObject(SBBA, 5165, 500, -1)->Right();
  CreateObject(SBBA, 5265, 500, -1);
  CreateObject(SBBA, 5355, 500, -1)->Right();
  CreateObject(SBBA, 5780, 450, -1)->Right();
  CreateObject(SBBA, 6340, 720, -1)->Right();
  CreateObject(SBBA, 6490, 440, -1)->Right();

  //Explosivtanks
  CreateObject(XTNK, 1335, 540, -1)->AutoRespawn();
  CreateObject(XTNK, 4345, 430, -1)->AutoRespawn();
  CreateObject(XTNK, 4690, 420, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 1635, 540, -1)->AutoRespawn();
  CreateObject(PTNK, 2855, 510, -1)->AutoRespawn();
  CreateObject(PTNK, 6295, 510, -1)->AutoRespawn();

  //Lampen
  CreateObject(BLGH, 1025, 360, -1);
  CreateObject(LLGH, 1035, 620, -1);
  CreateObject(BLGH, 1160, 545, -1);
  CreateObject(LLGH, 1250, 590, -1);
  CreateObject(LLGH, 1340, 590, -1);
  CreateObject(LLGH, 1500, 590, -1);
  CreateObject(LLGH, 1655, 590, -1);
  CreateObject(BLGH, 2690, 490, -1);
  CreateObject(BLGH, 3585, 575, -1);
  CreateObject(BLGH, 4335, 485, -1);
  CreateObject(BLGH, 4335, 625, -1);
  CreateObject(LLGH, 5120, 640, -1);
  CreateObject(LLGH, 5310, 640, -1);
  CreateObject(BLGH, 5725, 605, -1);
  CreateObject(BLGH, 6445, 485, -1);
  CreateObject(LLGH, 7280, 510, -1);

  //Kran
  CreateObject(CRN1, 4000, 459, -1)->Set(42);

  //Verbundene Räume
  var doorw = CreateObject(GAT1, 3845, 785, -1);
  doorw->SetClrModulation(RGB(125,125,205));
  CreateObject(GAT1, 3830, 625, -1)->Connect(doorw);
  doorw = CreateObject(GAT1, 4200, 785, -1);
  doorw->SetClrModulation(RGB(125,125,205));
  CreateObject(GAT1, 4215, 615, -1)->Connect(doorw);

  //Sonne umplatzieren
  FindObject(LENS)->SetPosition(4165,100);

  //Tore und Konsolen
  var autod = CreateObject (HNG3, 2780, 720, -1);
  autod->Open();
  CreateObject (CONS, 2880, 715, -1)->Set(autod);
  
  autod = CreateObject (HNG3, 3570, 600, -1);
  autod->Open();
  CreateObject (CONS, 3695, 565, -1)->Set(autod);
  
  autod = CreateObject (HNG3, 5532, 670, -1);
  autod->Open();
  CreateObject (CONS, 5630, 630, -1)->Set(autod);
  
  autod = CreateObject (HNG3, 6370, 720, -1);
  autod->Open();
  CreateObject (CONS, 6270, 715, -1)->Set(autod);

  //Selbstschussanlagen
  aSelfDefense[0] = CreateObject(SEGU, 1095, 409, -1);
  CreateObject(CONS, 1155, 630, -1)->Set(aSelfDefense[0]);

  aSelfDefense[1] = CreateObject(SEGU, 2565, 479, -1);
  CreateObject(CONS, 2800, 600, -1)->Set(aSelfDefense[1]);

  aSelfDefense[2] = CreateObject(SEGU, 6585, 479, -1);
  CreateObject(CONS, 6355, 600, -1)->Set(aSelfDefense[2]);

  aSelfDefense[3] = CreateObject(SEGU, 7190, 579, -1);
  CreateObject(CONS, 7260, 520, -1)->Set(aSelfDefense[3]);

  //Seegras entfernen
  for(var obj in FindObjects(Find_Or(Find_ID(SWD1), Find_ID(SWD2)), Find_InRect(1190, 490, 910, 150)))
   RemoveObject(obj);
  for(var obj in FindObjects(Find_Or(Find_ID(SWD1), Find_ID(SWD2)), Find_InRect(4590, 470, 1030, 230)))
   RemoveObject(obj);
  for(var obj in FindObjects(Find_Or(Find_ID(SWD1), Find_ID(SWD2)), Find_InRect(7120, 360, 710, 270)))
   RemoveObject(obj);

  //Sounds

  //Möven
  CreateObject(SE4K, 865, 580, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 2290, 600, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 2480, 560, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 4775, 560, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 5120, 520, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 6660, 550, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 6850, 600, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 7560, 375, -1)->Set("SeaSounds*.ogg",140,35);

  //Wellen
  CreateObject(SE4K, 240, 610, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 640, 610, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1620, 610, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 2090, 610, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 4000, 630, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 4910, 650, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 5370, 650, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 7050, 610, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 7660, 610, -1)->Set("Wave*.ogg",105,35);

  //Jungel
  CreateObject(SE4K, 3160, 390, -1)->Set("Jungle*.ogg",140,70);
  CreateObject(SE4K, 3375, 460, -1)->Set("Jungle*.ogg",140,70);
  CreateObject(SE4K, 5850, 440, -1)->Set("Jungle*.ogg",140,70);

  //Lautsprecher
  CreateObject(SE4K, 1140, 430, -1)->Set("Announce*.ogg",3000,200);
  CreateObject(SE4K, 4000, 530, -1)->Set("Announce*.ogg",3000,200);
  CreateObject(SE4K, 7215, 380, -1)->Set("Announce*.ogg",3000,200);

}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 1060, 640, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 7300, 530, -1);
  crate->Set(ABOX);

  //Kugeln
  PlaceSpawnpoint(ABOX, 3160, 390);
  PlaceSpawnpoint(ABOX, 5915, 615);

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 4735, 555);

  //Raketen
  PlaceSpawnpoint(MBOX, 2485, 740);
  PlaceSpawnpoint(MBOX, 6600, 750);

  //Artilleriebatterien
  aArtillery[0] = CreateObject(ATBY,1025,310,-1);
  aArtillery[0]->SetRotation(20);
  aArtillery[1] = CreateObject(ATBY,3640,430,-1);
  aArtillery[1]->SetRotation(20);
  aArtillery[2] = CreateObject(ATBY,5720,440,-1);
  aArtillery[2]->SetRotation(-30);
  aArtillery[3] = CreateObject(ATBY,7360,410,-1);
  aArtillery[3]->SetRotation(-30);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Denkmal
  CreateObject(MUMT, 3160, 400, -1);

  //Radio
  CreateObject(RDIO, 4300, 570, -1);

  //Rohre
  CreateObject(PIPL, 2730, 465, -1)->Up()->Right()->Right()->Right()->Right();
  CreateObject(PIPL, 2685, 465, -1)->Up()->Right();

  CreateObject(PIPL, 5165, 570, -1)->Right()->Right();
  CreateObject(PIPL, 5165, 585, -1)->Right()->Right();
  CreateObject(PIPL, 5350, 570, -1)->Right()->Right()->Right()->Up()->Up()->Right()->Right();
  CreateObject(PIPL, 5350, 585, -1)->Right()->Right()->Up()->Up()->Right()->Right()->Right();

  //Jetwrack
  CreateObject(_JW1, 3180, 405, -1);
  CreateObject(_JW2, 3380, 360, -1);

  //Kaputte U-Boote
  CreateObject(SUBB, 240, 730, -1);
  CreateObject(SUBB, 7200, 800, -1);

  //Kaputte Boote
  CreateObject(SLBB, 1785, 650, -1);
  CreateObject(SLBB, 4000, 775, -1);
  CreateObject(SLBB, 5270, 665, -1);
  CreateObject(SLBB, 7830, 725, -1);

  //Regale
  CreateObject(FRAM, 4250, 680, -1);
  CreateObject(FRAM, 4270, 680, -1);
  CreateObject(FRAM, 4290, 680, -1);
  CreateObject(FRAM, 4310, 680, -1);
  CreateObject(FRAM, 4330, 680, -1);

  CreateObject(FRAM, 4415, 680, -1);
  CreateObject(FRAM, 4435, 680, -1);
  CreateObject(FRAM, 4455, 680, -1);
  CreateObject(FRAM, 4475, 680, -1);
  CreateObject(FRAM, 4495, 680, -1);

  //Ventillatoren
  CreateObject(VENT, 2705, 563, -1)->SetCon(20);
  CreateObject(VENT, 6445, 563, -1)->SetCon(20);

  //Satellitenschüssel
  CreateObject(SADH, 7625, 420, -1);

  //Radare
  CreateObject(RADR, 1500, 390, -1);
  CreateObject(RADR, 2960, 370, -1);

  //Schilder
  CreateObject(ESGN, 1065, 560, -1);
  CreateObject(MSGN, 2610, 510, -1);
  CreateObject(MSGN, 2620, 510, -1);
  CreateObject(MSGN, 2630, 510, -1);
  CreateObject(ESGN, 2960, 605, -1);
  CreateObject(ESGN, 5830, 605, -1);
  CreateObject(ESGN, 6200, 605, -1);
  CreateObject(MSGN, 6520, 510, -1);
  CreateObject(MSGN, 6530, 510, -1);
  CreateObject(MSGN, 6540, 510, -1);
  CreateObject(MSGN, 7142, 460, -1);
  CreateObject(MSGN, 7418, 460, -1);

  //Topfpflanzen
  CreateObject(PLT3, 1030, 640, -1);
  CreateObject(PLNT, 1120, 625, -1);
  CreateObject(PLNT, 2715, 610, -1);
  CreateObject(PLNT, 6435, 610, -1);

  //Apparaturen
  CreateObject(GADG, 1135, 640, -1)->Set(1);
  CreateObject(GADG, 4340, 680, -1)->Set(2);
  CreateObject(GADG, 5810, 610, -1)->Set(1);
  CreateObject(GADG, 5820, 610, -1)->Set(1);

  //Orientierungslichter
  CreateObject(OLGH, 595, 540, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 1025, 310, -1)->Set(3, 15, 1, 1, 30);
  CreateObject(OLGH, 1500, 390, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 2560, 440, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 2960, 370, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 4750, 420, -1)->Set(5, 15, 1, 1, 30);
  CreateObject(OLGH, 6590, 440, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 7755, 420, -1)->Set(4, 15, 1, 1, 30);

  //Bojen
  CreateObject(BUOY, 445, 633, -1);
  CreateObject(BUOY, 1765, 633, -1);
  CreateObject(BUOY, 5040, 673, -1);
  CreateObject(BUOY, 5200, 673, -1);
  CreateObject(BUOY, 7665, 633, -1);

  //Büsche
  CreateObject(BSH2, 1875, 450, -1);
  CreateObject(BSH2, 2470, 610, -1);
  CreateObject(BSH2, 2460, 620, -1);
  CreateObject(BSH2, 3060, 740, -1);
  CreateObject(BSH2, 6035, 680, -1);
  CreateObject(BSH2, 6680, 610, -1);
  CreateObject(BSH2, 6690, 620, -1);

  //Steine
  CreateObject(STNE, 1460, 660, -1);
  CreateObject(STNE, 2030, 440, -1)->Set(2);
  CreateObject(STNE, 3320, 470, -1)->Set(1);
  CreateObject(STNE, 5815, 450, -1)->Set(0);
  CreateObject(STNE, 6640, 770, -1)->Set(5);
  CreateObject(STNE, 7410, 670, -1);

  //Dekoschleusen
  CreateObject(GAT1, 6045, 415, -1);
  CreateObject(GAT1, 6145, 415, -1);
  CreateObject(GAT1, 6195, 415, -1);

  //Monitor
  CreateObject(MNI2, 1095, 628, -1);

  //Spinde
  CreateObject(LCKR, 5705, 650, -1);
  CreateObject(LCKR, 5725, 650, -1);
  CreateObject(LCKR, 5745, 650, -1);

  //Geländer
  CreateObject(RAI1, 510, 540, -1)->SetRail([1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 930, 370, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1075, 370, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1300, 540, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 1600, 540, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 3700, 510, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 4190, 510, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 4660, 420, -1)->SetRail([1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 5063, 500, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 5253, 500, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 5530, 500, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 7140, 460, -1)->SetRail([1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 7395, 460, -1)->SetRail([1]);
  CreateObject(RAI1, 7480, 610, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 7660, 420, -1)->SetRail([1,1,1,1,1,1,1,1,1]);

  //Zäune
  CreateObject(FENC, 2850, 510, -1);
  CreateObject(FENC, 4470, 490, -1)->Set(2);
  CreateObject(FENC, 4531, 490, -1)->Set(2);
  CreateObject(FENC, 6300, 510, -1);

  //Bildschirme
  CreateObject(SCR1, 2745, 570, -1);
  CreateObject(SCR3, 3555, 695, -1);
  CreateObject(SCA1, 4375, 580, -1)->SetAction("Grenade");
  CreateObject(SCR1, 6405, 570, -1);
  CreateObject(SCR3, 7310, 510, -1);

  //Automaten
  CreateObject(VGMN, 1000, 640, -1);
  CreateObject(VGMN, 7210, 530, -1)->Set(3);

  //Lüftungsgitter
  CreateObject(ENGT, 1250, 570, -1);
  CreateObject(ENGT, 3610, 710, -1);
  CreateObject(ENGT, 5725, 650, -1);
  CreateObject(ENGT, 7190, 605, -1);
  CreateObject(ENGT, 7390, 605, -1);

  //Tische
  CreateObject(GTBL, 1105, 640, -1);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  var back = CreateObject(BD03,400,350,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGBa(50,50,50,5));
  back = CreateObject(BD05,1800,900,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGBa(50,50,50,5));
  back = CreateObject(BD03,4500,350,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGBa(50,50,50,5));
}

/* Sturmerstellung */

func FormStorm()
{
  //Sturm erstellen
  CreateObject(STRM);

  //Dunkelheit erhöhen
  if(GetDarkness() < 3)
   FadeDarkness(3,60);

  //Hintergründe verdunkeln
  var back = FindObjects(Find_Func("IsDeco"));
  for (var pObj in back)
   pObj->SetClrModulation(RGBa(50,50,50,5));

  //Umwelteffekte entfernen
  RemoveAll(BRDS);
  RemoveAll(LENS);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[0])
   aSelfDefense[0]->SetTeam(iTeam);

  if(pPoint == aFlag[1])
   aSelfDefense[1]->SetTeam(iTeam);

  if(pPoint == aFlag[5])
   aSelfDefense[2]->SetTeam(iTeam);

  if(pPoint == aFlag[6])
   aSelfDefense[3]->SetTeam(iTeam);
}

/* Bei Relaunch */
 
public func OnClassSelection(object pClonk, int iTeam)
{
  //Assault-Spielziel
  if(FindObject(GASS))
  {
   if(GetPlayerTeam(GetOwner(pClonk)) == 1)
   {
    if(GetAssaultTarget(0,1) || GetAssaultTarget(1,1) || GetAssaultTarget(2,1))
    {
     AddEffect("IntPara", pClonk, 1, 1);
     AddEffect("Flying", pClonk, 101, 5);
     Sound("Airstrike2", 0, pClonk);
    }
   }
  }
}
 
global func FxIntParaTimer(object pTarget)
{
  if(!Contained(pTarget))
   CreateObject(PARA,0,0,GetOwner(pTarget))->Set(pTarget);
  return -1;
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Blue Ressort.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Blue Ressort.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,1205,490,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(950,360);
   aFlag[0] -> AddSpawnPoint(985,560);
   aFlag[0] -> AddSpawnPoint(1170,630);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,2720,440,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(2640,730);
   aFlag[1] -> AddSpawnPoint(2710,600);
   aFlag[1] -> AddSpawnPoint(2870,600);
   if(aTeams[1] == true)
   {
    aFlag[1]->Set("$Flag2$",100,2);
    aFlag[1]->Capture(1,1);
   }
   else
   {
    aFlag[1]->Set("$Flag2$",0,2);
   }

   aFlag[2] = CreateObject(OFPL,3580,730,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(3395,610);
   aFlag[2] -> AddSpawnPoint(3610,500);
   aFlag[2] -> AddSpawnPoint(3640,420);
   if(aTeams[1] == true)
   {
    aFlag[2]->Set("$Flag3$",100,2);
    aFlag[2]->Capture(1,1);
   }
   else
   {
    aFlag[2]->Set("$Flag3$",0,2);
   }

   aFlag[3] = CreateObject(OFPL,4510,490,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(4230,500);
   aFlag[3] -> AddSpawnPoint(4335,670);
   aFlag[3] -> AddSpawnPoint(4550,670);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,5665,440,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(5760,640);
   aFlag[4] -> AddSpawnPoint(5785,600);
   aFlag[4] -> AddSpawnPoint(5890,600);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag5$",0,2);
   }

   aFlag[5] = CreateObject(OFPL,6430,440,NO_OWNER);
   aFlag[5] -> AddSpawnPoint(6260,710);
   aFlag[5] -> AddSpawnPoint(6440,600);
   aFlag[5] -> AddSpawnPoint(6510,730);
   if(aTeams[2] == true)
   {
    aFlag[5]->Set("$Flag6$",100,2);
    aFlag[5]->Capture(2,1);
   }
   else
   {
    aFlag[5]->Set("$Flag6$",0,2);
   }

   aFlag[6] = CreateObject(OFPL,7260,460,NO_OWNER);
   aFlag[6] -> AddSpawnPoint(7500,575);
   aFlag[6] -> AddSpawnPoint(7580,600);
   aFlag[6] -> AddSpawnPoint(7620,410);
   if(aTeams[2] == true)
   {
    aFlag[6]->Set("$Flag7$",100,2);
    aFlag[6]->Capture(2,1);
   }
   else
   {
    aFlag[6]->Set("$Flag7$",0,2);
   }

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1); aSelfDefense[1]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[2]->SetTeam(2); aSelfDefense[3]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();
   aSelfDefense[2]->TurnOn();
   aSelfDefense[3]->TurnOn();

   //Grenzen setzen
   CreateObject(BRDR, 440, 0, -1)->Set(0);
   CreateObject(BRDR, 7860, 0, -1)->Set(1);

   //Helikopter und Hinweisschilder
   if(!FindObject(NOHC))
   {
    SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,4700,390,-1),100*21);

    sign = CreateObject(SGNP, 4810, 420, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
   }

   //Hinweisschilder
   var sign = CreateObject(SNPT, 1460, 500, -1);
   sign->SetAction("Sign2");
   sign->SetMode(1);
   sign = CreateObject(SGNP, 2530, 570, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 6620, 570, -1);
   sign->SetMode(1);
   sign->Set("Turret");

   //Verbundene Räume
   var doorw = CreateObject(GAT1, 2960, 433, -1);
   CreateObject(ROOM, 3570, 510, -1)->Connect(doorw);
   doorw = CreateObject(GAT1, 6095, 415, -1);
   CreateObject(ROOM, 5735, 500, -1)->Connect(doorw);

   //Geschützstellungen
   CreateObject(GNET, 2560, 440, -1)->Set(SATW,-90);
   CreateObject(GNET, 6590, 440, -1)->Set(SATW,90);

   //MAV-Stationen
   CreateObject(MVSN, 2690, 510, -1)->Set(2690,440,1);
   CreateObject(MVSN, 6460, 510, -1)->Set(6460,440,1);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1245,610,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2195,609,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,4640,649,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5565,650,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,7440,609,-1),50*21);

   //Alarmleuchten
   //Flaggenposten 1
   var warn = CreateObject(ALGH, 935, 399, -1);
   warn->SetR(-180);
   AddGOCCWarnEffect(warn, aFlag[0]);
   warn = CreateObject(ALGH, 1115, 399, -1);
   warn->SetR(-180);
   AddGOCCWarnEffect(warn, aFlag[0]);
   warn = CreateObject(ALGH, 1305, 569, -1);
   warn->SetR(-180);
   AddGOCCWarnEffect(warn, aFlag[0]);

   //Flaggenposten 2
   warn = CreateObject (ALGH, 2525, 469, -1);
   warn->SetR(-180);
   AddGOCCWarnEffect(warn, aFlag[1]);
   warn = CreateObject (ALGH, 2595, 469, -1);
   warn->SetR(-180);
   AddGOCCWarnEffect(warn, aFlag[1]);
   warn = CreateObject (ALGH, 2780, 539, -1);
   warn->SetR(-180);
   AddGOCCWarnEffect(warn, aFlag[1]);

   //Flaggenposten 3
   warn = CreateObject (ALGH, 3570, 469, -1);
   warn->SetR(-180);
   AddGOCCWarnEffect(warn, aFlag[2]);
   warn = CreateObject (ALGH, 3710, 609, -1);
   warn->SetR(-180);
   AddGOCCWarnEffect(warn, aFlag[2]);

   //Flaggenposten 4
   warn = CreateObject (ALGH, 4245, 599, -1);
   warn->SetR(-180);
   AddGOCCWarnEffect(warn, aFlag[3]);
   warn = CreateObject (ALGH, 4415, 609, -1);
   warn->SetR(-180);
   AddGOCCWarnEffect(warn, aFlag[3]);

   //Flaggenposten 5
   warn = CreateObject(ALGH, 5705, 479, -1);
   warn->SetR(-180); 
   AddGOCCWarnEffect(warn, aFlag[4]);
   warn = CreateObject (ALGH, 5570, 619, -1);
   warn->SetR(-180);
   AddGOCCWarnEffect(warn, aFlag[4]);

   //Flaggenposten 6
   warn = CreateObject (ALGH, 6555, 469, -1);
   warn->SetR(-180);
   AddGOCCWarnEffect(warn, aFlag[5]);
   warn = CreateObject (ALGH, 6625, 469, -1);
   warn->SetR(-180);
   AddGOCCWarnEffect(warn, aFlag[5]);
   warn = CreateObject (ALGH, 6370, 539, -1);
   warn->SetR(-180);
   AddGOCCWarnEffect(warn, aFlag[5]);

   //Flaggenposten 7
   warn = CreateObject (ALGH, 7150, 460, -1);
   AddGOCCWarnEffect(warn, aFlag[6]);
   warn = CreateObject (ALGH, 7410, 460, -1);
   AddGOCCWarnEffect(warn, aFlag[6]);
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {
   //Zielobjekte
   AddAssaultTarget(RADR, 1200, 490, 30*30, 2, "$Target1$", 0, [[[1680, 540], [1790, 530], [1880, 520]], [[560, 0], [610, 0], [650, 0]]]);
   AddAssaultTarget(CMSN, 2720, 440, 0, 2, "$Target2$", 1, [[[2920, 720], [3025, 610], [3040, 730]], [[1650, 0], [1670, 0], [1710, 0]]]);
   AddAssaultTarget(CCP2, 3580, 730, 0, 2, "$Target3$", 2, [[[3690, 430], [3795, 510], [3850, 630]], [[2570, 0], [2700, 0], [2800, 0]]]);
   AddAssaultTarget(GSTA, 4520, 490, 0, 2, "$Target4$", 3, [[[4870, 540], [4930, 530], [5000, 490]], [[3570, 430], [3600, 510], [3470, 730]]]);
   AddAssaultTarget(LBPC, 5720, 650, 0, 2, "$Target5$", 4, [[[6040, 480], [6115, 452], [6180, 480]], [[4290, 510], [4320, 570], [4320, 680]]]);
   AddAssaultTarget(RADR, 6440, 440, 0, 2, "$Target6$", 5, [[[6520, 740], [6670, 750], [6695, 609]], [[5630, 500], [5630, 570], [5730, 650]]]);
   AddAssaultTarget(CMSN, 7210, 530, 30*30, 2, "$Target7$", 6, [[[7565, 510], [7580, 610], [7750, 420]], [[6410, 440], [6460, 610], [6370, 610]]]);

   //Grenzen setzen
   CreateObject(BRDR, 440, 0, -1)->Set(0,1);
   CreateObject(BRDR, 1980, 0, -1)->Set(1,1);

   //SSA Besitzer setzen
   if(aTeams[2] == true)
    aSelfDefense[3]->SetTeam(2);

   //SSA anschalten
   aSelfDefense[3]->TurnOn();

   //Helikopter und Hinweisschilder
   if(!FindObject(NOHC))
   {
    SetupVehicleSpawn([APCE],DIR_Left,CreateObject(VSPW,4730,390,-1),50*21);

    sign = CreateObject(SGNP, 4810, 420, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
   }

   //Boden
   DrawMaterialQuad("Wall-Bricks2", 7230,530, 7250,530, 7250,560, 7230,560);

   //Objekte entfernen
   RemoveObject(aSelfDefense[0]);
   RemoveObject(aSelfDefense[1]);
   RemoveObject(aSelfDefense[2]);
   RemoveObject(aArtillery[0]);
   RemoveAll(LCKR);
   RemoveObject(FindObject2(Find_ID(LADR),Find_InRect(915, 370, 20, 190)));
   RemoveObject(FindObject2(Find_ID(BUOY),Find_InRect(5160, 620, 100, 100)));
   RemoveObject(FindObject2(Find_ID(VGMN),Find_InRect(7200, 490, 40, 40)));
   RemoveObject(FindObject2(Find_ID(RSPT),Find_InRect(7224, 519, 3, 3)));
   RemoveObject(FindObject2(Find_ID(HA4K),Find_InRect(7225, 510, 20, 30)));
   RemoveObject(FindObject2(Find_ID(LADR),Find_InRect(7230, 530, 20, 30)));
   RemoveObject(FindObject2(Find_ID(CONS),Find_InRect(7245, 510, 20, 20)));

   //Konsole
   CreateObject(CONS, 7635, 465, -1)->Set(aSelfDefense[3]);

   //Leitern
   CreateObject(LADR, 6025, 450, -1)->Set(12);
   CreateObject(LADR, 7700, 590, -1)->Set(13);

   //Stahlbrücken
   CreateObject(_HBR, 495, 622, -1);
   CreateObject(_HBR, 565, 622, -1);
   CreateObject(_HBR, 635, 622, -1);
   CreateObject(_HBR, 705, 622, -1);
   CreateObject(_HBR, 775, 622, -1);
   CreateObject(_HBR, 2125, 532, -1);
   CreateObject(_HBR, 2195, 532, -1);
   CreateObject(_HBR, 5230, 662, -1);
   CreateObject(_HBR, 5300, 662, -1);
   CreateObject(_HBR, 5370, 662, -1);
   CreateObject(_HBR, 5440, 662, -1);

   //Gerüst
   CreateObject(SFFG, 650, 540, -1)->Set(4);

   //Metallkisten
   CreateObject(MWCR, 678, 490, -1);
   CreateObject(MWCR, 790, 610, -1);
   CreateObject(MWCR, 990, 510, -1);
   CreateObject(MWCR, 1070, 570, -1);
   CreateObject(MWCR, 2219, 520, -1);
   CreateObject(MWCR, 5400, 650, -1);

   //Kisten
   CreateObject(WCR2, 630, 610, -1);
   CreateObject(WCR2, 890, 580, -1);
   CreateObject(WCR2, 1310, 540, -1);
   CreateObject(WCR2, 1330, 540, -1);
   CreateObject(WCR2, 5288, 650, -1);
   CreateObject(WCR2, 5400, 632, -1);
   CreateObject(WCR2, 5795, 610, -1);
   CreateObject(WCR2, 5880, 610, -1);

   //Verbandskisten
   CreateObject(BECR, 650, 610, -1);
   CreateObject(BECR, 785, 592, -1);
   CreateObject(BECR, 2120, 520, -1);
   CreateObject(BECR, 5270, 650, -1);

   //Hinweisschild
   var sign = CreateObject(SNPT, 1460, 500, -1);
   sign->SetAction("Sign2");
   sign->SetMode(1);

   //Versorgungskiste (Dragnin)
   var crate = CreateObject(AMCT, 560, 540, -1);
   crate->Set(DGNN);

   //Geschützstellungen
   CreateObject(GNET, 675, 540, -1)->Set(0,90);
   aStationary[0] = CreateObject(GNET, 3570, 430, -1);
   aStationary[0] -> Set(0,-90);
   aStationary[1] = CreateObject(GNET, 4205, 510, -1);
   aStationary[1] -> Set(0,-90);
   aStationary[2] = CreateObject(GNET, 5580, 570, -1);
   aStationary[2] -> Set(0,-90);

   //MAV-Station
   CreateObject(MVSN, 7690, 420, -1)->Set(7720,420,1);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1245,610,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2195,609,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,4640,649,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,6945,609,-1),50*21);

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Versorgungskisten (Kugeln)
    var crate = CreateObject(AMCT, 2700, 610, -1);
    crate->Set(ABOX);
    crate = CreateObject(AMCT, 3715, 570, -1);
    crate->Set(ABOX);
    crate = CreateObject(AMCT, 4320, 570, -1);
    crate->Set(ABOX);
    crate = CreateObject(AMCT, 6240, 720, -1);
    crate->Set(ABOX);

    //Gewehrgranaten
    PlaceSpawnpoint(GBOX, 2840, 688);
    PlaceSpawnpoint(GBOX, 7595, 475);
   }
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,3580,730,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,6430,440,GetTeamColor(2));}

   //Grenzen setzen
   CreateObject(BRDR, 3370, 0, -1)->Set(0);
   CreateObject(BRDR, 6870, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 3295, 470, -1);
   CreateObject(SGNP, 3415, 470, -1);
   CreateObject(SGNP, 3320, 680, -1);
   CreateObject(SGNP, 6620, 570, -1);
   CreateObject(SGNP, 6740, 610, -1);
   CreateObject(SGNP, 6870, 610, -1);

   //Geschützstellungen
   CreateObject(GNET, 3570, 430, -1)->Set(LCAC,0,1);
   CreateObject(GNET, 6590, 440, -1)->Set(LCAC,0,1);

   //Helikopter und Hinweisschilder
   if(!FindObject(NOHC))
   {
    SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,4700,390,-1),100*21);

    sign = CreateObject(SGNP, 4810, 420, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
   }

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,4640,649,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5565,650,-1),50*21);

   //Objekte entfernen
   RemoveObject(aSelfDefense[0]);
   RemoveObject(aSelfDefense[1]);
   RemoveObject(aSelfDefense[2]);
   RemoveObject(aSelfDefense[3]);
   RemoveObject(aArtillery[0]);
   RemoveObject(aArtillery[3]);
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   //Grenzen setzen
   CreateObject(BRDR, 3370, 0, -1)->Set(0);
   CreateObject(BRDR, 5980, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 3295, 470, -1);
   CreateObject(SGNP, 3415, 470, -1);
   CreateObject(SGNP, 3320, 680, -1);
   CreateObject(SGNP, 5865, 450, -1);
   CreateObject(SGNP, 5885, 610, -1);
   CreateObject(SGNP, 5950, 450, -1);

   //Geschützstellungen
   CreateObject(GNET, 3800, 510, -1)->Set(0,90,1);
   CreateObject(GNET, 5580, 570, -1)->Set(0,-90,1);

   //Helikopter und Hinweisschilder
   if(!FindObject(NOHC))
   {
    SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,4700,390,-1),100*21);

    sign = CreateObject(SGNP, 4810, 420, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
   }

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,4640,649,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5565,650,-1),50*21);

   //Objekte entfernen
   RemoveObject(aSelfDefense[0]);
   RemoveObject(aSelfDefense[1]);
   RemoveObject(aSelfDefense[2]);
   RemoveObject(aSelfDefense[3]);
   RemoveObject(aArtillery[0]);
   RemoveObject(aArtillery[3]);
  }
}

/* Assault Zerstörung */

public func OnAssaultTargetDestruction(object pTarget, int iTeam, int iIndex)
{
  //Ziel 1
  if(!iIndex)
  {
   //Grenzen neu setzen
   RemoveAll(BRDR);
   CreateObject(BRDR, 440, 0, -1)->Set(0,1);
   CreateObject(BRDR, 3200, 0, -1)->Set(1,1);

   //Teamgrenze setzen
   FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),1880,0,1,1);

   //Lampen ausschalten
   for(var obj in FindObjects(Find_Or(Find_ID(BLGH), Find_ID(LLGH)), Find_InRect(1000,330,680,310)))
   {
    obj->EMPShock();
    obj->TurnOff();
   }

   //Rauch
   CreateParticle("GunSmoke",1155,640,0,-10,100,1);
   CreateParticle("GunSmoke",1160,640,0,-10,250,1);
   CreateParticle("GunSmoke",1165,640,0,-10,350,1);
  }

  //Ziel 2
  if(iIndex == 1)
  {
   //Grenzen neu setzen
   RemoveAll(BRDR);
   CreateObject(BRDR, 1560, 0, -1)->Set(0,1);
   CreateObject(BRDR, 3930, 0, -1)->Set(1,1);

   //Teamgrenze setzen
   FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),2990,0,1,1);

   //Lampe ausschalten
   for(var obj in FindObjects(Find_ID(BLGH), Find_InRect(2670,470,40,30)))
   {
    obj->EMPShock();
    obj->TurnOff();
   }

   //Rauch
   CreateParticle("GunSmoke",2780,610,0,-10,100,1);
   CreateParticle("GunSmoke",2785,610,0,-10,250,1);
   CreateParticle("GunSmoke",2790,610,0,-10,350,1);
  }

  //Ziel 3
  if(iIndex == 2)
  {
   //Grenzen neu setzen
   RemoveAll(BRDR);
   CreateObject(BRDR, 2600, 0, -1)->Set(0,1);
   CreateObject(BRDR, 5140, 0, -1)->Set(1,1);

   //Teamgrenze setzen
   FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),3820,0,1,1);

   //Geschützstellung entfernen
   aStationary[0]->DecoExplode(30);

   //Artillerie entfernen
   aArtillery[1]->DecoExplode(45);

   //Lampe ausschalten
   for(var obj in FindObjects(Find_ID(BLGH), Find_InRect(3560,550,40,30)))
   {
    obj->EMPShock();
    obj->TurnOff();
   }
  }

  //Ziel 4
  if(iIndex == 3)
  {
   //Grenzen neu setzen
   RemoveAll(BRDR);
   CreateObject(BRDR, 3200, 0, -1)->Set(0,1);
   CreateObject(BRDR, 6210, 0, -1)->Set(1,1);

   //Teamgrenze setzen
   FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),4910,0,1,1);

   //Geschützstellung entfernen
   aStationary[1]->DecoExplode(30);

   //Lampen ausschalten
   for(var obj in FindObjects(Find_ID(BLGH), Find_InRect(4310,450,40,180)))
   {
    obj->EMPShock();
    obj->TurnOff();
   }
  }

  //Ziel 5
  if(iIndex == 4)
  {
   //Grenzen neu setzen
   RemoveAll(BRDR);
   CreateObject(BRDR, 4190, 0, -1)->Set(0,1);
   CreateObject(BRDR, 6950, 0, -1)->Set(1,1);

   //Teamgrenze setzen
   FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),5860,0,1,1);

   //Spawnpoint entfernen
   RemoveObject(FindObject2(Find_ID(VSPW),Find_InRect(4729, 389, 2, 2)));

   //Geschützstellung entfernen
   aStationary[2]->DecoExplode(30);

   //Artillerie entfernen
   aArtillery[2]->DecoExplode(45);

   //Lampen ausschalten
   for(var obj in FindObjects(Find_Or(Find_ID(BLGH), Find_ID(LLGH)), Find_InRect(5100,580,650,70)))
   {
    obj->EMPShock();
    obj->TurnOff();
   }

   //Zeitverzögertes Gewitter
   Schedule("GameCall(\"FormStorm\")", RandomX(900,2700));
  }

  //Ziel 6
  if(iIndex == 5)
  {
   //Grenzen neu setzen
   RemoveAll(BRDR);
   CreateObject(BRDR, 5040, 0, -1)->Set(0,1);
   CreateObject(BRDR, 7850, 0, -1)->Set(1,1);

   //Teamgrenze setzen
   FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),6560,0,1,1);

   //Lampe ausschalten
   for(var obj in FindObjects(Find_ID(BLGH), Find_InRect(6420,460,40,30)))
   {
    obj->EMPShock();
    obj->TurnOff();
   }
  }

  //Ziel 7
  if(iIndex == 6)
  {
   //Artillerie entfernen
   aArtillery[3]->DecoExplode(45);

   //SSA zerstören
   aSelfDefense[3]->Disarm();
   aSelfDefense[3]->DecoExplode(30);

   //Lampe ausschalten
   for(var obj in FindObjects(Find_ID(LLGH), Find_InRect(7270,490,30,30)))
   {
    obj->EMPShock();
    obj->TurnOff();
   }
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 1700; iY = 450;
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
    return [[3600, 500], [3675, 420]];
   }
   if(iTeam == 2)
   {
    return [[6325, 600], [6460, 600]];
   }
   return(1);
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    return [[3600, 500], [3675, 420]];
   }
   if(iTeam == 2)
   {
    return [[5700, 430], [5785, 600]];
   }
   return(1);
  }
}