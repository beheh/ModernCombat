/*-- Endless Outpost --*/

#strict

static aDoor, aTarget, aSelfDefense, aLamp, aLamp2;
static X, Y;


/* Regelvoreinstellung */

func ChooserRuleConfig()
{
  //Standardregelsatz: Belohnungssystem, Kein FriendlyFire, Waffen bleiben, Arena, Klassenwahl
  return [RWDS,NOFF,WPST,NODR,MCSL];
}

/* Initalisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Getaway.ogg;CMC_Offensive.ogg;CMC_Deep Universe.ogg;CMC_Friendly Unit.ogg;CMC_Moving Squad.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Rock Go On.ogg;CMC_Techno.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Techno.ogg");
  //Bildschirmfärbung
  SetGamma(RGB(0,0,0), RGB(80,80,80), RGB(200,200,200));
  //Türen
  aDoor = [];
  //Ziele
  aTarget = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Lampen
  aLamp = [];
  //Lampen2
  aLamp2 = [];
  //Szenario einrichten
  CreateFurniture();
  //Equipment plazieren
  CreateEquipment();
  //Spawnpointplazierungen
  X=CreateArray(2);
  Y=CreateArray(2);
  X[AS_GetAttackerTeam()]=130;
  Y[AS_GetAttackerTeam()]=340;
  X[AS_GetDefenderTeam()]=1320;
  Y[AS_GetDefenderTeam()]=250;

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

  return(1);
}

/* Plazierungslisten */

func CreateFurniture()
{
  var tmp;
  Log("$CreatingFurniture$");
  //Alle Objekte folglich von links nach rechts

  //Sternenhimmel
  CreateObject(SLSY, 10, 10, -1);
  CreateObject(SLSY, 10, 10, -1);
  CreateObject(SLSY, 10, 10, -1);
  CreateObject(SLSY, 10, 10, -1);

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

  //Stahltüren
  CreateObject(STDR, 815, 410, -1)->Lock();
  CreateObject(STDR, 875, 550, -1);
  CreateObject(STDR, 965, 550, -1);
  CreateObject(STDR, 1065, 260, -1);
  CreateObject(STDR, 1065, 430, -1);
  CreateObject(STDR, 1165, 260, -1);
  CreateObject(STDR, 1245, 260, -1);
  CreateObject(STDR, 1445, 200, -1);
  CreateObject(STDR, 1695, 290, -1);
  CreateObject(STDR, 1795, 210, -1);
  CreateObject(STDR, 1795, 290, -1);
  CreateObject(STDR, 1905, 400, -1);

  //Geschlossene Stahltüren
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
  CreateObject(MWCR, 1910, 320, -1);

  //Kisten
  CreateObject(WCR2, 10, 350, -1);
  CreateObject(WCR2, 355, 450, -1);
  CreateObject(WCR2, 1100, 362, -1);
  CreateObject(WCR2, 1460, 52, -1);
  CreateObject(WCR2, 1460, 70, -1);
  CreateObject(WCR2, 1540, 70, -1);
  CreateObject(WCR2, 1505, 200, -1);
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

  //Explosive Kisten
  CreateObject(XWCR, 1545, 320, -1);
  CreateObject(XWCR, 2150, 210, -1);
  CreateObject(XWCR, 2670, 410, -1);

  //Benzinfässer
  CreateObject(PBRL, 480, 450, -1);
  CreateObject(PBRL, 2325, 530, -1);

  //Explosivfässer
  CreateObject(XBRL, 1260, 550, -1);
  CreateObject(XBRL, 1890, 320, -1);
  CreateObject(XBRL, 2335, 530, -1);
  CreateObject(XBRL, 2420, 320, -1);
  CreateObject(XBRL, 2430, 320, -1);

  //Phosphorfässer
  CreateObject(HBRL, 1720, 290, -1);
  CreateObject(HBRL, 2165, 320, -1);

  //Benzintanks
  CreateObject(XTNK, 1610, 520, -1);
  CreateObject(XTNK, 1740, 210, -1);
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
  CreateObject(FENC, 30, 450, -1);
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

  //Munitionskisten (Splittergranaten)
  var tmp = CreateObject(AMCT, 2030, 210, -1);
  tmp->Set(FRAG);

  //Abwehrschilder
  PlaceSpawnpoint(RSHL, 190, 340);
  PlaceSpawnpoint(RSHL, 1935, 390);
  PlaceSpawnpoint(RSHL, 2035, 100);

  //EHPs
  PlaceSpawnpoint(FAPK, 895, 150);
  PlaceSpawnpoint(FAPK, 1965, 310);

  //Splittergranaten
  PlaceSpawnpoint(FRAG, 170, 340);
  PlaceSpawnpoint(FRAG, 1725, 105);

  //Feldgranaten
  PlaceSpawnpoint(FGRN, 400, 640);
  PlaceSpawnpoint(FGRN, 1080, 550);
  PlaceSpawnpoint(FGRN, 1330, 310);

  //Blendgranate
  PlaceSpawnpoint(STUN, 1140, 190);
}

