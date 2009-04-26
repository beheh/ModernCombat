/*-- Tutorial --*/

#strict

static station;//Aktuelle Stationsnummer.
static benzinclonk;//Clonk der die Benzinfäßer "bewacht".

func Initialize()
{
  //Szenario einrichten
  CreateFurniture();
  ScriptGo(true);
  return(1);
}

func CreateFurniture()
{
  //Alle Objekte folglich von links oben nach rechts unten
  //Sektionen nummerisch von links nach rechts

  //Sektion 1--------------------------------------------------------
  
  //Benzinfässer
  CreateObject(PBRL, 649, 581, -1);
  CreateObject(PBRL, 622, 581, -1);
  CreateObject(PBRL, 609, 581, -1);
  CreateObject(PBRL, 551, 591, -1);
  CreateObject(PBRL, 785, 720, -1);

  //Aufzug
  CreateObject(LFTP, 745, 370, -1);

  //Sprungschanze
  CreateObject (JMPD, 405, 720, -1)->Set(80, -15);

  //Glasscheiben
  CreateObject(_WIN, 722, 350, -1);
  CreateObject(_WIN, 722, 379, -1);
  CreateObject(_WIN, 722, 408, -1);
  CreateObject(_WIN, 722, 437, -1);
  CreateObject(_WIN, 722, 466, -1);
  CreateObject(_WIN, 722, 495, -1);
  CreateObject(_WIN, 722, 524, -1);

  //Leitern
  CreateObject(LADR, 960, 720, -1)->Set(44);
  CreateObject(LADR, 1410, 770, -1)->Set(20);

  //Kisten
  CreateObject(WCR2, 25, 450, -1);
  CreateObject(WCR2, 815, 720, -1);
  CreateObject(WCR2, 835, 720, -1);
  CreateObject(WCR2, 830, 702, -1);
  CreateObject(WCR2, 845, 490, -1);

  //Regale
  CreateObject(FRAM, 635, 640, -1);
  CreateObject(FRAM, 655, 640, -1);
  CreateObject(FRAM, 675, 640, -1);
  CreateObject(FRAM, 695, 640, -1);
  CreateObject(FRAM, 890, 490, -1);

  //Labortische
  CreateObject(LTBL, 885, 550, -1);

  //Gasflaschen
  CreateObject(GSBO, 660, 640, -1);
  CreateObject(GSBO, 840, 550, -1);
  CreateObject(GSBO, 845, 550, -1);
  CreateObject(GSBO, 850, 550, -1);
  CreateObject(GSBO, 860, 550, -1);

  //Tische
  CreateObject(GTBL, 795, 260, -1);
  CreateObject(TABL, 875, 720, -1);
  CreateObject(TABL, 910, 360, -1);
  CreateObject(TABL, 920, 720, -1);

  //Munitionskisten (Kugeln)
  CreateObject(ABOX, 870, 360, -1)->Set(AMOC);

  //Stationärer Gastank
  CreateObject(GSTA, 520, 590, -1);

  //Satellitenschüssel
  CreateObject(SADH, 395, 470, -1);

  //Flutlichter
  CreateObject(FLGH, 180, 640, -1)->SetRotation(65);
  CreateObject(FLGH, 1355, 550, -1);

  //Radarbildschirm
  CreateObject(SCA1, 960, 330, -1);

  //Laborlicht
  CreateObject(LLGH, 395, 600, -1);

  //Deckenlichter
  CreateObject(CLGH, 45, 615, -1);
  CreateObject(CLGH, 665, 615, -1);
  CreateObject(CLGH, 760, 225, -1);
  CreateObject(CLGH, 830, 675, -1);
  CreateObject(CLGH, 900, 675, -1);
  CreateObject(CLGH, 1005, 295, -1);
  CreateObject(CLGH, 1035, 405, -1);
  CreateObject(CLGH, 1035, 465, -1);
  CreateObject(CLGH, 1035, 525, -1);
  CreateObject(CLGH, 1210, 605, -1);

  //Wandlampen
  CreateObject(BLGH, 745, 345, -1);
  CreateObject(BLGH, 745, 555, -1);
  CreateObject(BLGH, 745, 620, -1);

  //Spinde
  CreateObject(LCKR, 735, 260, -1);
  CreateObject(LCKR, 755, 260, -1);

  //Tür (Dekoration)
  CreateObject(ROOM, 615, 640, -1);
  CreateObject(ROOM, 1005, 720, -1);

  //Geländer
  CreateObject(RAI1, 317, 610, -1)->SetRail([1]);
  CreateObject(RAI1, 930, 360, -1)->SetRail([1,1,1,1,1]);

  //Lüftungsgitter
  CreateObject(ENGT, 375, 670, -1);

  //Bildschirm
  CreateObject(SCR3, 775, 245, -1);

  //Sandsackbarrieren
  CreateObject(SBBA, 98, 450, -1)->Right();

  //Markierungen
  CreateObject(MSGN, 220, 640, -1);
  CreateObject(MSGN, 260, 630, -1);
  CreateObject(MSGN, 300, 615, -1);
  CreateObject(MSGN, 410, 610, -1);
  CreateObject(MSGN, 440, 603, -1);
  CreateObject(MSGN, 470, 590, -1);

  //Bodenlucken
  CreateObject(HA4K, 960, 363, -1);
  CreateObject(HA4K, 960, 433, -1);
  CreateObject(HA4K, 960, 493, -1);

  //Bodenlucken (Script)
  CreateObject(HA4K, 960, 553, -1)->Lock();

  //Hydrauliktüren (Script)
  CreateObject(SLDR, 90, 640, -1);
  CreateObject(SLDR, 810, 350, -1);
  CreateObject(SLDR, 915, 490, -1);
  CreateObject(SLDR, 915, 550, -1)->Lock();
  CreateObject(SLDR, 1070, 550, -1);

  //Sektion 2--------------------------------------------------------

  //Deckenlichter
  CreateObject(CLGH, 1970, 405, -1);
  CreateObject(CLGH, 1970, 465, -1);
  CreateObject(CLGH, 1970, 525, -1);
  CreateObject(CLGH, 1985, 295, -1);
  CreateObject(CLGH, 2120, 455, -1);

  //Leiter
  CreateObject(LADR, 2030, 550, -1)->Set(22);

  //Tische
  CreateObject(TABL, 1970, 490, -1);

  //Automat
  CreateObject(CLVM, 1985, 360, -1);

  //Bodenlucken
  CreateObject(HA4K, 2030, 363, -1);
  CreateObject(HA4K, 2030, 433, -1);
  CreateObject(HA4K, 2030, 493, -1);

  //Ventillator
  CreateObject(MVNT, 2135, 489, -1)->SetCon(50);

  //Geländer
  CreateObject(RAI1, 1960, 360, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 2200, 490, -1)->SetRail([1,1,1,1,1,1,1]);

  //Glasscheiben
  CreateObject(_WIN, 2378, 490, -1);
  CreateObject(_WIN, 2378, 461, -1);

  //Laborlicht
  CreateObject(LLGH, 2345, 470, -1);

  //Hydrauliktüren (Script)
  CreateObject(SLDR, 1920, 550, -1);
  CreateObject(SLDR, 2190, 490, -1);

  //Sektion 3--------------------------------------------------------

  //Leitern
  CreateObject(LADR, 2340, 550, -1)->Set(6);
  CreateObject(LADR, 2890, 625, -1)->Set(18);

  //Bodenlucken
  CreateObject(HA4K, 2340, 493, -1);
  CreateObject(HA4K, 2890, 473, -1);
  CreateObject(HA4K, 2760, 473, -1);

  //Geländer
  CreateObject(RAI1, 2665, 470, -1)->SetRail([1,1,1,1,1,1,1,1,1]);

  //Tische
  CreateObject(TABL, 2265, 491, -1);

  //Flutlichter
  CreateObject(FLGH, 1635, 550, -1);
  CreateObject(FLGH, 2365, 490, -1)->SetRotation(25);
  CreateObject(FLGH, 2655, 680, -1)->SetRotation(-75);
  CreateObject(FLGH, 2785, 590, -1)->SetRotation(-10);

  //Rampen
  DrawMaterialQuad("Wall-Stripes",220,640,250,630,250,640,235,640,true);
  DrawMaterialQuad("Wall-Stripes",250,630,280,620,280,630,265,630,true);
  DrawMaterialQuad("Wall-Stripes",280,620,310,610,310,620,295,620,true);

  DrawMaterialQuad("Wall-Stripes",440,600,470,590,470,600,455,600,true);
  DrawMaterialQuad("Wall-Stripes",410,610,440,600,440,610,425,610,true);

  DrawMaterialQuad("Wall-Concrete3",570,590,600,580,600,590,585,590,true);

  DrawMaterialQuad("Wall-Concrete3",851,360,821,350,821,360,836,360,true);

  DrawMaterialQuad("Wall-Stripes",670,580,700,570,700,580,685,580,true);

  DrawMaterialQuad("Wall-PlateBlue",930,270,960,260,960,270,945,270,true);
  DrawMaterialQuad("Wall-PlateBlue",1081,270,1051,260,1051,270,1066,270,true);

  DrawMaterialQuad("Wall-Stripes",1030,360,1060,350,1060,360,1045,360,true);

  DrawMaterialQuad("Wall-PlateBlue",1910,270,1940,260,1940,270,1925,270,true);
  DrawMaterialQuad("Wall-PlateBlue",2091,270,2061,260,2061,270,2076,270,true);

  DrawMaterialQuad("Wall-Stripes",1961,360,1931,350,1931,360,1946,360,true);

  DrawMaterialQuad("Wall-PlateBlue",2201,420,2171,410,2171,420,2186,420,true);

  //Verbundene Türen
  var doorw = CreateObject(ROM2, 865, 490, -1);
  CreateObject(ROM2, 2095, 490, -1)->Connect(doorw);

  //Hydrauliktüren (Script)
  CreateObject(SLDR, 2615, 610, -1);
  CreateObject(SLDR, 2705, 680, -1);

  //Sonstiges--------------------------------------------------------

  //KIs
  PlaceAI(561,580,"PetrolGuard",MDIC);
  PlaceAI(1070,420,"Marksman",SOLD)->AIVar(1) = -20;
  
  Schedule("PlaceAI(1070,480,\"Marksman\",SOLD)",35+Random(35));
  
  Demo();
}

