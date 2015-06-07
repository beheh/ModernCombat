/*-- Black Thunder --*/

#strict 2
#include CSTD

static aFlag,aArtillery;

public func SpecificEquipment()  {return [[PPAR, 1]];}  //Zusatzausr¸stung: Fallschirmrucksack
func RecommendedGoals()    {return [GOCC, GHTF];}  //Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxit‰t
  SetSkyParallax(0,15,15);
  //Bildschirmf‰rbung
  SetGamma(RGB(2,3,10),RGB(131,135,158),RGB(252,253,255));
  //Flaggen
  aFlag = [];
  //Artillerie
  aArtillery = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausr¸stung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  return 1;
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Leitern
  CreateObject(LADR, 430, 930, -1)->Set(23, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 460, 1240, -1)->Set(35, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");

  CreateObject(LADR, 460, 545, -1)->Set(13);
  CreateObject(LADR, 790, 750, -1)->Set(19);
  CreateObject(LADR, 1220, 1178, -1)->Set(41, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 1250, 844, -1)->Set(27);
  CreateObject(LADR, 1374, 640, -1)->Set(6);

  CreateObject(LADR, 2020, 850, -1)->Set(19);
  CreateObject(LADR, 2040, 1170, -1)->Set(40, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 2200, 1170, -1)->Set(40, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 2220, 850, -1)->Set(19);

  CreateObject(LADR, 2866, 640, -1)->Set(6);
  CreateObject(LADR, 2990, 844, -1)->Set(27);
  CreateObject(LADR, 3020, 1178, -1)->Set(41, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3450, 750, -1)->Set(19);
  CreateObject(LADR, 3780, 545, -1)->Set(13);

  CreateObject(LADR, 3780, 1240, -1)->Set(35, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3810, 930, -1)->Set(23, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");

  //Bodenluken
  CreateObject(HA4K, 460, 503, -1);
  CreateObject(HA4K, 790, 593, -1);

  CreateObject(HA4K, 1250, 623, -1);
  CreateObject(HA4K, 1250, 673, -1);

  CreateObject(HA4K, 2990, 623, -1);
  CreateObject(HA4K, 2990, 673, -1);

  CreateObject(HA4K, 3450, 593, -1);
  CreateObject(HA4K, 3780, 503, -1);

  //Ger¸ste
  CreateObject(SFFG, 1580, 770, -1)->Set(2);
  CreateObject(SFFG, 1580, 820, -1)->Set(4);
  CreateObject(SFFG, 1660, 820, -1)->Set(4);
  CreateObject(SFFG, 1800, 820, -1)->Set(4);
  CreateObject(SFFG, 1880, 820, -1)->Set(4);

  CreateObject(SFFG, 2360, 820, -1)->Set(4);
  CreateObject(SFFG, 2440, 820, -1)->Set(4);
  CreateObject(SFFG, 2580, 820, -1)->Set(4);
  CreateObject(SFFG, 2660, 770, -1)->Set(3);
  CreateObject(SFFG, 2660, 820, -1)->Set(4);

  //Schutzt¸ren
  CreateObject(GDDR, 1125, 670, -1);
  CreateObject(GDDR, 1275, 670, -1);

  CreateObject(GDDR, 2965, 670, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 3115, 670, -1)->SetColorDw(HSL(145, 210, 100, 127));

  //Sandsackbarrieren
  CreateObject(SBBA, 361, 560, -1);
  CreateObject(SBBA, 401, 440, -1);
  CreateObject(SBBA, 440, 440, -1)->Right();
  CreateObject(SBBA, 711, 760, -1);
  CreateObject(SBBA, 780, 760, -1)->Right();

  CreateObject(SBBA, 1101, 670, -1);
  CreateObject(SBBA, 1101, 760, -1);
  CreateObject(SBBA, 1350, 850, -1)->Right();
  CreateObject(SBBA, 1391, 590, -1);
  CreateObject(SBBA, 1430, 590, -1)->Right();
  CreateObject(SBBA, 1470, 670, -1)->Right();

  CreateObject(SBBA, 1953, 850, -1);
  CreateObject(SBBA, 1961, 750, -1);
  CreateObject(SBBA, 2280, 750, -1)->Right();
  CreateObject(SBBA, 2288, 850, -1)->Right();

  CreateObject(SBBA, 2771, 670, -1);
  CreateObject(SBBA, 2811, 590, -1);
  CreateObject(SBBA, 2850, 590, -1)->Right();
  CreateObject(SBBA, 2891, 850, -1);
  CreateObject(SBBA, 3140, 670, -1)->Right();
  CreateObject(SBBA, 3140, 760, -1)->Right();

  CreateObject(SBBA, 3461, 760, -1);
  CreateObject(SBBA, 3530, 760, -1)->Right();
  CreateObject(SBBA, 3801, 440, -1);
  CreateObject(SBBA, 3840, 440, -1)->Right();
  CreateObject(SBBA, 3880, 560, -1)->Right();

  //Verbandskisten
  CreateObject(BECR, 420, 560, -1)->AutoRespawn();
  CreateObject(BECR, 3810, 560, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 1840, 770, -1);
  CreateObject(MWCR, 2400, 770, -1);

  //Phosphorf‰sser
  CreateObject(HBRL, 1385, 670, -1)->AutoRespawn();
  CreateObject(HBRL, 2855, 670, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 720, 760, -1)->AutoRespawn();
  CreateObject(GSBL, 3520, 760, -1)->AutoRespawn();

  //Hinweisschilder
  CreateObject(SNPT, 280, 1210, -1)->Light();
  CreateObject(SGNP, 1190, 1200, -1);
  CreateObject(SNPT, 1930, 1170, -1)->Light();
  CreateObject(SGNP, 2960, 1210, -1);
  CreateObject(SNPT, 3710, 1280, -1)->Light();

  //Grenze
  CreateObject(BRDR, 0, 1110, -1)->Set(3);

  //Verbundene R‰ume
  var doorw = CreateObject(ROOM, 740, 760, -1);
  CreateObject(ROOM, 1160, 760, -1)->Connect(doorw);

  doorw = CreateObject(GAT1, 1320, 810, -1);
  CreateObject(ROOM, 1980, 850, -1)->Connect(doorw);
  doorw = CreateObject(GAT1, 2920, 810, -1);
  CreateObject(ROOM, 2260, 850, -1)->Connect(doorw);

  var doorw = CreateObject(ROOM, 3500, 760, -1);
  CreateObject(ROOM, 3080, 760, -1)->Connect(doorw);

  //Sounds

  //Wind
  CreateObject(SE4K, 930, 220, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 2120, 220, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 3300, 220, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 620, 520, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1270, 600, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2970, 600, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 3620, 520, -1)->Set("Interior*.ogg",665,105);

  //Jungel
  CreateObject(SE4K, 200, 1100, -1)->Set("Jungle*.ogg",800,200);
  CreateObject(SE4K, 800, 1100, -1)->Set("Jungle*.ogg",300,200);
  CreateObject(SE4K, 1400, 1100, -1)->Set("Jungle*.ogg",800,200);
  CreateObject(SE4K, 2000, 1100, -1)->Set("Jungle*.ogg",300,200);
  CreateObject(SE4K, 2600, 1100, -1)->Set("Jungle*.ogg",800,200);
  CreateObject(SE4K, 3500, 1100, -1)->Set("Jungle*.ogg",300,200);
  CreateObject(SE4K, 4100, 1100, -1)->Set("Jungle*.ogg",800,200);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 415, 500, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 3830, 500, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  crate = CreateObject(AMCT, 1180, 670, -1);
  crate->Set(GBOX);
  crate = CreateObject(AMCT, 3060, 670, -1);
  crate->Set(GBOX);

  //Versorgungskisten (APW)
  crate = CreateObject(AMCT, 510, 730, -1);
  crate->Set(ATWN);
  crate = CreateObject(AMCT, 3730, 730, -1);
  crate->Set(ATWN);

  //Kugeln
  PlaceSpawnpoint(ABOX, 2120, 845);

  //Raketen
  PlaceSpawnpoint(MBOX, 430, 645);
  PlaceSpawnpoint(MBOX, 1460, 665);

  PlaceSpawnpoint(MBOX, 2780, 665);
  PlaceSpawnpoint(MBOX, 3810, 645);

  //Gesch¸tzstellungen
  CreateObject(GNET, 420, 440, -1)->Set(SATW,90);
  CreateObject(GNET, 1410, 590, -1)->Set(0,0,1);
  CreateObject(GNET, 2830, 590, -1)->Set(0,0,1);
  CreateObject(GNET, 3820, 440, -1)->Set(SATW,-90);

  //MAV-Stationen
  CreateObject(MVSN, 435, 500, -1)->Set(500,500,1);
  CreateObject(MVSN, 3805, 500, -1)->Set(3740,500,1);

  //Artilleriebatterie
  aArtillery[0] = CreateObject(ATBY,2120,690,-1);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Gel‰nder
  CreateObject(RAI3, 350, 560, -1)->SetRail([1,1]);
  CreateObject(RAI1, 630, 590, -1)->SetRail([1,1,1,1,1,1,1,1]);
  CreateObject(RAI3, 1090, 670, -1);
  CreateObject(RAI1, 1125, 620, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 1255, 620, -1);
  CreateObject(RAI3, 1450, 670, -1);

  CreateObject(RAI1, 2081, 850, -1)->SetRail([1,1,1,1]);

  CreateObject(RAI3, 2760, 670, -1);
  CreateObject(RAI1, 2960, 620, -1);
  CreateObject(RAI1, 3000, 620, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI3, 3120, 670, -1);
  CreateObject(RAI1, 3460, 590, -1)->SetRail([1,1,1,1,1,1,1,1]);
  CreateObject(RAI3, 3847, 560, -1)->SetRail([1,1]);

  //Bildschirm
  CreateObject(SCA2, 2120, 830, -1)->SetAction("News");

  //Verbranntes Schloss
  CreateObject(CSB3, 2690, 990, -1)->SetR(2);

  //Z‰une
  CreateObject(FENC, 1310, 670, -1);
  CreateObject(FENC, 1660, 770, -1)->Set(2);
  CreateObject(FENC, 2580, 770, -1)->Set(2);
  CreateObject(FENC, 2930, 670, -1);

  //Feuerlˆscher
  CreateObject(FIE2, 500, 555, -1);
  CreateObject(FIE2, 1200, 665, -1);
  CreateObject(FIE2, 3040, 665, -1);
  CreateObject(FIE2, 3740, 555, -1);

  //Schilder
  CreateObject(ESGN, 480, 550, -1);
  CreateObject(MSGN, 635, 590, -1);
  CreateObject(MSGN, 815, 590, -1);
  CreateObject(ESGN, 1220, 665, -1);
  CreateObject(MSGN, 1125, 620, -1);
  CreateObject(MSGN, 1275, 620, -1);
  CreateObject(MSGN, 2005, 850, -1);
  CreateObject(MSGN, 2235, 850, -1);
  CreateObject(ESGN, 3020, 665, -1);
  CreateObject(MSGN, 2965, 620, -1);
  CreateObject(MSGN, 3115, 620, -1);
  CreateObject(MSGN, 3425, 590, -1);
  CreateObject(MSGN, 3605, 590, -1);
  CreateObject(ESGN, 3760, 550, -1);

  //Orientierungslichter
  CreateObject(OLGH, 700, 590, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 1200, 620, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 2120, 700, -1)->Set(2, 30, 1, 1, 30);
  CreateObject(OLGH, 3040, 620, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 3540, 590, -1)->Set(4, 15, 1, 1, 30);

  //Straﬂenlichter
  CreateObject(SLGH, 750, 590, -1);
  CreateObject(SLGH, 1185, 620, -1);
  CreateObject(SLGH, 3055, 620, -1);
  CreateObject(SLGH, 3490, 590, -1);

  //Notausgangslichter
  CreateObject(ETLT, 410, 540, -1);
  CreateObject(ETLT, 1410, 655, -1);
  CreateObject(ETLT, 2830, 655, -1);
  CreateObject(ETLT, 3830, 540, -1);
}

/* Regelw‰hler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Matrix.ogg;CMC_Moving Squad.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_Techno.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Getaway.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Helikopter und Hinweisschilder
  if(!FindObject(NOHC))
  {
    SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,720,560,-1),70*21);
    SetupVehicleSpawn([APCE],DIR_Right,CreateObject(VSPW,1220,590,-1),70*21);
    SetupVehicleSpawn([APCE],DIR_Left,CreateObject(VSPW,3020,590,-1),70*21);
    SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,3520,560,-1),70*21);

    CreateObject(SNPT, 630, 590, -1)->SetAction("Sign3");
    var sign = CreateObject(SGNP, 1290, 670, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
    sign = CreateObject(SGNP, 2940, 670, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
    CreateObject(SNPT, 3610, 590, -1)->SetAction("Sign3");
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
    //Objekte entfernen
    RemoveObject(FindObject2(Find_ID(SNPT),Find_InRect(590, 500, 50, 50)));
    RemoveObject(FindObject2(Find_ID(SNPT),Find_InRect(3570, 500, 50, 50)));
  }

  //MR-Spielziel
  if(FindObject(GMNR))
  {
    //Gelds‰cke
    AddMoneySpawn(770, 750, [20]);
    AddMoneySpawn(1290, 840, [15]);
    AddMoneySpawn(2120, 680, [15]);
    AddMoneySpawn(2950, 840, [15]);
    AddMoneySpawn(3470, 750, [20]);
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Flaggenposten
    var flag = CreateObject(OFPL, 2120, 690, -1);
    flag->~Set("$Flag5$");

    //Teamgrenzen
    CreateObject(BRDR, 1760, 0, -1)->Set(0,1,0,1,1);
    CreateObject(BRDR, 2480, 0, -1)->Set(1,1,0,1,2);

    //Selbstschussanlagen
    var selfdef = CreateObject(SEGU, 1135, 620, -1);
    selfdef->Set(0,1,1,180,0,1680);
    selfdef->SetTeam(1);
    selfdef = CreateObject(SEGU, 3105, 620, -1);
    selfdef->Set(0,1,1,180,2560);
    selfdef->SetTeam(2);

    //Objekt entfernen
    RemoveObject(aArtillery[0]);

    //Zus‰tzliche Munition
    if(!FindObject(NOAM))
    {
      //Gewehrgranaten
      PlaceSpawnpoint(GBOX, 2010, 845);

      //Raketen
      PlaceSpawnpoint(MBOX, 2230, 845);
    }
  }
}

/* Relaunch */

public func PlaceSpawnplaces()
{
  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  var rsp;

  if(FindObject(GHTF) || FindObject(GMNR) || FindObject(GLMS))
  {  
    //Spawnorte
    rsp = CreateObject(CRSP, 380, 550);
    rsp->AddRespawnplaces([[380, 550], [475, 630], [585, 680]]);
    rsp->SetObjectTeam(1);

    rsp = CreateObject(CRSP, 3655, 680);
    rsp->AddRespawnplaces([[3655, 680], [3765, 630], [3860, 550]]);
    rsp->SetObjectTeam(2);
  }
  if(FindObject(GOCC))
  {
    //Flaggenposten
    aFlag[0] = CreateObject(OFPL,630,590,NO_OWNER);
    aFlag[0]->AddRespawnplace(460,740);
    aFlag[0]->AddRespawnplace(580,680);
    aFlag[0]->AddRespawnplace(380,550);
    if(aTeams[1] == true)
    {
      aFlag[0]->Set("$Flag1$",100,2);
      aFlag[0]->Capture(1,1);
    }
    else
    {
      aFlag[0]->Set("$Flag1$",0,2);
    }

    aFlag[1] = CreateObject(OFPL,1330,670,NO_OWNER);
    aFlag[1]->AddRespawnplace(1190,750);
    aFlag[1]->AddRespawnplace(1300,840);
    aFlag[1]->AddRespawnplace(1130,610);
    aFlag[1]->Set("$Flag2$",0,2);

    aFlag[2] = CreateObject(OFPL,2910,670,NO_OWNER);
    aFlag[2]->AddRespawnplace(2950,840);
    aFlag[2]->AddRespawnplace(3050,750);
    aFlag[2]->AddRespawnplace(3110,610);
    aFlag[2]->Set("$Flag3$",0,2);

    aFlag[3] = CreateObject(OFPL,3610,590,NO_OWNER);
    aFlag[3]->AddRespawnplace(3660,680);
    aFlag[3]->AddRespawnplace(3780,740);
    aFlag[3]->AddRespawnplace(3860,550);
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

/* Startsicht */

global func ChooserMenuPosition()
{
  return [[2220, 600]];
}
