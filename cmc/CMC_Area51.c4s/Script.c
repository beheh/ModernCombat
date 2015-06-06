/*-- Area 51 --*/

#strict
#include CSTD

public func SpecificEquipment()	{return [[JTPK, 1]];} //Zusatzausrüstung: Jetpack

static zipline1, zipline2;

/* Initalisierung */

public func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg");
  Music("");
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  return(1);
}

/* Plazierungslisten */

public func CreateInterior()
{
  Log("$CreatingInterior$");

  //Bodenluken
  CreateObject(HA4K, 680, 1063, -1);
  CreateObject(HA4K, 1400, 1063, -1);

  //Sprungschanzen
  CreateObject (JMPD, 210, 390, -1)->Set(130, 40);
  CreateObject (JMPD, 210, 650, -1)->Set(135, 40);
  CreateObject (JMPD, 385, 1060, -1)->Set(140, -10);
  CreateObject (JMPD, 1695, 1060, -1)->Set(140, 10);
  CreateObject (JMPD, 1870, 390, -1)->Set(130, -40);
  CreateObject (JMPD, 1870, 650, -1)->Set(135, -40);

  //Leitern
  CreateObject(LADR, 680, 1110, -1)->Set(6);
  CreateObject(LADR, 1400, 1110, -1)->Set(6);

  //Stahlbrücken
  CreateObject(_HBR, 395, 1072, -1)->SwitchMode();
  CreateObject(_HBR, 605, 1072, -1);
  CreateObject(_HBR, 726, 1122, -1)->SwitchMode();
  CreateObject(_HBR, 1354, 1122, -1)->SwitchMode();
  CreateObject(_HBR, 1475, 1072, -1);
  CreateObject(_HBR, 1685, 1072, -1)->SwitchMode();

  //Explosivfass
  CreateObject(XBRL, 880, 1060, -1)->AutoRespawn();

  //Benzinfass
  CreateObject(PBRL, 1190, 1060, -1)->AutoRespawn();

  //Grenze
  CreateObject(BRDR, 0, 1680, -1)->Set(3,0,1);

  //Selbstschussanlage und Konsole
  var selfd = CreateObject(SEGU, 1925, 689, -1);
  selfd->Arm(MISA);
  CreateObject(CONS, 1640, 1055, -1)->Set(selfd);

  //Tor und Konsole
  var autod = CreateObject (HNG3, 1040, 1140, -1);
  autod->Open();
  CreateObject (CONS, 650, 1105, -1)->Set(autod);
  
  //Ziplines
  zipline1 = CreateZipline(1860, 650, 810, 1010);
  zipline2 = CreateZipline(220, 650, 1270, 1010);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Clonks
  //CreateObject(PCMK, 650, 1060, -1)->AutoRespawn();
  //CreateObject(PCMK, 1430, 1060, -1)->AutoRespawn();

  //Bonusspawns
  PlaceBonusSpawnpoint([MNYB], 720, 1100);
  PlaceBonusSpawnpoint([MNYB], 740, 1100);
  PlaceBonusSpawnpoint([MNYB], 760, 1100);
  PlaceBonusSpawnpoint([MNYB], 780, 1100);
  PlaceBonusSpawnpoint([MNYB], 800, 1100);
  PlaceBonusSpawnpoint([MNYB], 820, 1100);

  PlaceBonusSpawnpoint([MNYB], 1030, 305);
  PlaceBonusSpawnpoint([MNYB], 1030, 555);
  PlaceBonusSpawnpoint([MNYB], 1050, 555);
  PlaceBonusSpawnpoint([MNYB], 1050, 305);

  PlaceBonusSpawnpoint([MNYB], 1260, 1100);
  PlaceBonusSpawnpoint([MNYB], 1280, 1100);
  PlaceBonusSpawnpoint([MNYB], 1300, 1100);
  PlaceBonusSpawnpoint([MNYB], 1320, 1100);
  PlaceBonusSpawnpoint([MNYB], 1340, 1100);
  PlaceBonusSpawnpoint([MNYB], 1360, 1100);

  //Geschützstellungen
  CreateObject(GNET, 1000, 310, -1)->Set(0,-90);
  CreateObject(GNET, 1000, 560, -1)->Set(SATW,-90);
  CreateObject(GNET, 1080, 560, -1)->Set(SATW,90);
  CreateObject(GNET, 1080, 310, -1)->Set(0,90);

  //MAV-Stationen
  CreateObject(MVSN, 185, 390, -1);
  CreateObject(MVSN, 185, 650, -1);
  CreateObject(MVSN, 500, 1060, -1);
  CreateObject(MVSN, 1890, 390, -1);
  CreateObject(MVSN, 1890, 650, -1);

  //Waffenautomaten
  SetUpStore(CreateObject(WPVM, 115, 390, -1));
  SetUpStore(CreateObject(WPVM, 115, 650, -1));
  SetUpStore(CreateObject(WPVM, 750, 1060, -1));
  SetUpStore(CreateObject(WPVM, 1330, 1060, -1));
  SetUpStore(CreateObject(WPVM, 1965, 390, -1));
  SetUpStore(CreateObject(WPVM, 1965, 650, -1));

  //Versorgungskiste (APW)
  var tmp = CreateObject(AMCT, 860, 1060, -1);
  tmp->Set(ATWN);

  //APW
  PlaceSpawnpoint(ATWN, 1210, 1060);

  //Blackhawk
  SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,1080,1030,-1),1,300);

  //Apache
  SetupVehicleSpawn([APCE],DIR_Left,CreateObject(VSPW,950,1030,-1),1,300);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Radios
  CreateObject(RDIO, 795, 1010, -1);
  CreateObject(RDIO, 1285, 1010, -1);

  //Leitkegel
  CreateObject(TCCN, 820, 1060, -1);
  CreateObject(TCCN, 840, 1060, -1);
  CreateObject(TCCN, 860, 1060, -1);
  CreateObject(TCCN, 880, 1060, -1);
  CreateObject(TCCN, 900, 1060, -1);
  CreateObject(TCCN, 920, 1060, -1);
  CreateObject(TCCN, 940, 1060, -1);
  CreateObject(TCCN, 960, 1060, -1);
  CreateObject(TCCN, 980, 1060, -1);
  CreateObject(TCCN, 1000, 1060, -1);
  CreateObject(TCCN, 1020, 1060, -1);
  CreateObject(TCCN, 1040, 1060, -1);
  CreateObject(TCCN, 1060, 1060, -1);
  CreateObject(TCCN, 1080, 1060, -1);
  CreateObject(TCCN, 1100, 1060, -1);
  CreateObject(TCCN, 1120, 1060, -1);
  CreateObject(TCCN, 1140, 1060, -1);
  CreateObject(TCCN, 1160, 1060, -1);
  CreateObject(TCCN, 1180, 1060, -1);
  CreateObject(TCCN, 1200, 1060, -1);
  CreateObject(TCCN, 1220, 1060, -1);
  CreateObject(TCCN, 1240, 1060, -1);
  CreateObject(TCCN, 1260, 1060, -1);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD03,300,700,-1);
  CreateObject(BD03,500,1400,-1);
  CreateObject(BD03,1580,1400,-1);
  CreateObject(BD03,1900,900,-1);
}

