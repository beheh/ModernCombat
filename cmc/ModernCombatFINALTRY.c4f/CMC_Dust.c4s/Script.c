/*-- Dust --*/

#strict 2
#include CSTD

static aFlag;

func RecommendedGoals()	{return [GOCC, GHTF];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Lichteinstellung
  SetGamma(RGB(3,1,0),RGB(95,83,68),RGB(213,212,187));
  //Flaggen
  aFlag = [];
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

  //Brückensegment
  CreateObject(BRDG, 1210, 455, -1);

  //Leitern
  CreateObject(LADR, 230, 650, -1)->Set(11, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 425, 338, -1)->Set(14, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 865, 790, -1)->Set(14, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 1090, 690, -1)->Set(31, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 1350, 595, -1)->Set(10, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");

  //Bodenluken
  CreateObject(HA4K, 1310, 623, -1);
  CreateObject(HA4K, 1430, 653, -1);

  //Große Bodenluke
  CreateObject(H24K, 425, 228, -1);

  //Gerüst
  CreateObject(SFFG, 1340, 620, -1)->Set(5);

  //Rampen
  DrawMaterialQuad("Wall-Bricks1",1471,730,1441,720,1441,730,1456,730,true);
  DrawMaterialQuad("Wall-Bricks1",1751,410,1721,400,1721,410,1736,410,true);

  //Kisten
  CreateObject(WCR2, 335, 740, -1)->AutoRespawn();
  CreateObject(WCR2, 463, 290, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 160, 680, -1)->AutoRespawn();
  CreateObject(MWCR, 1255, 440, -1)->AutoRespawn();

  //Verbandskiste
  CreateObject(BECR, 790, 450, -1)->AutoRespawn();

  //Explosive Kiste
  CreateObject(XWCR, 1090, 740, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 380, 540, -1)->AutoRespawn();
  CreateObject(PBRL, 390, 540, -1)->AutoRespawn();
  CreateObject(PBRL, 930, 750, -1)->AutoRespawn();
  CreateObject(PBRL, 1390, 720, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 400, 360, -1)->AutoRespawn();
  CreateObject(XBRL, 1405, 720, -1)->AutoRespawn();
  CreateObject(XBRL, 1630, 400, -1)->AutoRespawn();

  //Gasflasche
  CreateObject(GSBL, 385, 360, -1)->AutoRespawn();

  //Steine
  CreateObject(STNE, 540, 380, -1)->Set(0);
  CreateObject(STNE, 1300, 820, -1)->Set(2);

  //Grenze
  CreateObject(BRDR, 0, 900, -1)->Set(3,0,1);

  //Sounds

  //Zikaden
  CreateObject(SE4K, 70, 490, -1)->Set("Cicada*.ogg",1000,100);
  CreateObject(SE4K, 1710, 590, -1)->Set("Cicada*.ogg",1000,100);

  //Schlangen
  CreateObject(SE4K, 400, 780, -1)->Set("Snake*",2000,200);
  CreateObject(SE4K, 1120, 800, -1)->Set("Snake*",2000,200);

  //Wind
  CreateObject(SE4K, 945, 890, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 1550, 820, -1)->Set("WindSound*.ogg",775,250);

  //Erdrutsche
  CreateObject(SE4K, 70, 255, -1)->Set("FallingDirt*.ogg",775,250);
  CreateObject(SE4K, 170, 600, -1)->Set("FallingDirt*.ogg",775,250);
  CreateObject(SE4K, 1520, 790, -1)->Set("FallingDirt*.ogg",775,250);
  CreateObject(SE4K, 1740, 290, -1)->Set("FallingDirt*.ogg",775,250);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 350, 540, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 1395, 490, -1);
  crate->Set(ABOX);

  //Munitionskiste (Gewehrgranaten)
  crate = CreateObject(AMCT, 1055, 780, -1);
  crate->Set(GBOX);

  //Kugeln
  PlaceSpawnpoint(ABOX, 280, 404);
  PlaceSpawnpoint(ABOX, 1255, 805);

  //Raketen
  PlaceSpawnpoint(MBOX, 740, 420);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Radio
  CreateObject(RDIO, 390, 290, -1);

  //Verbrannte Hochöfen
  CreateObject(FNDB, 1010, 790, -1)->SetR(-2);
  CreateObject(FNDB, 1100, 750, -1)->SetR(2);

  //Scheinwerfer
  CreateObject(FLGH, 455, 630, -1)->SetRotation(30);
  CreateObject(FLGH, 975, 470, -1)->SetRotation(100);

  //Glühbirne
  CreateObject(LBGH, 1340, 645, -1);

  //Geländer
  CreateObject(RAI1, 370, 480, -1)->SetRail([1]);
  CreateObject(RAI1, 350, 540, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1250, 690, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1360, 720, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1480, 730, -1)->SetRail([1,1]);

  //Büsche
  CreateObject(BSH2, 320, 470, -1);
  CreateObject(BSH2, 650, 560, -1);
  CreateObject(BSH2, 1275, 825, -1);
  CreateObject(BSH2, 1550, 600, -1);
  CreateObject(BSH2, 1730, 350, -1);
}

