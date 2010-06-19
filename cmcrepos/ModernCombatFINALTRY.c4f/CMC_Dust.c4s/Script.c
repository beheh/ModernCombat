/*-- Dust --*/

#strict
#include CSTD

static aFlag;


/* Initalisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Offensive.ogg;CMC_Deep Universe.ogg;CMC_Friendly Unit.ogg;CMC_Hacking.ogg;CMC_Moving Squad.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Odyssey.ogg;CMC_Rock Go On.ogg;CMC_Techno.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Odyssey.ogg");
  //Lichteinstellung
  SetGamma(RGB(3,1,0),RGB(95,83,68),RGB(213,212,187));
  //Flaggen
  aFlag = [];
  //Szenario einrichten
  CreateFurniture();
  //Ausrüstung plazieren
  CreateEquipment();
  return(1);
}

/* Plazierungslisten */

func CreateFurniture()
{
  Log("$CreatingFurniture$");
  //Alle Objekte folglich von links nach rechts

  //Brückensegment
  CreateObject(BRDG, 1210, 455, -1);

  //Leitern
  CreateObject(LADR, 230, 650, -1)->Set(11);
  CreateObject(LADR, 425, 338, -1)->Set(14);
  CreateObject(LADR, 865, 790, -1)->Set(14);
  CreateObject(LADR, 1090, 690, -1)->Set(31);
  CreateObject(LADR, 1350, 595, -1)->Set(10);

  //Bodenlucken
  CreateObject(HA4K, 1310, 623, -1);
  CreateObject(HA4K, 1430, 653, -1);

  //Große Bodenlucke
  CreateObject(H24K, 425, 228, -1);

  ///Rampe
  DrawMaterialQuad("Wall-Bricks1",1471,730,1441,720,1441,730,1456,730,true);
  DrawMaterialQuad("Wall-Bricks1",1751,410,1721,400,1721,410,1736,410,true);

  //Metallkisten
  CreateObject(MWCR, 160, 680, -1)->AutoRespawn();
  CreateObject(MWCR, 1255, 440, -1)->AutoRespawn();

  //Kisten
  CreateObject(WCR2, 335, 740, -1)->AutoRespawn();
  CreateObject(WCR2, 463, 290, -1)->AutoRespawn();
  CreateObject(WCR2, 790, 450, -1)->AutoRespawn();

  //Explosive Kiste
  CreateObject(XWCR, 1090, 740, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 380, 540, -1)->AutoRespawn();
  CreateObject(PBRL, 390, 540, -1)->AutoRespawn();
  CreateObject(PBRL, 930, 750, -1)->AutoRespawn();
  CreateObject(PBRL, 1390, 720, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 400, 360, -1)->AutoRespawn();
  CreateObject(XBRL, 1405, 720, -1)->AutoRespawn();
  CreateObject(XBRL, 1630, 400, -1)->AutoRespawn();

  //Flutlichter
  CreateObject(FLGH, 455, 630, -1)->SetRotation(30);
  CreateObject(FLGH, 975, 470, -1)->SetRotation(100);

  //Glühbirne
  CreateObject(LBGH, 1340, 645, -1);

  //Gasflasche
  CreateObject(GSBO, 385, 360, -1)->AutoRespawn();

  //Geländer
  CreateObject(RAI1, 370, 480, -1)->SetRail([1]);
  CreateObject(RAI1, 350, 540, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1250, 690, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1360, 720, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1480, 730, -1)->SetRail([1,1]);

  //Büsche
  CreateObject(BSH2, 320, 470, -1);
  CreateObject(BSH2, 650, 560, -1);
  CreateObject(BSH2, 1275, 825, -1);
  CreateObject(BSH2, 1550, 600, -1);
  CreateObject(BSH2, 1730, 350, -1);

  //Steine
  CreateObject(STNE, 540, 380, -1);
  CreateObject(STNE, 1290, 820, -1);

  //Sounds

  //Wind
  CreateObject(SE4K, 945, 890, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 1550, 820, -1)->Set("WindSound*.ogg",775,250);

  //Erdrutsche
  CreateObject(SE4K, 70, 255, -1)->Set("FallingDirt*.ogg",775,250);
  CreateObject(SE4K, 170, 600, -1)->Set("FallingDirt*.ogg",775,250);

  CreateObject(SE4K, 1520, 790, -1)->Set("FallingDirt*.ogg",775,250);
  CreateObject(SE4K, 1740, 290, -1)->Set("FallingDirt*.ogg",775,250);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Munitionskisten (Kugeln)
  var tmp = CreateObject(AMCT, 350, 540, -1);
  tmp->Set(ABOX);
  tmp->SetGraphics("Desert");

  var tmp = CreateObject(AMCT, 1395, 490, -1);
  tmp->Set(ABOX);
  tmp->SetGraphics("Desert");

  //Munitionskiste (Granaten)
  var tmp = CreateObject(AMCT, 1055, 780, -1);
  tmp->Set(GBOX);
  tmp->SetGraphics("Desert");

  //Projektilmunition
  PlaceSpawnpoint(ABOX, 280, 404);
  PlaceSpawnpoint(ABOX, 1255, 805);

  //Raketen
  PlaceSpawnpoint(MIAP, 740, 420);

  //Feldgranaten
  PlaceSpawnpoint(FGRN, 630, 560);
  PlaceSpawnpoint(FGRN, 1155, 575);

  //Blendgranaten
  PlaceSpawnpoint(STUN, 700, 820);
  PlaceSpawnpoint(STUN, 1395, 395);

  //Sturmgewehre
  PlaceSpawnpoint(ASTR, 60, 620);
  PlaceSpawnpoint(ASTR, 1375, 320);

  //Maschinengewehr
  PlaceSpawnpoint(MNGN, 585, 815);

  //Raketenwerfer
  PlaceSpawnpoint(RTLR, 815, 445);

  //Shotgun
  PlaceSpawnpoint(PPGN, 455, 620);

  //Slingshot
  PlaceSpawnpoint(SGST, 1030, 775);

  //Maschinenpistole
  PlaceSpawnpoint(SMGN, 205, 405);

  //Sprengfalle
  PlaceSpawnpoint(BBTP, 450, 215);

  //Abwehrschild
  PlaceSpawnpoint(RSHL, 1565, 740);

  //Dragnin
  PlaceSpawnpoint(DGNN, 935, 825);

  //C4
  PlaceSpawnpoint(C4PA, 830, 655);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   aFlag[0] = CreateObject(OFPL,140,680,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(305, 530);
   aFlag[0] -> AddSpawnPoint(380, 470);
   aFlag[0] -> AddSpawnPoint(470, 770);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,145,320,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(360, 410);
   aFlag[1] -> AddSpawnPoint(480, 280);
   aFlag[1] -> AddSpawnPoint(580, 280);
   if(aTeams[4] == true)
   {
    aFlag[1]->Set("$Flag2$",100,2);
    aFlag[1]->Capture(4,1);
   }
   else
   {
    aFlag[1]->Set("$Flag2$",0,2);
   }

   aFlag[2] = CreateObject(OFPL,630,560,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(780, 550);
   aFlag[2] -> AddSpawnPoint(650, 440);
   aFlag[2] -> AddSpawnPoint(815, 440);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,1005,700,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(910, 820);
   aFlag[3] -> AddSpawnPoint(985, 560);
   aFlag[3] -> AddSpawnPoint(1070, 770);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,1495,730,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(1330, 610);
   aFlag[4] -> AddSpawnPoint(1380, 740);
   aFlag[4] -> AddSpawnPoint(1395, 630);
   if(aTeams[3] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(3,1);
   }
   else
   {
    aFlag[4]->Set("$Flag5$",0,2);
   }

   aFlag[5] = CreateObject(OFPL,1705,400,NO_OWNER);
   aFlag[5] -> AddSpawnPoint(1280, 430);
   aFlag[5] -> AddSpawnPoint(1420, 470);
   aFlag[5] -> AddSpawnPoint(1550, 480);
   if(aTeams[2] == true)
   {
    aFlag[5]->Set("$Flag6$",100,2);
    aFlag[5]->Capture(2,1);
   }
   else
   {
    aFlag[5]->Set("$Flag6$",0,2);
   }
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(aTeams[1] == true)
   {CreateFlag(1,130,680,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,1720,400,GetTeamColor(2));}
   if(aTeams[3] == true)
   {CreateFlag(3,1500,730,GetTeamColor(3));}
   if(aTeams[4] == true)
   {CreateFlag(4,145,320,GetTeamColor(4));}
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //DM/LMS-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS))
  {
   if(iTeam == 1)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 125; iY = 670; }
    if(!--rand)
     { iX = 250; iY = 550; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 1690; iY = 390; }
    if(!--rand)
     { iX = 1545; iY = 480; }
   }
   if(iTeam == 3)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 1500; iY = 720; }
    if(!--rand)
     { iX = 1395; iY = 630; }
   }
   if(iTeam == 4)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 180; iY = 310; }
    if(!--rand)
     { iX = 330; iY = 400; }
   }
   return(1);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   var rand = Random(2);
   if(iTeam == 1)
   {
    if(!rand)
     { iX = 305; iY = 535; }
    if(!--rand)
     { iX = 335; iY = 535; }
   }
   if(iTeam == 2)
   {
    if(!rand)
     { iX = 1380; iY = 405; }
    if(!--rand)
     { iX = 1410; iY = 405; }
   }
   if(iTeam == 3)
   {
    if(!rand)
     { iX = 1220; iY = 780; }
    if(!--rand)
     { iX = 1275; iY = 810; }
   }
   if(iTeam == 4)
   {
    if(!rand)
     { iX = 480; iY = 285; }
    if(!--rand)
     { iX = 580; iY = 285; }
   }
   return(1);
  }

  //Startsicht
  iX = 875; iY = 565;
}