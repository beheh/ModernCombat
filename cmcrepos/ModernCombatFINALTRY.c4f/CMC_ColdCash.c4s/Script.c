/*-- Cold Cash --*/

#strict
#include CSTD

static aFlag;


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetPlayList("CMC_WaitingforSpawn.ogg");
  Music("CMC_WaitingforSpawn.ogg",1);
  //Bildschirmf�rbung
  SetGamma(RGB(2,3,10), RGB(131,135,158), RGB(252,253,255) );
  //Himmelparallaxit�t
  SetSkyParallax(1, 40, 40, 0, 0, SkyPar_Keep, SkyPar_Keep);
  //Flaggen
  aFlag = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausr�stung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  return(1);
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Leitern
  CreateObject(LADR, 1185, 520, -1)->Set(6);
  CreateObject(LADR, 1335, 510, -1)->Set(18);
  CreateObject(LADR, 1665, 510, -1)->Set(18);
  CreateObject(LADR, 2690, 590, -1)->Set(9);

  CreateObject(LADR, 2850, 590, -1)->Set(9);
  CreateObject(LADR, 3875, 510, -1)->Set(18);
  CreateObject(LADR, 4205, 510, -1)->Set(18);
  CreateObject(LADR, 4355, 520, -1)->Set(6);

  //Bodenluken
  CreateObject(HA4K, 1390, 513, -1);
  CreateObject(HA4K, 1590, 513, -1);

  CreateObject(HA4K, 2690, 513, -1);
  CreateObject(HA4K, 2770, 513, -1);
  CreateObject(HA4K, 2850, 513, -1);

  CreateObject(HA4K, 3950, 513, -1);
  CreateObject(HA4K, 4150, 513, -1);

  //Gro�e Bodenluken
  CreateObject(H24K, 1335, 368, -1);
  CreateObject(H24K, 1335, 478, -1);
  CreateObject(H24K, 1665, 368, -1);
  CreateObject(H24K, 1665, 478, -1);

  CreateObject(H24K, 3875, 368, -1);
  CreateObject(H24K, 3875, 478, -1);
  CreateObject(H24K, 4205, 368, -1);
  CreateObject(H24K, 4205, 478, -1);

  //Glasscheiben
  CreateObject(_WIN, 1282, 410, -1);
  CreateObject(_WIN, 1718, 410, -1);
  CreateObject(_WIN, 1718, 470, -1);

  CreateObject(_WIN, 3822, 410, -1);
  CreateObject(_WIN, 3822, 470, -1);
  CreateObject(_WIN, 4258, 410, -1);

  //Schutzt�ren
  CreateObject(GDDR, 1290, 510, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1430, 510, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1570, 510, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1710, 510, -1)->SetColorDw(HSL(145, 210, 100, 127));

  CreateObject(GDDR, 3830, 510, -1);
  CreateObject(GDDR, 3970, 510, -1);
  CreateObject(GDDR, 4110, 510, -1);
  CreateObject(GDDR, 4250, 510, -1);

  //Metallkisten
  CreateObject(MWCR, 1530, 510, -1)->AutoRespawn();
  CreateObject(MWCR, 4010, 510, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 1045, 550, -1)->AutoRespawn();
  CreateObject(BECR, 4495, 550, -1)->AutoRespawn();

  //Benzinf�sser
  CreateObject(PBRL, 1240, 470, -1)->AutoRespawn();
  CreateObject(PBRL, 4300, 470, -1)->AutoRespawn();

  //Phosphorf�sser
  CreateObject(HBRL, 1400, 360, -1)->AutoRespawn();
  CreateObject(HBRL, 4140, 360, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1450, 510, -1)->AutoRespawn();
  CreateObject(GSBL, 4090, 510, -1)->AutoRespawn();

  //Paletten
  CreateObject(PLLT, 1440, 360, -1);
  CreateObject(PLLT, 4100, 360, -1);

  //Sandsackbarrieren
  CreateObject(SBBA, 1280, 360, -1);
  CreateObject(SBBA, 1720, 360, -1)->Right();
  CreateObject(SBBA, 2661, 510, -1);
  CreateObject(SBBA, 2880, 510, -1)->Right();
  CreateObject(SBBA, 3820, 360, -1);
  CreateObject(SBBA, 4260, 360, -1)->Right();

  //Explosivtanks
  CreateObject(XTNK, 1615, 360, -1)->AutoRespawn();
  CreateObject(XTNK, 3925, 360, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 2730, 510, -1)->AutoRespawn();
  CreateObject(PTNK, 2810, 510, -1)->AutoRespawn();

  //Hinweisschilder
  CreateObject(SNPT, 950, 520, -1);
  CreateObject(SNPT, 4600, 520, -1);

  //Grenzen
  CreateObject(BRDR, 940, 0, -1)->Set(0);
  CreateObject(BRDR, 4600, 0, -1)->Set(1);

  //Sounds

  //Wellen
  CreateObject(SE4K, 500, 560, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 5030, 560, -1)->Set("Wave*.ogg",105,35);

  //Hallen
  CreateObject(SE4K, 1500, 440, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2770, 540, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 4040, 440, -1)->Set("Interior*.ogg",665,105);

  //Wind
  CreateObject(SE4K, 600, 450, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 1120, 450, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 4350, 450, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 4930, 450, -1)->Set("WindSound*.ogg",245,105);

  //Windb�en
  CreateObject(SE4K, 2180, 450, -1)->Set("WindGust*.ogg",775,250);
  CreateObject(SE4K, 2770, 420, -1)->Set("WindGust*.ogg",775,250);
  CreateObject(SE4K, 3360, 450, -1)->Set("WindGust*.ogg",775,250);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 1385, 450, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 4155, 450, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  crate = CreateObject (AMCT, 1615, 510, -1);
  crate->Set(GBOX);
  crate = CreateObject (AMCT, 3925, 510, -1);
  crate->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MBOX, 1310, 355);
  PlaceSpawnpoint(MBOX, 4230, 355);

  //Gesch�tzstellungen
  CreateObject(GNET, 1520, 360, -1)->Set(SATW);
  CreateObject(GNET, 4020, 360, -1)->Set(SATW);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Orientierungslichter
  CreateObject(OLGH, 1500, 360, -1)->Set(8, 15, 1, 1, 60);
  CreateObject(OLGH, 2770, 510, -1)->Set(30, 11, 1, 1, 20);
  CreateObject(OLGH, 4040, 360, -1)->Set(8, 15, 1, 1, 60);

  //Stra�enlichter
  CreateObject(SLGH, 1300, 360, -1);
  CreateObject(SLGH, 1500, 360, -1);
  CreateObject(SLGH, 1700, 360, -1);
  CreateObject(SLGH, 2670, 510, -1);
  CreateObject(SLGH, 2870, 510, -1);
  CreateObject(SLGH, 3840, 360, -1);
  CreateObject(SLGH, 4040, 360, -1);
  CreateObject(SLGH, 4240, 360, -1);

  //B�sche
  CreateObject(BSH2, 1150, 550, -1);
  CreateObject(BSH2, 1170, 550, -1)->SetR(180);

  CreateObject(BSH2, 2170, 470, -1);
  CreateObject(BSH2, 2180, 470, -1)->SetR(90);
  CreateObject(BSH2, 2200, 470, -1);

  CreateObject(BSH2, 2480, 590, -1);
  CreateObject(BSH2, 2500, 590, -1);

  CreateObject(BSH2, 3040, 590, -1);
  CreateObject(BSH2, 3060, 590, -1);

  CreateObject(BSH2, 3340, 470, -1);
  CreateObject(BSH2, 3360, 470, -1)->SetR(90);
  CreateObject(BSH2, 3380, 470, -1);

  CreateObject(BSH2, 4360, 550, -1)->SetR(180);
  CreateObject(BSH2, 4370, 550, -1);

  //Bojen
  CreateObject(BUOY, 840, 540, -1)->SetR(40);
  CreateObject(BUOY, 4700, 540, -1)->SetR(-20);

  //Radar
  CreateObject(RADR, 2820, 430, -1);

  //Kaputtes U-Boot
  CreateObject(SUBB, 480, 680, -1);

  //Kaputtes Boot
  CreateObject(SLBB, 4850, 525, -1);

  //Ventillatoren
  CreateObject(VENT, 1500, 490, -1)->SetCon(60);
  CreateObject(VENT, 4040, 490, -1)->SetCon(60);

  //Monitore
  CreateObject(MNI2, 1530, 492, -1);
  CreateObject(MNI2, 4010, 492, -1);

  //Gel�nder
  CreateObject(RAI1, 1355, 360, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 2660, 510, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 3895, 360, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);

  //Z�une
  CreateObject(FENC, 1250, 470, -1);
  CreateObject(FENC, 4290, 470, -1);

  //L�ftungsgitter
  CreateObject(ENGT, 1370, 510, -1)->SetCon(70);
  CreateObject(ENGT, 1400, 510, -1)->SetCon(70);
  CreateObject(ENGT, 1600, 510, -1)->SetCon(70);
  CreateObject(ENGT, 1630, 510, -1)->SetCon(70);

  CreateObject(ENGT, 3910, 510, -1)->SetCon(70);
  CreateObject(ENGT, 3940, 510, -1)->SetCon(70);
  CreateObject(ENGT, 4140, 510, -1)->SetCon(70);
  CreateObject(ENGT, 4170, 510, -1)->SetCon(70);
}

