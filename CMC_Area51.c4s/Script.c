/*-- Area 51 --*/

#strict
#include CSTD


/* Initalisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg");
  Music("CMC_Eurocorps.ogg",1);

  //Verzögerte Hinweisnachricht ausgeben
  Schedule("EventInfo4K(0,Format(\"$MsgWelcome$\"),PCMK, 0, 0, 0, \"PriorityInfo.ogg\");", 100);

  //Bonusspawn
  PlaceBonusSpawnpoint([MNYB], 720, 1100);
  PlaceBonusSpawnpoint([MNYB], 740, 1100);
  PlaceBonusSpawnpoint([MNYB], 760, 1100);
  PlaceBonusSpawnpoint([MNYB], 780, 1100);
  PlaceBonusSpawnpoint([MNYB], 800, 1100);
  PlaceBonusSpawnpoint([MNYB], 820, 1100);

  PlaceBonusSpawnpoint([MNYB], 1260, 1100);
  PlaceBonusSpawnpoint([MNYB], 1280, 1100);
  PlaceBonusSpawnpoint([MNYB], 1300, 1100);
  PlaceBonusSpawnpoint([MNYB], 1320, 1100);
  PlaceBonusSpawnpoint([MNYB], 1340, 1100);
  PlaceBonusSpawnpoint([MNYB], 1360, 1100);

  //Bodenluken
  CreateObject(HA4K, 680, 1063, -1);
  CreateObject(HA4K, 1400, 1063, -1);

  //Clonks
  CreateObject(PCMK, 650, 1060, -1)->AutoRespawn();
  CreateObject(PCMK, 1430, 1060, -1)->AutoRespawn();

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

  //Sprungschanzen
  CreateObject (JMPD, 210, 390, -1)->Set(130, 40);
  CreateObject (JMPD, 210, 650, -1)->Set(135, 40);
  CreateObject (JMPD, 385, 1060, -1)->Set(140, -10);
  CreateObject (JMPD, 1695, 1060, -1)->Set(140, 10);
  CreateObject (JMPD, 1870, 390, -1)->Set(130, -40);
  CreateObject (JMPD, 1870, 650, -1)->Set(135, -40);

  //Radios
  CreateObject(RDIO, 795, 1010, -1);
  CreateObject(RDIO, 1285, 1010, -1);

  //Leitern
  CreateObject(LADR, 680, 1110, -1)->Set(6);
  CreateObject(LADR, 1400, 1110, -1)->Set(6);

  //Stahlbrücken
  CreateObject(_HBR, 395, 1072, -1)->SwitchRuinsMode();
  CreateObject(_HBR, 605, 1072, -1);
  CreateObject(_HBR, 726, 1122, -1)->SwitchRuinsMode();
  CreateObject(_HBR, 1354, 1122, -1)->SwitchRuinsMode();
  CreateObject(_HBR, 1475, 1072, -1);
  CreateObject(_HBR, 1685, 1072, -1)->SwitchRuinsMode();

  //Waffenautomaten
  SetUpStore(CreateObject(WPVM, 115, 390, -1));
  SetUpStore(CreateObject(WPVM, 115, 650, -1));
  SetUpStore(CreateObject(WPVM, 750, 1060, -1));
  SetUpStore(CreateObject(WPVM, 1330, 1060, -1));
  SetUpStore(CreateObject(WPVM, 1965, 390, -1));
  SetUpStore(CreateObject(WPVM, 1965, 650, -1));

  //Explosivfass
  CreateObject(XBRL, 880, 1060, -1)->AutoRespawn();

  //Benzinfass
  CreateObject(PBRL, 1190, 1060, -1)->AutoRespawn();

  //Versorgungskiste (APW)
  var tmp = CreateObject(AMCT, 860, 1060, -1);
  tmp->Set(ATWN);

  //APW
  PlaceSpawnpoint(ATWN, 1210, 1060);

  //Blackhawk
  SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,1080,1030,-1),1,300);

  //Apache
  SetupVehicleSpawn([APCE],DIR_Left,CreateObject(VSPW,950,1030,-1),1,300);

  //Reitwipfe
  CreateObject(RWPF, 505, 1060, -1)->AutoRespawn();
  CreateObject(RWPF, 1580, 1060, -1)->AutoRespawn();
}

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
  pStore->AddWare(XBRL,-1);
  pStore->AddWare(PBRL,-1);
  pStore->AddWare(HBRL,-1);
  pStore->AddWare(TBRL,-1);
  pStore->AddWare(WCR2,-1);

}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  var rand = Random(4);
  if(!rand)
    { iX = 160; iY = 380; }
  if(!--rand)
    { iX = 160; iY = 640; }
  if(!--rand)
    { iX = 1910; iY = 380; }
  if(!--rand)
    { iX = 1910; iY = 640; }
}