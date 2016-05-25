/*-- Broadcast Station --*/

#strict 2
#include CSTD

func RecommendedGoals()	{return [GTDM];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(1,20,15);
  //Bildschirmfärbung
  SetSkyAdjust(RGBa(255,255,255,128),RGB(64,196,255));
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

  //Aufzüge
  CreateObject(LFTP, 845, 510, -1);
  CreateObject(LFTP, 1265, 710, -1);
  var liftp = CreateObject(LFTP, 1500, 194, -1);
  liftp->DisableVertical();

  //Leitern
  CreateObject(LADR, 190, 690, -1)->Set(24);
  CreateObject(LADR, 430, 488, -1)->Set(15);
  CreateObject(LADR, 570, 692, -1)->Set(22);
  CreateObject(LADR, 810, 420, -1)->Set(9);
  CreateObject(LADR, 940, 400, -1)->Set(20);
  CreateObject(LADR, 1025, 315, -1)->Set(20);
  CreateObject(LADR, 1050, 798, -1)->Set(10);
  CreateObject(LADR, 1145, 190, -1)->Set(5);
  CreateObject(LADR, 1360, 572, -1)->Set(33);
  CreateObject(LADR, 1470, 770, -1)->Set(9);
  CreateObject(LADR, 1595, 291, -1)->Set(13);

  //Bodenluken
  CreateObject(HA4K, 190, 493, -1);
  CreateObject(HA4K, 430, 363, -1);
  CreateObject(HA4K, 570, 513, -1);
  CreateObject(HA4K, 810, 343, -1);
  CreateObject(HA4K, 1050, 713, -1);
  CreateObject(HA4K, 1360, 303, -1);

  //Große Bodenluke
  CreateObject(H24K, 1595, 188, -1);

  //Hydrauliktüren
  var door = CreateObject(SLDR, 140, 490, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  door = CreateObject(SLDR,1670, 180, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  //Schutztüren
  CreateObject(GDDR, 690, 510, -1);
  CreateObject(GDDR, 1388, 300, -1);
  CreateObject(GDDR, 1553, 180, -1);

  //Kisten
  CreateObject(WCR2, 30, 710, -1);
  CreateObject(WCR2, 215, 710, -1)->AutoRespawn();
  CreateObject(WCR2, 310, 682, -1);
  CreateObject(WCR2, 475, 500, -1)->AutoRespawn();
  CreateObject(WCR2, 885, 322, -1);

  //Metallkisten
  CreateObject(MWCR, 880, 340, -1)->AutoRespawn();
  CreateObject(MWCR, 1215, 590, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 400, 710, -1)->AutoRespawn();
  CreateObject(BECR, 1275, 140, -1)->AutoRespawn();

  //Explosive Kiste
  CreateObject(XWCR, 710, 710, -1)->AutoRespawn();

  //Container
  CreateObject(CON1, 300, 710, -1)->SetGraphics("4");
  CreateObject(CON1, 359, 710, -1)->SetGraphics("4");
  CreateObject(CON1, 1090, 710, -1)->SetGraphics("4");

  //Giftfässer
  CreateObject(TBRL, 50, 710, -1);
  CreateObject(TBRL, 340, 680, -1)->AutoRespawn();
  CreateObject(TBRL, 420, 710, -1)->AutoRespawn();
  CreateObject(TBRL, 450, 710, -1)->AutoRespawn();
  CreateObject(TBRL, 860, 340, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 1060, 150, -1)->AutoRespawn();
  CreateObject(PBRL, 1315, 300, -1)->AutoRespawn();
  CreateObject(PBRL, 1660, 180, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 1075, 680, -1)->AutoRespawn();
  CreateObject(XBRL, 1380, 580, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 440, 710, -1)->AutoRespawn();
  CreateObject(HBRL, 1130, 710, -1)->AutoRespawn();

  //Palette
  CreateObject(PLLT, 895, 710, -1);

  //Rampen
  DrawMaterialQuad("Backwall-Marble1",311,490,281,480,281,490,296,490,true);
  DrawMaterialQuad("Backwall-Marble1",561,500,531,490,531,500,546,500,true);

  DrawMaterialQuad("Wall-Concrete3",311,500,281,490,281,500,296,500,true);
  DrawMaterialQuad("Wall-Concrete3",561,510,531,500,531,510,546,510,true);
  DrawMaterialQuad("Wall-Concrete3",1300,590,1330,580,1330,590,1315,590,true);
  DrawMaterialQuad("Wall-Concrete3",1290,190,1320,180,1320,190,1305,190,true);

  //Sandsackbarrieren
  CreateObject(SBBA, 635, 710, -1)->Right();
  CreateObject(SBBA, 1125, 590, -1)->Left();

  //Explosivtank
  CreateObject(XTNK, 1170, 590, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 320, 500, -1)->AutoRespawn();
  CreateObject(GSBL, 1180, 710, -1)->AutoRespawn();
  CreateObject(GSBL, 1330, 300, -1)->AutoRespawn();
  CreateObject(GSBL, 1690, 180, -1);

  //Tore
  CreateObject(HNG2, 100, 710, -1);
  CreateObject(HNG2, 170, 710, -1);
  door = CreateObject(HNG2, 590, 710, -1);
  door->Opened();
  door = CreateObject(HNG2, 660, 710, -1);
  door->Opened();

  //Steine
  CreateObject(STNE, 140, 360, -1);
  CreateObject(STNE, 1015, 820, -1)->Set(1);
  CreateObject(STNE, 1580, 345, -1)->Set(2);
  CreateObject(STNE, 1715, 430, -1)->Set(3);
  CreateObject(STNE, 1805, 800, -1);

  //Stahlbrücken
  CreateObject(_HBR, 775, 522, -1)->SwitchMode();
  CreateObject(_HBR, 805, 722, -1)->SwitchMode();
  CreateObject(_HBR, 895, 722, -1)->SwitchMode();
  CreateObject(_HBR, 985, 722, -1)->SwitchMode();

  //Grenze
  CreateObject(BRDR, 0, 860, -1)->Set(3,0,1);

  //Säule
  CreateObject(PILR, 430, 710, -1)->Set("PreparePillarCollapse");

  //Sonne umplatzieren
  FindObject(LENS)->SetPosition(920,125);

  //Sendemast
  var tower = CreateObject(AATR, 970, 710, -1);
  tower->SwitchMode();
  tower->AddNode(459, 353, 0, CreateObject(REHR, 450, 360, -1), 0, 2);
  tower->AddNode(1076, 143, 0, CreateObject(REHR, 1080, 150, -1), 0, 2);
  tower->AddNode(1313, 578, 2, CreateObject(REHR, 1315, 585, -1), -16, 2);
  tower->AddNode(749, 690, 3, CreateObject(REHR, 740, 710, -1));

  //Soundkulisse

  //Vogelzwitschern
  CreateObject(SE4K, 200, 300, -1)->Set("BirdSong*",200,50);
  CreateObject(SE4K, 600, 350, -1)->Set("BirdSong*",200,50);

  //Wipfe
  CreateObject(SE4K, 300, 350, -1)->Set("Snuff*",800,100);

  //Erdrutsche
  CreateObject(SE4K, 1600, 250, -1)->Set("FallingDirt*.ogg",800,200, 50,75);
  CreateObject(SE4K, 1600, 650, -1)->Set("FallingDirt*.ogg",800,200);

  //Lautsprecher
  CreateObject(SE4K, 1000, 500, -1)->Set("Announce*.ogg",3000,500, 75);

  //Wind
  CreateObject(SE4K, 800, 200, -1)->Set("WindSound*.ogg",800,300, 50);

  //Innenbereich
  CreateObject(SE4K, 300, 600, -1)->Set("Interior*.ogg",700,100, 50,75);
  CreateObject(SE4K, 300, 600, -1)->Set("InteriorMetal*.ogg",700,100, 75);
  CreateObject(SE4K, 1400, 500, -1)->Set("InteriorStress*.ogg",700,100, 75);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 370, 683, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 1255, 190, -1);
  crate->Set(ABOX);

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 190, 705);
  PlaceSpawnpoint(GBOX, 1490, 550);

  //Raketen
  PlaceSpawnpoint(MBOX, 335, 395);
  PlaceSpawnpoint(MBOX, 1645, 170);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Leitkegel
  CreateObject(TCCN, 1300, 710, -1);
  CreateObject(TCCN, 1315, 710, -1);
  CreateObject(TCCN, 1330, 710, -1);

  //Raum
  CreateObject(ROM2, 50, 490, -1);

  //Alarmlampen
  CreateObject(ALGH, 905, 340, -1);
  CreateObject(ALGH, 1035, 150, -1)->TurnOn();

  //Radare
  CreateObject(RADR, 500, 360, -1);
  CreateObject(RADR, 1210, 190, -1);

  //Zäune
  CreateObject(FENC, 1070, 710, -1);
  CreateObject(FENC, 1130, 710, -1);
  CreateObject(FENC, 1190, 710, -1);
  CreateObject(FENC, 1340, 710, -1);

  //Regale
  CreateObject(FRAM, 20, 490, -1);
  CreateObject(FRAM, 85, 490, -1);
  CreateObject(FRAM, 105, 490, -1);
  CreateObject(FRAM, 340, 500, -1);
  CreateObject(FRAM, 360, 500, -1);
  CreateObject(FRAM, 380, 500, -1);
  CreateObject(FRAM, 400, 500, -1);
  CreateObject(FRAM, 420, 500, -1);
  CreateObject(FRAM, 440, 500, -1);

  //Rohr
  CreateObject(PIPL, 670, 665, -1)->Right()->Right()->Right()->Right()->Right()->Down()->Down()->Down()->Down()->Down()->Down();

  //Feuerlöscher
  CreateObject(FIE2, 545, 500, -1);
  CreateObject(FIE2, 540, 705, -1);
  CreateObject(FIE2, 1380, 295, -1);

  //Automat
  CreateObject(VGMN, 510, 710, -1);

  //Labortisch
  CreateObject(LTBL, 245, 490, -1);

  //Monitore
  CreateObject(MNI2, 235, 478, -1);
  CreateObject(MNI2, 255, 478, -1)->Off();

  //Flaschen
  CreateObject(BOTL, 1290, 140, -1);
  CreateObject(BOTL, 1295, 140, -1);

  //Papierstapel
  CreateObject(PPSK, 220, 490, -1);
  CreateObject(PPSK, 1275, 190, -1);

  //Apparaturen
  CreateObject(GADG, 660, 510, -1);
  CreateObject(GADG, 1340, 580, -1)->Set(1);
  CreateObject(GADG, 1730, 180, -1)->Set(1);

  //Schilder
  CreateObject(MSGN, 395, 360, -1);
  CreateObject(WLSN, 480, 700, -1)->Set("Bio");
  CreateObject(MSGN, 535, 360, -1);
  CreateObject(MSGN, 765, 710, -1);
  CreateObject(MSGN, 850, 710, -1);
  CreateObject(MSGN, 940, 710, -1);
  CreateObject(MSGN, 1025, 710, -1);

  //Notausgangslichter
  CreateObject(ETLT, 160, 475, -1);
  CreateObject(ETLT, 1635, 165, -1);

  //Scheinwerfer
  CreateObject(FLGH, 475, 360, -1)->SetRotation(-50);
  CreateObject(FLGH, 790, 340, -1)->SetRotation(40);
  CreateObject(FLGH, 1110, 150, -1)->SetRotation(-70);

  //Spinde
  CreateObject(LCKR, 1140, 710, -1);
  CreateObject(LCKR, 1160, 710, -1);
  CreateObject(LCKR, 1400, 300, -1);

  //Topfpflanze
  CreateObject(PLT3, 520, 500, -1);

  //Geländer
  CreateObject(RAI1, 1, 490, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 390, 360, -1)->SetRail([1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 700, 510, -1)->SetRail([1,1]);
  CreateObject(RAI1, 670, 710, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 1110, 590, -1)->SetRail([1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1080, 190, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1730, 180, -1)->SetRail([1,1,1,1,1,1,1,1,1,1]);

  //Bildschirme
  var screen = CreateObject(SCR3, 340, 480, -1);
  screen->SetClrModulation(RGB(255,255,255));
  CreateObject(SCR3, 390, 480, -1);
  screen = CreateObject(SCR3, 485, 480, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR3, 630, 485, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR3, 1770, 170, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR3, 1860, 170, -1);
  screen->SetClrModulation(RGB(255,255,255));

  //Ventillatoren
  CreateObject(MVNT, 135, 700, -1)->SetCon(80);
  CreateObject(MVNT, 625, 700, -1)->SetCon(80);

  //Anlagen
  CreateObject(CCP2, 75, 710, -1);
  CreateObject(CGLO, 630, 510, -1);
  CreateObject(CCP1, 1170, 190, -1);

  //Büsche
  CreateObject(BSH2, 115, 340, -1);
  CreateObject(BSH2, 315, 325, -1);
  CreateObject(BSH2, 530, 385, -1);
  CreateObject(BSH2, 710, 555, -1);
  CreateObject(BSH2, 1150, 245, -1);
  CreateObject(BSH2, 1190, 830, -1);
  CreateObject(BSH2, 1325, 520, -1);
  CreateObject(BSH2, 1400, 440, -1);
  CreateObject(BSH2, 1490, 565, -1);
  CreateObject(BSH2, 1600, 365, -1);

  //Dekoschleusen
  CreateObject(GAT1, 845, 590, -1);
  CreateObject(GAT1, 1700, 175, -1);

  //Deckenlampen
  CreateObject(CLGH, 40, 625, -1);
  CreateObject(CLGH, 100, 465, -1);
  CreateObject(CLGH, 200, 465, -1);
  CreateObject(CLGH, 280, 625, -1);
  CreateObject(CLGH, 300, 465, -1);
  CreateObject(CLGH, 340, 625, -1);
  CreateObject(CLGH, 400, 625, -1);
  CreateObject(CLGH, 460, 625, -1);
  CreateObject(CLGH, 575, 465, -1);
  CreateObject(CLGH, 1230, 655, -1);
  CreateObject(CLGH, 1300, 655, -1);
  CreateObject(CLGH, 1360, 275, -1);
  CreateObject(CLGH, 1595, 155, -1);
  CreateObject(CLGH, 1815, 155, -1);
}

/* Bei Säulenzerstörungen */

func PreparePillarCollapse(int iPlr)
{
  //Spielerbildschirme schütteln
  ShakeViewPort(100);

  //Effekte
  AddParticlesInRect("Smoke3",[10,20],[240,590,280,30],[10,30],[200,400]);
  AddParticlesInRect("Smoke",[10,20],[240,620,280,10],[10,25],[200,400]);

  //Lampen ausschalten
  for(var obj in FindObjects(Find_ID(CLGH), Find_InRect(270,620,200,10)))
  {
    obj->Damage();
    obj->TurnOff();
  }

  //Zerstörung planen
  Schedule(Format("GameCall(\"OnPillarCollapse\", %d)", iPlr), 200);
}

func OnPillarCollapse(int iPlr)
{
  //Effekte
  var effect = CreateObject(TIM1,380,610);
  effect->ShakeViewPort(800, this);
  effect->Sound("StructureHit*.ogg");
  effect->Schedule("RemoveObject()",100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,280,610,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,340,610,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,400,610,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,460,610,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,520,610,60,100);

  //Hintergrund zeichnen
  DrawMaterialQuad("BackWall-Metal4",240,590,520,590,520,621,240,621,true);
  DrawMaterialQuad("BackWall-Column1",420,590,440,590,440,621,420,621,true);

  //Lebewesen verletzen
  for(var obj in FindObjects(Find_OCF(OCF_Alive), Find_InRect(240,620,280,10)))
    DoDmg(200, DMG_Explosion, obj, 0, iPlr + 1);

  //Lampen entfernen
  for(var obj in FindObjects(Find_ID(CLGH), Find_InRect(270,620,200,10)))
    RemoveObject(obj);

  //Trümmer verschleudern
  var debris = CreateObject(DBRS, 280,630, iPlr);
  Fling(debris, RandomX(-4,4), RandomX(2,3));
  debris = CreateObject(DBRS, 320,630, iPlr);
  Fling(debris, RandomX(-4,4), RandomX(2,3));
  debris = CreateObject(DBRS, 380,630, iPlr);
  debris->Sound("StructureIntegrity*.ogg");
  Fling(debris, RandomX(-4,4), RandomX(2,3));
  debris = CreateObject(DBRS, 440,630, iPlr);
  Fling(debris, RandomX(-4,4), RandomX(2,3));
  debris = CreateObject(DBRS, 480,630, iPlr);
  Fling(debris, RandomX(-4,4), RandomX(2,3));
}

/* Bei Turmzusammenfall */

func OnTowerCollapse()
{
  EventInfo4K(0,Format("$MsgCollapse$"),AATR, 0, 0, 0, "Info_Event.ogg");
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Deep Universe.ogg");

  //MR-Spielziel
  if(FindObject(GMNR))
  {
    //Geldsäcke
    AddMoneySpawn(490, 355, [20]);
    AddMoneySpawn(850, 705, [20]);
    AddMoneySpawn(1090, 145, [20]);
    AddMoneySpawn(1320, 845, [20]);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 875; iY = 630;
    return 1;
  }

  //MR/LMS/DM-Spielziel
  if(FindObject(GMNR) || FindObject(GLMS) || FindObject(GTDM))
  {
    //Gesonderte Spawnpoints bei 2 Teams
    if(GetActiveTeamCount() == 2)
    {
      if(iTeam == GetActiveTeamByIndex(0))
        return [[210, 480], [260, 700], [520, 490]];
      if(iTeam == GetActiveTeamByIndex(1))
        return [[1535, 300], [1635, 240], [1690, 420]];
      return 1;
    }
    else
      return [[210, 480], [260, 700], [595, 410], [1155, 180], [1525, 650], [1635, 240], [1690, 420]];
  }
}