func CreateAssaultObjects()
{
  Log("$CreatingAssaultObjects$");
  //Alle Objekte folglich von links nach rechts
  
  //Ziel 1
  aTarget[0] = CreateObject(CGLO,1120,362,-1);
  aTarget[0] -> SetName("$Target1$");

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

  aSelfDefense[0]=CreateObject(SEGU,945,249,-1);
  aSelfDefense[0]->Arm(MISA);
  aSelfDefense[0]->TurnOn();
  aSelfDefense[0]->SetAutoRepair(1500);
  aSelfDefense[0]->SetTeam(AS_GetDefenderTeam());
  CreateObject(CONS, 1050, 550, -1)
  ->Set(aSelfDefense[0]);

  aLamp[0]=CreateObject(BLGH,880,300,-1);
  aLamp[1]=CreateObject(BLGH,980,300,-1);
  aLamp[2]=CreateObject(CLGH,1045,475,-1);
  aLamp[3]=CreateObject(CLGH,1125,475,-1);

  //Ziel 2
  aTarget[1] = CreateObject(MVNT,1865,250,-1);
  aTarget[1] -> SetName("$Target2$");

  aDoor[5] = CreateObject(HNG2,1355,550,-1);
  aDoor[8] = CreateObject(HA4K, 1440, 323, -1);
   aDoor[8]->Lock();
   aDoor[8]->SetMaxDamage(-1);
  aDoor[9] = CreateObject(HA4K, 1650, 293, -1);
   aDoor[9]->Lock();
   aDoor[9]->SetMaxDamage(-1);

  aLamp[4]=CreateObject(LLGH,1183,380,-1);
  aLamp[5]=CreateObject(LLGH,1227,295,-1);
  aLamp[6]=CreateObject(STLH,1430,225,-1);
  aLamp[7]=CreateObject(STLH,1650,95,-1);
  aLamp2[0]=CreateObject(BLGH,1300,305,-1);
  aLamp2[1]=CreateObject(BLGH,1300,390,-1);
  aLamp2[2]=CreateObject(BLGH,1865,270,-1);

  //Ziel 3
  aTarget[2] = CreateObject(LBPC,1795,492,-1);
  aTarget[2] -> SetName("$Target3$");

  aDoor[6] = CreateObject(HNG2,1905,490,-1);
  aDoor[7] = CreateObject(SEDR,1850,110,-1);
   aDoor[7]->Lock();
   aDoor[7]->SetMaxDamage(-1);

  aSelfDefense[1]=CreateObject(SEGU,1495,369,-1);
  aSelfDefense[1]->Arm(MISA);
  aSelfDefense[1]->TurnOn();
  aSelfDefense[1]->SetAutoRepair(1500);
  aSelfDefense[1]->SetTeam(AS_GetDefenderTeam());

  aLamp2[3]=CreateObject(LBGH,1565,340,-1);
  aLamp2[4]=CreateObject(LBGH,1705,340,-1);
  aLamp2[5]=CreateObject(STLH,1955,360,-1);

  //Ziel 4
  aTarget[3] = CreateObject(CCP1,2180,512,-1);
  aTarget[3] -> SetName("$Target4$");

  //Ziel 5
  aTarget[4] = CreateObject(CCP1,2200,322,-1);
  aTarget[4] -> SetName("$Target5$");

  //Ziel 6
  aTarget[5] = CreateObject(CCP1,2350,212,-1);
  aTarget[5] -> SetName("$Target6$");

  //Ziel 7
  aTarget[6] = CreateObject(CCP1,2460,322,-1);
  aTarget[6] -> SetName("$Target7$");

  //Ziel 8
  aTarget[7] = CreateObject(CCP1,2455,502,-1);
  aTarget[7] -> SetName("$Target8$");

  //Ziele einstellen
  FindObject(GAS_)->AddTarget(aTarget[0],0,500);
  FindObject(GAS_)->AddTarget(aTarget[1],0,600);
  FindObject(GAS_)->AddTarget(aTarget[2],0,700);
  FindObject(GAS_)->AddTarget(aTarget[3],0,350);
  FindObject(GAS_)->AddTarget(aTarget[4],0,350);
  FindObject(GAS_)->AddTarget(aTarget[5],0,350);
  FindObject(GAS_)->AddTarget(aTarget[6],0,350);
  FindObject(GAS_)->AddTarget(aTarget[7],0,350);

  return(1);
}

