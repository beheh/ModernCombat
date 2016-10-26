/*-- Atlantic Crisis --*/

#strict 2
#include CSTD

static aFlag, aLights, iLightsCounter;

func RecommendedGoals()	{return [GOCC,GHTF];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(1,20,15);
  //Bildschirmfärbung
  SetGamma(RGB(2,3,10),RGB(131,135,158),RGB(252,253,255));
  //Flaggen
  aFlag = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  //Neustart-Button setzen
  SetNextMission("ModernCombat.c4f\\CMC_AtlanticCrisis.c4s", "$RepeatButton$", "$RepeatButtonDesc$");
  //Lichter ausschalten
  LightsOff();
  return 1;
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Rampen
  DrawMaterialQuad("Wall-PlateBlue",1391,510,1361,500,1361,510,1376,510,true);
  DrawMaterialQuad("Wall-Plate",1421,520,1391,510,1391,520,1406,520,true);
  DrawMaterialQuad("Wall-PlateBlue",1451,530,1421,520,1421,530,1436,530,true);
  DrawMaterialQuad("Wall-Plate",1661,540,1631,530,1631,540,1646,540,true);

  DrawMaterialQuad("Wall-Unknown1",1300,640,1390,610,1390,640,1300,640,true);
  DrawMaterialQuad("BackWall-Stripes",1390,610,1420,600,1420,610,1390,610,true);
  DrawMaterialQuad("Wall-Stripes",1420,600,1420,585,1435,585,1430,600,true);

  DrawMaterialQuad("Wall-Unknown1",1631,619,1660,619,1660,620,1631,620,1);
  DrawMaterialQuad("Wall-Unknown1",1691,630,1661,620,1661,630,1676,630,true);
  DrawMaterialQuad("Wall-Unknown1",1721,640,1691,630,1691,640,1706,640,true);

  DrawMaterialQuad("Wall-Stripes",2280,520,2290,510,2290,520,2280,520,true);
  DrawMaterialQuad("Wall-Stripes",2380,510,2390,520,2380,520,2380,510,true);

  DrawMaterialQuad("Wall-Stripes",3620,520,3630,510,3630,520,3620,520,true);
  DrawMaterialQuad("Wall-Stripes",3720,510,3730,520,3720,520,3720,510,true);

  DrawMaterialQuad("Wall-Plate",4350,540,4380,530,4380,540,4365,540,true);
  DrawMaterialQuad("Wall-PlateRed",4560,530,4590,520,4590,530,4575,530,true);
  DrawMaterialQuad("Wall-Plate",4590,520,4620,510,4620,520,4605,520,true);
  DrawMaterialQuad("Wall-PlateRed",4620,510,4650,500,4650,510,4635,510,true);

  DrawMaterialQuad("Wall-Unknown1",4290,640,4320,630,4320,640,4305,640,true);
  DrawMaterialQuad("Wall-Unknown1",4320,630,4350,620,4350,630,4335,630,true);
  DrawMaterialQuad("Wall-Unknown1",4351,619,4379,619,4379,620,4351,620,1);

  DrawMaterialQuad("Wall-Stripes",4575,585,4580,600,4590,600,4590,585,true);
  DrawMaterialQuad("BackWall-Stripes",4590,600,4620,610,4590,610,4590,600,true);
  DrawMaterialQuad("Wall-Unknown1",4620,610,4710,640,4620,640,4620,610,true);

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

  //Bodenluken
  CreateObject(HA4K, 1410, 613, -1);
  CreateObject(HA4K, 1550, 533, -1);

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
  CreateObject(HA4K, 4600, 613, -1);

  //Gerüste
  CreateObject(SFFG, 2150, 470, -1)->Set(5);
  CreateObject(SFFG, 2150, 520, -1)->Set(4);

  CreateObject(SFFG, 3860, 470, -1)->Set(5);
  CreateObject(SFFG, 3860, 520, -1)->Set(4);

  //Schutztüren
  CreateObject(GDDR, 2795, 530, -1);
  CreateObject(GDDR, 2875, 460, -1);
  CreateObject(GDDR, 3105, 460, -1);
  CreateObject(GDDR, 3185, 530, -1);

  //Kisten
  CreateObject(WCR2, 2910, 460, -1)->AutoRespawn();
  CreateObject(WCR2, 3070, 460, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 2940, 530, -1)->AutoRespawn();
  CreateObject(MWCR, 2960, 530, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 2120, 470, -1)->AutoRespawn();
  CreateObject(BECR, 3890, 470, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 1290, 500, -1)->AutoRespawn();
  CreateObject(PBRL, 1580, 530, -1)->AutoRespawn();
  CreateObject(PBRL, 1580, 600, -1)->AutoRespawn();

  CreateObject(PBRL, 2160, 520, -1)->AutoRespawn();

  CreateObject(PBRL, 2940, 460, -1)->AutoRespawn();
  CreateObject(PBRL, 3020, 380, -1)->AutoRespawn();

  CreateObject(PBRL, 3850, 520, -1)->AutoRespawn();

  CreateObject(PBRL, 4435, 530, -1)->AutoRespawn();
  CreateObject(PBRL, 4435, 600, -1)->AutoRespawn();
  CreateObject(PBRL, 4720, 500, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 2430, 520, -1)->AutoRespawn();
  CreateObject(HBRL, 3580, 520, -1)->AutoRespawn();

  //Giftfässer
  CreateObject(TBRL, 2810, 530, -1)->AutoRespawn();
  CreateObject(TBRL, 2830, 530, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 3110, 410, -1)->AutoRespawn();
  CreateObject(XBRL, 3140, 530, -1)->AutoRespawn();

  //Gasflasche
  CreateObject(GSBL, 3160, 530, -1)->AutoRespawn();

  //Explosivkiste
  CreateObject(XWCR, 2815, 410, -1)->AutoRespawn();

  //Stahlbrücken
  CreateObject(_HBR, 2064, 532, -1);
  CreateObject(_HBR, 2335, 522, -1)->SwitchMode();
  CreateObject(_HBR, 2487, 532, -1);
  CreateObject(_HBR, 2734, 472, -1);
  CreateObject(_HBR, 3247, 472, -1);
  CreateObject(_HBR, 3524, 532, -1);
  CreateObject(_HBR, 3675, 522, -1)->SwitchMode();
  CreateObject(_HBR, 3947, 532, -1);

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

  //Sandsackbarrieren
  CreateObject(SBBA, 1705, 540, -1)->Right();
  CreateObject(SBBA, 2115, 520, -1);
  CreateObject(SBBA, 2306, 510, -1)->Right();
  CreateObject(SBBA, 3704, 510, -1);
  CreateObject(SBBA, 3895, 520, -1)->Right();
  CreateObject(SBBA, 4305, 540, -1);

  //Explosivtank
  CreateObject(XTNK, 3140, 410, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 1600, 530, -1)->AutoRespawn();
  CreateObject(PTNK, 2430, 520, -1)->AutoRespawn();
  CreateObject(PTNK, 3580, 520, -1)->AutoRespawn();
  CreateObject(PTNK, 4410, 530, -1)->AutoRespawn();

  //Seegras und Muscheln entfernen
  for(var obj in FindObjects(Find_Or(Find_ID(SWD1), Find_ID(SWD2), Find_ID(SHEL)), Find_InRect(1280, 370, 3450, 280)))
    RemoveObject(obj);

  //Wasseroberfläche ebnen
  DrawMaterialQuad("Water",1631,610,4379,610,4379,615,1631,615,1);

  //Grenzen
  CreateObject(BRDR, 1170, 0, -1)->Set(0);
  CreateObject(BRDR, 4835, 0, -1)->Set(1);

  //Soundkulisse

  //Möven
  CreateObject(SE4K, 1080, 550, -1)->Set("Seagulls*.ogg",1000,500, 10,50);
  CreateObject(SE4K, 2300, 380, -1)->Set("Seagulls*.ogg",1000,500, 10,50);
  CreateObject(SE4K, 3810, 380, -1)->Set("Seagulls*.ogg",1000,500, 10,50);
  CreateObject(SE4K, 4930, 550, -1)->Set("Seagulls*.ogg",1000,500, 10,50);

  //Wellen
  CreateObject(SE4K, 500, 650, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 1000, 650, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 1920, 650, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 2500, 650, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 3510, 650, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 4090, 650, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 5410, 650, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 5510, 650, -1)->Set("Wave*.ogg",100,50, 75);

  //Lautsprecher
  CreateObject(SE4K, 2990, 330, -1)->Set("Announce*.ogg",3000,500, 75);

  //Wind
  CreateObject(SE4K, 1540, 300, -1)->Set("WindSound*.ogg",300,100, 50);
  CreateObject(SE4K, 2990, 280, -1)->Set("WindSound*.ogg",300,100, 50);
  CreateObject(SE4K, 4460, 300, -1)->Set("WindSound*.ogg",300,100, 50);

  //Innenbereich
  CreateObject(SE4K, 1500, 550, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 2990, 500, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 4510, 550, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 1500, 550, -1)->Set("InteriorMetal*.ogg",1000,300, 75);
  CreateObject(SE4K, 2990, 500, -1)->Set("InteriorMetal*.ogg",1000,300, 75);
  CreateObject(SE4K, 4510, 550, -1)->Set("InteriorMetal*.ogg",1000,300, 75);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 1680, 400, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 4330, 400, -1);
  crate->Set(ABOX);

  //Versorgungskiste (Gewehrgranaten)
  crate = CreateObject(AMCT, 3020, 460, -1);
  crate->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MBOX, 2150, 515);
  PlaceSpawnpoint(MBOX, 3860, 515);

  //Geschützstellungen
  CreateObject(GNET, 1465, 530, -1)->Set(LCAC);
  CreateObject(GNET, 4545, 530, -1)->Set(LCAC);

  //Artilleriebatterien
  CreateObject(ATBY,2231,520,-1)->SetRotation(20);
  CreateObject(ATBY,3775,520,-1)->SetRotation(-20);

  //Patrouillenboote
  SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1480,600,-1),50*21);
  SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1705,609,-1),50*21);
  SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2990,571,-1),50*21);
  SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4310,609,-1),50*21);
  SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4530,600,-1),50*21);

  //Automaten
  var store = CreateObject(WPVM,1500, 530,-1);
  store->AddWare(C4PA,10);
  store->AddWare(FAPK,10);
  store->AddWare(CDBT,10);
  store->AddWare(BWTH,10);
  store->AddWare(RSHL,10);
  store->AddWare(ATWN,20);
  store->AddWare(FGRN,20);
  store->AddWare(FRAG,20);
  store->AddWare(PGRN,20);
  store->AddWare(SGRN,20);
  store->AddWare(STUN,20);
  store->AddWare(SRBL,20);
  store = CreateObject(WPVM,3025, 530,-1);
  store->AddWare(C4PA,10);
  store->AddWare(FAPK,10);
  store->AddWare(CDBT,10);
  store->AddWare(BWTH,10);
  store->AddWare(RSHL,10);
  store->AddWare(ATWN,20);
  store->AddWare(FGRN,20);
  store->AddWare(FRAG,20);
  store->AddWare(PGRN,20);
  store->AddWare(SGRN,20);
  store->AddWare(STUN,20);
  store->AddWare(SRBL,20);
  store = CreateObject(WPVM,4500, 530,-1);
  store->AddWare(C4PA,10);
  store->AddWare(FAPK,10);
  store->AddWare(CDBT,10);
  store->AddWare(BWTH,10);
  store->AddWare(RSHL,10);
  store->AddWare(ATWN,20);
  store->AddWare(FGRN,20);
  store->AddWare(FRAG,20);
  store->AddWare(PGRN,20);
  store->AddWare(SGRN,20);
  store->AddWare(STUN,20);
  store->AddWare(SRBL,20);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Orientierungslichter
  CreateObject(OLGH, 1655, 400, -1)->Set(7, 10, 1, 1, 10);
  CreateObject(OLGH, 2410, 420, -1)->Set(35, 5, 1, 1, 20);
  CreateObject(OLGH, 2990, 380, -1)->Set(4, 10, 1, 1, 20);
  CreateObject(OLGH, 3600, 420, -1)->Set(35, 5, 1, 1, 20);
  CreateObject(OLGH, 4355, 400, -1)->Set(7, 10, 1, 1, 10);

  //Straßenlichter
  CreateObject(SLGH, 1393, 511, -1);
  CreateObject(SLGH, 2780, 460, -1);
  CreateObject(SLGH, 3200, 460, -1);
  CreateObject(SLGH, 4617, 511, -1);

  //Automaten
  CreateObject(VGMN, 2840, 530, -1);
  CreateObject(VGMN, 3140, 530, -1)->Set(3);

  //Büsche
  CreateObject(BSH2, 2770, 570, -1);
  CreateObject(BSH2, 3180, 425, -1);

  //Bojen
  CreateObject(BUOY, 1180, 633, -1);
  CreateObject(BUOY, 2370, 633, -1);

  CreateObject(BUOY, 3840, 633, -1);
  CreateObject(BUOY, 4830, 633, -1);

  //Satellitenschüssel
  CreateObject(SADH, 2840, 410, -1);

  //Radare
  CreateObject(RADR, 1610, 400, -1);
  CreateObject(RADR, 4400, 400, -1);

  //Flaschen
  CreateObject(BOTL, 2940, 512, -1);
  CreateObject(BOTL, 2945, 512, -1);

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

  //Feuerlöscher
  CreateObject(FIE2, 1530, 595, -1);
  CreateObject(FIE2, 2970, 455, -1);
  CreateObject(FIE2, 4480, 595, -1);

  //Geländer
  CreateObject(RAI1, 1280, 500, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1450, 530, -1)->SetRail([1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1500, 600, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 2125, 520, -1)->SetRail([1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 2880, 460, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 3735, 520, -1)->SetRail([1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 4380, 530, -1)->SetRail([1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 4410, 600, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 4650, 500, -1)->SetRail([1,1,1,1]);

  //Schilder
  CreateObject(ESGN, 1515, 590, -1);
  CreateObject(ESGN, 2970, 440, -1);
  CreateObject(ESGN, 4495, 590, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 2890, 530, -1);
  CreateObject(ENGT, 3090, 530, -1);

  //Bildschirme
  CreateObject(SCR1, 1665, 595, -1);
  CreateObject(SCA1, 2825, 490, -1)->SetAction("Grenade");
  CreateObject(SCA1, 3155, 490, -1)->SetAction("Medic");
  CreateObject(SCR1, 4330, 595, -1);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  var back = CreateObject(BD03,700,800,-1);
  if(FindObject(STRM))
    back->SetClrModulation(RGBa(50,50,50,5));
  back = CreateObject(BD03,1700,700,-1);
  if(FindObject(STRM))
    back->SetClrModulation(RGBa(50,50,50,5));
  back = CreateObject(BD03,3000,800,-1);
  if(FindObject(STRM))
    back->SetClrModulation(RGBa(50,50,50,5));
}

/* Sturmerstellung */

func FormStorm()
{
  //Sturm erstellen
  CreateObject(STRM);

  //Dunkelheit erhöhen
  if(GetDarkness() < 3)
    FadeDarkness(3,60);

  //Hintergründe verdunkeln
  var back = FindObjects(Find_Func("IsDeco"));
  for (var pObj in back)
   pObj->SetClrModulation(RGBa(50,50,50,5));

  //Lichter einschalten
  LightsOn();
}

/* Lichtverwaltung */

func LightsOff()
{
  aLights = [];
  for(var light in FindObjects(Find_Func("IsLamp"), Sort_Func("Lamp_SortX")))
    light->~TurnOff();
}

func LightsOn()
{
  aLights = FindObjects(Find_Func("IsLamp"), Sort_Func("Lamp_SortX"));
  iLightsCounter = 0;
  if(aLights[iLightsCounter])
    Schedule("LightsOnHelper();", 50);
  else
    return -1;
}

global func Lamp_SortX()
{
  return GetX();
}

func LightsOnHelper()
{
  while(!aLights[iLightsCounter])
    if(iLightsCounter >= GetLength(aLights) - 1)
      return;
    else
      iLightsCounter++;

  aLights[iLightsCounter]->~TurnOn();
  aLights[iLightsCounter] = 0;

  Schedule("LightsOnHelper();", 50);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Offensive.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Zeitverzögertes Gewitter
  Schedule("GameCall(\"FormStorm\")", RandomX(4000,8000));

  //Helikopter und Hinweisschilder
  if(!FindObject(NOHC))
  {
    SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,1655,370,-1),100*21);
    SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,4355,370,-1),100*21);

    var sign = CreateObject(SGNP, 1600, 400, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
    sign = CreateObject(SGNP, 4410, 400, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
  }

  //OCC-Spielziel
  if(FindObject(GOCC))
  {
    //Flaggenposten
    aFlag[0] = CreateObject(OFPL,1320,500,NO_OWNER);
    aFlag[0] -> AddSpawnPoint(1500, 590);
    aFlag[0] -> AddSpawnPoint(1600, 390);
    aFlag[0] -> AddSpawnPoint(1670, 530);
    if(aTeams[1] == true)
    {
      aFlag[0]->Set("$Flag1$",0,4);
      aFlag[0]->Capture(1,1);
    }
    else
      aFlag[0]->Set("$Flag1$");

    aFlag[1] = CreateObject(OFPL,2335,600,NO_OWNER);
    aFlag[1] -> AddSpawnPoint(2150,460);
    aFlag[1] -> AddSpawnPoint(2180,510);
    aFlag[1] -> AddSpawnPoint(2260,510);
    aFlag[1]->Set("$Flag2$",80);

    aFlag[2] = CreateObject(OFPL,2990,380,NO_OWNER);
    aFlag[2] -> AddSpawnPoint(2890,520);
    aFlag[2] -> AddSpawnPoint(3040,450);
    aFlag[2] -> AddSpawnPoint(3090,520);
    aFlag[2]->Set("$Flag3$");

    aFlag[3] = CreateObject(OFPL,3675,600,NO_OWNER);
    aFlag[3] -> AddSpawnPoint(3750,510);
    aFlag[3] -> AddSpawnPoint(3830,510);
    aFlag[3] -> AddSpawnPoint(3860,460);
    aFlag[3]->Set("$Flag4$",80);

    aFlag[4] = CreateObject(OFPL,4685,500,NO_OWNER);
    aFlag[4] -> AddSpawnPoint(4340, 530);
    aFlag[4] -> AddSpawnPoint(4410, 390);
    aFlag[4] -> AddSpawnPoint(4510, 590);
    if(aTeams[2] == true)
    {
      aFlag[4]->Set("$Flag5$",0,4);
      aFlag[4]->Capture(2,1);
    }
    else
      aFlag[4]->Set("$Flag5$");

    //MAV-Stationen
    CreateObject(MVSN, 1350, 500, -1)->Set(1520,530,1);
    CreateObject(MVSN, 4660, 500, -1)->Set(4490,530,1);
  }

  //BAS-Spielziel
  if(FindObject(GBAS))
  {
    //Strukturen
    AddAssaultTarget(RADR, 1919, 600, 350, 1, "$Target1$", 0, [[1360,500], [1420,520], [1600,400]]);
    AddAssaultTarget(CMSN, 2470, 600, 350, 1, "$Target2$", 1, [[2150,470], [2180,520], [2270,520]]);
    AddAssaultTarget(CMSN, 3540, 600, 350, 2, "$Target2$", 2, [[3740,520], [3830,520], [3860,470]]);
    AddAssaultTarget(RADR, 4089, 600, 350, 2, "$Target1$", 3, [[4410,400], [4590,520], [4650,500]]);

    //Sprengsatz-Spawn
    SetupBombSpawnpoint([[2990, 380],[2990,530]]);

    //Boden
    DrawMaterialQuad("BackWall-Stripes", 1900,600, 1940,600, 1940,610, 1900,610);
    DrawMaterialQuad("BackWall-Metal2", 2420,600, 2490,600, 2490,610, 2420,610);
    DrawMaterialQuad("BackWall-Metal2", 3520,600, 3580,600, 3580,610, 3520,610);
    DrawMaterialQuad("BackWall-Stripes", 4070,600, 4110,600, 4110,610, 4070,610);
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Flaggenposten
    var flag = CreateObject(OFPL, 2990, 380, -1);
    flag->~Set("$Flag3$");

    //Teamgrenzen
    CreateObject(BRDR, 2000, 0, -1)->Set(0,1,0,1,1);
    CreateObject(BRDR, 4010, 0, -1)->Set(1,1,0,1,2);

    //MAV-Stationen
    CreateObject(MVSN, 1350, 500, -1)->Set(1520,530,1);
    CreateObject(MVSN, 4660, 500, -1)->Set(4490,530,1);

    //Selbstschussanlagen
    var selfdef = CreateObject(SEGU, 1690, 540, -1);
    selfdef->Set(0,1,1,180,0,2100);
    selfdef->SetTeam(1);
    selfdef = CreateObject(SEGU, 4320, 540, -1);
    selfdef->Set(0,1,1,180,3910);
    selfdef->SetTeam(2);

    //Zusätzliche Munition
    if(!FindObject(NOAM))
    {
      //Versorgungskiste (Kugeln)
      var crate = CreateObject(AMCT, 2965, 460, -1);
      crate->Set(ABOX);
    }
  }

  //MR-Spielziel
  if(FindObject(GMNR))
  {
    //Geldsäcke
    AddMoneySpawn(2360, 510, [20]);
    AddMoneySpawn(2785, 450, [20]);
    AddMoneySpawn(3195, 450, [20]);
    AddMoneySpawn(3650, 510, [20]);

    //Teamgrenzen
    CreateObject(BRDR, 2000, 0, -1)->Set(0,1,0,1,1);
    CreateObject(BRDR, 4010, 0, -1)->Set(1,1,0,1,2);

    //MAV-Stationen
    CreateObject(MVSN, 1350, 500, -1)->Set(1520,530,1);
    CreateObject(MVSN, 4660, 500, -1)->Set(4490,530,1);

    //Selbstschussanlagen
    var selfdef = CreateObject(SEGU, 1690, 540, -1);
    selfdef->Set(0,1,1,180,0,2100);
    selfdef->SetTeam(1);
    selfdef = CreateObject(SEGU, 4320, 540, -1);
    selfdef->Set(0,1,1,180,3910);
    selfdef->SetTeam(2);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
    //Flaggen
    if(aTeams[1] == true)
    {CreateFlag(1,1520,600,GetTeamColor(1));}
    if(aTeams[2] == true)
    {CreateFlag(2,4490,600,GetTeamColor(2));}

    //MAV-Station
    CreateObject(MVSN, 2990, 380, -1)->Set(2960,380,1);
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
    //MAV-Stationen
    CreateObject(MVSN, 1350, 500, -1)->Set(1520,530,1);
    CreateObject(MVSN, 4660, 500, -1)->Set(4490,530,1);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 2990; iY = 440;
    return 1;
  }

  //HTF/MR/CTF/LMS/DM-Spielziel
  if(FindObject(GHTF) || FindObject(GMNR) || FindObject(GCTF) || FindObject(GLMS) || FindObject(GTDM))
  {
    if(iTeam == 1)
      return [[1320, 490], [1600, 390], [1620, 520]];
    if(iTeam == 2)
      return [[4390, 520], [4410, 390], [4690, 490]];
  }
}