func Demo()
{
  CreateObject (TRGT, 1352, 373, 0)->SetCon(50);
  CreateObject (TRGT, 1350, 439, 0)->SetCon(50);
  CreateObject (TRGT, 1349, 502, 0)->SetCon(50);
  CreateObject (TRGT, 1218, 456, 0)->MoveLine(0,-100,1);
  
  CreateObject (M16A, 910, 342, -1);
}


/* Script0-X */
func Script10()
{
  SetCrewEnabled(true, GetCrew());
  SetCursor (0, GetCrew(), true, true);
  DoInfoMessage(GetCrew(),"Commander","Willkommen zur erweiterten Ausbildung, Soldat!|Du wirst hier alles Notwendige lernen, um in kommenden Schlachten bestehen zu können.|Fangen wir damit an, das wir deine Kenntnisse über Waffen auffrischen.|<c ffff00>Gehe zum Schießstand.</c>", HZCK, RGB(80,100,0),1);
}

func Script12()
{
  if(!FindObject(TRGT))
  {
    DoInfoMessage(GetCrew(),"Henry","So, hier ist Ende Gelände! :P|<c ff0000>Testzeit abgelaufen.</c>", HZCK, RGB(255),1);
  }
  else
    goto(11);
}


/* Stationen */
func Station0(object pClonk)
{
  goto();//zurücksetzen

  pClonk->SetPosition(8,630);
  SetCrewEnabled(false, pClonk);
  SetCommand(pClonk,"MoveTo",0,138,630);
}


