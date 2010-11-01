/*-- Endless Outpost --*/

#strict
#include CSTD

static aFlag,aDoor,aSelfDefense,aLamp;


/* Initialisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg");
  Music("CMC_Striking Force.ogg");
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
  //Szenario einrichten
  CreateFurniture();
  //Equipment plazieren
  CreateEquipment();
  return(1);
}

/* Plazierungslisten */

func CreateFurniture()
{
  var tmp;
  Log("$CreatingFurniture$");
  //Alle Objekte folglich von links nach rechts

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

  //Nebel
  CreateParticle("Fog", 110, 600,0,0,900+Random(100));
  CreateParticle("Fog", 180, 640,0,0,400+Random(100));
  CreateParticle("Fog", 260, 630,0,0,700+Random(100));
  CreateParticle("Fog", 390, 610,0,0,1000+Random(100));
  CreateParticle("Fog", 515, 610,0,0,500+Random(100));
  CreateParticle("Fog", 590, 570,0,0,1000+Random(100));
  CreateParticle("Fog", 710, 550,0,0,500+Random(100));

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

  CreateObject(LADR, 1985, 320, -1)->Set(14);
  CreateObject(LADR, 2080, 480, -1)->Set(33);
  CreateObject(LADR, 2120, 480, -1)->Set(20);
  CreateObject(LADR, 2280, 490, -1)->Set(21);
  CreateObject(LADR, 2310, 490, -1)->Set(34);
  CreateObject(LADR, 2490, 500, -1)->Set(10);

  //Bodenlucken
  CreateObject(HA4K, 70, 353, -1);
  CreateObject(HA4K, 70, 453, -1);
  CreateObject(HA4K, 720, 413, -1);
  CreateObject(HA4K, 1480, 203, -1);
  CreateObject(HA4K, 2080, 213, -1);

  //Große Bodenlucken
  CreateObject(H24K, 985, 438, -1);
  CreateObject(H24K, 1205, 448, -1);
  CreateObject(H24K, 1985, 118, -1);

  //Schutztüren
  CreateObject(GDDR, 815, 410, -1)->Lock();
  CreateObject(GDDR, 875, 550, -1);
  CreateObject(GDDR, 965, 550, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1065, 260, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1065, 430, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1165, 260, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1245, 260, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1445, 200, -1);
  CreateObject(GDDR, 1695, 290, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1795, 210, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1795, 290, -1)->SetColorDw(HSL(200, 150, 100, 127));
  CreateObject(GDDR, 1905, 400, -1)->SetColorDw(HSL(200, 150, 100, 127));

  //Hydrauliktüren
  CreateObject(SEDR,2070, 110,-1)->Lock();
  CreateObject(SEDR,2560, 410,-1)->Lock();

  //Stahlbrücken
  CreateObject(_HBR, 305, 462, -1);
  CreateObject(_HBR, 405, 462, -1);
  CreateObject(_HBR, 505, 462, -1);
  CreateObject(_HBR, 2355, 332, -1);

  //Metallkisten
  CreateObject(MWCR, 1190, 570, -1);
  CreateObject(MWCR, 1210, 570, -1);
  CreateObject(MWCR, 1910, 320, -1)->AutoRespawn();

  //Kisten
  CreateObject(WCR2, 10, 350, -1)->AutoRespawn();
  CreateObject(WCR2, 355, 450, -1)->AutoRespawn();
  CreateObject(WCR2, 1100, 360, -1);
  CreateObject(WCR2, 1460, 52, -1);
  CreateObject(WCR2, 1460, 70, -1);
  CreateObject(WCR2, 1505, 200, -1)->AutoRespawn();
  CreateObject(WCR2, 1540, 70, -1)->AutoRespawn();
  CreateObject(WCR2, 2000, 456, -1);
  CreateObject(WCR2, 2000, 474, -1);
  CreateObject(WCR2, 2000, 492, -1);
  CreateObject(WCR2, 2000, 510, -1);
  CreateObject(WCR2, 2020, 492, -1);
  CreateObject(WCR2, 2020, 510, -1);
  CreateObject(WCR2, 2040, 510, -1);
  CreateObject(WCR2, 2060, 510, -1);
  CreateObject(WCR2, 2110, 110, -1);
  CreateObject(WCR2, 2145, 320, -1);
  CreateObject(WCR2, 2255, 320, -1);
  CreateObject(WCR2, 2405, 320, -1);
  CreateObject(WCR2, 2415, 500, -1);

  //Gasflaschen
  CreateObject(GSBL, 1020, 430, -1)->AutoRespawn();
  CreateObject(GSBL, 1640, 520, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 1545, 320, -1)->AutoRespawn();
  CreateObject(XWCR, 2150, 210, -1)->AutoRespawn();
  CreateObject(XWCR, 2670, 410, -1);

  //Benzinfässer
  CreateObject(PBRL, 480, 450, -1);
  CreateObject(PBRL, 2325, 530, -1);

  //Explosivfässer
  CreateObject(XBRL, 1260, 550, -1)->AutoRespawn();
  CreateObject(XBRL, 1890, 320, -1)->AutoRespawn();
  CreateObject(XBRL, 2335, 530, -1);
  CreateObject(XBRL, 2420, 320, -1);
  CreateObject(XBRL, 2430, 320, -1);

  //Phosphorfässer
  CreateObject(HBRL, 1720, 290, -1)->AutoRespawn();
  CreateObject(HBRL, 2165, 320, -1);

  //Benzintanks
  CreateObject(XTNK, 1610, 520, -1)->AutoRespawn();
  CreateObject(XTNK, 1740, 210, -1)->AutoRespawn();
  CreateObject(XTNK, 2800, 410, -1);

  //Container
  CreateObject(CON1, 220, 450, -1);
  CreateObject(CON1, 457, 450, -1)->SetPerspective(2);
  CreateObject(CON1, 1485, 550, -1);
  CreateObject(CON1, 1495, 520, -1)->SetPerspective(2);
  CreateObject(CON1, 1680, 520, -1);
  CreateObject(CON1, 2170, 110, -1);
  CreateObject(CON1, 2570, 110, -1);
  CreateObject(CON1, 2670, 110, -1);
  CreateObject(CON1, 2720, 410, -1);

  //Bildschirme
  CreateObject(SCA1, 995, 350, -1);
  tmp = CreateObject(SCR3, 995, 380, -1);
  tmp->SetClrModulation(RGB(255,255,255));

  tmp = CreateObject(SCA2, 1570, 150, -1);
  tmp->SetClrModulation(RGB(255,255,255));
  tmp = CreateObject(SCR3, 1580, 180, -1);
  tmp->SetClrModulation(RGB(255,255,255));
  tmp = CreateObject(SCR1, 1580, 230, -1);
  tmp->SetClrModulation(RGB(255,255,255));
  tmp = CreateObject(SCR3, 1580, 260, -1);
  tmp->SetClrModulation(RGB(255,255,255));
  tmp = CreateObject(SCR3, 2200, 210, -1);
  tmp->SetClrModulation(RGB(255,255,255));
  tmp = CreateObject(SCA2, 2200, 400, -1);
  tmp->SetAction("Scan2");
  tmp = CreateObject(SCR3, 2445, 450, -1);
  tmp->SetClrModulation(RGB(255,255,255));

  //Labortische
  CreateObject(LTBL,1410, 320, -1);
  CreateObject(LTBL,1510, 320, -1);
  CreateObject(LTBL,1575, 70, -1);
  CreateObject(LTBL,1610, 290, -1);
  CreateObject(LTBL,2300, 530, -1);

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

  //Monitore
  CreateObject(MONI, 1420, 308, -1);
  CreateObject(MONI, 1510, 308, -1);
  CreateObject(MONI, 1520, 308, -1);
  CreateObject(MONI, 1600, 278, -1);
  CreateObject(MONI, 2285, 518, -1);
  CreateObject(MONI, 2295, 518, -1);
  CreateObject(MONI, 2305, 518, -1);

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

  //Dekoschleusen
  CreateObject(GAT1, 925, 350, -1);
  CreateObject(GAT1, 2200, 260, -1);
  CreateObject(GAT1, 2200, 445, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 1430, 440, -1);
  CreateObject(ENGT, 1615, 70, -1);
  CreateObject(ENGT, 1650, 440, -1);
  CreateObject(ENGT, 1870, 440, -1);

  //Automat
  CreateObject(CLVM, 1895, 110, -1);
  CreateObject(CLVM, 1980, 400, -1);

  //Warnschild
  CreateObject(EXSN, 1700, 200, -1);

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
  var obj0 = CreateObject(TRE4,521,606,-1);
  obj0->SetCon(59);
  CreateObject(TRE3,546,625,-1);
  CreateObject(TRB4,552,625,-1);
  CreateObject(TRE2,576,607,-1);
  CreateObject(TRE2,677,582,-1);
  CreateObject(BSH2,739,568,-1);
  CreateObject(BSH2,97,658,-1);
  var obj1 = CreateObject(TRE1,127,657,-1);
  obj1->SetCon(40);
  CreateObject(TRE4,102,656,-1);
  CreateObject(TRE4,112,657,-1);
  CreateObject(TRE4,127,656,-1);
  CreateObject(TRB4,169,674,-1);
  CreateObject(SE4K,210,620,-1);
  var obj2 = CreateObject(TRE3,288,664,-1);
  obj2->SetCon(54);
  CreateObject(TRE4,288,665,-1);
  CreateObject(TRE4,255,661,-1);
  CreateObject(TRE2,290,667,-1);
  CreateObject(TRE1,272,658,-1);
  var obj3 = CreateObject(TRE4,290,667,-1);
  obj3->SetCon(64);
  CreateObject(TRE4,275,656,-1);
  var obj4 = CreateObject(GRAS,276,652,-1);
  obj4->SetR(8);
  obj4->SetCon(59);
  CreateObject(TRB4,307,674,-1);
  CreateObject(TRE2,300,667,-1);
  CreateObject(TRB4,374,656,-1);
  var obj5 = CreateObject(GRAS,372,653,-1);
  obj5->SetR(-15);
  obj5->SetCon(58);
  CreateObject(BSH2,437,655,-1);
  CreateObject(TRE2,427,657,-1);
  CreateObject(TRE3,434,653,-1);
  var obj7 = CreateObject(GRAS,432,651,-1);
  obj7->SetR(-1);
  obj7->SetCon(43);
  var obj8 = CreateObject(TRE1,486,651,-1);
  obj8->SetCon(40);
  var obj9 = CreateObject(GRAS,480,653,-1);
  obj9->SetR(-30);
  obj9->SetCon(55);
  CreateObject(TRE4,494,632,-1);
  CreateObject(TRE4,493,634,-1);
  var obj10 = CreateObject(GRAS,492,631,-1);
  obj10->SetR(-27);
  obj10->SetCon(56);
  var obj11 = CreateObject(TRE4,546,627,-1);
  obj11->SetCon(36);
  var obj12 = CreateObject(TRE4,504,627,-1);
  obj12->SetCon(38);
  var obj13 = CreateObject(TRE2,508,622,-1);
  obj13->SetCon(30);
  var obj14 = CreateObject(GRAS,504,623,-1);
  obj14->SetR(-18);
  obj14->SetCon(56);
  CreateObject(BSH2,674,640,-1);
  CreateObject(TRB4,655,641,-1);
  CreateObject(STNE,735,665,-1);
  CreateObject(BSH2,149,666,-1);
  var obj15 = CreateObject(TRE4,144,661,-1);
  obj15->SetCon(30);
  CreateObject(BSH2,153,667,-1);
  var obj16 = CreateObject(TRE4,176,666,-1);
  obj16->SetCon(11);
  CreateObject(STNE,220,690,-1);
  var obj17 = CreateObject(BSH2,297,666,-1);
  obj17->SetCon(10);
  var obj18 = CreateObject(GRAS,288,661,-1);
  obj18->SetR(11);
  obj18->SetCon(58);
  CreateObject(BSH2,290,667,-1);
  var obj19 = CreateObject(GRAS,336,700,-1);
  obj19->SetR(13);
  obj19->SetCon(60);
  CreateObject(TRE2,312,686,-1);
  CreateObject(BSH2,303,670,-1);
  var obj20 = CreateObject(BSH2,338,705,-1);
  obj20->SetCon(59);
  var obj21 = CreateObject(BSH2,331,699,-1);
  obj21->SetCon(15);
  CreateObject(TRE4,312,686,-1);
  var obj22 = CreateObject(GRAS,312,682,-1);
  obj22->SetR(23);
  obj22->SetCon(55);
  var obj23 = CreateObject(GRAS,324,693,-1);
  obj23->SetR(10);
  obj23->SetCon(60);
  var obj24 = CreateObject(GRAS,300,661,-1);
  obj24->SetR(8);
  obj24->SetCon(44);
  CreateObject(BSH2,420,705,-1);

  //Büsche
  CreateObject(BSH2, 220, 380, -1);
  CreateObject(BSH2, 25, 570, -1);
  CreateObject(BSH2, 420, 705, -1);
  CreateObject(BSH2, 710, 615, -1);
  CreateObject(BSH2, 855, 520, -1);
  CreateObject(BSH2, 1515, 630, -1);
  CreateObject(BSH2, 2320, 610, -1);

  //Steine
  CreateObject(STNE, 220, 690, -1);
  CreateObject(STNE, 735, 665, -1);

  //Satellitenschüssel
  CreateObject(RADR, 855, 160, -1);
  CreateObject(RADR, 1785, 30, -1);

  //Zäune
  CreateObject(FENC, 30, 450, -1)->Set(4);
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

  //Notausgangslichter
  CreateObject(ETLT, 870, 390, -1);
  CreateObject(ETLT, 1220, 235, -1);
  CreateObject(ETLT, 1480, 170, -1);
  CreateObject(ETLT, 2530, 395, -1);

  //Hydrauliküren
  aDoor[0] = CreateObject(SEDR,1225, 560,-1);
   aDoor[0]->Lock();
   aDoor[0]->SetMaxDamage(-1);
  aDoor[1] = CreateObject(SEDR,1245, 430,-1);
   aDoor[1]->Lock();
   aDoor[1]->SetMaxDamage(-1);
  aDoor[2] = CreateObject(SEDR,1355, 260,-1);
   aDoor[2]->Lock();
   aDoor[2]->SetMaxDamage(-1);
  aDoor[3] = CreateObject(H24K, 1205, 208, -1);
   aDoor[3]->Lock();
   aDoor[3]->SetMaxDamage(-1);
  aDoor[4] = CreateObject(HA4K, 850, 163, -1);
   aDoor[4]->Lock();
   aDoor[4]->SetMaxDamage(-1);
  aDoor[5] = CreateObject(HNG2,1355,550,-1);
  aDoor[6] = CreateObject(HNG2,1905,490,-1);
  aDoor[7] = CreateObject(SEDR,1850,110,-1);
   aDoor[7]->Lock();
   aDoor[7]->SetMaxDamage(-1);
  aDoor[8] = CreateObject(HA4K, 1440, 323, -1);
   aDoor[8]->Lock();
   aDoor[8]->SetMaxDamage(-1);
  aDoor[9] = CreateObject(HA4K, 1650, 293, -1);
   aDoor[9]->Lock();
   aDoor[9]->SetMaxDamage(-1);

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
  aSelfDefense[0]->SetAutoRepair(1500);
  CreateObject(CONS, 1050, 550, -1)->Set(aSelfDefense[0]);

  aSelfDefense[1]=CreateObject(SEGU,1495,369,-1);
  aSelfDefense[1]->Arm(MISA);
  aSelfDefense[1]->SetAutoRepair(1500);

  //Sounds

  //Wind
  CreateObject(SE4K, 310, 380, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 540, 380, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 1000, 330, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1320, 380, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1570, 440, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1650, 190, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2040, 280, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2365, 400, -1)->Set("Interior*.ogg",670,105);

  //Eule
  CreateObject(SE4K, 210, 620, -1)->Set("Owl.wav",1400,35);

  //Erdrutsche
  CreateObject(SE4K, 30, 560, -1)->Set("FallingDirt*.ogg",950,250);
  CreateObject(SE4K, 760, 580, -1)->Set("FallingDirt*.ogg",850,250);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

   //Munitionskisten (Kugeln)
   var tmp = CreateObject (AMCT, 135, 450, -1);
   tmp->Set(ABOX);
   tmp->SetGraphics("Normal");
   var tmp = CreateObject (AMCT, 1090, 260, -1);
   tmp->Set(ABOX);
   tmp->SetGraphics("Normal");
   var tmp = CreateObject (AMCT, 1280, 550, -1);
   tmp->Set(ABOX);
   tmp->SetGraphics("Normal");
   var tmp = CreateObject (AMCT, 1720, 210, -1);
   tmp->Set(ABOX);
   tmp->SetGraphics("Normal");
   var tmp = CreateObject (AMCT, 1835, 490, -1);
   tmp->Set(ABOX);
   tmp->SetGraphics("Normal");
   var tmp = CreateObject (AMCT, 2375, 530, -1);
   tmp->Set(ABOX);
   tmp->SetGraphics("Normal");

   //Munitionskisten (Granaten)
   var tmp = CreateObject (AMCT, 105, 450, -1);
   tmp->Set(GBOX);
   tmp->SetGraphics("Normal");
   var tmp = CreateObject (AMCT, 1090, 430, -1);
   tmp->Set(GBOX);
   tmp->SetGraphics("Normal");

  //Munitionskisten (Splittergranaten)
  var tmp = CreateObject(AMCT, 2030, 210, -1);
  tmp->Set(FRAG);
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

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //DM/LMS-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS))
  {
   //SSA entfernen
   RemoveObject(aSelfDefense[0]);
   RemoveObject(aSelfDefense[1]);

   //Grenze setzen
   CreateObject(BRDR, 2010, 0, -1)->Set(1);

   //Türen öffnen
   aDoor[0]->Unlock();
   aDoor[1]->Unlock();
   aDoor[2]->Unlock();
   aDoor[3]->Unlock();
   aDoor[4]->Unlock();
   aDoor[5]->Open();
   aDoor[8]->Unlock();
   aDoor[9]->Unlock();

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

   //Munitionskiste entfernen
   RemoveObject(FindObject2(Find_ID(AMCT),Find_InRect(1080, 240, 40, 40)));

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Raketen
    PlaceSpawnpoint(MBOX, 830, 155);
    PlaceSpawnpoint(MBOX, 1430, 385);

    //Granaten
    PlaceSpawnpoint(GBOX, 1530, 545);
   }
  }

  //Assault-Spielziel
  if (FindObject(GASS))
  {
   //Zielobjekte
   AddAssaultTarget(CGLO, 1120, 362, 250, 2, "$Target1$", 0, [[[1320, 260], [1300, 260], [1130, 260]], [[130, 350], [170, 350], [150, 450]]]);
   AddAssaultTarget(MVNT, 1865, 250, 250, 2, "$Target2$", 1, [[[1570, 70], [1590, 70], [1830, 110]], [[1100, 430], [1140, 430], [1280, 550]]]);
   AddAssaultTarget(LBPC, 1795, 492, 300, 2, "$Target3$", 2, [[[1980, 400]], [[1280, 430], [1280, 550]]]);
   AddAssaultTarget(CCP2, 2180, 512, 200, 2, "$Target4$", 3, [[[1880, 110], [2050, 110]], [[1950, 400], [1550, 550], [1500, 490]]]);
   AddAssaultTarget(LBPC, 2200, 322, 200, 2, "$Target5$", 4, [[[1880, 110], [2050, 110]], [[1950, 400], [1550, 550], [1500, 490]]]);
   AddAssaultTarget(CMSN, 2350, 212, 200, 2, "$Target6$", 5, [[[1880, 110], [2050, 110]], [[1950, 400], [1550, 550], [1500, 490]]]);
   AddAssaultTarget(GSTA, 2460, 322, 200, 2, "$Target7$", 6, [[[1880, 110], [2050, 110]], [[1950, 400], [1550, 550], [1500, 490]]]);
   AddAssaultTarget(CCP1, 2455, 502, 200, 2, "$Target8$", 7, [[[1880, 110], [2050, 110]], [[1950, 400], [1550, 550], [1500, 490]]]);

   //Ziele verbinden
   ConnectAssaultTargets([3, 4, 5, 6, 7]);

   //SSA Besitzer setzen
   if(aTeams[2] == true)
   {aSelfDefense[0]->SetTeam(2); aSelfDefense[1]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,160,350,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(120,640);
   aFlag[0] -> AddSpawnPoint(355,690);
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

   //Konsole erstellen
   CreateObject(CONS, 1810, 485, -1)->Set(aSelfDefense[1]);

   //Grenze setzen
   CreateObject(BRDR, 2010, 0, -1)->Set(1);

   //Türen öffnen
   aDoor[0]->Unlock();
   aDoor[1]->Unlock();
   aDoor[2]->Unlock();
   aDoor[3]->Unlock();
   aDoor[4]->Unlock();
   aDoor[5]->Open();
   aDoor[8]->Unlock();
   aDoor[9]->Unlock();

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

   //Munitionskiste entfernen
   RemoveObject(FindObject2(Find_ID(AMCT),Find_InRect(1080, 240, 40, 40)));

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Raketen
    PlaceSpawnpoint(MBOX, 830, 155);
    PlaceSpawnpoint(MBOX, 1430, 385);

    //Granaten
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
   aDoor[0]->Open();
   aDoor[1]->Open();
   aDoor[2]->Open();
   aDoor[3]->Unlock();
   aDoor[4]->Open();

   //SSA zerstören
   aSelfDefense[0]->Disarm();
   DecoExplode(30, aSelfDefense[0]);

   //Lampen deaktivieren
   aLamp[00]->EMPShock();
   aLamp[01]->EMPShock();
   aLamp[02]->EMPShock();
   aLamp[03]->EMPShock();

   Sound("Announce1.ogg");
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
   aDoor[5]->Open();
   aDoor[8]->Open();
   aDoor[9]->Open();

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

   Sound("Announce3.ogg");
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
   aDoor[6]->Open();
   aDoor[7]->Open();

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

   Sound("Announce4.ogg");
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

   Sound("Announce1.ogg");
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

   Sound("Announce4.ogg");
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

   Sound("Announce3.ogg");
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

   Sound("Announce4.ogg");
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

   Sound("Announce4.ogg");
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