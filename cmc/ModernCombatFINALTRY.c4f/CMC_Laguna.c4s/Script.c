/*-- Laguna --*/

#strict 2
#include CSTD

static aFlag,aArtillery;

func RecommendedGoals()			{return [GOCC, GASS];}	//Spielzielempfehlung
public func AssaultDefenderTeam()	{return 2;}		//Verteidigerteam bei Assault


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(0,50,15);
  //Hintergrundfärbung
  SetSkyAdjust(RGBa(255,255,255,220));
  //Flaggen
  aFlag = [];
  //Artillerie
  aArtillery = [];
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

  //Rampen
  DrawMaterialQuad("Wall-Concrete3",771,500,741,490,741,500,756,500,true);
  DrawMaterialQuad("Wall-Concrete3",801,510,771,500,771,510,786,510,true);

  DrawMaterialQuad("Earth-ROUGH", 2730, 758, 2780, 758, 2730, 760, 2780, 760, 1);

  DrawMaterialQuad("Wall-Metal2",2910,570,2940,560,2940,570,2925,570,true);
  DrawMaterialQuad("Wall-Metal2",2940,560,2970,550,2970,560,2955,560,true);
  DrawMaterialQuad("Wall-Concrete3",2970,550,3000,540,3000,550,2985,550,true);

  //Leitern
  CreateObject(LADR, 540, 690, -1)->Set(49);
  CreateObject(LADR, 645, 690, -1)->Set(49);
  CreateObject(LADR, 1370, 710, -1)->Set(9, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
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

  //Auto
  CreateObject(CCAR, 960, 510, -1);

  //Stahlbrücken
  CreateObject(_HBR, 875, 522, -1)->SwitchMode();
  CreateObject(_HBR, 965, 522, -1)->SwitchMode();
  CreateObject(_HBR, 1784, 602, -1);
  CreateObject(_HBR, 2015, 592, -1)->SwitchMode();
  CreateObject(_HBR, 2257, 562, -1);

  //Haie
  CreateObject(SHRK, 230, 850, -1)->AutoRespawn();
  CreateObject(SHRK, 330, 850, -1)->AutoRespawn();
  CreateObject(SHRK, 630, 850, -1)->AutoRespawn();
  CreateObject(SHRK, 730, 850, -1)->AutoRespawn();
  CreateObject(SHRK, 1030, 850, -1)->AutoRespawn();

  //Glasscheiben
  CreateObject(PANE, 512, 440, -1);
  CreateObject(PANE, 512, 640, -1);
  CreateObject(PANE, 738, 440, -1);
  CreateObject(PANE, 738, 640, -1);
  CreateObject(PANE, 2792, 490, -1);
  CreateObject(PANE, 2998, 490, -1);

  //Kisten
  CreateObject(WCR2, 620, 490, -1)->AutoRespawn();
  CreateObject(WCR2, 1970, 760, -1)->AutoRespawn();
  CreateObject(WCR2, 2060, 580, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 530, 240, -1);
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
  CreateObject(BLGH, 560, 440, -1);
  CreateObject(BLGH, 560, 580, -1);
  CreateObject(BLGH, 690, 440, -1);
  CreateObject(BLGH, 690, 580, -1);
  CreateObject(BLGH, 2850, 520, -1);
  CreateObject(BLGH, 2940, 520, -1);

  //Wasseroberfläche ebnen
  DrawMaterialQuad("Water",510,790,740,790,740,795,510,795,1);
  DrawMaterialQuad("Water",1940,790,2100,790,2100,795,1940,795,1);

  //Tunnel ebnen
  DrawMaterialQuad("Tunnel-SMOOTH",1300,772,1420,772,1420,790,1300,790,1);

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
  var crate = CreateObject(AMCT, 600, 690, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 1940, 490, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 2960, 440, -1);
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

  //Schild
  CreateObject(WLSN, 590, 540, -1)->Set("Heights");

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
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Blue Ressort.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Grenade.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Toward the Flag.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Blue Ressort.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

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

   //Geschützstellungen
   CreateObject(GNET, 730, 440, -1)->Set(0,90,1);
   CreateObject(GNET, 2800, 490, -1)->Set(0,-90,1);

   //MAV-Station
   CreateObject(MVSN, 595, 290, -1)->Set(595,240,1);

   //Helikopter und Hinweisschild
   if(!FindObject(NOHC))
   {
    SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,2895,410,-1),100*21);

    var sign = CreateObject(SGNP, 2805, 440, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
   }

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,770,789,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2070,751,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2605,789,-1),50*21);

   //Objekte entfernen
   RemoveObject(FindObject2(Find_ID(PANE),Find_InRect(735, 410, 5, 30)));
   RemoveObject(FindObject2(Find_ID(PANE),Find_InRect(2790, 460, 5, 30)));
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {
   //Zielobjekte
   AddAssaultTarget(RADR, 1450, 640, 30*30, 2, "$Target1$", 0, [[[1735, 630], [1820, 700], [1830, 590]], [[670, 350], [670, 490], [570, 490]]]);
   AddAssaultTarget(CMSN, 1410, 740, 30*30, 2, "$Target2$", 1, [[[1735, 630], [1820, 700], [1830, 590]], [[670, 350], [670, 490], [570, 490]]]);
   AddAssaultTarget(CCP2, 2050, 760, 0, 2, "$Target3$", 2, [[[2560, 670], [2555, 600], [2655, 705]], [[1185, 730], [1215, 560], [1310, 760]]]);
   AddAssaultTarget(GSTA, 2815, 750, 30*30, 2, "$Target4$", 3, [[[3080, 430], [3220, 600], [3130, 650]], [[1910, 580], [1855, 730], [1905, 450]]]);
   AddAssaultTarget(LBPC, 2745, 570, 30*30, 2, "$Target5$", 4, [[[3080, 430], [3220, 600], [3130, 650]], [[1910, 580], [1855, 730], [1905, 450]]]);

   //Ziele verbinden
   ConnectAssaultTargets([0, 1]);
   ConnectAssaultTargets([3, 4]);

   //Grenzen setzen
   CreateObject(BRDR, 230, 0, -1)->Set(0,1);
   CreateObject(BRDR, 1870, 0, -1)->Set(1,1);

   //Objekte entfernen
   RemoveObject(FindObject2(Find_ID(VGMN),Find_InRect(2810, 520, 50, 100)));
   RemoveObject(FindObject2(Find_ID(LADR),Find_InRect(2820, 680, 15, 30)));
   RemoveObject(aArtillery[1]);
   RemoveAll(SPVM);

   //Metallkisten
   CreateObject(MWCR, 2280, 550, -1);
   CreateObject(MWCR, 2350, 550, -1);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,770,789,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1485,789,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2605,789,-1),50*21);
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 2015,760, -1);
   flag->~Set("$Flag3$");

   //Grenzen setzen
   CreateObject(BRDR, 1300, 0, -1)->Set(0);
   CreateObject(BRDR, 2770, 0, -1)->Set(1);

   //Teamgrenzen
   CreateObject(BRDR, 1630, 0, -1)->Set(0,1,0,1,1);
   CreateObject(BRDR, 2340, 0, -1)->Set(1,1,0,1,2);

   //Objekte entfernen
   RemoveObject(aArtillery[0]);
   RemoveObject(aArtillery[1]);

   //Hinweisschilder
   CreateObject(SGNP, 1255, 590, -1);
   CreateObject(SGNP, 2745, 570, -1);

   //Stahlbrücken
   CreateObject(_HBR, 2228, 770, -1);
   CreateObject(_HBR, 2300, 770, -1);

   //Versorgungskisten (APW)
   var crate = CreateObject(AMCT, 1855, 490, -1);
   crate->Set(ATWN);
   var crate = CreateObject(AMCT, 2130, 470, -1);
   crate->Set(ATWN);

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Kugeln
    PlaceSpawnpoint(ABOX, 2070, 755);

    //Raketen
    PlaceSpawnpoint(MBOX, 1930, 650);
   }

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1485,789,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2605,789,-1),50*21);
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   //Grenzen setzen
   CreateObject(BRDR, 1300, 0, -1)->Set(0);
   CreateObject(BRDR, 2770, 0, -1)->Set(1);

   //Objekte entfernen
   RemoveObject(aArtillery[0]);
   RemoveObject(aArtillery[1]);

   //Hinweisschilder
   CreateObject(SGNP, 1255, 590, -1);
   CreateObject(SGNP, 2745, 570, -1);

   //Stahlbrücken
   CreateObject(_HBR, 2228, 770, -1);
   CreateObject(_HBR, 2300, 770, -1);

   //Versorgungskisten (APW)
   var crate = CreateObject(AMCT, 1855, 490, -1);
   crate->Set(ATWN);
   var crate = CreateObject(AMCT, 2130, 470, -1);
   crate->Set(ATWN);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1485,789,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2605,789,-1),50*21);
  }
}