/* Wer kennt ihn nicht? Den Respawn! */
protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
    RelaunchPlayer(iPlr, pCrew, 0, iTeam);
}

public func RelaunchPlayer(int iPlr, object pCrew, object pKiller, int iTeam)
{
  //Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return();
  //Kein Team
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);
  //Clonk tot?
  if(!GetAlive(pCrew))
    pCrew = RelaunchClonk(iPlr, pCrew);
    
  //AddSpawnEffect(pCrew, pCrew->GetColorDw());
    
  //Station für den Clonk initialisieren. :D
  GameCall(Format("Station%d",station),pCrew);
}

public func RelaunchClonk(int iPlr, object pCursor)
{
  var pClonk = CreateObject(MDIC, 10, 10, iPlr);
  if(pCursor)
    GrabObjectInfo(pCursor, pClonk);
  else
    MakeCrewMember(pClonk, iPlr);

  DoEnergy(+150, pClonk);
  SetCursor(iPlr, pClonk);
  SetPlrView(iPlr, pClonk);

  return(pClonk);
}


/* Benzinclonk */
func AI_PetrolGuard_Init(object pClonk, int iEffect)
{
  //1
  AIDialog(pClonk)->NewChat("Hey, was machst du da?",
                           [AIDialog(pClonk)->NewAnswerInfo(0,0,0,2,"Benzin ist grade teuer, da dachte ich, ich nehm' mir ein Faß."),
                            AIDialog(pClonk)->NewAnswerInfo(0,0,0,3,"Was ist da drin?")]);

  //2
  AIDialog(pClonk)->NewChat("Ich stehe nicht umsonst hier, lass' die Finger davon.",
                           [AIDialog(pClonk)->NewAnswerInfo("UnGrab",0,0,0,"Ja, ist okay.")]);

  //3
  AIDialog(pClonk)->NewChat("Der stechende Geruch, die warnenden Farben.. ich tippe mal auf Benzin. Ein Grund es nicht durch die Weltgeschichte zu schieben, also nehm' deine Griffel weg.",
                           [AIDialog(pClonk)->NewAnswerInfo("UnGrab",0,0,0,"Oh, dann lass ich mal lieber die Finger davon.")]);

  //4
  AIDialog(pClonk)->NewChat("Was ich hier mache?|Ich wurde für die Benzinbewachung eingeteilt.",
                           [AIDialog(pClonk)->NewAnswerInfo(0,0,0,5,"Willst du mit ein bisschen schießen?"),
                            AIDialog(pClonk)->NewAnswerInfo(0,0,0,6,"Ist die Ausbildung schwirieg?"),
                            AIDialog(pClonk)->NewAnswerInfo(0,0,0,7,"Warum Benzin bewachen?")]);

  //5
  AIDialog(pClonk)->NewChat("Sorry Kumpel, ich kann hier nicht weg.|Du gehörst doch zur neuen Ausbildungstruppe, geh schon, sonst gibts sicher Ärger.",
                           [AIDialog(pClonk)->NewAnswerInfo(0,0,0,0,"Stimmt, ich geh dann mal.")]);

  //6
  AIDialog(pClonk)->NewChat("Ist schaffbar, du solltest damit keine Probleme haben. Du wirst aber welche bekommen, wen du jetzt nicht losgehst.",
                           [AIDialog(pClonk)->NewAnswerInfo(0,0,0,0,"Okay.")]);

  //7
  AIDialog(pClonk)->NewChat("Das komprimierte Benzin ist hochexplosiv und in der Nähe ist ein Schießstand. Nebenbei, musst du da jetzt nicht hin?",
                           [AIDialog(pClonk)->NewAnswerInfo(0,0,0,0,"Oh, dann werd ich mich mal verzeihen.")]);
}

