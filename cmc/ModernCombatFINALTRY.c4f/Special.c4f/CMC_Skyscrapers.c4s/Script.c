/*-- Skyscrapers --*/

#strict 2
#include CSTD

func RecommendedGoals()	{return [GLMS];}	//Spielzielempfehlung


/* Regelvoreinstellung */

func ChooserRuleConfig()
{
  //Abgewandelter Regelsatz: Belohnungssystem, Kein FriendlyFire, Fallschaden, Keine Schwerverletzten, Limitierte Ausrüstung
  var array = [RWDS, NOFF, FDMG, SICD, LIMT];
  return(array);
}

/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(1,45,15);
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

  //Leitern
  CreateObject(LADR, 355, 305, -1);
  CreateObject(LADR, 365, 425, -1);
  CreateObject(LADR, 405, 465, -1);
  CreateObject(LADR, 440, 444, -1)->Set(17);
  CreateObject(LADR, 525, 315, -1);
  CreateObject(LADR, 525, 465, -1);
  CreateObject(LADR, 575, 315, -1);
  CreateObject(LADR, 585, 465, -1);
  CreateObject(LADR, 655, 465, -1);
  CreateObject(LADR, 665, 315, -1);
  CreateObject(LADR, 715, 315, -1);
  CreateObject(LADR, 715, 465, -1);
  CreateObject(LADR, 801, 444, -1)->Set(17);
  CreateObject(LADR, 835, 465, -1);
  CreateObject(LADR, 875, 425, -1);
  CreateObject(LADR, 885, 305, -1);

  //Bodenluken
  CreateObject(HA4K,440,303,-1);
  CreateObject(HA4K,800,303,-1);

  //Gerüste
  CreateObject(SFFG, 450, 520, -1)->Set(5);
  CreateObject(SFFG, 450, 570, -1)->Set(5);
  CreateObject(SFFG, 450, 620, -1)->Set(5);
  CreateObject(SFFG, 450, 670, -1)->Set(5);
  CreateObject(SFFG, 450, 720, -1)->Set(5);
  CreateObject(SFFG, 450, 770, -1)->Set(5);
  CreateObject(SFFG, 620, 520, -1)->Set(5);
  CreateObject(SFFG, 620, 570, -1)->Set(5);
  CreateObject(SFFG, 620, 620, -1)->Set(5);
  CreateObject(SFFG, 620, 670, -1)->Set(5);
  CreateObject(SFFG, 620, 720, -1)->Set(5);
  CreateObject(SFFG, 620, 770, -1)->Set(5);
  CreateObject(SFFG, 790, 520, -1)->Set(5);
  CreateObject(SFFG, 790, 570, -1)->Set(5);
  CreateObject(SFFG, 790, 620, -1)->Set(5);
  CreateObject(SFFG, 790, 670, -1)->Set(5);
  CreateObject(SFFG, 790, 720, -1)->Set(5);
  CreateObject(SFFG, 790, 770, -1)->Set(5);

  //Explosivkisten
  CreateObject(XWCR,422,450,-1)->AutoRespawn();
  CreateObject(XWCR,821,450,-1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL,505,300,-1)->AutoRespawn();
  CreateObject(XBRL,585,300,-1)->AutoRespawn();
  CreateObject(XBRL,655,300,-1)->AutoRespawn();
  CreateObject(XBRL,735,300,-1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL,515,300,-1)->AutoRespawn();
  CreateObject(PBRL,595,300,-1)->AutoRespawn();
  CreateObject(PBRL,645,300,-1)->AutoRespawn();
  CreateObject(PBRL,725,300,-1)->AutoRespawn();

  //Grenze
  CreateObject(BRDR, 0, 680, -1)->Set(3,0,1);

  //Soundkulisse

  //Rush Hour
  CreateObject(SE4K, 340, 595, -1)->Set("Traffic*.ogg",500,100, 25,50);
  CreateObject(SE4K, 900, 595, -1)->Set("Traffic*.ogg",500,100, 25,50);

  //Wind
  CreateObject(SE4K, 330, 150, -1)->Set("WindSound*.ogg",1000,500);
  CreateObject(SE4K, 910, 150, -1)->Set("WindSound*.ogg",1000,500);

  //Innenbereich
  CreateObject(SE4K, 300, 370, -1)->Set("Interior*.ogg",700,100);
  CreateObject(SE4K, 620, 370, -1)->Set("InteriorStress*.ogg",1000,300, 75);
  CreateObject(SE4K, 620, 370, -1)->Set("InteriorMetal*.ogg",1000,300, 75);
  CreateObject(SE4K, 940, 370, -1)->Set("Interior*.ogg",700,100);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Glasboxen
  PlaceSpawnpoint(GBOX, 340, 280);
  PlaceSpawnpoint(GBOX, 350, 400);

  PlaceSpawnpoint(GBOX, 620, 290);
  PlaceSpawnpoint(GBOX, 620, 440);

  PlaceSpawnpoint(GBOX, 890, 400);
  PlaceSpawnpoint(GBOX, 900, 280);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Radar
  CreateObject(RADR, 616, 300, -1)->SetClrModulation(RGB(125,125,125));

  //Geländer
  CreateObject(RAI3,430,450,-1);
  CreateObject(RAI3,460,300,-1);
  CreateObject(RAI3,461,450,-1);
  CreateObject(RAI3,491,300,-1);
  CreateObject(RAI3,590,300,-1);
  CreateObject(RAI3,605,450,-1);
  CreateObject(RAI3,621,300,-1);
  CreateObject(RAI3,720,300,-1);
  CreateObject(RAI3,750,450,-1);
  CreateObject(RAI3,751,300,-1);
  CreateObject(RAI3,781,450,-1);

  //Bildschirme
  CreateObject(SCA1,485,400,-1);
  CreateObject(SCA1,755,400,-1)->SetAction("Grenade");

  //Orientierungslichter
  CreateObject(OLGH, 620, 300, -1)->Set(13, 15, 1, 1, 40);
  CreateObject(OLGH, 620, 450, -1)->Set(13, 15, 1, 1, 40);

  //Leitkegel
  CreateObject(TCCN, 325, 290, -1);
  CreateObject(TCCN, 335, 410, -1);
  CreateObject(TCCN, 415, 290, -1);
  CreateObject(TCCN, 515, 450, -1);
  CreateObject(TCCN, 595, 450, -1);
  CreateObject(TCCN, 645, 450, -1);
  CreateObject(TCCN, 725, 450, -1);
  CreateObject(TCCN, 825, 290, -1);
  CreateObject(TCCN, 905, 410, -1);
  CreateObject(TCCN, 915, 290, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 470, 450, -1)->SetCon(60);
  CreateObject(ENGT, 770, 450, -1)->SetCon(60);

  //Straßenlichter
  CreateObject(SLGH, 480, 300, -1);
  CreateObject(SLGH, 620, 300, -1);
  CreateObject(SLGH, 760, 300, -1);
}

/* Bei Ausrüstung */

public func OnClonkEquip(object pClonk)
{
  //Einsatzschild
  CreateContents(RSHL, pClonk);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Infiltration.ogg");

  //MR-Spielziel
  if (FindObject(GMNR))
  {
    //Geldsäcke
    AddMoneySpawn(555, 250, [10]);
    AddMoneySpawn(555, 405, [10]);
    AddMoneySpawn(690, 250, [10]);
    AddMoneySpawn(690, 405, [10]);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 620; iY = 380;
    return 1;
  }

  //MR/LMS-Spielziel
  if(FindObject(GMNR) || FindObject(GLMS))
  {
    return [[450, 440], [490, 440], [750, 440], [460, 290], [780, 290], [790, 440]];
    return 1;
  }
}