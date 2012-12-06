/*-- Beat Box Galaxy --*/

#strict
#include CSTD


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  if(!Random(2))
  {
   SetPlayList("CMC_WaitingforSpawn2.ogg");
   Music("CMC_WaitingforSpawn2.ogg",1);
  }
  else
  {
   SetPlayList("CMC_WaitingforSpawn.ogg");
   Music("CMC_WaitingforSpawn.ogg",1);
  }
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

  //Beat Boxen
  CreateObject(BTBX, 470, 260, -1)->ChangeSide();
  CreateObject(BTBX, 470, 410, -1)->ChangeSide();

  CreateObject(BTBX, 510, 310, -1);
  CreateObject(BTBX, 510, 350, -1);
  CreateObject(BTBX, 510, 390, -1);

  CreateObject(BTBX, 550, 430, -1)->ChangeSide();

  CreateObject(BTBX, 590, 570, -1)->ChangeSide();
  CreateObject(BTBX, 630, 290, -1);
  CreateObject(BTBX, 630, 430, -1);
  CreateObject(BTBX, 670, 570, -1);

  CreateObject(BTBX, 710, 290, -1);

  CreateObject(BTBX, 750, 450, -1)->ChangeSide();
  CreateObject(BTBX, 750, 490, -1)->ChangeSide();
  CreateObject(BTBX, 750, 530, -1)->ChangeSide();
  CreateObject(BTBX, 750, 570, -1)->ChangeSide();

  CreateObject(BTBX, 790, 210, -1)->ChangeSide();
  CreateObject(BTBX, 790, 250, -1)->ChangeSide();
  CreateObject(BTBX, 790, 290, -1)->ChangeSide();

  CreateObject(BTBX, 845, 430, -1)->ChangeSide();
  CreateObject(BTBX, 870, 290, -1)->ChangeSide();

  CreateObject(BTBX, 940, 450, -1);
  CreateObject(BTBX, 940, 490, -1);
  CreateObject(BTBX, 940, 530, -1);
  CreateObject(BTBX, 940, 570, -1);

  CreateObject(BTBX, 930, 170, -1)->ChangeSide();
  CreateObject(BTBX, 970, 170, -1)->ChangeSide();
  CreateObject(BTBX, 1010, 170, -1);

  CreateObject(BTBX, 1050, 540, -1)->ChangeSide();

  CreateObject(BTBX, 1090, 250, -1)->ChangeSide();
  CreateObject(BTBX, 1130, 250, -1);
  CreateObject(BTBX, 1170, 250, -1)->ChangeSide();
  CreateObject(BTBX, 1210, 250, -1);
  CreateObject(BTBX, 1250, 250, -1)->ChangeSide();
  CreateObject(BTBX, 1290, 250, -1);
  CreateObject(BTBX, 1290, 290, -1);

  CreateObject(BTBX, 1190, 400, -1);
  CreateObject(BTBX, 1230, 400, -1);
  CreateObject(BTBX, 1270, 400, -1);

  CreateObject(BTBX, 1190, 540, -1);
  CreateObject(BTBX, 1230, 540, -1)->ChangeSide();
  CreateObject(BTBX, 1270, 540, -1);
  CreateObject(BTBX, 1310, 540, -1)->ChangeSide();
  CreateObject(BTBX, 1350, 540, -1);
  CreateObject(BTBX, 1390, 540, -1)->ChangeSide();

  CreateObject(BTBX, 1350, 400, -1)->ChangeSide();
  CreateObject(BTBX, 1390, 400, -1)->ChangeSide();
  CreateObject(BTBX, 1430, 400, -1)->ChangeSide();
  CreateObject(BTBX, 1470, 400, -1)->ChangeSide();


  CreateObject(BTBX, 1510, 400, -1);
  CreateObject(BTBX, 1550, 400, -1);
  CreateObject(BTBX, 1590, 400, -1);
  CreateObject(BTBX, 1630, 400, -1);

  CreateObject(BTBX, 1590, 540, -1)->ChangeSide();
  CreateObject(BTBX, 1630, 540, -1);
  CreateObject(BTBX, 1670, 540, -1)->ChangeSide();
  CreateObject(BTBX, 1710, 540, -1);
  CreateObject(BTBX, 1750, 540, -1)->ChangeSide();
  CreateObject(BTBX, 1790, 540, -1);

  CreateObject(BTBX, 1710, 400, -1)->ChangeSide();
  CreateObject(BTBX, 1750, 400, -1)->ChangeSide();
  CreateObject(BTBX, 1790, 400, -1)->ChangeSide();


  CreateObject(BTBX, 1690, 250, -1)->ChangeSide();
  CreateObject(BTBX, 1690, 290, -1)->ChangeSide();
  CreateObject(BTBX, 1730, 250, -1);
  CreateObject(BTBX, 1770, 250, -1)->ChangeSide();
  CreateObject(BTBX, 1810, 250, -1);
  CreateObject(BTBX, 1850, 250, -1)->ChangeSide();
  CreateObject(BTBX, 1890, 250, -1);

  CreateObject(BTBX, 1930, 540, -1);

  CreateObject(BTBX, 1970, 170, -1);
  CreateObject(BTBX, 2010, 170, -1);
  CreateObject(BTBX, 2050, 170, -1)->ChangeSide();

  CreateObject(BTBX, 2040, 450, -1)->ChangeSide();
  CreateObject(BTBX, 2040, 490, -1)->ChangeSide();
  CreateObject(BTBX, 2040, 530, -1)->ChangeSide();
  CreateObject(BTBX, 2040, 570, -1)->ChangeSide();

  CreateObject(BTBX, 2110, 290, -1);

  CreateObject(BTBX, 2190, 210, -1);
  CreateObject(BTBX, 2190, 250, -1);
  CreateObject(BTBX, 2190, 290, -1);

  CreateObject(BTBX, 2230, 450, -1);
  CreateObject(BTBX, 2230, 490, -1);
  CreateObject(BTBX, 2230, 530, -1);
  CreateObject(BTBX, 2230, 570, -1);

  CreateObject(BTBX, 2135, 430, -1);
  CreateObject(BTBX, 2270, 290, -1)->ChangeSide();

  CreateObject(BTBX, 2310, 570, -1)->ChangeSide();
  CreateObject(BTBX, 2350, 290, -1)->ChangeSide();
  CreateObject(BTBX, 2350, 430, -1)->ChangeSide();
  CreateObject(BTBX, 2390, 570, -1);

  CreateObject(BTBX, 2430, 430, -1);

  CreateObject(BTBX, 2470, 310, -1)->ChangeSide();
  CreateObject(BTBX, 2470, 350, -1)->ChangeSide();
  CreateObject(BTBX, 2470, 390, -1)->ChangeSide();

  CreateObject(BTBX, 2510, 260, -1);
  CreateObject(BTBX, 2510, 410, -1);

  //Leitern
  CreateObject(LADR, 350, 620, -1)->Set(50);
  CreateObject(LADR, 1055, 370, -1)->Set(30);
  CreateObject(LADR, 350, 620, -1)->Set(40);
  CreateObject(LADR, 1130, 480, -1)->Set(14);
  CreateObject(LADR, 1470, 480, -1)->Set(10);
  CreateObject(LADR, 1510, 480, -1)->Set(10);
  CreateObject(LADR, 1850, 480, -1)->Set(14);
  CreateObject(LADR, 350, 620, -1)->Set(50);
  CreateObject(LADR, 1925, 370, -1)->Set(30);
  CreateObject(LADR, 2630, 620, -1)->Set(50);

  //Bodenlucken
  CreateObject(HA4K, 1130, 363, -1);
  CreateObject(HA4K, 1850, 363, -1);

  //Kisten
  CreateObject(WCR2, 870, 500, -1)->AutoRespawn();
  CreateObject(WCR2, 2110, 500, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 915, 250, -1)->AutoRespawn();
  CreateObject(PBRL, 2065, 250, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 795, 500, -1)->AutoRespawn();
  CreateObject(XBRL, 2185, 500, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1080, 390, -1)->AutoRespawn();
  CreateObject(GSBL, 1490, 500, -1)->AutoRespawn();
  CreateObject(GSBL, 1900, 390, -1)->AutoRespawn();

  //Explosivtanks
  CreateObject(XTNK, 1030, 390, -1)->AutoRespawn();
  CreateObject(XTNK, 1950, 390, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 1450, 500, -1)->AutoRespawn();
  CreateObject(PTNK, 1530, 500, -1)->AutoRespawn();

  //Grenze
  CreateObject(BRDR, 0, 720, -1)->Set(3,0,1);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var tmp = CreateObject (AMCT, 460, 530, -1);
  tmp->Set(ABOX);
  var tmp = CreateObject (AMCT, 2520, 530, -1);
  tmp->Set(ABOX);

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 390, 365);
  PlaceSpawnpoint(GBOX, 2590, 365);

  //Raketen
  PlaceSpawnpoint(MBOX, 830, 245);
  PlaceSpawnpoint(MBOX, 2150, 245);

  //Artilleriebatterien
  CreateObject(ATBY,725,150,-1)->SetRotation(17);
  CreateObject(ATBY,2255,150,-1)->SetRotation(-17);

  //MAV-Stationen
  CreateObject(MVSN, 965, 250, -1)->Set(895,130,1);
  CreateObject(MVSN, 2015, 250, -1)->Set(2085,130,1);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Zäune
  CreateObject(FENC, 410, 220, -1);
  CreateObject(FENC, 1060, 390, -1);
  CreateObject(FENC, 1920, 390, -1);
  CreateObject(FENC, 2570, 220, -1);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Process Rush.ogg;CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Process Rush.ogg",1);

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,845,500,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,2135,500,GetTeamColor(2));}
  }

  //HTF-Spielziel
  if (FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 1490, 360, -1);
   flag->~Set("$Flag$");
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //CTF/HTF-Spielziel
  if(FindObject(GCTF) || FindObject(GHTF))
  {
   if(iTeam == 1)
   {
    return [[510, 520], [410, 210], [410, 360]];
   }
   if(iTeam == 2)
   {
    return [[2570, 360], [2570, 210], [2470, 520]];
   }
   return(1);
  }

  //Startsicht
  iX = 1490; iY = 490;
}