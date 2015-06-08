/*-- Cold Cash --*/

#strict
#include CSTD

static aFlag;


/* Initialisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg");
  Music("CMC_Moving Squad.ogg");
  //Bildschirmfärbung
  SetGamma(RGB(0,0,0), RGB(80,80,80), RGB(200,200,200));
  //Hintergrundbewegung
  SetSkyParallax(0,15,15);
  //Flaggen
  aFlag = [];
  //Szenario einrichten
  CreateFurniture();
  //Ausrüstung plazieren
  CreateEquipment();
  //Rampen malen
  DrawRamps();
  return(1);
}

/* Plazierungslisten */

func CreateFurniture()
{
  Log("$CreatingFurniture$");
  
  //Lichter
  CreateObject(LLGH,424,152,-1);
  CreateObject(BLGH,345,182,-1);
  CreateObject(LLGH,465,173,-1);
  CreateObject(LLGH,506,192,-1);
  CreateObject(STLH,557,159,-1);
  CreateObject(CLGH,452,230,-1);
  CreateObject(LLGH,560,272,-1);
  CreateObject(LLGH,462,350,-1);
  CreateObject(LLGH,560,351,-1);
  CreateObject(BLGH,481,446,-1);
  CreateObject(ETLT,286,467,-1);
  CreateObject(BLGH,345,342,-1);

  CreateObject(LLGH,LandscapeWidth()-424,152,-1);
  CreateObject(BLGH,LandscapeWidth()-345,182,-1);
  CreateObject(LLGH,LandscapeWidth()-465,173,-1);
  CreateObject(LLGH,LandscapeWidth()-506,192,-1);
  CreateObject(STLH,LandscapeWidth()-557,159,-1);
  CreateObject(CLGH,LandscapeWidth()-452,230,-1);
  CreateObject(LLGH,LandscapeWidth()-560,272,-1);
  CreateObject(LLGH,LandscapeWidth()-462,350,-1);
  CreateObject(LLGH,LandscapeWidth()-560,351,-1);
  CreateObject(BLGH,LandscapeWidth()-481,446,-1);
  CreateObject(ETLT,LandscapeWidth()-286,467,-1);
  CreateObject(BLGH,LandscapeWidth()-345,342,-1);
  
  //Alarmlichter
  CreateObject(ALGH,511,98,-1)->SetR(180);
  CreateObject(ALGH,521,238,-1)->SetR(180);
  CreateObject(ALGH,432,319,-1)->SetR(180);
  CreateObject(ALGH,310,447,-1)->SetR(180);
  CreateObject(ALGH,384,446,-1)->SetR(180);
  CreateObject(ALGH,567,463,-1)->SetR(-90);
  CreateObject(ALGH,614,463,-1)->SetR(90);

  CreateObject(ALGH,LandscapeWidth()-511,98,-1)->SetR(180);
  CreateObject(ALGH,LandscapeWidth()-521,238,-1)->SetR(180);
  CreateObject(ALGH,LandscapeWidth()-432,319,-1)->SetR(180);
  CreateObject(ALGH,LandscapeWidth()-310,447,-1)->SetR(180);
  CreateObject(ALGH,LandscapeWidth()-384,446,-1)->SetR(180);
  CreateObject(ALGH,LandscapeWidth()-567,463,-1)->SetR(90);
  CreateObject(ALGH,LandscapeWidth()-614,463,-1)->SetR(-90);

  //Fensterscheiben (uiui)
  CreateObject(_WIN,588,124,-1);
  CreateObject(_WIN,588,153,-1);
  CreateObject(_WIN,593,153,-1);
  CreateObject(_WIN,593,124,-1);
  CreateObject(_WIN,2913,124,-1);
  CreateObject(_WIN,2913,153,-1);
  CreateObject(_WIN,2908,153,-1);
  CreateObject(_WIN,2908,124,-1);
  CreateObject(_WIN,588,182,-1);
  CreateObject(_WIN,588,211,-1);
  CreateObject(_WIN,593,211,-1);
  CreateObject(_WIN,593,182,-1);
  CreateObject(_WIN,2913,182,-1);
  CreateObject(_WIN,2913,211,-1);
  CreateObject(_WIN,2908,211,-1);
  CreateObject(_WIN,2908,182,-1);
  CreateObject(_WIN,593,262,-1);
  CreateObject(_WIN,588,262,-1);
  CreateObject(_WIN,2908,262,-1);
  CreateObject(_WIN,2913,262,-1);
  CreateObject(_WIN,593,291,-1);
  CreateObject(_WIN,588,291,-1);
  CreateObject(_WIN,2908,291,-1);
  CreateObject(_WIN,2913,291,-1);
  CreateObject(_WIN,588,342,-1);
  CreateObject(_WIN,593,342,-1);
  CreateObject(_WIN,2913,342,-1);
  CreateObject(_WIN,2908,342,-1);
  CreateObject(_WIN,588,371,-1);
  CreateObject(_WIN,593,371,-1);
  CreateObject(_WIN,2913,371,-1);
  CreateObject(_WIN,2908,371,-1);
  
  //Kräne
  var crane1 = CreateObject(CRN1,781,99,-1);
  crane1->Set(20);
  var crane2 = CreateObject(CRN1,1268,99,-1);
  crane2->Set(30);
  var crane3 = CreateObject(CRN1,LandscapeWidth()-781,99,-1);
  crane3->Set(20);
  var crane4 = CreateObject(CRN1,LandscapeWidth()-1268,99,-1);
  crane4->Set(30);
  
  //Türen
  CreateObject(GDDR,380,150,-1)->SetColorDw(GetTeamColor(1));
  CreateObject(GDDR,381,290,-1)->SetColorDw(GetTeamColor(1));
  CreateObject(GDDR,380,370,-1)->SetColorDw(GetTeamColor(1));
  CreateObject(GDDR,LandscapeWidth()-380,150,-1)->SetColorDw(GetTeamColor(2));
  CreateObject(GDDR,LandscapeWidth()-381,290,-1)->SetColorDw(GetTeamColor(2));
  CreateObject(GDDR,LandscapeWidth()-380,370,-1)->SetColorDw(GetTeamColor(2));
  
  //Bildschirme
  CreateObject(SCA2,511,168,-1);
  CreateObject(SCR3,511,346,-1);
  CreateObject(SCA2,LandscapeWidth()-511,168,-1);
  CreateObject(SCR3,LandscapeWidth()-511,346,-1);
  
  //Lüftung
  CreateObject(ENGT,1050,147,-1);
  CreateObject(ENGT,1465,146,-1);
  CreateObject(ENGT,2037,146,-1);
  CreateObject(ENGT,2453,144,-1);
  CreateObject(ENGT,538,453,-1);
  CreateObject(ENGT,LandscapeWidth()-538,453,-1);
  
  //Leitern
  CreateObject(LADR,870,385,-1)->Set(15);
  CreateObject(LADR,1200,385,-1)->Set(15);
  CreateObject(LADR,660,515,-1)->Set(15);
  CreateObject(LADR,990,515,-1)->Set(15);
  CreateObject(LADR,1080,515,-1)->Set(15);
  CreateObject(LADR,1430,515,-1)->Set(15);
  
  CreateObject(LADR,LandscapeWidth()-870,385,-1)->Set(15);
  CreateObject(LADR,LandscapeWidth()-1200,385,-1)->Set(15);
  CreateObject(LADR,LandscapeWidth()-660,515,-1)->Set(15);
  CreateObject(LADR,LandscapeWidth()-990,515,-1)->Set(15);
  CreateObject(LADR,LandscapeWidth()-1080,515,-1)->Set(15);
  CreateObject(LADR,LandscapeWidth()-1430,515,-1)->Set(15);
  
  //Liftplatten
  CreateObject(LFTP,345,411,-1)->SetAutoLimits();
  CreateObject(LFTP,LandscapeWidth()-345,411,-1)->SetAutoLimits();

  //Sentryguns
  var segu1 = CreateObject(SEGU,408,432,-1);
  segu1->SetR(-90);
  segu1->SetTeam(1);
  segu1->Arm(MISA);
  segu1->TurnOn();
  var segu2 = CreateObject(SEGU,LandscapeWidth()-408,432,-1);
  segu2->SetR(90);
  segu2->SetTeam(2);
  segu2->Arm(MISA);
  segu2->TurnOn();
  
  //Türen
  CreateObject(ROOM,490,290,-1)->Connect(CreateObject(ROOM,420,370,-1));
  CreateObject(ROOM,520,290,-1)->Connect(CreateObject(ROOM,525,210,-1));
  CreateObject(ROOM,LandscapeWidth()-490,290,-1)->Connect(CreateObject(ROOM,LandscapeWidth()-420,370,-1));
  CreateObject(ROOM,LandscapeWidth()-520,290,-1)->Connect(CreateObject(ROOM,LandscapeWidth()-525,210,-1));

  //Konsolen
  CreateObject(CONS,545,202,-1)->Set(crane1);
  CreateObject(CONS,569,202,-1)->Set(crane2);
  CreateObject(CONS,451,362,-1)->Set(segu1);
  CreateObject(CONS,LandscapeWidth()-545,202,-1)->Set(crane3);
  CreateObject(CONS,LandscapeWidth()-569,202,-1)->Set(crane4);
  CreateObject(CONS,LandscapeWidth()-451,362,-1)->Set(segu2);

  //Sounds
  CreateObject(SE4K, 500, 560, -1)->Set("Interior*.ogg",105,35);
  CreateObject(SE4K, 1500, 440, -1)->Set("WindSound*.ogg",665,105);
  CreateObject(SE4K, 2770, 540, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1120, 450, -1)->Set("Interior*.ogg",245,105);
  CreateObject(SE4K, 2180, 450, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 3360, 450, -1)->Set("Interior*.ogg",245,105);
  CreateObject(SE4K, 443, 450, -1)->Set("Interior*.ogg",245,105);

}

