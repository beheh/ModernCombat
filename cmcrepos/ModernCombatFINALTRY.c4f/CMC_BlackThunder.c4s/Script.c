/*-- Black Thunder --*/

#strict
#include CSTD

static aFlag,aArtillery;


/* Initialisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Getaway.ogg");
  //Hintergrundbewegung
  SetSkyParallax(0,15,15);
  //Flaggen
  aFlag = [];
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
  var tmp;
  Log("$CreatingFurniture$");

  //Leitern
  CreateObject(LADR, 430, 930, -1)->Set(22);
  CreateObject(LADR, 460, 1240, -1)->Set(35);

  CreateObject(LADR, 460, 540, -1)->Set(4);
  CreateObject(LADR, 790, 750, -1)->Set(19);
  CreateObject(LADR, 1250, 844, -1)->Set(27);

  CreateObject(LADR, 2020, 850, -1)->Set(19);
  CreateObject(LADR, 2040, 1170, -1)->Set(39);
  CreateObject(LADR, 2200, 1170, -1)->Set(39);
  CreateObject(LADR, 2220, 850, -1)->Set(19);

  CreateObject(LADR, 2990, 844, -1)->Set(27);
  CreateObject(LADR, 3450, 750, -1)->Set(19);
  CreateObject(LADR, 3780, 540, -1)->Set(4);

  CreateObject(LADR, 3810, 930, -1)->Set(22);
  CreateObject(LADR, 3780, 1240, -1)->Set(35);

  //Bodenlucken
  CreateObject(HA4K, 460, 503, -1);
  CreateObject(HA4K, 790, 593, -1);

  CreateObject(HA4K, 1250, 623, -1);
  CreateObject(HA4K, 1250, 673, -1);

  CreateObject(HA4K, 2990, 623, -1);
  CreateObject(HA4K, 2990, 673, -1);

  CreateObject(HA4K, 3450, 593, -1);
  CreateObject(HA4K, 3780, 503, -1);

  //Gerüste
  CreateObject(SFFG, 1580, 770, -1)->Set(2);
  CreateObject(SFFG, 1580, 820, -1);
  CreateObject(SFFG, 1660, 820, -1);
  CreateObject(SFFG, 1800, 820, -1);
  CreateObject(SFFG, 1880, 820, -1);

  CreateObject(SFFG, 2360, 820, -1);
  CreateObject(SFFG, 2440, 820, -1);
  CreateObject(SFFG, 2580, 820, -1);
  CreateObject(SFFG, 2660, 770, -1)->Set(3);
  CreateObject(SFFG, 2660, 820, -1);

  //Schutztüren
  CreateObject(GDDR, 1125, 670, -1);
  CreateObject(GDDR, 1275, 670, -1);

  CreateObject(GDDR, 2965, 670, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 3115, 670, -1)->SetColorDw(HSL(145, 210, 100, 127));

  //Sandsackbarrieren
  CreateObject(SBBA, 360, 560, -1);
  CreateObject(SBBA, 401, 440, -1);
  CreateObject(SBBA, 440, 440, -1)->Right();
  CreateObject(SBBA, 710, 760, -1);
  CreateObject(SBBA, 780, 760, -1)->Right();

  CreateObject(SBBA, 1100, 670, -1);
  CreateObject(SBBA, 1100, 760, -1);
  CreateObject(SBBA, 1350, 850, -1)->Right();
  CreateObject(SBBA, 1391, 590, -1);
  CreateObject(SBBA, 1430, 590, -1)->Right();
  CreateObject(SBBA, 1460, 670, -1)->Right();

  CreateObject(SBBA, 1961, 750, -1);
  CreateObject(SBBA, 1961, 850, -1);
  CreateObject(SBBA, 2280, 750, -1)->Right();
  CreateObject(SBBA, 2280, 850, -1)->Right();

  CreateObject(SBBA, 2780, 670, -1);
  CreateObject(SBBA, 2811, 590, -1);
  CreateObject(SBBA, 2850, 590, -1)->Right();
  CreateObject(SBBA, 2890, 850, -1);
  CreateObject(SBBA, 3140, 670, -1)->Right();
  CreateObject(SBBA, 3140, 760, -1)->Right();

  CreateObject(SBBA, 3460, 760, -1);
  CreateObject(SBBA, 3530, 760, -1)->Right();
  CreateObject(SBBA, 3801, 440, -1);
  CreateObject(SBBA, 3840, 440, -1)->Right();
  CreateObject(SBBA, 3880, 560, -1)->Right();

  //Metallkisten
  CreateObject(MWCR, 1840, 770, -1);
  CreateObject(MWCR, 2400, 770, -1);

  //Geländer
  CreateObject(RAI3, 350, 560, -1)->SetRail([1,1]);
  CreateObject(RAI1, 630, 590, -1)->SetRail([1,1,1,1,1,1,1,1]);
  CreateObject(RAI3, 1090, 670, -1);

  CreateObject(RAI1, 1125, 620, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 1255, 620, -1);
  CreateObject(RAI3, 1440, 670, -1);

  CreateObject(RAI3, 2770, 670, -1);
  CreateObject(RAI1, 2960, 620, -1);
  CreateObject(RAI1, 3000, 620, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI3, 3120, 670, -1);
  CreateObject(RAI1, 3460, 590, -1)->SetRail([1,1,1,1,1,1,1,1]);
  CreateObject(RAI3, 3847, 560, -1)->SetRail([1,1]);

  //Phosphorfässer
  CreateObject(HBRL, 1385, 670, -1)->AutoRespawn();
  CreateObject(HBRL, 2855, 670, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 720, 760, -1)->AutoRespawn();
  CreateObject(GSBL, 3520, 760, -1)->AutoRespawn();

  //Zäune
  CreateObject(FENC, 1310, 670, -1);
  CreateObject(FENC, 1660, 770, -1);
  CreateObject(FENC, 2580, 770, -1);
  CreateObject(FENC, 2930, 670, -1);

  //Markierungsschilder
  CreateObject(MSGN, 775, 760, -1);
  CreateObject(MSGN, 1125, 760, -1);
  CreateObject(MSGN, 2015, 850, -1);

  CreateObject(MSGN, 2235, 850, -1);
  CreateObject(MSGN, 3125, 760, -1);
  CreateObject(MSGN, 3475, 760, -1);

  //Orientierungslichter
  CreateObject(OLGH, 700, 590, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 1200, 620, -1)->Set(4, 15, 1, 1, 30);

  CreateObject(OLGH, 2120, 700, -1)->Set(2, 30, 1, 1, 30);

  CreateObject(OLGH, 3040, 620, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 3540, 590, -1)->Set(4, 15, 1, 1, 30);

  //Notausgangslichter
  CreateObject(ETLT, 410, 540, -1);
  CreateObject(ETLT, 1410, 655, -1);
  CreateObject(ETLT, 2830, 655, -1);
  CreateObject(ETLT, 3830, 540, -1);

  //Grenze
  CreateObject(BRDR, 0, 1110, -1)->Set(3);

  //Verbundene Räume
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
  var tmp = CreateObject (AMCT, 415, 500, -1);
  tmp->Set(ABOX);
  var tmp = CreateObject (AMCT, 3830, 500, -1);
  tmp->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  var tmp = CreateObject (AMCT, 1180, 670, -1);
  tmp->Set(GBOX);
  var tmp = CreateObject (AMCT, 3060, 670, -1);
  tmp->Set(GBOX);

  //Kugeln
  PlaceSpawnpoint(ABOX, 2120, 845);

  //Raketen
  PlaceSpawnpoint(MBOX, 430, 645);
  PlaceSpawnpoint(MBOX, 1410, 585);

  PlaceSpawnpoint(MBOX, 2830, 585);
  PlaceSpawnpoint(MBOX, 3810, 645);

  //Artilleriebatterie
  aArtillery[0] = CreateObject(ATBY,2120,690,-1);

  //Blackhawks
  SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,700,560,-1),60*21,300);
  SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,1185,590,-1),60*21,300);

  SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,3055,590,-1),60*21,300);
  SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,3540,560,-1),60*21,300);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //HTF-Spielziel
  if (FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 2120, 690, -1);
   flag->~Set("$Flag5$");

   //Objekt entfernen
   RemoveObject(aArtillery[0]);

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Kugeln
    PlaceSpawnpoint(ABOX, 2010, 845);

    //Raketen
    PlaceSpawnpoint(MBOX, 2230, 845);
   }
  }

  //Base Assault-Spielziel
  if(FindObject(GBAS))
  {
   //Zielobjekte
   AddAssaultTarget(RADR, 485, 500, 400, 1, "$Flag1$", 0, [[480, 640], [510, 730], [585, 690]]);
   AddAssaultTarget(CMSN, 1330, 670, 300, 1, "$Flag2$", 1, [[1160, 670], [1300, 850], [1455, 670]]);

   AddAssaultTarget(RADR, 3755, 500, 400, 2, "$Flag4$", 0, [[3760, 640], [3730, 730], [3655, 690]]);
   AddAssaultTarget(CMSN, 2910, 670, 300, 2, "$Flag3$", 1, [[3080, 670], [2940, 850], [2785, 670]]);
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,630,590,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(460,740);
   aFlag[0] -> AddSpawnPoint(580,680);
   aFlag[0] -> AddSpawnPoint(380,550);
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
   aFlag[1] -> AddSpawnPoint(1190,750);
   aFlag[1] -> AddSpawnPoint(1300,840);
   aFlag[1] -> AddSpawnPoint(1130,610);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,2910,670,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(2950,840);
   aFlag[2] -> AddSpawnPoint(3050,750);
   aFlag[2] -> AddSpawnPoint(3110,610);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,3610,590,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(3660,680);
   aFlag[3] -> AddSpawnPoint(3780,740);
   aFlag[3] -> AddSpawnPoint(3860,550);
   if(aTeams[1] == true)
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

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //LMS/HTF-Spielziel
  if(FindObject(GLMS) || FindObject(GHTF))
  {
   if(iTeam == 1)
   {
    var rand = Random(3);
    if(!rand)
     { iX = 380; iY = 550; }
    if(!--rand)
     { iX = 475; iY = 630; }
    if(!--rand)
     { iX = 585; iY = 680; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 3655; iY = 680; }
    if(!--rand)
     { iX = 3765; iY = 630; }
    if(!--rand)
     { iX = 3860; iY = 550; }
   }
   return(1);
  }

  //Startsicht
  iX = 2220; iY = 600;
}