/* Zielzerstörung */

func OnTarget1Destruction()
{
  //Positionen
  X[AS_GetAttackerTeam()]=1140;
  Y[AS_GetAttackerTeam()]=420;
  X[AS_GetDefenderTeam()]=1610;
  Y[AS_GetDefenderTeam()]=60;

  //Rauch
  CreateParticle("GunSmoke",925,340,0,-10,350,1);
  Smoke(925, 340, 30);

  //Türen öffnen
  aDoor[0]->Open();
  aDoor[1]->Open();
  aDoor[2]->Open();
  aDoor[3]->Unlock();
  aDoor[4]->Open();

  //Abwehranlage deaktivieren, sofern vorhanden
  if(aSelfDefense[0])
  {
   aSelfDefense[0]->SetAutoRepair(0);
   aSelfDefense[0]->Destroyed();
   aSelfDefense[0]->TurnOff();
  }

  //Lampen deaktivieren
  aLamp[0]->EMPShock();
  aLamp[1]->EMPShock();
  aLamp[2]->EMPShock();
  aLamp[3]->EMPShock();

  Sound("Announce1.ogg");
}

func OnTarget2Destruction()
{
  //Positionen
  X[AS_GetAttackerTeam()]=1290;
  Y[AS_GetAttackerTeam()]=420;
  X[AS_GetDefenderTeam()]=1990;
  Y[AS_GetDefenderTeam()]=390;

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
  aLamp[4]->EMPShock();
  aLamp[5]->EMPShock();
  aLamp[6]->EMPShock();
  aLamp[7]->EMPShock();
  aLamp2[0]->EMPShock();
  aLamp2[1]->EMPShock();
  aLamp2[2]->EMPShock();

  //Risse
  CreateObject(FAUD, 1845, 170, -1)->Set(2);
  CreateObject(FAUD, 1900, 180, -1)->Set(1);
  CreateObject(FAUD, 1850, 225, -1)->Set(3);
  CreateObject(FAUD, 1840, 280, -1)->Set(2);
  CreateObject(FAUD, 1890, 290, -1)->Set(3);

  Sound("Announce3.ogg");
}

func OnTarget3Destruction()
{
  //Positionen
  X[AS_GetAttackerTeam()]=1960;
  Y[AS_GetAttackerTeam()]=390;
  X[AS_GetDefenderTeam()]=1825;
  Y[AS_GetDefenderTeam()]=100;

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

  //Abwehranlage deaktivieren, sofern vorhanden
  if(aSelfDefense[1])
  {
   aSelfDefense[0]->SetAutoRepair(0);
   aSelfDefense[1]->Destroyed();
   aSelfDefense[1]->TurnOff();
  }

  //Lampen deaktivieren
  //aLamp2[3]->EMPShock();
  //aLamp2[4]->EMPShock();
  aLamp2[5]->EMPShock();

  //Risse
  CreateObject(FAUD, 1805, 430, -1)->Set(2);
  CreateObject(FAUD, 1750, 470, -1)->Set(1);
  CreateObject(FAUD, 1860, 460, -1)->Set(3);

  Sound("Announce4.ogg");
}

