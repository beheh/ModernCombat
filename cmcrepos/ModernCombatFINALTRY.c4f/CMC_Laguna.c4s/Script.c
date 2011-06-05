/*-- Laguna --*/

#strict
#include CSTD

static aFlag,aLamp,aArtillery;


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetPlayList("CMC_WaitingforSpawn.ogg");
  Music("CMC_WaitingforSpawn.ogg",1);
  //Himmel
  SetSkyParallax(0,45,24,0,0,0,0);
  //Hintergrundfärbung
  SetSkyAdjust(RGBa(255,255,255,220));
  //Flaggen
  aFlag = [];
  //Lampen
  aLamp = [];
  //Artillerie
  aArtillery = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
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

  //Rampen
  DrawMaterialQuad("Wall-Concrete3",771,500,741,490,741,500,756,500,true);
  DrawMaterialQuad("Wall-Concrete3",801,510,771,500,771,510,786,510,true);

  DrawMaterialQuad("Wall-Metal2",2910,570,2940,560,2940,570,2925,570,true);
  DrawMaterialQuad("Wall-Metal2",2940,560,2970,550,2970,560,2955,560,true);
  DrawMaterialQuad("Wall-Concrete3",2970,550,3000,540,3000,550,2985,550,true);

  //Leitern
  CreateObject(LADR, 540, 690, -1)->Set(49);
  CreateObject(LADR, 645, 690, -1)->Set(49);

  CreateObject(LADR, 1370, 710, -1)->Set(9);

  CreateObject(LADR, 1930, 732, -1)->Set(18);
  CreateObject(LADR, 2015, 580, -1)->Set(14);
  CreateObject(LADR, 2100, 732, -1)->Set(18);

  CreateObject(LADR, 2830, 730, -1)->Set(5);
  CreateObject(LADR, 2895, 632, -1)->Set(23);

  //Bodenluken
  CreateObject(HA4K, 540, 293, -1);
  CreateObject(HA4K, 540, 353, -1);
  CreateObject(HA4K, 540, 493, -1);
  CreateObject(HA4K, 540, 643, -1);

  CreateObject(HA4K, 1930, 583, -1);
  CreateObject(HA4K, 2100, 583, -1);

  //Große Bodenluken
  CreateObject(H24K, 645, 248, -1);
  CreateObject(H24K, 645, 298, -1);
  CreateObject(H24K, 645, 358, -1);
  CreateObject(H24K, 645, 498, -1);

  CreateObject(H24K, 2895, 448, -1);
  CreateObject(H24K, 2895, 578, -1);

  //Stahlbrücken
  CreateObject(_HBR, 875, 522, -1);
  CreateObject(_HBR, 965, 522, -1);

  CreateObject(_HBR, 1785, 602, -1);
  CreateObject(_HBR, 2015, 592, -1);
  CreateObject(_HBR, 2255, 562, -1);

  //Haie
  CreateObject(SHRK, 230, 850, -1)->AutoRespawn();
  CreateObject(SHRK, 330, 850, -1)->AutoRespawn();
  CreateObject(SHRK, 630, 850, -1)->AutoRespawn();
  CreateObject(SHRK, 730, 850, -1)->AutoRespawn();
  CreateObject(SHRK, 1030, 850, -1)->AutoRespawn();

  //Glasscheiben
  CreateObject(_WIN, 512, 440, -1);
  CreateObject(_WIN, 512, 640, -1);
  CreateObject(_WIN, 738, 440, -1);
  CreateObject(_WIN, 738, 640, -1);
  CreateObject(_WIN, 2792, 490, -1);
  CreateObject(_WIN, 2998, 490, -1);

  //Kisten
  CreateObject(WCR2, 620, 490, -1)->AutoRespawn();
  CreateObject(WCR2, 1000, 510, -1);
  CreateObject(WCR2, 1970, 760, -1)->AutoRespawn();
  CreateObject(WCR2, 2060, 580, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 532, 240, -1);
  CreateObject(MWCR, 2870, 570, -1);

  //Verbandskiste
  CreateObject(BECR, 1990, 760, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 561, 350, -1);
  CreateObject(XWCR, 1980, 742, -1);
  CreateObject(XWCR, 2970, 680, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 580, 350, -1)->AutoRespawn();
  CreateObject(XBRL, 680, 690, -1)->AutoRespawn();
  CreateObject(XBRL, 1975, 490, -1)->AutoRespawn();
  CreateObject(XBRL, 2815, 440, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 1020, 510, -1)->AutoRespawn();
  CreateObject(HBRL, 2010, 760, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 665, 690, -1)->AutoRespawn();
  CreateObject(GSBL, 2080, 580, -1)->AutoRespawn();
  CreateObject(GSBL, 2850, 570, -1)->AutoRespawn();
  CreateObject(GSBL, 2860, 570, -1)->AutoRespawn();

  //Explosivtanks
  CreateObject(XTNK, 590, 640, -1)->AutoRespawn();
  CreateObject(XTNK, 1950, 490, -1)->AutoRespawn();
  CreateObject(XTNK, 2940, 440, -1)->AutoRespawn();

  //Sandsackbarrieren
  CreateObject(SBBA, 830, 510, -1)->Right();

  CreateObject(SBBA, 1370, 740, -1);
  CreateObject(SBBA, 1445, 740, -1)->Right();

  CreateObject(SBBA, 1821, 490, -1);
  CreateObject(SBBA, 2160, 470, -1)->Right();

  CreateObject(SBBA, 2801, 440, -1);
  CreateObject(SBBA, 2990, 440, -1)->Right();

  //Schutztüren
  CreateObject(GDDR, 515, 490, -1);
  CreateObject(GDDR, 515, 690, -1);

  CreateObject(GDDR, 735, 490, -1);
  CreateObject(GDDR, 735, 690, -1);

  //Lampen
  aLamp[00]=CreateObject(BLGH, 560, 440, -1);
  aLamp[01]=CreateObject(BLGH, 560, 580, -1);
  aLamp[02]=CreateObject(BLGH, 690, 440, -1);
  aLamp[03]=CreateObject(BLGH, 690, 580, -1);
  aLamp[04]=CreateObject(BLGH, 2850, 520, -1);
  aLamp[05]=CreateObject(BLGH, 2940, 520, -1);

  //Sounds

  //Wind
  CreateObject(SE4K, 900, 435, -1)->Set("WindSound*.ogg",775,250);

  //Möven
  CreateObject(SE4K, 380, 660, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 1170, 630, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 1740, 705, -1)->Set("SeaSounds*.ogg",140,35);

  //Wellen
  CreateObject(SE4K, 170, 750, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 910, 750, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1230, 750, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1780, 750, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 2350, 750, -1)->Set("Wave*.ogg",105,35);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject (AMCT, 600, 690, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 1940, 490, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 2960, 440, -1);
  crate->Set(ABOX);

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 1675, 645);
  PlaceSpawnpoint(GBOX, 2215, 695);

  //Raketen
  PlaceSpawnpoint(MBOX, 680, 285);
  PlaceSpawnpoint(MBOX, 2920, 655);

  //Artilleriebatterien
  aArtillery[0] = CreateObject(ATBY,1855,490,-1);
  aArtillery[0]->SetRotation(-60);
  aArtillery[1] = CreateObject(ATBY,2130,470,-1);
  aArtillery[1]->SetRotation(20);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Geländer
  CreateObject(RAI1, 515, 290, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 515, 350, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 515, 490, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 515, 640, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 515, 690, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1]);

  CreateObject(RAI1, 1910, 580, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 2060, 580, -1)->SetRail([1,1,1]);

  CreateObject(RAI1, 2800, 440, -1)->SetRail([1,1,1,1,1,1,1,1,1,1]);

  //Radar
  CreateObject(RADR, 2985, 370, -1)->SetClrModulation(RGB(125,125,125));

  //Orientierungslichter
  CreateObject(OLGH, 625, 290, -1)->Set(7, 15, 1, 1, 40);
  CreateObject(OLGH, 2895, 440, -1)->Set(6, 15, 1, 1, 40);

  //Automaten
  CreateObject(VGMN, 700, 690, -1);
  CreateObject(VGMN, 2840, 570, -1)->Set(3);

  //Bojen
  CreateObject(BUOY, 230, 813,  -1);

  CreateObject(BUOY, 1900, 813,  -1);
  CreateObject(BUOY, 2130, 813,  -1);

  CreateObject(BUOY, 2535, 813,  -1);

  //Lüftungsgitter
  CreateObject(ENGT, 605, 460, -1);
  CreateObject(ENGT, 605, 600, -1);

  //Scheinwerfer
  CreateObject(FLGH, 560, 290, -1)->SetRotation(-90);
  CreateObject(FLGH, 705, 290, -1)->SetRotation(90);

  //Dekoschleusen
  CreateObject(GAT1, 690, 645, -1);
  CreateObject(GAT1, 2940, 495, -1);

  //Schild
  CreateObject(WLSN, 560, 690, -1);

  //Regale
  CreateObject(FRAM, 580, 690, -1);
  CreateObject(FRAM, 600, 690, -1);
  CreateObject(FRAM, 620, 690, -1);

  //Notausgangslichter
  CreateObject(ETLT, 705, 470, -1);
  CreateObject(ETLT, 2960, 540, -1);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD03,1250,530,-1)->SetClrModulation(RGB(125,125,125));
  CreateObject(BD05,1200,800,-1)->SetClrModulation(RGB(125,125,125));
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Your Eyes.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //DM/LMS-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS))
  {
   //Grenzen setzen
   CreateObject(BRDR, 1300, 0, -1)->Set(0);
   CreateObject(BRDR, 2770, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 1255, 590, -1);
   CreateObject(SGNP, 2745, 570, -1);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1485,800,-1),10*10);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2605,800,-1),10*10);
  }

  //HTF-Spielziel
  if (FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 2015,760, -1);
   flag->~Set("$Flag3$");

   //Grenzen setzen
   CreateObject(BRDR, 1300, 0, -1)->Set(0);
   CreateObject(BRDR, 2770, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 1255, 590, -1);
   CreateObject(SGNP, 2745, 570, -1);

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Kugeln
    PlaceSpawnpoint(ABOX, 2070, 755);

    //Raketen
    PlaceSpawnpoint(MBOX, 1930, 650);
   }

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1485,800,-1),10*10);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2605,800,-1),10*10);
  }

  //Assault-Spielziel
  if (FindObject(GASS))
  {
   //Zielobjekte
   AddAssaultTarget(RADR, 1450, 640, 250, 2, "$Target1$", 0, [[[1730, 630], [1730, 730]], [[580, 290], [610, 350], [580, 490]]]);
   AddAssaultTarget(CMSN, 1410, 740, 250, 2, "$Target2$", 1, [[[1730, 630], [1730, 730]], [[580, 290], [580, 350], [580, 490]]]);
   AddAssaultTarget(CCP2, 2050, 760, 300, 2, "$Target3$", 2, [[[2210, 550], [2060, 470]], [[1210, 560], [1190, 730]]]);
   AddAssaultTarget(GSTA, 2815, 750, 300, 2, "$Target4$", 3, [[[3080, 430], [3220, 600], [3130, 650]], [[1910, 580], [1855, 730], [1905, 450]]]);
   AddAssaultTarget(LBPC, 2850, 570, 300, 2, "$Target5$", 4, [[[3080, 430], [3220, 600], [3130, 650]], [[1910, 580], [1855, 730], [1905, 450]]]);

   //Ziele verbinden
   ConnectAssaultTargets([0, 1]);
   ConnectAssaultTargets([3, 4]);

   //Grenzen setzen
   CreateObject(BRDR, 230, 0, -1)->Set(0,1);
   CreateObject(BRDR, 1820, 0, -1)->Set(1,1);

   //Hinweisschild
   if(!FindObject(NOBH))
   {
    var sign = CreateObject(SGNP, 2920, 440, -1);
    sign->SetPhase(2);
    sign->SetMode(1);
   }

   //Objekte entfernen
   RemoveObject(FindObject2(Find_ID(VGMN),Find_InRect(2810, 520, 50, 100)));
   RemoveObject(aArtillery[1]);
   RemoveAll(SPVM);

   //Blackhawk
   SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,2895,410,-1),100*21);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,770,800,-1),10*10);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,1265,800,-1),10*10);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1485,800,-1),10*10);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2605,800,-1),10*10);
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,580,490,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(700,340);
   aFlag[0] -> AddSpawnPoint(560,680);
   aFlag[0] -> AddSpawnPoint(600,340);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,1405,740,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(1140,540);
   aFlag[1] -> AddSpawnPoint(1080,740);
   aFlag[1] -> AddSpawnPoint(1640,740);
   if(aTeams[1] == true)
   {
    aFlag[1]->Set("$Flag2$",100,2);
    aFlag[1]->Capture(1,1);
   }
   else
   {
    aFlag[1]->Set("$Flag2$",0,2);
   }

   aFlag[2] = CreateObject(OFPL,1950,580,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(2065,460);
   aFlag[2] -> AddSpawnPoint(1905,440);
   aFlag[2] -> AddSpawnPoint(1940,750);
   aFlag[2] -> AddSpawnPoint(2070,750);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,2855,440,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(3115,430);
   aFlag[3] -> AddSpawnPoint(3225,590);
   aFlag[3] -> AddSpawnPoint(2995,670);
   if(aTeams[2] == true)
   {
    aFlag[3]->Set("$Flag4$",100,2);
    aFlag[3]->Capture(2,1);
   }
   else
   {
    aFlag[3]->Set("$Flag4$",0,2);
   }

   //Grenzen setzen
   CreateObject(BRDR, 200, 0, -1)->Set(0);
   CreateObject(BRDR, 3245, 0, -1)->Set(1);

   //Hinweisschild
   if(!FindObject(NOBH))
   {
    var sign = CreateObject(SGNP, 2920, 440, -1);
    sign->SetPhase(2);
    sign->SetMode(1);
   }

   //Blackhawk
   SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,2895,410,-1),100*21);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,770,800,-1),10*10);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,1265,800,-1),10*10);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1485,800,-1),10*10);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2605,800,-1),10*10);
  }
}

