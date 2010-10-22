/*-- Laguna --*/

#strict
#include CSTD

static aFlag,aLamp,aArtillery;


/* Initialisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg");
  Music("CMC_Firehawk.ogg");
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
  //Szenario einrichten
  CreateFurniture();
  //Equipment plazieren
  CreateEquipment();
  return(1); 
}

/* Plazierungslisten */

func CreateFurniture()
{
  Log("$CreatingFurniture$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Rampen
  DrawMaterialQuad("Wall-Concrete3",771,350,741,340,741,350,756,350,true);
  DrawMaterialQuad("Wall-Concrete3",801,360,771,350,771,360,786,360,true);

  DrawMaterialQuad("Wall-Metal2",2910,420,2940,410,2940,420,2925,420,true);
  DrawMaterialQuad("Wall-Metal2",2940,410,2970,400,2970,410,2955,410,true);
  DrawMaterialQuad("Wall-Concrete3",2970,400,3000,390,3000,400,2985,400,true);

  //Leitern
  CreateObject(LADR, 540, 540, -1)->Set(49);
  CreateObject(LADR, 645, 540, -1)->Set(49);

  CreateObject(LADR, 1370, 560, -1)->Set(9);

  CreateObject(LADR, 1930, 582, -1)->Set(18);
  CreateObject(LADR, 2015, 430, -1)->Set(14);
  CreateObject(LADR, 2100, 582, -1)->Set(18);

  CreateObject(LADR, 2830, 580, -1)->Set(5);
  CreateObject(LADR, 2895, 482, -1)->Set(23);

  //Bodenlucken
  CreateObject(HA4K, 540, 143, -1);
  CreateObject(HA4K, 540, 203, -1);
  CreateObject(HA4K, 540, 343, -1);
  CreateObject(HA4K, 540, 493, -1);

  CreateObject(HA4K, 1930, 433, -1);
  CreateObject(HA4K, 2100, 433, -1);

  //Große Bodenlucken
  CreateObject(H24K, 645, 98, -1);
  CreateObject(H24K, 645, 148, -1);
  CreateObject(H24K, 645, 208, -1);
  CreateObject(H24K, 645, 348, -1);

  CreateObject(H24K, 2895, 298, -1);
  CreateObject(H24K, 2895, 428, -1);

  //Stahlbrücken
  CreateObject(_HBR, 875, 372, -1);
  CreateObject(_HBR, 965, 372, -1);

  CreateObject(_HBR, 1785, 452, -1);
  CreateObject(_HBR, 2015, 442, -1);
  CreateObject(_HBR, 2255, 412, -1);

  //Haie
  CreateObject(SHRK, 230, 700, -1)->AutoRespawn();
  CreateObject(SHRK, 330, 700, -1)->AutoRespawn();
  CreateObject(SHRK, 630, 700, -1)->AutoRespawn();
  CreateObject(SHRK, 730, 700, -1)->AutoRespawn();
  CreateObject(SHRK, 1030, 700, -1)->AutoRespawn();

  //Geländer
  CreateObject(RAI1, 515, 140, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 515, 200, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 515, 340, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 515, 490, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 515, 540, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1]);

  CreateObject(RAI1, 1910, 430, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 2060, 430, -1)->SetRail([1,1,1]);

  CreateObject(RAI1, 2800, 290, -1)->SetRail([1,1,1,1,1,1,1,1,1,1]);

  //Glasscheiben
  CreateObject(_WIN, 512, 290, -1);
  CreateObject(_WIN, 512, 490, -1);

  CreateObject(_WIN, 738, 290, -1);
  CreateObject(_WIN, 738, 490, -1);

  CreateObject(_WIN, 2792, 340, -1);
  CreateObject(_WIN, 2998, 340, -1);

  //Satellitenschüssel
  CreateObject(RADR, 2985, 220, -1)->SetClrModulation(RGB(125,125,125));

  //Orientierungslichter
  CreateObject(OLGH, 625, 90, -1)->Set(7, 15, 1, 1, 40);
  CreateObject(OLGH, 2895, 290, -1)->Set(6, 15, 1, 1, 40);

  //Automaten
  CreateObject(CLVM, 700, 540, -1);
  CreateObject(SPVM, 2840, 420, -1);

  //Bojen
  CreateObject(BUOY, 230, 663,  -1);

  CreateObject(BUOY, 1900, 663,  -1);
  CreateObject(BUOY, 2130, 663,  -1);

  CreateObject(BUOY, 2535, 663,  -1);

  //Metallkisten
  CreateObject(MWCR, 532, 90, -1);
  CreateObject(MWCR, 2870, 420, -1);

  //Kisten
  CreateObject(WCR2, 620, 340, -1)->AutoRespawn();
  CreateObject(WCR2, 1000, 360, -1);

  CreateObject(WCR2, 1970, 610, -1)->AutoRespawn();
  CreateObject(WCR2, 1990, 610, -1)->AutoRespawn();

  CreateObject(WCR2, 2060, 430, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 561, 200, -1);
  CreateObject(XWCR, 1980, 592, -1);
  CreateObject(XWCR, 2970, 530, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 580, 200, -1)->AutoRespawn();
  CreateObject(XBRL, 680, 540, -1)->AutoRespawn();
  CreateObject(XBRL, 1975, 340, -1)->AutoRespawn();
  CreateObject(XBRL, 2815, 290, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 1020, 360, -1)->AutoRespawn();
  CreateObject(HBRL, 2010, 610, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 665, 540, -1)->AutoRespawn();
  CreateObject(GSBL, 2080, 430, -1)->AutoRespawn();
  CreateObject(GSBL, 2850, 420, -1)->AutoRespawn();
  CreateObject(GSBL, 2860, 420, -1)->AutoRespawn();

  //Explosivtanks
  CreateObject(XTNK, 590, 490, -1)->AutoRespawn();
  CreateObject(XTNK, 1950, 340, -1)->AutoRespawn();
  CreateObject(XTNK, 2840, 290, -1)->AutoRespawn();

  //Sandsackbarrieren
  CreateObject(SBBA, 830, 360, -1)->Right();

  CreateObject(SBBA, 1370, 590, -1);
  CreateObject(SBBA, 1445, 590, -1)->Right();

  CreateObject(SBBA, 1821, 340, -1);
  CreateObject(SBBA, 2160, 320, -1)->Right();

  CreateObject(SBBA, 2801, 290, -1);
  CreateObject(SBBA, 2990, 290, -1)->Right();

  //Lüftungsgitter
  CreateObject(ENGT, 605, 310, -1);
  CreateObject(ENGT, 605, 450, -1);

  //Scheinwerfer
  CreateObject(FLGH, 560, 140, -1)->SetRotation(-90);
  CreateObject(FLGH, 705, 140, -1)->SetRotation(90);

  //Dekoschleusen
  CreateObject(GAT1, 690, 495, -1);
  CreateObject(GAT1, 2940, 345, -1);

  //Warnschild
  CreateObject(EXSN, 560, 540, -1);

  //Regale
  CreateObject(FRAM, 580, 540, -1);
  CreateObject(FRAM, 600, 540, -1);
  CreateObject(FRAM, 620, 540, -1);

  //Notausgangslichter
  CreateObject(ETLT, 705, 320, -1);
  CreateObject(ETLT, 2960, 390, -1);

  //Stahltüren
  CreateObject(STDR, 515, 340, -1);
  CreateObject(STDR, 515, 540, -1);

  CreateObject(STDR, 735, 340, -1);
  CreateObject(STDR, 735, 540, -1);

  //Grenzen
  CreateObject(BRDR, 200, 0, -1)->Set(0);
  CreateObject(BRDR, 3245, 0, -1)->Set(1);

  //Lampen
  aLamp[00]=CreateObject(BLGH, 560, 290, -1);
  aLamp[01]=CreateObject(BLGH, 560, 430, -1);
  aLamp[02]=CreateObject(BLGH, 690, 290, -1);
  aLamp[03]=CreateObject(BLGH, 690, 430, -1);
  aLamp[04]=CreateObject(BLGH, 2850, 370, -1);
  aLamp[05]=CreateObject(BLGH, 2940, 370, -1);

  //Sounds

  //Wind
  CreateObject(SE4K, 900, 285, -1)->Set("WindSound*.ogg",775,250);

  //Möven
  CreateObject(SE4K, 380, 510, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 1170, 480, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 1740, 555, -1)->Set("SeaSounds*.ogg",140,35);

  //Wellen
  CreateObject(SE4K, 170, 600, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 910, 600, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1230, 600, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1780, 600, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 2350, 600, -1)->Set("Wave*.ogg",105,35);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Munitionskisten (Kugeln)
  var tmp = CreateObject (AMCT, 600, 540, -1);
  tmp->Set(ABOX);
  tmp->SetGraphics("Normal");
  var tmp = CreateObject (AMCT, 1940, 340, -1);
  tmp->Set(ABOX);
  tmp->SetGraphics("Normal");
  var tmp = CreateObject (AMCT, 2960, 290, -1);
  tmp->Set(ABOX);
  tmp->SetGraphics("Normal");

  //Granatenmunition
  PlaceSpawnpoint(GBOX, 1675, 495);
  PlaceSpawnpoint(GBOX, 2215, 545);

  //Raketen
  PlaceSpawnpoint(MBOX, 680, 135);
  PlaceSpawnpoint(MBOX, 2920, 505);

  //Artilleriebatterien
  aArtillery[0] = CreateObject(ATBY,1855,340,-1);
  aArtillery[1] = CreateObject(ATBY,2130,320,-1);

  //Motorboote
  SetupVehicleSpawn([INFL],DIR_Right,CreateObject(VSPW,770,650,-1),10*10);

  SetupVehicleSpawn([INFL],DIR_Left,CreateObject(VSPW,1265,650,-1),10*10);
  SetupVehicleSpawn([INFL],DIR_Right,CreateObject(VSPW,1485,650,-1),10*10);

  SetupVehicleSpawn([INFL],DIR_Left,CreateObject(VSPW,2605,650,-1),10*10);

  //Blackhawk
  SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,2895,260,-1),100*21);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //DM/LMS-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS))
  {
   //Grenzen
   CreateObject(BRDR, 1300, 0, -1)->Set(0);
   CreateObject(BRDR, 2770, 0, -1)->Set(1);

   //Warnschilder
   CreateObject(SGNP, 1255, 440, -1);
   CreateObject(SGNP, 2745, 420, -1);

   //Blackhawk entfernen
   RemoveAll(BKHK);
  }

  //HTF-Spielziel
  if (FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 2015,610, -1);
   flag->~Set("$Flag3$");

   //Projektilmunition
   PlaceSpawnpoint(ABOX, 2070, 605);

   //Raketen
   PlaceSpawnpoint(MBOX, 1930, 500);

   //Grenzen
   CreateObject(BRDR, 1300, 0, -1)->Set(0);
   CreateObject(BRDR, 2770, 0, -1)->Set(1);

   //Warnschilder
   CreateObject(SGNP, 1255, 440, -1);
   CreateObject(SGNP, 2745, 420, -1);

   //Blackhawk entfernen
   RemoveAll(BKHK);
  }

  //Assault-Spielziel
  if (FindObject(GASS))
  {
   //Grenzen
   CreateObject(BRDR, 230, 0, -1)->Set(0,1);
   CreateObject(BRDR, 1820, 0, -1)->Set(1,1);

   //Automat entfernen
   RemoveAll(SPVM);

   //Zielobjekte
   AddAssaultTarget(RADR, 1450, 490, 250, 2, "$Target1$", 0, [[[1730, 480], [1730, 580]], [[580, 140], [580, 140], [580, 490]]]);
   AddAssaultTarget(CMSN, 1410, 590, 250, 2, "$Target2$", 1, [[[1730, 480], [1730, 580]], [[580, 140], [580, 140], [580, 490]]]);
   AddAssaultTarget(CCP2, 2050, 610, 300, 2, "$Target3$", 2, [[[2210, 400], [2060, 320]], [[1210, 410], [1190, 580]]]);
   AddAssaultTarget(GSTA, 2815, 600, 300, 2, "$Target4$", 3, [[[3080, 280], [3220, 450], [3130, 500]], [[1910, 430], [1855, 580], [1905, 300]]]);
   AddAssaultTarget(LBPC, 2850, 420, 300, 2, "$Target5$", 4, [[[3080, 280], [3220, 450], [3130, 500]], [[1910, 430], [1855, 580], [1905, 300]]]);

   //Ziele verbinden
   ConnectAssaultTargets([0, 1]);
   ConnectAssaultTargets([3, 4]);
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggen
   aFlag[0] = CreateObject(OFPL,580,340,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(700,190);
   aFlag[0] -> AddSpawnPoint(560,530);
   aFlag[0] -> AddSpawnPoint(600,190);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,1405,590,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(1140,390);
   aFlag[1] -> AddSpawnPoint(1080,590);
   aFlag[1] -> AddSpawnPoint(1640,590);
   if(aTeams[1] == true)
   {
    aFlag[1]->Set("$Flag2$",100,2);
    aFlag[1]->Capture(1,1);
   }
   else
   {
    aFlag[1]->Set("$Flag2$",0,2);
   }

   aFlag[2] = CreateObject(OFPL,1950,430,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(2065,310);
   aFlag[2] -> AddSpawnPoint(1905,290);
   aFlag[2] -> AddSpawnPoint(1940,600);
   aFlag[2] -> AddSpawnPoint(2070,600);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,2855,290,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(3115,280);
   aFlag[3] -> AddSpawnPoint(3225,440);
   aFlag[3] -> AddSpawnPoint(2995,520);
   if(aTeams[2] == true)
   {
    aFlag[3]->Set("$Flag4$",100,2);
    aFlag[3]->Capture(2,1);
   }
   else
   {
    aFlag[3]->Set("$Flag4$",0,2);
   }
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
    CreateParticle("GunSmoke",2940,340,0,-10,100,1);
    CreateParticle("GunSmoke",2810,290,0,-10,250,1);
    CreateParticle("GunSmoke",2980,290,0,-10,350,1);

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
     { iX = 1340; iY = 590; }
    if(!--rand)
     { iX = 1425; iY = 480; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 2560; iY = 510; }
    if(!--rand)
     { iX = 2650; iY = 540; }
   }
   return(1);
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {if(FindObject(GASS)->GetRespawnPoint(iX, iY, iTeam)) return 1;}

  //Startsicht
  iX = 410; iY = 470;
}