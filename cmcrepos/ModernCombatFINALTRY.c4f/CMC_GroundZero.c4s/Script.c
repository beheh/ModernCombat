/*-- Ground Zero --*/

#strict
#include CSTD

static aFlag, aSelfDefense;


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetPlayList("CMC_WaitingforSpawn.ogg");
  Music("CMC_WaitingforSpawn.ogg",1);
  //Himmel
  SetSkyParallax(0,45,24,0,0,0,0);
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausr�stung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  //Optionale Objekte plazieren
  CreateOptionalFeatures();
  return(1); 
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Aufz�ge
  CreateObject(LFTP, 165, 500, -1);
  CreateObject(LFTP, 615, 380, -1);
  CreateObject(LFTP, 1025, 390, -1);
  var liftp = CreateObject (LFTP, 1105, 185, -1);
  liftp->DisableVertical();
  CreateObject(LFTP, 1775, 400, -1);
  CreateObject(LFTP, 2185, 405, -1);
  CreateObject(LFTP, 2635, 435, -1);

  //Leitern
  CreateObject(LADR, 40, 534, -1)->Set(17);
  CreateObject(LADR, 535, 320, -1)->Set(7);
  CreateObject(LADR, 695, 320, -1)->Set(7);
  CreateObject(LADR, 835, 580, -1)->Set(16);

  CreateObject(LADR, 1025, 313, -1)->Set(17);
  CreateObject(LADR, 1205, 520, -1)->Set(9);
  CreateObject(LADR, 1595, 520, -1)->Set(9);
  CreateObject(LADR, 1775, 310, -1)->Set(17);

  CreateObject(LADR, 1965, 580, -1)->Set(16);
  CreateObject(LADR, 2105, 320, -1)->Set(7);
  CreateObject(LADR, 2265, 320, -1)->Set(7);
  CreateObject(LADR, 2760, 534, -1)->Set(17);

  //Bodenlucken
  CreateObject(HA4K, 40, 393, -1);
  CreateObject(HA4K, 2760, 393, -1);

  //Gro�e Bodenlucken
  CreateObject(H24K, 1025, 178, -1);
  CreateObject(H24K, 1775, 178, -1);

  //Stahlbr�cken
  CreateObject(_HBR, 305, 352, -1);
  CreateObject(_HBR, 2495, 352, -1);

  //Glasscheiben
  CreateObject(_WIN, 912, 320, -1);
  CreateObject(_WIN, 912, 370, -1);
  CreateObject(_WIN, 912, 420, -1);
  CreateObject(_WIN, 922, 260, -1);
  CreateObject(_WIN, 982, 170, -1);
  CreateObject(_WIN, 1128, 260, -1);
  CreateObject(_WIN, 1138, 320, -1);
  CreateObject(_WIN, 1138, 370, -1);
  CreateObject(_WIN, 1138, 420, -1);

  CreateObject(_WIN, 1662, 320, -1);
  CreateObject(_WIN, 1662, 370, -1);
  CreateObject(_WIN, 1662, 420, -1);
  CreateObject(_WIN, 1672, 260, -1);
  CreateObject(_WIN, 1818, 170, -1);
  CreateObject(_WIN, 1878, 260, -1);
  CreateObject(_WIN, 1888, 320, -1);
  CreateObject(_WIN, 1888, 370, -1);
  CreateObject(_WIN, 1888, 420, -1);

  //Explosivtanks
  CreateObject(XTNK, 300, 390, -1)->AutoRespawn();
  CreateObject(XTNK, 2500, 390, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 510, 320, -1);
  CreateObject(MWCR, 2290, 320, -1);

  //Kisten
  CreateObject(WCR2, 225, 390, -1)->AutoRespawn();
  CreateObject(WCR2, 660, 390, -1)->AutoRespawn();
  CreateObject(WCR2, 1115, 420, -1)->AutoRespawn();
  CreateObject(WCR2, 1330, 630, -1)->AutoRespawn();
  CreateObject(WCR2, 1480, 630, -1)->AutoRespawn();
  CreateObject(WCR2, 1685, 420, -1)->AutoRespawn();
  CreateObject(WCR2, 2130, 390, -1)->AutoRespawn();
  CreateObject(WCR2, 2575, 390, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 1155, 320, -1)->AutoRespawn();
  CreateObject(XWCR, 1645, 320, -1)->AutoRespawn();

  //Explosivf�sser
  CreateObject(XBRL, 335, 280, -1)->AutoRespawn();
  CreateObject(XBRL, 425, 450, -1)->AutoRespawn();
  CreateObject(XBRL, 760, 450, -1)->AutoRespawn();
  CreateObject(XBRL, 2035, 450, -1)->AutoRespawn();
  CreateObject(XBRL, 2375, 450, -1)->AutoRespawn();
  CreateObject(XBRL, 2465, 280, -1)->AutoRespawn();

  //Phosphorf�sser
  CreateObject(HBRL, 440, 450, -1)->AutoRespawn();
  CreateObject(HBRL, 2360, 450, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 970, 370, -1)->AutoRespawn();
  CreateObject(GSBL, 1830, 370, -1)->AutoRespawn();

  //Benzintanks
  CreateObject(XTNK, 320, 280, -1)->AutoRespawn();
  CreateObject(XTNK, 2480, 280, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 450, 450, -1)->AutoRespawn();
  CreateObject(PTNK, 2355, 450, -1)->AutoRespawn();

  //Sandsackbarrieren
  CreateObject(SBBA, 425, 290, -1)->Right();
  CreateObject(SBBA, 1345, 420, -1);
  CreateObject(SBBA, 1455, 420, -1)->Right();
  CreateObject(SBBA, 2375, 290, -1);

  //Hinweisschilder
  CreateObject(SGNP, 480, 450, -1)->SetPhase(1);
  CreateObject(SGNP, 2320, 450, -1)->SetPhase(1);

  //Schutzt�ren
  CreateObject(GDDR, 230, 290, -1);
  CreateObject(GDDR, 350, 390, -1);
  CreateObject(GDDR, 350, 460, -1);

  CreateObject(GDDR, 530, 450, -1);
  CreateObject(GDDR, 540, 390, -1);
  CreateObject(GDDR, 580, 320, -1);
  CreateObject(GDDR, 650, 320, -1);
  CreateObject(GDDR, 690, 390, -1);
  CreateObject(GDDR, 700, 450, -1);

  CreateObject(GDDR, 990, 260, -1)->SetColorDw(HSL(0, 0, 120, 127));
  CreateObject(GDDR, 990, 370, -1)->SetColorDw(HSL(0, 0, 120, 127));
  CreateObject(GDDR, 1060, 260, -1)->SetColorDw(HSL(0, 0, 120, 127));
  CreateObject(GDDR, 1060, 370, -1)->SetColorDw(HSL(0, 0, 120, 127));

  CreateObject(GDDR, 1740, 260, -1)->SetColorDw(HSL(0, 0, 120, 127));
  CreateObject(GDDR, 1740, 370, -1)->SetColorDw(HSL(0, 0, 120, 127));
  CreateObject(GDDR, 1810, 260, -1)->SetColorDw(HSL(0, 0, 120, 127));
  CreateObject(GDDR, 1810, 370, -1)->SetColorDw(HSL(0, 0, 120, 127));

  CreateObject(GDDR, 2100, 450, -1);
  CreateObject(GDDR, 2110, 390, -1);
  CreateObject(GDDR, 2150, 320, -1);
  CreateObject(GDDR, 2220, 320, -1);
  CreateObject(GDDR, 2260, 390, -1);
  CreateObject(GDDR, 2270, 450, -1);

  CreateObject(GDDR, 2450, 390, -1);
  CreateObject(GDDR, 2450, 460, -1);
  CreateObject(GDDR, 2570, 290, -1);

  //Selbstschussanlagen und Konsolen
  aSelfDefense[0] = CreateObject(SEGU, 415, 329, -1);
    aSelfDefense[0]->Arm(MISA);
    aSelfDefense[0]->SetAutoRepair(1500);
    CreateObject(CONS, 250, 385, -1)->Set(aSelfDefense[0]);
  
  aSelfDefense[1] = CreateObject(SEGU, 2385, 329, -1);
    aSelfDefense[1]->Arm(MISA);
    aSelfDefense[1]->SetAutoRepair(1500);
    CreateObject(CONS, 2550, 385, -1)->Set(aSelfDefense[1]);

  //Tore und Konsolen
  var autod = CreateObject (HNG2, 1400, 610, -1);
  autod->Open();
  CreateObject (CONS, 1270, 605, -1)
  ->Set(autod);
  var autod = CreateObject (HNG2, 1400, 520, -1);
  autod->Open();
  CreateObject (CONS, 1530, 605, -1)
  ->Set(autod);

  //Sounds

  //Wind
  CreateObject(SE4K, 800, 160, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 1990, 160, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 190, 340, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 615, 340, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1025, 340, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1775, 340, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2185, 340, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2610, 340, -1)->Set("Interior*.ogg",670,105);

  //Rush Hour
  CreateObject(SE4K, 820, 390, -1)->Set("Traffic*.ogg",245,70);
  CreateObject(SE4K, 1970, 390, -1)->Set("Traffic*.ogg",245,70);

  //Sirenen
  CreateObject(SE4K, 450, 350, -1)->Set("Siren*.ogg",4000,140);
  CreateObject(SE4K, 2350, 350, -1)->Set("Siren*.ogg",4000,140);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Kugeln
  PlaceSpawnpoint(ABOX, 130, 285);
  PlaceSpawnpoint(ABOX, 1100, 250);
  PlaceSpawnpoint(ABOX, 1700, 250);
  PlaceSpawnpoint(ABOX, 2670, 285);

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 1100, 615);
  PlaceSpawnpoint(GBOX, 1580, 615);

  //Raketen
  PlaceSpawnpoint(MBOX, 950, 250);
  PlaceSpawnpoint(MBOX, 1850, 250);

  //Artilleriebatterien
  CreateObject(ATBY,725,320,-1);
  CreateObject(ATBY,2073,320,-1);

}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Gel�nder
  CreateObject(RAI1, 0, 120, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 360, 390, -1)->SetRail([1]);
  CreateObject(RAI1, 360, 460, -1)->SetRail([1]);
  CreateObject(RAI1, 1330, 420, -1)->SetRail([1,1,1,1,1,1,1]);
  CreateObject(RAI1, 2415, 390, -1)->SetRail([1]);
  CreateObject(RAI1, 2415, 460, -1)->SetRail([1]);
  CreateObject(RAI1, 2740, 120, -1)->SetRail([1,1,1]);

  //Radare
  CreateObject(RADR, 170, 220, -1);
  CreateObject(RADR, 2630, 220, -1);

  //Automaten
  CreateObject(CLVM, 225, 460, -1);
  CreateObject(SPVM, 2570, 460, -1);

  //Glastische
  CreateObject(GTBL, 300, 460, -1);
  CreateObject(GTBL, 2500, 460, -1);

  //Wandlampen
  CreateObject(BLGH, 940, 310, -1);
  CreateObject(BLGH, 1110, 310, -1);
  CreateObject(BLGH, 1690, 305, -1);
  CreateObject(BLGH, 1860, 305, -1);

  //B�sche
  CreateObject(BSH2, 1230, 450, -1);
  CreateObject(BSH2, 1245, 450, -1);
  CreateObject(BSH2, 1550, 450, -1);
  CreateObject(BSH2, 1565, 450, -1);

  //B�ume
  CreateObject(TRE3, 1300, 433, -1);
  CreateObject(TRE3, 1500, 433, -1);

  //L�ftungsgitter
  CreateObject(ENGT, 165, 510, -1);
  CreateObject(ENGT, 2635, 510, -1);

  //Topfpflanzen
  CreateObject(PLT2, 260, 290, -1);
  CreateObject(PLT2, 2540, 290, -1);

  //Scheinwerfer
  CreateObject(FLGH, 1370, 420, -1);
  CreateObject(FLGH, 1430, 420, -1);

  //Flaschen
  CreateObject(BOTL, 290, 448, -1);
  CreateObject(BOTL, 2510, 448, -1);

  //Monitore
  CreateObject(MNI2, 310, 448, -1);
  CreateObject(MNI2, 2485, 448, -1);

  //Pflanzen
  CreateObject(PLNT, 200, 460, -1);
  CreateObject(PLNT, 1010, 310, -1);
  CreateObject(PLNT, 1790, 310, -1);
  CreateObject(PLNT, 2600, 460, -1);

  //Dekoschleusen
  CreateObject(GAT1, 105, 340, -1);
  CreateObject(GAT1, 2695, 340, -1);

  //Warnschilder
  CreateObject(EXSN, 200, 380, -1);
  CreateObject(EXSN, 2600, 380, -1);

  //Z�une
  CreateObject(FENC, 630, 260, -1);
  CreateObject(FENC, 2170, 260, -1);

  //Topfpflanzen
  CreateObject(PLT3, 580, 390, -1);
  CreateObject(PLT3, 2150, 390, -1);

  //Regale
  CreateObject(FRAM, 555, 440, -1);
  CreateObject(FRAM, 675, 440, -1);
  CreateObject(FRAM, 985, 420, -1);
  CreateObject(FRAM, 985, 440, -1);
  CreateObject(FRAM, 1065, 420, -1);
  CreateObject(FRAM, 1065, 440, -1);

  CreateObject(FRAM, 1735, 420, -1);
  CreateObject(FRAM, 1735, 440, -1);
  CreateObject(FRAM, 1815, 420, -1);
  CreateObject(FRAM, 1815, 440, -1);
  CreateObject(FRAM, 2125, 440, -1);
  CreateObject(FRAM, 2245, 440, -1);

  //Notausgangslichter
  CreateObject(ETLT, 325, 430, -1);
  CreateObject(ETLT, 2475, 430, -1);
}