func CreateOptionalFeatures()
{
  //Hintergr�nde
  CreateObject(BD03,1250,680,-1);
  CreateObject(BD02,1750,580,-1);
  CreateObject(BD01,1250,680,-1);
}

/* Regelw�hler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Slow Motion.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Blackhawk und Hinweisschilder
  if(!FindObject(NOBH))
  {
   SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,2770,480,-1),100*21,300);

   CreateObject(SGNP, 2655, 550, -1)->SetPhase(2);
   CreateObject(SGNP, 2885, 550, -1)->SetPhase(2);
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,1480,510,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(1100, 570);
   aFlag[0] -> AddSpawnPoint(1250, 570);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,2770,630,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(2710,500);
   aFlag[1] -> AddSpawnPoint(2745,530);
   aFlag[1] -> AddSpawnPoint(2830,500);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,4060,510,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(4290, 570);
   aFlag[2] -> AddSpawnPoint(4440, 570);
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

  //Base Assault-Spielziel
  if(FindObject(GBAS))
  {
   //Zielobjekte
   AddAssaultTarget(CMSN, 1480, 510, 300, 1, "$Target1$", 0, [[1100, 580], [1250, 580]]);
   AddAssaultTarget(RADR, 1570, 320, 400, 1, "$Target2$", 1, [[1290, 470], [1360, 360]]);

   AddAssaultTarget(CMSN, 4060, 510, 300, 2, "$Target1$", 3, [[4290, 580], [4440, 580]]);
   AddAssaultTarget(RADR, 3970, 320, 400, 2, "$Target2$", 2, [[4180, 360], [4250, 470]]);
  }

  //HTF-Spielziel
  if (FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 2770, 630, -1);
   flag->~Set("$Flag2$");

   //Zus�tzliche Munition
   if(!FindObject(NOAM))
   {
    //Versorgungskiste (Kugeln)
    var crate = CreateObject(AMCT, 2680, 650, -1);
    crate->Set(ABOX);

    //Raketen
    PlaceSpawnpoint(MBOX, 2860, 645);
   }
  }

  //MR-Spielziel
  if (FindObject(GMNR))
  {
   //Gelds�cke
   AddMoneySpawn(2180, 455, [30]);
   AddMoneySpawn(2770, 625, [30]);
   AddMoneySpawn(3360, 455, [30]);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,1480,510,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,4060,510,GetTeamColor(2));}
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //HTF/MR/CTF/LMS/DM-Spielziel
  if(FindObject(GHTF) || FindObject(GMNR) || FindObject(GCTF) || FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 1220; iY = 460; }
    if(!--rand)
     { iX = 1370; iY = 350; }
    return(1);
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 4170; iY = 350; }
    if(!--rand)
     { iX = 4320; iY = 460; }
    return(1);
   }
  }

  //Startsicht
  iX = 2770; iY = 420;
}