func AI_PetrolGuard_StartDialog(object pClonk, int iEffect, object pPartner)
{
  if(!AIDialog(pClonk)->StartChat(4,pPartner))
  {
    AIDialog(pClonk)->SetGreeting("Jetzt geh schon, die warten nicht ewig!");
    AIDialog(pClonk)->StartDialog(pPartner);
    AIDialog(pClonk)->AddAnswer(0, "Ich mach ja schon.",0,TALK);
  }
}

func AI_PetrolGuard_Timer(object pClonk, int iEffect)
{
  var guy = GetCrew();
  if(GetX(guy) <= 840)
    if(GetAction(guy) eq "Push")
      if(GetID(guy->GetActionTarget()) == PBRL)//Keine Fässer anfingern! >:/
      {
        if(AIDialog(pClonk)->GetPartner())
          if(AIDialog(pClonk)->GetPartner() != guy)
            AIDialog(pClonk)->StopChat();//Gespäche stoppen.
        
        if(ObjectDistance(pClonk,guy) >= 30)//Zu weit weg?
        {
          Go2Clonk(pClonk,guy);//Hingehen.
        }
        else
        {
          if(!AIDialog(pClonk)->GetPartner())
            StartPetrolDialog(pClonk,guy);//Los meckern!
        }
        
        return();
      }

  if(Distance(GetX(pClonk),GetY(pClonk),AIHomeX(pClonk),AIHomeY(pClonk)) >= 30)
    AIGoHome(pClonk);
}