func OnTarget4Destruction()
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

func OnTarget5Destruction()
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

func OnTarget6Destruction()
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

func OnTarget7Destruction()
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

func OnTarget8Destruction()
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

/* Resets */

func PreResetAssault()
{
  X[AS_GetAttackerTeam()]=130;
  Y[AS_GetAttackerTeam()]=340;
  X[AS_GetDefenderTeam()]=1320;
  Y[AS_GetDefenderTeam()]=250;
}

func ResetAssault()
{
  CreateFurniture();
  CreateAssaultObjects();
  CreateEquipment();

  if(!FindObject(NOAM))
  {
   //Munitionskisten (Kugeln)
   var tmp = CreateObject (AMCT, 105, 450, -1);
   tmp->Set(GBOX);
   tmp->SetGraphics("Normal");
   var tmp = CreateObject (AMCT, 135, 450, -1);
   tmp->Set(ABOX);
   tmp->SetGraphics("Normal");
   var tmp = CreateObject (AMCT, 1090, 260, -1);
   tmp->Set(ABOX);
   tmp->SetGraphics("Normal");
   var tmp = CreateObject (AMCT, 1090, 430, -1);
   tmp->Set(GBOX);
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
  }
}

func ChooserFinished()
{
  CreateAssaultObjects();

  if(!FindObject(NOAM))
  {
   //Munitionskisten (Kugeln)
   var tmp = CreateObject (AMCT, 105, 450, -1);
   tmp->Set(GBOX);
   tmp->SetGraphics("Normal");
   var tmp = CreateObject (AMCT, 135, 450, -1);
   tmp->Set(ABOX);
   tmp->SetGraphics("Normal");
   var tmp = CreateObject (AMCT, 1090, 260, -1);
   tmp->Set(ABOX);
   tmp->SetGraphics("Normal");
   var tmp = CreateObject (AMCT, 1090, 430, -1);
   tmp->Set(GBOX);
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
  }
}

func Reset(obj, iTeam)
{
  var x=X[iTeam];
  var y=Y[iTeam];
  SetPosition(x,y,obj);

  DoEnergy(150,obj);
}

func ResetContents(obj)
{

  //obj->~StopAiming();
  var con, cnt;
  while(con = Contents(cnt++,obj))
  {
   RemoveObject(con);
   ClearScheduleCall(obj,"Contents(0)->~Selection(this());");
  }
}

/* Relaunch */

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  RelaunchPlayer(iPlr, GetCrew(iPlr), 0, iTeam);
  DoScoreboardShow(1,iPlr);
}

public func RelaunchPlayer(int iPlr, object pCrew, object pKiller, int iTeam)
{
  // Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return();
  // Kein Team
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);
  // Clonk tot?
  if(!GetAlive(pCrew))
    pCrew = RelaunchClonk(iPlr, pCrew);

  for(var i=0 ; pCrew = GetCrew(iPlr, i) ; i++)    
    {
    if(Contained(pCrew))
      Reset(Contained(pCrew),iTeam);
    else
      Reset(pCrew,iTeam);
    }
}

public func RelaunchClonk(int iPlr, object pCursor)
{
  var pClonk = CreateObject(PCMK, 10, 10, iPlr);
  if(pCursor)
    GrabObjectInfo(pCursor, pClonk);
  else
    MakeCrewMember(pClonk, iPlr);

  DoEnergy(+150, pClonk);
  SetCursor(iPlr, pClonk);
  SetPlrView(iPlr, pClonk);

  // Geldspritze
  if(FindObject(WPCH))
    DoWealth(iPlr, +25);

  // Wegstecken
  var tim = CreateObject(TIM2, 10, 10, -1);
  pClonk->Enter(tim);
  PlayerMessage(iPlr, Format("@%s", GetName(pClonk)), tim);

  return(pClonk);
}