func CreateEquipment()
{
  var obj;
  
  //Waffenautomaten
  CreateObject(WPVM,454,290,-1);
  CreateObject(WPVM,LandscapeWidth()-454,290,-1);
  
  //Munitionskisten
  CreateObject(AMCT,415,288,-1)->Set(MBOX);
  CreateObject(AMCT,548,288,-1)->Set(GBOX);
  CreateObject(AMCT,568,288,-1)->Set(ABOX);
  CreateObject(AMCT,LandscapeWidth()-415,288,-1)->Set(MBOX);
  CreateObject(AMCT,LandscapeWidth()-548,288,-1)->Set(GBOX);
  CreateObject(AMCT,LandscapeWidth()-568,288,-1)->Set(ABOX);
  
  //Granatenkisten
  obj = CreateObject(AMCT,485,368,-1);
  obj->Set(FRAG);
  SetGraphics("Normal",obj);
  obj = CreateObject(AMCT,504,368,-1);
  obj->Set(STUN);
  SetGraphics("Normal",obj);
  obj = CreateObject(AMCT,523,368,-1);
  obj->Set(FGRN);
  SetGraphics("Normal",obj);
  obj = CreateObject(AMCT,541,368,-1);
  obj->Set(SGRN);
  SetGraphics("Normal",obj);
  obj = CreateObject(AMCT,560,368,-1);
  obj->Set(PGRN);
  SetGraphics("Normal",obj);
  
  obj = CreateObject(AMCT,LandscapeWidth()-485,368,-1);
  obj->Set(FRAG);
  SetGraphics("Normal",obj);
  obj = CreateObject(AMCT,LandscapeWidth()-504,368,-1);
  obj->Set(STUN);
  SetGraphics("Normal",obj);
  obj = CreateObject(AMCT,LandscapeWidth()-523,368,-1);
  obj->Set(FGRN);
  SetGraphics("Normal",obj);
  obj = CreateObject(AMCT,LandscapeWidth()-541,368,-1);
  obj->Set(SGRN);
  SetGraphics("Normal",obj);
  obj = CreateObject(AMCT,LandscapeWidth()-560,368,-1);
  obj->Set(PGRN);
  SetGraphics("Normal",obj);
  
  //Und jetzt, omg, die Kiste um die es sich dreht!
  CreateObject(CMSN,LandscapeWidth()/2,460,-1);
  
  Log("$CreatingEquipment$");
}