func CreateOptionalFeatures()
{
  //Hintergrund
  CreateObject(BD07,600,930,-1);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Grenade.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Toward the Flag.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Firehawk.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,140,680,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(305, 530);
   aFlag[0] -> AddSpawnPoint(380, 470);
   aFlag[0] -> AddSpawnPoint(470, 770);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,145,320,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(360, 410);
   aFlag[1] -> AddSpawnPoint(480, 280);
   aFlag[1] -> AddSpawnPoint(580, 280);
   if(aTeams[4] == true)
   {
    aFlag[1]->Set("$Flag2$",100,2);
    aFlag[1]->Capture(4,1);
   }
   else
   {
    aFlag[1]->Set("$Flag2$",0,2);
   }

   aFlag[2] = CreateObject(OFPL,630,560,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(780, 550);
   aFlag[2] -> AddSpawnPoint(650, 440);
   aFlag[2] -> AddSpawnPoint(815, 440);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,1005,700,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(910, 820);
   aFlag[3] -> AddSpawnPoint(985, 560);
   aFlag[3] -> AddSpawnPoint(1070, 770);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,1495,730,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(1330, 610);
   aFlag[4] -> AddSpawnPoint(1380, 740);
   aFlag[4] -> AddSpawnPoint(1395, 630);
   if(aTeams[3] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(3,1);
   }
   else
   {
    aFlag[4]->Set("$Flag5$",0,2);
   }

   aFlag[5] = CreateObject(OFPL,1705,400,NO_OWNER);
   aFlag[5] -> AddSpawnPoint(1280, 430);
   aFlag[5] -> AddSpawnPoint(1420, 470);
   aFlag[5] -> AddSpawnPoint(1550, 480);
   if(aTeams[2] == true)
   {
    aFlag[5]->Set("$Flag6$",100,2);
    aFlag[5]->Capture(2,1);
   }
   else
   {
    aFlag[5]->Set("$Flag6$",0,2);
   }
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 830, 850, -1);
   flag->~Set("$Flag4$");

   //Kisten
   CreateObject(WCR2, 660, 830, -1);
   CreateObject(WCR2, 960, 830, -1);

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Kugeln
    PlaceSpawnpoint(ABOX, 825, 655);

    //Raketen
    PlaceSpawnpoint(MBOX, 1000, 695);
   }
  }

  //MR-Spielziel
  if(FindObject(GMNR))
  {
   //Geldsäcke
   AddMoneySpawn(585, 815, [20]);
   AddMoneySpawn(790, 555, [20]);
   AddMoneySpawn(965, 465, [20]);
   AddMoneySpawn(1010, 695, [20]);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,130,680,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,1720,400,GetTeamColor(2));}
   if(aTeams[3] == true)
   {CreateFlag(3,1500,730,GetTeamColor(3));}
   if(aTeams[4] == true)
   {CreateFlag(4,145,320,GetTeamColor(4));}
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 875; iY = 565;
   return 1;
  }

  //HTF/MR/LMS/DM-Spielziel
  if(FindObject(GHTF) || FindObject(GMNR) || FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    return [[130, 670], [200, 670], [250, 550]];
   }
   if(iTeam == 2)
   {
    return [[1550, 480], [1620, 440], [1690, 390]];
   }
   if(iTeam == 3)
   {
    return [[1370, 740], [1395, 630], [1500, 720]];
   }
   if(iTeam == 4)
   {
    return [[180, 310], [330, 400], [400, 210]];
   }
   return 1;
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(iTeam == 1)
   {
    return [[305, 530], [380, 470]];
   }
   if(iTeam == 2)
   {
    return [[1380, 405], [1410, 405]];
   }
   if(iTeam == 3)
   {
    return [[1220, 780], [1275, 810]];
   }
   if(iTeam == 4)
   {
    return [[480, 285], [580, 285]];
   }
   return 1;
  }
}