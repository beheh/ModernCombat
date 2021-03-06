/*-- Neues Script --*/

#strict 2

#appendto CASS

public func PlantTime()		{return 3*35;}	//Zeit zur Plazierung einer Sprengladung
public func DefuseTime()	{return 3*35;}	//Zeit zur Entschärfung einer Sprengladung

protected func FxIntAssaultTargetStart(object pTarget, int iEffect, int iTemp, int iMaxTime, bool fNoBar, id idTarget)
{
  if(iTemp)
    return;

  if(!iMaxTime)
    iMaxTime = 35 * 30;

  var bar = EffectVar(0, pTarget, iEffect) = CreateObject(SBAR, 0, 0, -1);
  bar->Set(pTarget, 0xFF0000/*GetTeamColor(pTarget->~GetTeam())*/, BAR_AssaultBar, 200, 0, SM16, 0, 0, true);
  bar->ChangeDefHeight(GetObjHeight(pTarget));
  bar->ChangeDefOffset(-GetObjHeight(pTarget)/2+5);
  bar->Update(0, true, true);
  bar->SetIcon(0, SM16, 0, 0, 32);
  bar->PositionToVertex(0, true);

  EffectVar(1, pTarget, iEffect) = idTarget;
  EffectVar(2, pTarget, iEffect) = 0;
  EffectVar(3, pTarget, iEffect) = 0;		//Status
  EffectVar(4, pTarget, iEffect) = 0;		//Entschärfung
  EffectVar(5, pTarget, iEffect) = iMaxTime;

  //Entschärfungsbalken
  bar = EffectVar(6, pTarget, iEffect) = CreateObject(SBAR, 0, 0, -1);
  bar->Set(pTarget, 0x33CCFF, BAR_AssaultBar+1, 200);
  bar->ChangeDefHeight(GetObjHeight(pTarget));
  bar->ChangeDefOffset(-GetObjHeight(pTarget)/2+5);
  bar->Update(0, true);
  bar->PositionToVertex(0, true);

  EffectVar(7, pTarget, iEffect) = [];

  //Bepunktung
  EffectVar(8, pTarget, iEffect) = [];
  EffectVar(9, pTarget, iEffect) = [];
}

public func PlantingCondition() { return true; }
public func IsAttackable() { return true; }