public func DrawRamps()
{
  DrawMaterialQuad("Wall-Plate",390,150,510,210,390,210,390,210);
  DrawMaterialQuad("Wall-Stripes",580,540,600,540,610,550,570,550);
  DrawMaterialQuad("Wall-Plate",3110,150,2990,210,3110,210,3110,210);
  DrawMaterialQuad("Wall-Stripes",2900,540,2920,540,2930,550,2890,550);
}

/* Regelwähler */

public func ChooserFinished()
{
  //"Keine Munition"-Regel
  if(FindObject(NOAM))
  {
    //Munitionsspawnpoints entfernen
    for(var spwn in FindObjects(Find_ID(SPNP)))
      if(Contents(0, spwn)->~IsAmmoPacket())
        RemoveObject(spwn);

    //Munitionskisten mit Munition entfernen
    for(var ammobox in FindObjects(Find_ID(AMCT)))
      if(ammobox->GetSpawnID())
        if(ammobox->GetSpawnID()->~IsAmmoPacket())
          RemoveObject(ammobox);
  }
  //Effektmanager erstellen
  CreateObject(EFMN);

  //Ohne Klassenwahl
  if(!FindObject(MCSL))
  {
    for(var i = 0; i < GetPlayerCount(); i++)
      for(var j = 0, pCrew; pCrew = GetCrew(GetPlayerByIndex(i), j) ; j++)
        GameCallEx("OnClassSelection",pCrew);
  }

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(aTeams[1] == true)
   {CreateFlag(1,470,550,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,LandscapeWidth()-470,550,GetTeamColor(2));}
  }

  //HTF-Spielziel
  if (FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, LandscapeWidth()/2, 460, -1);
   flag->~Set("$Flag2$");
  }

  //Base Assault-Spielziel
  if(FindObject(GBAS))
  {
   //Zielobjekte
   AddAssaultTarget(CMSN, 470, 550, 500, 1, Format("%s - %s",GetName(0,CMSN),GetTeamName(1)), 0, [[520, 280], [530, 360]]);

   AddAssaultTarget(CMSN, LandscapeWidth()-470, 550, 500, 2, Format("%s - %s",GetName(0,CMSN),GetTeamName(2)), 1, [[LandscapeWidth()-520, 280], [LandscapeWidth()-530, 360]]);
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggen
   aFlag[0] = CreateObject(OFPL,470,550,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(520, 280);
   aFlag[0] -> AddSpawnPoint(530, 360);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,LandscapeWidth()/2,460,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(1620,540);
   aFlag[1] -> AddSpawnPoint(1850,540);
   aFlag[1] -> AddSpawnPoint(1850,250);
   aFlag[1] -> AddSpawnPoint(1670,310);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,LandscapeWidth()-470, 550,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(LandscapeWidth()-520, 280);
   aFlag[2] -> AddSpawnPoint(LandscapeWidth()-530, 360);
   if(aTeams[2] == true)
   {
    aFlag[2]->Set("$Flag3$",100,2);
    aFlag[2]->Capture(2,1);
   }
   else
   {
    aFlag[2]->Set("$Flag3$",0,2);
   }
  }
  
  //Und jetzt Timer starten, Clonk in Cursor stecken, und losstapeln!
  CreateObject(CLK_,0,0,-1);
  
  for(var i = 0; i < GetPlayerCount(); i++)
    DoWealth(GetPlayerByIndex(i),1500/GetPlayerCount()*2);
}