func StartPetrolDialog(object pClonk, object pPartner)
{
  if(!AIDialog(pClonk)->StartChat(1,pPartner))
  {
    if(AIVar(0,pClonk)++ >= 1) return(Punish(pPartner));
    AIDialog(pClonk)->SetGreeting("Du weißt genau was ich meine!");
    AIDialog(pClonk)->StartDialog(pPartner);
    AIDialog(pClonk)->AddAnswer(0, "Ja, Entschuldigung","UnGrab",NONE);
  }
}

/* Schütze */
func AI_Marksman_Init(object pClonk, int iEffect)
{
  CreateContents(M16A,pClonk)->SetFireTec(1,1,true);//Einzelschuss.
  DoAmmo(STAM,9999,pClonk);
  //pClonk->StartSquatAiming();
  
  AIVar(0,pClonk) = 2+Random(6);//Skill. :>
}

func AI_Marksman_StartDialog(object pClonk, int iEffect, object pPartner)
{
  var rand = Random(3);
  if(!rand)
    AIDialog(pClonk)->SetGreeting("Was ich hier mache?|Siehst du doch!");
  if(!--rand)
    AIDialog(pClonk)->SetGreeting("Hey, ich muss mich konzentrieren.");
  if(!--rand)
    AIDialog(pClonk)->SetGreeting("Zieh Leine, ich muss das Training bestehen.");
    
  AIDialog(pClonk)->StartDialog(pPartner);
    
  rand = Random(3);
  
  if(!rand)
    AIDialog(pClonk)->AddAnswer(0, "Reg dich ab, ich geh ja schon.",0,TALK);
  if(!--rand)
    AIDialog(pClonk)->AddAnswer(0, "Du bist ja gut drauf.",0,TALK);
  if(!--rand)
    AIDialog(pClonk)->AddAnswer(0, "Ok ok, ich lass dich mal besser in Ruhe.",0,TALK);
}

func AI_Marksman_Timer(object pClonk, int iEffect)
{
  var aTargets = [];
  
  for(var obj in pClonk->FindObjects(Find_ID(TRGT),Sort_Distance(),Find_Not(Find_Func("Triggered"))))
  {
    if(obj->GetPlr(obj) != NO_OWNER) continue;
    if(obj->GetTarget() == pClonk) aTargets[GetLength(aTargets)] = obj;
  }
  
  if(!GetLength(aTargets))
  {
    if(Random(5)) return();
  
    //Ziele erstellen.
    var target = CreateObject(TRGT, 1300+AIVar(1,pClonk), 373+Random(100), NO_OWNER);
    target->SetCon(75);
    target->SetTarget(pClonk);
  }
  else
  {
    if(Random(5)) return();
  
    //Drauf knallen.
    if(WeaponReady(pClonk))
    {
      AimAtTarget(pClonk,aTargets[0]);
      Fire(pClonk);
    }
  }
}

//Helperz
func Go2Clonk(object pC1, object pC2)
{
  var x = -10;
  if(GetX(pC1) > GetX(pC2))
    x = +10;
    
  SetCommand(pC1,"MoveTo",0,GetX(pC2)+x,GetY(pC2));
}

func UnGrab(unused, object pClonk, object pPartner)
{
  SetCommand(pPartner,"UnGrab");
}

func Punish(object pClonk)
{
  Log("%s wurde vom Dienst suspendiert.",GetTaggedPlayerName(GetController(pClonk)));
  EliminatePlayer(GetOwner(pClonk));
  AddEffect("ReleaseClonkFadeOut", pClonk, 320, 1, 0, REAC, GetOwner(pClonk));
}