func CreateOptionalFeatures()
{
  //Hintergr�nde
  CreateObject(BD04,220,550,-1);
  CreateObject(BD04,727,550,-1);
  CreateObject(BD04,1234,550,-1);
}

/* Bei Flaggen�bernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[0])
   aSelfDefense[0]->SetTeam(iTeam);

  if(pPoint == aFlag[4])
   aSelfDefense[1]->SetTeam(iTeam);
}

/* Regelw�hler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Offensive.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //DM/LMS-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS))
  {
   //Grenzen setzen
   CreateObject(BRDR, 700, 0, -1)->Set(0);
   CreateObject(BRDR, 2100, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 720, 390, -1);
   CreateObject(SGNP, 720, 450, -1);

   CreateObject(SGNP, 2080, 390, -1);
   CreateObject(SGNP, 2080, 450, -1);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,790,450,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,2010,450,GetTeamColor(2));}

   //Grenzen setzen
   CreateObject(BRDR, 440, 0, -1)->Set(0);
   CreateObject(BRDR, 2360, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 410, 290, -1);
   CreateObject(SGNP, 460, 450, -1);
   CreateObject(SGNP, 2340, 450, -1);
   CreateObject(SGNP, 2385, 290, -1);
  }

  //HTF-Spielziel
  if (FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 1400,420, -1);
   flag->~Set("$Flag3$");

   //Grenzen setzen
   CreateObject(BRDR, 440, 0, -1)->Set(0);
   CreateObject(BRDR, 2360, 0, -1)->Set(1);

   //Sandsackbarrieren
   CreateObject(SBBA, 1221, 440, -1);
   CreateObject(SBBA, 1281, 430, -1);
   CreateObject(SBBA, 1520, 430, -1)->Right();
   CreateObject(SBBA, 1580, 440, -1)->Right();

   //Hinweisschilder
   CreateObject(SGNP, 410, 290, -1);
   CreateObject(SGNP, 460, 450, -1);
   CreateObject(SGNP, 2340, 450, -1);
   CreateObject(SGNP, 2385, 290, -1);

   //Zus�tzliche Munition
   if(!FindObject(NOAM))
   {
    //Kugeln
    PlaceSpawnpoint(ABOX, 1370, 415);

    //Raketen
    PlaceSpawnpoint(MBOX, 1430, 415);
   }
  }

  //Base Assault-Spielziel
  if(FindObject(GBAS))
  {
   //Strukturen
   AddAssaultTarget(CCP2, 100, 390, 400, 1, "$Flag1$", 0, [[130, 540], [200, 540], [260, 560]]);
   AddAssaultTarget(CMSN, 990, 520, 300, 1, "$Flag2$", 1, [[930, 320], [1120, 320], [1000, 170]]);

   AddAssaultTarget(CCP2, 2700, 390, 400, 2, "$Flag5$", 0, [[2670, 540], [2600, 540], [2540, 560]]);
   AddAssaultTarget(CMSN, 1810, 520, 300, 2, "$Flag4$", 1, [[1870, 320], [1680, 320], [1800, 170]]);

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[1]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,100,390,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(100,570);
   aFlag[0] -> AddSpawnPoint(110,210);
   aFlag[0] -> AddSpawnPoint(260,450);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,780,450,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(550,310);
   aFlag[1] -> AddSpawnPoint(570,380);
   aFlag[1] -> AddSpawnPoint(640,250);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,1400,420,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(1060,310);
   aFlag[2] -> AddSpawnPoint(1090,410);
   aFlag[2] -> AddSpawnPoint(1710,410);
   aFlag[2] -> AddSpawnPoint(1740,310);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,2020,450,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(2160,250);
   aFlag[3] -> AddSpawnPoint(2230,380);
   aFlag[3] -> AddSpawnPoint(2250,310);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,2690,390,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(2540,450);
   aFlag[4] -> AddSpawnPoint(2690,210);
   aFlag[4] -> AddSpawnPoint(2700,570);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag5$",0,2);
   }

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[1]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();
  }

  //MR-Spielziel
  if (FindObject(GMNR))
  {
   //Grenzen setzen
   CreateObject(BRDR, 440, 0, -1)->Set(0);
   CreateObject(BRDR, 2360, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 410, 290, -1);
   CreateObject(SGNP, 460, 450, -1);
   CreateObject(SGNP, 2340, 450, -1);
   CreateObject(SGNP, 2385, 290, -1);

   //Gelds�cke
   AddMoneySpawn(1100, 545, [5, 10]);
   AddMoneySpawn(1400, 165, [10, 15, 20]);
   AddMoneySpawn(1400, 415, [10, 15, 20]);
   AddMoneySpawn(1700, 545, [5, 10]);
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
    iX = 940;
    iY = 250;
    if(!Random(2)) iY = 410;
   }
   if(iTeam == 2)
   {
    iX = 1860;
    iY = 250;
    if(!Random(2)) iY = 410;
   }
   return(1);
  }

  //CTF/HTF/MR-Spielziel
  if(FindObject(GCTF) || FindObject(GHTF) || FindObject(GMNR))
  {
   if(iTeam == 1)
   {
    iX = 560;
    iY = 310;
    if(!Random(2)) iY = 380;
   }
   if(iTeam == 2)
   {
    iX = 2240;
    iY = 310;
    if(!Random(2)) iY = 380;
   }
   return(1);
  }

  //Startsicht
  iX = 1400; iY = 300;
}