/* Assault Zerstörung */

public func OnAssaultTargetDestruction(object pTarget, int iTeam, int iIndex, bool fConnectedDestroyed)
{
  //Ziel 1 und 2
  if(!iIndex || iIndex == 1)
  {
   if(fConnectedDestroyed)
   {
    //Grenze neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 510, 0, -1)->Set(0,1);
    CreateObject(BRDR, 2670, 0, -1)->Set(1,1);

    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),1620,0,1,1);
   }
  }

  //Ziel 3
  if(iIndex == 2)
  {
   //Grenzen neu setzen
   RemoveAll(BRDR);
   CreateObject(BRDR, 1250, 0, -1)->Set(0,1);

   //Teamgrenze setzen
   FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),2290,0,1,1);

   //Artillerie entfernen
   aArtillery[0]->DecoExplode(45);
  }

  //Ziel 4 und 5
  if(iIndex == 3 || iIndex == 4)
  {
   if(fConnectedDestroyed)
   {
    //Rauch
    CreateParticle("GunSmoke",2940,490,0,-10,100,1);
    CreateParticle("GunSmoke",2810,440,0,-10,250,1);
    CreateParticle("GunSmoke",2980,440,0,-10,350,1);

    //Lampen ausschalten
    for(var obj in FindObjects(Find_Or(Find_ID(BLGH), Find_ID(ETLT)), Find_InRect(2820,490,160,60)))
    {
     obj->EMPShock();
     obj->TurnOff();
    }
   }
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 410; iY = 620;
   return 1;
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {if(FindObject(GASS)->GetRespawnPoint(iX, iY, iTeam)) return 1;}

  //HTF/LMS/DM-Spielziel
  if(FindObject(GHTF) || FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    return [[1340, 740], [1425, 630]];
   }
   if(iTeam == 2)
   {
    return [[2560, 660], [2650, 690]];
   }
   return 1;
  }
}