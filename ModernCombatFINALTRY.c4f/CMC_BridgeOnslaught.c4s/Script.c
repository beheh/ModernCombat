/*-- Bridge Onslaught --*/

#strict
#include CSTD

static aFlag, aSelfDefense, switchright, switchleft;


/* Initalisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg");
  Music("CMC_Getaway.ogg");
  //Hintergrundbewegung
  SetSkyParallax(0,15,15);
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
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

  //Leitern
  CreateObject(LADR, 400, 630, -1)->Set(24);
  CreateObject(LADR, 495, 421, -1)->Set(18);
  CreateObject(LADR, 585, 775, -1)->Set(16);
  CreateObject(LADR, 755, 623, -1)->Set(22);
  CreateObject(LADR, 810, 778, -1)->Set(15);

  CreateObject(LADR, 1305, 621, -1)->Set(38);
  CreateObject(LADR, 1365, 791, -1)->Set(18);
  CreateObject(LADR, 1425, 621, -1)->Set(38);

  CreateObject(LADR, 1920, 778, -1)->Set(15);
  CreateObject(LADR, 1975, 623, -1)->Set(22);
  CreateObject(LADR, 2145, 775, -1)->Set(16);
  CreateObject(LADR, 2235, 421, -1)->Set(18);
  CreateObject(LADR, 2330, 630, -1)->Set(24);

  //Rampen
  DrawMaterialQuad("Wall-Concrete3",330,440,360,430,360,440,345,440,true);
  DrawMaterialQuad("Wall-Concrete3",471,440,441,430,441,440,456,440,true);

  DrawMaterialQuad("Wall-Concrete3",331,641,361,631,361,641,346,641,true);
  DrawMaterialQuad("Wall-Concrete3",470,641,440,631,440,641,455,641,true);

  DrawMaterialQuad("Wall-Concrete3",801,650,771,640,771,650,786,650,true);
  DrawMaterialQuad("Wall-Concrete3",820,650,850,640,850,650,835,650,true);

  DrawMaterialQuad("Wall-Concrete3",1911,650,1881,640,1881,650,1896,650,true);
  DrawMaterialQuad("Wall-Concrete3",1930,650,1960,640,1960,650,1945,650,true);

  DrawMaterialQuad("Wall-Concrete3",2261,641,2291,631,2291,641,2276,641,true);
  DrawMaterialQuad("Wall-Concrete3",2400,641,2370,631,2370,641,2385,641,true);

  DrawMaterialQuad("Wall-Concrete3",2260,440,2290,430,2290,440,2275,440,true);
  DrawMaterialQuad("Wall-Concrete3",2401,440,2371,430,2371,440,2386,440,true);

  //Bodenlucken
  CreateObject(HA4K, 400, 433, -1);
  CreateObject(HA4K, 810, 653, -1);

  CreateObject(HA4K, 1920, 653, -1);
  CreateObject(HA4K, 2330, 433, -1);

  //Große Bodenlucken
  CreateObject(H24K, 585, 648, -1);
  CreateObject(H24K, 755, 448, -1);

  CreateObject(H24K, 1305, 318, -1);
  CreateObject(H24K, 1305, 448, -1);
  CreateObject(H24K, 1305, 528, -1);

  CreateObject(H24K, 1365, 648, -1);

  CreateObject(H24K, 1425, 318, -1);
  CreateObject(H24K, 1425, 448, -1);
  CreateObject(H24K, 1425, 528, -1);

  CreateObject(H24K, 1975, 448, -1);
  CreateObject(H24K, 2145, 648, -1);

  //Hydrauliktüren
  tmp = CreateObject(SLDR, 270, 530, -1);
  tmp->Lock();
  tmp->SetMaxDamage(-1);
  tmp->SetSwitchLock(DIR_Right);

  tmp = CreateObject(SLDR, 2460, 530, -1);
  tmp->Lock();
  tmp->SetMaxDamage(-1);
  tmp->SetSwitchLock(DIR_Left);

  //Verbundene Türen
  var doorw = CreateObject(ROOM, 200, 530, -1);
  CreateObject(ROOM, 2530, 530, -1)->Connect(doorw);

  //Geländer
  CreateObject(RAI1, 505, 270, -1)->SetRail([1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 725, 440, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1227, 310, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1945, 440, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 2075, 270, -1)->SetRail([1,1,1,1,1,1,1,1]);

  //Glasscheiben
  CreateObject(_WIN, 1262, 510, -1);
  CreateObject(_WIN, 1262, 520, -1);

  CreateObject(_WIN, 1468, 510, -1);
  CreateObject(_WIN, 1468, 520, -1);

  //Satellitenschüsseln
  CreateObject(RADR, 1020, 310, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(RADR, 1700, 310, -1)->SetClrModulation(RGB(125,125,125));

  //Spinde
  CreateObject(LCKR, 220, 530, -1);
  CreateObject(LCKR, 240, 530, -1);
  CreateObject(LCKR, 2490, 530, -1);
  CreateObject(LCKR, 2510, 530, -1);

  //Automaten
  CreateObject(SPVM, 615, 800, -1);
  CreateObject(CLVM, 2115, 800, -1);

  //Glastische
  CreateObject(GTBL, 660, 800, -1);
  CreateObject(GTBL, 2065, 800, -1);

  //Wandlampen
  CreateObject(BLGH, 635, 400, -1);
  CreateObject(BLGH, 635, 600, -1);
  CreateObject(BLGH, 1075, 400, -1);

  CreateObject(BLGH, 1655, 400, -1);
  CreateObject(BLGH, 2095, 400, -1);
  CreateObject(BLGH, 2095, 600, -1);

  //Notausgangslichter
  CreateObject(ETLT, 750, 770, -1);
  CreateObject(ETLT, 1980, 770, -1);

  //Bildschirme
  CreateObject(SCR3, 360, 520, -1);
  CreateObject(SCA2, 680, 765, -1)->SetAction("Clonk");
  CreateObject(SCA2, 2045, 765, -1)->SetAction("Clonk");
  CreateObject(SCR3, 2370, 520, -1);

  //Monitore
  CreateObject(MONI, 655, 788, -1)->On();
  CreateObject(MONI, 665, 788, -1)->On();

  CreateObject(MONI, 2055, 788, -1)->On();
  CreateObject(MONI, 2065, 788, -1)->On();

  //Dekoschleusen
  CreateObject(GAT1, 1075, 520, -1);
  CreateObject(GAT1, 1655, 520, -1);

  //Explosivtanks
  CreateObject(XTNK, 230, 440, -1)->AutoRespawn();
  CreateObject(XTNK, 1200, 800, -1)->AutoRespawn();
  CreateObject(XTNK, 1530, 800, -1)->AutoRespawn();
  CreateObject(XTNK, 2500, 440, -1)->AutoRespawn();

  //Container
  CreateObject(CON1, 25, 440, -1)->SetPerspective(2);
  CreateObject(CON1, 75, 440, -1);
  CreateObject(CON1, 120, 640, -1);
  CreateObject(CON1, 250, 640, -1)->SetPerspective(3);
  CreateObject(CON1, 265, 614, -1)->SetPerspective(2);

  CreateObject(CON1, 530, 440, -1)->SetPerspective(2);
  CreateObject(CON1, 530, 640, -1);
  CreateObject(CON1, 1365, 440, -1)->SetPerspective(3);
  CreateObject(CON1, 2200, 440, -1)->SetPerspective(2);
  CreateObject(CON1, 2200, 640, -1);

  CreateObject(CON1, 2470, 640, -1);
  CreateObject(CON1, 2500, 614, -1)->SetPerspective(2);
  CreateObject(CON1, 2530, 640, -1);
  CreateObject(CON1, 2680, 440, -1)->SetPerspective(3);

  //Metallkiste
  CreateObject(MWCR, 75, 413, -1);
  CreateObject(MWCR, 295, 640, -1);

  CreateObject(MWCR, 570, 800, -1)->AutoRespawn();
  CreateObject(MWCR, 1065, 640, -1)->AutoRespawn();

  CreateObject(MWCR, 1395, 310, -1)->AutoRespawn();

  CreateObject(MWCR, 1665, 640, -1)->AutoRespawn();
  CreateObject(MWCR, 2160, 800, -1)->AutoRespawn();

  CreateObject(MWCR, 2635, 440, -1);

  //Kisten
  CreateObject(WCR2, 590, 800, -1);
  CreateObject(WCR2, 610, 640, -1)->AutoRespawn();
  CreateObject(WCR2, 915, 640, -1);
  CreateObject(WCR2, 1270, 440, -1)->AutoRespawn();
  CreateObject(WCR2, 1400, 640, -1)->AutoRespawn();
  CreateObject(WCR2, 1415, 640, -1);
  CreateObject(WCR2, 1405, 622, -1);
  CreateObject(WCR2, 1815, 640, -1);
  CreateObject(WCR2, 2120, 640, -1)->AutoRespawn();
  CreateObject(WCR2, 2140, 800, -1);

  //Sandsackbarrieren
  CreateObject(SBBA, 790, 800, -1)->Right();
  CreateObject(SBBA, 960, 640, -1);
  CreateObject(SBBA, 1160, 800, -1);
  CreateObject(SBBA, 1260, 640, -1);
  CreateObject(SBBA, 1470, 640, -1)->Right();
  CreateObject(SBBA, 1570, 800, -1)->Right();
  CreateObject(SBBA, 1770, 640, -1)->Right();
  CreateObject(SBBA, 1940, 800, -1);

  //Stahlbrücken
  CreateObject(_HBR, 595, 452, -1);
  CreateObject(_HBR, 685, 452, -1);

  CreateObject(_HBR, 825, 452, -1);
  CreateObject(_HBR, 925, 452, -1);
  CreateObject(_HBR, 1025, 452, -1);
  CreateObject(_HBR, 1125, 452, -1);
  CreateObject(_HBR, 1225, 452, -1);

  CreateObject(_HBR, 915, 652, -1);
  CreateObject(_HBR, 1115, 652, -1);
  CreateObject(_HBR, 1215, 652, -1);

  CreateObject(_HBR, 1515, 652, -1);
  CreateObject(_HBR, 1615, 652, -1);
  CreateObject(_HBR, 1815, 652, -1);

  CreateObject(_HBR, 1505, 452, -1);
  CreateObject(_HBR, 1605, 452, -1);
  CreateObject(_HBR, 1705, 452, -1);
  CreateObject(_HBR, 1805, 452, -1);
  CreateObject(_HBR, 1905, 452, -1);

  CreateObject(_HBR, 2045, 452, -1);
  CreateObject(_HBR, 2135, 452, -1);

  //Flutlichter
  CreateObject(FLGH, 190, 440, -1)->SetRotation(30);
  CreateObject(FLGH, 2535, 440, -1)->SetRotation(-30);

  //Gasflaschen
  CreateObject(GSBL, 540, 613, -1)->AutoRespawn();
  CreateObject(GSBL, 630, 800, -1)->AutoRespawn();
  CreateObject(GSBL, 995, 439, -1);
  CreateObject(GSBL, 1045, 640, -1)->AutoRespawn();
  CreateObject(GSBL, 1685, 640, -1)->AutoRespawn();
  CreateObject(GSBL, 1735, 439, -1);
  CreateObject(GSBL, 2100, 800, -1)->AutoRespawn();
  CreateObject(GSBL, 2190, 613, -1)->AutoRespawn();

  //Explosive Kiste
  CreateObject(XWCR, 975, 440, -1)->AutoRespawn();
  CreateObject(XWCR, 1755, 440, -1)->AutoRespawn();

  //Giftfässer
  CreateObject(TBRL, 435, 430, -1)->AutoRespawn();
  CreateObject(TBRL, 2295, 430, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 990, 800, -1)->AutoRespawn();
  CreateObject(XBRL, 1225, 800, -1)->AutoRespawn();
  CreateObject(XBRL, 1350, 310, -1)->AutoRespawn();
  CreateObject(XBRL, 1370, 310, -1)->AutoRespawn();
  CreateObject(XBRL, 1290, 640, -1)->AutoRespawn();
  CreateObject(XBRL, 1505, 800, -1)->AutoRespawn();
  CreateObject(XBRL, 1745, 800, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 1360, 310, -1)->AutoRespawn();
  CreateObject(HBRL, 1300, 640, -1)->AutoRespawn();

  //Tore
  CreateObject (HNG2, 175, 440, -1);
  CreateObject (HNG2, 365, 630, -1);
  CreateObject (HNG2, 2365, 630, -1);
  CreateObject (HNG2, 2555, 440, -1);

  //Zäune
  CreateObject(FENC, 210, 440, -1);
  CreateObject(FENC, 270, 440, -1);
  CreateObject(FENC, 330, 440, -1);

  CreateObject(FENC, 470, 640, -1);
  CreateObject(FENC, 530, 640, -1);
  CreateObject(FENC, 590, 640, -1);
  CreateObject(FENC, 650, 640, -1);
  CreateObject(FENC, 710, 640, -1);

  CreateObject(FENC, 2020, 640, -1);
  CreateObject(FENC, 2080, 640, -1);
  CreateObject(FENC, 2140, 640, -1);
  CreateObject(FENC, 2200, 640, -1);
  CreateObject(FENC, 2260, 640, -1);

  CreateObject(FENC, 2400, 440, -1);
  CreateObject(FENC, 2460, 440, -1);
  CreateObject(FENC, 2520, 440, -1);

  //Regale
  CreateObject(FRAM, 755, 800, -1);
  CreateObject(FRAM, 775, 800, -1);

  CreateObject(FRAM, 1330, 520, -1);
  CreateObject(FRAM, 1400, 520, -1);

  CreateObject(FRAM, 1950, 800, -1);
  CreateObject(FRAM, 1970, 800, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 40, 430, -1);
  CreateObject(ENGT, 140, 430, -1);

  CreateObject(ENGT, 40, 630, -1);
  CreateObject(ENGT, 140, 630, -1);
  CreateObject(ENGT, 240, 630, -1);
  CreateObject(ENGT, 340, 630, -1);

  CreateObject(ENGT, 2390, 630, -1);
  CreateObject(ENGT, 2490, 630, -1);
  CreateObject(ENGT, 2590, 630, -1);
  CreateObject(ENGT, 2690, 630, -1);

  CreateObject(ENGT, 2590, 430, -1);
  CreateObject(ENGT, 2690, 430, -1);

  //Feuerlöcher
  CreateObject(FIEX, 115, 430, -1);
  CreateObject(FIEX, 315, 630, -1);
  CreateObject(FIEX, 1285, 510, -1);
  CreateObject(FIEX, 2415, 630, -1);
  CreateObject(FIEX, 2615, 430, -1);

  //Büsche
  CreateObject(BSH2, 505, 515, -1);
  CreateObject(BSH2, 1350, 815, -1)->SetR(-50);
  CreateObject(BSH2, 1365, 815, -1)->SetR(90);
  CreateObject(BSH2, 1390, 820, -1);
  CreateObject(BSH2, 1455, 570, -1);
  CreateObject(BSH2, 1780, 820, -1);
  CreateObject(BSH2, 2190, 515, -1);

  //Steine
  CreateObject(STNE, 1290, 800, -1);
  CreateObject(STNE, 1450, 800, -1);

  //Ventillatoren
  CreateObject(VEN3, 490, 595, -1)->SetCon(20);
  tmp = CreateObject(VEN3, 510, 595, -1);
  tmp->SetCon(20);
  tmp->SetPhase(4);
  tmp = CreateObject(VEN3, 530, 595, -1);
  tmp->SetCon(20);
  tmp->SetPhase(7);

  CreateObject(VENT, 975, 595, -1)->SetCon(20);
  CreateObject(VENT, 1075, 595, -1)->SetCon(20);
  CreateObject(VENT, 1175, 595, -1)->SetCon(20);

  CreateObject(VENT, 1555, 595, -1)->SetCon(20);
  CreateObject(VENT, 1655, 595, -1)->SetCon(20);
  CreateObject(VENT, 1755, 595, -1)->SetCon(20);

  tmp = CreateObject(VEN3, 2200, 595, -1);
  tmp->SetCon(20);
  tmp->SetPhase(7);
  tmp = CreateObject(VEN3, 2220, 595, -1);
  tmp->SetCon(20);
  tmp->SetPhase(4);
  CreateObject(VEN3, 2240, 595, -1)->SetCon(20);

  //Selbstschussanlagen und Konsolen
  aSelfDefense[0] = CreateObject(SEGU, 590, 329, -1);
    aSelfDefense[0]->Arm(MISA);
    aSelfDefense[0]->SetAutoRepair(1500);
    CreateObject(CONS, 310, 525, -1)->Set(aSelfDefense[0]);

  aSelfDefense[1] = CreateObject(SEGU, 1252, 556, -1);
    aSelfDefense[1]->SetR(90);
    aSelfDefense[1]->Arm(MISA);
    aSelfDefense[1]->SetAutoRepair(1500);
    CreateObject(CONS, 1280, 515, -1)->Set(aSelfDefense[1]);

  aSelfDefense[2] = CreateObject(SEGU, 1478, 556, -1);
    aSelfDefense[2]->SetR(-90);
    aSelfDefense[2]->Arm(MISA);
    aSelfDefense[2]->SetAutoRepair(1500);
    CreateObject(CONS, 1450, 515, -1)->Set(aSelfDefense[2]);

  aSelfDefense[3] = CreateObject(SEGU, 2140, 329, -1);
    aSelfDefense[3]->Arm(MISA);
    aSelfDefense[3]->SetAutoRepair(1500);
    CreateObject(CONS, 2420, 525, -1)->Set(aSelfDefense[3]);

   //Grenzen
   CreateObject(BRDR, 170, 0, -1)->Set(0);
   CreateObject(BRDR, 2560, 0, -1)->Set(1);

  //Sounds

  //Wind
  CreateObject(SE4K, 580, 170, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 1365, 170, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 2150, 170, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 90, 410, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 540, 560, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 960, 670, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1365, 550, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1770, 670, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2190, 560, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2635, 410, -1)->Set("Interior*.ogg",665,105);

  //Rush Hour
  CreateObject(SE4K, 1075, 880, -1)->Set("Traffic*.ogg",245,70);
  CreateObject(SE4K, 1655, 880, -1)->Set("Traffic*.ogg",245,70);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Munitionskisten (Kugeln)
  var tmp = CreateObject (AMCT, 530, 730, -1);
  tmp->Set(ABOX);
  tmp->SetGraphics("Normal");
  var tmp = CreateObject (AMCT, 2200, 730, -1);
  tmp->Set(ABOX);
  tmp->SetGraphics("Normal");

  //Munitionskiste (Granaten)
  var tmp = CreateObject (AMCT, 1380, 411, -1);
  tmp->Set(GBOX);
  tmp->SetGraphics("Normal");

  //Raketen
  PlaceSpawnpoint(MIAP, 755, 715);
  PlaceSpawnpoint(MIAP, 1975, 715);

  //Feldgranaten
  PlaceSpawnpoint(FGRN, 755, 795);
  PlaceSpawnpoint(FGRN, 1975, 795);

  //Splittergranaten
  PlaceSpawnpoint(FRAG, 730, 435);
  PlaceSpawnpoint(FRAG, 2000, 435);

  //Blendgranaten
  PlaceSpawnpoint(STUN, 400, 510);
  PlaceSpawnpoint(STUN, 2330, 510);

  //Sturmgewehr
  PlaceSpawnpoint(ASTR, 640, 265);
  PlaceSpawnpoint(ASTR, 2090, 265);

  //Maschinengewehre
  PlaceSpawnpoint(MNGN, 875, 435);
  PlaceSpawnpoint(MNGN, 1855, 435);

  //Raketenwerfer
  PlaceSpawnpoint(RTLR, 790, 715);
  PlaceSpawnpoint(RTLR, 1940, 715);

  //Shotgun
  PlaceSpawnpoint(PPGN, 1330, 635);

  //Slingshot
  PlaceSpawnpoint(SGST, 1365, 408);

  //Maschinenpistolen
  PlaceSpawnpoint(SMGN, 380, 625);
  PlaceSpawnpoint(SMGN, 2350, 625);

  //Abwehrschild
  PlaceSpawnpoint(RSHL, 1075, 795);

  //Sprengfalle
  PlaceSpawnpoint(BBTP, 1655, 795);

  //EHP
  PlaceSpawnpoint(FAPK, 1365, 805);

  //Automat
  var store = CreateObject(WPVM,1365, 520,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);

  //Artilleriebatterien
  CreateObject(ATBY,1260,310,-1);
  CreateObject(ATBY,1470,310,-1);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[0])
   aSelfDefense[0]->SetTeam(iTeam);

  if(pPoint == aFlag[2])
   aSelfDefense[1]->SetTeam(iTeam);
   aSelfDefense[2]->SetTeam(iTeam);

  if(pPoint == aFlag[4])
   aSelfDefense[3]->SetTeam(iTeam);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[3]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[3]->TurnOn();

   if(aTeams[1] == true)
   {CreateFlag(1,530,612,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,2200,612,GetTeamColor(2));}
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[3]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[3]->TurnOn();

   //Flaggen
   aFlag[0] = CreateObject(OFPL,380,430,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(330,520);
   aFlag[0] -> AddSpawnPoint(540,260);
   aFlag[0] -> AddSpawnPoint(780,430);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,715,800,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(485,630);
   aFlag[1] -> AddSpawnPoint(630,630);
   aFlag[1] -> AddSpawnPoint(860,630);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,1365,413,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(1330,300);
   aFlag[2] -> AddSpawnPoint(1400,510);
   aFlag[2] -> AddSpawnPoint(1315,790);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,2015,800,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(1870,630);
   aFlag[3] -> AddSpawnPoint(2100,630);
   aFlag[3] -> AddSpawnPoint(2245,630);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,2350,430,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(2400,520);
   aFlag[4] -> AddSpawnPoint(1950,430);
   aFlag[4] -> AddSpawnPoint(2190,260);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag5$",0,2);
   }
  }

  //HTF-Spielziel
  if (FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 1365,413, -1);
   flag->~Set("$Flag3$");

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[3]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[3]->TurnOn();

   //Munitionskiste (Kugeln)
   var tmp = CreateObject(AMCT, 1330, 640, -1);
   tmp->Set(ABOX);

   //Raketen
   PlaceSpawnpoint(MIAP, 1330, 305);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //DM/LMS/HTF-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS) || FindObject(GHTF))
  {
   if(iTeam == 1)
   {
    var rand = Random(3);
    if(!rand)
     { iX = 260; iY = 430; }
    if(!--rand)
     { iX = 330; iY = 520; }
    if(!--rand)
     { iX = 620; iY = 790; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 2110; iY = 790; }
    if(!--rand)
     { iX = 2400; iY = 520; }
    if(!--rand)
     { iX = 2470; iY = 430; }
   }
   return(1);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   var rand = Random(2);
   if(iTeam == 1)
   {
    if(!rand)
      { iX = 260; iY = 430; }
    if(!--rand)
      { iX = 620; iY = 790; }
    return(1);
   }
   else
   {
    if(!rand)
      { iX = 2110; iY = 790; }
    if(!--rand)
      { iX = 2470; iY = 430; }
    return(1);
   }
  }

  //Startsicht
  iX = 1365; iY = 570;
}