/*-- Atlantic Crisis --*/

#strict
#include CSTD

static aFlag;


/* Initialisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Offensive.ogg");
  //Bildschirmf�rbung
  SetGamma(RGB(2,3,10), RGB(131,135,158), RGB(252,253,255) );
  //Hintergrundbewegung
  SetSkyParallax(0,15,15);
  //Flaggen
  aFlag = [];
  //Szenario einrichten
  CreateFurniture();
  //Ausr�stung plazieren
  CreateEquipment();
  return(1);
}

/* Plazierungslisten */

func CreateFurniture()
{
  Log("$CreatingFurniture$");

  //Rampen
  DrawMaterialQuad("Wall-PlateBlue",1391,510,1361,500,1361,510,1376,510,true);
  DrawMaterialQuad("Wall-Plate",1421,520,1391,510,1391,520,1406,520,true);
  DrawMaterialQuad("Wall-PlateBlue",1451,530,1421,520,1421,530,1436,530,true);
  DrawMaterialQuad("Wall-Plate",1661,540,1631,530,1631,540,1646,540,true);

  DrawMaterialQuad("Wall-Unknown1",1380,640,1410,630,1410,640,1395,640,true);
  DrawMaterialQuad("Wall-Unknown1",1410,630,1440,620,1440,630,1425,630,true);
  DrawMaterialQuad("Wall-Unknown1",1440,620,1470,610,1470,620,1455,620,true);

  DrawMaterialQuad("Wall-Stripes",1470,610,1500,600,1500,610,1485,610,true);

  DrawMaterialQuad("Wall-Unknown1",1691,630,1661,620,1661,630,1676,630,true);
  DrawMaterialQuad("Wall-Unknown1",1721,640,1691,630,1691,640,1706,640,true);

  DrawMaterialQuad("Wall-Plate",4350,540,4380,530,4380,540,4365,540,true);
  DrawMaterialQuad("Wall-PlateRed",4560,530,4590,520,4590,530,4575,530,true);
  DrawMaterialQuad("Wall-Plate",4590,520,4620,510,4620,520,4605,520,true);
  DrawMaterialQuad("Wall-PlateRed",4620,510,4650,500,4650,510,4635,510,true);

  DrawMaterialQuad("Wall-Unknown1",4290,640,4320,630,4320,640,4305,640,true);
  DrawMaterialQuad("Wall-Unknown1",4320,630,4350,620,4350,630,4335,630,true);

  DrawMaterialQuad("Wall-Stripes",4541,610,4511,600,4511,610,4526,610,true);

  DrawMaterialQuad("Wall-Unknown1",4631,640,4601,630,4601,640,4616,640,true);
  DrawMaterialQuad("Wall-Unknown1",4601,630,4571,620,4571,630,4586,630,true);
  DrawMaterialQuad("Wall-Unknown1",4571,620,4541,610,4541,620,4556,620,true);

  //Leitern
  CreateObject(LADR, 1275, 596, -1)->Set(11);
  CreateObject(LADR, 1550, 586, -1)->Set(6);
  CreateObject(LADR, 1575, 504, -1)->Set(12);

  CreateObject(LADR, 2200, 600, -1)->Set(9);
  CreateObject(LADR, 2400, 600, -1)->Set(9);

  CreateObject(LADR, 2765, 602, -1)->Set(8);
  CreateObject(LADR, 2870, 602, -1)->Set(8);
  CreateObject(LADR, 2910, 602, -1)->Set(8);

  CreateObject(LADR, 2990, 532, -1)->Set(8);

  CreateObject(LADR, 3070, 602, -1)->Set(8);
  CreateObject(LADR, 3110, 602, -1)->Set(8);
  CreateObject(LADR, 3215, 602, -1)->Set(8);

  CreateObject(LADR, 3610, 600, -1)->Set(9);
  CreateObject(LADR, 3810, 600, -1)->Set(9);

  CreateObject(LADR, 4435, 504, -1)->Set(12);
  CreateObject(LADR, 4460, 586, -1)->Set(6);
  CreateObject(LADR, 4738, 596, -1)->Set(11);

  //Bodenlucken
  CreateObject(HA4K, 1550, 533, -1);
  CreateObject(HA4K, 1550, 603, -1);

  CreateObject(HA4K, 2200, 523, -1);
  CreateObject(HA4K, 2400, 523, -1);

  CreateObject(HA4K, 2870, 533, -1);
  CreateObject(HA4K, 2910, 533, -1);

  CreateObject(HA4K, 2990, 463, -1);

  CreateObject(HA4K, 3070, 533, -1);
  CreateObject(HA4K, 3110, 533, -1);

  CreateObject(HA4K, 3610, 523, -1);
  CreateObject(HA4K, 3810, 523, -1);

  CreateObject(HA4K, 4460, 533, -1);
  CreateObject(HA4K, 4460, 603, -1);

  //Orientierungslichter
  CreateObject(OLGH, 1645, 400, -1)->Set(4, 15, 1, 1, 40);
  CreateObject(OLGH, 2410, 420, -1)->Set(35, 5, 1, 1, 20);
  CreateObject(OLGH, 2990, 380, -1)->Set(4, 10, 1, 1, 20);
  CreateObject(OLGH, 3600, 420, -1)->Set(35, 5, 1, 1, 20);
  CreateObject(OLGH, 4365, 400, -1)->Set(4, 15, 1, 1, 40);

  //Automat
  CreateObject(CLVM, 2840, 530, -1);

  //B�sche
  CreateObject(BSH2, 2770, 570, -1);
  CreateObject(BSH2, 3180, 425, -1);

  //Schutzt�ren
  CreateObject(GDDR, 2795, 530, -1);
  CreateObject(GDDR, 2875, 460, -1);
  CreateObject(GDDR, 3105, 460, -1);
  CreateObject(GDDR, 3185, 530, -1);

  //Bojen
  CreateObject(BUOY, 1180, 633, -1);
  CreateObject(BUOY, 2370, 633, -1);

  CreateObject(BUOY, 3840, 633, -1);
  CreateObject(BUOY, 4830, 633, -1);

  //Satellitensch�ssel
  CreateObject(SADH, 2840, 410, -1);

  //Radare
  CreateObject(RADR, 1610, 400, -1);
  CreateObject(RADR, 4400, 400, -1);

  //Kisten
  CreateObject(WCR2, 2120, 520, -1)->AutoRespawn();
  CreateObject(WCR2, 2910, 460, -1)->AutoRespawn();
  CreateObject(WCR2, 3070, 460, -1)->AutoRespawn();
  CreateObject(WCR2, 3890, 520, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 2940, 530, -1)->AutoRespawn();
  CreateObject(MWCR, 2960, 530, -1)->AutoRespawn();

  //Benzinf�sser
  CreateObject(PBRL, 1290, 500, -1)->AutoRespawn();
  CreateObject(PBRL, 1580, 530, -1)->AutoRespawn();
  CreateObject(PBRL, 1580, 600, -1)->AutoRespawn();

  CreateObject(PBRL, 2155, 520, -1)->AutoRespawn();
  CreateObject(PBRL, 2170, 520, -1)->AutoRespawn();

  CreateObject(PBRL, 2940, 460, -1)->AutoRespawn();
  CreateObject(PBRL, 3020, 380, -1)->AutoRespawn();

  CreateObject(PBRL, 3835, 520, -1)->AutoRespawn();
  CreateObject(PBRL, 3850, 520, -1)->AutoRespawn();

  CreateObject(PBRL, 4435, 530, -1)->AutoRespawn();
  CreateObject(PBRL, 4435, 600, -1)->AutoRespawn();
  CreateObject(PBRL, 4720, 500, -1)->AutoRespawn();

  //Phosphorf�sser
  CreateObject(HBRL, 2430, 520, -1)->AutoRespawn();
  CreateObject(HBRL, 3580, 520, -1)->AutoRespawn();

  //Giftf�sser
  CreateObject(TBRL, 2810, 530, -1)->AutoRespawn();
  CreateObject(TBRL, 2830, 530, -1)->AutoRespawn();

  //Explosivf�sser
  CreateObject(XBRL, 3110, 410, -1)->AutoRespawn();
  CreateObject(XBRL, 3140, 530, -1)->AutoRespawn();

  //Gasflasche
  CreateObject(GSBL, 3160, 530, -1)->AutoRespawn();

  //Explosivkiste
  CreateObject(XWCR, 2815, 410, -1)->AutoRespawn();

  //Stahlbr�cken
  CreateObject(_HBR, 2065, 532, -1);
  CreateObject(_HBR, 2335, 532, -1);
  CreateObject(_HBR, 2485, 532, -1);
  CreateObject(_HBR, 2735, 472, -1);
  CreateObject(_HBR, 3247, 472, -1);
  CreateObject(_HBR, 3525, 532, -1);
  CreateObject(_HBR, 3675, 532, -1);
  CreateObject(_HBR, 3945, 532, -1);

  //Haie
  CreateObject(SHRK, 500, 680, -1)->AutoRespawn();
  CreateObject(SHRK, 600, 680, -1)->AutoRespawn();
  CreateObject(SHRK, 700, 680, -1)->AutoRespawn();
  CreateObject(SHRK, 800, 680, -1)->AutoRespawn();

  CreateObject(SHRK, 2270, 680, -1)->AutoRespawn();
  CreateObject(SHRK, 2370, 680, -1)->AutoRespawn();
  CreateObject(SHRK, 2470, 680, -1)->AutoRespawn();
  CreateObject(SHRK, 2570, 680, -1)->AutoRespawn();

  CreateObject(SHRK, 3440, 680, -1)->AutoRespawn();
  CreateObject(SHRK, 3540, 680, -1)->AutoRespawn();
  CreateObject(SHRK, 3640, 680, -1)->AutoRespawn();
  CreateObject(SHRK, 3740, 680, -1)->AutoRespawn();

  CreateObject(SHRK, 5100, 680, -1)->AutoRespawn();
  CreateObject(SHRK, 5200, 680, -1)->AutoRespawn();
  CreateObject(SHRK, 5300, 680, -1)->AutoRespawn();
  CreateObject(SHRK, 5400, 680, -1)->AutoRespawn();

  //Jetwrack
  CreateObject(_JW1, 1110, 700, -1);
  CreateObject(_JW2, 1275, 740, -1);

  //Kaputte U-Boote
  CreateObject(SUBB, 3805, 780, -1);
  CreateObject(SUBB, 7200, 800, -1);

  //Kaputte Boote
  CreateObject(SLBB, 110, 740, -1);
  CreateObject(SLBB, 5385, 760, -1);

  //Ventillatoren
  CreateObject(VENT, 1380, 590, -1)->SetCon(30);
  CreateObject(VEN3, 2410, 500, -1)->SetCon(30);
  CreateObject(VEN3, 3600, 500, -1)->SetCon(30);
  CreateObject(VENT, 4630, 590, -1)->SetCon(30);

  //Dekoschleusen
  CreateObject(GAT1, 2800, 590, -1);
  CreateObject(GAT1, 2990, 590, -1);
  CreateObject(GAT1, 3180, 590, -1);

  //Panele
  CreateObject(CPP1, 1520, 600, -1);
  CreateObject(CPP1, 4490, 600, -1);

  //Sandsackbarrieren
  CreateObject(SBBA, 1705, 540, -1)->Right();
  CreateObject(SBBA, 2115, 520, -1);
  CreateObject(SBBA, 2285, 520, -1)->Right();
  CreateObject(SBBA, 3725, 520, -1);
  CreateObject(SBBA, 3895, 520, -1)->Right();
  CreateObject(SBBA, 4305, 540, -1);

  //Gel�nder
  CreateObject(RAI1, 1280, 500, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1450, 530, -1)->SetRail([1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1500, 600, -1)->SetRail([1,1,1,1,1]);

  CreateObject(RAI1, 2100, 520, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1]);

  CreateObject(RAI1, 2880, 460, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1]);

  CreateObject(RAI1, 3710, 520, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1]);

  CreateObject(RAI1, 4380, 530, -1)->SetRail([1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 4410, 600, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 4650, 500, -1)->SetRail([1,1,1,1]);

  //Explosivtank
  CreateObject(XTNK, 3140, 410, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 1600, 530, -1)->AutoRespawn();
  CreateObject(PTNK, 2410, 520, -1)->AutoRespawn();
  CreateObject(PTNK, 3600, 520, -1)->AutoRespawn();
  CreateObject(PTNK, 4410, 530, -1)->AutoRespawn();

  //Schilder
  CreateObject(ESGN, 1515, 590, -1);
  CreateObject(ESGN, 2970, 440, -1);
  CreateObject(ESGN, 4495, 590, -1);

  //L�ftungsgitter
  CreateObject(ENGT, 2890, 530, -1);
  CreateObject(ENGT, 3090, 530, -1);

  //Bildschirme
  CreateObject(SCR1, 1665, 595, -1);
  CreateObject(SCA1, 2825, 490, -1);
  CreateObject(SCA1, 3155, 490, -1);
  CreateObject(SCR1, 4330, 595, -1);

  //Grenzen
  CreateObject(BRDR, 1170, 0, -1)->Set(0);
  CreateObject(BRDR, 4835, 0, -1)->Set(1);

  //Sounds

  //M�ven
  CreateObject(SE4K, 1080, 550, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 2200, 380, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 3810, 380, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 4930, 550, -1)->Set("SeaSounds*.ogg",140,35);

  //Wellen
  CreateObject(SE4K, 500, 650, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1000, 650, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1900, 650, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 2525, 650, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 3460, 650, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 4080, 650, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 5040, 650, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 5540, 650, -1)->Set("Wave*.ogg",105,35);

  //Wind
  CreateObject(SE4K, 1540, 350, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 2990, 290, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 4460, 350, -1)->Set("WindSound*.ogg",245,105);

  //Lautsprecher
  CreateObject(SE4K, 2990, 330, -1)->Set("Announce*.ogg",1600,130);
}

