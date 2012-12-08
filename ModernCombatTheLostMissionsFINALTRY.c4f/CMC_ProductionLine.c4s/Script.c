/*-- Production Line --*/

#strict
#include CSTD

static aFlag, aSelfDefense;

func RecommendedGoals()	{return [GCTF, GLMS];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetPlayList("CMC_WaitingforSpawn2.ogg");
  Music("CMC_WaitingforSpawn2.ogg",1);
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
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

  //Rampen
  DrawMaterialQuad("Wall-Unknown1",361,240,331,230,331,240,346,240,true);
  DrawMaterialQuad("Wall-Unknown1",521,250,491,240,491,250,506,250,true);
  DrawMaterialQuad("Wall-Stripes",560,251,590,241,590,251,575,251,true);
  DrawMaterialQuad("Wall-Stripes",1181,250,1151,240,1151,250,1166,250,true);
  DrawMaterialQuad("Wall-Stripes",1220,250,1250,240,1250,250,1235,250,true);
  DrawMaterialQuad("Wall-Stripes",1841,250,1811,240,1811,250,1826,250,true);
  DrawMaterialQuad("Wall-Unknown1",1880,250,1910,240,1910,250,1895,250,true);
  DrawMaterialQuad("Wall-Unknown1",2040,240,2070,230,2070,240,2055,240,true);

  //Leitern
  CreateObject(LADR, 195, 345, -1)->Set(17);
  CreateObject(LADR, 340, 158, -1)->Set(5);
  CreateObject(LADR, 545, 320, -1)->Set(8);
  CreateObject(LADR, 1200, 320, -1)->Set(9);
  CreateObject(LADR, 1855, 320, -1)->Set(8);
  CreateObject(LADR, 2060, 158, -1)->Set(5);
  CreateObject(LADR, 2205, 345, -1)->Set(17);

  //Bodenlucken
  CreateObject(HA4K, 340, 113, -1);
  CreateObject(HA4K, 2060, 113, -1);

  //Große Bodenlucken
  CreateObject(H24K, 545, 258, -1);
  CreateObject(H24K, 1855, 258, -1);

  //Sandsäcke
  CreateObject (SBBA, 390, 170, -1)->Right();
  CreateObject (SBBA, 480, 240, -1)->Right();
  CreateObject (SBBA, 1920, 240, -1);
  CreateObject (SBBA, 2010, 170, -1);

  //Fließbänder
  var pro = CreateObject (PF1C, 570, 390, -1);
  pro->CreateProductionLine(29);
  pro->SetActions("Right");
  var pro = CreateObject (PF1C, 710, 260, -1);
  pro->CreateProductionLine(6);
  pro->SetActions("Left");
  var pro = CreateObject (PF1C, 1010, 260, -1);
  pro->CreateProductionLine(7);
  pro->SetActions("Left");

  var pro = CreateObject (PF1C, 1390, 260, -1);
  pro->CreateProductionLine(7);
  pro->SetActions("Right");
  var pro = CreateObject (PF1C, 1690, 260, -1);
  pro->CreateProductionLine(6);
  pro->SetActions("Right");
  var pro = CreateObject (PF1C, 1830, 390, -1);
  pro->CreateProductionLine(29);
  pro->SetActions("Left");

  //Sequiumtüren
  CreateObject (SEDR, 37, 170, -1)->Lock();
  CreateObject (SEDR, 2368, 170, -1)->Lock();

  //Schutztüren
  CreateObject(GDDR, 220, 110, -1);
  CreateObject(GDDR, 220, 170, -1);
  CreateObject(GDDR, 230, 230, -1);
  CreateObject(GDDR, 330, 230, -1);
  CreateObject(GDDR, 440, 110, -1);
  CreateObject(GDDR, 1960, 110, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 2070, 230, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 2170, 230, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 2180, 110, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 2180, 170, -1)->SetColorDw(HSL(40, 210, 100, 127));

  //Explosivtanks
  CreateObject(XTNK, 450, 240, -1);
  CreateObject(XTNK, 1950, 240, -1);

  //Grenze
  CreateObject(BRDR, 0, 440, -1)->Set(3,0,1);

  //Selbstschussanlagen und Konsolen
  aSelfDefense[0] = CreateObject(SEGU, 550, 99, -1);
    aSelfDefense[0]->Arm(MISA);
    aSelfDefense[0]->SetAutoRepair(1500);
    CreateObject(CONS, 105, 275, -1)->Set(aSelfDefense[0]);
  
  aSelfDefense[1] = CreateObject(SEGU, 1850, 99, -1);
    aSelfDefense[1]->Arm(MISA);
    aSelfDefense[1]->SetAutoRepair(1500);
    CreateObject(CONS, 2295, 275, -1)->Set(aSelfDefense[1]);

  //Sounds

  //Hallen
  CreateObject(SE4K, 195, 260, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 850, 240, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1550, 240, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2205, 260, -1)->Set("Interior*.ogg",665,105);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 245, 230, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 2155, 230, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Dragnin)
  crate = CreateObject (AMCT, 60, 170, -1);
  crate->Set(DGNN);
  crate = CreateObject (AMCT, 2340, 170, -1);
  crate->Set(DGNN);

  //Raketen
  PlaceSpawnpoint(MBOX, 845, 95);
  PlaceSpawnpoint(MBOX, 1555, 95);

  //Automaten
  var store = CreateObject(WPVM,170, 170,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(BWTH,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(ATWN,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
  store->AddWare(SRBL,-1);
  store = CreateObject(WPVM,2230, 170,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(BWTH,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(ATWN,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
  store->AddWare(SRBL,-1);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Zäune
  CreateObject(FENC, 32, 390, -1)->Set(2);
  CreateObject(FENC, 96, 390, -1)->Set(2);
  CreateObject(FENC, 2304, 390, -1)->Set(2);
  CreateObject(FENC, 2368, 390, -1)->Set(2);

  //Lüftungsgitter
  CreateObject (ENGT, 97, 121, -1)->SetCon(70);
  CreateObject (ENGT, 240, 380, -1);
  CreateObject (ENGT, 340, 380, -1);
  CreateObject (ENGT, 440, 380, -1);
  CreateObject (ENGT, 670, 380, -1);
  CreateObject (ENGT, 770, 380, -1);
  CreateObject (ENGT, 845, 260, -1);
  CreateObject (ENGT, 920, 380, -1);
  CreateObject (ENGT, 1020, 380, -1);
  CreateObject (ENGT, 1380, 380, -1);
  CreateObject (ENGT, 1480, 380, -1);
  CreateObject (ENGT, 1555, 260, -1);
  CreateObject (ENGT, 1630, 380, -1);
  CreateObject (ENGT, 1740, 380, -1);
  CreateObject (ENGT, 1970, 380, -1);
  CreateObject (ENGT, 2070, 380, -1);
  CreateObject (ENGT, 2165, 380, -1);
  CreateObject (ENGT, 2304, 121, -1)->SetCon(70);

  //Ventilatoren
  CreateObject (VENT, 521, 166, -1)->SetCon(40);
  CreateObject (VENT, 717, 166, -1)->SetCon(40);
  CreateObject (VENT, 981, 166, -1)->SetCon(40);
  CreateObject (VENT, 1423, 166, -1)->SetCon(40);
  CreateObject (VENT, 1692, 166, -1)->SetCon(40);
  CreateObject (VENT, 1846, 166, -1)->SetCon(40);

  //Wandlampen
  CreateObject(BLGH, 140, 120, -1);
  CreateObject(BLGH, 845, 190, -1);
  CreateObject(BLGH, 1200, 240, -1);
  CreateObject(BLGH, 1555, 190, -1);
  CreateObject(BLGH, 2265, 120, -1);

  //Radarbildschirme
  CreateObject(SCR1, 450, 210, -1);
  CreateObject(SCR1, 1960, 210, -1);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[0])
   aSelfDefense[0]->SetTeam(iTeam);

  if(pPoint == aFlag[2])
   aSelfDefense[1]->SetTeam(iTeam);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Script starten
  ScriptGo(1);

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Battle Mode.mid;CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Battle Mode.mid",1);

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
    if(aTeams[1] == true)
    {CreateFlag(1,410,240,GetTeamColor(1));}
    if(aTeams[2] == true)
    {CreateFlag(2,1990,240,GetTeamColor(2));}
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
    //Flaggen
    aFlag[0] = CreateObject(OFPL,110,170,NO_OWNER);
    aFlag[0] -> AddSpawnPoint(390,100);
    aFlag[0] -> AddSpawnPoint(400,230);
    aFlag[0] -> AddSpawnPoint(145,270);
    if(aTeams[1] == true)
    {
      aFlag[0]->Set("$Flag1$",100,2);
      aFlag[0]->Capture(1,1);
    }
    else
    {
      aFlag[0]->Set("$Flag1$",0,2);
    }

    aFlag[1] = CreateObject(OFPL,1200,160,NO_OWNER);
    aFlag[1] -> AddSpawnPoint(1150,310);
    aFlag[1] -> AddSpawnPoint(1250,310);
    aFlag[1] -> AddSpawnPoint(1160,232);
    aFlag[1] -> AddSpawnPoint(1240,232);
    aFlag[1]->Set("$Flag2$",0,2);

    aFlag[2] = CreateObject(OFPL,2300,170,NO_OWNER);
    aFlag[2] -> AddSpawnPoint(2010,100);
    aFlag[2] -> AddSpawnPoint(2000,230);
    aFlag[2] -> AddSpawnPoint(2255,270);
    if(aTeams[2] == true)
    {
      aFlag[2]->Set("$Flag3$",100,2);
      aFlag[2]->Capture(2,1);
    }
    else
    {
      aFlag[2]->Set("$Flag3$",0,2);
    }

    //SSA Besitzer setzen
    if(aTeams[1] == true)
    {aSelfDefense[0]->SetTeam(1);}
    if(aTeams[2] == true)
    {aSelfDefense[1]->SetTeam(2);}

    //SSA anschalten
    aSelfDefense[0]->TurnOn();
    aSelfDefense[1]->TurnOn();
  }
}

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //DM/LMS/CTF-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS) || FindObject(GCTF))
  {
   if(iTeam == 1)
   {
    return [[140, 160], [270, 100], [400, 100]];
   }
   if(iTeam == 2)
   {
    return [[2265, 160], [2130, 100], [2000, 100]];
   }
   return(1);
  }

  //Startsicht
  iX = 1200; iY = 245;
}

/* Objekterstellung */

protected func Script50()
{
  //Zufällige Objekte
  CreateObject(RndObjectID(), 60, 370, -1);
  CreateObject(RndObjectID(), 100, 370, -1);
  CreateObject(RndObjectID(), 2300, 370, -1);
  CreateObject(RndObjectID(), 2340, 370, -1);
}

protected func Script100()
{
  //Zufällige Objekte
  CreateObject(RndObjectID(), 60, 370, -1);
  CreateObject(RndObjectID(), 2340, 370, -1);
}

protected func Script150()
{
  //Zufällige Objekte
  CreateObject(RndObjectID(), 100, 370, -1);
  CreateObject(RndObjectID(), 2300, 370, -1);

  goto(0);
}

protected func RndObjectID()
{
 var r = Random(30);
 if (!r)   return(MNI2);
 if (!--r) return(BOTL);
 if (!--r) return(RDIO);

 r = Random(10);
 if (!r)   return(XBRL);
 if (!--r) return(GSBL);
 if (!--r) return(PBRL);
 if (!--r) return(HBRL);
 if (!--r) return(TBRL);
 if (!--r) return(BECR);
 if (!--r) return(WCR2);
 if (!--r) return(XWCR);
 return(MWCR);
}