public func TestDestroyZiplines()
{
	RemoveObject(zipline1);
	return 1;
}

/* Sonstiges */

public func SetUpStore(pStore)
{
  pStore->AddWares("IsWeapon", -1);
  pStore->AddWares("IsAmmoPacket", -1);
  pStore->AddWares("IsUpgrade", -1);
  pStore->AddWares("IsEquipment", -1);
  pStore->SortWare("IsWeapon","IsAmmoPacket","IsUpgrade","IsEquipment");
  pStore->AddWare(DGNN,-1);
  pStore->AddWare(DRSU,-1);
  pStore->AddWare(CATA,-1);
  pStore->AddWare(BLMP,-1);
  pStore->AddWare(WCR2,-1);
}

public func MessagePlayers()
{
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    DoScoreboardShow(1, GetPlayerByIndex(i) + 1);
    CreateObject(TKBT, 0, 0, GetPlayerByIndex(i));
    Sound("ObjectiveReceipt.ogg", true, 0, 100, GetPlayerByIndex(i) + 1);
  }
}

/* Relaunch */
public func PlaceSpawnpoints()
{
  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  var rsp;
  //Spawnorte
  rsp = CreateObject(CRSP, 160, 380);
  rsp->AddRespawnpoints([[160, 380], [160, 640], [1910, 380], [1910, 640]]);
  rsp->SetObjectTeam(1);

  rsp = CreateObject(CRSP, 160, 380);
  rsp->AddRespawnpoints([[160, 380], [160, 640], [1910, 380], [1910, 640]]);
  rsp->SetObjectTeam(2);
}