func CreateEquipment()
{
  var tmp;
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  tmp = CreateObject(AMCT, 1680, 400, -1);
  tmp->Set(ABOX);
  tmp = CreateObject(AMCT, 4330, 400, -1);
  tmp->Set(ABOX);

  //Versorgungskiste (Gewehrgranaten)
  var tmp = CreateObject (AMCT, 3020, 460, -1);
  tmp->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MBOX, 2150, 515);
  PlaceSpawnpoint(MBOX, 3860, 515);

  //Artilleriebatterien
  CreateObject(ATBY,2231,520,-1);
  CreateObject(ATBY,3775,520,-1);

  //Patrouillenboote
  SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1710,620,-1),50*21,300);
  SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2990,620,-1),50*21,300);
  SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4310,620,-1),50*21,300);

  //Automaten
  var store = CreateObject(WPVM,1500, 530,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
  var store = CreateObject(WPVM,3025, 530,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
  var store = CreateObject(WPVM,4500, 530,-1);
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

/* Regelw�hler */

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
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,1520,600,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,4490,600,GetTeamColor(2));}
  }

  //HTF-Spielziel
  if (FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 2990, 380, -1);
   flag->~Set("$Flag2$");

   //Blackhawks
   SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,1655,370,-1),100*21,300);
   SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,4355,370,-1),100*21,300);

   //Zus�tzliche Munition
   if(!FindObject(NOAM))
   {
    //Versorgungskiste (Kugeln)
    var tmp = CreateObject(AMCT, 2965, 460, -1);
    tmp->Set(ABOX);

    //Raketen
    PlaceSpawnpoint(MBOX, 2890, 525);
   }
  }

  //Base Assault-Spielziel
  if(FindObject(GBAS))
  {
   //Zielobjekte
   AddAssaultTarget(CMSN, 1610, 600, 500, 1, "$Flag1$", 0, [[1680, 540], [1340, 500]]);

   AddAssaultTarget(CMSN, 4400, 600, 500, 2, "$Flag3$", 1, [[4330, 540], [4690, 500]]);

   //Blackhawks
   SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,1655,370,-1),100*21,300);
   SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,4355,370,-1),100*21,300);
  }

  //Money Run
  if (FindObject(GMNR))
  {
    AddMoneySpawn(2335, 510, [5, 10]);
    AddMoneySpawn(2785, 450, [10, 15, 20]);
    AddMoneySpawn(3195, 450, [10, 15, 20]);
    AddMoneySpawn(3675, 510, [5, 10]);
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,1320,500,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(1500, 590);
   aFlag[0] -> AddSpawnPoint(1610, 390);
   aFlag[0] -> AddSpawnPoint(1680, 530);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,2990,380,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(2850,520);
   aFlag[1] -> AddSpawnPoint(2965,450);
   aFlag[1] -> AddSpawnPoint(3090,520);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,4685,500,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(4330, 530);
   aFlag[2] -> AddSpawnPoint(4400, 390);
   aFlag[2] -> AddSpawnPoint(4510, 590);
   if(aTeams[2] == true)
   {
    aFlag[2]->Set("$Flag3$",100,2);
    aFlag[2]->Capture(2,1);
   }
   else
   {
    aFlag[2]->Set("$Flag3$",0,2);
   }

   //Blackhawks
   SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,1655,370,-1),100*21,300);
   SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,4355,370,-1),100*21,300);
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
     { iX = 1350; iY = 490; }
    if(!--rand)
     { iX = 1670; iY = 530; }
    return(1);
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 4660; iY = 490; }
    if(!--rand)
     { iX = 4340; iY = 530; }
    return(1);
   }
  }

  //Startsicht
  iX = 2990; iY = 440;
}