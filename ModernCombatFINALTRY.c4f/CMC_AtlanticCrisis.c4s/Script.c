/*-- Atlantic Crisis --*/

#strict
#include CSTD

static aFlag;


/* Initalisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_City Lights.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg");
  Music("CMC_Offensive.ogg");
  //Bildschirmfärbung
  SetGamma(RGB(2,3,10), RGB(131,135,158), RGB(252,253,255) );
  //Hintergrundbewegung
  SetSkyParallax(0,15,15);
  //Flaggen
  aFlag = [];
  //Szenario einrichten
  CreateFurniture();
  //Ausrüstung plazieren
  CreateEquipment();
  return(1);
}

/* Plazierungslisten */

func CreateFurniture()
{
  Log("$CreatingFurniture$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Rampen
  DrawMaterialQuad("Wall-PlateBlue",1391,410,1361,400,1361,410,1376,410,true);
  DrawMaterialQuad("Wall-Plate",1421,420,1391,410,1391,420,1406,420,true);
  DrawMaterialQuad("Wall-PlateBlue",1451,430,1421,420,1421,430,1436,430,true);
  DrawMaterialQuad("Wall-Plate",1661,440,1631,430,1631,440,1646,440,true);

  DrawMaterialQuad("Wall-Unknown1",1380,540,1410,530,1410,540,1395,540,true);
  DrawMaterialQuad("Wall-Unknown1",1410,530,1440,520,1440,530,1425,530,true);
  DrawMaterialQuad("Wall-Unknown1",1440,520,1470,510,1470,520,1455,520,true);

  DrawMaterialQuad("Wall-Stripes",1470,510,1500,500,1500,510,1485,510,true);

  DrawMaterialQuad("Wall-Unknown1",1691,530,1661,520,1661,530,1676,530,true);
  DrawMaterialQuad("Wall-Unknown1",1721,540,1691,530,1691,540,1706,540,true);


  DrawMaterialQuad("Wall-Plate",4350,440,4380,430,4380,440,4365,440,true);
  DrawMaterialQuad("Wall-PlateRed",4560,430,4590,420,4590,430,4575,430,true);
  DrawMaterialQuad("Wall-Plate",4590,420,4620,410,4620,420,4605,420,true);
  DrawMaterialQuad("Wall-PlateRed",4620,410,4650,400,4650,410,4635,410,true);

  DrawMaterialQuad("Wall-Unknown1",4290,540,4320,530,4320,540,4305,540,true);
  DrawMaterialQuad("Wall-Unknown1",4320,530,4350,520,4350,530,4335,530,true);

  DrawMaterialQuad("Wall-Stripes",4541,510,4511,500,4511,510,4526,510,true);

  DrawMaterialQuad("Wall-Unknown1",4631,540,4601,530,4601,540,4616,540,true);
  DrawMaterialQuad("Wall-Unknown1",4601,530,4571,520,4571,530,4586,530,true);
  DrawMaterialQuad("Wall-Unknown1",4571,520,4541,510,4541,520,4556,520,true);

  //Leitern
  CreateObject(LADR, 1275, 496, -1)->Set(11);
  CreateObject(LADR, 1550, 486, -1)->Set(6);
  CreateObject(LADR, 1575, 404, -1)->Set(12);

  CreateObject(LADR, 2200, 500, -1)->Set(9);
  CreateObject(LADR, 2400, 500, -1)->Set(9);

  CreateObject(LADR, 2765, 502, -1)->Set(8);
  CreateObject(LADR, 2870, 502, -1)->Set(8);
  CreateObject(LADR, 2910, 502, -1)->Set(8);

  CreateObject(LADR, 2990, 432, -1)->Set(8);

  CreateObject(LADR, 3070, 502, -1)->Set(8);
  CreateObject(LADR, 3110, 502, -1)->Set(8);
  CreateObject(LADR, 3215, 502, -1)->Set(8);

  CreateObject(LADR, 3610, 500, -1)->Set(9);
  CreateObject(LADR, 3810, 500, -1)->Set(9);

  CreateObject(LADR, 4435, 404, -1)->Set(12);
  CreateObject(LADR, 4460, 486, -1)->Set(6);
  CreateObject(LADR, 4738, 496, -1)->Set(11);

  //Bodenlucken
  CreateObject(HA4K, 1550, 433, -1);
  CreateObject(HA4K, 1550, 503, -1);

  CreateObject(HA4K, 2200, 423, -1);
  CreateObject(HA4K, 2400, 423, -1);

  CreateObject(HA4K, 2870, 433, -1);
  CreateObject(HA4K, 2910, 433, -1);

  CreateObject(HA4K, 2990, 363, -1);

  CreateObject(HA4K, 3070, 433, -1);
  CreateObject(HA4K, 3110, 433, -1);

  CreateObject(HA4K, 3610, 423, -1);
  CreateObject(HA4K, 3810, 423, -1);

  CreateObject(HA4K, 4460, 433, -1);
  CreateObject(HA4K, 4460, 503, -1);

  //Orientierungslichter
  CreateObject(OLGH, 1645, 300, -1)->Set(4, 15, 1, 1, 40);
  CreateObject(OLGH, 2410, 320, -1)->Set(35, 5, 1, 1, 20);
  CreateObject(OLGH, 2990, 280, -1)->Set(4, 10, 1, 1, 20);
  CreateObject(OLGH, 3600, 320, -1)->Set(35, 5, 1, 1, 20);
  CreateObject(OLGH, 4365, 300, -1)->Set(4, 15, 1, 1, 40);

  //Automat
  CreateObject(CLVM, 2840, 430, -1);

  //Büsche
  CreateObject(BSH2, 2770, 470, -1);
  CreateObject(BSH2, 3180, 325, -1);

  //Stahltüren
  CreateObject(STDR, 2795, 430, -1);
  CreateObject(STDR, 2875, 360, -1);
  CreateObject(STDR, 3105, 360, -1);
  CreateObject(STDR, 3185, 430, -1);

  //Bojen
  CreateObject(BUOY, 1180, 533, -1);
  CreateObject(BUOY, 2370, 533, -1);

  CreateObject(BUOY, 3840, 533, -1);
  CreateObject(BUOY, 4830, 533, -1);

  //Satellitenschüssel
  CreateObject(SADH, 2840, 310, -1);

  //Kisten
  CreateObject(WCR2, 2120, 420, -1)->AutoRespawn();
  CreateObject(WCR2, 2910, 360, -1)->AutoRespawn();
  CreateObject(WCR2, 3070, 360, -1)->AutoRespawn();
  CreateObject(WCR2, 3890, 420, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 2940, 430, -1)->AutoRespawn();
  CreateObject(MWCR, 2960, 430, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 1290, 400, -1)->AutoRespawn();
  CreateObject(PBRL, 1580, 430, -1)->AutoRespawn();
  CreateObject(PBRL, 1580, 500, -1)->AutoRespawn();

  CreateObject(PBRL, 2155, 420, -1)->AutoRespawn();
  CreateObject(PBRL, 2170, 420, -1)->AutoRespawn();

  CreateObject(PBRL, 2940, 360, -1)->AutoRespawn();
  CreateObject(PBRL, 3020, 280, -1)->AutoRespawn();

  CreateObject(PBRL, 3835, 420, -1)->AutoRespawn();
  CreateObject(PBRL, 3850, 420, -1)->AutoRespawn();

  CreateObject(PBRL, 4435, 430, -1)->AutoRespawn();
  CreateObject(PBRL, 4435, 500, -1)->AutoRespawn();
  CreateObject(PBRL, 4720, 400, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 2430, 420, -1)->AutoRespawn();
  CreateObject(HBRL, 3580, 420, -1)->AutoRespawn();

  //Giftfässer
  CreateObject(TBRL, 2810, 430, -1)->AutoRespawn();
  CreateObject(TBRL, 2830, 430, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 3110, 310, -1)->AutoRespawn();
  CreateObject(XBRL, 3140, 430, -1)->AutoRespawn();

  //Gasflasche
  CreateObject(GSBL, 3160, 430, -1)->AutoRespawn();

  //Explosivkiste
  CreateObject(XWCR, 2815, 310, -1)->AutoRespawn();

  //Stahlbrücken
  CreateObject(_HBR, 2065, 432, -1);
  CreateObject(_HBR, 2335, 432, -1);
  CreateObject(_HBR, 2485, 432, -1);
  CreateObject(_HBR, 2735, 372, -1);
  CreateObject(_HBR, 3247, 372, -1);
  CreateObject(_HBR, 3525, 432, -1);
  CreateObject(_HBR, 3675, 432, -1);
  CreateObject(_HBR, 3945, 432, -1);

  //Haie
  CreateObject(SHRK, 500, 580, -1)->AutoRespawn();
  CreateObject(SHRK, 600, 580, -1)->AutoRespawn();
  CreateObject(SHRK, 700, 580, -1)->AutoRespawn();
  CreateObject(SHRK, 800, 580, -1)->AutoRespawn();

  CreateObject(SHRK, 2270, 580, -1)->AutoRespawn();
  CreateObject(SHRK, 2370, 580, -1)->AutoRespawn();
  CreateObject(SHRK, 2470, 580, -1)->AutoRespawn();
  CreateObject(SHRK, 2570, 580, -1)->AutoRespawn();

  CreateObject(SHRK, 3440, 580, -1)->AutoRespawn();
  CreateObject(SHRK, 3540, 580, -1)->AutoRespawn();
  CreateObject(SHRK, 3640, 580, -1)->AutoRespawn();
  CreateObject(SHRK, 3740, 580, -1)->AutoRespawn();

  CreateObject(SHRK, 5100, 580, -1)->AutoRespawn();
  CreateObject(SHRK, 5200, 580, -1)->AutoRespawn();
  CreateObject(SHRK, 5300, 580, -1)->AutoRespawn();
  CreateObject(SHRK, 5400, 580, -1)->AutoRespawn();

  //Jetwrack
  CreateObject(_JW1, 1110, 600, -1);
  CreateObject(_JW2, 1275, 640, -1);

  //Kaputte U-Boote
  CreateObject(SUBB, 3805, 680, -1);
  CreateObject(SUBB, 7200, 700, -1);

  //Kaputte Boote
  CreateObject(SLBB, 110, 640, -1);
  CreateObject(SLBB, 5385, 660, -1);

  //Ventillatoren
  CreateObject(VENT, 1380, 490, -1)->SetCon(30);
  CreateObject(VEN3, 2410, 400, -1)->SetCon(30);
  CreateObject(VEN3, 3600, 400, -1)->SetCon(30);
  CreateObject(VENT, 4630, 490, -1)->SetCon(30);

  //Satellitenschüsseln
  CreateObject(RADR, 1610, 300, -1);
  CreateObject(RADR, 4400, 300, -1);

  //Dekoschleusen
  CreateObject(GAT1, 2800, 490, -1);
  CreateObject(GAT1, 2990, 490, -1);
  CreateObject(GAT1, 3180, 490, -1);

  //Panele
  CreateObject(CPP1, 1520, 500, -1);
  CreateObject(CPP1, 4490, 500, -1);

  //Sandsackbarrieren
  CreateObject(SBBA, 1705, 440, -1)->Right();
  CreateObject(SBBA, 2115, 420, -1);
  CreateObject(SBBA, 2285, 420, -1)->Right();
  CreateObject(SBBA, 3725, 420, -1);
  CreateObject(SBBA, 3895, 420, -1)->Right();
  CreateObject(SBBA, 4305, 440, -1);

  //Geländer
  CreateObject(RAI1, 1280, 400, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1450, 430, -1)->SetRail([1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1500, 500, -1)->SetRail([1,1,1,1,1]);

  CreateObject(RAI1, 2100, 420, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1]);

  CreateObject(RAI1, 2880, 360, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1]);

  CreateObject(RAI1, 3710, 420, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1]);

  CreateObject(RAI1, 4380, 430, -1)->SetRail([1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 4410, 500, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 4650, 400, -1)->SetRail([1,1,1,1]);

  //Explosivtank
  CreateObject(XTNK, 3140, 310, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 1600, 430, -1)->AutoRespawn();
  CreateObject(PTNK, 2410, 420, -1)->AutoRespawn();
  CreateObject(PTNK, 3600, 420, -1)->AutoRespawn();
  CreateObject(PTNK, 4410, 430, -1)->AutoRespawn();

  //Schilder
  CreateObject(ESGN, 1515, 490, -1);
  CreateObject(ESGN, 2970, 340, -1);
  CreateObject(ESGN, 4495, 490, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 2890, 430, -1);
  CreateObject(ENGT, 3090, 430, -1);

  //Bildschirme
  CreateObject(SCR1, 1665, 495, -1);
  CreateObject(SCA1, 2825, 390, -1);
  CreateObject(SCA1, 3155, 390, -1);
  CreateObject(SCR1, 4330, 495, -1);

  //Grenzen
  CreateObject(BRDR, 1170, 0, -1)->Set(0);
  CreateObject(BRDR, 4835, 0, -1)->Set(1);

  //Sounds

  //Möven
  CreateObject(SE4K, 1080, 450, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 2200, 280, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 3810, 280, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 4930, 450, -1)->Set("SeaSounds*.ogg",140,35);

  //Wellen
  CreateObject(SE4K, 500, 550, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1000, 550, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1900, 550, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 2525, 550, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 3460, 550, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 4080, 550, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 5040, 550, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 5540, 550, -1)->Set("Wave*.ogg",105,35);

  //Wind
  CreateObject(SE4K, 1540, 250, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 2990, 190, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 4460, 250, -1)->Set("WindSound*.ogg",245,105);

  //Lautsprecher
  CreateObject(SE4K, 2990, 230, -1)->Set("Announce*.ogg",1600,130);
}