/* BAS-Ziel Zerstörung */

public func OnAssaultTargetDestruction(pTarget, iTeam)
{
  Alarm(true,iTeam);
}


/* Flagge geklaut */

public func FlagReturned(team, clonk)
{
  if(FindObject(GCTF))
    Alarm(false, team);
}

public func FlagCaptured(team, enemyteam, clonk)
{
  if(FindObject(GCTF))
    Alarm(true, team);
}

public func Alarm(bool alarm, int team)
{
  var x, width = LandscapeWidth()/2;
  if(team == 1) x = 0;
  if(team == 2) x = LandscapeWidth()/2;
  
  var alarmobj;
  while(alarmobj = FindObject(ALGH,x,0,width,LandscapeHeight(),0,0,0,0,alarmobj))
  {
    if(alarm) alarmobj -> TurnOn();
    else      alarmobj -> TurnOff();
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
     { iX = 520; iY = 280; }
    if(!--rand)
     { iX = 530; iY = 360; }
    return(1);
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = LandscapeWidth()-520; iY = 280; }
    if(!--rand)
     { iX = LandscapeWidth()-530; iY = 360; }
    return(1);
   }
  }

  //Startsicht
  if(iTeam == 1)
    iX = 1000;
  else
    iX = 2500;
  iY = 400;
}
