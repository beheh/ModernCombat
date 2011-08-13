/*-- Oasis --*/

#strict
#include CSTD

static aFlag,aSelfDefense,aLamp,aArtillery;


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetPlayList("CMC_WaitingforSpawn.ogg");
  Music("CMC_WaitingforSpawn.ogg",1);
  //Bildschirmfärbung
  SetGamma(RGB(7,6,0), RGB(152,147,128), RGB(255,254,236) );
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
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

  //Leitern
  CreateObject(LADR, 1420, 532, -1)->Set(8);
  CreateObject(LADR, 2600, 678, -1)->Set(5);

  CreateObject(LADR, 2665, 605, -1)->Set(15);
  CreateObject(LADR, 2760, 576, -1)->Set(11);

  CreateObject(LADR, 3415, 682, -1)->Set(18);

  CreateObject(LADR, 4070, 576, -1)->Set(11);
  CreateObject(LADR, 4165, 605, -1)->Set(15);

  CreateObject(LADR, 4230, 678, -1)->Set(5);
  CreateObject(LADR, 5410, 532, -1)->Set(8);

  //Bodenluken
  CreateObject(HA4K, 1420, 463, -1);
  CreateObject(HA4K, 2600, 633, -1);
  CreateObject(HA4K, 2760, 483, -1);
  CreateObject(HA4K, 4070, 483, -1);
  CreateObject(HA4K, 4230, 633, -1);
  CreateObject(HA4K, 5410, 463, -1);

  //Große Bodenluke
  CreateObject(H24K, 3415, 538, -1);

  //Stahlbrücken
  CreateObject(_HBR, 1495, 472, -1);
  CreateObject(_HBR, 2856, 492, -1);
  CreateObject(_HBR, 3975, 492, -1);
  CreateObject(_HBR, 5335, 472, -1);

  //Steine
  CreateObject(STNE, 320, 460, -1);
  CreateObject(STNE, 1685, 600, -1);
  CreateObject(STNE, 2445, 710, -1);
  CreateObject(STNE, 2915, 680, -1);
  CreateObject(STNE, 3275, 580, -1);
  CreateObject(STNE, 3555, 580, -1);
  CreateObject(STNE, 3915, 680, -1);
  CreateObject(STNE, 4385, 710, -1);
  CreateObject(STNE, 5150, 600, -1);
  CreateObject(STNE, 6510, 460, -1);

  //Paletten
  CreateObject(PLLT, 1230, 530, -1);
  CreateObject(PLLT, 3130, 680, -1);

  CreateObject(PLLT, 3700, 680, -1);
  CreateObject(PLLT, 5600, 530, -1);

  //Gerüste
  CreateObject(SFFG, 1980, 460, -1)->Set(4);
  CreateObject(SFFG, 1980, 510, -1)->Set(3);
  CreateObject(SFFG, 1980, 560, -1)->Set(2);
  CreateObject(SFFG, 1980, 610, -1)->Set(3);

  CreateObject(SFFG, 2540, 530, -1)->Set(4);
  CreateObject(SFFG, 2540, 580, -1)->Set(4);
  CreateObject(SFFG, 2540, 630, -1);
  CreateObject(SFFG, 2620, 530, -1)->Set(2);
  CreateObject(SFFG, 2620, 580, -1)->Set(4);
  CreateObject(SFFG, 2620, 630, -1)->Set(2);

  CreateObject(SFFG, 3160, 530, -1)->Set(5);
  CreateObject(SFFG, 3160, 580, -1)->Set(2);
  CreateObject(SFFG, 3160, 630, -1);
  CreateObject(SFFG, 3160, 680, -1)->Set(2);

  CreateObject(SFFG, 3335, 530, -1)->Set(2);
  CreateObject(SFFG, 3415, 480, -1)->Set(5);
  CreateObject(SFFG, 3415, 530, -1)->Set(4);
  CreateObject(SFFG, 3495, 530, -1)->Set(3);

  CreateObject(SFFG, 3670, 530, -1)->Set(5);
  CreateObject(SFFG, 3670, 580, -1)->Set(3);
  CreateObject(SFFG, 3670, 630, -1);
  CreateObject(SFFG, 3670, 680, -1)->Set(3);

  CreateObject(SFFG, 4210, 530, -1)->Set(3);
  CreateObject(SFFG, 4210, 580, -1)->Set(4);
  CreateObject(SFFG, 4210, 630, -1)->Set(3);
  CreateObject(SFFG, 4290, 530, -1)->Set(4);
  CreateObject(SFFG, 4290, 580, -1)->Set(4);
  CreateObject(SFFG, 4290, 630, -1);

  CreateObject(SFFG, 4850, 460, -1)->Set(4);
  CreateObject(SFFG, 4850, 510, -1)->Set(2);
  CreateObject(SFFG, 4850, 560, -1)->Set(3);
  CreateObject(SFFG, 4850, 610, -1)->Set(2);

  //Kisten
  CreateObject(WCR2, 1400, 550, -1);
  CreateObject(WCR2, 2580, 580, -1);
  CreateObject(WCR2, 4250, 580, -1);
  CreateObject(WCR2, 5430, 550, -1);

  //Explosive Kiste
  CreateObject(XWCR, 3415, 700, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 2550, 530, -1)->AutoRespawn();
  CreateObject(BECR, 4280, 530, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 1190, 530, -1)->AutoRespawn();
  CreateObject(PBRL, 2010, 510, -1);
  CreateObject(PBRL, 4820, 510, -1);
  CreateObject(PBRL, 5640, 530, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 1600, 460, -1)->AutoRespawn();
  CreateObject(XBRL, 1950, 410, -1)->AutoRespawn();
  CreateObject(XBRL, 2580, 630, -1)->AutoRespawn();
  CreateObject(PBRL, 2620, 533, -1)->SetR(-90);

  CreateObject(PBRL, 4210, 533, -1)->SetR(-90);
  CreateObject(XBRL, 4250, 630, -1)->AutoRespawn();
  CreateObject(XBRL, 4880, 410, -1)->AutoRespawn();
  CreateObject(XBRL, 5230, 460, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1420, 550, -1)->AutoRespawn();
  CreateObject(GSBL, 3130, 580, -1);
  CreateObject(GSBL, 3700, 580, -1);
  CreateObject(GSBL, 5410, 550, -1)->AutoRespawn();

  //Sandsackbarrieren
  CreateObject(SBBA, 2790, 600, -1)->Right();
  CreateObject(SBBA, 4041, 600, -1);

  //Explosivtanks
  CreateObject(XTNK, 1305, 530, -1)->AutoRespawn();
  CreateObject(XTNK, 3415, 530, -1);
  CreateObject(XTNK, 5525, 530, -1)->AutoRespawn();

  //Hinweisschilder
  CreateObject(SGNP, 70, 480, -1);
  CreateObject(SGNP, 360, 520, -1);
  CreateObject(SGNP, 635, 530, -1);
  CreateObject(SGNP, 6195, 530, -1);
  CreateObject(SGNP, 6445, 520, -1);
  CreateObject(SGNP, 6780, 490, -1);

  //Grenzen
  CreateObject(BRDR, 625, 0, -1)->Set(0);
  CreateObject(BRDR, 6205, 0, -1)->Set(1);

  //Sounds

  //Zikaden
  CreateObject(SE4K, 630, 450, -1)->Set("Cicada*.ogg",700,100);
  CreateObject(SE4K, 6200, 670, -1)->Set("Cicada*.ogg",700,100);

  //Schlangen
  CreateObject(SE4K, 1815, 550, -1)->Set("Snake*",1500,200);
  CreateObject(SE4K, 3415, 630, -1)->Set("Snake*",2000,200);
  CreateObject(SE4K, 5015, 550, -1)->Set("Snake*",1500,200);

  //Wellen
  CreateObject(SE4K, 2690, 670, -1)->Set("Wave*.ogg",1000,100);
  CreateObject(SE4K, 4140, 670, -1)->Set("Wave*.ogg",1000,100);

  //Windböen
  CreateObject(SE4K, 400, 400, -1)->Set("WindGust*.ogg",775,250);
  CreateObject(SE4K, 1780, 400, -1)->Set("WindGust*.ogg",775,250);
  CreateObject(SE4K, 5060, 400, -1)->Set("WindGust*.ogg",775,250);
  CreateObject(SE4K, 6410, 400, -1)->Set("WindGust*.ogg",775,250);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Munitionskisten (Kugeln)
  var crate = CreateObject(AMCT, 1390, 460, -1);
  crate->SetGraphics("Desert");
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 3450, 530, -1);
  crate->SetGraphics("Desert");
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 5440, 460, -1);
  crate->SetGraphics("Desert");
  crate->Set(ABOX);

  //Munitionskisten (Granaten)
  crate = CreateObject(AMCT, 3380, 530, -1);
  crate->SetGraphics("Desert");
  crate->Set(GBOX);

  //Versorgungskisten (APW)
  crate = CreateObject (AMCT, 1980, 690, -1);
  crate->Set(ATWN);
  crate = CreateObject (AMCT, 4850, 690, -1);
  crate->Set(ATWN);

  //Granatenmunition
  PlaceSpawnpoint(GBOX, 1980, 595);
  PlaceSpawnpoint(GBOX, 4850, 595);

  //Raketen
  PlaceSpawnpoint(MBOX, 750, 595);
  PlaceSpawnpoint(MBOX, 3330, 685);
  PlaceSpawnpoint(MBOX, 6080, 595);

  //Artilleriebatterien
  CreateObject(ATBY,1565,460,-1)->SetRotation(20);
  CreateObject(ATBY,2795,480,-1)->SetRotation(20);
  CreateObject(ATBY,4035,480,-1)->SetRotation(-20);
  CreateObject(ATBY,5265,460,-1)->SetRotation(-20);

  //Motorboote
  SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2360,680,-1),50*21,300);
  SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4470,680,-1),50*21,300);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Orientierungslichter
  CreateObject(OLGH, 845, 600, -1)->Set(8, 10, 1, 1, 15);
  CreateObject(OLGH, 5990, 600, -1)->Set(8, 10, 1, 1, 15);

  //Regale
  CreateObject(FRAM, 1440, 460, -1);
  CreateObject(FRAM, 5390, 460, -1);

  //Satellitenschüssel
  CreateObject(SADH, 7625, 320, -1);

  //Radare
  CreateObject(RADR, 890, 460, -1);
  CreateObject(RADR, 5940, 460, -1);

  //Büsche
  CreateObject(BSH2, 920, 510, -1);
  CreateObject(BSH2, 2650, 670, -1);
  CreateObject(BSH2, 3520, 560, -1);
  CreateObject(BSH2, 5370, 500, -1);

  //Dekoschleusen
  CreateObject(GAT1, 890, 540, -1);
  CreateObject(GAT1, 5940, 540, -1);

  //Geländer
  CreateObject(RAI1, 3320, 530, -1)->SetRail([1,1,1,1,1,1,1,1,1,1]);

  //Zäune
  CreateObject(FENC, 790, 600, -1);
  CreateObject(FENC, 850, 600, -1);
  CreateObject(FENC, 910, 600, -1);

  CreateObject(FENC, 5920, 600, -1);
  CreateObject(FENC, 5980, 600, -1);
  CreateObject(FENC, 6040, 600, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 2760, 660, -1);
  CreateObject(ENGT, 4070, 660, -1);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD09,0,730,-1);
  CreateObject(BD09,1020,730,-1);
  CreateObject(BD09,2040,730,-1);
  CreateObject(BD09,3060,730,-1);
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

  //Blackhawks und Hinweisschilder
  if(!FindObject(NOBH))
  {
   SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,855,570,-1),100*21,300);
   SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,5975,570,-1),100*21,300);

   var sign = CreateObject(SGNP, 940, 600, -1);
   sign->SetPhase(2);
   sign->SetMode(1);
   sign = CreateObject(SGNP, 5890, 600, -1);
   sign->SetPhase(2);
   sign->SetMode(1);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,1550,580,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,5280,580,GetTeamColor(2));}
  }

  //HTF-Spielziel
  if (FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 3415, 480, -1);
   flag->~Set("$Flag3$");
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,1550,580,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(670,610);
   aFlag[0] -> AddSpawnPoint(1130,620);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,2675,630,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(2540,520);
   aFlag[1] -> AddSpawnPoint(2500,670);
   aFlag[1] -> AddSpawnPoint(2540,470);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,3415,480,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(3270,710);
   aFlag[2] -> AddSpawnPoint(3560,710);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,4155,630,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(4290,520);
   aFlag[3] -> AddSpawnPoint(4330,670);
   aFlag[3] -> AddSpawnPoint(4290,470);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,5280,580,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(5710,620);
   aFlag[4] -> AddSpawnPoint(6160,610);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag5$",0,2);
   }
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //DM/LMS/CTF/HTF-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS) || FindObject(GCTF) || FindObject(GHTF))
  {
   if(iTeam == 1)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 670; iY = 610; }
    if(!--rand)
     { iX = 1130; iY = 620; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 5710; iY = 620; }
    if(!--rand)
     { iX = 6160; iY = 610; }
   }
   return(1);
  }

  //Startsicht
  iX = 3415; iY = 360;
}