func CreateEquipment()
{
  var tmp;
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Munitionskisten (Kugeln)
  tmp = CreateObject(AMCT, 1680, 300, -1);
  tmp->Set(ABOX);
  tmp = CreateObject(AMCT, 4330, 300, -1);
  tmp->Set(ABOX);

  //Munitionskiste (Granaten)
  var tmp = CreateObject (AMCT, 3020, 360, -1);
  tmp->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MIAP, 2150, 415);
  PlaceSpawnpoint(MIAP, 3860, 415);

  //Sturmgewehr
  PlaceSpawnpoint(ASTR, 2890, 425);

  //Maschinengewehr
  PlaceSpawnpoint(MNGN, 2990, 510);

  //Raketenwerfer
  PlaceSpawnpoint(RTLR, 2180, 415);
  PlaceSpawnpoint(RTLR, 3830, 415);

  //Shotguns
  PlaceSpawnpoint(PPGN, 1550, 530);
  PlaceSpawnpoint(PPGN, 4460, 530);

  //Slingshot
  PlaceSpawnpoint(SGST, 3040, 355);

  //Maschinenpistolen
  PlaceSpawnpoint(SMGN, 1520, 495);
  PlaceSpawnpoint(SMGN, 4490, 495);

  //Dragnin
  PlaceSpawnpoint(DGNN, 1700, 295);
  PlaceSpawnpoint(DGNN, 4310, 295);

  //Artilleriebatterien
  CreateObject(ATBY,2231,420,-1);
  CreateObject(ATBY,3775,420,-1);

  //Motorboote
  SetupVehicleSpawn([INFL],DIR_Right,CreateObject(VSPW,1710,520,-1),10*10);
  SetupVehicleSpawn([INFL],DIR_Left,CreateObject(VSPW,2990,520,-1),10*10);
  SetupVehicleSpawn([INFL],DIR_Left,CreateObject(VSPW,4310,520,-1),10*10);

  //Automaten
  var store = CreateObject(WPVM,1500, 430,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
  var store = CreateObject(WPVM,3025, 430,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
  var store = CreateObject(WPVM,4500, 430,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
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
   if(aTeams[1] == true)
   {CreateFlag(1,1520,500,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,4490,500,GetTeamColor(2));}
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggen
   aFlag[0] = CreateObject(OFPL,1320,400,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(1500, 490);
   aFlag[0] -> AddSpawnPoint(1610, 290);
   aFlag[0] -> AddSpawnPoint(1680, 430);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,2990,280,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(2850,420);
   aFlag[1] -> AddSpawnPoint(2965,350);
   aFlag[1] -> AddSpawnPoint(3090,420);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,4685,400,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(4330, 430);
   aFlag[2] -> AddSpawnPoint(4400, 290);
   aFlag[2] -> AddSpawnPoint(4510, 490);
   if(aTeams[2] == true)
   {
    aFlag[2]->Set("$Flag3$",100,2);
    aFlag[2]->Capture(2,1);
   }
   else
   {
    aFlag[2]->Set("$Flag3$",0,2);
   }
  }

  //HTF-Spielziel
  if (FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 2990, 280, -1);
   flag->~Set("$Flag2$");
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //DM/LMS/CTF/HTF-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS) || FindObject(GCTF) || FindObject(GHTF))
  {
   if(iTeam == 1)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 1350; iY = 390; }
    if(!--rand)
     { iX = 1670; iY = 430; }
    return(1);
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 4660; iY = 390; }
    if(!--rand)
     { iX = 4340; iY = 430; }
    return(1);
   }
  }

  //Startsicht
  iX = 2990; iY = 340;
}