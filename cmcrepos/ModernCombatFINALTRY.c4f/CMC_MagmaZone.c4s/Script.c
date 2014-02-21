/*-- Magma Zone --*/

#strict
#include CSTD

static aFlag,aSelfDefense;

func RecommendedGoals()	{return [GOCC, GBAS];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(0,15,15);
  //Flaggen
  aFlag = [];
  //Selbstschussanlage
  aSelfDefense = [];
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

  //Bodenluken
  CreateObject(HA4K, 410, 333, -1);
  CreateObject(HA4K, 800, 393, -1);
  CreateObject(HA4K, 1340, 343, -1);
  CreateObject(HA4K, 1340, 463, -1);
  CreateObject(HA4K, 1740, 373, -1);
  CreateObject(HA4K, 2120, 243, -1);

  //Leitern
  CreateObject(LADR, 410, 442, -1)->Set(13);
  CreateObject(LADR, 800, 502, -1)->Set(13);
  CreateObject(LADR, 1340, 540, -1)->Set(24);
  CreateObject(LADR, 1740, 466, -1)->Set(11);
  CreateObject(LADR, 2120, 528, -1)->Set(35);

  //Sprungschanzen
  CreateObject (JMPD, 365, 330, -1)->Set(100, 20);
  CreateObject (JMPD, 1375, 300, -1)->Set(85, 10);

  //Stahlbrücken
  CreateObject(_HBR, 785, 660, -1);
  CreateObject(_HBR, 1740, 475, -1);
  CreateObject(_HBR, 1925, 510, -1);

  //Benzinfässer
  CreateObject(PBRL, 625, 520, -1)->AutoRespawn();
  CreateObject(PBRL, 1790, 290, -1)->AutoRespawn();

  //Giftfässer
  CreateObject(TBRL, 1380, 340, -1)->AutoRespawn();
  CreateObject(TBRL, 1395, 340, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 2065, 480, -1)->AutoRespawn();
  CreateObject(XBRL, 2100, 560, -1)->AutoRespawn();
  CreateObject(XBRL, 2115, 560, -1)->AutoRespawn();

  //Explosivtank
  CreateObject(XTNK, 2100, 560, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 800, 260, -1)->AutoRespawn();
  CreateObject(GSBL, 820, 390, -1)->AutoRespawn();
  CreateObject(GSBL, 1760, 370, -1)->AutoRespawn();

  //Steine
  CreateObject(STNE, 170, 435, -1);
  CreateObject(STNE, 400, 655, -1)->Set(2);
  CreateObject(STNE, 830, 165, -1)->Set(0);
  CreateObject(STNE, 1140, 435, -1)->Set(2);
  CreateObject(STNE, 1645, 495, -1)->Set(0);
  CreateObject(STNE, 2270, 375, -1);

  //Kisten
  CreateObject(WCR2, 760, 260, -1)->AutoRespawn();
  CreateObject(WCR2, 770, 242, -1);
  CreateObject(WCR2, 780, 260, -1)->AutoRespawn();
  CreateObject(WCR2, 840, 390, -1)->AutoRespawn();
  CreateObject(WCR2, 844, 370, -1);
  CreateObject(WCR2, 1780, 370, -1)->AutoRespawn();

  //Verbandskiste
  CreateObject(BECR, 1290, 460, -1)->AutoRespawn();

  //Hinweisschild
  var sign = CreateObject(SGNP, 1390, 550, -1);
  sign->SetMode(1);
  sign->Set("Turret");

  //Selbstschussanlage und Konsole
  aSelfDefense[0] = CreateObject(SEGU, 1305, 369, -1);
  CreateObject(CONS, 1300, 330, -1)->Set(aSelfDefense[0]);

  //Sounds

  //Lava
  CreateObject(SE4K, 150, 660, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 710, 660, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 1200, 580, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 1730, 500, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 2260, 310, -1)->Set("Lava*",1200,350);

  //Erdrutsche
  CreateObject(SE4K, 470, 480, -1)->Set("FallingDirt*.ogg",800,250);
  CreateObject(SE4K, 1200, 200, -1)->Set("FallingDirt*.ogg",800,250);
  CreateObject(SE4K, 2240, 450, -1)->Set("FallingDirt*.ogg",800,250);

  //Hallen
  CreateObject(SE4K, 290, 250, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1275, 395, -1)->Set("Interior*.ogg",765,105);
  CreateObject(SE4K, 2170, 290, -1)->Set("Interior*.ogg",665,105);

  //Lautsprecher
  CreateObject(SE4K, 545, 250, -1)->Set("Announce*.ogg",3000,200);
  CreateObject(SE4K, 1925, 210, -1)->Set("Announce*.ogg",3000,200);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 1290, 80);

  //Raketen
  PlaceSpawnpoint(MBOX, 1360, 330);

  //Automat
  var store = CreateObject(WPVM,1305, 300,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(BWTH,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(ATWN,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
  store->AddWare(SRBL,-1);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Alarmlampen
  CreateObject(ALGH, 185, 260, -1)->TurnOn();
  CreateObject(ALGH, 1340, 250, -1)->TurnOn();
  CreateObject(ALGH, 2300, 170, -1)->TurnOn();

  //Radar
  CreateObject(RADR, 2085, 240, -1);

  //Feuerlöscher
  CreateObject(FIE2, 215, 295, -1);
  CreateObject(FIE2, 1375, 335, -1);
  CreateObject(FIE2, 2320, 205, -1);

  //Bildschirme
  CreateObject(SCR3, 410, 271, -1);
  CreateObject(SCR3, 1340, 281, -1);
  CreateObject(SCR3, 2120, 151, -1);

  //Ventillatoren
  CreateObject(VENT, 365, 240, -1)->SetCon(20);
  CreateObject(VENT, 365, 290, -1)->SetCon(20);
  CreateObject(VENT, 365, 330, -1)->SetCon(20);

  CreateObject(VENT, 765, 300, -1)->SetCon(20);
  CreateObject(VENT, 765, 350, -1)->SetCon(20);
  CreateObject(VENT, 765, 390, -1)->SetCon(20);

  CreateObject(VENT, 1305, 375, -1)->SetCon(20);
  CreateObject(VENT, 1375, 375, -1)->SetCon(20);

  CreateObject(VENT, 1775, 330, -1)->SetCon(20);

  CreateObject(VENT, 2085, 520, -1)->SetCon(20);

  CreateObject(VENT, 2155, 155, -1)->SetCon(20);
  CreateObject(VENT, 2155, 200, -1)->SetCon(20);
  CreateObject(VENT, 2155, 240, -1)->SetCon(20);

  //Regale
  CreateObject(FRAM, 430, 330, -1);
  CreateObject(FRAM, 450, 330, -1);
  CreateObject(FRAM, 470, 330, -1);

  //Büsche
  CreateObject(BSH2, 295, 130, -1);
  CreateObject(BSH2, 2305, 515, -1);

  //Scheinwerfer
  CreateObject(FLGH, 445, 330, -1)->SetRotation(20);
  CreateObject(FLGH, 1775, 290, -1)->SetRotation(-80);
  CreateObject(FLGH, 2160, 240, -1)->SetRotation(-20);

  //Geländer
  CreateObject(RAI1, 143, 300, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 760, 390, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1300, 340, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1300, 460, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1700, 370, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2080, 240, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2080, 560, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2250, 210, -1)->SetRail([1,3,1,3,1,3,1]);

  //Schilder
  CreateObject(MSGN, 140, 300, -1);
  CreateObject(MSGN, 230, 300, -1);
  CreateObject(MSGN, 2240, 210, -1);
  CreateObject(MSGN, 2330, 210, -1);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[2])
   aSelfDefense[0]->SetTeam(iTeam);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Moving Squad.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Sprungschanzen
  if(FindObject(FDMG))
  {
   CreateObject (JMPD, 1385, 460, -1)->Set(40, -20);
   CreateObject (JMPD, 1690, 370, -1)->Set(75, -80);
  }
  else
  {
   CreateObject (JMPD, 1690, 370, -1)->Set(85, -80);
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,165,300,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(300,490);
   aFlag[0] -> AddSpawnPoint(355,450);
   aFlag[0] -> AddSpawnPoint(545,340);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,780,390,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(730,510);
   aFlag[1] -> AddSpawnPoint(825,570);
   aFlag[1] -> AddSpawnPoint(885,480);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,1375,460,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(1285,540);
   aFlag[2] -> AddSpawnPoint(1395,540);
   aFlag[2] -> AddSpawnPoint(1205,410);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,1705,330,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(1665,150);
   aFlag[3] -> AddSpawnPoint(1790,130);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,2155,560,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(1855,440);
   aFlag[4] -> AddSpawnPoint(1980,510);
   aFlag[4] -> AddSpawnPoint(2090,470);
   aFlag[4]->Set("$Flag5$",0,2);

   aFlag[5] = CreateObject(OFPL,2255,210,NO_OWNER);
   aFlag[5] -> AddSpawnPoint(2000,300);
   aFlag[5] -> AddSpawnPoint(2100,300);
   aFlag[5] -> AddSpawnPoint(2140,300);
   if(aTeams[2] == true)
   {
    aFlag[5]->Set("$Flag6$",100,2);
    aFlag[5]->Capture(2,1);
   }
   else
   {
    aFlag[5]->Set("$Flag6$",0,2);
   }

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Versorgungskiste (Kugeln)
    var crate = CreateObject(AMCT, 245, 550, -1);
    crate->Set(ABOX);
    crate = CreateObject(AMCT, 1315, 460, -1);
    crate->Set(ABOX);
    crate = CreateObject(AMCT, 2075, 560, -1);
    crate->Set(ABOX);
   }
  }

  //Base Assault-Spielziel
  if(FindObject(GBAS))
  {
   //Strukturen
   AddAssaultTarget(CCP2, 180, 300, 400, 1, "$Flag1$", 0, [[350, 460], [245, 550], [550, 350]]);
   AddAssaultTarget(CMSN, 680, 150, 300, 1, "$Flag2$", 1, [[610, 300], [840, 260], [880, 270]]);
   AddAssaultTarget(CCP1, 780, 390, 300, 1, "$Flag3$", 2, [[730, 520], [825, 580], [605, 520]]);

   AddAssaultTarget(CCP2, 2280, 210, 400, 2, "$Flag6$", 0, [[2100, 310], [2000, 310], [1900, 320]]);
   AddAssaultTarget(CMSN, 1840, 130, 300, 2, "$Flag4$", 1, [[1845, 240], [1770, 220]]);
   AddAssaultTarget(CCP1, 1710, 330, 300, 2, "$Flag5$", 2, [[1610, 500], [1820, 450], [1885, 475]]);

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Versorgungskiste (Kugeln)
    var crate = CreateObject(AMCT, 245, 550, -1);
    crate->Set(ABOX);
    crate = CreateObject(AMCT, 1315, 460, -1);
    crate->Set(ABOX);
    crate = CreateObject(AMCT, 2075, 560, -1);
    crate->Set(ABOX);
   }
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,455,330,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,2010,310,GetTeamColor(2));}

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Versorgungskiste (Kugeln)
    var crate = CreateObject(AMCT, 605, 520, -1);
    crate->Set(ABOX);
    crate = CreateObject(AMCT, 1825, 450, -1);
    crate->Set(ABOX);
   }
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   //Grenzen setzen
   CreateObject(BRDR, 510, 0, -1)->Set(0);
   CreateObject(BRDR, 1965, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 525, 350, -1);
   CreateObject(SGNP, 585, 650, -1);
   CreateObject(SGNP, 560, 530, -1);
   CreateObject(SGNP, 660, 150, -1);
   CreateObject(SGNP, 1850, 130, -1);
   CreateObject(SGNP, 1850, 450, -1);
   CreateObject(SGNP, 1960, 300, -1);

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Versorgungskiste (Kugeln)
    var crate = CreateObject(AMCT, 605, 520, -1);
    crate->Set(ABOX);
    crate = CreateObject(AMCT, 1825, 450, -1);
    crate->Set(ABOX);
   }
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 1545; iY = 350;
   return(1);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(iTeam == 1)
   {
    return [[150, 290], [220, 290]];
   }
   if(iTeam == 2)
   {
    return [[2155, 230], [2250, 200]];
   }
   return(1);
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    return [[635, 280], [685, 390]];
   }
   if(iTeam == 2)
   {
    return [[1825, 350], [1845, 230]];
   }
   return(1);
  }
}