/* Assault Zerstörung */

public func OnAssaultTargetDestruction(object pTarget, int iTeam, int iIndex, bool fConnectedDestroyed)
{
  //Ziel 1 und 2
  if (!iIndex || iIndex == 1)
  {
   if(fConnectedDestroyed)
   {
    //Grenze neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 510, 0, -1)->Set(0,1);
    CreateObject(BRDR, 2430, 0, -1)->Set(1,1);

    //Lampen deaktivieren
    aLamp[00]->EMPShock();
    aLamp[01]->EMPShock();
    aLamp[02]->EMPShock();
    aLamp[03]->EMPShock();
   }
  }

  //Ziel 3
  if (iIndex == 2)
  {
   //Grenzen neu setzen
   RemoveAll(BRDR);
   CreateObject(BRDR, 1250, 0, -1)->Set(0,1);

   //Artillerie entfernen
   aArtillery[0]->DecoExplode(45);
  }

  //Ziel 4 und 5
  if (!iIndex || iIndex == 4)
  {
   if(fConnectedDestroyed)
   {
    //Rauch
    CreateParticle("GunSmoke",2940,490,0,-10,100,1);
    CreateParticle("GunSmoke",2810,440,0,-10,250,1);
    CreateParticle("GunSmoke",2980,440,0,-10,350,1);

    //Lampen deaktivieren
    aLamp[04]->EMPShock();
    aLamp[05]->EMPShock();
   }
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
    var rand = Random(2);
    if(!rand)
     { iX = 1340; iY = 740; }
    if(!--rand)
     { iX = 1425; iY = 630; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 2560; iY = 660; }
    if(!--rand)
     { iX = 2650; iY = 690; }
   }
   return(1);
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {if(FindObject(GASS)->GetRespawnPoint(iX, iY, iTeam)) return 1;}

  //Startsicht
  iX = 410; iY = 620;
}