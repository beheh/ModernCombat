/*-- Neues Script --*/

#strict 2

#appendto BKHK

local BKHK_LockSeats;

public func IsBulletTarget()
{
	if(FindObject(MBKD))
		return true;

	return _inherited(...);
}

public func DoRotation(int iChange) { rotation = BoundBy(rotation+iChange, 0, 30); }

public func SetAutopilot(object pTarget, int iX, int iY)
{
  if(!GetPilot() && !Minigame()) return;
  ResetAutopilot();
  var xto, yto;
  if(pTarget)
  {
    xto = AbsX(GetX(pTarget));
    yto = AbsY(GetY(pTarget));
  }
  xto += iX;
  yto += iY;
  AddEffect("BlackhawkAutopilot", this, 10, 1, this, 0, xto, yto);
  return true;
}

protected func FxBlackhawkAutopilotTimer(object pTarget, int iNumber, int iTime)
{
  if(!pTarget->GetPilot(0) && !Minigame()) return -1;
  var iX = EffectVar(0, pTarget, iNumber);
  var iY = EffectVar(1, pTarget, iNumber);
  if(iX == 0 || iY == 0) return -1;
  if(GetY(pTarget) < iY-50)
  {
    if(GetYDir(pTarget) < 3 || GetContact(this, -1, CNAT_Bottom))
    {
      //vom Gas weg
      if(GetAction() == "Fly" || GetAction() == "Turn")
      {
      	throttle = BoundBy(throttle - BKHK_AutoThrottleSpeed, 0, BKHK_AutoMaxThrottle);
      }
      else if(throttle != 0 && GetAction() == "Fly")
      {
        SetAction("EngineShutDown");
        return pTarget->ResetAutopilot();
      }
    }
  }
  else if(GetY(pTarget) > iY+50)
  {
    if(GetYDir(pTarget) > -3)
    {
      if(GetAction() == "Fly" || GetAction() == "Turn")
      {
      	throttle = BoundBy(throttle + BKHK_AutoThrottleSpeed, 0, BKHK_AutoMaxThrottle);
      }
      else if(throttle == 0 && GetAction() == "Stand")
      {
      	SetAction("EngineStartUp");
      	return FX_OK;
      }
    }
  }
  else
  {
    if(!(iTime % BoundBy(5-GetYDir(pTarget), 5, 0)) && GetYDir(pTarget) > 0)
    {
      //beim Flug mehr Schub
      if(GetAction()=="Fly" || GetAction()=="Turn")
      throttle = BoundBy(throttle + BKHK_AutoThrottleSpeed, 0, BKHK_AutoMaxThrottle);
    }
    else
    if(!(iTime % BoundBy(GetYDir(pTarget), 5, 0)) && GetYDir(pTarget) < 0)
    {
      //vom Gas weg
      if(GetAction()=="Fly" || GetAction()=="Turn")
      throttle = BoundBy(throttle - BKHK_AutoThrottleSpeed, 0, BKHK_AutoMaxThrottle);
    }
  }
  if(GetX(pTarget) > iX+50)
  {
    if (GetAction()=="Fly" || GetAction()=="Turn")
    rotation = BoundBy(rotation - BKHK_ControlSpeed, -BKHK_AutoMaxRotation, BKHK_AutoMaxRotation);
    if (rotation < 0 && GetDir() && GetAction()=="Fly")
    {
      if (GetAction() == "Turn" || GetContact(this, -1)) return true;
      SetAction("Turn");
    }
  }
  else
  if(GetX(pTarget) < iX-50)
  {
    if (GetAction()=="Fly" || GetAction()=="Turn") 
    rotation = BoundBy(rotation + BKHK_ControlSpeed, -BKHK_AutoMaxRotation, BKHK_AutoMaxRotation);
    if (rotation > 0 && !GetDir() && GetAction()=="Fly")
    {
      if(GetAction() == "Turn" || GetContact(this, -1)) return true;
      SetAction("Turn");
    }
  }
  else
  if(GetXDir() != 0)
  {
    if(GetXDir(pTarget) < -1)
    {
      rotation = BoundBy(rotation + BKHK_ControlSpeed, 0, BKHK_AutoMaxRotation);
    }
    else if(GetXDir(pTarget) > 1)
    {
      rotation = BoundBy(rotation - BKHK_ControlSpeed, -BKHK_AutoMaxRotation, 0);
    }
    else
    {
      if(GetXDir(pTarget) < 0)
      {
        rotation = BoundBy(rotation - BKHK_ControlSpeed, 0, BKHK_AutoMaxRotation);
      }
      else if(GetXDir(pTarget) > 0)
      {
        rotation = BoundBy(rotation + BKHK_ControlSpeed, -BKHK_AutoMaxRotation, 0);
      }
    }
  }
  return FX_OK;
}

