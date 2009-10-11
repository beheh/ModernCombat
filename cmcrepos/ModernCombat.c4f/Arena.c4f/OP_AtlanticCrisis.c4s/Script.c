/*-- Atlantic Crisis --*/

#strict
#include CSTD

static aFlag;


func ChooserRuleConfig()
{
  return [NOFF,WPST,NODR];
}

/* Initalisierung */

func Initialize()
{
  //Bildschirmfärbung
  SetGamma(RGB(2,3,10), RGB(131,135,158), RGB(252,253,255) );
  //Hintergrundbewegung
  SetSkyParallax(0,15,15);
  //Musiktitel, welches für dieses Szenario bestimmt ist, zuerst abspielen
  Music("Missile Alert");
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
  DrawMaterialQuad("Wall-Stripes",1631,510,1601,500,1601,510,1616,510,true);

  DrawMaterialQuad("Wall-Unknown1",1661,520,1631,510,1631,520,1646,520,true);
  DrawMaterialQuad("Wall-Unknown1",1691,530,1661,520,1661,530,1676,530,true);
  DrawMaterialQuad("Wall-Unknown1",1721,540,1691,530,1691,540,1706,540,true);


  DrawMaterialQuad("Wall-Plate",4350,440,4380,430,4380,440,4365,440,true);
  DrawMaterialQuad("Wall-PlateRed",4560,430,4590,420,4590,430,4575,430,true);
  DrawMaterialQuad("Wall-Plate",4590,420,4620,410,4620,420,4605,420,true);
  DrawMaterialQuad("Wall-PlateRed",4620,410,4650,400,4650,410,4635,410,true);

  DrawMaterialQuad("Wall-Unknown1",4350,520,4380,510,4380,520,4365,520,true);
  DrawMaterialQuad("Wall-Unknown1",4290,540,4320,530,4320,540,4305,540,true);
  DrawMaterialQuad("Wall-Unknown1",4320,530,4350,520,4350,530,4335,530,true);

  DrawMaterialQuad("Wall-Stripes",4380,510,4410,500,4410,510,4395,510,true);
  DrawMaterialQuad("Wall-Stripes",4541,510,4511,500,4511,510,4526,510,true);

  DrawMaterialQuad("Wall-Unknown1",4631,540,4601,530,4601,540,4616,540,true);
  DrawMaterialQuad("Wall-Unknown1",4601,530,4571,520,4571,530,4586,530,true);
  DrawMaterialQuad("Wall-Unknown1",4571,520,4541,510,4541,520,4556,520,true);

  //Leitern
  CreateObject(LADR, 1275, 480, -1)->Set(9);
  CreateObject(LADR, 1550, 480, -1)->Set(5);
  CreateObject(LADR, 1575, 400, -1)->Set(12);

  CreateObject(LADR, 2200, 470, -1)->Set(5);

  CreateObject(LADR, 2870, 485, -1)->Set(6);
  CreateObject(LADR, 2910, 485, -1)->Set(6);

  CreateObject(LADR, 2990, 430, -1)->Set(8);

  CreateObject(LADR, 3070, 485, -1)->Set(6);
  CreateObject(LADR, 3110, 485, -1)->Set(6);

  CreateObject(LADR, 3810, 470, -1)->Set(5);

  CreateObject(LADR, 4435, 400, -1)->Set(12);
  CreateObject(LADR, 4460, 480, -1)->Set(5);
  CreateObject(LADR, 4738, 480, -1)->Set(9);

  //Bodenlucken
  CreateObject(HA4K, 1550, 433, -1);
  CreateObject(HA4K, 1550, 503, -1);

  CreateObject(HA4K, 2200, 423, -1);

  CreateObject(HA4K, 2870, 433, -1);
  CreateObject(HA4K, 2910, 433, -1);

  CreateObject(HA4K, 2990, 363, -1);

  CreateObject(HA4K, 3070, 433, -1);
  CreateObject(HA4K, 3110, 433, -1);

  CreateObject(HA4K, 3810, 423, -1);

  CreateObject(HA4K, 4460, 433, -1);
  CreateObject(HA4K, 4460, 503, -1);

  //Orientierungslichter
  CreateObject(OLGH, 1645, 300, -1)->Set(4, 15, 1, 1, 40);
  CreateObject(OLGH, 2990, 280, -1)->Set(4, 10, 1, 1, 20);
  CreateObject(OLGH, 4365, 300, -1)->Set(4, 15, 1, 1, 40);

  //Automat
  CreateObject(CLVM, 2955, 430, -1);

  //Büsche
  CreateObject(BSH2, 2770, 470, -1);
  CreateObject(BSH2, 3180, 325, -1);

  //Stahlbrücken
  CreateObject(_HBR, 2735, 372, -1);
  CreateObject(_HBR, 3245, 372, -1);

  //Bojen
  CreateObject(BUOY, 1180, 510, -1);
  CreateObject(BUOY, 2370, 510, -1);

  CreateObject(BUOY, 3840, 510, -1);
  CreateObject(BUOY, 4830, 510, -1);

  //Satellitenschüssel
  CreateObject(SADH, 2840, 310, -1);

  //Kisten
  CreateObject(WCR2, 2120, 420, -1)->AutoRespawn();
  CreateObject(WCR2, 2910, 360, -1)->AutoRespawn();
  CreateObject(WCR2, 3070, 360, -1)->AutoRespawn();
  CreateObject(WCR2, 3890, 420, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 1290, 400, -1)->AutoRespawn();
  CreateObject(PBRL, 1580, 430, -1)->AutoRespawn();
  CreateObject(PBRL, 1580, 500, -1)->AutoRespawn();

  CreateObject(PBRL, 2220, 420, -1)->AutoRespawn();
  CreateObject(PBRL, 2240, 420, -1)->AutoRespawn();

  CreateObject(PBRL, 2940, 360, -1)->AutoRespawn();
  CreateObject(PBRL, 3020, 280, -1)->AutoRespawn();

  CreateObject(PBRL, 3790, 420, -1)->AutoRespawn();
  CreateObject(PBRL, 3770, 420, -1)->AutoRespawn();

  CreateObject(PBRL, 4435, 430, -1)->AutoRespawn();
  CreateObject(PBRL, 4435, 500, -1)->AutoRespawn();
  CreateObject(PBRL, 4720, 400, -1)->AutoRespawn();

  //Giftfässer
  CreateObject(TBRL, 2810, 430, -1)->AutoRespawn();
  CreateObject(TBRL, 2830, 430, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 3110, 310, -1)->AutoRespawn();
  CreateObject(XBRL, 3140, 430, -1)->AutoRespawn();
  CreateObject(XBRL, 3160, 430, -1)->AutoRespawn();

  //Explosivkiste
  CreateObject(XWCR, 2815, 310, -1)->AutoRespawn();

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
  CreateObject(VENT, 4630, 490, -1)->SetCon(30);

  //Satellitenschüsseln
  CreateObject(RADR, 1610, 300, -1);
  CreateObject(RADR, 4400, 300, -1);

  //Einfache Türen
  CreateObject(LBDR, 2875, 360, -1);
  CreateObject(LBDR, 3095, 360, -1);

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

  //Stationärer Gastank
  CreateObject(GSTA, 3140, 310, -1)->AutoRespawn();

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

  //Tisch
  CreateObject(GTBL, 2950, 430, -1);

  //Flaggen
  aFlag[0] = CreateObject(OFPL,1320,400,NO_OWNER);
    aFlag[0]->Set("$Flag1$",100);
    aFlag[0]->Capture(1);

  aFlag[1] = CreateObject(OFPL,2990,280,NO_OWNER);
    aFlag[1]->Set("$Flag2$",100);

  aFlag[2] = CreateObject(OFPL,4685,400,NO_OWNER);
    aFlag[2]->Set("$Flag3$",100);
    aFlag[2]->Capture(2);

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
  tmp = CreateObject(ABOX, 1680, 300, -1);
  tmp->Set(AMOC);
  tmp = CreateObject(ABOX, 4330, 300, -1);
  tmp->Set(AMOC);

  //Projektilmunition
  PlaceSpawnpoint(AMOC, 2145, 420);
  PlaceSpawnpoint(AMOC, 3870, 420);

  //Raketen
  PlaceSpawnpoint(MIAP, 1550, 540);
  PlaceSpawnpoint(MIAP, 4460, 540);

  //Feldgranaten
  PlaceSpawnpoint(FGRN, 1415, 408);
  PlaceSpawnpoint(FGRN, 2990, 315);
  PlaceSpawnpoint(FGRN, 4600, 408);

  //Motorboote
  SetupVehicleSpawn([INFL],CreateObject(VSPW,1710,520,-1),10*10,1);
  SetupVehicleSpawn([INFL],CreateObject(VSPW,4310,520,-1),10*10,2);

  //Waffenautomaten
  SetUpStore(CreateObject(WPVM, 1500, 430, -1));
  SetUpStore(CreateObject(WPVM, 3025, 430, -1));
  SetUpStore(CreateObject(WPVM, 4500, 430, -1));
}

/* Kaufliste */

public func SetUpStore(pStore)
{
  pStore->AddWares("IsWeapon", -1);
  pStore->AddWares("IsAmmoPacket", -1);
  pStore->AddWares("IsUpgrade", -1);
  pStore->AddWares("IsEquipment", -1);
  pStore->SortWare("IsWeapon","IsAmmoPacket","IsUpgrade","IsEquipment");
}