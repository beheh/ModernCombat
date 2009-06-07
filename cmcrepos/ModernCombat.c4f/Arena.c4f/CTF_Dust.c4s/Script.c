/*-- Dust --*/

#strict
#include CSTD


func Initialize()
{
  //Lichteinstellung
  SetGamma(RGB(3,1,0),RGB(95,83,68),RGB(213,212,187));
  SetSkyParallax(0,33,57,0,0,0,0); 
  //Nebel
  for(var i=0;i<180;++i) 
    CreateParticle("Fog",Random(LandscapeWidth()),Random(LandscapeHeight()),0,0,RandomX(900,1700));
  //Musiktitel, welches für dieses Szenario bestimmt ist, zuerst abspielen
  Music("Commander Fights");
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
  CreateObject(LADR, 425, 335, -1)->Set(14);
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

  //Kisten
  CreateObject(WCR2, 335, 740, -1)->AutoRespawn();
  CreateObject(WCR2, 463, 290, -1)->AutoRespawn();
  CreateObject(WCR2, 794, 450, -1)->AutoRespawn();

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
  var tmp = CreateObject(ABOX, 350, 540, -1);
  tmp->Set(AMOC);
  tmp->SetGraphics("Desert");

  var tmp = CreateObject(ABOX, 1395, 490, -1);
  tmp->Set(AMOC);
  tmp->SetGraphics("Desert");

  //Munitionskiste (Raketen)
  var tmp = CreateObject(ABOX, 1055, 780, -1);
  tmp->Set(MIAP);
  tmp->SetGraphics("Desert");

  //Projektilmunition
  PlaceSpawnpoint(AMOC, 280, 404);
  PlaceSpawnpoint(AMOC, 990, 700);

  //Feldgranaten
  PlaceSpawnpoint(FGRN, 630, 560);
  PlaceSpawnpoint(FGRN, 1155, 575);

  //Blendgranaten
  PlaceSpawnpoint(STUN, 700, 820);
  PlaceSpawnpoint(STUN, 1395, 395);

  //PzF
  PlaceSpawnpoint(PZFW, 205, 405);

  //Pumpgun
  PlaceSpawnpoint(PPGN, 455, 620);

  //P29
  PlaceSpawnpoint(P29W, 1270, 680);

  //Abwehrschild
  PlaceSpawnpoint(RSHL, 1565, 740);

  //MP7
  PlaceSpawnpoint(MP7R, 60, 620);

  //M16
  PlaceSpawnpoint(M16A, 1375, 320);

  //Predator
  PlaceSpawnpoint(PDTW, 650, 445);
}


/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  var rand = Random(2);
  if(iTeam == 1)
  {
    if(!rand)
      { iX = 370; iY = 350; }
    if(!--rand)
      { iX = 400; iY = 210; }
  }
  else
  {
    if(!rand)
      { iX = 1395; iY = 630; }
    if(!--rand)
      { iX = 1395; iY = 710; }
  }
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //FDM-Spielziel
  if(FindObject(GFDM))
  {
    CreateFDMBase(830, 660);
  }

  //HtF-Spielziel
  if(FindObject(GHTF))
  {
    CreateHTFBase(830, 660);
  }
  
  //CtF-Spielziel
  if(FindObject(GCTF))
  {
    CreateFlag(1,130,680,GetTeamColor(1)); 
    CreateFlag(2,1720,400,GetTeamColor(2)); 
  }
}