public func FxAIRocketShootTimer()
{
	//Log("Controller %d, Controller %d", GetController(), GetController(pRocketStation));
	pRocketStation->~ControlThrow(this);
	return true;
}

public func MGStartBlackhawk(int iDir, int iThrottle, int iRotation)
{
	EngineStarted(); // Blackhawk startet
	throttle = iThrottle;
	rotation = iRotation;
	SetAction("Fly");
	SetDir(iDir);
	return true;
}

//für Warnsounds und Grafik zuständig
/*protected func TimerCall()
{
  //Zerstört?
  if(IsDestroyed())
    return;	

  //Absinken, wenn kein Pilot
  if(((!GetPilot() && !GetAutopilot() || GetY() < 100) && throttle) || (GetAction() == "Fly" && throttle && !GetPilot() && !GetAutopilot()))
    if(!Minigame())
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
    if(GetPilot() && !Hostile(GetOwner(GetPilot()), iPlr) && (!GetCursor(iPlr) || (Contained(GetCursor(iPlr)) != this) && Contained(GetCursor(iPlr)->~GetRealCursor()) != this))
    {
      if (first)
        first = false;
      else
        iFlags = MSG_Multiple;
      var szStr = Format("@%s (%s)", GetName(GetPilot()), GetPlayerName(GetOwner(GetPilot())));
      CustomMessage(szStr, this, iPlr, 0, 15, SetRGBaValue(GetPlrColorDw(GetOwner(GetPilot())), 128), 0, 0, iFlags);
    }
    else
      CustomMessage("@", this, iPlr);
  }

  //Scheinwerfer aktualisieren
  if(fShowSpotlight)
  {
    if(pSpotlight[0])
    {
      UpdateSpotlights();
    }
    else
    {
      CreateSpotlights();
    }
  }
  else
  {
    if(pSpotlight[0])
    {
      RemoveSpotlights();
    }
  }

  //Alle 50 Frames nach Leichen im Heli suchen und löschen.
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

  //Bei Wasser abschalten
  Water();

  //Piloten anpassen
  DrawPilot();

  //Bodenpartikel zeichnen
  DrawGroundParticles();

  //Lebewesen schrappneln
  if(GetRotorSpeed() > 0)
  {
    for(var pClonk in FindObjects(Find_OnLine(GetVertex(7), GetVertex(7, true), GetVertex(11), GetVertex(11, true)) , Find_NoContainer(), Find_OCF(OCF_Alive), Find_Not(Find_ID(FKDT))))
    {
      if(GetOwner(pClonk) != NO_OWNER && GetOwner() != NO_OWNER && !Hostile(GetOwner(), GetOwner(pClonk)))
        continue;
      if(GetEffect("NoRotorHit", pClonk))
        continue;
      Fling(pClonk, GetXDir(pClonk, 1) * 3 / 2 + RandomX(-1, 1), RandomX(-3, -2) - GetRotorSpeed() / 100);
      DoDmg(GetRotorSpeed() / 4, DMG_Projectile, pClonk, 0, GetOwner() + 1);
      Sound("BKHK_RotorHit*.ogg", false, pClonk);
      AddEffect("NoRotorHit", pClonk, 1, 20, pClonk);
    }

    //Festes Material im Rotor tut weh, wird von Contact-Calls bei Stillstand nicht erfasst
    if (!PathFree(GetX() + GetVertex(7), GetY() + GetVertex(7, true), GetX() + GetVertex(11), GetY() + GetVertex(11, true)))
      DoDamage(1, this, FX_Call_DmgScript, GetController(GetPilot()) + 1);
  }
  
  //Nachladezeit reduzieren
  if(smokereload) 
    smokereload--;
  if(flarereload)
    flarereload--;

  //Schadensverhalten
  //bis 50% nichts
  if (GetDamage() < MaxDamage() / 2)
    return;

  //ab 50% rauchen
  Smoking();

  if (GetDamage() < MaxDamage() * 3 / 4)
    return;

  //ab 75% Feuer
  DrawFire();

  if (!GetEffect("Engine", this))
    return;
  
  //Warnsound
  WarningSound();
}*/

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
  if(((!GetPilot() && !GetAutopilot() || GetY() < 100) && throttle) || (GetAction() == "Fly" && throttle && !GetPilot() && !GetAutopilot()))
 		if(!Minigame())
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
    for(var pClonk in FindObjects(Find_OnLine(GetVertex(7), GetVertex(7, true), GetVertex(11), GetVertex(11, true)) , Find_NoContainer(), Find_OCF(OCF_Alive), Find_Not(Find_ID(FKDT))))
    {
      if(GetOwner(pClonk) != NO_OWNER && GetOwner() != NO_OWNER && !Hostile(GetOwner(), GetOwner(pClonk)))
        continue;
      if(GetEffect("NoRotorHit", pClonk))
        continue;
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

private func ExitClonk(a, ByObj)
{
  SetCommand(ByObj, "Exit");
	AddEffect("WaitClonk", ByObj, 101, 1);
}

/*----- Sitzsteuerung -----*/

protected func ContainedDigDouble(object ByObj)
{
  [$CtrlDigD$]
  CreateMenu(GetID(), ByObj, this, 0, "$Seats$", 0, 1);

    //Ausstieg
    AddMenuItem("<c ffff33>$Exit$</c>", "ExitClonk", STDR, ByObj, 0, ByObj, "$ExitDesc$");

		if(BKHK_LockSeats)	return true;
    //Pilot
    if(GetPilot())
      AddMenuItem("<c 777777>$Pilot$</c>", "SeatOccupied", GetID(), ByObj, 0, ByObj, "$SeatOccupied$");
    else
      AddMenuItem("<c ffffff>$Pilot$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Pilot, ObjectNumber(ByObj)), GetID(), ByObj, 0, ByObj, "$PilotDesc$");

    //Schütze
    if(GetGunner())
      AddMenuItem("<c 777777>$Gunner$</c>", "SeatOccupied", GetID(), ByObj, 0, ByObj, "$SeatOccupied$");
    else
      AddMenuItem("<c ffffff>$Gunner$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Gunner, ObjectNumber(ByObj)), GetID(), ByObj, 0, ByObj, "$GunnerDesc$");

    //Koordinator
    if(GetCoordinator())
      AddMenuItem("<c 777777>$Coordinator$</c>", "SeatOccupied", GetID(), ByObj, 0, ByObj, "$SeatOccupied$");
    else
      AddMenuItem("<c ffffff>$Coordinator$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Coordinator, ObjectNumber(ByObj)), GetID(), ByObj, 0, ByObj, "$CoordinatorDesc$");

    //Passagier 1
    if(GetPassenger1())
      AddMenuItem("<c 777777>$Passenger$</c>", "SeatOccupied", GetID(), ByObj, 0, ByObj, "$SeatOccupied$");
    else
      AddMenuItem("<c ffffff>$Passenger$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Passenger1, ObjectNumber(ByObj)), GetID(), ByObj, 0, ByObj, "$PassengerDesc$");

    //Passagier 2
    if(GetPassenger2())
      AddMenuItem("<c 777777>$Passenger$</c>", "SeatOccupied", GetID(), ByObj, 0, ByObj, "$SeatOccupied$");
    else
      AddMenuItem("<c ffffff>$Passenger$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Passenger2, ObjectNumber(ByObj)), GetID(), ByObj, 0, ByObj, "$PassengerDesc$");
    
    //Ausstieg per Seil
    AddMenuItem("<c ffff33>$ExitRope$</c>", "ExitClonkByRope", CK5P, ByObj, 0, ByObj, "$ExitRopeDesc$");

  return 1;
}

protected func Collection2(object pObj)
{
  if(GetOCF(pObj) & OCF_Alive && GetOCF(pObj) & OCF_CrewMember)
  {
    //Freund: Rein. Feind: Raus
    if(!Hostile(GetOwner(this),GetOwner(pObj)) || (GetOwner() == -1) || !GetPassengerCount() || Minigame())
    {
      //Soundschleife übergeben
      Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(pObj)+1, +1);
      
      if(Minigame())
      	return EnterSeat(BKHK_Seat_Passenger1, pObj);
      	
      //Freien Sitz belegen
      if(!GetPilot())
        return EnterSeat(BKHK_Seat_Pilot, pObj);
      if(!GetGunner())
        return EnterSeat(BKHK_Seat_Gunner, pObj);
      if(!GetCoordinator())
        return EnterSeat(BKHK_Seat_Coordinator, pObj);        
      if(!GetPassenger1())
        return EnterSeat(BKHK_Seat_Passenger1, pObj);        
      if(!GetPassenger2())
        return EnterSeat(BKHK_Seat_Passenger2, pObj);

      //Kein Platz mehr. :/
      return SetCommand(pObj, "Exit");
    }
    else
      return SetCommand(pObj, "Exit");
  }
}

//hier wird das Flugverhalten gesteuert
protected func FxEngineTimer(object Target, int EffectNumber, int EffectTime)
{
  //Variablen abspeichern
  var rot = LocalN("rotation", Target);
  var thr = LocalN("throttle", Target);
  var Fg, Fv, Fh, Fw, Fs, dFv, dFh, m, mh, g, av, ah;  //physikalische Kräfte
  
  //Überprüfung, ob überhaupt noch ein Pilot drin...
  if (Target->GetPilot() || Target->GetAutopilot() || Minigame())
  {
	  //Rotation anpassen
    var speed;
    speed = BoundBy(rot-GetR(Target), -iRotorSpeed, iRotorSpeed);
    SetRDir(speed, Target);
  }
  else
  {
    LocalN("rotation", Target) = GetR(Target);
  }

  //Gewichtskraft berechnen
  m = GetMass(Target);
  if(Minigame())
  	m = 1500;
  
  g = GetGravity();
  Fg = (m + mh) * g;
  Fw = GetWind(0, -20, false) * 200;
 	if(GetContact(this, -1, CNAT_Bottom)) Fw = 0;
 
  //Hubkraft und vertikale Beschleunigung berechnen
  Fv  = - Cos(GetR(Target), 1500*thr);
  dFv = Fg + Fv;
  av  = dFv / m; //BoundBy(dFh / m, -80, 80);

  //Kraft nach links oder rechts sowie deren Beschleunigung berechnen
  Fh  = - Sin(-GetR(Target), 1500*thr);
  dFh = Fh + Fw + Fs;
  ah  = dFh / m; //BoundBy(dFf / m, -80, 80);

  //Geschwindigkeit anpassen
  SetYDir(GetYDir(Target, 95) + av, Target, 100);
  SetXDir(GetXDir(Target, 95) + ah, Target, 100);

  return true;
}