protected func FxIntAssaultTargetTimer(object pTarget, int iNr, int iTime)
{
  if(!IsAttackable(pTarget, iNr))
  	return;

	var bar = EffectVar(0, pTarget, iNr); 

  var idTarget = EffectVar(1, pTarget, iNr);

  var status = EffectVar(2, pTarget, iNr);
  var process = EffectVar(3, pTarget, iNr);
  var def_process = EffectVar(4, pTarget, iNr); 
  var maxTime = EffectVar(5, pTarget, iNr);

  var attacker = EffectVar(8, pTarget, iNr), defender = EffectVar(9, pTarget, iNr);

  var clonks = FindObjects(pTarget->Find_Distance(GASS_PlantRadius), Find_OCF(OCF_CrewMember|OCF_Alive), Find_NoContainer());
  var aEnemies = [], aAllies = [], team = pTarget->~GetTeam();

  //Fraktionen identifizieren
  for(var clonk in clonks)
  {
    if(GetOwner(clonk) == NO_OWNER) continue;
    if(!GetPlayerName(GetOwner(clonk)) || !GetPlayerTeam(GetOwner(clonk))) continue;
    if(!PathFree4K(GetX(pTarget),GetY(pTarget)-GetObjHeight(pTarget)/2,GetX(clonk),GetY(clonk),4)) continue;

    if(GetPlayerTeam(GetOwner(clonk)) == team)
    {
      if(status == 3 && GetIndexOf(clonk, defender) == -1)
        defender[GetLength(defender)] = clonk;

      aAllies[GetLength(aAllies)] = clonk;
    }
    else
    {
    	if(!PlantingCondition(clonk, pTarget)) continue;
      if(status == 1 && GetIndexOf(clonk, attacker) == -1)
        attacker[GetLength(attacker)] = clonk;

      aEnemies[GetLength(aEnemies)] = clonk;
    }
  }

  //Angreifer- und Verteidigeraufkommen jeweils zusammenrechnen
  var alliescnt = GetLength(aAllies), enemycnt = GetLength(aEnemies);

  //Status 0: Keine Aktivität
  if(!status)
  {
    bar->Update(0, true, true);

    //Angreifer vorhanden: Sprengladung wird platziert
    if(!GetEffect("TeamBorder", this) && enemycnt)
    {
      status = 1;
      process = 1;
      bar->SetIcon(0, SM17, 0, 0, 32);
      ShowPlantRadius(pTarget);
    }
  }

  //Status 1: Sprengladung wird platziert
  else if(status == 1)
  {
    bar->Update(process * 100 / PlantTime(), false);

    //Nur Angreifer vorhanden: Platzierung voranbringen
    if(enemycnt && !alliescnt)
    {
      process++;
      bar->SetIcon(0, SM17, 0, 0, 32);

      Sound("AHBS_Progress*.ogg", false, pTarget);
    }
    //Keine Angreifer vorhanden: Platzierung abbrechen
    else if(!enemycnt)
    {
      process = 0;
      status = 0;
      attacker = [];

      bar->Update(0, true, true);
      bar->SetIcon(0, SM16, 0, 0, 32);
    }
    //Angreifer und Verteidiger vorhanden: Platzierung blockieren
    else if(enemycnt && alliescnt)
      bar->SetIcon(0, SM19, 0, 0, 32);

    //Platzierung erfolgreich: Sprengladung ist scharf
    if(process >= PlantTime())
    {
      status = 2;
      process = maxTime;
      bar->SetIcon(0, SM18, 0, 0, 32);

      var assist = false;
      for(var clonk in attacker)
      {
        if(clonk)
          if(assist)
          {
            //Punkte bei Belohnungssystem (Hilfe bei Sprengladungsplazierung)
            DoPlayerPoints(BonusPoints("ASTargetArmedAssist"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC15);
          }
          else
          {
            //Punkte bei Belohnungssystem (Sprengladungsplazierung)
            DoPlayerPoints(BonusPoints("ASTargetArmed"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC15);

            //Achievement-Fortschritt (Bomberman)
            DoAchievementProgress(1, AC43, GetOwner(clonk));
          }

        assist = true;
      }

      AddEffect("IntAlarmBlink", pTarget, 1, 1, this);
			
      Sound("AHBS_Fused.ogg", false, pTarget);
    	this->~OnPlantingComplete(attacker, pTarget);
    }
  }

  //Status 2: Sprengladung ist scharf
  else if(status == 2)
  {
    this->~OnBombTimer(pTarget);
    bar->Update(process * 100 / maxTime, false);
    bar->SetIcon(0, SM18, 0, 0, 32);
    process--;

    //Verteidiger vorhanden: Entschärfung starten
    if(alliescnt)
    {
      def_process = 1;
      status = 3;
      ShowPlantRadius(pTarget);
    }
    //Countdown abgelaufen: Sprengsatz explodiert
    else if(process <= 0)
    {
      SetController(GetOwner(attacker[0]), pTarget);
      return ReportAssaultTargetDestruction(pTarget, team, attacker);
    }
  }

  //Status 3: Sprengladung wird entschärft
  else if(status == 3)
  {
    EffectVar(6, pTarget, iNr)->Update(def_process * 100 / DefuseTime(), false);

    //Nur Verteidiger vorhanden: Entschärfung voranbringen
    if(!enemycnt && alliescnt)
    {
      def_process++;
      bar->SetIcon(0, SM16, 0, 0, 32);

      Sound("AHBS_Progress*.ogg", false, pTarget);
    }
    //Keine Verteidiger vorhanden: Entschärfung abbrechen
    else if(!alliescnt)
    {
      def_process = 0;
      status = 2;
      EffectVar(6, pTarget, iNr)->Update(0, true);
      defender = [];
    }
    //Angreifer und Verteidiger vorhanden: Entschärfung blockieren
    else if(enemycnt && alliescnt)
      bar->SetIcon(0, SM19, 0, 0, 32);

    //Entschärfung erfolgreich: Status zurücksetzen
    if(def_process >= DefuseTime())
    {
      process = 0;
      def_process = 0;
      status = 0;
      EffectVar(6, pTarget, iNr)->Update(0, true);

      var assist = false;
      for(var clonk in defender)
      {
        if(clonk)
          if(assist)
          {
            //Punkte bei Belohnungssystem (Hilfe bei Sprengladungsentschärfung)
            DoPlayerPoints(BonusPoints("ASTargetDefusedAssist"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC15);
          }
          else
          {
            //Punkte bei Belohnungssystem (Sprengladungsentschärfung)
            DoPlayerPoints(BonusPoints("ASTargetDefused"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC15);

            //Achievement-Fortschritt (Firewall)
            DoAchievementProgress(1, AC44, GetOwner(clonk));
          }
        assist = true;
      }

      bar->Update(0, true, true);
      bar->SetIcon(0, SM16, 0, 0, 32);

      Sound("AHBS_Defused.ogg", false, pTarget);
      this->~OnDefusingComplete(defender, pTarget);

      attacker = [];
      defender = [];
    }
  }

  EffectVar(2, pTarget, iNr) = status;		//Bomben-Zustand
  EffectVar(3, pTarget, iNr) = process;		//Status
  EffectVar(4, pTarget, iNr) = def_process;	//Entschärfung
  EffectVar(8, pTarget, iNr) = attacker;	//Angreifer
  EffectVar(9, pTarget, iNr) = defender;	//Verteidiger
}

protected func FxIntAssaultTargetDamage(object pTarget, int iEffect, int iDamage)		{}
protected func FxIntAssaultTargetStop(object pTarget, int iEffect, int iCause, bool fTemp)	{}

/* Umkreis-Effekt */

protected func ShowPlantRadius(object pTarget)
{
  //Kreis-Symbol erstellen
  var obj = CreateObject(SM09, 0, 0, -1);
  obj->Set(pTarget);

  //Symbolgröße anpassen
  var wdt = GASS_PlantRadius * 2000 / GetDefWidth(SM09);

  //Symbol konfigurieren
  obj->SetObjDrawTransform(wdt, 0, 0, 0, wdt, 0);
  obj->SetGraphics("Big");
  obj->SetColorDw(RGB(255,0,0));

  return true;
}

/* Alarm-Effekt */

protected func FxIntAlarmBlinkStart(object pTarget, int iNr)
{
  EffectVar(0, pTarget, iNr) = 18;
  EffectVar(2, pTarget, iNr) = AddLight(300, RGB(220, 0, 0), pTarget);
  SetVisibility(VIS_None, EffectVar(2, pTarget, iNr));
}

protected func FxIntAlarmBlinkTimer(object pTarget, int iNr, int iTime)
{
  var light = EffectVar(2, pTarget, iNr);

  var effect = GetEffect("IntAssaultTarget", pTarget);
  if(!effect)
    return;

  var process = EffectVar(3, pTarget, effect);
  var status = EffectVar(2, pTarget, effect);
  var maxTime = EffectVar(5, pTarget, effect);
  if(status < 2)
    return -1;

  //Zeit abgelaufen?
  if(iTime % EffectVar(0, pTarget, iNr))
    return;

  //Beep-Geräusch
  var sound = EffectVar(1, pTarget, iNr) = !EffectVar(1, pTarget, iNr);
  if(sound)
  {
    if(process > maxTime/3*2)
      Sound("AHBS_Beep1.ogg", false, pTarget);
    else if(Inside(process, maxTime/3, maxTime/3*2))
    {
      Sound("AHBS_Beep2.ogg", false, pTarget);
      EffectVar(0, pTarget, iNr) = 9;
    }
    else if(process < maxTime/3)
    {
      Sound("AHBS_Beep3.ogg", false, pTarget);
      EffectVar(0, pTarget, iNr) = 4;
    }
  }

  //Alarmlicht-Blinken
  if(GetVisibility(light) == VIS_None)
    SetVisibility(VIS_All, light);
  else
    SetVisibility(VIS_None, light);
}

protected func FxIntAlarmBlinkStop(object pTarget, int iNr)
{
  if(EffectVar(2, pTarget, iNr))
    RemoveObject(EffectVar(2, pTarget, iNr));
}
