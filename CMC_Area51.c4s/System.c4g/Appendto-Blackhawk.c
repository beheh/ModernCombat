//Blackhawk soll MAVs schreddern; Änderungen beginnen ab z133

#strict 2
#appendto BKHK


//für Warnsounds und Grafik zuständig
protected func TimerCall()
{
  //Zerstört?
  if(IsDestroyed())
    return;	

  //Richtung ermitteln
  var iDir = GetDir() * 2 - 1;

  //Rotorgeschwindigkeit aktualisieren
  iRotorSpeed = throttle;
  if(EngineRunning())
  {
    if(GetAction() == "Fly" || GetAction() == "Turn") iRotorSpeed += 95;
  }
  else
    if(GetAction() != "Fly" && GetAction() != "Turn")
    {
      if(GetAction() == "EngineStartUp") iRotorSpeed += GetActTime();
      else if(GetAction() == "EngineStartUp2") iRotorSpeed += 95/(45+24)*(45+GetActTime());
      else if(GetAction() == "EngineStartUp3") iRotorSpeed += 95/(45+24+16)*(45+24+GetActTime());
      else if(GetAction() == "EngineShutDown") iRotorSpeed += 95-GetActTime();
      else if(GetAction() == "EngineShutDown2") iRotorSpeed += 95-60/(30+30)*(30+GetActTime());
      else if(GetAction() == "EngineShutDown3") iRotorSpeed += 95-90/(30+30+30)*(30+30+GetActTime());
    }
	
  //Absinken, wenn kein Pilot
  if(((!GetPilot() && !GetAutopilot() || GetY() < 100) && throttle) || (GetAction() == "Fly" && !GetPilot() && !GetAutopilot()))
    if(!Random(3))
    {
      if(throttle > 100)
        throttle -= 3;
      else if(throttle > 75)
        throttle--;
      else if(throttle > 50)
        throttle -= 5;
      else if(throttle > 0)
        throttle -= 10;
      if(throttle <= 0 && !GetPilot() && !GetAutopilot())
      {
        throttle = 0;
        SetAction("EngineShutDown");
      }
    }

  //Namensanzeige für Verbündete
  for(var first = true, iFlags, i = 0; i < GetPlayerCount(); i++)
  {
    var iPlr = GetPlayerByIndex(i);
    if(GetPilot() && !Hostile(GetOwner(GetPilot()), iPlr) && (!GetCursor(iPlr) || (GetCursor(iPlr) != GetPilot()) && GetCursor(iPlr)->~GetRealCursor() != GetPilot()))
    {
      if(first)
        first = false;
      else
        iFlags = MSG_Multiple;
      var szStr = Format("@%s (%s)", GetName(GetPilot()), GetPlayerName(GetOwner(GetPilot())));
      CustomMessage(szStr, this, iPlr, 0, 15, SetRGBaValue(GetPlrColorDw(GetOwner(GetPilot())), 128), 0, 0, iFlags);
    }
    if(!GetPilot())
      CustomMessage("@", this, iPlr);
  }

  //Scheinwerfer aktualisieren
  if(fShowSpotlight)
    if(pSpotlight[0])
      UpdateSpotlights();
    else
      CreateSpotlights();
  else if(pSpotlight[0])
    RemoveSpotlights();

  /*Nach gestorbenen Clonks suchen
  if(Contents())
  {
    var fUpdate = false;
    for(var pDead in FindObjects(Find_Container(this), Find_Or(Find_And(Find_Not(Find_OCF(OCF_Alive)), Find_Func("IsClonk")), Find_Func("IsFakeDeath"))))
    {
      pDead->Exit();
    }
    //if(fUpdate) UpdateSeats();
  }*/

  if(!(GetActTime() % 50))
  {
    if(!GetAlive(GetPilot()))
      DeleteActualSeatPassenger(GetPilot());
    if(!GetAlive(GetGunner()))
      DeleteActualSeatPassenger(GetGunner());
    if(!GetAlive(GetCoordinator()))
      DeleteActualSeatPassenger(GetCoordinator());
    if(!GetAlive(GetPassenger1()))
      DeleteActualSeatPassenger(GetPassenger1());
    if(!GetAlive(GetPassenger2()))
      DeleteActualSeatPassenger(GetPassenger2());
  }

  //Bodenpartikel zeichnen
  if(GetEffectData(EFSM_Fog) > 0)
  {
    if(GetRotorSpeed() != 0 && !GBackSemiSolid())
    {
      var iX = GetX();
      var iY = GetY();
      var iXDir = 0;
      var iYDir = 0;
      if(SimFlight(iX, iY, iXDir, iYDir, 1, 100, -1))
      {
        iY = iY-GetY();
        if(iY < 150 && GBackSemiSolid(0, iY))
        {
          var iMaterial = GetMaterial(0, iY);
          if(iMaterial == Material("Vehicle")) iMaterial = Material("Wall");
          var iLightness = 0;
          if(GBackLiquid(0, iY)) iLightness += 175;
          var iClrOffset = Random(3)*3;
          var iColor = RGB(Min(GetMaterialVal("Color", "Material", iMaterial, 0 + iClrOffset) + iLightness, 255), Min(GetMaterialVal("Color", "Material", iMaterial, 1 + iClrOffset) + iLightness, 255), Min(GetMaterialVal("Color", "Material", iMaterial, 2 + iClrOffset) + iLightness, 255));
          var iPower = Min(GetRotorSpeed(), 130);
          CreateParticle("GroundSmoke", iDir*21-3, iY, -(70 - iY / 3), RandomX(-5, 5),  RandomX(30, 15 + (14- iY / 10) * iPower / 5), iColor);
          CreateParticle("GroundSmoke", iDir*21+3, iY, (70 - iY / 3), RandomX(-5, 5), RandomX(30, 15 + (14 - iY / 10) * iPower / 5), iColor);
          CreateParticle("GroundSmoke", iDir*21-3, iY - 3, RandomX(-30, -(70 - iY)), -2, RandomX(30, 15 + (14 - iY / 10) * iPower / 5), iColor);
          CreateParticle("GroundSmoke", iDir*21+3, iY - 3, RandomX(30, (70 - iY)), -2, RandomX(30, 15 + (14 - iY / 10) * iPower / 5), iColor);
        }
      }
    }
  }

  //Lebewesen schrappneln
  if(GetRotorSpeed() != 0)
  {
    for(var pClonk in FindObjects(Find_OnLine(GetVertex(7), GetVertex(7, true), GetVertex(11), GetVertex(11, true)) , Find_NoContainer(), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsMAV")), Find_Not(Find_ID(FKDT))))
    {
      if(GetOwner(pClonk) != NO_OWNER && GetOwner() != NO_OWNER && !Hostile(GetOwner(), GetOwner(pClonk)))
        continue;
      if(GetEffect("NoRotorHit", pClonk))
        continue;
      if(pClonk->~IsMAV())
      {
        DoDmg(GetRotorSpeed(), DMG_Projectile, pClonk, GetOwner() + 1);
        continue;
      }
      Fling(pClonk, GetXDir(pClonk, 1) * 3 / 2 + RandomX(-1, 1), RandomX(-3, -2) - Pow(GetRotorSpeed(), 2) / 15000);
      DoDmg(GetRotorSpeed() / 4, DMG_Projectile, pClonk, 0, GetOwner() + 1);
      Sound("BKHK_RotorHit*.ogg", false, pClonk);
      AddEffect("NoRotorHit", pClonk, 1, 20, pClonk);
      //Achievement-Fortschritt (Meat Grinder)
      if(GetPilot() && (!GetAlive(pClonk) || IsFakeDeath(pClonk)))
        DoAchievementProgress(1, AC29, GetController(GetPilot()));
    }

    //Festes Material im Rotor tut weh, wird von Contact-Calls bei Stillstand nicht erfasst
    if(!PathFree(GetX() + GetVertex(7), GetY() + GetVertex(7, true), GetX() + GetVertex(11), GetY() + GetVertex(11, true)))
      DoDamage(1, this, FX_Call_DmgScript, GetController(GetPilot()) + 1);
  }
  
  //Nachladezeit reduzieren
  if(smokereload) 
    smokereload--;
  if(flarereload)
    flarereload--;

  //Bei Wasserkontakt Schaden nehmen
  if(GBackLiquid(0, 10))
    DoDamage(5);

  //Schadensverhalten
  if(GetDamage() >= MaxDamage() / 2)
  {
    for (var a = 0; a < 3; a++)
      if (!GBackLiquid(-Sin(GetR() + iDir * 80, 25), +Cos(GetR() + iDir * 80, 25)))
        Smoke(-Sin(GetR() + iDir * 80, 25), +Cos(GetR() + iDir * 80, 25), Random(10));
  }
  if(GetDamage() >= MaxDamage() * 3 / 4)
  {
    if (!GBackLiquid(-Sin(GetR() + iDir * 80, 25), +Cos(GetR() + iDir * 80, 25)))
      CreateParticle("Blast", -Sin(GetR() + iDir * 80, 25) + RandomX(-10, 10), +Cos(GetR() + iDir * 80, 25) + RandomX(-10, 10),0, -10, 100 + Random(30), RGB(255, 255, 255), this);
    if (!GBackLiquid(-Sin(GetR() + iDir * 80, 25), +Cos(GetR() + iDir * 80, 25)))
      CreateParticle("Blast", -Sin(GetR() - iDir * 60, 25) + RandomX(-10, 10), +Cos(GetR() + iDir * 100, 25) + RandomX(-10, 10),0, -10, 100 + Random(30), RGB(255, 255, 255), this);

    if(EngineRunning())
    {
      if(GetDamage() < MaxDamage() * 3 / 4) 
      {
        if (!(iWarningSound % 36))
        for (var obj in FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this)))
          Sound("WarningDamage.ogg", false, pMGStation, 100, GetOwner(obj) + 1);
        iWarningSound++;
        if(iWarningSound >= 100)
          iWarningSound = 0;
      }
      else
      {
        if (!(iWarningSound % 20))
        {
          Local(2) = 0;
          for (var obj in FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this)))
            if(obj != GetPilot())
              Sound("WarningDamageCritical.ogg", false, pMGStation, 100, GetOwner(obj) + 1);
        }
        iWarningSound++;
        if (iWarningSound >= 100)
          iWarningSound = 0;
      }
    }
  }
}