//Aus meiner nie veröffentlichten Hazard-KI. *schniff* (Allerdings stark vereinfacht.)
func AimAtTarget(object pClonk, object pTarget)
{
  if(!pClonk->IsAiming())
      pClonk->StartSquatAiming();
      
  TurnToObject(pClonk,pTarget);
        
  var iAngle = Angle(GetX(pClonk),GetY(pClonk),GetX(pTarget),GetY(pTarget));
  
  if(Abs(iAngle-GetAiming(pClonk)) <= 1)//Müssen wir überhaupt zielen?
    return(true);
  
  iAngle = iAngle + RandomX(-AIVar(0,pClonk),AIVar(0,pClonk));//Ungenauigkeit
  if(iAngle > 180)
    iAngle -= 360;

  //Winkel wird zu groß?
  iAngle = BoundBy(iAngle,-AIM_Max,+AIM_Max);
  
  SetAiming(pClonk,iAngle);
    
  return(true);
}

func GetAiming(object pClonk)
{
  var cross = LocalN ("crosshair",pClonk);
  
  if(!cross)
    return(90*(GetDir(pClonk)*2-1));
  else
    return(cross->~GetAngle());
}

func SetAiming(object pClonk, int iAngle)
{
  var cross = LocalN ("crosshair",pClonk);
  
  if(!cross)//Notfallhack
  {
    pClonk->StopAiming();
    pClonk->StartSquatAiming();
  }
  
  if(!Inside(cross->GetAngle(),iAngle + 20, iAngle - 20))
  {
    cross->~SetAngle(iAngle);
    pClonk->UpdateAiming();
  }
  
  //Wichtig: Waffe updaten
  EffectCall(pClonk,LocalN("wpneffect",pClonk),"Timer");
}

func Fire(object pClonk)
{
  var weapon = pClonk->Contents();
  if(!weapon) return(false);
  if(!weapon->IsWeapon()) return(false);

  if(IsFiring(pClonk))
    return(true);
    
  if(pClonk->Contents()->~Fire())
  {
    return(true);
  }
  else
  {
    var ammoid = weapon->GetFMData(FM_AmmoID);
    var ammousage = weapon->GetFMData(FM_AmmoUsage);
    
    if(!CheckAmmo(ammoid,ammousage,weapon))//nicht mehr genügend muni in der waffe?
    {
      if(CheckAmmo(ammoid,ammousage,pClonk))//der clonk hat noch was?
      {
        weapon->Reload();
        pClonk->StopAiming();//xD Sieht halt toller aus. :)
      }
    }
  }
  
  return(false);
}

func WeaponReady(object pClonk)
{
  var weapon = pClonk->Contents();
  if(!weapon) return(false);
  if(!weapon->IsWeapon()) return(false);
  if(weapon->IsReloading()) return(false);
  
  var ammoid = weapon->GetFMData(FM_AmmoID);
  var ammousage = weapon->GetFMData(FM_AmmoUsage);
  
  if(!CheckAmmo(ammoid,ammousage,weapon))//nicht mehr genügend muni in der waffe?
  {
    if(CheckAmmo(ammoid,ammousage,pClonk))//der clonk hat noch was?
    {
      weapon->Reload();
      pClonk->StopAiming();//xD Sieht halt toller aus. :)
      
      return(false);
    }
  }
  
  return(true);
}

/*func StopFire(object pClonk)
{
  if(pClonk->Contents())
    if(pClonk->Contents()->~GetFMData(FM_Auto,true))
      pClonk->Contents()->StopAutoFire();
    
  return(true);
}*/

func IsFiring(object pClonk)
{
  var weapon = pClonk->Contents();
  if(!weapon) return(false);
  if(!weapon->IsWeapon()) return(false);
  
  if(GetEffect("Recharge",weapon))
    return(true);
  return(false);
}

func TurnToObject(object pClonk, object pObj)
{
  if(GetX(pObj) > GetX(pClonk))
    pClonk->SetDir(DIR_Right());
  else
    pClonk->SetDir(DIR_Left());
}